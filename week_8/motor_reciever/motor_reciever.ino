#include <esp_now.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//buzzer
#include <Arduino.h>
#include <analogWrite.h>

int buzzerPin = 13;
int channel = 1;
int freq = 1000;
int resolution = 8;
int freqOff = 0;


//LED
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN    4
#define LED_COUNT 10
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
unsigned long currentMillis = millis();
unsigned long previousMillis;
int timeOn;

//Motor
const int motorOnePinOne = 2;
const int motorOnePinTwo = 16;
const int motorTwoPinOne = 12;
const int motorTwoPinTwo = 15;

//ESP Now
int dataArray[4];

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&dataArray, incomingData, sizeof(dataArray));
//  Serial.print("Bytes received: ");
//  Serial.println(len);
//  Serial.print("byte: ");
//  Serial.println(myData);
}

//--------------------------------------------------------------------
void setup() {

  //LED
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)


  //Buzzer
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(buzzerPin, channel);


  //Motor
  pinMode(motorOnePinOne, OUTPUT);
  pinMode(motorOnePinTwo, OUTPUT);
  pinMode(motorTwoPinOne, OUTPUT);
  pinMode(motorTwoPinTwo, OUTPUT);

  //ESP Now
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}


//--------------------------------------------------------------------
void loop() {
  //Buzzer
  if (dataArray[2] < 1)
  {
  ledcWriteTone(channel, freqOff);
  }
  else 
  {
  ledcWriteTone(channel, freq);
  }

  //LED
  rainbow();

  //Motor
  analogWrite(motorOnePinOne, dataArray[0]);
  digitalWrite(motorOnePinTwo, LOW);
  analogWrite(motorTwoPinOne, dataArray[1]);
  digitalWrite(motorTwoPinTwo, LOW);

  //Print in Serial Monitor Incoming Data
  Serial.print(dataArray[0]);
  Serial.print("\t");
  Serial.print(dataArray[1]);
  Serial.print("\t");
  Serial.print(dataArray[2]);
  Serial.print("\t");
  Serial.print(dataArray[3]);
  Serial.println();
  delay(100);
}


//Rainbow Function-------------------------------------------------------
void rainbow(){
  if(dataArray[3] > 0){
    for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
      for(int i=0; i<strip.numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
      strip.show();
    }
  }
  else if (dataArray[3] < 1){
    strip.clear();
  }
}
