#include "LedControl.h"
#include "binary.h"

LedControl lc=LedControl(8,10,9,1); //DIN, CLK, CS

// happy face
byte hf[8]= {B00100100,B00100100,B00100100,B00000000,B10000001,B01111110,B00000000,B00000000};
// neutral face
byte nf[8]={B00100100, B00100100,B00100100,B00000000,B11111111,B10000001,B01000010,B00111100};
// sad face
byte sf[8]= {B00100100,B00100100,B00100100,B00000000,B00000000,B00111100,B01000010,B10000001};

// delay time between faces
unsigned long delaytime=1000;
int inByte = 2; 
int emotion = 2;

int ledState = 0; // 1 --- heart,  2 --- face

///lcd
int lcdState = 0; // 0 ---- begin, 1 --- nextOrEnd, 2 --- result, 3 --- wait
int breakTime = 0;
bool dataReceived = false;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
// emotion analysis result
String tendencyRe[3] = {"Pessimistic", "Optimistic" ,"Neutral"}; // 0, 1, 2
String emotionType[6] = {"Happy", "Like", "Sad", "Fearful", "Angry", "Disgusting"}; // 3, 4, 5, 6, 7, 8
String printEmotion = "";

//initialize the eomiton analysis result
String tendency = "";
String type = "";

//button
int enButton = 12;
int cnButton = 11;
bool lastStateEN = false;
bool lastStateCN = false;

//light sensor
int lightSensor = A0;
boolean lightState = false;

int seedCount = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  lc.shutdown(0,false);
  // Set brightness to a medium value
  lc.setIntensity(0,5);
  // Clear the display
  lc.clearDisplay(0); 

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Press button to");
  lcd.setCursor(0,1);
  lcd.print("inject emotion.");
  
  pinMode(enButton, INPUT);
  pinMode(cnButton, INPUT);

  pinMode(lightSensor, INPUT);

  Serial.begin(9600);
  while (!Serial) {;}
}

void loop(){
   state();
   delay(1); 
  
   if (Serial.available()) {
    while (Serial.available() > 0) {
      //Serial.println("read");
      inByte = Serial.read();
      if(inByte < 3){ 
        emotion = inByte;
        dataReceived = true;
      }
      //Serial.println(inByte);
      inputEmotion(inByte);
    }
  }
  
  //if(ledState == 0) lc.clearDisplay(0); 
  //else 
  if(ledState == 2) drawFaces(emotion); 

  lcdDiplay();

  //light sensor to end the game

  int light = analogRead(lightSensor);
  //Serial.println(light);
  if(light > 990){

    int buttonStateEN = digitalRead(enButton);
    int buttonStateCN = digitalRead(cnButton);

    if(buttonStateEN == 1 && buttonStateCN == 1 && lightState == false){
      Serial.write("3");                            //end the game
      lightState = true;
      //Serial.println(light);
      seedCount++;
      
    }
  }

  //restart
  if(lightState){
    lc.clearDisplay(0);
    //print lcd
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("#");
    lcd.print(seedCount);
    lcd.print(" Seed");
    lcd.setCursor(0,1);
    lcd.print("Successfully planted."); 
    delay(10000); //10s
    
    lightState = false;
    
    delay(1000);
    lcdState = 0;
    ledState = 0; 
  }
  
}

void inputEmotion(int b){
  switch(b) {
  case 0:
    tendency = tendencyRe[0];
    break;
  case 1:
    tendency = tendencyRe[1];
    break;
  case 2:
    tendency = tendencyRe[2];
    type = " ////";
    break;
  case 3:
    type = emotionType[0];
    break;
  case 4:
    type = emotionType[1];
    break;
  case 5:
    type = emotionType[2];
    break;
  case 6:
    type = emotionType[3];
    break;
  case 7:
    type = emotionType[4];
    break;
  case 8:
    type = emotionType[5];
    break;
  case 9:
    type = "wrong";
    break;
  }  
  
}

void state(){
  int buttonStateEN = digitalRead(enButton);
  //Serial.println(buttonStateEN);

  if(buttonStateEN == 0){      //Pressed 0
    if(lastStateEN == false){
      Serial.write("1");      //start recording
      lastStateEN = true;
      breakTime = 0;
      ledState = 1;
    }
    drawHeart();
  }else if(buttonStateEN == 1){    //Not pressed 1
    if(lastStateEN == true){
      Serial.write("0");      //end recording
      lastStateEN = false;
      //Serial.println("0");
      ledState = 2;           //stop led
      lcdState = 2;            //show result
      dataReceived = false;
    }
    if(lcdState == 2) breakTime ++;
  }
  
  int buttonStateCN = digitalRead(cnButton);
  //Serial.println(buttonStateCN);

  if(buttonStateCN == 0){      //Pressed 0
    if(lastStateCN == false){
      Serial.write("2");          //start recording
      lastStateCN = true;
      //Serial.println("2");
      breakTime = 0;
      ledState = 1;
    }
    drawHeart();
    
  }else if(buttonStateCN == 1){ //Not pressed 1
    if(lastStateCN == true){
      Serial.write("0");        //end recording
      lastStateCN = false;
      //Serial.println("0");
      ledState = 2;
      lcdState = 2;            //show result
      dataReceived = false;
    }
    if(lcdState == 2) breakTime ++;
  }
}


void drawFaces(int emotion){

  if(emotion == 0){
    // Display sad face
    lc.setRow(0,0,sf[0]);
    lc.setRow(0,1,sf[1]);
    lc.setRow(0,2,sf[2]);
    lc.setRow(0,3,sf[3]);
    lc.setRow(0,4,sf[4]);
    lc.setRow(0,5,sf[5]);
    lc.setRow(0,6,sf[6]);
    lc.setRow(0,7,sf[7]);
    delay(delaytime);
  }else if(emotion == 1){
  // Display happy face
    lc.setRow(0,0,nf[0]);
    lc.setRow(0,1,nf[1]);
    lc.setRow(0,2,nf[2]);
    lc.setRow(0,3,nf[3]);
    lc.setRow(0,4,nf[4]);
    lc.setRow(0,5,nf[5]);
    lc.setRow(0,6,nf[6]);
    lc.setRow(0,7,nf[7]);
    delay(delaytime);
  }else if(emotion == 2){
  // Display netural face
    lc.setRow(0,0,hf[0]);
    lc.setRow(0,1,hf[1]);
    lc.setRow(0,2,hf[2]);
    lc.setRow(0,3,hf[3]);
    lc.setRow(0,4,hf[4]);
    lc.setRow(0,5,hf[5]);
    lc.setRow(0,6,hf[6]);
    lc.setRow(0,7,hf[7]);
    delay(delaytime);
  }
}

void lcdDiplay(){
   if( breakTime > 100 ){
        breakTime = 0;
        lcdState = 1;
   }
   
   if(lcdState == 0){
     lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Press button to");
      lcd.setCursor(0,1);
      lcd.print("inject emotion.");
      delay(100); 
   }else if(lcdState == 1){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("inject again or");
        lcd.setCursor(0,1);
        lcd.print("plant the seed");
        delay(100); 
   }else if(lcdState == 2){
    if(dataReceived){
      
     lcd.clear();
      lcd.setCursor(0,0); // define where to print the text
      lcd.print(tendency);
      lcd.setCursor(0,1);
      if(type != "") {
        if(type == "wrong"){
          
          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.print("Failed. Please");
          lcd.setCursor(0,1);
          lcd.print("record again.");
        }else{
        lcd.print(">> " + type);
        }
      }
      delay(100); 
    }else{
      lcd.clear();
      lcd.setCursor(0,0); // define where to print the text
      lcd.print("Please Wait");
    }
   }
   
}
