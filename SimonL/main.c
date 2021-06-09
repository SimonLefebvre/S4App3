/******************************************************************************/
/*MAIN                                                                        */
/*Auteur: Simon Lefebvre                                                      */
/*Créer le 7 juin 2021                                                        */
/*                                                                            */
/******************************************************************************/


#include "S4App3.h"

/*      VARRIABLES GLOBALES     */

bool flag_1s = false;

/*      VARRIABLES GLOBALES     */

extern int sqrt_babylon(int value, unsigned int precision);
extern unsigned int magnitude(int ax, int ay, int az);



void main(void)
{
    Timer1Init();
    macro_enable_interrupts();
    LCD_Init();
    LCD_WriteStringAtPos("S4App3 ", 0, 0); 
    BTN_Init();
    ACL_Init();   
    SPIFLASH_Init();
    UART_Init(9600);
    ACL_SetRange(1);
    ADC_Init();
 
    
    
    // Main loop
    while(1) {
        flag_1s = heure();
        
        if(flag_1s)
        {
            flag_1s = false;
            testFlash();
            // Code à effectuer chaque seconde ici
        }
    }
}