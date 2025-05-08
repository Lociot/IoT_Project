
#include <WiFi.h>
#include "DHTesp.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
// cấu hình dht11
#define DHTpin 4
#define DHTTYPE DHT11
DHT dht(DHTpin,DHTTYPE);
#define MQ135PIN 35  // khai báo pin nối với chân G35
#define RL_VALUE 10.0  // Điện trở tải RL của cảm biến (kΩ)
#define VCC 5.0  // Điện áp cấp cho cảm biến MQ135 (V)

// Định nghĩa các chân LED
#define Led_temp 5
#define Led_humidity 17
#define Led_dust 18
#define Led_co2 19
//
String ledStatus1 = "ON";
String ledStatus2 = "ON";
String ledStatus3 = "ON";
String ledStatus4 = "ON";

//
#define MQTT_LED_temp_TOPIC "ESP32/LED_temp"
#define MQTT_LED_humidity_TOPIC "ESP32/LED_humidity"
#define MQTT_LED_dust_TOPIC "ESP32/LED_dust"
#define MQTT_LED_co2_TOPIC "ESP32/LED_co2"


// wifi  setup 
const char* ssid = "Tret";  // Wifi connect
const char* password = "123456@#";   // Password
// MQTT setup 
const char* mqtt_server = "dcf9f7549a59456eb2c908d5e9affee5.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "hivemq.webclient.1732007688407"; // User
const char* mqtt_password = "DQ59vg@ps7bKSNTn1><*"; // Password

//Setting up WiFi and MQTT client
WiFiClientSecure espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];


void setup_wifi() {
  delay(10);
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


//------------Connect to MQTT Broker-----------------------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientID =  "ESP32Client-";
    clientID += String(random(0xffff), HEX);
    if (client.connect(clientID.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("esp32/client");
      client.subscribe(MQTT_LED_temp_TOPIC);
      client.subscribe(MQTT_LED_humidity_TOPIC);
      client.subscribe(MQTT_LED_dust_TOPIC);
      client.subscribe(MQTT_LED_co2_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//-----Call back Method for Receiving MQTT message---------
void callback(char* topic, byte* payload, unsigned int length) {
  char status[20];
  String incomingMessage = "";
  for (int i = 0; i < length; i++) incomingMessage += (char)payload[i];
  Serial.println("Message arrived [" + String(topic) + "]" + incomingMessage);

  for(int i = 0; i<length; i++)
  {
    status[i] = payload[i];
  }
  if(String(topic) == MQTT_LED_temp_TOPIC)
  {
    if(String(status) == "OFF")
    {
      ledStatus1 = "OFF";
      digitalWrite(Led_temp, LOW);
      Serial.println("LED_temp OFF");
    }
    else if(String(status) == "ON")
    {
      ledStatus1 = "ON";
      digitalWrite(Led_temp, HIGH);
      Serial.println("LED_temp ON");
    }
  }
  // led2
  if(String(topic) == MQTT_LED_humidity_TOPIC)
  {
    if(String(status) == "OFF")
    {
      ledStatus2 = "OFF";
      digitalWrite(Led_humidity, LOW);
      Serial.println("LED_humidity OFF");
    }
    else if(String(status) == "ON")
    {
      ledStatus2 = "ON";
      digitalWrite(Led_humidity, HIGH);
      Serial.println("LED_humidity ON");
    }
  }
  // led3
  if(String(topic) == MQTT_LED_dust_TOPIC)
  {
    if(String(status) == "OFF")
    {
      ledStatus3 = "OFF";
      digitalWrite(Led_dust, LOW);
      Serial.println("LED_dust OFF");
    }
    else if(String(status) == "ON")
    {
      ledStatus3 = "ON";
      digitalWrite(Led_dust, HIGH);
      Serial.println("LED_dust ON");
    }
  }
  // led4
  if(String(topic) == MQTT_LED_co2_TOPIC)
  {
    if(String(status) == "OFF")
    {
      ledStatus4 = "OFF";
      digitalWrite(Led_co2, LOW);
      Serial.println("LED_co2 OFF");
    }
    else if(String(status) == "ON")
    {
      ledStatus4 = "ON";
      digitalWrite(Led_co2, HIGH);
      Serial.println("LED_co2 ON");
    }
  }

}

//-----Method for Publishing MQTT Messages---------
void publishMessage(const char* topic, String payload, boolean retained) {
  if (client.publish(topic, payload.c_str(), retained))
    Serial.println("Message published [" + String(topic) + "]: " + payload);
}
// Khởi tạo giá trị R0 (hiệu chuẩn trong không khí sạch)
float R0 = 500;  // Giá trị R0 phải hiệu chuẩn tùy vào cảm biến và điều kiện môi trường
// Hàm tính giá trị Rs từ cảm biến
float getResistance(int analogValue) {
  float voltage = analogValue * (VCC / 4095.0);  // Chuyển đổi giá trị ADC thành điện áp
  float Rs = ((VCC / voltage) - 1) * RL_VALUE;  // Tính điện trở Rs của cảm biến
  return Rs;
}
// Hàm tính toán ppm CO2 dựa trên Rs/R0
float calculatePPM(float Rs) {
  float ratio = Rs / R0;  // Tính tỷ lệ Rs/R0
  // Công thức tính ppm dựa trên đường cong của MQ135 cho CO2
  float ppm = 116.6020682 * pow(ratio, -2.769034857);  // Hằng số từ datasheet MQ135 cho CO2
  return ppm;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(1);

  pinMode(Led_temp, OUTPUT);
  digitalWrite(Led_temp,HIGH); 
  pinMode(Led_humidity, OUTPUT);
  digitalWrite(Led_humidity,HIGH); 
  pinMode(Led_dust, OUTPUT);
  digitalWrite(Led_dust,HIGH); 
  pinMode(Led_co2,OUTPUT);
  digitalWrite(Led_co2,HIGH); 

  dht.begin();
  setup_wifi();
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

unsigned long timeUpdate = millis();
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read DHT11
  if (millis() - timeUpdate > 5000) {
   
    float h = dht.readHumidity();
    float t = dht.readTemperature();
      // Đọc giá trị bụi mịn từ MQ135
    int analogValue = analogRead(MQ135PIN);  // Đọc giá trị từ chân analog
    float Rs = getResistance(analogValue);  // Tính Rs từ giá trị ADC
    float ppm = calculatePPM(Rs);  // Tính nồng độ CO2 (ppm) từ Rs

    DynamicJsonDocument doc(1024);
    doc["humidity"] = h;
    doc["temperature"] = t;
    doc["co2"]=ppm;
    char mqtt_message[128];
    serializeJson(doc, mqtt_message);
    publishMessage("esp32/value", mqtt_message, true);

    timeUpdate = millis();
  }
}
