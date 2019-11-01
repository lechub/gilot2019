/*
 * FieldOfNumbers.cpp
 *
 *  Created on: 28 paź 2019
 *      Author: lechu
 */

#include <NumberEditor.h>

namespace HMI {



//NumberEditor::NumberEditor() {
//	// TODO Auto-generated constructor stub
//
//}
//
//NumberEditor::~NumberEditor() {
//	// TODO Auto-generated destructor stub
//}

//uint8_t FieldOfNumbers::fillTempBuffer(char * buf){
//	int32_t x = value;
//	bool minus = value < 0;
//	if (minus) x = -x;
//	for(int8_t i = MAX_TEMP_BUFFER-1; i > 0; i--){
//		uint8_t div10 = x % 10;
//		x = x/10;
//		buf[i] = div10 + '0';
//		if (x == 0){
//			if (minus){
//				buf[--i] = '-';
//			}
//			while(--i >= 0){
//				buf[i] = '0';
//			}
//		}
//	}
//	//	if (minus) buf[0] = '-';
//}



bool NumberEditor::print(char * destination){
	//char tempBuffer[MAX_TEMP_BUFFER];
	uint32_t digitsOrdinal = countChars(digitChar);
	uint32_t digitsTrail = countChars(digitCharTrail);
	uint8_t usedChars = toString();
	normalizeToChars(digitsOrdinal + digitsTrail);

	const char * patt = pattern;
	int32_t bufOffset = 0;
	while(*patt != '\0'){
		if(*patt == digitsTrail){
			*destination = staticBuffer[bufOffset++];
		}else if(*patt == digitsOrdinal){
			*destination = staticBuffer[bufOffset] == ' ' ? '0' : staticBuffer[bufOffset];
			bufOffset++;
		}else{
			*destination = *patt;
		}
		patt++;
		destination++;
	}
	return true;
}
/**
 const char * patt = pattern;
	int32_t bufOffset = MAX_TEMP_BUFFER - (digitsOrdinal + digitsTrail);
	while(*patt != '\0'){
		if(*patt == digitsTrail){
			*destination = (bufOffset < 0) ? ' ' : tempBuffer[bufOffset];
			bufOffset++;
		}else if(*patt == digitsOrdinal){
			*destination = (bufOffset < 0) ? '0' : tempBuffer[bufOffset];
			bufOffset++;
		}else{
			*destination = *patt;
		}
		patt++;
		destination++;
	}
	return true;
 */

} /* namespace HMI */

