/*
 * HD44780.cpp
 *
 *  Created on: 2 paź 2019
 *      Author: lechu
 */


#include <Settings.h>
#include <string.h>
#include <stdlib.h>
#include "HD44780.h"
#include "QuickTask.h"
#include "Fifo.h"
#include "Hardware.h"

HD44780 instance = HD44780();

HD44780 * HD44780::getInstance(){
  return &instance;
}

void pollCallback(){
  HD44780::getInstance()->poll();
}

// periodycznie wykonywana funkcja monitor() opakowana w aku_callback()
QuickTask hd44780Task(QuickTask::QT_PERIODIC, pollCallback, HD44780::LCD_REFRESH_INTERVAL_MILISECOND);


uint8_t dataTab1[HD44780::LCD_CHARS_AMOUNT];
FrameBuffer frame_buffer(HD44780::LCD_COLUMNS, HD44780::LCD_ROWS, dataTab1);

FrameBuffer * HD44780::getFrameBuffer(){
  return &frame_buffer;
}

uint8_t dataTab2[HD44780::LCD_CHARS_AMOUNT];
Fifo dataFifo(dataTab2, HD44780::LCD_CHARS_AMOUNT);

Fifo * HD44780::getDataFifo(){
  return &dataFifo;
}


void HD44780::delayMs(uint32_t milis){
  Hardware::delayMsWithBackground(milis);
}

void HD44780::delayMsDirty(uint32_t milis){
  Hardware::delayMsDirty(milis);
}


// LCD initialization procedure
bool HD44780::setup(Gpio * gpioLcdE, Gpio * gpioLcdRW, Gpio * gpioLcdRS, HD44780::GpioPack8 * gpioLcdD, Gpio * backLightPin){
  gpioE = gpioLcdE;
  gpioRW = gpioLcdRW;
  gpioRS = gpioLcdRS;
  gpioData = gpioLcdD;
  gpioBackLight = backLightPin;

  return localInit();
}

bool HD44780::localInit(){
  gpioE->setup(Gpio::GpioMode::OUTPUT, Gpio::GpioOType::PushPull, Gpio::GpioPuPd::NoPull, Gpio::GpioSpeed::HighSpeed);
  gpioRW->setupFromClone(gpioE);
  gpioBackLight->setupFromClone(gpioE);
  gpioRS->setupFromClone(gpioE);
  for (uint8_t i = 0; i < 8; i++){
    gpioData->gpios[i]->setupFromClone(gpioE);
  }

  lcdStage = LcdStage::START;

  setBackLight(true);
  //setResetPin(false);
  delayMsDirty(50);
  //setResetPin(true);
  delayMsDirty(100);

  sendCommandDirty(0x38, 300);
  sendCommandDirty(0x39, 300);

  sendCommandDirty(0x14, 100);
#ifdef LCD5V
  sendCommandDirty(0x79,100);
  sendCommandDirty(0x50,100);
  sendCommandDirty(0x6c,100);
#else
  sendCommandDirty(0x74,30);
  sendCommandDirty(0x54,30);
  sendCommandDirty(0x6f,30);
#endif
  sendCommandDirty(0x0c,100);
  sendCommandDirty(0x01,100);

  sendCommandDirty(0x38,100);

  setBackLight(false);
  return true;
}



bool HD44780::sendCommandDirty(uint8_t cmd, uint32_t delayMs){
  prepareCommandToWrite(static_cast<LcdCommand>(cmd));
  gpioE->setOutput(true);
  delayMsDirty(2);
  gpioE->setOutput(false);
  delayMsDirty(delayMs);
  return true;
}

char HD44780::getNextChar(){
  FrameBuffer * fb = getFrameBuffer();
  char *buf = reinterpret_cast<char*>(fb->getBuffer());
  if (charOffset >= (LCD_COLUMNS * LCD_ROWS) ){
    charOffset = 0;
  }
  char result = *(buf + charOffset);
  charOffset++;
  return result;
}

void HD44780::poll(){
	if (gpioE->getOutput()){		// tylko zbocze na E?
		gpioE->setOutputDown();
		return;
	}
	if ((charOffset % HD44780::LCD_COLUMNS) == 0){
		uint8_t lineNr = charOffset / HD44780::LCD_COLUMNS;
		uint8_t ddAdr = getDDRamAdres(0, lineNr);
		if (newLine){
			prepareCommandToWrite(static_cast<LcdCommand>(LCD_DDRAM_ADDR | ddAdr));
			newLine = false;
		}
	}else{
		char znak = getNextChar();
		prepareCharToWrite(znak);
		newLine = true;
	}
	gpioE->setOutputUp();

  LcdStage stage = lcdStage;


}

//void HD44780::process(){
//  LcdStage stage = lcdStage;
//  switch(stage){
//  case LcdStage::START:
//  case LcdStage::GOTO_LINE1:
//    if (!isBusy()){
//      sendCommand(LcdCommand::LCD_HOME);
//      lcdStage = LcdStage::SEND_LINE1;
//    }
//    break;
//  case LcdStage::SEND_LINE1:
//    if (!isBusy()){
//      sendLine(0);
//      lcdStage = LcdStage::GOTO_LINE2;
//    }
//    break;
//  case LcdStage::GOTO_LINE2:
//    if (!isBusy()){
//      gotoXY(0,1);
//      lcdStage = LcdStage::SEND_LINE2;
//    }
//    break;
//  case LcdStage::SEND_LINE2:
//    if (!isBusy()){
//      sendLine(1);
//      lcdStage = LcdStage::START;
//    }
//    break;
//  default:
//    lcdStage = LcdStage::START;
//    break;
//  }
//}



