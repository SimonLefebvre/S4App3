/******************************************************************************/
/*MAIN                                                                        */
/*Auteur: Simon Lefebvre                                                      */
/*Créer le 7 juin 2021                                                        */
/*                                                                            */
/******************************************************************************/


#include "S4App3.h"
#include "heure.h"
#include <stdbool.h>
/*      VARRIABLES GLOBALES     */

bool flag_1s = false;

/*      VARRIABLES GLOBALES     */

extern int sqrt_babylon(int value, unsigned int precision);
extern unsigned int magnitude(int ax, int ay, int az);

unsigned char rgRawVals[6] = {0, 0, 0, 0, 0, 0};    
int16_t *valACC_XYZ = NULL;

void main(void)
{
    Timer1Init();
    macro_enable_interrupts();
    LCD_Init();
    LCD_WriteStringAtPos("S4App3 ", 0, 0); 
    BTN_Init();
    ACL_Init();   
    ACL_SetRange(1);
    
    // Main loop
    while(1) {
        flag_1s = heure();
        
        if(flag_1s)
        {
            flag_1s = false;
            // Code à effectuer chaque seconde ici
        }
    }
}