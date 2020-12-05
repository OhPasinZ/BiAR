uint64_t CurrentTime          = 0    ; 

const uint64_t ResetDelay    = 3000 ;
uint64_t PreviousReset       = 0    ;

const uint64_t SleepDelay    = 3000 ;
uint64_t PreviousSleep       = 0    ;

const uint64_t EmergencyDelay = 500  ;
uint64_t PreviousEmergency    = 0    ;

const uint64_t BatteryDelay   = 60000;
uint64_t PreviousBattery      = 0    ;

#define LeftButton  25
#define MidButton   26
#define RightButton 27

#define RedLight    12
#define GreenLight  14

uint16_t LeftValue   = 0 ;
uint16_t MidValue    = 0 ;
uint16_t RightValue  = 0 ;

uint16_t LeftStatus  = 0 ;
uint16_t MidStatus   = 0 ;
uint16_t RightStatus = 0 ;

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

String DataSending  ;
Data   IncomingData ;
Data   ButtonData   ;
Data   BatteryData  ;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  if(DataSending == "Button")
  {
    Serial.println("Delivery Button: " + ButtonData.Value);
    digitalWrite(status == ESP_NOW_SEND_SUCCESS ? GreenLight : RedLight ,HIGH);
    delay(100);
    digitalWrite(RedLight  , LOW);
    digitalWrite(GreenLight, LOW);
  }
  else if(DataSending == "Battery")
  {
    Serial.println("Delivery Battery: " + BatteryData.Value + "%");
    MainConnected = true ;
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  memcpy(&IncomingData, incomingData, sizeof(IncomingData));

  Serial.println("Recieve " + IncomingData.Name + ": " + IncomingData.Value);
  if(IncomingData.Name == "Setting")
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
  
  memcpy(peerInfo.peer_addr, BackPart, 6);
  esp_now_add_peer(&peerInfo);
  memcpy(peerInfo.peer_addr, MainPart, 6);
  esp_now_add_peer(&peerInfo);

  esp_now_register_recv_cb(OnDataRecv);
}

void setup() 
{
  Serial.begin(115200);
  
  ButtonData.Name   = "Button"  ; 
  ButtonData.Value  = ""        ; //ButtonValue (MID, LEFT, RIGHT, EMERGENCY)

  BatteryData.Name  = "Battery" ;
  BatteryData.Value = "C,0"     ; //Battery (C,0 - 100)
  
  pinMode(RightButton, INPUT_PULLUP);
  pinMode(MidButton  , INPUT_PULLUP);
  pinMode(LeftButton , INPUT_PULLUP);

  pinMode(RedLight   , OUTPUT);
  pinMode(GreenLight , OUTPUT);

  pinMode(Volt       , INPUT);
  
  Init_ESPNOW();

  Serial.println("This is Control Part");
  delay(100);
  digitalWrite(RedLight, HIGH);
  while(!MainConnected){Serial.print(".");delay(500);}
  digitalWrite(RedLight, LOW);
  for(int i = 0 ; i <= 5 ; i++)
  {
    digitalWrite(GreenLight, (i % 2 == 0 ? HIGH : LOW ));
    delay(500);
  }
}
 
void loop() 
{
  CurrentTime = millis();
  
  LeftStatus  = !digitalRead(LeftButton );
  MidStatus   = !digitalRead(MidButton  );
  RightStatus = !digitalRead(RightButton);

  if(CurrentTime - PreviousBattery >= BatteryDelay)
  {
    PreviousBattery = CurrentTime ;
    SendBattery();
  }
  
  if(LeftStatus == HIGH && LeftValue == LOW)
  {
    LeftValue = HIGH ;
    SendButton("Left");
    PreviousReset = CurrentTime ; 
  }
  else if(LeftStatus == LOW && LeftValue == HIGH)LeftValue = LOW ;
  else if(LeftStatus == HIGH && LeftValue == HIGH)
    if(CurrentTime - PreviousReset >= ResetDelay && ButtonData.Value != "Reset")SendButton("Reset");

  if(RightStatus == HIGH && RightValue == LOW)
  {
    RightValue = HIGH ;
    SendButton("Right");
    PreviousSleep = CurrentTime ; 
  }
  else if(RightStatus == LOW && RightValue == HIGH)RightValue = LOW ;
  else if(RightStatus == HIGH && RightStatus == HIGH)
    if(CurrentTime - PreviousSleep >= SleepDelay && ButtonData.Value != "Sleep")SendButton("Sleep");

  if(MidStatus == HIGH && MidValue == LOW)
  {
    MidValue = HIGH ;
    SendButton("Mid");
    PreviousEmergency = CurrentTime ; 
  }
  else if(MidStatus == LOW  && MidValue == HIGH)MidValue = LOW ;
  else if(MidStatus == HIGH && MidValue == HIGH)
    if(CurrentTime - PreviousEmergency >= EmergencyDelay && ButtonData.Value != "Emergency")SendButton("Emergency");
}

void SendButton(String Value) 
{
    ButtonData.Value = Value    ;
    delay(300);                  
    DataSending      = ButtonData.Name ;
    if(Value != "Reset" && Value != "Sleep")
      esp_now_send(BackPart, (uint8_t *) &ButtonData, sizeof(ButtonData));
    delay(50);
    esp_now_send(MainPart, (uint8_t *) &ButtonData, sizeof(ButtonData));
    delay(50);                           
}

void SendBattery()                  
{
  int Sum = 0 ;
  for(int i = 0 ; i < NumMean ; i++)Sum += analogRead(Volt) * 2 * Factor ;
  int Value = Sum / NumMean ;
  int Level = (Value - MIN_BATTERY) / 7 > 100 ? 100 : (Value - MIN_BATTERY) / 7 < 0 ? 0 : (Value - MIN_BATTERY)/ 7 ;
  BatteryData.Value = "C," + String(Level);
  DataSending        = BatteryData.Name ;
  esp_now_send(MainPart, (uint8_t *) &BatteryData, sizeof(BatteryData));
  delay(50);
}

  
