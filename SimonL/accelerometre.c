/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int16_t valeurAccXYZ[3] = {0, 0, 0};

int16_t *Acc_val_16bits(unsigned char *rgRawVals)
{
    // Initialisation des variables x,y,z sur 16 bits
    valeurAccXYZ[0] = (((int16_t)rgRawVals[0]) << 4) + (rgRawVals[1] >> 4);
    valeurAccXYZ[1] = (((int16_t)rgRawVals[2]) << 4) + (rgRawVals[3] >> 4);
    valeurAccXYZ[2] = (((int16_t)rgRawVals[4]) << 4) + (rgRawVals[5] >> 4);           
                
    // Mettre le bit de signe au MSB du 16 bits
    if(valeurAccXYZ[0] & 1<<11)
    {
        valeurAccXYZ[0] |= (1<<15);
        valeurAccXYZ[0] &= ~(1<<11);
    }

    if(valeurAccXYZ[1] & 1<<11)
    {
        valeurAccXYZ[1] |= (1<<15);
        valeurAccXYZ[1] &= ~(1<<11);
    }

    if(valeurAccXYZ[2] & 1<<11)
    {
        valeurAccXYZ[2] |= (1<<15);
        valeurAccXYZ[2] &= ~(1<<11);
    }
    
    return valeurAccXYZ;
}
 





/* *****************************************************************************
 End of File
 */
