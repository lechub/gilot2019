/*
 * FieldOfNumbers.cpp
 *
 *  Created on: 28 paź 2019
 *      Author: lechu
 */

#include <NumberEditor.h>

namespace HMI {

char NumberEditor::staticBuffer[MAX_TEMP_BUFFER];

bool NumberEditor::print(char * destination){
	//char tempBuffer[MAX_TEMP_BUFFER];
	uint32_t digitsOrdinal = countChars(digitChar);
	uint32_t digitsTrail = countChars(digitCharTrail);
	uint8_t usedChars = toString();
	normalizeToChars(digitsOrdinal + digitsTrail);

	const char * patt = pattern;
	int32_t bufOffset = 0;
	while(*patt != '\0'){
		if(*patt == digitCharTrail){
			*destination = staticBuffer[bufOffset++];
		}else if(*patt == digitChar){
			*destination = staticBuffer[bufOffset] == ' ' ? '0' : staticBuffer[bufOffset];
			bufOffset++;
		}else{
			*destination = *patt;
		}
		patt++;
		destination++;
	}
	*destination = '\0';
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

