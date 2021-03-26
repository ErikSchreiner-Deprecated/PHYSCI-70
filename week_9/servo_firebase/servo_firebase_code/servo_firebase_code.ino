
#include <WiFi.h>                                 // esp32 library
#include <FirebaseESP32.h>                        // firebase library
#include "esp32-hal-ledc.h"

#define FIREBASE_HOST "https://esp-32-servo-default-rtdb.firebaseio.com/"  // the project name address from firebase id
#define FIREBASE_AUTH "i33PYvQrrDyKhZ8Pkdr1w4atYu0tb0Qx1O8rmABd"                          // the secret key generated from firebase
#define WIFI_SSID "HOME-AC7D-2.4"                                // input your home or public wifi name
#define WIFI_PASSWORD "A4HD9E4EACPDFJ33"                            // password of wifi ssid

int fireInt = 0;                                          // led status received from firebase
const int ServoPin = 32;

// setting PWM properties
const int ServoChannel = 0;
const int resolution = 12;   //use 12 bits of resolution

//Define FirebaseESP32 data object
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(ServoPin, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                          // try to connect with wifi

  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                // print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                  // connect to firebase
  Firebase.reconnectWiFi(true);
  Firebase.set(firebaseData, "/SERVO_ANGLE", fireInt);              // set initial string of "OFF"

  // configure PWM functionalitites
  ledcSetup(ServoChannel, 50, 16); // 50 hz PWM, 16-bit resolution
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ServoPin, ServoChannel);
}

void loop() {

  Firebase.get(firebaseData, "/SERVO_ANGLE");                     // get led status input from firebase
  fireInt = firebaseData.intData();                        // change to e.g. intData() or boolData()
  Serial.println(fireInt);

  int ServoValue = map(fireInt, 0, 180, 0, 4095);

  Serial.println(ServoValue);
  ledcWrite(ServoChannel, ServoValue);    

  delay(1000);                                 // not strictly necessary
}
