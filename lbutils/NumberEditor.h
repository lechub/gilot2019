/*
 * FieldOfNumbers.h
 *
 *  Created on: 28 paź 2019
 *      Author: lechu
 */

#ifndef NUMBEREDITOR_H_
#define NUMBEREDITOR_H_

#include "Settings.h"
#include "NumberField.h"
#include <cstring>

/**
 * Wyswietla i pozwala na edycje liczby według wzorca
 * np.pattern= "000:00.00" i initValue=123456 wyswietli "012:34.56"
 */

namespace HMI {

class NumberEditor :public NumberField {
public:
	static constexpr uint32_t MAX_TEMP_BUFFER = 12;		// maksymalnie 10 cyfr dla 32-bitowej wartości + ewentualny minus + znak '\0'

protected:
	const char * pattern = emptyPattern;
	//int32_t value = 0;
	static char staticBuffer[MAX_TEMP_BUFFER];

	uint8_t digitsCursor = 0;
	//uint8_t patternCursor = 0;

	// Konwertuje value do bufora wewn. równając do lewej. Zwraca ilość znaków (bez terminującego '\0')
	uint8_t toString(){
		return toString(value, staticBuffer);
		//uint8_t len = charsNumberCount();
		//printValue(&staticBuffer[MAX_TEMP_BUFFER - len -1]);
		//staticBuffer[MAX_TEMP_BUFFER-1] = '\0';
		//return len;
	}

	// obcina value od lewej, zeby mieściło sie w ilości znaków
	bool truncateToChars(uint8_t charsToUse){
		if (charsToUse >= MAX_TEMP_BUFFER) charsToUse = MAX_TEMP_BUFFER-1;
		uint8_t used = charsNumberCount();
		if (used > charsToUse){
			uint32_t maxval = 10;
			while (--charsToUse > 0){
				maxval *= 10;
			}
			value = value % (maxval);
			return true;
		}
		return false;
	}

	// obcina value od lewej albo całość dosuwa do prawej, żeby było dokładnie tyle znaków ile trzeba
	bool normalizeToChars(uint8_t charsToUse){
		if (charsToUse >= MAX_TEMP_BUFFER) charsToUse = MAX_TEMP_BUFFER-1;
		bool result = truncateToChars(charsToUse);
		uint8_t used = charsNumberCount();
		uint8_t offset = charsToUse - used;
		if (offset > 0){
			fill(staticBuffer, ' ', offset);
			printValue(value, staticBuffer + offset);
		}
		return result;
	}


public:
	NumberEditor(const char * fieldPattern, int32_t initValue) : NumberField(initValue){
		//		NumberField::NumberField(initValue);
		setPattern(fieldPattern);
	}
	virtual ~NumberEditor(){;}

	bool setup(const char * fieldPattern, int32_t initValue ){
		value = initValue;
		setPattern(fieldPattern);
		return true;
	}

	void setPattern(const char * fieldPattern){
		pattern = fieldPattern;
	}

	bool print(char * destination);

	//	// Ustawia cyfry w liczbie i przesuwa kursor o 1. Po ostatniej zwraca false;
	//	bool setDigit(uint8_t digit){
	//		return setDigit(digitsCursor, digit);
	//	}
	//
	//	// Ustawia cyfry w liczbie i przesuwa kursor o 1. Po ostatniej zwraca false;
	//	bool setDigit(uint32_t position, uint8_t digit){
	//		uint8_t spaceCount = getUsedCharsQuantity();
	//		if (position >= spaceCount) position =  spaceCount-1;
	//		digitsCursor = position;
	//		if (digit > 9){ 		// >9 oznacza zmiane znaku 'minus' liczby
	//			value = - value;
	//		}else{
	//			uint32_t pow = getPower10(spaceCount - 1 - position);
	//			uint32_t nowaCyfra = pow * digit;
	//			int32_t reszta = value % pow;
	//			pow *= 10;
	//			int32_t mnoznik = (value / pow)*pow;
	//			value = mnoznik + reszta + nowaCyfra;
	//		}
	//		digitsCursor++;
	//		return digitsCursor >= spaceCount;
	//	}

	// Wstawia cyfrę na koniec a całość przesuwa w lewo;
	bool setDigit(uint8_t digit){
		uint8_t spaceCount = getUsedCharsQuantity();
		if (digit > 9){ 		// >9 oznacza zmiane znaku 'minus' liczby
			value = - value;
		}else{
			uint32_t pow = getPower10(spaceCount);
			value = (value * 10) + digit;
			value %= pow;
		}
	}

	uint32_t getUsedCharsQuantity(){
		uint32_t result = countChars(digitChar);
		result += countChars(digitCharTrail);
		return result;
	}

	bool setCursorAt(uint8_t position){
		uint32_t size = getUsedCharsQuantity();
		digitsCursor = position < size ? position : size;
		return position < size;
	}

	bool cursorBack(){
		if (digitsCursor > 0){
			digitsCursor--;
			return false;
		}
		return true;
	}

	bool cursorForward(){
		uint32_t size = getUsedCharsQuantity();
		if (digitsCursor < size){
			digitsCursor++;
		}
		return (digitsCursor >= size);
	}

	bool isCursorAtEnd(){
		return (digitsCursor >= getUsedCharsQuantity());
	}

	uint32_t getFieldSize(){
		const char * patt = pattern;
		return strlen(patt);
	}

	// Zlicza ilość wystąpienia znaku w ciągu
	uint32_t countChars(char znak){
		const char * str = pattern;
		return countChars(znak, pattern);
	}

	//********** statyczne metody klasy

	// Konwertuje value do bufora. Zwraca ilość znaków (bez terminującego '\0')
	static uint8_t toString(uint32_t val, char * destination){
		uint8_t len = charsNumberCount(val);
		printValue(val, destination);
		destination[len] = '\0';
		return len;
	}

	// Zlicza ilość wystąpienia znaku w ciągu
	static uint32_t countChars(char znak, const char * chars){
		uint32_t result = 0;
		while(*chars != '\0'){
			if (*chars++ == znak) result++;
		}
		return result;
	}

	// liczy potęgi liczby 10;
	static uint32_t getPower10 (uint8_t power){
		uint32_t result = 1;
		while (power-- > 0){
			result *= 10;
		}
		return result;
	}

};

} /* namespace HMI */

#endif /* NUMBEREDITOR_H_ */
