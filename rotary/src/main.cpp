#include <BleKeyboard.h>
#include <AsyncDelay.h>
#include <SimpleRotary.h>
#include <oled.h>
#include <pthread.h>
#include "main.h"

// toggles LED at set interval (ms)
void* btStatus(void *arg)
{
  while (1) {
    if (!ledDelay.isExpired()) {
      delay((int) LEDDELAY/10);
      continue;
    }

    if (ledON || bleKeyboard.isConnected()){
      digitalWrite(LEDPIN, LOW);
      ledDelay.repeat();
      ledON = false;
    } else { // LED is OFF
      digitalWrite(LEDPIN, HIGH);
      ledDelay.repeat();
      ledON = true;
    }
  }
  return nullptr;
}

void* rotary_parse(void *arg)
{
  while (1) {
    byte r = rotary.rotate();

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
  return nullptr;
}

void* display_parse(void *arg)
{
  while (1) {
    display.clear();
    display.draw_string(1, 1, "gamo to spiti :(", OLED::NORMAL_SIZE);
    display.draw_string(1, 10, "to spiti gamo :)", OLED::NORMAL_SIZE);
    display.display();
    delay(1000);
  }
  return nullptr;
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
  bleKeyboard.setName(DEVICE_NAME);
  bleKeyboard.begin();
  bleKeyboard.setBatteryLevel(69);

  ledDelay.start(LEDDELAY, AsyncDelay::MILLIS);
  
  pthread_create(&ledThread, NULL, &btStatus, NULL);  
  pthread_create(&rotaryThread, NULL, &rotary_parse, NULL);
  pthread_create(&displayThread, NULL, &display_parse, NULL);
  
  Serial.println("Script running :3");
}

void loop()
{}
