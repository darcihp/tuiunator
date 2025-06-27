#include <esp_now.h>
#include <WiFi.h>
#include <Tone32.h>

//#define BUZZER_PIN 4
//#define BUZZER_CHANNEL 0

//ToneESP32 buzzer(BUZZER_PIN, BUZZER_CHANNEL);

/*
Constantes
*/
const int vrx = 34;
const int vry = 35;
const int btn_1_h = 27;
const int btn_2_h = 26;

//const int out_1 = 12;

int vrx_read;
int vrx_max_read;
int vrx_min_read;

int vry_read;
int vry_max_read;
int vry_min_read;


uint8_t broadcastAddress[] = { 0xC8, 0x2E, 0x18, 0xF7, 0xA3, 0x28 };

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
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Calibration...");

  for (int i = 0; i < 1000; i++) {
    vrx_read = analogRead(vrx);
    delay(1);
    if (i == 0) {
      vrx_max_read = vrx_read;
      vrx_min_read = vrx_read;
    } else {
      if (vrx_read > vrx_max_read) {
        vrx_max_read = vrx_read;
      }
      if (vrx_read < vrx_min_read) {
        vrx_min_read = vrx_read;
      }
    }
  }

  for (int i = 0; i < 1000; i++) {
    vry_read = analogRead(vry);
    delay(1);
    if (i == 0) {
      vry_max_read = vry_read;
      vry_min_read = vry_read;
    } else {
      if (vry_read > vry_max_read) {
        vry_max_read = vry_read;
      }
      if (vry_read < vry_min_read) {
        vry_min_read = vry_read;
      }
    }
  }

  //TODO
  //vrx_min_read = vrx_min_read - 50;
  //vrx_max_read = vrx_max_read + 50;
  //Serial.println(vrx_max_read);
  //Serial.println(vrx_min_read);

  pinMode(btn_1_h, INPUT_PULLUP);
  pinMode(btn_2_h, INPUT_PULLUP);
  //pinMode(out_1, OUTPUT);

  //digitalWrite(out_1, LOW);
  
}

void loop() {

  int vrx_map = 90;
  int vry_map = 0;

  vrx_read = analogRead(vrx);
  delay(1);
  vry_read = analogRead(vry);
  delay(1);

  if (vrx_read >= vrx_min_read && vrx_read <= vrx_max_read) {
    vrx_map = 90;
  } else {
    if (vrx_read < vrx_min_read) {
      vrx_map = map(vrx_read, 0, vrx_min_read, 0, 89);
    } else {
      if (vrx_read > vrx_max_read) {
        vrx_map = map(vrx_read, vrx_max_read, 4095, 91, 180);
      }
    }
  }

  if (vry_read >= vry_min_read && vry_read <= vry_max_read) {
    vry_map = 0;
  } else {
    if (vry_read < vry_min_read) {
      vry_map = map(vry_read, 0, vry_min_read, -90, 0);
    } else {
      if (vry_read > vry_max_read) {
        vry_map = map(vry_read, vry_max_read, 4095, 0, 90);
      }
    }
  }

  myData.vrx = vrx_map;
  myData.vry = vry_map;

  
  bool btn_1_h_r = digitalRead(btn_1_h);
  if (btn_1_h_r) { 
    myData.btn_1 = false;

  } else {
    myData.btn_1 = true;
  }

  bool btn_2_h_r = digitalRead(btn_2_h);
  if (btn_2_h_r) { 
    myData.btn_2 = false;

  } else {
    myData.btn_2 = true;
  }

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

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