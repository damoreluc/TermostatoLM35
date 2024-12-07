#ifndef _TASKS_H
#define _TASKS_H

#include <Arduino.h>

// funzione ausiliaria per la creazione dei mutex
void creaMutex();

// dichiarazione delle function dei due task

// task1 legge il valore di soglia dalla seriale
void task1(void *parameters);

// task2 acquisisce la temperatura dal LM35
// ed effettua tutte le elaborazioni
void task2(void *parameters);

// funzione ausiliaria per la lettura del trasduttore
float leggiLM35(uint8_t channel);

#endif