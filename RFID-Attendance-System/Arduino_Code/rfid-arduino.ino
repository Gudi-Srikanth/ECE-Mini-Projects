#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HTTPSRedirect.h>

#include<Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
//---------------------------------------------------------------------------------------------------------
// Enter Google Script Deployment ID:
const char *GScriptId = "AKfycbznimerne94m86_4H6BQqxbIb22R8ZbAXmYwWPGb9l-4Jqulm58n8U66SBOTozYz0_A";
//---------------------------------------------------------------------------------------------------------
// Enter network credentials:
const char* ssid     = "srisam";
const char* password = "shoba1234";
//---------------------------------------------------------------------------------------------------------
// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";
//---------------------------------------------------------------------------------------------------------
// Google Sheets setup (do not edit)
const char* host        = "script.google.com";
const int   httpsPort   = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;
//------------------------------------------------------------
// Declare variables that will be published to Google Sheets
String student_id;
//------------------------------------------------------------
int blocks[] = {4,5,6,8,9};
#define total_blocks  (sizeof(blocks) / sizeof(blocks[0]))
//------------------------------------------------------------
#define RST_PIN  0  //D3
#define SS_PIN   2  //D4
#define BUZZER   4  //D2
//------------------------------------------------------------
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;  
MFRC522::StatusCode status;
//------------------------------------------------------------
/* Be aware of Sector Trailer Blocks */
int blockNum = 2;  
/* Create another array to read data from Block */
/* Legthn of buffer should be 2 Bytes more than the size of Block (16 Bytes) */
byte bufferLen = 18;
byte readBlockData[18];
//------------------------------------------------------------
/****************************************************************************************************
 * setup Function
****************************************************************************************************/
void setup() 
{
  
  //----------------------------------------------------------
  Serial.begin(9600);        
  delay(10);
  Serial.println('\n');
  //-----------------------------------------------------------
   if (!mlx.begin()) 
   {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
   };
  //----------------------------------------------------------
  SPI.begin();
  //----------------------------------------------------------
  //initialize lcd screen
  lcd.init();
  // turn on the backlight
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0); //col=0 row=0
  lcd.print("Connecting to");
  lcd.setCursor(0,1); //col=0 row=0
  lcd.print("WiFi...");
  //----------------------------------------------------------
  // Connect to WiFi
  WiFi.begin(ssid, password);             
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  //----------------------------------------------------------
  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  //----------------------------------------------------------
  lcd.clear();
  lcd.setCursor(0,0); //col=0 row=0
  lcd.print("Connecting to");
  lcd.setCursor(0,1); //col=0 row=0
  lcd.print("Google ");
  delay(5000);
  //----------------------------------------------------------
  Serial.print("Connecting to ");
  Serial.println(host);
  //----------------------------------------------------------
  // Try to connect for a maximum of 5 times
  bool flag = false;
  for(int i=0; i<5; i++){ 
    int retval = client->connect(host, httpsPort);
    //*************************************************
    if (retval == 1){
      flag = true;
      String msg = "Connected. OK";
      Serial.println(msg);
      lcd.clear();
      lcd.setCursor(0,0); //col=0 row=0
      lcd.print(msg);
      delay(2000);
      break;
    }
    //*************************************************
    else
      Serial.println("Connection failed. Retrying...");
    //*************************************************
  }
  //----------------------------------------------------------
  if (!flag){
    //____________________________________________
    lcd.clear();
    lcd.setCursor(0,0); //col=0 row=0
    lcd.print("Connection fail");
    //____________________________________________
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    delay(5000);
    return;
    //____________________________________________
  }
  //----------------------------------------------------------
  delete client;    // delete HTTPSRedirect object
  client = nullptr; // delete HTTPSRedirect object
  //----------------------------------------------------------
}
/****************************************************************************************************
 * loop Function
****************************************************************************************************/
void loop() 
{
  x:;
  Serial.print("Ambient temperature = "); 
  Serial.print(mlx.readAmbientTempC());
  Serial.print("°C");      
  Serial.print("   ");
  Serial.print("Object temperature = "); 
  Serial.print(mlx.readObjectTempC()); 
  Serial.println("°C");
  float h=mlx.readObjectTempF()+10;
  Serial.print("Ambient temperature = ");
  Serial.print(mlx.readAmbientTempF());
  Serial.print("°F");      
  Serial.print("   ");
  Serial.print("Object temperature = "); 
  Serial.print(mlx.readObjectTempF()); 
  Serial.println("°F");
  Serial.println("-----------------------------------------------------------------");
  delay(1000);
  //----------------------------------------------------------------
  static bool flag = false;
  if (!flag){
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr){
    if (!client->connected())
      {client->connect(host, httpsPort);}
  }
  else{Serial.println("Error creating client object!");}
  //----------------------------------------------------------------
  lcd.clear();
  lcd.setCursor(0,0); //col=0 row=0
  lcd.print("Scan your Tag");
  if(h>99)
  {
  lcd.clear();
  lcd.print("Temp is high!!!");
  lcd.print(h);
  delay(1000);
  lcd.clear();
  lcd.print("cannot enter");
  goto x;
  }
  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
  if ( ! mfrc522.PICC_IsNewCardPresent()) {return;}
  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial()) {return;}
  /* Read data from the same block */
  Serial.println();
  Serial.println(F("Reading last data from RFID..."));  
  //----------------------------------------------------------------
  String values = "", data,data2,datadate;
  char datamsg[100]={'=','I','F','(','I','S','B','L','A','N','K','(','C','4',')',',','"','"',',','"','R','e','s',
        'p','e','c','t','e','d',' ','P','a','r','e','n','t',',',' ','Y','o','u','r',' ','c','h','i','l','d',' ','"','&','E',
        '4','8','"',' ','w','a','s',' ','P','R','E','S','E','N','T',' ','o','n',' ','"','&','A','4','&','"',' ','"',')'};
 char datasend[1000]={'=','H','Y','P','E','R','L','I','N','K','(','"','h','t','t','p','s',':','/','/','a','p','i','.','w','h','a','t','s','a','p','p','.','c','o','m','/',
's','e','n','d','?','p','h','o','n','e','=','"',' ','&',' ','E','N','C','O','D','E','U','R','L','(','9','1','&','F','6',':','F',')',' ','&',' ','"','&','t','e','x','t',
'=','"','&', ' ','e','n','c','o','d','e','u','r','l','(','J','6',':','J',')',',','"','S','E','N','D','"',')'};
 datadate="=TO_TEXT(A4)";
  for (byte i = 0; i < total_blocks; i++) 
  {
    ReadDataFromBlock(blocks[i], readBlockData);
    //*************************************************
    if(i == 0){
      data = String((char*)readBlockData);
      data.trim();
      student_id = data;
      values = "\"" + data + "^";
    }
    //*************************************************
    else if(i == total_blocks-1){
      data = String((char*)readBlockData);
      data2=String(h);
      data.trim();
      data2.trim();
      values += data + "^"+ data2 + "^"+datamsg+"^"+datasend+"^"+datadate+"\"}";
    }
    //*************************************************
    else{
      data = String((char*)readBlockData);
      data.trim();
      values += data + "^";
    }
  }
  
  
  //----------------------------------------------------------------
  // Create json object string to send to Google Sheets
  // values = "\"" + value0 + "," + value1 + "," + value2 + "\"}"
  payload = payload_base + values;
  //----------------------------------------------------------------
  lcd.clear();
  lcd.setCursor(0,0); //col=0 row=0
  lcd.print("Publishing Data");
  lcd.setCursor(0,1); //col=0 row=0
  lcd.print("Please Wait...");
  //----------------------------------------------------------------
  // Publish data to Google Sheets
  Serial.println("Publishing data...");
  Serial.println(payload);
  if(client->POST(url, host, payload)){ 
    // do stuff here if publish was successful
    lcd.clear();
    lcd.setCursor(0,0); //col=0 row=0
    lcd.print("Student ID: "+student_id);
    lcd.setCursor(0,1); //col=0 row=0
    lcd.print("Present");
  }
  //----------------------------------------------------------------
  else{
    // do stuff here if publish was not successful
    Serial.println("Error while connecting");
    lcd.clear();
    lcd.setCursor(0,0); //col=0 row=0
    lcd.print("Failed.");
    lcd.setCursor(0,1); //col=0 row=0
    lcd.print("Try Again");
  }
  //----------------------------------------------------------------
  // a delay of several seconds is required before publishing again    
  delay(5000);
}


/****************************************************************************************************
 * 
****************************************************************************************************/
/****************************************************************************************************
 * ReadDataFromBlock() function
 ****************************************************************************************************/
void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{ 
  //----------------------------------------------------------------------------
  /* Prepare the ksy for authentication */
  /* All keys are set to FFFFFFFFFFFFh at chip delivery from the factory */
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  //----------------------------------------------------------------------------
  /* Authenticating the desired data block for Read access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  //----------------------------------------------------------------------------s
  if (status != MFRC522::STATUS_OK){
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  //----------------------------------------------------------------------------
  else {
    Serial.println("Authentication success");
  }
  //----------------------------------------------------------------------------
  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //----------------------------------------------------------------------------
  else {
    readBlockData[16] = ' ';
    readBlockData[17] = ' ';
    Serial.println("Block was read successfully");  
  }
  //----------------------------------------------------------------------------
}