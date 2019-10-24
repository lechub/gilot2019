
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "QuickTask.h"
#include "Pinout.h"
#include "Hardware.h"
#include "Keyboard.h"
#include "VEprom.h"
#include "HD44780.h"


Pinout pinout = Pinout();
Pinout * pins = &pinout;

Keyboard keyboard = Keyboard(&pins->keyb_K1, &pins->keyb_K2, &pins->keyb_K3,
		&pins->keyb_W1, &pins->keyb_W2, &pins->keyb_W3, &pins->keyb_W4);
Keyboard * keys = &keyboard;

HD44780 *hd44780 = HD44780::getInstance();
FrameBuffer * lcd = hd44780->getFrameBuffer(); //nullptr;

HD44780::GpioPack8 gpioP8 = {
		&pins->lcd_D0,
		&pins->lcd_D1,
		&pins->lcd_D2,
		&pins->lcd_D3,
		&pins->lcd_D4,
		&pins->lcd_D5,
		&pins->lcd_D6,
		&pins->lcd_D7
};

void keysPoll(){
	keys->poll();
}
QuickTask keybTask(QuickTask::QTType::QT_PERIODIC, keysPoll, Keyboard::TIME_PERIOD_KEYB_MS);

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


int main(int argc, char* argv[])
{
	Hardware::init();
	QuickTask::hold(true);
	VEprom::init();

	pins->setup();

	hd44780->setup(&pins->lcd_E, &pins->lcd_RW, &pins->lcd_RS, &gpioP8, &pins->lcd_bckLight);
	//lcd = hd44780.getFrameBuffer();

	keys->setup();

	QuickTask::hold(false);
	lcd->printXY(0,0,"1234567890");


	while (true)
	{
		QuickTask::poll();

	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
