# Virtual Flower Planting Experiment

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/0.png)

### Video ： https://www.youtube.com/watch?v=DIsOEUccMT4

Pcomp final project - Yan Wang 22019755

## week 1 


### Inspiration and idea

Furniture giant Ikea created a live experiment for school children in Dubai.It involved two identical plants being put in identical conditions for 30 days.The only difference was that one plant was spoken to negatively, while the other was showered with regular compliments, At the end of the period, there was a clear difference in how the plants looked.

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/1.png)

https://www.dailymail.co.uk/femail/article-5703247/Bizarre-experiment-shows-talking-plants-thrive.html
https://www.dailymail.co.uk/femail/article-5703247/Bizarre-experiment-shows-talking-plants-thrive.html#v-6317337125629376301

Although the results of this experiment are not necessarily scientific, there is no denying that it is interesting. It inspired me, why not make an interactive work using the emotions of language as input? It allows every audience member to participate in the 'experiment', where each person's emotions would become the fertiliser for a virtual flower to grow, and ultimately the input of different emotions would lead to a variety of fantastic results.

### Sketch & User Flow

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/2.png)

The device has an LCD screen that shows the current state of the flower, the participants have to speak to the virtual flower and choose different language options (English/Mandarin)(through buttons). The computer will call different APIs to convert the recording into text, then the APIs will be called to analyse the emotional tendencies of the text. At the same time, a 8x8 LED matrix will display emoji to indicate the result of the analysis, and this emotional energy will become fertiliser for the flower to grow. After several inputs, the last participant needs to inject the flower with sunlight (using the torch of their mobile phone to illuminate the brightness sensor) and finally a wonderful virtual flower is created.

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/3.png)

During multiple inputs, different emotions will affect the generation of the flower attributes, which may be its size, shape, variety, flavour, use, or even character. (A list of all the attributes and the algorithm for determining them will be needed later)

### Material List

Input
- Button x 2
- Brilliance Sensor
- Recording Equipment (computer)

Output
- TFT IPS Screen (shows pictures of flowers)
- LCD Screen (shows texts of results)
- 8x8 LED Matrix (shows emoji)


## week 2

### Game flow modifications

Changed : There is no limit to the number of times the player can record, at any time you want to end the game you can shine the brightness sensor with your phone and the IPS screen will show the flower according to the maximum value of recorded emotions.

### Button & Recording Test

When the button is pressed, a start command is sent to Processing to begin recording, and a stop command is sent when the button is released, then recording is complete.

- Problems：The digitalRead(input) continues to pass the number 0 when the button is pressed and 1 when it is not pressed, but I want to pass the message ‘start recording’ once only when the button is first pressed and pause recording the moment the button is released. How do I set this up?

- Solution：A boolean value is added to record the on/off state of the previous frame, so that the key is only just turned on if the key was off in the previous frame and Digitalread(input) now passes in the number 0. By the same token it is possible to know when the key has been released.

```
//setup
bool astStateCN = false;

//loop
 int buttonStateCN = digitalRead(cnButton);

  if(buttonStateCN == 0){    			  //being pressed
    if(lastStateCN == false){
      Serial.write("2");              //start recording
      lastStateCN = true;
    }
    drawHeart();
  }else if(buttonStateCN == 1){ 		    //not pressed
    if(lastStateCN == true){
      Serial.write("0");        		    //finish recording
      lastStateCN = false;
    }
  }
```

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/4.png)

### API Testing 

I used HTTP request library of Processing to make POST requests.  There were many difficulties in this process, , such as incorrect conversion of text encoding formats and inaccurate recognition of the recording format. However, as this part is not part of physical computing, I will only briefly document the process in this section：

1. Record ( processing minim Library )
2. Audio to text（Baidu Voice Recognition Api）
   - 1537 for Chinese, 1737 for English, translates to Chinese when English is entered（Baidu Translation api）
3. Analysis of textual emotions（Baidu Conversational emotion recognition Api）

Sources :

- Baidu Voice Recognition Api : https://ai.baidu.com/tech/speech/asr
- Baidu Translation Api : https://ai.baidu.com/tech/mt/text_trans
- Baidu Emotion Detection Api : https://ai.baidu.com/tech/nlp_apply/emotion_detection

## week4

### LED 8x8 Matrix

Sketch
- neutral / optimistic /pessimistic

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/5.png)

Control led using the LedControl library

- Emoji display : each digit represents an led , 0 not lit, 1 lit, each digit represents an led.

```
// happy face
byte hf[8]= {B00100100,B00100100,B00100100,B00000000,B10000001,B01111110,B00000000,B00000000};

// neutral face
byte nf[8]={B00100100, B00100100,B00100100,B00000000,B11111111,B10000001,B01000010,B00111100};

// sad face
byte sf[8]= {B00100100,B00100100,B00100100,B00000000,B00000000,B01111110,B10000001,B00000000}; 
```

Displaying emoji according to the analysis results.

1. Extract the text of results of the api and determine which sentiment is input. (Processing)
 - Problem: cannot be determined correctly.
 - Reason：The String a extracted from the Json cannot be used directly to determine whether it  is equal to the String b declared in processing using '=='.  

```                   
String a = emotionJson1.getString(“label”); //neutral
String b = “neutral”；
if(a == b) println (“ture”);                //can’t work
```

  - Solutions ：

```
if(a.equals.(b)) println (“ture”);          //print true
```

2. Transferring the results back to the arduino. 

```
// setup()
String portName = Serial.list()[3]; 
myPort = new Serial(this, portName, 9600);  

// draw()
if (label.equals("pessimistic") && ledState == false ) { 
    myPort.write(0); 
    println("send");
    ledState = true;  
  }
```

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/6.jpeg)
![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/7.png)

### Designing pictures of the virtual flowers

Set 7 types of flowers representing different emotions.

```
//The type of sentiment extracted from Json data of the api result .
String[] emotionType = {"happy", "like", "sad", "fearful", "angry", "disgusting", "neutral"}; 
```

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/8.1.png)

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/9.png)

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/10.png)


## week4

### LCD screen displaying text

Display of input results and usage tips.

```
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int lcdState = 0; // 0 ---- begin, 1 --- nextOrEnd, 2 --- result, 3 --- wait
int breakTime = 0;
bool dataReceived = false;

// emotion analysis result
String tendencyRe[3] = {"Pessimistic", "Optimistic" ,"Neutral"}; // 0, 1, 2
String emotionType[6] = {"Happy", "Like", "Sad", "Fearful", "Angry", "Disgusting"}; // 3, 4, 5, 6, 7, 8
String printEmotion = "";

//setup()
lcd.init();            
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("Press button to");
lcd.setCursor(0,1);
lcd.print("inject emotion.");

void lcdDiplay(){
   if( breakTime > 100 ){
        breakTime = 0;
        lcdState = 1;
   }
   if(lcdState == 0){                   //start
     lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Press button to");
      lcd.setCursor(0,1);
      lcd.print("inject emotion.");
      delay(100); 
   }else if(lcdState == 1){            //input again tips
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("inject again or");
        lcd.setCursor(0,1);
        lcd.print("plant the seed");
        delay(100); 
   }else if(lcdState == 2){            //results
    if(dataReceived){ 
     lcd.clear();
      lcd.setCursor(0,0);    // define where to print the text
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
    }else{                    //waiting for processing messages
      lcd.clear();
      lcd.setCursor(0,0);     // define where to print the text
      lcd.print("Please Wait");
    }
   }
}
```

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/12.png)


### TFT screen displaying the result image

- Image Display
  - Need sd card to store images, format it to fax32 using win (need to buy sd card, will test again next week)
  - I can get a moving image by playing the image in bmp format, the size of the screen. It is not possible to play a video directly.

- Text Display

```
  tft.setRotation(2);
  tft.setCursor(0, 150);                         // Text coordinates
  tft.setTextColor(ST77XX_GREEN);                 // Color
  tft.setTextSize(4);                               // Size
  tft.println("Virtual");          
  tft.println("Flower");
  tft.println("Planting");
  tft.println("Experiment");
```

The tft screen display requires the use of the Adafruit library, which takes up a lot of memory space and requires a separate piece of arduino to control. Therefore two Arduinos need to communicate with each other.

- Problems: 
   1. When two arduino boards are of different versions (uno and leonardo), it is not possible to successfully communicate via tx and rx ports.
   2. processing needs to have the serial port open in order to continuously receive signals from the buttons, while the arduino can only have one serial port open and cannot receive signals from both the computer and the other arduino.

- Solutions: Using processing to send messages to two arduinos separately, the two arduinos do not communicate with each other.

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/13.png)

### Button: select language and record

```
void state(){
  int buttonStateEN = digitalRead(enButton);   //English
  
  if(buttonStateEN == 0){
    if(lastStateEN == false){
      Serial.write("1");      //start recording
      lastStateEN = true;
      breakTime = 0;         //clear all texts 
      ledState = 1;
    }
    drawHeart();
  }else if(buttonStateEN == 1){
    if(lastStateEN == true){
      Serial.write("0");      //end recording
      lastStateEN = false;
      ledState = 2;            //stop led
      lcdState = 2;            //show text result 
      dataReceived = false;
    }
    if(lcdState == 2) breakTime ++; 
  }
  
  int buttonStateCN = digitalRead(cnButton);   // Chinese

  if(buttonStateCN == 0){      //being pressed ： 0
    if(lastStateCN == false){
      Serial.write("2");        //start recording
      lastStateCN = true;
      breakTime = 0;           //clear all texts 
      ledState = 1;            //show text result
    }
    drawHeart();
    
  }else if(buttonStateCN == 1){ //not pressed ： 1
    if(lastStateCN == true){
      Serial.write("0");        //end recording
      lastStateCN = false;
      ledState = 2;            //show
      lcdState = 2;            //show text result
      dataReceived = false;
    }
    if(lcdState == 2) breakTime ++;
  }
}
```

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/14.png)

### Illuminate the brightness sensor to end the game

You can only end the game when buttons are not being preesed.

```
  int light = analogRead(lightSensor);
  if(light > 1000){
    int buttonStateEN = digitalRead(enButton);
    int buttonStateCN = digitalRead(cnButton);
    
    if(buttonStateEN == 1 && buttonStateCN == 1 && lightState == false){
      Serial.write("3");                     //send message to Processing to end the game
      lightState = true;
      seedCount++;
    }
```

### Connect all arduino parts

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/15.png)

### Making the shell

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/16.png)

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/17.png)


### Writing game logic

Each time a sentiment value is entered, it is stored and added in an array.

```
//Processing
String[] emotionType = {"happy", "like", "sad", "fearful", "angry", "disgusting", "neutral"}; 
float[] sourceCount = {0, 0, 0, 0, 0, 0, 0}; 
```

And the result of each game is the strongest emotion - the maximum value in the array. 
Find the maximum value and return the index of this value.

```
int findMax(){
  float num = 0;
  int index = 0;
  for(int i = 0; i <= 6; i++){     
      if (sourceCount[i] > num ) { 
        num = sourceCount[i];
        index = i;
      }
    }
  if(num <= 0) return 6; 
  return index;
```

## Conclusion 

All in all, I used two arduino uno boards, two buttons used as input switches for recording in Chinese and English, and called the api to analyse the tendency of people's feelings when they speak, which affects the final game outcome. An 8x8 led Matrix displays the positive and negative tendencies of the feelings, and an LCD screen displays the specific type of feelings being recorded in text. Ultimately, after the player has made several inputs, the brightness sensor can be lit using a torch and a graph of the game's outcome (i.e. a virtual flower) is displayed on the IPS screen.

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/18.png)

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/19.png)

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/20.png)

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/21.png)

![](https://github.com/tomoko-tiba/-PComp-Final/blob/pics/22.png)

### Future work

After that I would like to be able to automatically generate patterns of flowers as a result of the game, rather than playing pictures, which would allow for a more varied and interesting outcome.

