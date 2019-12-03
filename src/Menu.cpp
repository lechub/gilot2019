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
#include "VEprom.h"

#include "NumberEditor.h"

using namespace HMI;

void Menu::goToEkran(EKRAN nowyEkran){
	ekran = nowyEkran;
	if (ekran == EKRAN::e_INIT) {
		Praca::getInstance()->stopWork();
	}


	lcd->clearScreen();
	showEkran();
}


void Menu::poll(){

	if (ekran == e_INIT){
		if(!Praca::getInstance()->isStartup()) goToEkran(EKRAN::e_READY);
	}

	showEkran();

	Keyboard::Key key = keys->getKey();

	switch(ekran){
	case e_INIT:{ // jakikolwiek klawisz albo czas startup
		if ( key == Keyboard::Key::ASTERIX){
			//neIlosc.setCursorAt(0);
			goToEkran(EKRAN::e_UST_KALIBR);
		}else if ( key != Keyboard::Key::NONE){
			goToEkran(EKRAN::e_READY);
		}else if (!Praca::getInstance()->isStartup()) {
			goToEkran(EKRAN::e_READY);
		}
	}break;

	case e_READY:  {
		if (key == Keyboard::Key::HASH){
		  Praca::getInstance()->startWork();
//			Praca::getInstance()->startWorkCycle();
			goToEkran(e_PRACA_MOTOR);
		}else if (key == Keyboard::Key::ASTERIX){
			//neDlugosc.setCursorAt(0);
			//neIlosc.setCursorAt(0);
			goToEkran(EKRAN::e_UST_DLUGOSC);
		}
	}break;

	case e_PRACA_MOTOR:{
		if (key != Keyboard::Key::NONE){
			Praca::getInstance()->stopWork();
			goToEkran(EKRAN::e_READY);
		}else if (krokowy->isStopped()){
			knife->start();
			goToEkran(EKRAN::e_PRACA_KNIFE);
		}
	}break;

	case e_PRACA_KNIFE:	{
		if (key != Keyboard::Key::NONE){
			Praca::getInstance()->stopWork();
			goToEkran(EKRAN::e_READY);
		}else if (knife->isStopped()){
			if (knife->isInError()){
				goToEkran(EKRAN::e_KNIFE_ERROR);
				break;
			}
			Praca::getInstance()->cycleWasDone();
			if(Praca::getInstance()->isAllDone()){
				goToEkran(EKRAN::e_READY);
			}else{
				Praca::getInstance()->startWorkCycle();
				goToEkran(e_PRACA_MOTOR);
			}
		}
	}break;

	case e_UST_DLUGOSC: {
		if (key == Keyboard::Key::HASH){
			VEprom::writeWord(VEprom::VirtAdres::DLUGOSC, neDlugosc.getValue());
			goToEkran(EKRAN::e_UST_ILOSC);
		}else if (key == Keyboard::Key::ASTERIX){
			goToEkran(EKRAN::e_READY);
		}else if (key != Keyboard::Key::NONE){
			neDlugosc.setDigit(Keyboard::keyToInt(key));
		}
	}break;

	case e_UST_ILOSC: {
		if (key == Keyboard::Key::HASH){
			VEprom::writeWord(VEprom::VirtAdres::ILOSC, neIlosc.getValue());
			goToEkran(EKRAN::e_READY);
		}else if (key == Keyboard::Key::ASTERIX){
			goToEkran(EKRAN::e_READY);
		}else if (key != Keyboard::Key::NONE){
			neIlosc.setDigit(Keyboard::keyToInt(key));
		}
	}break;

	case e_UST_KALIBR: {
		if (key == Keyboard::Key::HASH){
			VEprom::writeWord(VEprom::VirtAdres::KALIBRACJA, neKalibr.getValue());
			goToEkran(EKRAN::e_READY);
		}else if (key == Keyboard::Key::ASTERIX){
			goToEkran(EKRAN::e_READY);
		}else if (key != Keyboard::Key::NONE){
			neKalibr.setDigit(Keyboard::keyToInt(key));
		}
	}break;

	case e_KNIFE_ERROR:{
		if (key != Keyboard::Key::NONE){
			goToEkran(EKRAN::e_READY);
		}
	}break;

	case e_DEBUG:  {

	}break;

	default: break;
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
		neDlugosc.setValue(VEprom::readWord(VEprom::VirtAdres::DLUGOSC));
		neIlosc.setValue(VEprom::readWord(VEprom::VirtAdres::ILOSC));
		lcd->gotoXY(0, 0);
		lcd->print(&neDlugosc);
		lcd->gotoXY(0, 1);
		lcd->print(&neIlosc);
	}break;

	case e_PRACA_MOTOR: {
		//--------------->1234567890123456<
		lcd->printXY(0,0, "    PRZESUW    ");  // przerwa

		neToGo.setValue(krokowy->getLengthToGo());
		neToCut.setValue(Praca::getInstance()->getCountToGo());
		lcd->print(&neToGo);
		lcd->print(&neToCut);
	}break;

	case e_PRACA_KNIFE:{
		//--------------->1234567890123456<
		lcd->printXY(0,0, "   CIECIE   ");  // przerwa
		lcd->gotoXY(0,1);
		neToCut.setValue(Praca::getInstance()->getCountToGo());
		lcd->print( "Zostalo");
		lcd->print(&neToCut);
	}break;

	case e_UST_DLUGOSC:{
		//----------------->1234567890123456<
		lcd->printXY(0, 0, " USTAW DLUGOSC  ");  // przerwa
		lcd->gotoXY(0,1);
		lcd->print(&neDlugosc);
	} break;

	case e_UST_ILOSC:{
		//----------------->1234567890123456<
		lcd->printXY(0, 0, "   USTAW ILOSC  ");  // przerwa
		lcd->gotoXY(0,1);
		lcd->print(&neIlosc);
	} break;

	case e_UST_KALIBR:{
		//----------------->1234567890123456<
		lcd->printXY(0, 0, "  USTAW KALIBR. ");  // przerwa
		lcd->gotoXY(0,1);
		lcd->print(&neKalibr);
	} break;

	case e_KNIFE_ERROR:  {
		//----------------->1234567890123456<
		lcd->printXY(0, 0, "** BLAD NOZA! **");
		lcd->gotoXY(0, 1);
		neToCut.setValue(Praca::getInstance()->getCountToGo());
		lcd->print( "Zostalo");
		lcd->print(&neToCut);
	}break;



	case e_DEBUG:{

	}break;

	default:
		break;
	}
}

void Menu::init(FrameBuffer * pLcd){
	lcd = pLcd;
	ekran = EKRAN::e_INIT;
	//goToEkran(EKRAN::e_INIT);
	//  neDlugosc = NumberEditor("DLUGOSC:00000 mm", VEprom::readWord(VEprom::VirtAdres::DLUGOSC));
	//  neIlosc   = NumberEditor("ILOSC:00000 szt.", VEprom::readWord(VEprom::VirtAdres::ILOSC));
	//  neKalibr  = NumberEditor("KALIBRAC.:000 mm", VEprom::readWord(VEprom::VirtAdres::KALIBRACJA));
	//  neToGo 	  = NumberEditor(" [000] ", neDlugosc.getValue());
	//  neToCut 	  = NumberEditor(        "<00000> ", neIlosc.getValue());

	neDlugosc.setValue(VEprom::readWord(VEprom::VirtAdres::DLUGOSC));
	neIlosc.setValue(VEprom::readWord(VEprom::VirtAdres::ILOSC));
	neKalibr.setValue(VEprom::readWord(VEprom::VirtAdres::KALIBRACJA));
	neToGo.setValue(VEprom::readWord(VEprom::VirtAdres::DLUGOSC));
	neToCut.setValue(VEprom::readWord(VEprom::VirtAdres::ILOSC));
}


