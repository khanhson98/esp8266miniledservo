#include <SocketIoClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

#define USER_SERIAL Serial
Servo myservo;


//const char* ssid1 = "DODO-78D9";
const char* ssid1 = "Galaxy Tab S6 Lite87F1";
const char* ssid2 = "Son";
const char* ssid3 = "Mechanics";
//const char* pass = "UV5NNLGWGT";
const char* pass = "88888888";
const char* passWorkshop = "ask Will he knows the password!";
SocketIoClient webSocket;

int R = D1;
int G = D2;
int B = D3;

void setup() {
  USER_SERIAL.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  myservo.attach(D4);

  connectWiFi();

 //webSocket.begin("192.168.233.8", 5000);
  //webSocket.begin("https://khanhsonesp8266miniledservo.herokuapp.com");
  webSocket.begin("khanhsonesp8266miniledservo.herokuapp.com");
  webSocket.on("colorChanged", changeColor);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  webSocket.loop();

}

void changeColor(const char* data, size_t length){ 
  USER_SERIAL.println(data);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, data);

  int r = doc[0];
  int g = doc[1];
  int b = doc[2];
  int servo = doc[3];
  
  
  analogWrite(R, r);
  analogWrite(G, g);
  analogWrite(B, b);
  myservo.write(servo);
}

void connectWiFi(){
  WiFi.begin(ssid1, pass);
  int count = 0;
  
  while(WiFi.status() != WL_CONNECTED){
    USER_SERIAL.print(".");
    delay(1000);
    count ++;
    if (count == 7)
        WiFi.begin(ssid2, pass);
    else if (count == 14)
        WiFi.begin(ssid3, passWorkshop);
  }

  USER_SERIAL.print("");
  USER_SERIAL.println("WiFi connected");
  USER_SERIAL.print("IP Address : ");
  USER_SERIAL.println(WiFi.localIP());
  
}
