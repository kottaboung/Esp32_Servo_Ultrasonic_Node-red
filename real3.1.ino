#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Bin1";
const char* password = "12345678";
const char* mqtt_server = "broker.hivemq.com";
int inPin = 14;
const int pingPin = 27;

Servo myservo;
int servoPin = 12; // กำหนดขาที่เชื่อมกับ Servo
int servoAngle = 90;  // ตำแหน่งเริ่มต้นของ Servo

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(pingPin, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  myservo.attach(servoPin);
  myservo.write(servoAngle);
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String string;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print("Received message on mqttTopic2: ");

  for (int i = 0; i < length; i++) {
    string += (char)payload[i];
  }
  
  if (topic= "servo/control")  {
    Serial.print("   ");
    myservo.write(0);
    int resultado = string.toInt();
    int angle = map(resultado, 1, 100, 0, 180);

    Serial.println(angle);
    myservo.write(angle);
    delay(2000);
    
  } 
  

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("esp32Client")) {
      Serial.println("connected");
      client.subscribe("servo/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {

  long duration, cm;

  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(inPin, INPUT);
  duration = pulseIn(inPin, HIGH);

  cm = microsecondsToCentimeters(duration);

  Serial.print("cm");
  Serial.println(cm);
  delay(1000);

  if (cm <= 15 ) {
    myservo.write(180);
    delay(3000);
  } 
  else if (cm >16){
    myservo.write(0);
  }
  else if (cm <= 0){
    myservo.write(0);
  }
  

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
}

long microsecondsToCentimeters(long microseconds) {
    return microseconds / 29 / 2;
  }