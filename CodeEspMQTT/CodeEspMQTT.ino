#include <ESP8266WiFi.h> // ESP32  <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>  

const long utcOffsetInSeconds = 25200;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

String inputString = "";
bool stringComplete = false;
String timestamp = "";
//String input = Serial.readString();

// Thông tin server MQTT
const char* mqtt_server = "mqtt.ngoinhaiot.com";  // tên server
const char* mqtt_user = "huuminh1999hp";
const char* mqtt_pass = "huuminh1999hp";
int mqtt_port = 1111; // esp kết nối mqtt => TCP
String topicsub = "huuminh1999hp/quat"; // nhận dữ liệu
String topicpub = "huuminh1999hp/bom_xang"; // gửi dữ liệu

WiFiClient espClient;
PubSubClient client(espClient);


// khai báo id mk wifi nhà mình
const char* ssid = "Tuan Hung";
const char* pass = "tuanhung0203";

String apiKey = "053398";              //Add your Token number that bot has sent you on signal messenger
String phone_number = "+84368896774"; //Add your signal app registered phone number (same number that bot send you in url)

String url; 

unsigned long last = 0;

String ChuoiSendMQTT = "";

void callback(char* topic, byte* payload, unsigned int length)
{
  for (int i = 0; i < length; i++)
  {
    inputString += (char)payload[i];
  }
  Serial.print("Data nhận: ");
  String input = Serial.readString();
  Serial.println(input);

  delay(100);

  inputString = "";
}

// Hàm connect wifi
void connectwifi()
{
  WiFi.disconnect();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connect wifi");
}

// Hàm reconnect wifi nếu kết nối failed
void reconnect()
{

  while (!client.connected())
  {
    String clientId = String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
    {
      Serial.println("Connected MQTT ngoinhaiot.com");
      client.subscribe(topicsub.c_str()); // nhận dữ liệu APP , web
    }
    else
    {
      Serial.println("Không kết nối MQTT ngoinhaiot.com");
      delay(5000);
    }
  }
}

// Hàm connect tới server
void ConnectMqttBroker()
{
  client.setServer(mqtt_server, mqtt_port); // sét esp client kết nối MQTT broker
  delay(10);
  client.setCallback(callback); // => đọc dữ liệu mqtt broker
  delay(10);
}

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  // connect wifi
  connectwifi();
  // kết nối server, setup
  ConnectMqttBroker();
  Serial.println("Start ESP");
  last = millis();
  timeClient.begin();

  // Nếu Signal app nhận được dòng "Hello from ESP8266" là send thành công
  message_to_signal("Hello from ESP8266");
}

// Hàm gửi message tới Signal app
void  message_to_signal(String message)       
{
  // Thay đổi phone number bà apikey trong url bên dưới
  url = "http://api.callmebot.com/signal/send.php?phone=+84368896774&apikey=053398&text="  + urlencode(message);

  postData(); // calling postData to run the above-generated url once so that you will receive a message.
}

// Hàm gọi API POST
void postData()     
{
  int httpCode;     // Biến để lưu trữ phản hồi sau khi gọi API
  HTTPClient http;  

  http.begin(espClient, url); //HTTP
 
 // Bắt đầu kết nối http
  httpCode = http.POST(url);
  if (httpCode == 200)      // Kiểm tra httpcode
  {
    Serial.println("Sent ok."); 
  }
  else                      // Gửi thành công khi httpcode là 200, khác 200 là lỗi
  {
    Serial.println("Error."); 
    Serial.print("httpCode: "); 
    Serial.println(httpCode);
  }
  // Kết thúc kết nối http
  http.end();          
}

// Hàm mã hóa url
String urlencode(String str)  
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}

char buff[50]={};
int incomingByte = 0;   // lưu tín hiệu đến
unsigned char chuoi_mqtt[50] ;
void loop()
{
  // Nếu ESP không connect thì sẽ reconnect lại
  if (!client.connected())
  {
    reconnect();
  }
  client.loop(); // duy trì kết nối esp server mqtt broker
  
 // Send data tới server qua MQTT khi có input từ stm32 gửi sang 
  String input = Serial.readStringUntil('\0');
  if(input != "")
    {
      timeClient.update();
      // Lấy thời gian
      String timestamp = String(timeClient.getHours()) + ":" + String(timeClient.getMinutes()) + ":" + String(timeClient.getSeconds());
      
      String chuoi_mqtt = String(input) + ";time:" + String(timestamp);
      Serial.println(chuoi_mqtt);
      client.publish(topicpub.c_str(), chuoi_mqtt.c_str());
     
      message_to_signal(chuoi_mqtt);
    }
}
