#include <Arduino.h>
#include <data.h>
#include <tasks.h>

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
