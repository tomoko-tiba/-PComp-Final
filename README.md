# Virtual Flower Planting Experiment

Pcomp final project - Yan Wang 22019755


## week 1 


### Inspiration and idea

Furniture giant Ikea created a live experiment for school children in Dubai.It involved two identical plants being put in identical conditions for 30 days.The only difference was that one plant was spoken to negatively, while the other was showered with regular compliments, At the end of the period, there was a clear difference in how the plants looked.

//pic

https://www.dailymail.co.uk/femail/article-5703247/Bizarre-experiment-shows-talking-plants-thrive.html
https://www.dailymail.co.uk/femail/article-5703247/Bizarre-experiment-shows-talking-plants-thrive.html#v-6317337125629376301

Although the results of this experiment are not necessarily scientific, there is no denying that it is interesting. It inspired me, why not make an interactive work using the emotions of language as input? It allows every audience member to participate in the 'experiment', where each person's emotions would become the fertiliser for a virtual flower to grow, and ultimately the input of different emotions would lead to a variety of fantastic results.

### Sketch & User Flow

//pic2

The device has an LCD screen that shows the current state of the flower, the participants have to speak to the virtual flower and choose different language options (English/Mandarin)(through buttons). The computer will call different APIs to convert the recording into text, then the APIs will be called to analyse the emotional tendencies of the text. At the same time, a 8x8 LED matrix will display emoji to indicate the result of the analysis, and this emotional energy will become fertiliser for the flower to grow. After several inputs, the last participant needs to inject the flower with sunlight (using the torch of their mobile phone to illuminate the brightness sensor) and finally a wonderful virtual flower is created.

//pic3

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
      Serial.write("2"); 				    //start recording
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

//pic4

### API Testing 

I used HTTP request library of Processing to make POST requests.  There were many difficulties in this process, , such as incorrect conversion of text encoding formats and inaccurate recognition of the recording format. However, as this part is not part of physical computing, I will briefly document the process in this section：

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




