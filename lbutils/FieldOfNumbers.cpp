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
	for(int8_t i = MAX_TEMP_BUFFER-1; i > 0; i--){
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
		str++;
	}
	return result;
}

bool FieldOfNumbers::print(char * destination){
	char tempBuffer[MAX_TEMP_BUFFER];
	uint32_t digitsOrdinal = countChars(digitChar);
	uint32_t digitsTrail = countChars(digitCharTrail);
	uint8_t usedDigits = fillTempBuffer(tempBuffer);

	const char * patt = pattern;
	int32_t bufOffset = MAX_TEMP_BUFFER - (digitsOrdinal + digitsTrail);
	while(*patt != '\0'){
		if(*patt == digitsTrail){
			*destination = (bufOffset < 0) ? ' ' : tempBuffer[bufOffset];
		}else if(*patt == digitsOrdinal){
			*destination = (bufOffset < 0) ? '0' : tempBuffer[bufOffset];
		}else{
			*destination = *patt;
		}
		patt++;
		destination++;
		bufOffset++;
	}
	return true;
}

//bool FieldOfNumbers::printNumbersWithPattern(){
//			char buff[MAX_TEMP_BUFFER];
//			if (strlen(pattern) >= MAX_TEMP_BUFFER) return false;
//			bool result = numberWithPattern(pattern, value, (char*)buff);
//			print((const char*)buff);
//			return result;
//		}
