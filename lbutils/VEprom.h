/*
 * Eprom.h
 *
 *  Created on: 18 sty 2019
 *      Author: lechu
 */

#ifndef VEPROM_H_
#define VEPROM_H_

#include "systemDefs.h"
#include "eeprom.h"
//#include "stm32f405_flashEE.h"
#include "stm32f0xx_flash.h"

class VEprom {

private:

public:
  typedef enum{
    Checksum = 0,
    SoftVersion,
    LICZNIK,
    NAPED,
    PASSWORD,
    FLAGS1,
    Adres,
    OPOZN_ZAMK_POZAR,
    KLAW_BISTABILNA,
    AUTO_OTWARCIE_PO_POZARZE,
    OVC_LEVEL,        // poziom zadziałania OVC od 0..9 -> 0=wyłączony, 9=od razu po wykryciu
    COUNT,
  }VirtAdres;



  static bool init();
  //  {
  //    FLASH_Unlock();
  //    bool result = true;
  //    result = (EE_Init() == FLASH_COMPLETE);
  //    FLASH_Lock();
  //    return result;
  //  }

  static bool writeWord(VirtAdres virtAdres, uint16_t val);
  //  {
  //    if (virtAdres > NB_OF_VAR) return false;
  //    if (virtAdres > VirtAdres::COUNT) return false;
  //    FLASH_Unlock();
  //    uint16_t result = 0;
  //    result = EE_WriteVariable((uint16_t)virtAdres, val);
  //    FLASH_Lock();
  //    return (result == FLASH_COMPLETE);
  //  }

  static bool readWord(VirtAdres virtAdres, uint16_t * result);
  //  {
  //    if (virtAdres > NB_OF_VAR) return false;
  //    if (virtAdres > VirtAdres::COUNT) return false;
  //    uint16_t res = 0;
  //    res = EE_ReadVariable((uint16_t)virtAdres, result);
  //    if (res == 0){    // read ok
  //      return true;
  //    }else if (res == 1){  // variable not found
  //      *result = 0;
  //      return writeWord(virtAdres, *result);
  //    }
  //    return false;
  //  }

  static uint32_t readWord(VirtAdres virtAdres){
    uint16_t result;
    if (readWord(virtAdres, &result)){
      return (uint32_t)result;
    }else{
      return 0xffffffff;
    }
  }

  static bool readBool(VirtAdres virtAdres){
    uint16_t bist  = (uint16_t)VEprom::readWord(virtAdres);
    return bist != 0;
  }

  static void writeBool(VirtAdres virtAdres, bool value){
    uint16_t u16value = value ? 1 : 0;
    writeWord(virtAdres, u16value);
  }

  static bool toggleBool(VirtAdres virtAdres){
    bool value = !readBool(virtAdres);
    writeBool(virtAdres, value);
    return value;
  }


  static bool addToValue(VirtAdres virtAdres, uint16_t valueToAdd){
    uint16_t val;
    if (!readWord(virtAdres, &val)) return false;
    if (!writeWord(virtAdres, (uint16_t)(val + valueToAdd))) return false;
    return true;
  }

  static bool subtractFromValue(VirtAdres virtAdres, uint16_t valueToSub){
    uint16_t val;
    if (!readWord(virtAdres, &val)) return false;
    if (!writeWord(virtAdres, (uint16_t)(val - valueToSub))) return false;
    return true;
  }

  static bool isWritePossible(){
    //if (Pomiar::getPomiar(Pomiar::Nr::)) // Todo zakaz zapisu, gdy napiecie zasilania za niskie
    return true;
  }

};

#endif /* VEPROM_H_ */
