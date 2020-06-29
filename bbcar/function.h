#ifndef FUNCTION_H
#define FUNCTION_H
#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include <math.h>
extern BBCar car;
extern parallax_ping ping;
extern Serial xbee;
extern float distance;
extern bool pingOnOff;
extern parallax_encoder encoderL;
extern parallax_encoder encoderR;
extern const float pi;


void cali();


#endif