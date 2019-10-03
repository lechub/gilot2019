/*
 * Eprom.cpp
 *
 *  Created on: 18 sty 2019
 *      Author: lechu
 */

#include <VEprom.h>

uint16_t VirtAddVarTab[NB_OF_VAR];

uint16_t Vars[NB_OF_VAR];



bool VEprom::init(){
  FLASH_Unlock();
  bool result = true;
  result = (EE_Init() == FLASH_COMPLETE);
  FLASH_Lock();
  for (uint16_t i = 0; i < VirtAdres::COUNT; i++){
    uint16_t res = 0;
    res = EE_ReadVariable(i, &Vars[i]);
    if (res == 1){  // variable not found
      if (!writeWord((VirtAdres)i, 0x00)) result = false;
    }else if (res >1){
      result = false;
    }
  }
  return result;
}

bool VEprom::writeWord(VirtAdres virtAdres, uint16_t val){
  if (virtAdres > NB_OF_VAR) return false;
  if (virtAdres > VirtAdres::COUNT) return false;
  if (Vars[virtAdres] == val) return true;  // już jest
  bool result = true;
  if (isWritePossible()){
    FLASH_Unlock();
    if (EE_WriteVariable((uint16_t)virtAdres, val) != FLASH_COMPLETE){
      result = false;
    }else{
      Vars[virtAdres] = val;
    }
    FLASH_Lock();
  }else{
    result = false;
  }
  return result;
}

bool VEprom::readWord(VirtAdres virtAdres, uint16_t * result){
  if (virtAdres > NB_OF_VAR) return false;
  if (virtAdres > VirtAdres::COUNT) return false;
  *result = Vars[virtAdres];
  return true;
  //  uint16_t res = 0;
  //  res = EE_ReadVariable((uint16_t)virtAdres, result);
  //  if (res == 0){    // read ok
  //    return true;
  //  }else if (res == 1){  // variable not found
  //    *result = 0;
  //    return writeWord(virtAdres, *result);
  //  }
  //  return false;
}



