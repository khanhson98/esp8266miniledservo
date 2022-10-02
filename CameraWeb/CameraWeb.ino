#include "esp_camera.h"
#include <WiFi.h>
#include <ArduinoWebsockets.h>

typedef enum {
 WStype_ERROR,
 WStype_DISCONNECTED,
 WStype_CONNECTED,
 WStype_TEXT,
 WStype_BIN,
 WStype_FRAGMENT_TEXT_START,
 WStype_FRAGMENT_BIN_START,
 WStype_FRAGMENT,
 WStype_FRAGMENT_FIN,
 WStype_PING,
 WStype_PONG,
} WStype_t;

//
// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled
//

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

const char* ssid = "Galaxy Tab S6 Lite87F1";
const char* ssid2 = "Mechanics";
const char* ssid3 = "Son";
const char* password = "88888888";
const char* passWorkshop = "ask Will he knows the password!";
const char* websocket_server_host = "ws://khanhsonesp32cam.herokuapp.com/";
const uint16_t websocket_server_port = 8888;

using namespace websockets;
WebsocketsClient client;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 4000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers

//  config.frame_size = FRAMESIZE_UXGA;
//  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
//  config.fb_location = CAMERA_FB_IN_PSRAM;
//  config.jpeg_quality = 12;
//  config.fb_count = 1;
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 40;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

 

  WiFi.begin(ssid, password);
  int count = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    count++;
    if (count == 14)
      WiFi.begin(ssid2, passWorkshop);
    else if (count == 28)
      WiFi.begin(ssid3, password);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  connectWebSocket();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:                         // Sự kiện khi client ngắt kết nối
      Serial.printf("[WSc] Disconnected!\n");
      connectWebSocket();
      break;
    case WStype_CONNECTED:                            // Sự kiện khi client kết nối
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      //webSocket.sendTXT("Connected");          // Thông báo kết nối thành công
      break;
    case WStype_TEXT:                                 // Sự kiện khi nhận được thông điệp dạng TEXT
      Serial.printf("[WSc] get text: %s\n", payload);
      //webSocket.sendTXT("Hi Server!");               // Gởi thông điệp đến server
      break;
    case WStype_BIN:                                  // Sự kiện khi nhận được thông điệp dạng BINARY
      Serial.printf("[WSc] get binary length: %u\n", length);
      //hexdump(payload, length);
      // webSocket.sendBIN(payload, length);
      break;
  }
}

void connectWebSocket() {

//while(!client.connect(websocket_server_host, websocket_server_port, "/")){
  while(!client.connect(websocket_server_host)){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Websocket Connected!");
  client.onEvent(webSocketEvent);
  
  //client.begin("echo.websocket.org", 80, "/");          // Địa chỉ websocket server, port và URL
  //client.onEvent(webSocketEvent);
  // client.setAuthorization("user", "password");        // Sử dụng thông tin chứng thực nếu cần
  //client.setReconnectInterval(5000);                     // Thử lại sau 5s nếu kết nối không thành công
}

int i =0;
void loop() {
  
  camera_fb_t *fb = esp_camera_fb_get();
  if(!fb){
    Serial.println("Camera capture failed");
    esp_camera_fb_return(fb);
    return;
  }

  if(fb->format != PIXFORMAT_JPEG){
    Serial.println("Non-JPEG data not implemented");
    return;
  }
  i++;
  if (i==65000) i = 0;
  Serial.println(i);
  client.sendBinary((const char*) fb->buf, fb->len);
  
  esp_camera_fb_return(fb);
}
