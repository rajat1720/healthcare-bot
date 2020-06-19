#include<ESP8266WiFi.h>
#include<FirebaseArduino.h>
#include<DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

#define FIREBASE_HOST "covid19-healthcare-bot.firebaseio.com"
#define FIREBASE_AUTH ""

int in1 = 3;
int in2 = 5;
int in3 = 6;
int in4 = 9;
int ena = 10;
int enb = 11;

WiFiClient client;
WiFiServer server(80);

const char* ssid = "OPPO F11";
const char* password = "12345678";

String data ="";

void setup()
{

  Serial.begin(115200);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.print(".");
}
  dht.begin();
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  server.begin();
}

void loop()
{
    client = server.available();
    if (!client) return; 
    data = checkClient ();

    if (data == "forward") MotorForward();
    else if (data == "backward") MotorBackward();
    else if (data == "left") TurnLeft();
    else if (data == "right") TurnRight();
    else if (data == "stop") MotorStop();
    
    Serial.println();

    int chk = dht.read(DHTPIN);

    float t = dht.readTemperature();
    Serial.print(t);
    Firebase.setFloat ("Temp",t);

    delay(2000);
}


void motorDirection()
{
  char i;
  i = Serial.read();

  analogWrite(ena, 255);
  analogWrite(enb, 255);

  if (i == 'w')
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(2000);
  }
  else if (i == 's')
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(2000);
  }
  else if (i == 'a')
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(2000);
  }
  else if (i == 'd')
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(2000);
  }
  else if (i == 'q')
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(2000);
  }
}

void MotorForward(void)   
{
  digitalWrite(ena,HIGH);
  digitalWrite(enb,HIGH);
  digitalWrite(in1,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in4,LOW);
}

void MotorBackward(void)   
{
  digitalWrite(ena,HIGH);
  digitalWrite(enb,HIGH);
  digitalWrite(in1,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in4,LOW);
}

void TurnLeft(void)   
{
  digitalWrite(ena,HIGH);
  digitalWrite(enb,HIGH); 
  digitalWrite(in1,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in4,HIGH);  
}

void TurnRight(void)   
{
  digitalWrite(ena,HIGH);
  digitalWrite(enb,HIGH);
  digitalWrite(in1,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in4,LOW);
}

void MotorStop(void)   
{
  digitalWrite(ena,LOW);
  digitalWrite(enb,LOW);
  digitalWrite(in1,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in4,LOW);
}

String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
  



String req = client.readStringUntil('\r');
Serial.println(req);
client.flush();
  
if (req.indexOf("Forward") != -1)
{
digitalWrite(in1,HIGH);
digitalWrite(in2,LOW);
digitalWrite(in3,HIGH);
digitalWrite(in4,LOW);
Serial.println("Forward");
}
else if(req.indexOf("Left") != -1)
{
digitalWrite(in1,HIGH);
digitalWrite(in2,LOW);
digitalWrite(in3,LOW);
digitalWrite(in4,HIGH);
Serial.println("left");
}
else if (req.indexOf("Right") != -1)
{
digitalWrite(in1,LOW);
digitalWrite(in2,HIGH);
digitalWrite(in3,HIGH);
digitalWrite(in4,LOW);
Serial.println("right");
}
else if (req.indexOf("Backward") != -1)
{
digitalWrite(in1,LOW);
digitalWrite(in2,HIGH);
digitalWrite(in3,LOW);
digitalWrite(in4,HIGH);
Serial.println("backward");
}
else if (req.indexOf("Stop") != -1)
{
digitalWrite(in1,LOW);
digitalWrite(in2,LOW);
digitalWrite(in3,LOW);
digitalWrite(in4,LOW);
Serial.println("Stop");
}



client.println("HTTP/1.1 200 OK"); //
client.println("Content-Type: text/html");
client.println("");
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<br><br>");
client.println("<a href=\"/Forward\"\"><button>ON</button></a>");
client.println("<a href=\"/Backward\"\"><button>OFF</button></a><br />");
client.println("<a href=\"/Right\"\"><button>ON</button></a>");
client.println("<a href=\"/Left\"\"><button>OFF</button></a><br />");
client.println("<a href=\"/Stop\"\"><button>OFF</button></a><br />");
client.println("</html>");
}
