#include <SoftwareSerial.h>

SoftwareSerial ESP11 = SoftwareSerial(2,3);

#define DEBUG true
#define SERVER "things.ubidots.com"
#define PORT 80
#define USER_AGENT "UbidotsESP8266"
#define VERSION "1.1"
#define TOKEN "8jyhmm2dO4wLOPymuwxlQQ7AkzD1cq"
#define wifiSSID "PLDTHOMEDSL"
#define wifiPASSWORD "mementom0r1"
//#define wifiSSID "phyre"
//#define wifiPASSWORD "c4pst0n3pr0j3ct"
int smokeA0 = A0;
int redLed = 12;
int greenLed = 13;
int sensorThres = 500;
int buzzer = 5;

void setup()
{
      Serial.begin(9600);
      ESP11.begin(9600);
      connectToWifi(wifiSSID,wifiPASSWORD);
      setupTCPConnection(SERVER,PORT);
//      String data = "{\"wifi\":1}";
//      int i  = data.length();
//      String toPost = "POST /api/v1.6/devices/test/?token=8jyhmm2dO4wLOPymuwxlQQ7AkzD1cq HTTP/1.1\r\n"
//                "Host: things.ubidots.com\r\n"
//                "User-Agent:" + String(USER_AGENT) + "/" + String(VERSION) + "\r\n"
//                "Content-Type: application/json\r\n"
//                "Content-Length: " + String(i) + "\r\n"
//                "\r\n"
//                + data +
//                "\r\n";
      //sendData(toPost,1000,DEBUG);
      Serial.print("\nWIFI SETUP DONE.....\n");
      pinMode(redLed, OUTPUT);
      pinMode(greenLed, OUTPUT);
      pinMode(buzzer, OUTPUT);
      pinMode(smokeA0, INPUT);



}

void loop() {
    int analogSensor = analogRead(smokeA0);
    Serial.print("\nSensor In: ");
    Serial.println(analogSensor);
    Serial.print("\n");
    // Checks if it has reached the threshold value
    if (analogSensor < sensorThres)
    {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      tone(buzzer, 1000, 200);
      alertCloud();
    }
    else
    {
      Serial.print("yes"); 
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      noTone(buzzer);
    }
//    sendDataToCloud(analogSensor);
    delay(1000);
}
void alertCloud(){
//   String data = "{\"firesense\":"+String(sensorval)+"}";
   String data = "{\"alarm\":1}";

   int dataLength = data.length();
   String postRequest = "POST /api/v1.6/devices/capstone/?token=8jyhmm2dO4wLOPymuwxlQQ7AkzD1cq HTTP/1.1\r\n"
                "Host: things.ubidots.com\r\n"
                "User-Agent:" + String(USER_AGENT) + "/" + String(VERSION) + "\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: " + String(dataLength) + "\r\n"
                "\r\n"
                + data +
                "\r\n";
   sendData(postRequest,1000,DEBUG);       
}
void setupTCPConnection(String host, int port)
{
    sendData("AT+CIPMUX=0\r\n",2000,DEBUG);
    sendData("AT+CIPSTART=\"TCP\",\""+host+"\","+String(port)+"\r\n",1000,DEBUG);
    sendData("AT+CIPMODE=1\r\n",1000,DEBUG);
    sendData("AT+CIPSEND\r\n",1000,DEBUG);
}
void connectToWifi(String ssid, String pass)
{
    Serial.print("\nConnecting to "+ssid+".....\n");
    sendData("AT+RST\r\n",1000,DEBUG);
    sendData("AT+CWMODE=1\r\n",1000,DEBUG);
    sendData("AT+CWJAP=\""+ssid+"\",\""+pass+"\"\r\n",10000, DEBUG);
    sendData("AT+CWJAP?\r\n",2000, DEBUG);
    sendData("AT+CIFSR\r\n",2000,DEBUG);
}
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    ESP11.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(ESP11.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = ESP11.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
