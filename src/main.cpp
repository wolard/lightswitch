#include <Arduino.h>
#include <Bounce2.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "talli";
const char* password = "kopo2008";
const char* mqtt_server = "192.168.0.3";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


Bounce debouncer = Bounce(); 
int inPin = D1;         // the number of the input pin
int outPin = LED_BUILTIN;       // the number of the output pin

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time1 = 0;           // the last time the output pin was toggled
unsigned long debounce = 200UL;   // the debounce time, increase if the output flickers
boolean reconnect() {
  if (client.connect("button1")) {
    // Once connected, publish an announcement...
   // client.publish("outTopic","hello world");
    // ... and resubscribe
  //  client.subscribe("inTopic2");
  }
  return client.connected();
}
void setup()
{
  pinMode(inPin,  INPUT_PULLUP);
  pinMode(outPin, OUTPUT);

  debouncer.attach(inPin);
  debouncer.interval(5); // interval in ms
 Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop()
{
  //reading = digitalRead(inPin);
 debouncer.update();
 if (!client.connected()) {
    reconnect();
  }
 
    client.loop();
  // Get the updated value :
  int reading = debouncer.read();
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == LOW && previous == HIGH)
  {
    if (state == HIGH){
       snprintf (msg, 5, "1");
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("/talli/heppatalli/valokytkin", msg);
      
      state = LOW;
    }
    else
    {
      state = HIGH;
       snprintf (msg, 5, "0");
    Serial.print("Publish message: ");
    Serial.println(msg);
   client.publish("/talli/heppatalli/valokytkin", msg);

    time1 = millis();
  }
  }
  digitalWrite(outPin, state);
  
  previous = reading;
}