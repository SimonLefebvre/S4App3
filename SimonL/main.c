/******************************************************************************/
/*MAIN                                                                        */
/*Auteur: Simon Lefebvre                                                      */
/*Créer le 7 juin 2021                                                        */
/*                                                                            */
/******************************************************************************/


#include "S4App3.h"


int Flag_1s = 0;


void __ISR(_TIMER_1_VECTOR, IPL2AUTO) Timer1ISR(void) 
{  
   Flag_1s = 1;           //    Indique à la boucle principale qu'on doit traiter
  
   IFS0bits.T1IF = 0;     //    clear interrupt flag
}


void main(void)
{
    LED_Init();
    Timer1Init();
    int count = 0;
    int LEDValue = 0;
    
    macro_enable_interrupts();
    
    // Main loop
    while(1) {
        if(Flag_1s)                 // Flag d'interruption à chaque 1 ms
        {
            Flag_1s = 0;            // Reset the flag to capture the next event
            if (++count >= 1000) 
            {
                count = 0;
                LEDValue ^= 0x0f;
                LED_SetGroupValue(LEDValue);
                //LED_ToggleValue(4); // Toggle the led at 2 Hz
            }
        }
    }
  
    while(1)
    { 
        
    }  
}