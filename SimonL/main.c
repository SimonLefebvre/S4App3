/******************************************************************************/
/*MAIN                                                                        */
/*Auteur: Simon Lefebvre                                                      */
/*Créer le 7 juin 2021                                                        */
/*                                                                            */
/******************************************************************************/


#include "S4App3.h"

/*      VARRIABLES GLOBALES     */
int Flag_1s = 0;



/*      VARRIABLES GLOBALES     */




void main(void)
{
    macro_enable_interrupts();
    UART_Init(9600);
    UART_PutString("Hello Wolrd\n\r");
    
    Timer1Init();
    /****/
    LCD_Init();
    LCD_WriteStringAtPos("S4App3 ", 0, 0); 
    char heures[2] = {0};
    char minutes[2] = {0};
    char secondes[2] = {0};
    int count = 0;
    /*****/
    ADC_Init();
    TRISBbits.TRISB2 = 1;
    ANSELBbits.ANSB2 = 1;
    unsigned int ADCValue = 0;
    /*****/
    SPIFLASH_Init();
    SPIFLASH_WriteEnable();
    SPIFLASH_EraseAll();
    unsigned char data[] = "Bien le bon soir mon ami";
    SPIFLASH_ProgramPage(0, data, sizeof(data));
    unsigned char DataRead[256] = {0};
    SPIFLASH_Read(0,DataRead,sizeof(data));
    UART_PutString(DataRead);
    
    
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
                ADCValue = ADC_AnalogRead(2);
                char stringToSend[15] = {heures[1]+48,heures[0]+48,':',minutes[1]+48,minutes[0]+48,':',secondes[1]+48,secondes[0]+48,
                'A','N',(ADCValue/1000)+48,(ADCValue%1000/100)+48,(ADCValue%100/10)+48,(ADCValue%10)+48,'\0'};
                LCD_WriteStringAtPos(stringToSend, 1, 0);
                
            }
        }
    }
}