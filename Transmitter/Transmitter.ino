#include <ESP8266WiFi.h>
#include <espnow.h>

// Reciever MAC address (94:B9:7E:13:51:E1)
uint8_t recieverAddress[] = {0x94, 0xB9, 0x7E, 0x13, 0x51, 0xE1};

// Data structure
typedef struct struct_message {
  int quandagle_the_third;
  int quandagle_the_forth;
} struct_message;

// create struct_message buttonData
struct_message buttonData;

// Please for the sake of the esp, don't spam the connection (add delay)
unsigned long lastTime = 0;  
unsigned long timerDelay = 200;


// Define button pins
// D4
 int b1 = 2;
 //D3
 int b2 = 0;


// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success" + String(buttonData.quandagle_the_third) + String(buttonData.quandagle_the_forth));
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup() {
  // Setup button pins
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);

  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW, if it fails display that there was an error in the serial monitor
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // set up ESPnow role and then set up callbakc to give info on the data being sent
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(recieverAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    buttonData.quandagle_the_third = digitalRead(b1);
    buttonData.quandagle_the_forth = digitalRead(b2);

    // Send message via ESP-NOW
    esp_now_send(recieverAddress, (uint8_t *) &buttonData, sizeof(buttonData));

    lastTime = millis();
  }  

}
