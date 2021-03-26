#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0xC0, 0x26, 0x28};

int myDataOne = 0;
int myDataTwo = 0;
int dutyCycleOne = 0;
int dutyCycleTwo = 0;
int myDataBuzzer =0;
int myDataLED = 0;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
void loop() {
  dutyCycleOne = analogRead(32);
  dutyCycleTwo = analogRead(33);
  
  myDataOne = map(dutyCycleOne, 0, 4095, 0, 255);
  myDataTwo = map(dutyCycleTwo, 0, 4095, 0, 255);
  myDataBuzzer = digitalRead(23);
  myDataLED = digitalRead(22);
  
  int dataArray[4] = {myDataOne, myDataTwo, myDataBuzzer, myDataLED};
  esp_err_t resultOne = esp_now_send(broadcastAddress, (uint8_t *) &dataArray, sizeof(dataArray));
  Serial.print(myDataOne);
  Serial.print("\t");
  Serial.print(myDataTwo);
  Serial.print("\t");
  Serial.print(myDataBuzzer);
  Serial.print("\t");
  Serial.print(myDataLED);
  Serial.print("\t");
  Serial.print(dutyCycleOne);
  Serial.print("\t");
  Serial.print(dutyCycleTwo);
  Serial.println();

//  if (result == ESP_OK) {
//    Serial.println("Sent with success");
//    Serial.println(myDataOne);
//  }
//  else {
//    Serial.println("Error sending the data");
//  }
  delay(10);
}
