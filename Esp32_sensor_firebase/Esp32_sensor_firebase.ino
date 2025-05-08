#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <MQ135.h>


// Thông tin WiFi
#define WIFI_SSID "Tret"
#define WIFI_PASSWORD "123456@#"
              /* cài đặt firebase */
// Thông tin Firebase
#define FIREBASE_HOST "smart-city-210ce-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "DUGwN5U4jXbRAAQjlQ213pmckOP2rpvfKRAUCQlg"
// Khởi tạo Firebase
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
String path = "/";

#define DHTPIN 4 // khai báo pin DHT nối với chân G4
#define DHTTYPE DHT11  // Định nghĩa loại cảm biến DHT11
DHT dht(DHTPIN, DHTTYPE);  // Khởi tạo đối tượng DHT
#define MQ135PIN 35  // khai báo pin nối với chân G35
#define RL_VALUE 10.0  // Điện trở tải RL của cảm biến (kΩ)
#define VCC 5.0  // Điện áp cấp cho cảm biến MQ135 (V)

// Định nghĩa các chân LED
#define Led_temp 5
#define Led_humidity 17
#define Led_dust 18
#define Led_co2 19

// Khởi tạo biến ban đầu 
float temp = 0;
float humidity = 0;


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

     /* dust */
int measurePin = 34; // ESP32 ADC pin
int ledPower = 16; // Modify if using a different GPIO pin for the LED

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

float tinhdust(){
                   /* đọc giá trị bụi mịn */
   // Đọc giá trị bụi mịn từ PM2.5
  digitalWrite(ledPower, LOW); // Turn on IR LED
  delayMicroseconds(samplingTime);  // Delay 0.28ms
  voMeasured = analogRead(measurePin); // Read the ADC value
  delayMicroseconds(deltaTime); // Delay 0.04ms
  digitalWrite(ledPower, HIGH); // Turn off LED
  delayMicroseconds(sleepTime); // Delay 9.68ms
  // Convert the ADC value to voltage (ESP32 ADC range is 0 to 4095)
  calcVoltage = voMeasured * (3.3 / 4095); // Assuming the ESP32 is running at 3.3V
  // Linear Equation for dust density
  dustDensity = 0.17 * calcVoltage ;
  return dustDensity;
}

void setup() {
  Serial.begin(115200);  // Khởi tạo Serial Monitor
  delay(500);
  pinMode(MQ135PIN, INPUT);  // Chân MQ135 là chân đầu vào (analog)
  pinMode(Led_temp, OUTPUT);
  digitalWrite(Led_temp,LOW); 
  pinMode(Led_humidity, OUTPUT);
  digitalWrite(Led_humidity,LOW); 
  pinMode(Led_dust, OUTPUT);
  digitalWrite(Led_dust,LOW); 
  pinMode(Led_co2, OUTPUT);
  digitalWrite(Led_co2,LOW); 
  pinMode(ledPower, OUTPUT);


// Kết nối Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");

// Cấu hình Firebase
config.host = FIREBASE_HOST;  // Đặt host Firebase
config.signer.tokens.legacy_token = FIREBASE_AUTH;  // Đặt legacy token Firebase

// Kết nối Firebase
Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);

// Khởi động DHT
  dht.begin();
}

void loop() {
                /* Đọc giá trị từ DHT11 */
  temp = dht.readTemperature();  // Đọc nhiệt độ
  humidity = dht.readHumidity();  // Đọc độ ẩm
   // Kiểm tra nếu không có lỗi khi đọc DHT
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
                  /* Đọc giá trị từ MQ135 */
   // Đọc nồng độ khí co2 từ MQ135
  int analogValue = analogRead(MQ135PIN);  // Đọc giá trị từ chân analog
  float Rs = getResistance(analogValue);  // Tính Rs từ giá trị ADC
  float ppm = calculatePPM(Rs);  // Tính nồng độ CO2 (ppm) từ Rs
  float dustDensity = tinhdust();
                 /* Temperature */
  // Gửi nhiệt độ lên Firebase
  if (Firebase.setFloat(firebaseData, "/HRroom/nhietdo", temp)) {
    Serial.print("Temperature: ");
    Serial.println(temp);
  
  } else {
    Serial.print("Failed to send temp: ");
    Serial.println(firebaseData.errorReason());
  }
  delay(500);

// Kiểm tra giá trị trạng thái LED temp trong Firebase
  if (Firebase.getString(firebaseData, "/HRroom/device1/state")) {
    String led1Status = firebaseData.stringData();

    // Kiểm tra trạng thái LED 1 từ Firebase
    if (led1Status.equals("on")) {  // So sánh chuỗi chính xác
      digitalWrite(Led_temp, HIGH); // Bật LED 1
      Serial.println("LED 1: BẬT");
    } else if (led1Status.equals("off")) {  // Đảm bảo chuỗi "off" dùng đúng cách
      digitalWrite(Led_temp, LOW); // Tắt LED 1
      Serial.println("LED 1: TẮT");
    }
  } else {
    Serial.println("Lỗi khi đọc trạng thái LED từ Firebase:");
    Serial.println(firebaseData.errorReason());
  }
  delay(500);
                  /* HUMIDITY */
  // Gửi độ ẩm lên Firebase
  if (Firebase.setFloat(firebaseData, "/HRroom/doam", humidity)) {
    Serial.print("Humidity : ");
    Serial.println(humidity);
   
  } else {
    Serial.print("Failed to send humidity: ");
    Serial.println(firebaseData.errorReason());
  }
  // Kiểm tra giá trị trạng thái LED humidity trong Firebase
  if (Firebase.getString(firebaseData, "/HRroom/device2/state")) {
    String led2Status = firebaseData.stringData();

    // Kiểm tra trạng thái LED 2 từ Firebase
    if (led2Status.equals("on")) {  
      digitalWrite(Led_humidity, HIGH); 
      Serial.println("LED 2: BẬT");
    } else if (led2Status.equals("off")) {  
      digitalWrite(Led_humidity, LOW); 
      Serial.println("LED 2: TẮT");
    }
  } else {
    Serial.println("Lỗi khi đọc trạng thái LED từ Firebase:");
    Serial.println(firebaseData.errorReason());
  }
  delay(500);
                /* Nồng độ khí co2 */
// Gửi CO2 lên Firebase
  if (Firebase.setFloat(firebaseData, "/HRroom/khico2", ppm)) {
    Serial.print("CO2 (ppm): ");
    Serial.println(ppm);
  } else {
    Serial.print("Failed to send air quality: ");
    Serial.println(firebaseData.errorReason());
  }
  delay(500);
// Kiểm tra giá trị trạng thái LED dust trong Firebase
  if (Firebase.getString(firebaseData, "/HRroom/device3/state")) {
    String led3Status = firebaseData.stringData();

    // Kiểm tra trạng thái LED 3 từ Firebase
    if (led3Status.equals("on")) {  // So sánh chuỗi chính xác
      digitalWrite(Led_dust, HIGH); // Bật LED 3
      Serial.println("LED 3: BẬT");
    } else if (led3Status.equals("off")) {  // Đảm bảo chuỗi "off" dùng đúng cách
      digitalWrite(Led_dust, LOW); // Tắt LED 3
      Serial.println("LED 3: TẮT");
    }
  } else {
    Serial.println("Lỗi khi đọc trạng thái LED từ Firebase:");
    Serial.println(firebaseData.errorReason());
    // Kiểm tra giá trị trạng thái LED co2 trong Firebase
  }
  delay(500);
                /* DUST */
// Gửi bụi mịn lên Firebase
  if (Firebase.setFloat(firebaseData, "/HRroom/buimin", dustDensity)) {
    Serial.print("Raw Signal Value (0-4095): ");
    Serial.print(voMeasured);
    Serial.print(" - Voltage: ");
    Serial.print(calcVoltage);
    Serial.print(" - Dust Density: ");
    Serial.println(dustDensity);
  } else {
    Serial.print("Failed to send dust data: ");
    Serial.println(firebaseData.errorReason());
  }
  delay(500);
  if (Firebase.getString(firebaseData, "/HRroom/device4/state")) {
    String led4Status = firebaseData.stringData();
    // Kiểm tra trạng thái LED 4 từ Firebase
    if (led4Status.equals("on")) {  // So sánh chuỗi chính xác
      digitalWrite(Led_co2, HIGH); // Bật LED 4
      Serial.println("LED 4: BẬT");
    } else if (led4Status.equals("off")) {  // Đảm bảo chuỗi "off" dùng đúng cách
      digitalWrite(Led_co2, LOW); // Tắt LED 4
      Serial.println("LED 4: TẮT");
    }
  } else {
    Serial.println("Lỗi khi đọc trạng thái LED từ Firebase:");
    Serial.println(firebaseData.errorReason());
  }
delay(500);
}
