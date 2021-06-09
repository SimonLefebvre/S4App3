/******************************************************************************/
/*MAIN                                                                        */
/*Auteur: Simon Lefebvre                                                      */
/*Créer le 7 juin 2021                                                        */
/*                                                                            */
/******************************************************************************/


#include "S4App3.h"
#include "heure.h"
/*      VARRIABLES GLOBALES     */



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
    
    
    // Main loop
    while(1) {
        heure();
    }
}