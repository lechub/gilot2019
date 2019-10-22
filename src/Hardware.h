/*
 * Hardware.h
 *
 *  Created on: 28.06.2017
 *      Author: lechu
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "systemDefs.h"

#include "QuickTask.h"
//#include "Fifo.h"

class Hardware {

public:
	static constexpr uint32_t CPU_FREQUENCY_HZ = HSE_VALUE ;//8000000UL;

	static constexpr uint32_t nvicPriority = 0x03U;
	static constexpr uint32_t SYSTICK_FREQUENCY_HZ = 1000;

	typedef enum{
		OK, Incident, Failure, DMA_FAIL, ADC_FAIL,
	}ErrorCode;


	Hardware(){	}

	static void init();

	static uint32_t getTickMs();
	static inline void delayMsDirty(uint32_t milis){
		QuickTask::delayMsWithStoppedTasks(milis);
	}
	static inline void delayMsWithBackground(uint32_t milis){
		QuickTask::delayMsWithActiveTasks(milis);
	}

	static void WDOG_Init();
	static void WDOG_Reload();
	static void NVIC_init();
	static bool adcDmaError;


	static void errorDispatch(ErrorCode errCode){
		switch(errCode){
		case Failure:
			while (true){;}// ToDo mruganie dioda
			break;
		case Incident: break;
		//		case DMA_FAIL: adcDmaError = true; break;
		//		case ADC_FAIL: adcDmaError = true; break;
		case OK:
			// Todo zasygnalizowanie
			break;
		}
	}

private:
	static void gpioInit();
	//static void adcInit();

};

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* HARDWARE_H_ */
