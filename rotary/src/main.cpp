#define US_KEYBOARD 1

#include <BleKeyboard.h>
#include <AsyncDelay.h>
#include <SimpleRotary.h>
#include <oled.h>

#define LEDCW  5
#define ROTA   33
#define ROTB   32
#define ROTBUT 23
SimpleRotary rotary(ROTA, ROTB, ROTBUT);

#define LEDPIN 22
#define LEDDELAY 1500
AsyncDelay ledDelay;
boolean ledON = true;

// Change the below values if desired
#define MESSAGE "Hello from your shota\n"
#define DEVICE_NAME "Haiiiii"

BleKeyboard bleKeyboard;
OLED display = OLED(12,14,NO_RESET_PIN,OLED::W_128,OLED::H_32);
#define LEDTOG 32
bool ledTog = 0;

// EXPERIMENTAL
#define Consumer_BrightnessUp CONSUMER_KEY(0x6f, HID_TYPE_OSC)
#define Consumer_BrightnessDown CONSUMER_KEY(0x70, HID_TYPE_OSC)
const uint8_t BRIGHTNESS_UP = 0x6F;
const uint8_t BRIGHTNESS_DOWN = 0x70;

#define VIN 4


// toggles LED at set interval (ms)
void btStatus(int pin)
{
  if (ledON || bleKeyboard.isConnected()){
    digitalWrite(pin, LOW);
    ledDelay.repeat();
    ledON = false;
  } else { // LED is OFF
    digitalWrite(pin, HIGH);
    ledDelay.repeat();
    ledON = true;
  }
}

void rotaryTest()
{
  byte r = rotary.rotate();

  // CW
	if ( r == 1 ) {
		Serial.println("CW");
    digitalWrite(LEDCW, HIGH);
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
	} else if ( r == 2 ) {
		Serial.println("CCW");
    digitalWrite(LEDCW, LOW);
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
	}

  byte p = rotary.push();
	
	if ( p == 1 ){
		Serial.println("Pushed");
    bleKeyboard.write(KEY_MEDIA_MUTE);
		if ( ledTog ){
      ledTog = 0;
    digitalWrite(LEDTOG, LOW);
    } else {
      ledTog = 1;
    digitalWrite(LEDTOG, HIGH);
    }
	}
}

/// YoU cAn'T rEaD bAtTeRy LeVeL iN wEmOs BoArDs
/// VIN pin needs to be connected to Vin (duh) with a voltage divider,
/// as the input needs to be in the 0.1V to 3.2V range and Li-Ion batteries are like 3 - 4.2V.
/// Additionally, it could be combined with a transistor (or just use very high resistor values),
/// to reduce power consumption (mmm voltage divider, power yummy)
int getBatteryLevel()
{
  uint16_t read = analogRead(VIN);
  return (read / 4095.0) * 100;
}

void initPins()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(LEDCW, OUTPUT);
  pinMode(LEDTOG, OUTPUT);
  pinMode(VIN, ANALOG);
  digitalWrite(LEDPIN, HIGH);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Script running :3");

  initPins();

  display.begin();
  display.display();
  display.setTTYMode(true);
  display.printf("gamo to spiti :(");
  bleKeyboard.setName(DEVICE_NAME);
  bleKeyboard.begin();
  bleKeyboard.setBatteryLevel(69);

  ledDelay.start(LEDDELAY, AsyncDelay::MILLIS);
  
  Serial.println("Script running :3");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (ledDelay.isExpired()) {
    btStatus(LEDPIN);
  }
  rotaryTest();
}
