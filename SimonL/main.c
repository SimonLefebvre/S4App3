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
    Timer1Init();
    macro_enable_interrupts();
    LCD_Init();
    LCD_WriteStringAtPos("S4App3 ", 0, 0); 
    char heures[2] = {0};
    char minutes[2] = {0};
    char secondes[2] = {0};
    int count = 0;
    // Main loop
    while(1) {
        if(Flag_1s)// Flag d'interruption à chaque 1 ms
        {
            Flag_1s = 0;
            if (++count >= 1000) 
            {
                count = 0;
                secondes[0] ++;
                if(secondes[0] > 9){secondes[0] = 0; secondes[1] ++;}
                if(secondes[1] > 5){secondes[1] = 0; minutes[0] ++;}
                if(minutes[0] > 9){minutes[0] = 0; minutes[1] ++;}
                if(minutes[1] > 5){minutes[1] = 0; heures[0] ++;}
                if(heures[0] > 9){heures[0] = 0; heures[1] ++;}
                if(heures[0] > 3 && heures[1] > 1){heures[0] = 0; heures[1] =0;}
                char stringToSend[9] = {heures[1]+48,heures[0]+48,':',minutes[1]+48,minutes[0]+48,':',secondes[1]+48,secondes[0]+48,'\0'};
                LCD_WriteStringAtPos(stringToSend, 1, 0);
            }
        }
    }
}