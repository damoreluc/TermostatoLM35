#include <Arduino.h>
#include <data.h>
#include <tasks.h>

// // assegnazione dei pin dei led
// #define pinTmin 12
// #define pinTmax 14
// #define pinLM35 36

// // costanti di sistema
// const float isteresi = 1.0;
// const float Tmax = 30.0;

// // mutex per gestire l'accesso alla variabile globale soglia
// static SemaphoreHandle_t mutexSoglia;

// // dichiarazione delle function dei due task

// // task1 legge il valore di soglia dalla seriale
// void task1(void *parameters);

// // task2 acquisisce la temperatura dal LM35
// // ed effettua tutte le elaborazioni
// void task2(void *parameters);

// // funzione ausiliaria per la lettura del trasduttore-
// float leggiLM35(uint8_t channel);
// // variabile di temperatura soglia scambiata tra i due task
// float soglia = 25.0;

// configurazione del sistema
void setup()
{
  // attivazione della seriale
  Serial.begin(115200);
  // aumento il tempo massimo di attesa dei caratteri dal terminale
  Serial.setTimeout(2000);  
  Serial.println("Termostato con LM35");

  // configurazione delle uscite
  pinMode(pinTmin, OUTPUT);
  pinMode(pinTmax, OUTPUT);

  digitalWrite(pinTmin, LOW);
  digitalWrite(pinTmax, LOW);

  // creazione del mutex prima di avviare i task
  creaMutex();
  //mutexSoglia = xSemaphoreCreateMutex();

  // creazione dei due task
  xTaskCreatePinnedToCore(
      task1,      // funzione da richiamare nel task
      "Task1",    // nome del task (etichetta utile per debug)
      2048,       // dimensione in byte dello stack per le variabili locali del task (minimo 768 byte)
      NULL,       // puntatore agli eventuali parametri da passare al task
      1,          // priorità del task
      NULL,       // eventuale task handle per gestire il task da un altro task
      APP_CPU_NUM // core su cui far girare il task
  );

  xTaskCreatePinnedToCore(
      task2,      // funzione da richiamare nel task
      "Task2",    // nome del task (etichetta utile per debug)
      2048,       // dimensione in byte dello stack per le variabili locali del task (minimo 768 byte)
      NULL,       // puntatore agli eventuali parametri da passare al task
      2,          // priorità del task
      NULL,       // eventuale task handle per gestire il task da un altro task
      APP_CPU_NUM // core su cui far girare il task
  );

  Serial.println("Task avviati");
}

void loop()
{
  // qui non faccio nulla, uso task1 e task2
}

// // ---- Task1 -----------------------------------------------------------------
// // task1 legge il valore di soglia dalla seriale
// void task1(void *parameters)
// {
//   // uso t come variabile locale di appoggio
//   float t;
//   char c;

//   // loop del task
//   while (1)
//   {
//     // se c'è qualcosa nella seriale...
//     if (Serial.available() > 0)
//     {
//       t = Serial.parseFloat();

//       // rimuove \n e \r dal buffer di ricezione
//       // 1. guarda il prossimo carattere nel buffer...
//       c = Serial.peek();
//       // 2. ripeti mentre si tratta di \n o \r
//       while (c == '\n' || c == '\r')
//       {
//         // 3. rimuovilo dal buffer e..
//         c = Serial.read();
//         // 4. guarda il prossimo carattere nel buffer...
//         c = Serial.peek();
//       }

//       // acquisizione del mutex prima di aggiornare la variabile soglia
//       if (xSemaphoreTake(mutexSoglia, 0) == pdTRUE)
//       {
//         // copio il valore letto nella variabile globale soglia
//         soglia = t;

//         // fine sezione critica, restituisco il mutex
//         xSemaphoreGive(mutexSoglia);
//       }
//     }
//   }
// }

// // ---- Task2 -----------------------------------------------------------------
// // task2 acquisisce la temperatura dal LM35
// // ed effettua tutte le elaborazioni
// void task2(void *parameters)
// {
//   // variabili locali
//   float temperatura;
//   float miasoglia;

//   // loop del task
//   while (1)
//   {
//     // logica del termostato...

//     // 1. lettura della temperatura dal LM35
//     temperatura = leggiLM35(pinLM35);

//     // acquisizione del mutex per leggere la variabile soglia
//     if (xSemaphoreTake(mutexSoglia, 0) == pdTRUE)
//     {
//       miasoglia = soglia;
//       xSemaphoreGive(mutexSoglia);
//     }

//     // 2. confronto soglia e comando pinTmin
//     if (temperatura < miasoglia)
//     {
//       digitalWrite(pinTmin, HIGH);
//     }
//     else if (temperatura > miasoglia + isteresi)
//     {
//       digitalWrite(pinTmin, LOW);
//     }

//     // 3. confronto 30°C e comando pinTmax
//     if (temperatura > Tmax)
//     {
//       digitalWrite(pinTmax, HIGH);
//     }
//     else if (temperatura < Tmax - isteresi)
//     {
//       digitalWrite(pinTmax, LOW);
//     }

//     // 4. stampa temperatura
//     Serial.printf("%.2f\n", miasoglia);

//     // 5. attesa 1 secondo
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }

// // funzione ausiliaria per la lettura del trasduttore
// float leggiLM35(uint8_t channel)
// {
//   int n = analogRead(channel);

//   float v = (float)n / 4096 * 3.3;
//   float t = v / 0.01;
//   return t;
// }