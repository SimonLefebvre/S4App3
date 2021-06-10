/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    calculCaptures.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _CALCULCAPTURES_H/* Guard against multiple inclusion */
#define _CALCULCAPTURES_H

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    int16_t captureMinSigned(int16_t *tbDonnees);
    int16_t captureMaxSigned(int16_t *tbDonnees);
    int16_t captureMoyenSigned(int16_t *tbDonnees);
    void captureCalculSigned(int16_t *tbDonnees, int16_t*);

    uint16_t captureMinUnsigned(uint16_t *tbDonnees);
    uint16_t captureMaxUnsigned(uint16_t *tbDonnees);
    uint16_t captureMoyenUnsigned(uint16_t *tbDonnees);    
    void captureCalculUnsigned(uint16_t *tbDonnees, uint16_t*);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _CALCULCAPTURES_H */

/* *****************************************************************************
 End of File
 */
