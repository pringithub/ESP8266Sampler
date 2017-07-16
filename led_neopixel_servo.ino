/*********
  Phil Ring
  Major borrowing from Rui Santos
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel leds = Adafruit_NeoPixel(2, 15, NEO_GRB + NEO_KHZ800);
Servo servo;
int servoOn=0;
int servoPin=12; // TODO: solder later

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

ESP8266WebServer server(80);

String webPage = "";

int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
  webPage += "<h1>ESP8266 Web Server</h1>";
  webPage += "<p>Socket #1 <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #2 <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></p>";
  webPage += "<p>Neopixel LED #1 Color <a href=\"neoLED1Red\"><button>Red</button></a>&nbsp;";
  webPage +=    "<a href=\"neoLED1Green\"><button>Green</button></a>&nbsp;";
  webPage +=    "<a href=\"neoLED1Blue\"><button>Blue</button></a>&nbsp;";
  webPage +=    "<a href=\"neoLED1RoyalBlue\"><button>RoyalBlue</button></a>&nbsp;";
  webPage +=    "<a href=\"neoLED1Lavender\"><button>Lavender</button></a>";
  webPage += "</p>";
  webPage += "<p>Neopixel LED #1 Brightness <a href=\"neoLED125pct\"><button>25%</button></a>&nbsp;";
  webPage +=    "<a href=\"neoLED150pct\"><button>50%</button></a>&nbsp;";
  webPage +=    "<a href=\"neoLED175pct\"><button>75%</button></a>&nbsp;";
  webPage +=    "<a href=\"neoLED1100pct\"><button>100%</button></a>&nbsp;";
  webPage += "</p>";
  webPage += "<p>Servo <a href=\"servoOn\"><button>Start Sweep</button></a>&nbsp;<a href=\"servoOff\"><button>End Sweep</button></a></p>";
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // preparing LEDs
  leds.begin();
  leds.show();

  // prepare servo
  servo.attach(servoPin);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });

  // LED handlers
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, LOW);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, HIGH);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio2_pin, LOW);
    delay(1000);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio2_pin, HIGH);
    delay(1000); 
  });
  
  // NeoPixel handlers
  server.on("/neoLED1Red", [](){
    server.send(200, "text/html", webPage);
    leds.setPixelColor(0, 255, 0, 0);
    leds.show();
    delay(1000);
  });
  server.on("/neoLED1Green", [](){
    server.send(200, "text/html", webPage);
    leds.setPixelColor(0, 0, 255, 0);
    leds.show();
    delay(1000);
  }); 
  server.on("/neoLED1Blue", [](){
    server.send(200, "text/html", webPage);
    leds.setPixelColor(0, 0, 0, 255);
    leds.show();
    delay(1000);
  });
  server.on("/neoLED1RoyalBlue", [](){
    server.send(200, "text/html", webPage);
    leds.setPixelColor(0, 65, 105, 225);
    leds.show();
    delay(1000);
  });  
  server.on("/neoLED1Lavender", [](){
    server.send(200, "text/html", webPage);
    leds.setPixelColor(0, 128, 0, 139);
    leds.show();
    delay(1000);
  });
  server.on("/neoLED125pct", [](){
    server.send(200, "text/html", webPage);
    leds.setBrightness(25);
    leds.show();
    delay(1000);
  });
  server.on("/neoLED150pct", [](){
    server.send(200, "text/html", webPage);
    leds.setBrightness(50);
    leds.show();
    delay(1000);
  });
  server.on("/neoLED175pct", [](){
    server.send(200, "text/html", webPage);
    leds.setBrightness(75);
    leds.show();
    delay(1000);
  });
  server.on("/neoLED1100pct", [](){
    server.send(200, "text/html", webPage);
    leds.setBrightness(100);
    leds.show();
    delay(1000);
  });

  // Servo handler
  server.on("/servoOn", [](){
    server.send(200, "text/html", webPage);
    servoOn=true;
    delay(1000);
  });
  server.on("/servoOff", [](){
    server.send(200, "text/html", webPage);
    servoOn=false;
    delay(1000);
  });
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();

  // lazy sweep - TODO: maybe figure something out later
  if (servoOn) {
    int pos;

    for(pos = 0; pos <= 180; pos += 1) {                                   
      servo.write(pos);              
      delay(15);                       
    } 
    for(pos = 180; pos>=0; pos-=1) {                                
      servo.write(pos);              
      delay(15);                       
    } 
  }
  
} 
