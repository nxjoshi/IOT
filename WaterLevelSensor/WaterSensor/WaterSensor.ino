#include <Arduino.h>
#include <ESP8266WiFi.h>
#define watersensor D2
#define sensorPower 7
#define sensorPin A0
const char* ssid = "Pritish";
const char* password = "Nitipriya123*";
int val = 0;
WiFiServer server(3001);
IPAddress ip(192, 168, 1, 5);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 43, 1);
void setup() {

  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  //digitalWrite(13, LOW);
  delay(10);
  // Connect to WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //Static IP Setup Info Here...
  WiFi.config(ip, dns, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started");
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
void loop()
{ int level = readSensor();
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    //    delay(1);
    client.setNoDelay(1);
  }
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  // Set ledPin according to the request
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("");
  client.println("<html>");
  client.println("<meta http-equiv='refresh' content='20'>");
  client.println("<title>WATER OVERFLOW ALARM WITH ONLINE UPDATES</title>");
  client.println("<center>");
  client.println("<style>");
  client.println("H1 {");
  client.println("background-color: blue;");
  client.println("}");
  client.println("</style>");
  client.println("<H1 style='color:white'><table>");
  client.println("<tr>");
  client.println("<td><div class='inner'><font size= '+3' color='white'><b>");
  client.println("WATER OVERFLOW ALARM WITH ONLINE UPDATES<br>");
  client.println("USING NODEMCU 1.0 BOARD</font></b>");
  client.println("</div></td>");
  client.println("<td><div class='relative'>");
  if (level > 300)
  { pinMode(13, OUTPUT);
    client.println("<button style='background-color:red;width:200px;height:100px'><font size= '+2' color='white'><b>FULL LEVEL ALERT</b></font></button><br>");
    client.println("<script>");
    client.println(" function play() { ");
    client.println(" var audio = new Audio( ");
    client.println("'https://media.geeksforgeeks.org/wp-content/uploads/20190531135120/beep.mp3');");
    client.println("audio.play();");
    client.println(" } ");
    client.println("</script>");
    delay(20);
    
    client.println("<button style='background-color:gray;width:200px;height:100px'<font size= '+2' color='white'><b>NOT FULL</b></font></button><br>");
    client.println("<script>");
    client.println(" function play() { ");
    client.println(" var audio = new Audio( ");
    client.println("'https://media.geeksforgeeks.org/wp-content/uploads/20190531135120/beep.mp3');");
    client.println("audio.play();");
    client.println(" } ");
    client.println("</script>");
  }
  else
  {
    client.println("<button style='background-color:gray;width:200px;height:100px'<font size= '+2' color='white'><b>NOT FULL</b></font></button><br>");
  }
  client.println("</div></td>");
  client.println("</tr>");
  client.println("</table><hr></center>");
  client.println("<body style = 'background-color:lightgray;'>");
  client.println("<br>");
  client.println("<style>");
  client.println("div.relative {");
  client.println("position: relative;");
  client.println("left:100px;");
  client.println("}");
  client.println("div.inner {");
  client.println("position: relative;");
  client.println("left:10px;");
  client.println("}");
  client.println("div.sample1 {");
  client.println("position: relative;");
  client.println("left:85px;");
  client.println("}");
  client.println("</style><hr><br>");
  client.println("<b>LEGENDS: </b> <button style='background-color:gray;width:100px;height:50px'><font color='white'<b>NOT FULL</b></font></button>");
  client.println("<font color='black'><b>WATER LEVEL IS BELOW SENSOR.</b></font><p>");
  client.println("<div class='sample1'><button style='background-color:red;width:100px;height:50px'><font color='white'><b>FULL LEVEL ALERT</b></font></button><font color='black'><b> WATER LEVEL IS ALREADY FULL.</b></font>");
  client.println("</div><br><hr>");
  client.println("</body>");
  client.println("</html>");
  delay(1);
  Serial.println("Client disconnected");
  //Serial.println(watersensor);
  Serial.println("");

  Serial.print("Water level: ");
  Serial.println(level);
  if (level > 300) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }
  delay(100);
  delay(10);
}

int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}
