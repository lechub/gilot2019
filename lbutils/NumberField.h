/*
 * NumberField.h
 *
 *  Created on: 1 lis 2019
 *      Author: lechu
 */

#ifndef NUMBERFIELD_H_
#define NUMBERFIELD_H_

#include <cstdint>
#include <cstdlib>

namespace HMI {

// Holds one value and converts it to string. Uses minus.
class NumberField {
public:
	static constexpr char digitChar	= '0';			// oznacza 1 cyfre
	static constexpr char digitCharTrail = '#';			// oznacza cyfre lub wiodącą spację
	static constexpr char * emptyPattern = "";			// pusty pattern - dla inicjacji
	static constexpr int	numberBase = 10;

protected:
	int32_t value = 0;
	//static char staticBuffer[sizeof(int)*8+1];

public:
	NumberField(int32_t initValue){
		setValue(initValue);
	}

	virtual ~NumberField(){;}

	virtual uint32_t getValue()const{
		return value;
	}

	virtual void setValue(uint32_t newValue){
		value = newValue;
	}

	//Wypisuje wartość w postaci stringa zakończonego '\0'
	virtual void printValue(char * destination){
		printValue(value, destination);
	}

	//Wypisuje wartość w postaci stringa zakończonego '\0'
	static void printValue(int32_t val, char * destination){
		itoa (val, destination, numberBase );
	}

	// Oblicza ilość znaków do stringa włącznie z minusem, ale bez terminującego '\0'
	uint8_t charsNumberCount()const {
		return charsNumberCount(value);
	}

	// Oblicza ilość znaków do stringa włącznie z minusem, ale bez terminującego '\0'
	static constexpr uint8_t charsNumberCount(int32_t x) {
		if (x == static_cast<int32_t>(0x80000000)) return 11;
		uint8_t result = 1;
		if (x < 0){		// +1 żeby policzyć dodoatkowy znak na minus
			result++;
			x = -x;
		}
		if (x < 10) return result;
		result++;
		if (x < 100) return result;
		result++;
		if (x < 1000) return result;
		result++;
		if (x < 10000) return result;
		result++;
		if (x < 100000) return result;
		result++;
		if (x < 1000000) return result;
		result++;
		if (x < 10000000) return result;
		result++;
		if (x < 100000000) return result;
		result++;
		if (x < 1000000000) return result;
		return ++result;
	}

	static inline void fill(char * destination, char znak, uint32_t quantity){
		while(quantity-- > 0){
			*destination = znak;
			destination++;
		}
	}


};

} /* namespace HMI */

/*
 int32_t x = value;
		bool minus = value < 0;
		if (minus) x = -x;
		for(int8_t i = MAX_TEMP_BUFFER-1; i > 0; i--){
			uint8_t div10 = x % 10;
			x = x/10;
			buf[i] = div10 + '0';
			if (x == 0){
				if (minus){
					buf[--i] = '-';
				}
				while(--i >= 0){
					buf[i] = '0';
				}
			}
		}
		//	if (minus) buf[0] = '-';
	}
 *
 * */

#endif /* NUMBERFIELD_H_ */
