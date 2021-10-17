#include "M5Atom.h"
 #include <WiFi.h>
#include "fauxmoESP.h"
#include <ESP32Servo.h> 

Servo myservo;  // create servo object to control a servo

#define ID_YELLOW           "yellow"
#define ID_GREEN            "green"
#define ID_BLUE             "blue"
#define ID_RED              "red"
#define ID_WHITE            "white"

#define ID_OPC1           "switch"
#define ID_TXT2           "Hello World"
#define ID_TXT3           "text 1"
#define ID_TXT4           "text 2"
#define ID_TXT5           "text 3"


// Images
extern const unsigned char image_helloWord[902];
extern const unsigned char image_text3[452];
extern const unsigned char image_text4[452];
extern const unsigned char image_text5[452];

unsigned int state = 0;
unsigned int prev_state = 0;
static bool state_led =false , event= false;

fauxmoESP fauxmo;
const char* ssid = "YOUR SSID";
const char* password = "**********";

void shutDown(unsigned int time_delay){
  for (int i = 0;i<25;i++){
    M5.dis.drawpix(i, 0x000000);
    delay(time_delay);
  }
}

void onColor(unsigned int  color, unsigned int time_delay){
  for (int i = 0;i<25;i++){
    M5.dis.drawpix(i, color);
    delay(time_delay);
  }
}

// Callback function is called an event coming from Alexa to the registered device
void AlexaCallback(unsigned char device_id, const char *device_name, bool state, unsigned char val) {
  Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", val);
  event = true;
    if(state){
        if (strcmp(device_name, ID_YELLOW)==0) {                   
          state = 1;      
        } else if (strcmp(device_name, ID_GREEN)==0) {
          state = 2; 
        } else if (strcmp(device_name, ID_BLUE)==0) {
            state = 3; 
        } else if (strcmp(device_name, ID_RED)==0) {
            state = 4; 
        } else if (strcmp(device_name, ID_WHITE)==0) {
            state = 5; 
        }

        
        if (strcmp(device_name, ID_OPC1)==0) {
            state = 6; 
        } else if (strcmp(device_name, ID_TXT2)==0) {
            state = 7; 
        } else if (strcmp(device_name, ID_TXT3)==0) {
            state = 8; 
        } else if (strcmp(device_name, ID_TXT4)==0) {
            state = 9; 
        } else if (strcmp(device_name, ID_TXT5)==0) {
            state = 10; 
        }
   prev_state = state;}
   else{
    if (strcmp(device_name, ID_OPC1)==0) {
            state = 11; 
        }    
   shutDown(10);   
   state = 0;
   prev_state = 0;
   }
}

void setup() {
  myservo.attach(25,500, 2400); 
  M5.begin(true, false, true);
  delay(50);
  onColor(0xff0000,10);
  delay(100);
  onColor(0x00ff00,10);
  delay(100);
  onColor(0x0000ff,10);
  delay(100);
  onColor(0xffff00,10);
  delay(100);
  onColor(0xffffff,10);
  delay(100);
  onColor(0xffffff,10);
  shutDown(10);
              
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.print("WiFi connected\r\nIP address: ");
  Serial.println(WiFi.localIP());

  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);

// register device
  fauxmo.addDevice(ID_YELLOW);
  fauxmo.addDevice(ID_GREEN);
  fauxmo.addDevice(ID_BLUE);
  fauxmo.addDevice(ID_RED);
  fauxmo.addDevice(ID_WHITE);
  fauxmo.addDevice(ID_OPC1);
  fauxmo.addDevice(ID_TXT2);
  fauxmo.addDevice(ID_TXT3);
  fauxmo.addDevice(ID_TXT4);
  fauxmo.addDevice(ID_TXT5);
  fauxmo.onSetState(AlexaCallback);
  myservo.write(100);  
}

void loop() {  

  if (M5.Btn.wasPressed()) {
    event = true;
    state_led = !(state_led);
    if(state_led) {
      //myservo.write(120);  
      state = 0;
    } else {
      state = prev_state;
      // myservo.write(130);  
    }
  }
  if (event){
    event = false;
    switch (state) {
      case 0:
      shutDown(10);
        break;
      case 1:
      onColor(0xffff00,10);
        break;
      case 2:
      onColor(0xff0000,10);      
        break;
      case 3:
      onColor(0x0000ff,10);
        break;
      case 4:
      onColor(0x00ff00,10);
        break;
      case 5:
      onColor(0xffffff,10);
        break;
      case 6:
     myservo.write(140);
     delay(500);
     myservo.write(125);  
     break;  
      case 7:
      M5.dis.animation((uint8_t *)image_helloWord, 400, LED_DisPlay::kMoveTop, 0);
        break;
      case 8:
      M5.dis.animation((uint8_t *)image_text3, 400, LED_DisPlay::kMoveTop, 0);
        break;
      case 9:
      M5.dis.animation((uint8_t *)image_text4, 400, LED_DisPlay::kMoveTop, 0);
        break;
      case 10:
      M5.dis.animation((uint8_t *)image_text5, 400, LED_DisPlay::kMoveTop, 0);
        break;
      case 11:
      myservo.write(140);
     delay(500);
     myservo.write(125);
        break;        
      default:
        break;
    }
  }

  fauxmo.handle();
  M5.update();
  delay(50);
}
