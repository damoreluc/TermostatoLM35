#include <tasks.h>
#include <data.h>

// mutex per gestire l'accesso alla variabile globale soglia
SemaphoreHandle_t mutexSoglia;

// funzione ausiliaria per la creazione dei mutex
void creaMutex()
{
    mutexSoglia = xSemaphoreCreateMutex();
}

// ---- Task1 -----------------------------------------------------------------
// task1 legge il valore di soglia dalla seriale
void task1(void *parameters)
{
    // uso t come variabile locale di appoggio
    float t;
    char c;

    // loop del task
    while (1)
    {
        // se c'è qualcosa nella seriale...
        if (Serial.available() > 0)
        {
            t = Serial.parseFloat();

            // rimuove \n e \r dal buffer di ricezione
            // 1. guarda il prossimo carattere nel buffer...
            c = Serial.peek();
            // 2. ripeti mentre si tratta di \n o \r
            while (c == '\n' || c == '\r')
            {
                // 3. rimuovilo dal buffer e..
                c = Serial.read();
                // 4. guarda il prossimo carattere nel buffer...
                c = Serial.peek();
            }

            // acquisizione del mutex prima di aggiornare la variabile soglia
            if (xSemaphoreTake(mutexSoglia, 0) == pdTRUE)
            {
                // copio il valore letto nella variabile globale soglia
                soglia = t;

                // fine sezione critica, restituisco il mutex
                xSemaphoreGive(mutexSoglia);
            }
        }
    }
}

// ---- Task2 -----------------------------------------------------------------
// task2 acquisisce la temperatura dal LM35
// ed effettua tutte le elaborazioni
void task2(void *parameters)
{
    // variabili locali
    float temperatura;
    float miasoglia;

    // loop del task
    while (1)
    {
        // logica del termostato...

        // 1. lettura della temperatura dal LM35
        temperatura = leggiLM35(pinLM35);

        // acquisizione del mutex per leggere la variabile soglia
        if (xSemaphoreTake(mutexSoglia, 0) == pdTRUE)
        {
            miasoglia = soglia;
            xSemaphoreGive(mutexSoglia);
        }

        // 2. confronto soglia e comando pinTmin
        if (temperatura < miasoglia)
        {
            digitalWrite(pinTmin, HIGH);
        }
        else if (temperatura > miasoglia + isteresi)
        {
            digitalWrite(pinTmin, LOW);
        }

        // 3. confronto 30°C e comando pinTmax
        if (temperatura > Tmax)
        {
            digitalWrite(pinTmax, HIGH);
        }
        else if (temperatura < Tmax - isteresi)
        {
            digitalWrite(pinTmax, LOW);
        }

        // 4. stampa temperatura
        Serial.printf("%.2f\n", miasoglia);

        // 5. attesa 1 secondo
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// ---- Funzioni ausiliarie ---------------------------------------------------
// funzione ausiliaria per la lettura del trasduttore
float leggiLM35(uint8_t channel)
{
    int n = analogRead(channel);

    float v = (float)n / 4096 * 3.3;
    float t = v / 0.01;
    return t;
}