uint64_t CurrentTime  = 0 ;

uint64_t       LightDelay      = 300  ;
uint64_t       PreviousLight   = 0    ;

const uint64_t GyroDelay       = 2000 ;
uint64_t       PreviousGyro    = 0    ;

const uint64_t GPSDelay        = 10000;
uint64_t       PreviousGPS     = 0    ;

const uint64_t BatteryDelay    = 60000;
uint64_t       PreviousBattery = 0    ;

#include <SoftwareSerial.h>
static const int      RXPin    = 16   ;
static const int      TXPin    = 17   ;
static const uint32_t GPSBaud  = 9600 ;
SoftwareSerial ss(RXPin, TXPin);
#include <TinyGPS++.h>
TinyGPSPlus gps;
int   Satelize  ;
float Latitude  ;   
float Longitude ;

#include <Adafruit_NeoPixel.h>
#define  NUMOFLIGHT  16  
#define  LIGHTPIN    23 
#define  LIGHTLINE    5 
Adafruit_NeoPixel strip       = Adafruit_NeoPixel(NUMOFLIGHT, LIGHTPIN, NEO_GRB + NEO_KHZ800);
String         LightStatus    = "Mid";  
String         tmpLightStatus = ""   ;  
uint16_t       LightCount     = 0    ;  

#include <Wire.h>
const int      MPU_addr       = 0x68 ;
const uint16_t AccidentAcZ    = 12000;
uint16_t       AccidentCount  = 0    ;
uint16_t       SafeCount      = 0    ;
int16_t        AcX,AcY,AcZ           ;

boolean        AlertMode      = true ;
boolean        DangerMode     = false;

#define Volt 34 
#define MAX_BATTERY 4000 
#define MIN_BATTERY 3300 
#define Factor      0.8743 
uint16_t NumMean = 10 ;

#include <esp_now.h>
#include <WiFi.h>
esp_now_peer_info_t peerInfo;

uint8_t BackPart[]    = {0x30, 0xAE, 0xA4, 0x97, 0xF5, 0xF0};
uint8_t ControlPart[] = {0x30, 0xAE, 0xA4, 0x97, 0x4A, 0xAC};
uint8_t MainPart[]    = {0x8C, 0xAA, 0xB5, 0x93, 0x4F, 0xCC};
uint8_t SensorPart[]  = {0x8C, 0xAA, 0xB5, 0x94, 0x33, 0x40};

bool MainConnected  = false ;

typedef struct Data
{
  String Name  ;
  String Value ;
} Data ;

typedef struct GPS
{
  String Name     ;
  float Latitude  ;
  float Longitude ;
  int   Satelize  ;
} GPS ;

String DataSending  ;
Data   IncomingData ;
Data   BatteryData  ;
Data   AccidentData ;
GPS    GPSData      ;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{ 
  if     (DataSending == "Battery" )MainConnected = true ;
  else if(DataSending == "Accident")
    Serial.println("Delivery Accident: " + AccidentData.Value);
  else if(DataSending == "GPS")
    Serial.println("Delivery GPS: " + String(GPSData.Latitude,6) 
    + "," + String(GPSData.Longitude,6) 
    + "," + String(GPSData.Satelize));
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
    memcpy(&IncomingData, incomingData, sizeof(IncomingData));

    Serial.println("Recieved " + IncomingData.Name + ": " + IncomingData.Value);
    if(IncomingData.Name == "Button")
    {
      if(LightStatus != "Accident" )
        LightStatus = IncomingData.Value ;
    }
    else if(IncomingData.Name == "Setting")
    {
      setCpuFrequencyMhz(getIndex(IncomingData.Value,1).toInt());
      SendBattery();
    }
}

String getIndex(String str, int Index)
{
  int tmpdelimiter = -1 ;
  int delimiter    = 0  ;

  for(int i = 1 ; i <= Index ; i++)
  {
    delimiter = str.indexOf(',',++tmpdelimiter);
    if(i == Index)return str.substring(tmpdelimiter, delimiter);
    tmpdelimiter = delimiter ;
  }
}

void Init_ESPNOW()
{
  WiFi.mode(WIFI_STA);
  esp_now_init();

  esp_now_register_send_cb(OnDataSent);
  
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  memcpy(peerInfo.peer_addr, MainPart, 6);
  esp_now_add_peer(&peerInfo);

  esp_now_register_recv_cb(OnDataRecv);
}

void setup()
{
  Serial.begin(115200);

  AccidentData.Name  = "Accident" ; 
  AccidentData.Value = "0"        ; //Accident (1, 0)

  BatteryData.Name   = "Battery"  ;
  BatteryData.Value  = "B,0"      ; //Battery (BackPart, 0 - 100)

  GPSData.Name       = "GPS"      ;
  GPSData.Latitude   = 0.000000   ; //Latitude
  GPSData.Longitude  = 0.000000   ; //Longitude
  GPSData.Satelize   = 0          ; //Satelize
  
  strip.begin();
  strip.show();
  strip.setBrightness(100);

  ss.begin(GPSBaud);

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  Init_ESPNOW();

  Serial.println("This is Back Part");
  strip.fill(strip.Color( 255, 0, 0), 0, NUMOFLIGHT); 
  strip.show();
  while(!MainConnected){Serial.print("."); delay(500); }
  for(int i = 0 ; i <= 5 ; i++)
  {
    strip.fill(strip.Color(  0, (i % 2 == 0 ? 255 : 0 ), 0), 0, NUMOFLIGHT);
    strip.show();
    delay(500);    
  }
}
 
void loop()
{
  CurrentTime = millis();
  if(CurrentTime - PreviousGPS     >= GPSDelay && LightStatus == "Mid"){PreviousGPS     = CurrentTime ; SendGPS(1000); }
  if(CurrentTime - PreviousBattery >= BatteryDelay)                    {PreviousBattery = CurrentTime ; SendBattery(); }
  if(CurrentTime - PreviousGyro    >= GyroDelay   )                    {PreviousGyro    = CurrentTime ; checkGyro();   }
  if(CurrentTime - PreviousLight   >= LightDelay  )                    {PreviousLight   = CurrentTime ; showLight();   }
}

void checkGyro()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true); 
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
    
  if(AccidentData.Value == "0" && AcZ < AccidentAcZ)
  {
    AccidentCount++ ;
    if(AccidentCount == 5)
    {
      AccidentCount      = 0          ;
      LightDelay         = 10         ;
      LightCount         = 0          ;
      AccidentData.Value = "1"        ;
      LightStatus        = "Accident" ;
      DataSending        = AccidentData.Name ;
      esp_now_send(MainPart, (uint8_t *) &AccidentData, sizeof(AccidentData));
      delay(50);
    }
  }
  else AccidentCount = 0 ;

  if(AccidentData.Value == "1" && AcZ >= AccidentAcZ)
  {
    SafeCount++;
    if(SafeCount == 5)
    {
      SafeCount          = 0       ;
      LightDelay         = 300     ;
      AccidentData.Value = "0"     ;
      LightStatus        = "Mid"   ;
      DataSending        = AccidentData.Name ;
      esp_now_send(MainPart, (uint8_t *) &AccidentData, sizeof(AccidentData));
      delay(50);
    }
  }
  else SafeCount = 0 ;
}

void showLight()
{
  strip.fill(strip.Color( 255, 0, 0), 0, NUMOFLIGHT); 
  if     (LightStatus == "Mid" )
  {
    LightCount = 0 ;
    if(tmpLightStatus != LightStatus)strip.show();
  }
  else if(LightStatus == "Left")
  {
    LightCount++ ;
    if(LightCount % 2 == 0)strip.fill(strip.Color(255, 75,  0), NUMOFLIGHT - LIGHTLINE,  LIGHTLINE);
    else                   strip.fill(strip.Color(  0,  0,  0), NUMOFLIGHT - LIGHTLINE,  LIGHTLINE);
    strip.show();
  }
  else if(LightStatus == "Right")
  { 
    LightCount++ ;
    if(LightCount % 2 == 0)strip.fill(strip.Color(255, 75,  0),  0,  LIGHTLINE);
    else                   strip.fill(strip.Color(  0,  0,  0),  0,  LIGHTLINE);
    strip.show();
  }
  else if(LightStatus == "Emergency")
  {
    LightCount++ ;
    if(LightCount % 2 == 0)
    {
      strip.fill(strip.Color(255, 75,  0),  0,  LIGHTLINE);
      strip.fill(strip.Color(255, 75,  0), NUMOFLIGHT - LIGHTLINE,  LIGHTLINE);
    }
    else
    {
      strip.fill(strip.Color(  0,  0,  0),  0,  LIGHTLINE);
      strip.fill(strip.Color(  0,  0,  0), NUMOFLIGHT - LIGHTLINE,  LIGHTLINE);
    }
    strip.show();
  }
  else if(LightStatus == "Accident")
  {
    strip.fill(strip.Color(  0, 0, 0), 0, NUMOFLIGHT);
    if(LightCount == NUMOFLIGHT)LightCount = 0 ;
    if(LightCount + LIGHTLINE > NUMOFLIGHT)
    { 
      strip.fill(strip.Color(255,  0,  0), 0,    LightCount + LIGHTLINE - NUMOFLIGHT);
      strip.fill(strip.Color(255,  0,  0), LightCount++, NUMOFLIGHT - LightCount + 1);    
    }
    else strip.fill(strip.Color(255,  0,  0),  LightCount++, LIGHTLINE);
    strip.show();
  }
  tmpLightStatus = LightStatus ;
}

void SendBattery()            
{
  int Sum = 0 ;
  for(int i = 0 ; i < NumMean ; i++)Sum += analogRead(Volt) * 2 * Factor ;
  int Value = Sum / NumMean ;
  int Level = (Value - MIN_BATTERY) / 7 > 100 ? 100 : (Value - MIN_BATTERY) / 7 < 0 ? 0 : (Value - MIN_BATTERY)/ 7 ;
  BatteryData.Value  = "B," + String(Level);
  DataSending        = BatteryData.Name ;
  esp_now_send(MainPart, (uint8_t *) &BatteryData, sizeof(BatteryData));
  delay(50);
}

static void SendGPS(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
    {
      char Text = ss.read();
      gps.encode(Text);
    }
  }
  while (millis() - start < ms);
  
  GPSData.Satelize = gps.satellites.value();
  if(gps.location.isValid()) 
  {
    GPSData.Latitude  = gps.location.lat();
    GPSData.Longitude = gps.location.lng();
    DataSending       = GPSData.Name      ;   
    esp_now_send(MainPart, (uint8_t *) &GPSData, sizeof(GPSData));
    delay(50);
  }
}
