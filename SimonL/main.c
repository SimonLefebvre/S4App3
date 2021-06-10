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
extern void parallelPortSend(unsigned char value);


static void initPmodGpios()
{
    PMODS_InitPin(0, 2, 0, 0, 0); // JA2 - PORTC1
    PMODS_InitPin(0, 1, 0, 0, 0); // JA1 - PORTC2
    PMODS_InitPin(0, 7, 0, 0, 0); // JA7 - PORTC3
    PMODS_InitPin(0, 3, 0, 0, 0); // JA3 - PORTC4
    PMODS_InitPin(0, 4, 0, 0, 0); // JA4 - PORTG6
    PMODS_InitPin(0, 8, 0, 0, 0); // JA8 - PORTG7
    PMODS_InitPin(0, 9, 1, 0, 1); // JA9 - PORTG8
}

void main(void)
{
    Timer1Init();
    macro_enable_interrupts();
    LCD_Init();
    LCD_WriteStringAtPos("S4App3 ", 0, 0); 
    BTN_Init();
    ACL_Init();   
    SPIFLASH_Init();
    UART_Init(115200);
    ACL_SetRange(1);
    initPmodGpios();
    ADC_Init();
    SWT_Init();
    SPIFLASH_Erase4k(0);
    UART_PutString("\n\rS4APP3");
    UART_PutString("\n\rSimon Lefebvre : lefs3303");
    UART_PutString("\n\rXavier Dufour  : dufx2901");
    UART_PutString("\n\rJeremy Chenard : chej2806");
    
    // Main loop
    while(1) 
    {
        flag_1s = heure();
        
        if(flag_1s)
        {
            
            flag_1s = false;
            testFlash();
            // Code à effectuer chaque seconde ici
        }
    }
}