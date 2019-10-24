/*
 * Keyboard.h
 *
 *  Created on: 15.09.2016
 *      Author: lechu
 */


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "Button.h"
#include "Fifo.h"
#include "QuickTask.h"
#include "Gpio.h"



class Keyboard {

public:
	static constexpr uint16_t BUFFERSIZE = 4;	// rozmiar bufora znakow
	static constexpr uint32_t TIME_PERIOD_KEYB_MS = 10;

	typedef enum{
		NONE = 0,
		KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, KEY8, KEY9, KEY0,
		ASTERIX, HASH,
	}Key;


private:
	Gpio *gpioCol1;
	Gpio *gpioCol2;
	Gpio *gpioCol3;
	Gpio *gpioRow1;
	Gpio *gpioRow2;
	Gpio *gpioRow3;
	Gpio *gpioRow4;

	Key oldKey = Key::NONE;

	// bufor znakow - od najstarszego na najmlodszego. Brak znaku to 'Key::NONE'
	uint8_t buffer[BUFFERSIZE];
	Fifo fifo = Fifo(buffer, BUFFERSIZE);

	/**
	 * kasuje ostatni znak, przesuwa wszystkie o jeden i wpisuje najnowszy na koniec
	 */
	void addToBuffer(Key key){
		if (key == NONE) return;
		fifo.put((uint8_t)key);
	}

	Key getFromBuffer(){
		while(fifo.isNotEmpty()){
			Key result = (Key)fifo.get();
			if (result != Key::NONE) return result;	// pomijac przypadkowe NONE az bedzie wlasciwy klawisz
		}
		return Key::NONE;
	}

	void setColumnLow(uint8_t nrColumn){
		gpioCol1->setOutput(Gpio::Level::High);
		gpioCol2->setOutput(Gpio::Level::High);
		gpioCol3->setOutput(Gpio::Level::High);
		switch(nrColumn){
		case 1:	gpioCol1->setOutput(Gpio::Level::Low);
		//while (gpioCol1->getOutput() == Gpio::Level::Low){};
		break;
		case 2:	gpioCol2->setOutput(Gpio::Level::Low);
		//while (gpioCol2->getOutput() == Gpio::Level::Low){};
		break;
		case 3:	gpioCol3->setOutput(Gpio::Level::Low);
		//while (gpioCol3->getOutput() == Gpio::Level::Low){};
		break;
		default:
			break;
		}
	}

	uint8_t getRowPressed(){
		if (gpioRow1->getInput() == Gpio::Level::Low) return 1;
		if (gpioRow2->getInput() == Gpio::Level::Low) return 2;
		if (gpioRow3->getInput() == Gpio::Level::Low) return 3;
		if (gpioRow4->getInput() == Gpio::Level::Low) return 4;
		return 0;
	}

	static Key getKeyAt(uint8_t column, uint8_t row){
		switch(column){
		case 1:{
			switch(row){
			case 1: return Key::KEY1;
			case 2: return Key::KEY4;
			case 3: return Key::KEY7;
			case 4: return Key::ASTERIX;
			default: break;
			}
		}break;
		case 2:{
			switch(row){
			case 1: return Key::KEY2;
			case 2: return Key::KEY5;
			case 3: return Key::KEY8;
			case 4: return Key::KEY0;
			default: break;
			}
		}break;
		case 3:{
			switch(row){
			case 1: return Key::KEY3;
			case 2: return Key::KEY6;
			case 3: return Key::KEY9;
			case 4: return Key::HASH;
			default: break;
			}
		}break;
		default: break;
		}
		return Key::NONE;
	}

	// unbuffered key state
	Key getPressedKey(){
		for( int col = 1; col <= 3; col++){
			setColumnLow(col);
			uint8_t row = getRowPressed();
			if ( row == 0){
				continue;
			}
			return getKeyAt(col, row);
		}
		return Key::NONE;
	}



public:
	Keyboard(Gpio *col1, Gpio *col2, Gpio *col3,
			Gpio *row1, Gpio *row2, Gpio *row3, Gpio *row4):
				gpioCol1(col1), gpioCol2(col2), gpioCol3(col3),
				gpioRow1(row1), gpioRow2(row2), gpioRow3(row3), gpioRow4(row4){

		for (uint16_t i = 0; i<BUFFERSIZE; i++)	buffer[i] = NONE;
	}

	void setup(){
		gpioCol1->setup(Gpio::GpioMode::OUTPUT, Gpio::GpioOType::PushPull, Gpio::GpioPuPd::NoPull, Gpio::GpioSpeed::HighSpeed);
		gpioCol2->setupFromClone(gpioCol1);
		gpioCol3->setupFromClone(gpioCol1);
		gpioRow1->setup(Gpio::GpioMode::INPUT, Gpio::GpioOType::NoMatter, Gpio::GpioPuPd::PullUp, Gpio::GpioSpeed::HighSpeed);
		gpioRow2->setupFromClone(gpioRow1);
		gpioRow3->setupFromClone(gpioRow1);
		gpioRow4->setupFromClone(gpioRow1);
	}

	void poll(){
		Key newKey = getUnbufferedKey();
		if (oldKey != newKey){
			oldKey = newKey;
			if (newKey != NONE) addToBuffer(newKey);
		}
	}

	bool haveBufferedKey() {
		return fifo.isNotEmpty();
	}

	Key getKey(){
		return getFromBuffer();
	}

	Key getUnbufferedKey(){
		return getPressedKey();
	}

};

extern Keyboard * keys;

#endif /* KEYBOARD_H_ */
