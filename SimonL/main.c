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
    UART_PutString("\n\r");
    /****/
    ACL_Init();
    unsigned char rgRawVals[6] = {0, 0, 0, 0, 0, 0};    
    signed short valX = 0;
    signed short valY = 0;
    signed short valZ = 0;
    
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
                
                
                ACL_ReadRawValues(rgRawVals);
                valX = (((signed short)rgRawVals[0]) << 4) + (rgRawVals[1] >> 4);
                valY = (((signed short)rgRawVals[2]) << 4) + (rgRawVals[3] >> 4);
                valZ = (((signed short)rgRawVals[4]) << 4) + (rgRawVals[5] >> 4);           
                
                if(valX & 1<<11)
                {
                    valX |= (1<<15);
                    valX &= ~(1<<11);
                }
                
                if(valY & 1<<11)
                {
                    valY |= (1<<15);
                    valY &= ~(1<<11);
                }
                
                if(valZ & 1<<11)
                {
                    valZ |= (1<<15);
                    valZ &= ~(1<<11);
                }
                UART_PutString("\rX:");
                char StringACL1[6] = {(valX/10000)+48,(valX%10000/1000)+48,(valX%1000/100)+48,(valX%100/10)+48,(valX%10)+48,'\0'};
                UART_PutString(StringACL1);
                UART_PutString(" Y: ");
                char StringACL2[6] = {(valY/10000)+48,(valY%10000/1000)+48,(valY%1000/100)+48,(valY%100/10)+48,(valY%10)+48,'\0'};
                UART_PutString(StringACL2);
                UART_PutString(" Z: ");
                char StringACL3[6] = {(valZ/10000)+48,(valZ%10000/1000)+48,(valZ%1000/100)+48,(valZ%100/10)+48,(valZ%10)+48,'\0'};
                UART_PutString(StringACL3);
            }
        }
    }
}