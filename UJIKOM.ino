#define BLYNK_TEMPLATE_ID "TMPL61meT2PyC"
#define BLYNK_TEMPLATE_NAME "uji"
#define BLYNK_AUTH_TOKEN "QjLqA0PTM1X8Opbzhl45_x1DjIRq_HdN"

//#define BLYNK_TEMPLATE_ID "TMPL68ir4R6SX"
//#define BLYNK_TEMPLATE_NAME "UJIKOM"
//#define BLYNK_AUTH_TOKEN "SwpX1nMcPsLkpBzYks66o6y3oPr6SFeG"

#define BLYNK_PRINT Serial

#include <Keypad.h>
#include <key.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS]=
{
  '1','2','3','A',
  '4','5','6','B',
  '7','8','9','C',
  '*','0','#','D'
};

byte rowPins[ROWS] = {23,22,19,18};
byte colPins[COLS] = {5,4,2,15};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int Relay1=26;
const int Relay2=25;
const int Buzzer=13;
const int LED=12;

String Password = "123ABC";
String InputPassword = "";
const int MaksimalPercobaan = 3;
int Percobaan = 0;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Farhan";
char pass[] = "tanyamama";

#define DHTPIN 21
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth,ssid,pass);
  dht.begin();

  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(Buzzer,OUTPUT);

  digitalWrite(Relay1,HIGH);
}

void loop()
{
  Blynk.run();
  char key=keypad.getKey();
  if (key)
  {
    Serial.print("Tombol ditekan: ");
    Serial.println(key);
    digitalWrite(Buzzer,HIGH);
    delay(50);
    digitalWrite(Buzzer,LOW);

    if (key=='#')
    {
      if (InputPassword==Password)
      {
        Serial.println("Password benar!");
        digitalWrite(Relay1,LOW);
        Blynk.logEvent("Pintu","Terbuka");
        delay(3000);
        digitalWrite(Relay1,HIGH);
      }
      else
      {
        Serial.println("Password salah!");
        digitalWrite(Buzzer,HIGH);
        delay(2000);
        digitalWrite(Buzzer,LOW);
        Percobaan++;
        if(Percobaan>=MaksimalPercobaan)
        {
          Blynk.logEvent("Percobaan", "3x gagal terdeteksi");
          delay(5000);
          Percobaan=0;
        }
      }
      InputPassword="";
    }

    else if (key=='*')
    {
      Serial.println("Password direset");
      InputPassword = "";
    }
    else
    {
      InputPassword+=key;
    }
  }
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (!isnan(suhu) || !isnan(kelembapan))
  {
    Blynk.virtualWrite(V2, suhu);
    Blynk.virtualWrite(V3, kelembapan);
  }
  else
  {
    Serial.println("Gagal membaca data dari sensor DHT22!");
  }
  delay(10);
}

BLYNK_WRITE(V0)
{
  int pinValue=param.asInt();
  if (pinValue==1)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED,LOW);
  }
}

BLYNK_WRITE(V1)
{
  int pinValue=param.asInt();
  if (pinValue==1)
  {
    digitalWrite(Relay2, HIGH);
  }
  else
  {
    digitalWrite(Relay2,LOW);
  }
}