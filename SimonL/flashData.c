/******************************************************************************/
/*Flash functions                                                                       */
/*Auteur: Simon Lefebvre                                                      */
/*Créer le 7 juin 2021                                                        */
/*                                                                            */
/******************************************************************************/

#include "S4App3.h"

extern unsigned int magnitude(int ax, int ay, int az);
extern void parallelPortSend(unsigned char value);


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
    static int16_t valACC_XYZ[3] = {0, 0, 0};
    static int16_t donneesAx[3] = {0};
    static int16_t donneesAy[3] = {0};
    static int16_t donneesAz[3] = {0};
    static uint16_t donneesSqrt[3] = {0};
    static uint16_t donneesADC[3] = {0};
    
    char buffer [50];
    SecondesCpt ++;
    ADCValue = ADC_AnalogRead(2);
    ACL_ReadRawValues(rgRawVals);
    Acc_val_16bits(rgRawVals, valACC_XYZ);
    magnitude_val = magnitude(valACC_XYZ[0], valACC_XYZ[1], valACC_XYZ[2]);
    
    
    //sprintf(buffer,"\n\rSending:0x%d",valACC_XYZ[0]);UART_PutString(buffer);//For testing
    FlashAddressPointer = SendToFlash(SecondesCpt, valACC_XYZ[0], valACC_XYZ[1], valACC_XYZ[2], magnitude_val, ADCValue, FlashAddressPointer);//write
    //ReadFlash(secondes, Ax, Ay, Az, sqrt, ADC, 0, FlashAddressPointer); //For testing
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
        printAllData(Ax, Ay, Az, sqrt, ADC);
        
        
        SendToServer(secondes, donneesAx, donneesAy, donneesAz, donneesSqrt, donneesADC, Ax, Ay, Az, sqrt, ADC);
    }
    
    
}

uint32_t SendToFlash(uint32_t secondes,int16_t Ax,int16_t Ay,int16_t Az,uint16_t sqrt,uint16_t ADC, uint32_t Address)
{
    unsigned char data[] = {secondes >>24, (secondes >> 16) & 0xFF, (secondes >> 8) & 0xFF, secondes & 0xFF, (Ax>>8) & 0xFF, Ax & 0xFF,
    (Ay>>8) & 0xFF, Ay & 0xFF, (Az>>8) & 0xFF, Az & 0xFF, (sqrt>>8) & 0xFF, sqrt & 0xFF, (ADC>>8) & 0xFF, ADC & 0xFF};
    
    SPIFLASH_ProgramPage(Address, data, sizeof(data));
    
    
    /*
    char buffer [50];
    UART_PutString("\n\rData sent TO MEMORY:");
    for(int i =0;i<sizeof(data);i++)
    {
        sprintf(buffer,"\n\rData#%d:0x%x",i,data[i]);UART_PutString(buffer);
    }*/
    
    
    
    return Address + sizeof(data);
}

void ReadFlash(uint32_t* secondes,int16_t* Ax,int16_t* Ay,int16_t* Az,uint16_t* sqrt,uint16_t* ADC, uint32_t Address, uint16_t Lenght)
{
    unsigned char DataRead[256] = {0};
    SPIFLASH_Read(Address, DataRead, Lenght);
    
    /*
    char buffer [50];
    UART_PutString("\n\rData reaf from MEMORY:");
    for(int i =0;i<Lenght;i++)
    {
        sprintf(buffer,"\n\rData#%d:0x%x",i,DataRead[i]);UART_PutString(buffer);
    }*/
    
    for(int i = 0;i<15;i++)
    {
        secondes[i] = DataRead[0 + (i*14)]<<24 | DataRead[1 + (i*14)]<<16 | DataRead[2 + (i*14)]<<8 | DataRead[3 + (i*14)];
        Ax[i] = DataRead[4 + (i*14)]<<8 | DataRead[5 + (i*14)]; 
        Ay[i] = DataRead[6 + (i*14)]<<8 | DataRead[7 + (i*14)]; 
        Az[i] = DataRead[8 + (i*14)]<<8 | DataRead[9 + (i*14)]; 
        sqrt[i] = DataRead[10 + (i*14)]<<8 | DataRead[11 + (i*14)]; 
        ADC[i] = DataRead[12 + (i*14)]<<8 | DataRead[13 + (i*14)];  
    }
}

void printAllData(int16_t* Ax,int16_t* Ay,int16_t* Az,uint16_t* sqrt,uint16_t* ADC)
{
    char buffer [50];
    UART_PutString("\n\n\rAllData");
    UART_PutString("\n\rAx");
    for(int i =0;i<16;i++)
    {
        sprintf(buffer,"\n\r%d:0d%d",i,Ax[i]);UART_PutString(buffer);
    }
    
     UART_PutString("\n\rAy");
    for(int i =0;i<15;i++)
    {
        sprintf(buffer,"\n\r%d:0d%d",i,Ay[i]);UART_PutString(buffer);
    }
    
      UART_PutString("\n\rAz");
    for(int i =0;i<15;i++)
    {
        sprintf(buffer,"\n\r%d:0d%d",i,Az[i]);UART_PutString(buffer);
    }
    
}

void SendUartData(int16_t* Ax, int16_t* Ay, int16_t* Az, uint16_t* sqrt, uint16_t* ADC)
{
    UART_PutString("\n\n\r-------------------------------------------");
    UART_PutString("\n\n\rNEW VALUES");
    UART_PutString("\n\n\rTemps d'envoie: ");
    char buffer [50];
    UART_PutString("\n\n\r    Accelerometre");
    
    UART_PutString("\n\rAx min:0d");sprintf(buffer,"%d",Ax[0]);UART_PutString(buffer);
    UART_PutString("\n\rAx max:0d");sprintf(buffer,"%d",Ax[1]);UART_PutString(buffer);
    UART_PutString("\n\rAx moy:0d");sprintf(buffer,"%d",Ax[2]);UART_PutString(buffer);
    
    UART_PutString("\n\n\rAy min:0d");sprintf(buffer,"%d",Ay[0]);UART_PutString(buffer);
    UART_PutString("\n\rAy max:0d");sprintf(buffer,"%d",Ay[1]);UART_PutString(buffer);
    UART_PutString("\n\rAy moy:0d");sprintf(buffer,"%d",Ay[2]);UART_PutString(buffer);
    
    UART_PutString("\n\n\rAz min:0d");sprintf(buffer,"%d",Az[0]);UART_PutString(buffer);
    UART_PutString("\n\rAz max:0d");sprintf(buffer,"%d",Az[1]);UART_PutString(buffer);
    UART_PutString("\n\rAz moy:0d");sprintf(buffer,"%d",Az[2]);UART_PutString(buffer);
    
    UART_PutString("\n\n\r    Magnetude");
    UART_PutString("\n\rsqrt min:");sprintf(buffer,"%d",sqrt[0]);UART_PutString(buffer);
    UART_PutString("\n\rsqrt max:");sprintf(buffer,"%d",sqrt[1]);UART_PutString(buffer);
    UART_PutString("\n\rsqrt moy:");sprintf(buffer,"%d",sqrt[2]);UART_PutString(buffer);
    
    
    UART_PutString("\n\n\r    ADC");
    UART_PutString("\n\rADC min:");sprintf(buffer,"%d",ADC[0]);UART_PutString(buffer);
    UART_PutString("\n\rADC max:");sprintf(buffer,"%d",ADC[1]);UART_PutString(buffer);
    UART_PutString("\n\rADC moy:");sprintf(buffer,"%d",ADC[2]);UART_PutString(buffer);
    
}

void SendToServer(uint32_t* secondes, int16_t* AxCal, int16_t* AyCal, int16_t* AzCal, uint16_t* sqrtCal, uint16_t* ADCCal, 
        int16_t* Ax, int16_t* Ay, int16_t* Az, uint16_t* sqrt, uint16_t* ADC)
{           
    unsigned char checksum = 0;
    
    //Envoie de l'entête
    parallelPortSend(0x05);
    
    //Envoie de la longeur du paquet
    parallelPortSend(0x10); 
    parallelPortSend(0x02); 

    //Envoie de l'étiquette de temps
    parallelPortSend((secondes[15]>>28) & 0X0F); 
    parallelPortSend((secondes[15]>>24) & 0X0F);
    parallelPortSend((secondes[15]>>20) & 0X0F); 
    parallelPortSend((secondes[15]>>16) & 0X0F);
    parallelPortSend((secondes[15]>>12) & 0X0F);
    parallelPortSend((secondes[15]>>8) & 0X0F); 
    parallelPortSend((secondes[15]>>4) & 0X0F);
    parallelPortSend(secondes[15] & 0X0F); 
    
    //Envoie des données calculées
    for(int i=0; i<3; i++)
    {
        parallelPortSend(AxCal[i]>>12); 
        parallelPortSend((AxCal[i]>>8) & 0X0F); 
        parallelPortSend((AxCal[i]>>4) & 0X0F); 
        parallelPortSend(AxCal[i] & 0X0F); 
        
        parallelPortSend(AyCal[i]>>12); 
        parallelPortSend((AyCal[i]>>8) & 0X0F); 
        parallelPortSend((AyCal[i]>>4) & 0X0F); 
        parallelPortSend(AyCal[i] & 0X0F);
        
        parallelPortSend(AzCal[i]>>12); 
        parallelPortSend((AzCal[i]>>8) & 0X0F); 
        parallelPortSend((AzCal[i]>>4) & 0X0F); 
        parallelPortSend(AzCal[i] & 0X0F);
        
        parallelPortSend(sqrtCal[i]>>12); 
        parallelPortSend((sqrtCal[i]>>8) & 0X0F); 
        parallelPortSend((sqrtCal[i]>>4) & 0X0F); 
        parallelPortSend(sqrtCal[i] & 0X0F);
        
        parallelPortSend(ADCCal[i]>>12); 
        parallelPortSend((ADCCal[i]>>8) & 0X0F); 
        parallelPortSend((ADCCal[i]>>4) & 0X0F); 
        parallelPortSend(ADCCal[i] & 0X0F);
    }
    
    //Envoie des 16 captures
    for(int i=0; i<16; i++)
    {
        parallelPortSend((secondes[i]>>28) & 0X0F); 
        parallelPortSend((secondes[i]>>24) & 0X0F);
        parallelPortSend((secondes[i]>>20) & 0X0F); 
        parallelPortSend((secondes[i]>>16) & 0X0F);
        parallelPortSend((secondes[i]>>12) & 0X0F);
        parallelPortSend((secondes[i]>>8) & 0X0F); 
        parallelPortSend((secondes[i]>>4) & 0X0F);
        parallelPortSend(secondes[i] & 0X0F); 
        
        parallelPortSend(Ax[i]>>12);
        parallelPortSend((Ax[i]>>8) & 0X0F); 
        parallelPortSend((Ax[i]>>4) & 0X0F);
        parallelPortSend(Ax[i] & 0X0F);        
        
        parallelPortSend(Ay[i]>>12);
        parallelPortSend((Ay[i]>>8) & 0X0F); 
        parallelPortSend((Ay[i]>>4) & 0X0F);
        parallelPortSend(Ay[i] & 0X0F);   
        
        parallelPortSend(Az[i]>>12);
        parallelPortSend((Az[i]>>8) & 0X0F); 
        parallelPortSend((Az[i]>>4) & 0X0F);
        parallelPortSend(Az[i] & 0X0F);   
        
        parallelPortSend(sqrt[i]>>12);
        parallelPortSend((sqrt[i]>>8) & 0X0F); 
        parallelPortSend((sqrt[i]>>4) & 0X0F);
        parallelPortSend(sqrt[i] & 0X0F); 
        
        parallelPortSend(ADC[i]>>12);
        parallelPortSend((ADC[i]>>8) & 0X0F); 
        parallelPortSend((ADC[i]>>4) & 0X0F);
        parallelPortSend(ADC[i] & 0X0F);   
    }
    
    //Envoie du CHK
        //Envoie du CHK
    checksum = 0x05;
    
    checksum ^= (0x10); 
    checksum ^= (0x02); 

    //Envoie de l'étiquette de temps
    checksum ^=((secondes[15]>>28) & 0X0F); 
    checksum ^=((secondes[15]>>24) & 0X0F);
    checksum ^=((secondes[15]>>20) & 0X0F); 
    checksum ^=((secondes[15]>>16) & 0X0F);
    checksum ^=((secondes[15]>>12) & 0X0F);
    checksum ^=((secondes[15]>>8) & 0X0F); 
    checksum ^=((secondes[15]>>4) & 0X0F);
    checksum ^=(secondes[15] & 0X0F); 
    
    //Envoie des données calculées
    for(int i=0; i<3; i++)
    {
        checksum ^=(AxCal[i]>>12); 
        checksum ^=((AxCal[i]>>8) & 0X0F); 
        checksum ^=((AxCal[i]>>4) & 0X0F); 
        checksum ^=(AxCal[i] & 0X0F); 
        
        checksum ^=(AyCal[i]>>12); 
        checksum ^=((AyCal[i]>>8) & 0X0F); 
        checksum ^=((AyCal[i]>>4) & 0X0F); 
        checksum ^=(AyCal[i] & 0X0F);
        
        checksum ^=(AzCal[i]>>12); 
        checksum ^=((AzCal[i]>>8) & 0X0F); 
        checksum ^=((AzCal[i]>>4) & 0X0F); 
        checksum ^=(AzCal[i] & 0X0F);
        
        checksum ^=(sqrtCal[i]>>12); 
        checksum ^=((sqrtCal[i]>>8) & 0X0F); 
        checksum ^=((sqrtCal[i]>>4) & 0X0F); 
        checksum ^=(sqrtCal[i] & 0X0F);
        
        checksum ^=(ADCCal[i]>>12); 
        checksum ^=((ADCCal[i]>>8) & 0X0F); 
        checksum ^=((ADCCal[i]>>4) & 0X0F); 
        checksum ^=(ADCCal[i] & 0X0F);
    }
    
    //Envoie des 16 captures
    for(int i=0; i<16; i++)
    {
        checksum ^=((secondes[i]>>28) & 0X0F); 
        checksum ^=((secondes[i]>>24) & 0X0F);
        checksum ^=((secondes[i]>>20) & 0X0F); 
        checksum ^=((secondes[i]>>16) & 0X0F);
        checksum ^=((secondes[i]>>12) & 0X0F);
        checksum ^=((secondes[i]>>8) & 0X0F); 
        checksum ^=((secondes[i]>>4) & 0X0F);
        checksum ^=(secondes[i] & 0X0F); 
        
        checksum ^=(Ax[i]>>12);
        checksum ^=((Ax[i]>>8) & 0X0F); 
        checksum ^=((Ax[i]>>4) & 0X0F);
        checksum ^=(Ax[i] & 0X0F);        
        
        checksum ^=(Ay[i]>>12);
        checksum ^=((Ay[i]>>8) & 0X0F); 
        checksum ^=((Ay[i]>>4) & 0X0F);
        checksum ^=(Ay[i] & 0X0F);   
        
        checksum ^=(Az[i]>>12);
        checksum ^=((Az[i]>>8) & 0X0F); 
        checksum ^=((Az[i]>>4) & 0X0F);
        checksum ^=(Az[i] & 0X0F);   
        
        checksum ^=(sqrt[i]>>12);
        checksum ^=((sqrt[i]>>8) & 0X0F); 
        checksum ^=((sqrt[i]>>4) & 0X0F);
        checksum ^=(sqrt[i] & 0X0F); 
        
        checksum ^=(ADC[i]>>12);
        checksum ^=((ADC[i]>>8) & 0X0F); 
        checksum ^=((ADC[i]>>4) & 0X0F);
        checksum ^=(ADC[i] & 0X0F);   
    }
    
    parallelPortSend(checksum); 
}