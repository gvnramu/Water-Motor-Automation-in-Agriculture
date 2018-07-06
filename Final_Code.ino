#include <ESP8266WiFi.h>
 
const char* ssid     = "Not Available";
const char* password = "notavailable";
 
const char* host = "api.thingspeak.com";
const char* server = "api.thingspeak.com";
String apiKey = "N1UU0M33U6CMSRGP"; 
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(D0,OUTPUT);         //to the relay
  pinMode(A0,INPUT);          //from the water sensor
  digitalWrite(D0,HIGH);
 
  // We start by connecting to a WiFi network
 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println();
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
}
 
int state = 0;
int sensorvalue;
String want;
 
void loop() {
  delay(5000);
 
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/channels/409368/fields/1/last?api_key=LBQRU9ZYYATF2X91";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  String line="hello";

  // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(5000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available())
  {
    line = client.readStringUntil('\r');
    //Serial.print(line);
  }
  Serial.print(line);  
  Serial.println();
  client.stop();
  int data=line.toInt();
  if(data==1)
  {
    if(state==1)
    {
      
    }
    else if(state==0)
    {
      digitalWrite(D0,LOW);    //On the motor
      state=1;
    }
  }
  else if(data==0)
  {
    if(state==1)
    {
      digitalWrite(D0,HIGH);  //off the motor
      state=0;
    }
    else if(state==0)
    {
      
    }
  }
  //Serial.println("closing connection");
  client.stop();
  delay(5000);
  if(state==1)
  {
    sensorvalue=analogRead(A0);
    Serial.println(sensorvalue);
    if(sensorvalue<50)
    {
                          digitalWrite(D0,HIGH);    //off the motor
                          state=0;
                          if (!client.connect(server, httpPort)) {
                              Serial.println("connection failed");
                              return;
                          }
                          String url = "/update";
                          url += "?api_key=";
                          url += apiKey;
                          url += "&field1=";
                          url += "0";
                          url += "&field2=";
                          url += "1";

                          Serial.print("Requesting URL: ");
                          Serial.println(url);
  
                          // This will send the request to the server
                          client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                          "Host: " + server + "\r\n" + 
                          "Connection: close\r\n\r\n");
                          delay(5000);
  
                        while(client.connected() && !client.available()) delay(1); //waits for data
                        while (client.connected() || client.available())
                        {
                             char charIn = client.read();
                             //Serial.print(charIn);
                        }
                        Serial.println();
                        //Serial.println("closing connection");
                        client.stop();
                        //delay(10000);
     }
  }
}
