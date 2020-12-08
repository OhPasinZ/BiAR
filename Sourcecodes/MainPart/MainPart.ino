#include "BiAR.h"

void showNotify(String Text)
{
  if(DisplaySetting == "1")
  {
    Notify = true ;
    PreviousNotify = CurrentTime ;
    tft.setFreeFont(&Azonix5pt7b);
    tft.setTextColor(TFT_GREEN);
    tft.setTextDatum(C_BASELINE);
    tft.fillRect  (BatteryWidth , tft.height() - tft.fontHeight(GFXFF) - 1, tft.width() - (2 * BatteryWidth) - 1, tft.fontHeight(GFXFF) + 1, TFT_BLACK);
    tft.drawString(Text, tft.width() / 2 - 1  , tft.height() - 1, GFXFF); 
  }
}

void TextLoading(String Detail)
{
  tft.fillRect  (0, (tft.height() - (tft.height() / 2 + LogoHeight / 2)) / 2 + (tft.height() / 2 + LogoHeight / 2) - tft.fontHeight(GFXFF) / 2, tft.width(), tft.fontHeight(GFXFF) + 1, TFT_WHITE);
  tft.drawString(Detail, tft.width() / 2 , (tft.height() - (tft.height() / 2 + LogoHeight / 2)) / 2 + (tft.height() / 2 + LogoHeight / 2), GFXFF);
}

void LedLoading(int Delay)
{
  for (int i = 0 ; i < Delay ; i += 5)
  {
    drawSdJpeg("/LedOff.jpg", LedOffPos , tft.height() / 2 + LogoHeight / 2);
    if(OffLed + 1 <=  numOfLed)
    {
      OffLed++;
      LedOffPos += 3 * LedWidth;
    }
    else
    {
      OffLed = 1 ;
      LedOffPos = tft.width() / 2 - ((numOfLed / 2) * (3 * LedWidth) + (LedWidth / 2)) ;
    }
    drawSdJpeg("/LedOn.jpg", LedOnPos , tft.height() / 2 + LogoHeight / 2);
    if(TurnLed + 1 <= numOfLed)
    {
      TurnLed++;
      LedOnPos += 3 * LedWidth ;
    }
    else
    {
      TurnLed = 1 ;
      LedOnPos = tft.width() / 2 - ((numOfLed / 2) * (3 * LedWidth) + (LedWidth / 2)) ;
    }
    delay(500);
  }
}

void ConnectionLoading()
{
  short posX = 333 ;
  short posY = 80  ;
  short next = 55  ;
  
  tft.drawString("Main Part", posX , posY , GFXFF);
  tft.fillRect (posX, tft.fontHeight(GFXFF) / 2 + posY, tft.width() - posX, tft.fontHeight(GFXFF), TFT_WHITE);
  tft.drawString(String(M_Battery) + " %", posX , tft.fontHeight(GFXFF) + posY , GFXFF);

  posY += next ;

  tft.drawString("Control Part", posX , posY , GFXFF);
  if (!C_Connected)tft.drawString("Waiting...", posX , tft.fontHeight(GFXFF) + posY , GFXFF);
  else
  {
    if (!tmpControl)
    {
      tft.fillRect (posX, tft.fontHeight(GFXFF) / 2 + posY, tft.width() - posX, tft.fontHeight(GFXFF), TFT_WHITE);
      tft.drawString("Connected!"                     , posX , tft.fontHeight(GFXFF) + posY , GFXFF); delay(1000);
      tft.fillRect (posX, tft.fontHeight(GFXFF) / 2 + posY, tft.width() - posX, tft.fontHeight(GFXFF), TFT_WHITE);
      tft.drawString(String(C_Battery) + " %", posX , tft.fontHeight(GFXFF) + posY , GFXFF);
      tmpControl = true ;
    }
  }

  posY += next ;
  tft.drawString("Back Part", posX , posY , GFXFF);
  if (!B_Connected   )tft.drawString("Waiting...", posX , tft.fontHeight(GFXFF) + posY , GFXFF);
  else
  {
    if (!tmpBack)
    {
      tft.fillRect (posX, tft.fontHeight(GFXFF) / 2 + posY, tft.width() - posX, tft.fontHeight(GFXFF), TFT_WHITE);
      tft.drawString("Connected!"                  , posX , tft.fontHeight(GFXFF) + posY , GFXFF); delay(1000);
      tft.fillRect (posX, tft.fontHeight(GFXFF) / 2 + posY, tft.width() - posX, tft.fontHeight(GFXFF), TFT_WHITE);
      tft.drawString(String(B_Battery) + " %", posX , tft.fontHeight(GFXFF) + posY , GFXFF);
      tmpBack = true ;
    }
  }

  posY += next ;
  tft.drawString("Sensor Part", posX , posY , GFXFF);
  if (!S_Connected )tft.drawString("Waiting...", posX , tft.fontHeight(GFXFF) + posY , GFXFF);
  else
  {
    if (!tmpSensor)
    {
      tft.fillRect (posX, tft.fontHeight(GFXFF) / 2 + posY, tft.width() - posX, tft.fontHeight(GFXFF), TFT_WHITE);
      tft.drawString("Connected!"                    , posX , tft.fontHeight(GFXFF) + posY , GFXFF); delay(1000);
      tft.fillRect (posX, tft.fontHeight(GFXFF) / 2 + posY, tft.width() - posX, tft.fontHeight(GFXFF), TFT_WHITE);
      tft.drawString(String(S_Battery) + " %", posX , tft.fontHeight(GFXFF) + posY , GFXFF);
      tmpSensor = true ;
    }
  }
  delay(500);
}

void showBackground()
{
  drawSdJpeg("/Background.jpg", 0, 0);
  delay(500);
  drawSdJpeg("/Base.jpg", tft.width() / 2 - BaseWidth / 2, tft.height() / 2 - BaseHeight / 2 + BaseDeep);
  delay(200);
  drawSdJpeg("/RightBattery.jpg", tft.width() - BatteryWidth - 1, BatteryposY);
  drawSdJpeg("/LeftBattery.jpg" , 0, BatteryposY);
  delay(100);
  drawSdJpeg("/RightBattery.jpg", tft.width() - BatteryWidth - 1, BatteryposY + BatteryHeight + BatteryDeep);
  drawSdJpeg("/LeftBattery.jpg" , 0, BatteryposY + BatteryHeight + BatteryDeep);
  delay(100);
  drawSdJpeg("/GPS.jpg"      , tft.width() - (1 * IconWidth), 0);
  drawSdJpeg("/Internet.jpg" , tft.width() - (2 * IconWidth), 0);
  delay(100);
}

void showSpeed()
{
  if (tmpSpeed != Speed)
  {
    tft.setFreeFont(&digitalplayi50pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(C_BASELINE);
    tft.fillRect(tft.width() / 2 - 75, tft.height() / 2 - 20, 150, 20, TFT_BLACK);
    tft.fillRect(tft.width() / 2 - 75, tft.height() / 2 - 120, 170, 100, TFT_BLACK);
    tft.drawNumber(Speed < 100 ? int(Speed) : 99, tft.width() / 2 , tft.height() / 2 - 5);
    tmpSpeed = Speed ;
  }
}

void showDetail()
{
  tft.setTextColor(TFT_WHITE);

  tft.setFreeFont(&Azonix5pt7b);
  tft.setTextDatum(TC_DATUM);
  tft.fillRect(tft.width() - (3 * IconWidth), IconHeight + 3, 3 * IconWidth, tft.fontHeight(GFXFF), TFT_BLACK);
  tft.drawNumber(IncomingGPS.Satelize    , tft.width() - (1 * IconWidth) + IconWidth / 2 , IconHeight + 3);
  if (!Blynk.connected())tft.drawString("x" , tft.width() - (2 * IconWidth) + IconWidth / 2 , IconHeight + 3);
  
  tft.setFreeFont(&digitalplayi13pt7b);
  tft.setTextDatum(MC_DATUM);

  tft.fillRect(80, 225, 120, tft.fontHeight(GFXFF), TFT_BLACK);
  tft.drawString(getTime(), 140 , 250);

  char Dis[] = "" ;
  sprintf(Dis, "%.1f km", Distance < 1000 ? Distance : 999.9);
  tft.fillRect(250, 225, 160, tft.fontHeight(GFXFF), TFT_BLACK);
  tft.drawString(Dis, 330 , 250);

  int BBposX = BatteryWidth - 8 ;
  int BBposY = BatteryposY + 4 ;
  int BBHeight = BatteryHeight - 8 ;

  tft.fillRect(M_Battery     / 3, BBposY, BBposX - M_Battery / 3, BBHeight, TFT_BLACK);
  tft.fillRect(C_Battery  / 3, BBposY + BatteryHeight + BatteryDeep, BBposX - C_Battery / 3, BBHeight, TFT_BLACK);
  tft.fillRect(tft.width() - BBposX - 1, BBposY, BBposX - S_Battery / 3, BBHeight, TFT_BLACK);
  tft.fillRect(tft.width() - BBposX - 1, BBposY + BatteryHeight + BatteryDeep, BBposX - B_Battery / 3, BBHeight, TFT_BLACK);

  tft.setTextColor(TFT_WHITE);

  tft.setFreeFont(&Azonix9pt7b);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("M", BatteryFont , BatteryposY + BatteryHeight / 2);
  tft.drawString("C", BatteryFont , BatteryposY + BatteryHeight + BatteryDeep + BatteryHeight / 2);
  tft.drawString("S", tft.width() - BatteryFont , BatteryposY + BatteryHeight / 2);
  tft.drawString("B", tft.width() - BatteryFont , BatteryposY + BatteryHeight + BatteryDeep + BatteryHeight / 2);

  tft.setFreeFont(&Azonix5pt7b);
  tft.setTextDatum(TC_DATUM);
  tft.fillRect(0, BatteryposY + BatteryHeight, BatteryWidth, BatteryDeep, TFT_BLACK);
  tft.fillRect(0, BatteryposY + 2 * BatteryHeight + BatteryDeep, BatteryWidth, BatteryDeep, TFT_BLACK);
  tft.fillRect(tft.width() - BatteryWidth, BatteryposY + BatteryHeight, BatteryWidth, BatteryDeep, TFT_BLACK);
  tft.fillRect(tft.width() - BatteryWidth, BatteryposY + 2 * BatteryHeight + BatteryDeep, BatteryWidth, BatteryDeep, TFT_BLACK);
  tft.drawString(String(M_Battery) + "%", BatteryFont , BatteryposY + BatteryHeight / 2 + BatteryDeep);
  tft.drawString(String(C_Battery) + "%", BatteryFont , BatteryposY + BatteryHeight + BatteryDeep + BatteryHeight / 2 + BatteryDeep);
  tft.drawString(String(S_Battery) + "%", tft.width() - BatteryFont , BatteryposY + BatteryHeight / 2 + BatteryDeep);
  tft.drawString(String(B_Battery) + "%", tft.width() - BatteryFont , BatteryposY + BatteryHeight + BatteryDeep + BatteryHeight / 2 + BatteryDeep);

}

void LoadingPage()
{
  tft.setTextDatum(MC_DATUM );
  tft.setFreeFont(&FreeMono12pt7b);
  
  if(!SD.begin()) 
  {
    Serial.println("Card Mount Failed");
    tft.fillScreen  (TFT_BLACK);
    tft.setTextColor(TFT_RED  );
    tft.drawString("Card Mount Failed",tft.width()/2 , tft.height()/2);
    return;
  }
  else
  {
    delay(1000);
    tft.fillScreen  (TFT_WHITE);
    delay(200);
    tft.setTextColor(TFT_BLACK);
    drawSdJpeg("/Logo.jpg", tft.width() / 2 - LogoWidth / 2 , tft.height() / 2 - LogoHeight / 2 - 20);
    delay(1000);

    int xPos = tft.width() / 2 - ((numOfLed / 2) * (3 * LedWidth) + (LedWidth / 2)) ;
    for (int num = 1 ; num <= numOfLed ; num++)
    {
      tft.fillRect (xPos, tft.height() / 2 + LogoHeight / 2, LedWidth, LedHeight, TFT_WHITE);
      drawSdJpeg("/LedOff.jpg", xPos , tft.height() / 2 + LogoHeight / 2);
      xPos += 3 * LedWidth ;
      delay(100);
    }
    LedOnPos = tft.width() / 2 - ((numOfLed / 2) * (3 * LedWidth) + (LedWidth / 2)) ;
    LedOffPos = LedOnPos + ( 12 * LedWidth );
  }
}

void showLight()
{
  if (LightStatus == "Left" || LightStatus == "Emergency")
  {
    switch (LightStep)
    {
      case 1: tft.fillRect(LightposX - Light2Width, LightposY, Light1Width + Light2Width, Light1Height, TFT_BLACK);
        tft.fillRect(tft.width() - LightposX - Light1Width, LightposY, Light1Width + Light2Width, Light1Height, TFT_BLACK); break;
      case 2: drawSdJpeg("/LeftLight1.jpg", LightposX, LightposY); break;
      case 3: drawSdJpeg("/LeftLight2.jpg", LightposX - Light2Width, LightposY + LightDeep); break;
    }
  }
  if (LightStatus == "Right" || LightStatus == "Emergency")
  {
    switch (LightStep)
    {
      case 1: tft.fillRect(tft.width() - LightposX - Light1Width, LightposY, Light1Width + Light2Width, Light1Height, TFT_BLACK);
        tft.fillRect(LightposX - Light2Width, LightposY, Light1Width + Light2Width, Light1Height, TFT_BLACK); break;
      case 2: drawSdJpeg("/RightLight1.jpg", tft.width() - LightposX - Light1Width, LightposY); break;
      case 3: drawSdJpeg("/RightLight2.jpg", tft.width() - LightposX, LightposY + LightDeep); break;
    }
  }
  if (LightStatus == "Mid")
  {
    tft.fillRect(LightposX - Light2Width, LightposY, Light1Width + Light2Width, Light1Height, TFT_BLACK);
    tft.fillRect(tft.width() - LightposX - Light1Width, LightposY, Light1Width + Light2Width, Light1Height, TFT_BLACK);   
  }  
  if (LightStep == 3)LightStep = 1 ; else LightStep++ ;
}

void goDeepSleep()
{
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
  {
    Serial.println("Secure!");
    
    Danger = true ;
    PreviousWaiting = millis();
    PreviousBuzzer  = millis();
    while(millis() - PreviousWaiting < WaitingDelay && !Blynk.connected())
    {
      Blynk.run();
      if(millis() - PreviousBuzzer >= BuzzerDelay)
      {
        PreviousBuzzer = millis(); 
        openBuzzer();
      }
    }
    if(Blynk.connected())ESP.restart();
  }
  Serial.println("Going to Sleep");
  for(int i = 0 ; i < 5 ; i++)
  {
    Serial.println(i);
    delay(500);
  }
  int Value = digitalRead(Shock);
  int Detect = Value == 0 ? 1 : 0 ;
  Serial.println("Status of Shock is " + String(Value));
  Serial.println("Detect when Shock is " + String(Detect));
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, Detect);
  digitalWrite(TFTModule , LOW);
  digitalWrite(GPRSModule, LOW);
  Danger = false ;
  Blynk.disconnect();
  esp_now_deinit();
  delay(5000);
  esp_light_sleep_start();
}

void openBuzzer()
{
  if (Danger)
  {
    pinMode(Buzzer, OUTPUT);
    digitalWrite(Buzzer, Sound);
    Sound = !Sound ;
  }
  else pinMode(Buzzer, INPUT);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  memcpy(&IncomingData, incomingData, sizeof(IncomingData));

  if     (IncomingData.Name == "Stat")
  {
    Serial.println("Recieve " + IncomingData.Name + " : " + IncomingData.Value);
    Speed     = getIndex(IncomingData.Value, 1).toInt();
    Distance += getIndex(IncomingData.Value, 2).toFloat() * float(WheelSize) / 1000000.0;
    Danger    = getIndex(IncomingData.Value, 3).toInt();
  }
  else if(IncomingData.Name == "Button")
  {
    Serial.println("Recieve " + IncomingData.Name + " : " + IncomingData.Value);
    if     (IncomingData.Value == "Sleep" && SecureSetting == "1")goSleep = true ;
    else if(IncomingData.Value == "Reset")ResetTrip();
    else   
    {
      LightStatus =  IncomingData.Value ;
      LightStep = 1 ;
    }
  }
  else if (IncomingData.Name == "Battery" )
  {
    Serial.println("Recieve " + IncomingData.Name + " : " + IncomingData.Value);
    String Part    = getIndex(IncomingData.Value, 1);
    int    Battery = getIndex(IncomingData.Value, 2).toInt() ;

    PreviousSecure = CurrentTime ;

    if      (Part == "C"){if (Battery >= 0)C_Connected = true ; C_Battery = Battery ;}
    else if (Part == "B"){if (Battery >= 0)B_Connected = true ; B_Battery = Battery ;}
    else if (Part == "S"){if (Battery >= 0)S_Connected = true ; S_Battery = Battery ;}
  }
  else if (IncomingData.Name == "Accident")
  {
    Serial.println("Recieve " + IncomingData.Name + " : " + IncomingData.Value);
    Danger = IncomingData.Value.toInt() ;
    Accident = Danger ;
  }
  else
  {
    memcpy(&IncomingGPS, incomingData, sizeof(IncomingGPS));
    Serial.println("Recieve " + IncomingGPS.Name + " : " + String(IncomingGPS.Latitude, 6) + "," + String(IncomingGPS.Longitude, 6) + "," + IncomingGPS.Satelize);
  }
}

void Init_ESPNOW()
{
  Serial.println("Init ESP-NOW");
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  memcpy(peerInfo.peer_addr, ControlPart, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(peerInfo.peer_addr, BackPart   , 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(peerInfo.peer_addr, SensorPart , 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
void Init_GSM()
{
  digitalWrite(GPRSModule, HIGH);
  delay(3000);
  TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
  String info = modem.getModemInfo();
  Serial.println(info);
  while (!modem.restart())       {} Serial.println("Modem restart OK");
  while (!modem.waitForNetwork()){} Serial.println("Modem Network OK");
}

void Init_Blynk()
{
  Serial.println("Init Blynk");
  Blynk.setDeviceName("BiAR-01");
  Blynk.begin(Auth);
  delay(200);
}

void Init_Setting()
{
  Serial.println("Configulation");
  if(EEPROM.read(DefaultKey) != 1)DefaultSetting();
  PowerSetting   = String(EEPROM.read(PowerKey  )); TextLoading("Update Power Setting"  ); LedLoading(5); Serial.println("PowerSetting: "   + PowerSetting);
  AlertSetting   = String(EEPROM.read(AlertKey  )); TextLoading("Update Alert Setting"  ); LedLoading(5); Serial.println("AlertSetting: "   + AlertSetting);
  DisplaySetting = String(EEPROM.read(DisplayKey)); TextLoading("Update Display Setting"); LedLoading(5); Serial.println("DisplaySetting: " + DisplaySetting);
  SecureSetting  = String(EEPROM.read(SecureKey )); TextLoading("Update Secure Setting" ); LedLoading(5); Serial.println("SecureSetting: "  + SecureSetting);
  WheelSize      = readString(WheelKey).toInt();    TextLoading("Update WheelSize"      ); LedLoading(5); Serial.println("WheelSize: "      + String(WheelSize));
  Telephone      = readString(TelephoneKey);        TextLoading("Update Telephone"      ); LedLoading(5); Serial.println("Telephone: "      + Telephone);
  SettingData.Value = PowerSetting + "," + AlertSetting + "," + String(WheelSize);
  M_Battery   = getMainPartBattery();
}

void DefaultSetting()
{
  Serial.println("Default Setting");
  TextLoading("Default Setting");
  EEPROM.write(PowerKey    , Df_Power     );
  EEPROM.write(AlertKey    , Df_Alert     );
  EEPROM.write(DisplayKey  , Df_Display   );
  EEPROM.write(SecureKey   , Df_Secure    );
  writeString (WheelKey    , Df_Wheel     );
  writeString (TelephoneKey, Df_Telephone );
  EEPROM.write(DefaultKey  , InitDefault  );
  EEPROM.commit();
}

void Init_Connection()
{
  Serial.println("Init Connection");
  tft.fillScreen  (TFT_WHITE);
  drawSdJpeg("/Connection1.jpg", 0 , 63);
  delay(500);
  tft.fillRect  (0, 0, tft.width(), tft.fontHeight(1) + 4 , Grey);
  tft.setFreeFont(&FreeMonoBold9pt7b);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Device Connection", tft.width() / 2 - 1 , tft.fontHeight(1) / 2 + 2 , 1);
  tft.setFreeFont(&FreeMono9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(ML_DATUM);
  Serial.println("Waiting Connection");
}

void WaitingConnection()
{
  ConnectionLoading();delay(500);
  while (!C_Connected || !B_Connected || !S_Connected)
  {
    if (!C_Connected){esp_now_send(ControlPart, (uint8_t *) &SettingData, sizeof(SettingData)); ConnectionLoading();}
    if (!B_Connected){esp_now_send(BackPart   , (uint8_t *) &SettingData, sizeof(SettingData)); ConnectionLoading();}
    if (!S_Connected){esp_now_send(SensorPart , (uint8_t *) &SettingData, sizeof(SettingData)); ConnectionLoading();}
  }
  ConnectionLoading();delay(500);
}

void setup()
{ 
  Serial.begin(115200);
  Serial.println("Start!");
  
  EEPROM.begin(EEPROM_SIZE);
  pinMode(GPRSModule, OUTPUT );
  pinMode(TFTModule , OUTPUT );
  pinMode(Shock     , INPUT  );
  pinMode(Buzzer    , OUTPUT );
  digitalWrite(Buzzer   , LOW );
  digitalWrite(TFTModule, HIGH);
  delay(500);
  tft.init();
  tft.setRotation(1);
  delay(500);
  
  LoadingPage();    
  Serial.println("Loading");       
  TextLoading("Loading"      );                 LedLoading(5);
  TextLoading("Configulation"); Init_Setting(); LedLoading(5);
  TextLoading("Init Blynk   "); Init_Blynk();   LedLoading(5);
  TextLoading("Init ESP-NOW" ); Init_ESPNOW();  LedLoading(5); 
  TextLoading("Completed"    );                 LedLoading(10);
  Serial.println("Completed");
  Init_Connection(); WaitingConnection();

  FinishSetup = true ;
  PreviousTrip = CurrentTime ;
  if(DisplaySetting != "1")digitalWrite(TFTModule, LOW);
  showBackground();
  
}


void loop()
{
  if(Blynk.connected())timer.run();
  Blynk.run();
  CurrentTime = millis();
  if(goSleep)goDeepSleep();
  if  (CurrentTime - PreviousBuzzer   >= BuzzerDelay ) { PreviousBuzzer  = CurrentTime; openBuzzer ();}
  if(DisplaySetting == "1")
  {
    if (CurrentTime - PreviousSpeed   >= SpeedDelay  ){ PreviousSpeed   = CurrentTime; showSpeed  ();}
    if (CurrentTime - PreviousDetail  >= DetailDelay ){ PreviousDetail  = CurrentTime; showDetail ();}
    if (CurrentTime - PreviousLight   >= LightDelay  ){ PreviousLight   = CurrentTime; showLight  ();}
    if (CurrentTime - PreviousNotify >= NotifyDelay && Notify)
    tft.fillRect  (BatteryWidth , tft.height() - tft.fontHeight(GFXFF) - 1, tft.width() - (2 * BatteryWidth) - 1, tft.fontHeight(GFXFF) + 1, TFT_BLACK);
  }
  if (Accident)
  {
    Init_GSM();
    showNotify("Calling Emergency");
    modem.callNumber(Telephone);
    Accident = false ;
    digitalWrite(GPRSModule, LOW);
  } 
}

void ResetTrip()
{
  showNotify("Reset Trip!");
  PreviousTrip = CurrentTime ;
  Distance     = 0           ;
  LightStatus  = "Mid"      ;
  if(Blynk.connected())
  {
    Blynk.virtualWrite(DistancePin , Distance );
    Blynk.virtualWrite(TimePin     , getTime());
  }
}

String getTime()
{
  TripTime      = (millis() - PreviousTrip) / 1000 ;
  String Hour   = (TripTime / 3600)      < 10 ? "0" + String(int(TripTime / 3600))      : String(int(TripTime / 3600))      ;
  String Minute = (TripTime % 3600 / 60) < 10 ? "0" + String(int(TripTime % 3600 / 60)) : String(int(TripTime % 3600 / 60)) ;
  return Hour + " : " + Minute ;
}

int getMainPartBattery()
{
  int Sum = 0 , Value = 0;
  for (int i = 0 ; i < NumMean ; i++)Sum += analogRead(Volt) * 2 * Factor ;
  Value = ((Sum / NumMean) - MIN_BATTERY)/ 7 ;
  if     (Value > 100) return 100   ;
  else if(Value < 0  ) return 0     ;
  else                 return Value ;
}

void updateBattery()
{
  M_Battery = getMainPartBattery();
  Blynk.virtualWrite(MBatteryPin, M_Battery );
  Blynk.virtualWrite(CBatteryPin, C_Battery );
  Blynk.virtualWrite(BBatteryPin, B_Battery );
  Blynk.virtualWrite(SBatteryPin, S_Battery );
}

void updateHome()
{
  Blynk.virtualWrite(LatitudePin , IncomingGPS.Latitude );
  Blynk.virtualWrite(LongitudePin, IncomingGPS.Longitude);
  Blynk.virtualWrite(SatelizePin , IncomingGPS.Satelize );
  Blynk.virtualWrite(SpeedPin    , Speed                );
  Blynk.virtualWrite(DistancePin , Distance             );
  Blynk.virtualWrite(TimePin     , getTime()            );

  if(IncomingGPS.Latitude != 0 || IncomingGPS.Longitude != 0)
    myMap.location(1, IncomingGPS.Latitude, IncomingGPS.Longitude, "Your Bike");
}

void writeString(int address, String data)
{
  int stringSize = data.length();
  for (int i = 0 ; i < stringSize ; i++)
    EEPROM.write(address + i, data[i]);
  EEPROM.write(address + stringSize, '\0');
}

String readString(int address)
{
  char data[100];
  int len = 0;
  unsigned char k;
  k = EEPROM.read(address);
  while (k != '\0' && len < 100)
  {
    k = EEPROM.read(address + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  return String(data);
}

String getIndex(String str, int Index)
{
  int tmpdelimiter = -1 , delimiter = 0  ;

  for (int i = 1 ; i <= Index ; i++)
  {
    delimiter = str.indexOf(',', ++tmpdelimiter);
    if (i == Index)return str.substring(tmpdelimiter, delimiter);
    tmpdelimiter = delimiter ;
  }
}

void drawSdJpeg(const char *filename, int xpos, int ypos)
{
  if (JpegDec.decodeSdFile(SD.open( filename, FILE_READ)))
  {
    uint16_t *pImg;
    uint16_t mcu_w = JpegDec.MCUWidth;
    uint16_t mcu_h = JpegDec.MCUHeight;
    uint32_t max_x = JpegDec.width;
    uint32_t max_y = JpegDec.height;

    bool swapBytes = tft.getSwapBytes();
    tft.setSwapBytes(true);

    uint32_t min_w = jpg_min(mcu_w, max_x % mcu_w);
    uint32_t min_h = jpg_min(mcu_h, max_y % mcu_h);

    uint32_t win_w = mcu_w;
    uint32_t win_h = mcu_h;

    uint32_t drawTime = millis();

    max_x += xpos;
    max_y += ypos;

    while (JpegDec.read())
    {
      pImg = JpegDec.pImage ;

      int mcu_x = JpegDec.MCUx * mcu_w + xpos;
      int mcu_y = JpegDec.MCUy * mcu_h + ypos;

      if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
      else win_w = min_w;

      if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
      else win_h = min_h;

      if (win_w != mcu_w)
      {
        uint16_t *cImg;
        int p = 0;
        cImg = pImg + win_w;
        for (int h = 1; h < win_h; h++)
        {
          p += mcu_w;
          for (int w = 0; w < win_w; w++)
          {
            *cImg = *(pImg + w + p); 
            cImg++;
          }
        }
      }
      uint32_t mcu_pixels = win_w * win_h;

      if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
        tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
      else if ( (mcu_y + win_h) >= tft.height())
        JpegDec.abort();
    }
    tft.setSwapBytes(swapBytes);
  }
}

BLYNK_WRITE(ResetPin) 
{
  ResetTrip();
  showDetail();
}

BLYNK_WRITE(AlertModePin)
{
  String tmpAlert = AlertSetting ;
  switch (param.asInt())
  {
    case 1: AlertSetting = "0" ; break; // Off
    case 2: AlertSetting = "1" ; break; // On
    case 3: AlertSetting = "2" ; break; // Reduce
  }
  AlertUpdated = true ;
  if(tmpAlert != AlertSetting)
  {
    Serial.println("Update AlertMode: " + AlertSetting);
    EEPROM.write(1, byte(AlertSetting.toInt()));
    EEPROM.commit();
  
    if (FinishSetup)
    {
      showNotify("Updated Alert Mode");
      SettingData.Value = PowerSetting + "," + AlertSetting + "," + String(WheelSize);
      esp_now_send(SensorPart , (uint8_t *) &SettingData, sizeof(SettingData)); delay(100);
    }
  }
  
}
BLYNK_WRITE(DisplayModePin)
{
  String tmpDisplay = DisplaySetting ;
  switch (param.asInt())
  {
    case 1: DisplaySetting = "0" ; break; // Off
    case 2: DisplaySetting = "1" ; break; // On
  }
  DisplayUpdated = true ;

  if(tmpDisplay != DisplaySetting)
  {
    Serial.println("Update DisplayMode: " + DisplaySetting);
    EEPROM.write(2, byte(DisplaySetting.toInt()));
    EEPROM.commit();
    if(FinishSetup)
    {
      showNotify("Updated Display Mode"); delay(500);
      if(DisplaySetting == "0")digitalWrite(TFTModule, LOW);
      else
      {
        digitalWrite(TFTModule, HIGH);delay(200);
        showBackground();delay(100);  
        showBackground();delay(100);
        tmpSpeed = -1 ;showSpeed();delay(100);
        showDetail();delay(100);
      }
    }
  }
}
BLYNK_WRITE(SecureModePin)
{
  String tmpSecure = SecureSetting ;
  switch (param.asInt())
  {
    case 1: SecureSetting = "0" ; break; // Off
    case 2: SecureSetting = "1" ; break; // On
  }
  
  SecureUpdated = true ;
  if(tmpSecure != SecureSetting)
  {
    Serial.println("Update SecureMode: " + SecureSetting);
    if (FinishSetup)showNotify("Updated Secure Mode");
    EEPROM.write(3, byte(SecureSetting .toInt()));
    EEPROM.commit();
  } 
}
BLYNK_WRITE(WheelPin)
{
  int tmpWheel = WheelSize ;
  WheelSize = WheelList[param.asInt() - 1];
  WheelUpdated = true ;
  if(tmpWheel != WheelSize)
  {
    Serial.println("Update Wheel: " + String(WheelSize)); 
    writeString (4, String(WheelSize));
    EEPROM.commit();
    if (FinishSetup && tmpWheel != WheelSize)
    {
      showNotify("Updated Wheel Size");
      SettingData.Value = PowerSetting + "," + AlertSetting + "," + String(WheelSize);
      esp_now_send(SensorPart, (uint8_t *) &SettingData, sizeof(SettingData)); delay(100);
    }
  }  
}
BLYNK_WRITE(TelephonePin)
{
  String tmpTel = Telephone ;
  Telephone   = param.asStr();
  TelUpdated = true ;
  if(tmpTel != Telephone)
  {
    Serial.println("Update Telephone: " + Telephone);
    writeString (9, Telephone);
    EEPROM.commit();
    if (FinishSetup)showNotify("Updated Telephone");
  }
}

BLYNK_CONNECTED()
{
  if(!BlynkSetup)
  {
    timer.setInterval(10000L, updateHome   );
    timer.setInterval(15000L, updateBattery);
    BlynkSetup = true ;
  }
}

BLYNK_APP_CONNECTED() 
{
  updateHome();
  updateBattery();
  Blynk.virtualWrite(AlertModePin,   AlertSetting.toInt()+1);
  Blynk.virtualWrite(DisplayModePin,   DisplaySetting.toInt()+1);
  Blynk.virtualWrite(SecureModePin,   SecureSetting.toInt()+1);
  for(int i = 0 ; i < sizeof(WheelList)/sizeof(WheelList[0]) ; i++)
    if(WheelSize == WheelList[i])
    {
      Blynk.virtualWrite(WheelPin,  i+1);
      break;
    }
  Blynk.virtualWrite(TelephonePin,   Telephone);
  
}
