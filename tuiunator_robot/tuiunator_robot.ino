#include <esp_now.h>
#include <WiFi.h>
#include <Deneyap_Servo.h>

Servo myservo;
bool connection;


typedef struct struct_message {
    int vrx;
    int vry;
    bool btn_1;
    bool btn_2;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  connection = true;
  memcpy(&myData, incomingData, sizeof(myData));
  /*
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("vrx: ");
  Serial.println(myData.vrx);
  Serial.print("vry: ");
  Serial.println(myData.vry);
  Serial.print("btn_1: ");
  Serial.println(myData.btn_1);
  Serial.print("btn_2: ");
  Serial.println(myData.btn_2);
  Serial.println();
  */
  myservo.write(myData.vrx);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  myservo.attach(5);

  connection = false;
}
 
void loop() {
  if(connection == false)
  {
    myservo.write(90);
  }
  connection = false;
  delay(100);
}