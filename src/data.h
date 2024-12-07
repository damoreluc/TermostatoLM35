#ifndef _DATA_H
#define _DATA_H

#include <Arduino.h>

// assegnazione dei pin dei led
#define pinTmin 12
#define pinTmax 14
#define pinLM35 36

// costanti di sistema
const float isteresi = 1.0;
const float Tmax = 30.0;

// variabile di temperatura soglia scambiata tra i due task
extern float soglia;

#endif