#include <esp_now.h>
#include <WiFi.h>
#include <Deneyap_Servo.h>

Servo myservo_e;
Servo myservo_d;

bool connection;

const int arm_e = 17;
const int arm_d = 19;

typedef struct struct_message {
  //char a[32];
  int vrx;
  int vry;
  bool btn_b_l;
  bool btn_b_r;
  bool btn_r_l;
  bool btn_r_r;
  //float c;
  //bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
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

  int m_vrx = map(myData.vrx, 0, 180, -90, 90);
  int m_vry = map(myData.vry, -90, 90, 180, 0);

  myservo_e.write(m_vrx + m_vry);
  myservo_d.write(m_vry - m_vrx);
  
  bool btn_1_h_r = myData.btn_b_r;
  bool btn_2_h_r = myData.btn_b_l;

  if (btn_1_h_r)
  {
    digitalWrite(arm_e, LOW);
    delay(1);
  }
  else
  {
    digitalWrite(arm_e, HIGH);
    delay(1);
  }

  if (btn_2_h_r)
  {
    digitalWrite(arm_d, LOW);
    delay(1);
  }
  else
  {
    digitalWrite(arm_d, HIGH);
    delay(1);
  }
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

  myservo_e.attach(5);
  myservo_d.attach(18, 1);

  connection = false;

  pinMode(arm_e, OUTPUT);
  pinMode(arm_d, OUTPUT);

  digitalWrite(arm_e, HIGH);
  delay(1);
  digitalWrite(arm_d, HIGH);
  delay(1);
}

void loop() {
  if (connection == false) {
    myservo_e.write(90);
    myservo_d.write(90);
    digitalWrite(arm_e, HIGH);
    delay(1);
    digitalWrite(arm_d, HIGH);
    delay(1);
  }
  connection = false;
  delay(100);
}