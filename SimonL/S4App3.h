/******************************************************************************/
/*MAIN HEADER FILE                                                            */
/*Auteur: Simon Lefebvre                                                      */
/*Créer le 7 juin 2021                                                        */
/*                                                                            */
/******************************************************************************/

#ifndef S4APP3_H
#define S4APP3_H

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>
#include "config.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "btn.h"
#include "acl.h"
#include "i2c.h"
#include "uart.h"
#include "spiflash.h"
#include "pmods.h"
#include "accelerometre.h"
#include "heure.h"
#include "calculCaptures.h"


void Timer1Init(void);
void testFlash(void);
uint32_t SendToFlash(uint32_t secondes,int16_t Ax,int16_t Ay,int16_t Az,uint16_t sqrt,uint16_t ADC, uint32_t Address);
void ReadFlash(uint32_t* secondes,int16_t* Ax,int16_t* Ay,int16_t* Az,uint16_t* sqrt,uint16_t* ADC, uint32_t Address, uint16_t Lenght);
void SendUartData(int16_t* Ax, int16_t* Ay, int16_t* Az, uint16_t* sqrt, uint16_t* ADC);
void printAllData(int16_t* Ax,int16_t* Ay,int16_t* Az,uint16_t* sqrt,uint16_t* ADC);

void SendToServer(uint32_t* secondes, int16_t* AxCal, int16_t* AyCal, int16_t* AzCal, uint16_t* sqrtCal, uint16_t* ADCCal, 
        int16_t* Ax, int16_t* Ay, int16_t* Az, uint16_t* sqrt, uint16_t* ADC);
#endif /* S4APP3_H */