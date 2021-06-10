/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    calculCaptures.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "calculCaptures.h"
#include "S4App3.h"

int16_t tbFiltreDonneesSigned[3] = {0, 0, 0};
uint16_t tbFiltreDonneesUnsigned[3] = {0, 0, 0};

//==================================================
// Valeur Unsigned
//==================================================

int16_t captureMinSigned(int16_t *tbDonnees)
{
    int16_t valeurMin = 0x7FFF;
    for(int i=0; i<16; i++)
    {
        if(valeurMin > tbDonnees[i])
        {
            valeurMin = tbDonnees[i];
        }
    }
    return valeurMin;
}

int16_t captureMaxSigned(int16_t *tbDonnees)
{
    int16_t valeurMax = 0;
    for(int i=0; i<16; i++)
    {
        if(valeurMax < tbDonnees[i])
        {
            valeurMax = tbDonnees[i];
        }
    }
    return valeurMax;
}

int16_t captureMoyenSigned(int16_t *tbDonnees)
{
    int32_t valeurMoyenne = 0;
    for(int i=0; i<16; i++)
    {
        valeurMoyenne = valeurMoyenne + tbDonnees[i];
    }
    return (valeurMoyenne/16);
}

int16_t *captureCalculSigned(int16_t *tbDonnees)
{       
    for(int i=0; i<16; i++)
    {
        tbFiltreDonneesSigned[i] = 0;
    }
    
    tbFiltreDonneesSigned[0] = captureMinSigned(tbDonnees);
    tbFiltreDonneesSigned[1] = captureMaxSigned(tbDonnees);
    tbFiltreDonneesSigned[2] = captureMoyenSigned(tbDonnees);
            
    return tbFiltreDonneesSigned;
}

//==================================================
// Valeur Unsigned
//==================================================
uint16_t captureMinUnsigned(uint16_t *tbDonnees)
{
    uint16_t valeurMin = 0xFFFF;
    for(int i=0; i<16; i++)
    {
        if(valeurMin > tbDonnees[i])
        {
            valeurMin = tbDonnees[i];
        }
    }
    return valeurMin;
}

uint16_t captureMaxUnsigned(uint16_t *tbDonnees)
{
    uint16_t valeurMax = 0;
    for(int i=0; i<16; i++)
    {
        if(valeurMax < tbDonnees[i])
        {
            valeurMax = tbDonnees[i];
        }
    }
    return valeurMax;
}

uint16_t captureMoyenUnsigned(uint16_t *tbDonnees)
{
    uint32_t valeurMoyenne = 0;
    for(int i=0; i<16; i++)
    {
        valeurMoyenne = valeurMoyenne + tbDonnees[i];
    }
    return (valeurMoyenne/16);
}

uint16_t *captureCalculUnsigned(uint16_t *tbDonnees)
{   
    for(int i=0; i<16; i++)
    {
        tbFiltreDonneesUnsigned[i] = 0;
    }
    
    tbFiltreDonneesUnsigned[0] = captureMinUnsigned(tbDonnees);
    tbFiltreDonneesUnsigned[1] = captureMaxUnsigned(tbDonnees);
    tbFiltreDonneesUnsigned[2] = captureMoyenUnsigned(tbDonnees);
            
    return tbFiltreDonneesUnsigned;
}



/* *****************************************************************************
 End of File
 */
