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
#include "accelerometre.h"
#include "S4App3.h"



void Acc_val_16bits(unsigned char *rgRawVals, int16_t *valeurAccXYZ)
{
    // Initialisation des variables x,y,z sur 16 bits
    valeurAccXYZ[0] = (((uint16_t)rgRawVals[0]) << 4) | (rgRawVals[1] >> 4);
    valeurAccXYZ[1] = (((uint16_t)rgRawVals[2]) << 4) | (rgRawVals[3] >> 4);
    valeurAccXYZ[2] = (((uint16_t)rgRawVals[4]) << 4) | (rgRawVals[5] >> 4);           
    // Mettre le bit de signe au MSB du 16 bits
    if(valeurAccXYZ[0] & 1<<11)valeurAccXYZ[0] |= 0xF000;
    if(valeurAccXYZ[1] & 1<<11)valeurAccXYZ[0] |= 0xF000;
    if(valeurAccXYZ[2] & 1<<11)valeurAccXYZ[0] |= 0xF000;
}
 





/* *****************************************************************************
 End of File
 */
