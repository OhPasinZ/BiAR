
uint8_t SensorPart[]  = {0x8C, 0xAA, 0xB5, 0x94, 0x33, 0x40};

bool MainConnected  = false ;uint64_t CurrentTime         = 0     ;

const uint64_t StatDelay     = 1000    ;
uint64_t PreviousStat        = 0       ;

uint64_t SpeedDelay          = 100   ;
uint64_t PreviousSpeed       = 0     ;

uint64_t UpDateSpeedDelay    = 1000  ;
uint64_t PreviousUpdateSpeed = 0     ;

const uint64_t BatteryDelay  = 60000 ;
uint64_t PreviousBattery     = 0     ;

const uint64_t CheckDelay = 100 ;
uint64_t PreviousCheck    = 0  ;

int buf[10],temp ;
int Index = 0 ;

TaskHandle_t MainTask;
TaskHandle_t BlynkTask;

#define RedLight    14
#define GreenLight  12

#include <NewPing.h>
#define TRIGGER_PIN     26
#define ECHO_PIN        27
#define MAX_DISTANCE    450
#define DANGER_DISTANCE 250
int      Distance       = 0 ;
int      tmpDistance    = 0 ;
uint16_t AlertMode      = 1 ;
int      CpuFrequency   = 240 ;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#define  HallPin 25
uint8_t  HallStatus          = 0    ;
uint8_t  HallValue           = 0    ;

double   Wheel               = 2096 ;
double   Speed               = 0    ;
uint16_t Count               = 0    ;
boolean  Riding              = false;

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

typedef struct Data
{
  String Name  ;
  String Value ;
} Data ;

String DataSending  ;
String tmpData      ;
Data   IncomingData ;
Data   StatData     ;
Data   BatteryData  ;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  if     (DataSending == "Stat" )
  {
    Count = 0 ;
    Serial.println("Delivery Stat: "  + StatData.Value);
  }
  else if(DataSending == "Battery")
  {
    Serial.println("Delivery Battery: " + BatteryData.Value + " %");
    MainConnected = true ;
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  memcpy(&IncomingData, incomingData, sizeof(IncomingData));
  Serial.println("Test");
  Serial.println("Recieve " + IncomingData.Name + ": " + IncomingData.Value);
  if(IncomingData.Name == "Setting")
  {  
    setCpuFrequencyMhz(getIndex(IncomingData.Value,1).toInt());
    AlertMode        = getIndex(IncomingData.Value,2).toInt();
    Wheel            = getIndex(IncomingData.Value,3).toInt();
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

  StatData.Name   = "Stat"       ;
  StatData.Value  = "0,0,0"      ; //Speed, CountDistance, Alert (0 - infi,0 - infi, 0 - 1)

  BatteryData.Name  = "Battery"  ;
  BatteryData.Value = "S,0"      ; //Battery (StatPart,0 - 100)

  pinMode(RedLight   , OUTPUT);
  pinMode(GreenLight , OUTPUT);
  
  pinMode(HallPin, INPUT_PULLUP); 
  
  Init_ESPNOW();

  Serial.println("This is Sensor Part");
  digitalWrite(RedLight, HIGH);
  while(!MainConnected){Serial.print(".");delay(500);}
  digitalWrite(RedLight, LOW);
  for(int i = 0 ; i <= 5 ; i++)
  {
    digitalWrite(GreenLight, (i % 2 == 0 ? HIGH : LOW ));
    delay(500);
  }

  xTaskCreatePinnedToCore(MainTaskcode , "MainTask" , 10000, NULL, 1, &MainTask , 0); disableCore0WDT(); delay(500);
  xTaskCreatePinnedToCore(BlynkTaskcode, "DistanceTask", 10000, NULL, 1, &BlynkTask, 1); disableCore1WDT(); delay(500);

}

void MainTaskcode( void * pvParameters )
{
  Serial.print("MainTask running on core ");
  Serial.println(xPortGetCoreID());
  
  while(true)
  {
    CurrentTime = millis();
    HallStatus = !digitalRead(HallPin);
    
    if(Speed >= 1)Riding = true  ;
    else          Riding = false ;
  
    if(CurrentTime - PreviousBattery >= BatteryDelay)
    {
      PreviousBattery = CurrentTime ;
      SendBattery();
    }
    
    if(HallStatus == HIGH && HallValue == LOW)
    {
      Count++ ;
      HallValue           = HIGH ;
      Speed               = float(Wheel) / (float(CurrentTime - PreviousSpeed)/3.6) ;
      PreviousSpeed       = CurrentTime ;
      PreviousUpdateSpeed = CurrentTime ;
      Serial.println("Speed: " + String(int(Speed)));
    }
    else if(HallStatus == LOW && HallValue == HIGH)HallValue = LOW ;
  
    if(CurrentTime - PreviousUpdateSpeed >= UpDateSpeedDelay && Riding)
    {
      PreviousUpdateSpeed = CurrentTime ;
      Speed               = (float)Wheel / (float)((CurrentTime - PreviousSpeed)/3.6) ;
    }
    
    if(CurrentTime - PreviousStat  >= StatDelay)
    {
      PreviousStat   = CurrentTime ;
      StatData.Value = String(int(Speed)) + "," + String(Count) + "," ;
      switch(AlertMode)
      {
        case 0:
          StatData.Value += "0" ;
        break;
        case 1:
          if(Distance < DANGER_DISTANCE)StatData.Value += "1" ;
          else                          StatData.Value += "0" ;
        break;
        case 2:
          if(Speed >= 1)
          {
            if(Distance < DANGER_DISTANCE)StatData.Value += "1" ;
            else                          StatData.Value += "0" ;
          }
          else          StatData.Value += "0" ;
          
        break;
      }
      Distance = 1 ;
      if(tmpData != StatData.Value)
      {
        tmpData     = StatData.Value ;
        DataSending = StatData.Name ;
        esp_now_send(MainPart, (uint8_t *) &StatData, sizeof(StatData));
        delay(50);
      }
    }
  }
}

void BlynkTaskcode( void * pvParameters )
{
  Serial.print("Distance running on core ");
  Serial.println(xPortGetCoreID());
  while (true)
  {
    if(CurrentTime - PreviousCheck >= CheckDelay)
    {
      do{buf[Index]=int(sonar.ping_cm());}while(buf[Index] == 0);
      if(Index + 1 == 10)
      {
        Index = 0 ;
        for(int i=0;i<9;i++)        //sort the analog from small to large
        {
          for(int j=i+1;j<10;j++)
          {
            if(buf[i]>buf[j])
            {
              temp=buf[i];
              buf[i]=buf[j];
              buf[j]=temp;
            }
          }
        }  
        int avgValue= 0;
        for(int i=2;i<7;i++)avgValue+=buf[i];
        Distance = avgValue/6;
      }
      else Index++ ;
    }
  }
}

void loop() {}

void SendBattery()  
{
  int Sum = 0 ;
  for(int i = 0 ; i < NumMean ; i++)Sum += analogRead(Volt) * 2 * Factor ;
  int Value = Sum / NumMean ;
  int Level = (Value - MIN_BATTERY) / 7 > 100 ? 100 : (Value - MIN_BATTERY) / 7 < 0 ? 0 : (Value - MIN_BATTERY)/ 7 ;
  BatteryData.Value = "S," + String(Level);
  DataSending        = BatteryData.Name ;
  esp_now_send(MainPart, (uint8_t *) &BatteryData, sizeof(BatteryData));
  delay(50);
}
