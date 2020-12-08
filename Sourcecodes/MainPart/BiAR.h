uint64_t CurrentTime  = 0      ;
uint64_t PreviousTrip = 0      ;
uint64_t TripTime     = 0      ;

const uint64_t NotifyDelay  = 5000   ;  
uint64_t PreviousNotify     = 0      ;

const uint64_t WaitingDelay = 60000  ;
uint64_t PreviousWaiting    = 0      ;

const uint64_t SpeedDelay   = 1000   ;
uint64_t PreviousSpeed      = 0      ;

const uint64_t DetailDelay  = 5000   ;
uint64_t PreviousDetail     = 0      ;

const uint64_t BatteryDelay = 10000  ;
uint64_t PreviousBattery    = 0      ;

const uint64_t LightDelay   = 500    ;  
uint64_t PreviousLight      = 0      ;

const uint64_t BuzzerDelay  = 200    ; 
uint64_t PreviousBuzzer     = 0      ;

const uint64_t SecureDelay  = 180000 ; 
uint64_t PreviousSecure     = 0      ;

#define Shock      4                
#define Buzzer     12                 
#define GPRSModule 21
#define TFTModule  22

#define LogoWidth 250
#define LogoHeight 150
#define BicycleWidth 250
#define BicycleHeight 150
#define BaseWidth 244
#define BaseHeight 44
#define BatteryWidth 42
#define BatteryHeight 38
#define IconWidth 34
#define IconHeight 16
#define Light1Width 60
#define Light1Height 54
#define Light2Width 30
#define Light2Height 27
#define LedWidth  10
#define LedHeight 10
#define numOfLed  5
#define MainposY    180 
#define LightposX   70 
#define LightposY   80 
#define LightDeep   10 
#define BatteryposX 40 
#define BatteryposY 200 
#define BatteryDeep 20 
#define BatteryFont 16 
#define BaseDeep    10 
#define GFXFF 1
#define Grey   int(122/8) *2048 + int(122/4) *32+ int(122/8)

#define Volt        34     
#define MAX_BATTERY 4000   
#define MIN_BATTERY 3300     
#define Factor      0.8743 
#define NumMean     10      

#define LatitudePin     V1  
#define LongitudePin    V2   
#define MapPin          V3 
#define SpeedPin        V4  
#define SatelizePin     V5 
#define DistancePin     V6  
#define TimePin         V7 
#define ResetPin        V8
#define MBatteryPin     V9   
#define CBatteryPin     V10
#define BBatteryPin     V11  
#define SBatteryPin     V12  
#define AlertModePin    V14  
#define DisplayModePin  V15  
#define SecureModePin   V16 
#define WheelPin        V17 
#define TelephonePin    V18 

#define PowerKey     0
#define AlertKey     1
#define DisplayKey   2
#define SecureKey    3
#define WheelKey     4
#define TelephoneKey 9
#define DefaultKey   50

#define Df_Power     240 
#define Df_Alert     1   
#define Df_Display   1   
#define Df_Secure    1   
#define Df_Wheel     "2000" 
#define Df_Telephone "+66620581954" 
#define InitDefault  1   

uint8_t BackPart[]    = {0x30, 0xAE, 0xA4, 0x97, 0xF5, 0xF0};
uint8_t ControlPart[] = {0x30, 0xAE, 0xA4, 0x97, 0x4A, 0xAC};
uint8_t MainPart[]    = {0x8C, 0xAA, 0xB5, 0x93, 0x4F, 0xCC};
uint8_t SensorPart[]  = {0x8C, 0xAA, 0xB5, 0x94, 0x33, 0x40};

const uint16_t WheelList[] = {2097, 2105, 2136, 2155, 2168, 2180, 2224, 2293, 2325, 2169, 2151, 2155, 2145, 1953, 1985, 2055, 2070, 2288, 2298, 2326, 1995, 2100};

#define TINY_GSM_MODEM_A6
#include <TinyGsmClient.h>
char APN[]  = "www.dtac.co.th";     
char User[] = "" , Pass[] = "";       
#define SerialAT Serial2              
#define GSM_AUTOBAUD_MIN 9600        
#define GSM_AUTOBAUD_MAX 38400
TinyGsm modem(SerialAT);

#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
char Auth[] = "4_e3McGRy2HKjlRtQmCi1lzR3x8T-4nf";

bool AlertUpdated = false ;
bool DisplayUpdated = false ;
bool SecureUpdated = false ;
bool WheelUpdated = false ;
bool TelUpdated = false ;


BlynkTimer timer;
WidgetMap myMap  (MapPin);

String tmpLightStatus = "-Mid" ;
String LightStatus = "Mid" ; 
bool   goSleep     = false ;
bool   FinishSetup = false ;
bool   BlynkSetup  = false ; 
bool   Accident    = false ; 
bool   Danger      = false ;  
bool   Sound       = false ;
bool   Notify      = false ; 
bool   C_Connected = false ; 
bool   B_Connected = false ;  
bool   S_Connected = false ;
bool   tmpControl  = false ;
bool   tmpBack     = false ;
bool   tmpSensor   = false ;
short  LightStep   = 1     ;  
short  TurnLed     = 1     ; 
short LedOnPos     = 1     ;
short LedOffPos    = 1     ;
short  OffLed      = 5     ;
uint16_t C_Battery = 0     ;
uint16_t B_Battery = 0     ;
uint16_t S_Battery = 0     ;
uint16_t M_Battery = 0     ;
float  Speed       = 0     ;  
float  tmpSpeed    = -1    ; 
float  Distance    = 0     ; 

String   PowerSetting   ;
String   AlertSetting   ;
String   DisplaySetting ;
String   SecureSetting  ;
String   Telephone      ;
uint32_t WheelSize      ;

#include <FS.h>
#include <SD.h>
#include <JPEGDecoder.h>
#include <TFT_eSPI.h>
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();
  
#include <EEPROM.h>
#define EEPROM_SIZE 60

TaskHandle_t MainTask;
TaskHandle_t BlynkTask;

#include <esp_now.h>
#include <WiFi.h>
esp_now_peer_info_t peerInfo;

typedef struct Data
{
  String Name  = "Setting";
  String Value = "0,0,0"  ;
} Data ;

typedef struct GPS
{
  String Name     = "GPS" ;
  float Latitude  = 0.000 ;
  float Longitude = 0.000 ;
  byte  Satelize  = 0     ;
} GPS ;

String DataSending  ;
Data   IncomingData ;
Data   SettingData  ;
GPS    IncomingGPS  ;
