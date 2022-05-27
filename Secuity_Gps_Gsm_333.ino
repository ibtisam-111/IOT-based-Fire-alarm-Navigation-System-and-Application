#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Stdio.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define ONE_WIRE_BUS 10//temp sensor

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

DeviceAddress insideThermometer;

const int numRows = 4;
const int numCols = 20;

char      inChar; //for gsp
char      a_1,a_2,a_3,a_4,a_5,a_6,a_7,a_8,a_9,a_10,a_11,a_12,a_13,a_14;
char      b_1,b_2,b_3,b_4,b_5,b_6,b_7,b_8,b_9,b_10,b_11,b_12;

char      inputString[20];         
boolean   stringComplete = false;  

int       led    =  13;

int       str_ctr   = 0;

int       buzzer = 7;

int       counter_1 = 0;

int       loop_a = 0;

int       ir_value = 0;

int       reason_value = 0;

const int analogInPin_1 = A0;//smoke

const int analogInPin_2 = A1;//gas

const int sensor_ir_1   = 22;//flame
const int sensor_ir_2   = 23;
const int sensor_ir_3   = 24;

const int sw_msg_1 = A15;//TEST MSG

long      smoke_1 = 0; 
long      smoke_2 = 0; 

long      gas_1 = 0; 
long      gas_2 = 0;

float     tempC=0;

void      gas_sensor();
void      smoke_sensor();

float     latitude,longitude;

char      lat[10];         
char      longi[10];         

boolean   flag_msg = 1;

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void setup() {

    Serial.begin(9600);//PC
    Serial1.begin(9600);//Gsm,gps
  
    lcd.begin(numCols, numRows);
  
    pinMode(led, OUTPUT);
    pinMode(buzzer, OUTPUT);

    pinMode(sw_msg_1, INPUT);
    
    pinMode(sensor_ir_1, INPUT);
    pinMode(sensor_ir_2, INPUT);
    pinMode(sensor_ir_3, INPUT);

    digitalWrite(buzzer, LOW);//on

    //===============================================
  
    lcd.setCursor(0, 0);  
    lcd.print("    ' IOT Based '   "); 
    lcd.setCursor(0, 1);  
    lcd.print("    ' Fire Alarm '  ");  
    lcd.setCursor(0, 2);  
    lcd.print("'Navigation System '");  
    lcd.setCursor(0, 3);  
    lcd.print(" ' Using Gsm Gps'   ");  

    //===============================================
  
    Serial.print("Locating devices...");
    sensors.begin();
    Serial.print("Found ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" devices.");
  
    // report parasite power requirements
    Serial.print("Parasite power is: "); 
    if (sensors.isParasitePowerMode()) Serial.println("ON");
    else Serial.println("OFF");
    
    if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
    
    Serial.print("Device 0 Address: ");
    printAddress(insideThermometer);
    Serial.println();
  
    // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
    
    //sensors.setResolution(insideThermometer, 9);
    sensors.setResolution(insideThermometer, 16);
   
    Serial.print("Device 0 Resolution: ");
    Serial.print(sensors.getResolution(insideThermometer), DEC); 
    Serial.println();
   
    //===============================================
  
    //Initial_Send_Msg();
  
    //===============================================
  
    stringComplete = true;

    digitalWrite(buzzer, HIGH);//OFF 
    
    lcd.setCursor(0, 0);  
    lcd.print("                    "); 
    lcd.setCursor(0, 1);  
    lcd.print("                    ");  
    lcd.setCursor(0, 2);  
    lcd.print("                    ");  
    lcd.setCursor(0, 3);  
    lcd.print("                    ");

    stringComplete = 1;

    flag_msg = 1;

    reason_value = 0;

    ir_value = 0;

}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void loop() {
   
     smoke_sensor();
     
     gas_sensor();
     
   //-------------------
   if(digitalRead(sw_msg_1) == LOW){

    b_1 = '3';
    b_2 = '1';
    b_3 = '3';
    b_4 = '3';
    b_5 = '.';
    b_6 = '8';
    b_7 = '0';
    b_8 = '6';
    b_9 = '2';
    b_10 = '5';
  
    lcd.setCursor(0, 0);  
    
    lcd.print("LT=");    
    lcd.print(b_1);
    lcd.print(b_2);
    lcd.print(b_3);
    lcd.print(b_4);
    lcd.print(b_5);
    lcd.print(b_6);
    lcd.print(b_7);
    lcd.print(b_8);
    lcd.print(b_9);
    lcd.print(b_10);
    lcd.print('+');//(b_11);//+
    lcd.print('N');//(b_12);//N
    lcd.print(' ');
    lcd.print(' ');
    lcd.print(' ');
    lcd.print(' ');
    lcd.print(' ');

    
    lat[0] = b_3;
    lat[1] = b_4;
    lat[2] = b_5;//'.'
    lat[3] = b_6;
    lat[4] = b_7;
    lat[5] = b_8;
    lat[6] = b_9;
    lat[7] = b_10;
    
    lat[8] = '\0';
              
    latitude = atof(lat);
    latitude = latitude/60;

    latitude = latitude+(((b_1-0x30)*10)+((b_2-0x30)*1));
    Serial.println(latitude,5);      
    
    //--------------------------------------
    
    a_1 = '0';
    a_2 = '7';
    a_3 = '4';
    a_4 = '1';
    a_5 = '9';
    a_6 = '.';
    a_7 = '0';
    a_8 = '8';
    a_9 = '6';
    a_10 = '7';
    a_11 = '3';
    
    lcd.setCursor(0, 1);  
      
    lcd.print("LN=");    
    lcd.print(a_1);
    lcd.print(a_2);
    lcd.print(a_3);
    lcd.print(a_4);
    lcd.print(a_5);
    lcd.print(a_6);
    lcd.print(a_7);
    lcd.print(a_8);
    lcd.print(a_9);
    lcd.print(a_10);
    lcd.print(a_11);
    lcd.print('+');//(a_12);//+
    lcd.print('E');//(a_13);//E
    lcd.print(' ');
    lcd.print(' ');
    lcd.print(' ');

    longi[0] = a_4;
    longi[1] = a_5;
    longi[2] = a_6;//'.'
    longi[3] = a_7;
    longi[4] = a_8;
    longi[5] = a_9;
    longi[6] = a_10;
    longi[7] = a_11;
    
    longi[8] = '\0';
              
    longitude = atof(longi);
    longitude = longitude/60;

    longitude = longitude+(((a_2-0x30)*10)+((a_3-0x30)*1));
    
    Serial.println(longitude,5);      
    
    //---------------------------------------

       //-----------------

       Make_Link();

   while(digitalRead(sw_msg_1) == LOW);
   
   }
   //-------------------
   if(digitalRead(sensor_ir_1) == LOW){

     lcd.setCursor(12, 3);  
     lcd.print("IR=");    
     lcd.print(" 1 ");
     
     Serial.println("IR Detect = 1");
     digitalWrite(buzzer,LOW);//ON

     reason_value = 1;

     ir_value = 1;
     
     if(flag_msg == 1){
      flag_msg = 0;
      
      Make_Link();

     } 
   }
   //-------------------
   else if(digitalRead(sensor_ir_2) == LOW){

     lcd.setCursor(12, 3);  
     lcd.print("IR=");    
     lcd.print(" 2 ");
     
     Serial.println("IR Detect = 2");
     digitalWrite(buzzer,LOW);//ON

     reason_value = 2;

     ir_value = 2;

     if(flag_msg == 1){
      flag_msg = 0;
      
      Make_Link();

     }
   }
   //-------------------
   else if(digitalRead(sensor_ir_3) == LOW){

     reason_value = 3;

     lcd.setCursor(12, 3);  
     lcd.print("IR=");    
     lcd.print(" 3 ");
     
     Serial.println("IR Detect = 3");
     digitalWrite(buzzer,LOW);//ON

     ir_value = 3;

     if(flag_msg == 1){
      flag_msg = 0;
      
      Make_Link();

     }
   }
   //-------------------
   else{

     Serial.println("IR Detect Not");
     digitalWrite(buzzer,HIGH);//OFF
    
     lcd.setCursor(12, 3);  
     lcd.print("IR=");    
     lcd.print("Not");

     ir_value = 0;
     flag_msg = 1;
     reason_value = 0;
   }
   //-------------------
   if(smoke_2 > 50){

     reason_value = 4;

     digitalWrite(buzzer,LOW);//ON
     
     if(flag_msg == 1){
      flag_msg = 0;
      
      Make_Link();

     }
   }
   else{

    flag_msg = 1;
    reason_value = 0;
   }
   //-------------------
   if(gas_2 > 50){

     reason_value = 5;

     digitalWrite(buzzer,LOW);//ON

     if(flag_msg == 1){
      flag_msg = 0;
      
      Make_Link();

     }
   }
   else{

    reason_value = 0;
    flag_msg = 1;
   }
   //-------------------
   if(tempC > 50){

     reason_value = 6;

     digitalWrite(buzzer,LOW);//ON

     if(flag_msg == 1){
      flag_msg = 0;
      
      Make_Link();

     }
   }
   else{

    reason_value = 0;
    flag_msg = 1;
   }
   
   //-------------------

   delay(500);
   
   //--------------------------------------------------------------------------------
   
   Serial.print("Requesting temperatures...");
   sensors.requestTemperatures(); // Send the command to get temperatures
   Serial.println("DONE");
  
   // It responds almost immediately. Let's print out the data
   printTemperature(insideThermometer); // Use a simple function to print out the data

   //--------------------------------------------------------------------------------

  if(stringComplete == 1) {
  
    lcd.setCursor(0, 0);  
    
    lcd.print("LT=");    
    lcd.print(b_1);
    lcd.print(b_2);
    lcd.print(b_3);
    lcd.print(b_4);
    lcd.print(b_5);
    lcd.print(b_6);
    lcd.print(b_7);
    lcd.print(b_8);
    lcd.print(b_9);
    lcd.print(b_10);
    lcd.print('+');//(b_11);//+
    lcd.print('N');//(b_12);//N
    lcd.print(' ');
    lcd.print(' ');
    lcd.print(' ');
    lcd.print(' ');
    lcd.print(' ');

    
    lat[0] = b_3;
    lat[1] = b_4;
    lat[2] = b_5;//'.'
    lat[3] = b_6;
    lat[4] = b_7;
    lat[5] = b_8;
    lat[6] = b_9;
    lat[7] = b_10;
    
    lat[8] = '\0';
              
    latitude = atof(lat);
    latitude = latitude/60;

    latitude = latitude+(((b_1-0x30)*10)+((b_2-0x30)*1));
    Serial.println(latitude,5);      
    
    //--------------------------------------
    
    lcd.setCursor(0, 1);  
      
    lcd.print("LN=");    
    lcd.print(a_1);
    lcd.print(a_2);
    lcd.print(a_3);
    lcd.print(a_4);
    lcd.print(a_5);
    lcd.print(a_6);
    lcd.print(a_7);
    lcd.print(a_8);
    lcd.print(a_9);
    lcd.print(a_10);
    lcd.print(a_11);
    lcd.print('+');//(a_12);//+
    lcd.print('E');//(a_13);//E
    lcd.print(' ');
    lcd.print(' ');
    lcd.print(' ');

    longi[0] = a_4;
    longi[1] = a_5;
    longi[2] = a_6;//'.'
    longi[3] = a_7;
    longi[4] = a_8;
    longi[5] = a_9;
    longi[6] = a_10;
    longi[7] = a_11;
    
    longi[8] = '\0';
              
    longitude = atof(longi);
    longitude = longitude/60;

    longitude = longitude+(((a_2-0x30)*10)+((a_3-0x30)*1));
    
    Serial.println(longitude,5);      
    
    //---------------------------------------
        
    stringComplete = false;
}

}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
/*
void Initial_Send_Msg(void){
  
  Serial1.println("AT");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial1.println("AT");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  
  Serial1.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  
  Serial1.println("AT+CMGS=\"03060670855\"\r"); // Replace x with mobile number
  delay(1000);
  Serial1.print("System Initialize");// The SMSext you want to send
  delay(1000);
  Serial1.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
 
}
*/
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void Make_Link(void){

    //------------------------------------------------------------
    
    Serial1.println("AT");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    Serial1.println("AT");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    
    
    Serial1.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    
    Serial1.println("AT+CMGS=\"03058329915\"\r"); // Replace x with mobile number
    delay(1000);

    //-----------------------

    Serial1.print(latitude,5);//1
    Serial1.print(',');

    //-----------------------
 
    Serial1.print(longitude,5);//1
    Serial1.print(','); 
    
    //-----------------------

    Serial1.print(tempC,1);//1
    Serial1.print(',');

    //-----------------------

    Serial1.print(smoke_2);//1
    Serial1.print(',');
    
    //-----------------------

    Serial1.print(gas_2);//1
    Serial1.print(',');
    
    //-----------------------
    
    if(reason_value == 0){

    Serial1.print("Normal");//1
    Serial1.print(',');  
    }
    else if(reason_value == 1){

    Serial1.print("Flame 1");//1
    Serial1.print(',');  
    }
    else if(reason_value == 2){

    Serial1.print("Flame 2");//2
    Serial1.print(',');  
    }
    else if(reason_value == 3){

    Serial1.print("Flame 3");//3
    Serial1.print(',');
    }
    else if(reason_value == 4){

    Serial1.print("Smoke");//4
    Serial1.print(',');
    }
    else if(reason_value == 5){

    Serial1.print("Gas");//5
    Serial1.print(',');
    }
    else if(reason_value == 6){

    Serial1.print("Temperature");//5
    Serial1.print(',');
    }
    //---------------------------------------------------------

    if(ir_value == 0){

    Serial1.print("Fire Normal");//1
      
    }
    else if(ir_value == 1){

    Serial1.print("Sensor 1");//1
      
    }
    else if(ir_value == 2){

    Serial1.print("Sensor 2");//2
      
    }
    else if(ir_value == 3){

    Serial1.print("Sensor 3");//3
    
    }
    //-----------------------
    
    Serial1.write('\0');
    delay(1000);
   
    Serial1.println((char)26);// ASCII code of CTRL+Z
    delay(1000);

}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void smoke_sensor(){

    smoke_1  =  analogRead(analogInPin_1);
    delay(1);  
    
    smoke_2 = ((smoke_1*500)/1023)-21;

    smoke_2 = map(smoke_2, 1, 100, 0, 100); 
     
    lcd.setCursor(0, 3);  
    lcd.print("S=");    
    lcd.print(smoke_2);
    lcd.print(" ");  

    Serial.print("Smoke_2=");
    Serial.println(smoke_2);
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void gas_sensor(){

    gas_1  =  analogRead(analogInPin_2);
    delay(1);
    
    gas_2 = ((gas_1*500)/1023)-55;
    
    gas_2 = map(gas_2, 1, 400, 0, 100); 

    lcd.setCursor(6, 3);  
    lcd.print("G=");    
    lcd.print(gas_2);
    lcd.print(" ");  

    Serial.print("Gas_2=");
    Serial.println(gas_2);

}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void ir_sensor(){

    
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void serialEvent1() {

     inChar = Serial1.read();    

     Serial.print(inChar);
     
     if(inChar == 'A' || inChar == 'B'){
       str_ctr=0;
       inputString[str_ctr] = inChar;
       str_ctr++;
       
     }
     else{
       
       inputString[str_ctr] = inChar;
       str_ctr++;
            
     }
     
     ////////////////////////////////////
     
   if(str_ctr >= 14){//15
     
      if(inputString[0] == 'A'){
      
         a_1  = inputString[1];    
         a_2  = inputString[2];    
         a_3  = inputString[3];    
         a_4  = inputString[4];    
         a_5  = inputString[5];    
         a_6  = inputString[6];    
         a_7  = inputString[7];    
         a_8  = inputString[8];    
         a_9  = inputString[9];    
         a_10 = inputString[10];    
         a_11 = inputString[11];    
         a_12 = inputString[12];    
         a_13 = inputString[13];    
         a_14 = inputString[14];    
               
      }
    
      else if(inputString[0] == 'B'){
      
         b_1  = inputString[1];    
         b_2  = inputString[2];    
         b_3  = inputString[3];    
         b_4  = inputString[4];    
         b_5  = inputString[5];    
         b_6  = inputString[6];    
         b_7  = inputString[7];    
         b_8  = inputString[8];    
         b_9  = inputString[9];    
         b_10 = inputString[10];    
         b_11 = inputString[11];    
         b_12 = inputString[12];     
         
    }
      
       str_ctr = 0;
       stringComplete = true;
       
    }
    
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
    // method 2 - faster
    tempC = sensors.getTempC(deviceAddress);
    if(tempC == DEVICE_DISCONNECTED_C) 
    {
      Serial.println("Error: Could not read temperature data");
      return;
    }
    Serial.print("Temp C: ");
    Serial.print(tempC);
    Serial.print(" Temp F: ");
    Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit

    lcd.setCursor(0, 2);  
    lcd.print("Temperature="); 
 
    lcd.print(tempC,1);     
    lcd.write(223);    
    lcd.print("C  ");  

}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}



