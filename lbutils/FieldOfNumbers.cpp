/*
 * FieldOfNumbers.cpp
 *
 *  Created on: 28 paź 2019
 *      Author: lechu
 */

#include <FieldOfNumbers.h>

FieldOfNumbers::FieldOfNumbers() {
	// TODO Auto-generated constructor stub

}

FieldOfNumbers::~FieldOfNumbers() {
	// TODO Auto-generated destructor stub
}

uint8_t FieldOfNumbers::fillTempBuffer(char * buf){
	int32_t x = value;
	bool minus = value < 0;
	if (minus) x = -x;
	for(int8_t i = MAX_TEMP_BUFFER-1; i > 0; i++){
		uint8_t div10 = x % 10;
		x = x/10;
		buf[i] = div10 + '0';
		if (x == 0){
			i++;
			while(i >= 0){
				buf[i] = '0';
			}
		}
	}
	if (minus) buf[0] = '-';
//	buf[0] = minus ? '-' : '0';
}

uint32_t FieldOfNumbers::countChars(char znak){
	const char * str = pattern;
	uint32_t result = 0;
	while(*str != '\0'){
		if (*str == znak) result++;
	}
	return result;
}

bool FieldOfNumbers::print(){
	char tempBuffer[MAX_TEMP_BUFFER];
	uint8_t digitsOrdinal = countChars(digitChar);
	uint8_t digitsTrail = countChars(digitCharTrail);
	uint8_t usedDigits = fillTempBuffer(tempBuffer);


	char * txt = pattern;
	while (*txt != '\0'){ txt++; }		// wyszukanie konca wzoru
	while(txt > pattern){				// dzialanie od konca do poczatku
		txt--;
		if  ((*txt == patternChar)||(*txt == patternCharInv)) {
			uint8_t cyfra = (uint8_t)(initValue % 10);
			initValue /= 10;
			if ((cyfra == 0) && (initValue == 0)){
				if (*txt == patternCharInv)	*txt = ' ';	// spacja dla niewidocznych zer
			}
			else *txt = (char)(cyfra + '0');

		}	// else{ ;} // nic nie zapisuj, tylko przesun
	}
	return initValue == 0;
}
//bool FieldOfNumbers::printNumbersWithPattern(){
//			char buff[MAX_TEMP_BUFFER];
//			if (strlen(pattern) >= MAX_TEMP_BUFFER) return false;
//			bool result = numberWithPattern(pattern, value, (char*)buff);
//			print((const char*)buff);
//			return result;
//		}
