#include <esp_now.h>
#include <WiFi.h>

/*
Constantes
*/
const int vrx = 34;
const int vry = 35;

int temp_read = 0;

uint8_t broadcastAddress[] = {0xB0, 0xA7, 0x32, 0xDC, 0xE9, 0x90};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  //char a[32];
  int vrx;
  int vry;
  bool btn_1;
  bool btn_2;
  //float c;
  //bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  //strcpy(myData.a, "THIS IS A CHAR");
  //myData.b = random(1,20);
  //myData.c = 1.2;
  //myData.d = false;
  
  
  myData.vrx = analogRead(vrx);
  myData.vry = analogRead(vry);

  myData.btn_1 = false;
  myData.btn_2 = true;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  /*
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  */
  
  delay(10);
}