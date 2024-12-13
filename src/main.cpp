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

  // creazione dei due task: 
  // 1. lettura dalla tastiera
  xTaskCreatePinnedToCore(
      task1,      // funzione da richiamare nel task
      "Task1",    // nome del task (etichetta utile per debug)
      2048,       // dimensione in byte dello stack per le variabili locali del task (minimo 768 byte)
      NULL,       // puntatore agli eventuali parametri da passare al task
      1,          // priorità del task
      NULL,       // eventuale task handle per gestire il task da un altro task
      APP_CPU_NUM // core su cui far girare il task
  );

  // 2. controllo di temperatura
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
  // // se usassimo un superloop

  // // 1. lettura del sensore
  // uint16_t Nadc = analogRead(pinLM35);
  // float vout = (float)Nadc * 3.3 / 4096;
  // float t = vout / 0.01;  // sensibilità LM35 k = 0.01 V/°C

  // // 2. confronto soglia programmabile
  // if( t < soglia) {
  //   digitalWrite(pinTmin, HIGH);
  // } else if (t > soglia + 1.0) {
  //   digitalWrite(pinTmin, LOW);
  // }

  // // 3. confronto soglia a 30°C
  // if( t > 30.0) {
  //   digitalWrite(pinTmax, HIGH);
  // } else if (t < (30.0 - 1.0)) {
  //   digitalWrite(pinTmax, LOW);
  // }

  // // 4. visualizza la temperatura corrente
  // Serial.println(t);

  // // 5. attesa i 1 secondo
  // delay(1000);

  // // 6. acquisizione del nuovo valore di soglia
  // //    dall'input seriale
  // if(Serial.available()) {
  //   soglia = Serial.parseFloat();

  //   // rimuovere \n e \r
  //   char c = Serial.peek();
  //   while(c == '\n' || c == '\r' || c == ' ') {
  //     Serial.read();
  //     c = Serial.peek();
  //   }
  // }

}
