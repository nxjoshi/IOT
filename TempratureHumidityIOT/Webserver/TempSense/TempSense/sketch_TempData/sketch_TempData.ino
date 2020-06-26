/*******************Esp8266_Websocket.ino****************************************/

#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
#include<DHT.h>

boolean handshakeFailed=0;
String data= "";

#define DHTPIN 0

DHT dht (DHTPIN, DHT11);

char path[] = "/";   //identifier of this device

const char* ssid     = "Pritish";
const char* password = "Nitipriya123*";
char* host = "192.168.1.6";  //replace this ip address with the ip address of your Node.Js server
const int espport= 3001;
  
WebSocketClient webSocketClient;
unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long interval=300; //interval for sending data to the websocket server in ms

// Use WiFiClient class to create TCP connections
WiFiClient client;


void setup() {
  Serial.begin(115200);
//    pinMode(LED_BUILTIN, INPUT);     // Initialize the LED_BUILTIN pin as an output

  delay(10);
dht.begin();
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    pinMode(5, OUTPUT);
    digitalWrite(5,HIGH);
  }
  digitalWrite(5,LOW);
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  delay(1000);
  
wsconnect();
//  wifi_set_sleep_type(LIGHT_SLEEP_T);
}

void loop() {
 float humid = dht.readHumidity();
 float temp = dht.readTemperature();

 if (isnan(humid) || isnan(temp)){
   Serial.println("Sensor Failed");
   webSocketClient.sendData("Sensor Failed");
   return;
  }

  
  if (client.connected()) {
currentMillis=millis(); 
    webSocketClient.getData(data);    
    if (data.length() > 0) {
Serial.println(String(humid) + " || "+ String(temp));




    //*************send log data to server in certain interval************************************
 //currentMillis=millis();   
 if (abs(currentMillis - previousMillis) >= interval) {
previousMillis = currentMillis;
data= (String) analogRead(A0); //read adc values, this will give random value, since no sensor is connected. 
//For this project we are pretending that these random values are sensor values

webSocketClient.sendData("Humidity: "+String(humid) + " || "+"Temprature: "+ String(temp));//send sensor data to websocket server
}

  }
  else{
}

delay(5000);

  }
  digitalWrite(13,HIGH);
delay(100);
digitalWrite(13,LOW);
delay(100);
}

//*********************************************************************************************************************
//***************function definitions**********************************************************************************
void wsconnect(){
  // Connect to the websocket server
  if (client.connect(host, espport)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
      delay(1000);  
   
   if(handshakeFailed){
    handshakeFailed=0;
    ESP.restart();
    }
    handshakeFailed=1;
  }

  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    
    Serial.println("Handshake failed.");
   delay(4000);  
   
   if(handshakeFailed){
    handshakeFailed=0;
    ESP.restart();
    }
    handshakeFailed=1;
  }
}
