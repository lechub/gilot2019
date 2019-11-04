
// ----------------------------------------------------------------------------


#include "diag/Trace.h"

#include "QuickTask.h"
#include "main.h"
#include "Hardware.h"
#include "VEprom.h"
#include "HD44780.h"
#include "Praca.h"
#include "Menu.h"

void keysPoll();


Pinout pinout = Pinout();
Pinout * pins = &pinout;

Keyboard keyboard = Keyboard(&pins->keyb_K1, &pins->keyb_K2, &pins->keyb_K3,
		&pins->keyb_W1, &pins->keyb_W2, &pins->keyb_W3, &pins->keyb_W4);
Keyboard * keys = &keyboard;

HD44780 *hd44780 = HD44780::getInstance();
FrameBuffer * lcd = hd44780->getFrameBuffer(); //nullptr;

Krokowy krok = Krokowy(&pins->mot_step, &pins->mot_dir, &pins->mot_enable,
		&pins->mot_sleep, &pins->mot_ms1, &pins->mot_ms2 );

Krokowy * krokowy = &krok;

Knife noz = Knife(&pins->noz_Action, &pins->noz_detect, Menu::POLL_PERIOD_MS );

Knife * knife = &noz;


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


void keysPoll(){		// klawiatura przez HMI
	keys->poll();
}
QuickTask keybTask(QuickTask::QTType::QT_PERIODIC, keysPoll, Keyboard::POLL_PERIOD_MS);


Menu menu = Menu();
void menuPoll(){		// klawiatura przez HMI
	menu.poll();
	knife->poll();
}
QuickTask menuTask(QuickTask::QTType::QT_PERIODIC, menuPoll, Menu::POLL_PERIOD_MS);


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

	keys->setup();

	Praca::getInstance()->setup();

	krokowy->setup();
	knife->setup();

	QuickTask::hold(false);
	lcd->printXY(0,0,"1234567890");
	lcd->homeScreen();
	lcd->forceRefresh();

	menu.init(lcd);

	while (true)
	{
		QuickTask::poll();

	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
