
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "QuickTask.h"
#include "Pinout.h"
<<<<<<< HEAD
#include "Hardware.h"
=======
#include "Keyboard.h"
>>>>>>> branch 'master' of git@github.com:lechub/gilot2019.git



Pinout pinout = Pinout();
Pinout * pins = &pinout;
Keyboard keyboard = Keyboard(&pins->keyb_K1, &pins->keyb_K2, &pins->keyb_K3,
		&pins->keyb_W1, &pins->keyb_W2, &pins->keyb_W3, &pins->keyb_W4);
Keyboard * keys = &keyboard;

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

<<<<<<< HEAD

Pinout pinout = Pinout();
Pinout * pins = &pinout;



int
main(int argc, char* argv[])
{
  pins->init();
  Hardware::init();
=======
int main(int argc, char* argv[]){
	pins->setup();
	keys->setup();
>>>>>>> branch 'master' of git@github.com:lechub/gilot2019.git

<<<<<<< HEAD
  while (true)
  {
    QuickTask::poll();
=======
	// Infinite loop
	while (true)
	{
		QuickTask::poll();
>>>>>>> branch 'master' of git@github.com:lechub/gilot2019.git

<<<<<<< HEAD

  }
=======
	}
>>>>>>> branch 'master' of git@github.com:lechub/gilot2019.git
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
