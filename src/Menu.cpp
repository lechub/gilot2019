/*
 * Menu.cpp
 *
 *  Created on: 18 paź 2017
 *      Author: lechu
 */

#include "Menu.h"
#include <ctype.h>
#include "main.h"
//#include <VEprom.h>
//#include "Keyboard.h"
//#include "FrameBuffer.h"
//#include "HMI.h"
#include "Praca.h"
//#include "CSP.h"


void Menu::goToEkran(EKRAN nowyEkran){
	ekran = nowyEkran;
	if (ekran == EKRAN::e_INIT) {
		krokowy->stop();
	}

	editMode = false;

	lcd->clearScreen();
	showEkran();
}


void Menu::poll(){

	Keyboard::Key key = keys->getKey();

	//	// przywracanie ekranu glownego jesli klawiatura jest nieuzywana
	//	if ((key == Keyboard::Key::NONE)&&
	//			(!HMI::getInstance()->isBackLightInUse())&&
	//			(ekran != e_MAIN)){
	//		goToEkran(EKRAN::e_MAIN);
	//	}

	switch(ekran){

	case e_INIT:{ // jakikolwiek klawisz albo czas startup
		//		if ( key != Keyboard::Key::NONE) goToEkran(EKRAN::e_READY);
		//		if (!HMI::getInstance()->isStartup()) goToEkran(EKRAN::e_MAIN);
		break;
	}

	case e_READY:  {
		if (key == Keyboard::Key::HASH){
			Praca::getInstance()->startWork();
		}else if (key == Keyboard::Key::ASTERIX){
			goToEkran(EKRAN::e_UST_DLUGOSC);
		}
	}break;

	case e_PRACA_MOTOR:
	case e_PRACA_KNIFE:	{
		Praca::getInstance()->stopWork();
		goToEkran(EKRAN::e_READY);
	}break;

	case e_UST_DLUGOSC: {
		if (key == Keyboard::Key::HASH){
			Praca::getInstance()->setLength(tmpValue);
			goToEkran(EKRAN::e_UST_DLUGOSC);
		}
		if (key == Keyboard::Key::ASTERIX){
			goToEkran(EKRAN::e_READY);
		}
	}break;
	case e_UST_ILOSC: {
		if (key == Keyboard::Key::HASH){
			Praca::getInstance()->setQuantity(tmpValue);
		}
		goToEkran(EKRAN::e_READY);
	}break;


	case e_DEBUG:  {

	}break;

	default: break;
	}

	showEkran();
}



void Menu::printPattern(const char * pattern, uint32_t value){
	lcd->printNumbersWithPattern(pattern, value);
	if (editMode){
		const char * ptr = pattern;
		while (*ptr != '\0'){ptr++;}
		char znak;
		uint16_t offset = 0;
		do{
			znak = char(*ptr);
			if (isdigit(znak)) break;
			ptr--;
			offset++;
		}while (ptr >= pattern);
		lcd->cursorLeft(offset);
		lcd->cursorMode(FrameBuffer::CursorMode::BLINK);
	}else{
		lcd->cursorMode(FrameBuffer::CursorMode::HIDDEN);
	}
}



void Menu::showEkran(){

	switch(ekran){

	case e_INIT: {
		//---------------->1234567890123456<
		lcd->printXY(0, 0," GILOTYNKA KABE ");
		lcd->gotoXY(0, 1);
		//--------------------------->1234567890123456<
		lcd->printNumbersWithPattern("WERSJA: 000.", getProgMainVersion());
		lcd->printNumbersWithPattern(            "000 ", getProgSubVersion());
	}
	break;


	case e_READY:  {
		lcd->gotoXY(0,0);
		//--------------------------->1234567890123456<
		lcd->printNumbersWithPattern("DLUGOSC: 000 mm ", VEprom::readWord(VEprom::VirtAdres::DLUGOSC));
		lcd->gotoXY(0,1);
		lcd->printNumbersWithPattern("ILOSC:00000 szt.", VEprom::readWord(VEprom::VirtAdres::ILOSC));
		break;
	}

	case e_PRACA_MOTOR: {
		//--------------->1234567890123456<
		lcd->printXY(0,0, "    PRZESUW    ");  // przerwa
		lcd->gotoXY(0,1);
		//--------------------------->1234567890123456<
		lcd->printNumbersWithPattern("[00000]",krokowy->getLengthToGo());
		lcd->printNumbersWithPattern(        "<000000>",Praca::getInstance()->getCountToGo());
	}break;

	case e_PRACA_KNIFE:{
		//--------------->1234567890123456<
		lcd->printXY(0,0, "   CIECIE   ");  // przerwa
		lcd->gotoXY(0,1);
		//--------------------------->1234567890123456<
		lcd->printNumbersWithPattern( "   <000000>    ",Praca::getInstance()->getCountToGo());
	}break;

	/*    e_UST_DLUGOSC,		// ustawienie dlugosci ciecia
	       e_UST_ILOSC,		// ustawienie ilosci blaszek
	       e_UST_KALIBR,		// ustawienie danych kalibracyjnych
	       e_DEBUG,	*/
	case e_UST_DLUGOSC:{
		//----------------->1234567890123456<
		lcd->printXY(0, 0, " USTAW DLUGOSC  ");  // przerwa
		lcd->gotoXY(0,1);
		//------------------->1234567890123456<
		lcd->editWithPattern("  <0000> mm     ", tmpValue);
	} break;

	case e_UST_ILOSC:{
		//----------------->1234567890123456<
		lcd->printXY(0, 0, "   USTAW ILOSC  ");  // przerwa
		lcd->gotoXY(0,1);
		//------------------->1234567890123456<
		lcd->editWithPattern("  <000000> mm   ", tmpValue);
	} break;

	case e_UST_KALIBR:{
		//----------------->1234567890123456<
		lcd->printXY(0, 0, "  USTAW KALIBR. ");  // przerwa
		lcd->gotoXY(0,1);
		//------------------->1234567890123456<
		lcd->editWithPattern("  <000000> mm   ", tmpValue);
	} break;

	case e_DEBUG:{
		//----------------->1234567890123456<
		//    lcd->gotoXY(0, 0);
		//
		//    lcd->print("1:");
		//    lcd->print( pins->gpioWyj1H.getOutput() ? 'H' : 'L');
		//    lcd->print(" 2:");
		//    lcd->print( pins->gpioWyj2H.getOutput() ? 'H' : 'L');
		//    lcd->print(" 3:");
		//    lcd->print( pins->gpioWyj3H.getOutput() ? 'H' : 'L');
		//    lcd->print(" 4:");
		//    lcd->print( pins->gpioWyj4H.getOutput() ? 'H' : 'L');
		//
		//    //    uint16_t val = Pomiar::getPomiar(Pomiar::Nr::K1Stat);
		//    //    lcd->printNumbersWithPattern("######0,", val);
		//    //    val = Pomiar::getPomiar(Pomiar::Nr::K2Stat);
		//    //    lcd->printNumbersWithPattern("######0,", val);
		//    lcd->gotoXY(0, 1);
		//
		//    lcd->print("1:");
		//    lcd->print( pins->gpioWyj1L.getOutput() ? 'H' : 'L');
		//    lcd->print(" 2:");
		//    lcd->print( pins->gpioWyj2L.getOutput() ? 'H' : 'L');
		//    lcd->print(" 3:");
		//    lcd->print( pins->gpioWyj3L.getOutput() ? 'H' : 'L');
		//    lcd->print(" 4:");
		//    lcd->print( pins->gpioWyj4L.getOutput() ? 'H' : 'L');
		//
		//    //    lcd->printNumbersWithPattern("##0,", Hardware::getAHB_HCLK()/1000000);
		//    //    lcd->printNumbersWithPattern("##0,", Hardware::getAPB1_PCLK()/1000000);
		//    //    lcd->printNumbersWithPattern("##0,", Hardware::getAPB2_PCLK()/1000000);
	}break;

	default:
		break;
	}
}



