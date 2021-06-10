/******************************************************************************/
/*Flash functions                                                                       */
/*Auteur: Simon Lefebvre                                                      */
/*Créer le 7 juin 2021                                                        */
/*                                                                            */
/******************************************************************************/

#include "S4App3.h"

extern unsigned int magnitude(int ax, int ay, int az);

static uint32_t secondes[16];
static int16_t Ax[16];
static int16_t Ay[16];
static int16_t Az[16];
static uint16_t sqrt[16];
static uint16_t ADC[16];


void testFlash(void)
{
    static uint32_t magnitude_val = 0;
    static uint32_t SecondesCpt = 0;
    static uint32_t FlashAddressPointer = 0;
    static uint16_t ADCValue = 0;
    static unsigned char rgRawVals[6] = {0, 0, 0, 0, 0, 0};    
    static int16_t* valACC_XYZ = NULL;
    static int16_t donneesAx[3] = {0};
    static int16_t donneesAy[3] = {0};
    static int16_t donneesAz[3] = {0};
    static uint16_t donneesSqrt[3] = {0};
    static uint16_t donneesADC[3] = {0};
    
    SecondesCpt ++;
    ADCValue = ADC_AnalogRead(2);
    ACL_ReadRawValues(rgRawVals);
    valACC_XYZ = Acc_val_16bits(rgRawVals);
    magnitude_val = magnitude(valACC_XYZ[0], valACC_XYZ[1], valACC_XYZ[2]);
    
    FlashAddressPointer = SendToFlash(SecondesCpt, valACC_XYZ[0], valACC_XYZ[1], valACC_XYZ[2], magnitude_val, ADCValue, FlashAddressPointer);
    
    if(SecondesCpt > 15)
    {
        SecondesCpt = 0;
        
        ReadFlash(secondes, Ax, Ay, Az, sqrt, ADC, 0, 250); 
        
        SPIFLASH_Erase4k(0);//For the next write to be successfull
        FlashAddressPointer = 0;
        
        captureCalculSigned(Ax,donneesAx); 
        captureCalculSigned(Ay,donneesAy); 
        captureCalculSigned(Az,donneesAz); 
        captureCalculSigned(sqrt,donneesSqrt);
        captureCalculSigned(ADC,donneesADC);
        SendUartData(donneesAx,donneesAy,donneesAz,donneesSqrt,donneesADC);
    }
    
    
}

uint32_t SendToFlash(uint32_t secondes,int16_t Ax,int16_t Ay,int16_t Az,uint16_t sqrt,uint16_t ADC, uint32_t Address)
{
    unsigned char data[] = {secondes >>24, (secondes >> 16) & 0xFF, (secondes >> 8) & 0xFF, secondes & 0xFF, (Ax>>8) & 0xFF, Ax & 0xFF,
    (Ay>>8) & 0xFF, Ay & 0xFF, (Az>>8) & 0xFF, Az & 0xFF, (sqrt>>8) & 0xFF, sqrt & 0xFF, (ADC>>8) & 0xFF, ADC & 0xFF};
    SPIFLASH_ProgramPage(Address, data, sizeof(data));
    return Address + sizeof(data);
}

void ReadFlash(uint32_t* secondes,int16_t* Ax,int16_t* Ay,int16_t* Az,uint16_t* sqrt,uint16_t* ADC, uint32_t Address, uint16_t Lenght)
{
    unsigned char DataRead[256] = {0};
    SPIFLASH_Read(Address, DataRead, Lenght);
    int i = 0;
    for(;i<15;i++)
    {
        secondes[i] = DataRead[0 + (i*14)]<<24 | DataRead[1 + (i*14)]<<16 | DataRead[2 + (i*14)]<<8 | DataRead[3 + (i*14)];
        Ax[i] = DataRead[4 + (i*14)]<<8 | DataRead[5 + (i*14)]; 
        Ay[i] = DataRead[6 + (i*14)]<<8 | DataRead[7 + (i*14)]; 
        Az[i] = DataRead[8 + (i*14)]<<8 | DataRead[9 + (i*14)]; 
        sqrt[i] = DataRead[10 + (i*14)]<<8 | DataRead[11 + (i*14)]; 
        ADC[i] = DataRead[12 + (i*14)]<<8 | DataRead[13 + (i*14)]; 
    }
}



void SendUartData(int16_t* Ax, int16_t* Ay, int16_t* Az, uint16_t* sqrt, uint16_t* ADC)
{
    UART_PutString("\n\n\r-------------------------------------------");
    UART_PutString("\n\n\rNEW VALUES");
    UART_PutString("\n\n\rTemps d'envoie: ");
    char buffer [50];
    UART_PutString("\n\n\r    Accelerometre");
    
    UART_PutString("\n\rAx min:0x");sprintf(buffer,"%x",Ax[0]);UART_PutString(buffer);
    UART_PutString("\n\rAx max:0x");sprintf(buffer,"%x",Ax[1]);UART_PutString(buffer);
    UART_PutString("\n\rAx moy:0x");sprintf(buffer,"%x",Ax[2]);UART_PutString(buffer);
    
    UART_PutString("\n\n\rAy min:0x");sprintf(buffer,"%x",Ay[0]);UART_PutString(buffer);
    UART_PutString("\n\rAy max:0x");sprintf(buffer,"%x",Ay[1]);UART_PutString(buffer);
    UART_PutString("\n\rAy moy:0x");sprintf(buffer,"%x",Ay[2]);UART_PutString(buffer);
    
    UART_PutString("\n\n\rAz min:0x");sprintf(buffer,"%x",Az[0]);UART_PutString(buffer);
    UART_PutString("\n\rAz max:0x");sprintf(buffer,"%x",Az[1]);UART_PutString(buffer);
    UART_PutString("\n\rAz moy:0x");sprintf(buffer,"%x",Az[2]);UART_PutString(buffer);
    
    UART_PutString("\n\n\r    Magnetude");
    UART_PutString("\n\rsqrt min:");sprintf(buffer,"%x",sqrt[0]);UART_PutString(buffer);
    UART_PutString("\n\rsqrt max:");sprintf(buffer,"%x",sqrt[1]);UART_PutString(buffer);
    UART_PutString("\n\rsqrt moy:");sprintf(buffer,"%x",sqrt[2]);UART_PutString(buffer);
    
    
    UART_PutString("\n\n\r    ADC");
    UART_PutString("\n\rADC min:");sprintf(buffer,"%d",ADC[0]);UART_PutString(buffer);
    UART_PutString("\n\rADC max:");sprintf(buffer,"%d",ADC[1]);UART_PutString(buffer);
    UART_PutString("\n\rADC moy:");sprintf(buffer,"%d",ADC[2]);UART_PutString(buffer);
    
}

uint32_t SendToServer(uint16_t secondes, int16_t *Ax, int16_t *Ay, int16_t *Az, uint16_t *sqrt, uint16_t *ADC, uint32_t Address)
{
    
    return 0;
}