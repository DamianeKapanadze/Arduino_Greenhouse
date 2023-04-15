#include <DFRobot_DHT11.h>

  //#include "dht.h"                 //air humidity/temperature sensor
  #include <Wire.h>                //LCD I2C communication
  #include <LiquidCrystal_I2C.h>   //LCD SCL-A5   SDA - A4
  DFRobot_DHT11 DHT;
  #define dht_apin 10 // Analog Pin sensor is connected to  D10

  const int AirValue = 694;   //you need to replace this value with Value_1
  const int WaterValue = 340;  //you need to replace this value with Value_2
  const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
  const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
  
  LiquidCrystal_I2C lcd(0x27,20,4);
    
  int Soil_Humidity, Air_Humidity, Air_Temperature;
  int lastWaterTime = 0;
  
  // Design my own new characters:
  byte Degree_Symbol[8]={
    B000110,
    B001001,
    B001001,
    B000110,
    B000000,
    B000000,
    B000000,
    B000000
  };
  byte Thermometer_Symbol_1[8]={
    B001110,
    B001010,
    B001010,
    B001010,
    B001010,
    B001010,
    B001010,
    B001010
  };
  byte Thermometer_Symbol_2[8] = {
    B001010,
    B001010,
    B001110,
    B001110,
    B011101,
    B011111,
    B011111,
    B001110
  };
  
  
  void setup() {
  
    lcd.init();                      // initialize the lcd 
    lcd.backlight();
    Serial.begin(9600);
    
    // Create the new characters:
    lcd.clear();
    lcd.createChar(0, Degree_Symbol);
    lcd.createChar(1, Thermometer_Symbol_1);
    lcd.createChar(2, Thermometer_Symbol_2);
    
    lcd.setCursor(1, 0);
    lcd.print("Air       H:");
    lcd.setCursor(1, 1);
    lcd.print("Soil H:");
  
    Draw_Temperature_Symbol();
    Draw_Celsius_Symbol(7, 0);
    
    pinMode(A2, OUTPUT); 
    pinMode(A3, OUTPUT); 
    
  }

  void loop() {
    DHT.read(dht_apin); 
    Air_Temperature = DHT.temperature;
    Air_Humidity = DHT.humidity;
    Soil_Humidity = analogRead(A0);
    
    //Serial.println(analogRead(A0)); // soil moisture sensor connected to A0 pin
    int soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
    //Serial.println(soilMoistureValue);
    int soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
    if(soilmoisturepercent >= 100){
      Soil_Humidity = 100;
    }
    else if(soilmoisturepercent <=0){
      Soil_Humidity = 0;
    }
    else if(soilmoisturepercent >0 && soilmoisturepercent < 100){
     Soil_Humidity = soilmoisturepercent;
    }

    Serial.println(soilmoisturepercent);      
    if(soilmoisturepercent < 60){ // && millis() - lastWaterTime > 60000){
      //lastWaterTime = millis();
      digitalWrite(A3, LOW);
      delay(1000);
      digitalWrite(A3, HIGH);
      }
    //digitalWrite(A3, HIGH);
   
    digitalWrite(12, HIGH);
    delayMicroseconds(1450); // Duration of the pusle in microseconds
    digitalWrite(12, LOW);
    delayMicroseconds(18550); // 20ms - duration of the pusle
    // Pulses duration: 600 - 0deg; 1450 - 90deg; 2300 - 180deg

    lcd.setCursor(8, 1);
    lcd.print("    ");
     
    //digitalWrite(A2, HIGH);
    //delay(500);
    //digitalWrite(A3, HIGH);
    // A pulse each 20ms
  
    digitalWrite(12, HIGH);
    delayMicroseconds(1450); // Duration of the pusle in microseconds
    digitalWrite(12, LOW);
    delayMicroseconds(18550); // 20ms - duration of the pusle
    // Pulses duration: 600 - 0deg; 1450 - 90deg; 2300 - 180deg

    lcd.setCursor(8, 1);
    lcd.print("    ");
    
    
    Draw_Temperature_Symbol();
    if(Air_Temperature < 100 && Air_Humidity < 100){
      Print_on_LSD(Air_Temperature, 0, 5);
      Print_on_LSD(Air_Humidity, 0, 13);
      Print_on_LSD(Soil_Humidity, 1, 8);
      if(Soil_Humidity == 100){
        lcd.setCursor(11, 1);
        lcd.print("%");
      }else{
        lcd.setCursor(10, 1);
        lcd.print("%");        
      }
      delay(500);
    }

  Serial.println(checkWaterHight());
    
  }//loop
  
  int checkWaterHight() {
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   return pulseIn(echoPin, HIGH) / 29 / 2;
   delay(100);
}
  
  
  void Draw_Temperature_Symbol(){
    // Drwas thermometer character:
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.write(byte(2));
  }
  
  void Draw_Celsius_Symbol(int x, int y){
    lcd.setCursor(x, y);
    lcd.write(byte(0));
    lcd.setCursor(x+1, y);
    lcd.write('C');
    lcd.setCursor(15, y);
    lcd.write('%');
  }
  
  void Print_on_LSD(int x, int row, int column){
    char myStg[10];
    lcd.setCursor(column, row);
    sprintf(myStg, "%d", x);
    for(int i=0; i<strlen(myStg); i++){
      lcd.print(myStg[i]);
    }  
  }