#define US_KEYBOARD 1

// Rotary
#define LEDCW  5
#define ROTA   33
#define ROTB   32
#define ROTBUT 23
#define LEDTOG 34
// Status LED
#define LEDPIN 22
#define LEDDELAY 1500
#define OLED_RESET -1
// BT Name / Message
#define MESSAGE "Hello from your shota\n"
#define DEVICE_NAME "Haiiiii"
// Brightness Control
#define Consumer_BrightnessUp CONSUMER_KEY(0x6f, HID_TYPE_OSC)
#define Consumer_BrightnessDown CONSUMER_KEY(0x70, HID_TYPE_OSC)
// Voltage divider input
#define VIN 4

// Globals

SimpleRotary rotary(ROTA, ROTB, ROTBUT);
AsyncDelay ledDelay;
boolean ledON = true;
BleKeyboard bleKeyboard;
OLED display = OLED(12,14,NO_RESET_PIN,OLED::W_128,OLED::H_32);
bool ledTog = 0;
const uint8_t BRIGHTNESS_UP = 0x6F;
const uint8_t BRIGHTNESS_DOWN = 0x70;

pthread_t ledThread, rotaryThread, displayThread;