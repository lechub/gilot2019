/*
 * FieldOfNumbers.h
 *
 *  Created on: 28 paź 2019
 *      Author: lechu
 */

#ifndef FIELDOFNUMBERS_H_
#define FIELDOFNUMBERS_H_

#include "Settings.h"

/**
 * Wyswietla i pozwala na edycje liczby według wzorca
 * np.pattern= "000:00.00" i initValue=123456 wyswietli "012:34.56"
 */

class FieldOfNumbers {
public:
	static constexpr uint32_t MAX_TEMP_BUFFER = 12;		// maksymalnie 10 cyfr dla 32-bitowej wartości + ewentualny minus + znak '\0'
	static constexpr char digitChar	= '0';			// oznacza 1 cyfre
	static constexpr char digitCharTrail = '#';			// oznacza cyfre lub wiodącą spację
	static constexpr char * emptyPattern = "";			// pusty pattern - dla inicjacji

private:
	const char * pattern = emptyPattern;
	int32_t value = 0;
protected:
	bool printNumbersWithPattern();
	uint8_t fillTempBuffer(char * buf);
	uint32_t countChars(char znak);
public:
	FieldOfNumbers();
	virtual ~FieldOfNumbers();

	bool setup(const char * fieldPattern, int32_t initValue );
	bool print(char * destination);
	bool setNumber(uint8_t digit);
	bool setNumber(uint32_t position, uint8_t digit);
	bool backspace();
	uint32_t getValue();

};

#endif /* FIELDOFNUMBERS_H_ */
