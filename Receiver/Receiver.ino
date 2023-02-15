#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>

// Data structure
typedef struct struct_message {
  int quandagle_the_third;
  int quandagle_the_forth;
} struct_message;

// create struct_message buttonData
struct_message buttonData;

int button1;
int button2;

float servoPos = 0;
Servo servo;
int servoPin = 2;


// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&buttonData, incomingData, sizeof(buttonData));
  /*Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Button 1");
  Serial.println();
  Serial.println(buttonData.quandagle_the_third);
  Serial.print("Button 2");
  Serial.println();
  Serial.println(buttonData.quandagle_the_forth);
  Serial.println();*/
  
  button1 = buttonData.quandagle_the_third;
  button2 = buttonData.quandagle_the_forth;
}
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  servo.attach(servoPin);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW and tell us if there is an error
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (button1 == 0 && servoPos < 199){
    servoPos+=1;
  }else if (button2 == 0 && servoPos > 0){
    servoPos-=1;
  }
  Serial.println(servoPos);
  servo.write(servoPos);
  delay(0.02);
  
}
