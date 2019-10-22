//hd44780.h 


#include <stdint.h>
#include "Gpio.h"
#include "FrameBuffer.h"

//HD44780 command summary
/*
clear display 	                                0x01
Return cursor to home, and un-shift display 	0x02
move cursor right, don’t shift display 	        0x04
move cursor right, do shift display (left) 	0x05
move cursor right, don’t shift display  	0x06
move cursor right, do shift display (left) 	0x07
turn display off 	                        0x08
display on, cursor off, 	                0x0C
display on, cursor on, steady cursor 	        0x0E
display on, cursor on, blinking cursor 	        0x0F
shift cursor left 	                        0x10
shift cursor right 	                        0x14
shift display left 	                        0x18
shift display right 	                        0x1C
set cursor position                             0x80+position
 */

class HD44780{
public:
  //some commands that are defined
  //#define SET_DDRAM_ADDR  0x80    // must "OR" in the address
  //#define RETURN_HOME     0x02
  //#define CLEAR_DISPLAY   0x01

  //	static constexpr uint16_t COLUMNS = 16;
  //	static constexpr uint16_t ROWS = 2;
  //	static constexpr uint16_t LCD_DATA_AMOUNT = HD44780::COLUMNS * HD44780::ROWS +5;


  //assumes timing specified at LCD Vdd=4.4-5.5v

  //#define CURSOR_VISIBLE 0 //Set 1 for visible cursor,  0 for invisible
  //#define CURSOR_BLINK   0 //Set 1 for blinking cursor, 0 for continuous
  //
  //#define CMD_BYTE  0x00
  //#define CHAR_BYTE 0x01

  //	static constexpr uint8_t CURSOR_VISIBLE = 0; //Set 1 for visible cursor,  0 for invisible
  //	static constexpr uint8_t CURSOR_BLINK   = 0; //Set 1 for blinking cursor, 0 for continuous

  //	static constexpr uint8_t CMD_BYTE  = 0x00;
  //	static constexpr uint8_t CHAR_BYTE = 0x01;



  static constexpr  uint8_t LCD_DDRAM_ADDR = 0x80;    // must "OR" in the address
  static constexpr  uint8_t LCD_CGRAM_ADDR = 0x40;  // Start address of LCD CGRAM
  static constexpr  uint8_t LCD_CGMAX   = 64;     // Max CGRAM bytes

  static constexpr  uint8_t LCD_COLUMNS   = 16;
  static constexpr  uint8_t LCD_ROWS   = 2;
  static constexpr  uint8_t LCD_CHARS_AMOUNT  = LCD_COLUMNS * LCD_ROWS + 1;

  static constexpr  uint8_t LCD_DDRAM_WRITE = 0x80;
  static constexpr  uint8_t LCD_L1  = 0x00; //0x80;
  static constexpr  uint8_t LCD_L2  = 0x40; //C0;
  static constexpr  uint8_t LCD_L3  = 0x10;
  static constexpr  uint8_t LCD_L4  = 0x50;

  static constexpr  uint8_t CMD = 0x00;
  static constexpr  uint8_t DATA = 0b01000000;

  static constexpr  uint32_t LCD_REFRESH_INTERVAL_MILISECOND  = 150;


  typedef enum {
    LCD_CLR   = 0x01,   // LCD Return home
    LCD_HOME  = 0x02,   // LCD Return home
    LCD_ENTRY   = 0x06,   // Set LCD Entry Mode
    LCD_C2L   = 0b010000,   // Move Cursor to the left
    LCD_C2R   = 0b010100,   // Move Cursor to the right
    LCD_D2L   = 0x18,   // Move display to the left
    LCD_D2R   = 0x1C,   // Move display to the right

    LCD_ON    = 0x0E,   // Turn on LCD and Cursor
    LCD_OFF   = 0x08,   // Turn off LCD
    CURSOR_LINE = 0b01110,    // Turn on cursor - line
    CURSOR_BLINK    = 0b01111,    // Turn on cursor - blink
    CURSOR_INVISIBLE  = 0b01100,    // Turn off cursor
  }LcdCommand;

 typedef struct{
    Gpio * gpios[8];
  }GpioPack8;

private:
  typedef enum{
    START,
    //    WAIT_GOTO_L1,
    GOTO_LINE1,
    //    WAIT_LINE1,
    SEND_LINE1,
    //    WAIT_GOTO_L2,
    GOTO_LINE2,
    //    WAIT_LINE2,
    SEND_LINE2,
  }LcdStage;



  Gpio * gpioE = nullptr;
  Gpio * gpioRW = nullptr;     // 0 - write, 1 - read
  Gpio * gpioRS = nullptr;    // 0 - instrukcja, 1 - dane
  GpioPack8 * gpioData = nullptr;
  Gpio * gpioBackLight = nullptr;
//  Gpio * gpioResetLCD = nullptr;
  uint32_t charOffset = 0;
  LcdStage lcdStage = LcdStage::START;
  bool newLine = true;

private:
  bool localInit();

  // Ustawia dane na liniach danych
  inline void prepareDataByte(uint8_t byteValue){
    for(uint8_t i = 0; i < 8; i++){
      gpioData->gpios[i]->setOutput(byteValue & 0x01);
      byteValue = static_cast<uint8_t>(byteValue >> 1);
    }
  }

  inline void prepareCommandToWrite(LcdCommand command){
    prepareDataByte(static_cast<uint8_t>(command));
    gpioRS->setOutput(false);
    gpioRW->setOutput(false);    // zapis
  }

  inline void prepareCharToWrite(char data){
    prepareDataByte(static_cast<uint8_t>(data));
    gpioRS->setOutput(true);
    gpioRW->setOutput(false);    // zapis
  }

  void writeCommandDirty(LcdCommand cmd, uint32_t delayMs){
    prepareCommandToWrite(cmd);
    gpioE->setOutput(true);
    delayMsDirty(2);
    if (delayMs >= 2) {
      delayMs -= 2;
    }
    gpioE->setOutput(false);
    delayMsDirty(delayMs);
  }

public:
  HD44780(){;}


  bool setup(Gpio * gpioLcdE, Gpio * gpioLcdRW, Gpio * gpioLcdRS, GpioPack8 * gpioLcdD,  Gpio * backLightPin);

  static HD44780 * getInstance();



  bool sendCommandDirty(uint8_t cmd, uint32_t delayMs);
  inline bool sendCommandDirty(LcdCommand cmd, uint32_t delayMs){
    return sendCommandDirty((uint8_t)cmd, delayMs);
  }


  //bool setCmdOrData(bool isCMD, uint8_t byteValue);

  // ToDo dorobić busy bit
  inline bool isBusy(){
    return false; //i2c->isBusy();
  }


  void delayMs(uint32_t milis);
  void delayMsDirty(uint32_t milis);

  char getNextChar();

public:

  //void setResetPin(bool newstate){ gpioResetLCD->setOutput(newstate); }
  void setBackLight(bool newstate){ gpioBackLight->setOutput(newstate); }

  uint8_t getDDRamAdres(uint8_t column, uint8_t row){
      if (column > LCD_COLUMNS) return 0;
      uint32_t offset = 0;
      switch(row){
      case 1: offset = LCD_L2; break;
      case 2: offset = LCD_L3; break;
      case 3: offset = LCD_L4; break;
      case 0:
      default: offset = LCD_L1; break;
      }
      return (LCD_DDRAM_WRITE | offset | column) ;
    }

//  inline bool gotoXY(uint8_t Col, uint8_t Row){
//    if (Col > LCD_COLUMNS) return false;
//    uint32_t offset = 0;
//    switch(Row){
//    case 1: offset = LCD_L2; break;
//    case 2: offset = LCD_L3; break;
//    case 3: offset = LCD_L4; break;
//    case 0:
//    default: offset = LCD_L1; break;
//    }
//    return sendCommand(uint8_t(LCD_DDRAM_WRITE | offset | Col)) ;
//  }

  //  //bool print(char znak);
  //  //bool print(const char * str);
  //  bool clearScreen(void);
  //  bool lcd_Home(void){ return sendCommand(LcdCommand::LCD_HOME); }
  //  bool lcd_ON(){ return sendCommand(LcdCommand::LCD_ON); }
  //  bool lcd_OFF(){ return sendCommand(LcdCommand::LCD_OFF); }
  //  bool cursorLeft(void){ return sendCommand(LCD_C2L); }
  //  bool cursorRight(void){ return sendCommand(LCD_C2R);  }

//  bool cursorSet(FrameBuffer::CursorMode cursorMode){
//    uint8_t cmd = CURSOR_INVISIBLE;
//    switch(cursorMode){
//    case FrameBuffer::CursorMode::DASH: cmd = CURSOR_LINE; break;
//    case FrameBuffer::CursorMode::SOLID: cmd = CURSOR_LINE; break;
//    case FrameBuffer::CursorMode::BLINK: cmd = CURSOR_BLINK; break;
//    case FrameBuffer::CursorMode::HIDDEN:
//    default: break;
//    }
//    sendCommand(cmd);
//    return true;
//  }


  //bool sendCommand(LcdCommand cmd);
  bool sendByte(uint8_t dataByte);
  bool sendLine(uint32_t lineNr);
  void process(); // wywolywac cyklicznie

  FrameBuffer * getFrameBuffer();
  Fifo * getDataFifo();

  Gpio * getBackLight(){ return gpioBackLight; }

  void poll();



};

void pollCallback();


