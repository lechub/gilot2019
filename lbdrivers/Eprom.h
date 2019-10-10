/*
 * Eprom.h
 *
 *  Created on: 18 sty 2019
 *      Author: lechu
 */

#ifndef EPROM_H_
#define EPROM_H_

#include "systemDefs.h"
#include "eeprom.h"

class Eprom {

private:

public:
  typedef enum{
    Version,
    Adres,
    Dummy,
  }VirtAdres;



  static bool init(){
    FLASH_Unlock();
    return (EE_Init() == FLASH_Status::FLASH_COMPLETE);
    FLASH_Lock();
  }

  static bool writeWord(VirtAdres virtAdres, uint16_t val){
    if (virtAdres > NB_OF_VAR) return false;
    FLASH_Unlock();
    uint16_t result = EE_WriteVariable((uint16_t)virtAdres, val);
    FLASH_Lock();
    return (result == FLASH_Status::FLASH_COMPLETE);
  }

  static bool readWord(VirtAdres virtAdres, uint16_t * result){
    if (virtAdres > NB_OF_VAR) return false;
    uint16_t res = EE_ReadVariable((uint16_t)virtAdres, result);
    if (res == 0){    // read ok
      return true;
    }else if (res == 1){  // variable not found
      *result = 0;
      return writeWord(virtAdres, *result);
    }
    return false;
  }

};

#endif /* EPROM_H_ */
