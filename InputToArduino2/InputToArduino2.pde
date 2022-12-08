import processing.serial.*;
import ddf.minim.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import http.requests.*;

Serial myPort;                       // The serial port
Serial tftPort;                      // Port to tft display
boolean tftSent = false;

boolean ledState = false;
boolean firstContact = false;        // Whether we've heard from the microcontroller       
int c = 255;


String APP_KEY = "Q9nGc4IoTE5gTSMZ1PDof32Y";
String SECRET_KEY = "slXcSQM6YpiDwbMUnOin3y8sTT4Lgypw";
String APP_KEY2 = "Rv9CQj0x4P9HgKDOjyOKgEIu";
String SECRET_KEY2 = "AoggtEWKAaY6BsL7X8UYb2hpBArpeHjn";

Minim minim;
AudioInput in;
AudioRecorder recorder;
Api demo;

int time = 0;
int wait = 2000;             //限制录音60s
boolean isOpened = false;    //button state
String recordUrl;
int English = 1737;
int Mandarin = 1537;
int lan = 0;

// emotion analysis result
String[] emotionType = {"happy", "like", "sad", "fearful", "angry", "disgusting", "neutral"}; 
float[] sourceCount = {0, 0, 0, 0, 0, 0, 0}; 
float emotionValue = 0;
 
void setup() {
  size(256, 256);  // Stage size
  println(Serial.list());
  String portName = Serial.list()[3]; 
  myPort = new Serial(this, portName, 9600);  
  String portName2 = Serial.list()[2];
  tftPort = new Serial(this, portName2, 9600);
  
  while(myPort == null){;}
  while(tftPort == null){;}

  minim = new Minim(this);
  in = minim.getLineIn(Minim.MONO, 1024, 16000);

}

void draw() {
  Timer();
  background(c, 255, 255);
      
  char input = myPort.readChar();         // read it and store it in val

  if(input == '1' && isOpened == false){   
    //button is pressed. Start recording.
    recordUrl = "record" + hour()+minute()+ second()+ month()+day();
    recorder = minim.createRecorder(in, recordUrl+".wav");
    println(recordUrl+".wav");
    while(recorder == null){;}
    recorder.beginRecord();
    isOpened = true;
    c = 0;
    lan = 1737; // 1737
  }else if(input == '2' && isOpened == false){                  //Stop recording. 
    //button is pressed. Start recording.
    recordUrl = "record" + hour()+minute()+ second()+ month()+day();
    recorder = minim.createRecorder(in, recordUrl+".wav");
    println(recordUrl+".wav");
    while(recorder == null){;}
    recorder.beginRecord();
    isOpened = true;
    c = 100;
    lan = 1537;
  }else if(input == '0' && isOpened == true){                  //Stop recording. 
    isOpened = false;
    time = 0;
    c = 255;
    recorder.endRecord();
    recorder.save();
    println(lan);
    println("done");
    delay(2000);
    runApi(lan);
  }else if(input == '3'  && tftSent == false){
    int maxIndex = findMax() + 1;
    tftPort.write(maxIndex);     
    tftSent = true;
    println(sourceCount);
    println(maxIndex);
      for(int i = 0; i <= 6; i++){     //这里是6，因为要包括最后一个neutral
        sourceCount[i] = 0;
    }
  }

  delay(1000);
  tftSent = false;
}

void Timer(){
  if(isOpened){
    time++;
    if(time >= wait){
      time=0;
      isOpened = false;
      println("Limit 60s.Done saving.");
    }
  }
}

String getToken(String aKEY,String sKEY){
  String urlToken = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id="+aKEY+"&client_secret="+sKEY+"&";
  PostRequest post = new PostRequest(urlToken);
  post.send();
  //System.out.println("Reponse Content: " + post.getContent());
  String result = post.getContent();
  JSONObject json = parseJSONObject(result);
  return json.getString("access_token");
}

void runApi(int Languague){
  demo = new Api(recordUrl);  
  //Voice recognition
  String token = getToken(APP_KEY, SECRET_KEY);
  String result = demo.getResult(token, Languague);
  JSONObject response = parseJSONObject(result);
  JSONArray values = (JSONArray) response.get("result");
  try{
    result = (String)values.get(0);
    System.out.println("You said：" + result);
    if(Languague == English){
      print("Translate English to Chinese : ");
      String tokenTransApi = getToken(APP_KEY2, SECRET_KEY2);
      String trans = demo.transApiPostMethod(tokenTransApi, result);
      JSONObject trans1 = parseJSONObject(trans);
      JSONObject trans2 = trans1.getJSONObject("result");
      JSONArray trans3 = trans2.getJSONArray("trans_result");
      JSONObject trans4 = trans3.getJSONObject(0);
      String transCn = trans4.getString("dst");
      result = transCn;
      println(result);
    }
    //Analyse emotions
    String emotion = demo.emotionAnalysePostMethod(token,result);
    JSONObject response1 = parseJSONObject(emotion);
    JSONArray emotionJson = response1.getJSONArray("items");
    JSONObject emotionJson1 = emotionJson.getJSONObject(0);
    //tendency
    String emotionLabel = emotionJson1.getString("label");
    //emotion name
    JSONArray subitems = emotionJson1.getJSONArray("subitems");
    String emotionName = " ";
    if(emotionLabel.equals("neutral") == false){
      JSONObject subitems1 = subitems.getJSONObject(0);
      emotionName = subitems1.getString("label");
      emotionValue = subitems1.getFloat("prob");
    
    println("Sentiment analysis : "+ emotionLabel + " >>>> " + emotionName +  " : prob = " + emotionValue);
  }else{
     println("Sentiment analysis : "+ emotionLabel);
  }
 
  //send result to Arduino
   delay(1000);
   
  // emotion analysis result
    sendToLed(emotionLabel, emotionName);
    ledState = false;
     
   }catch(Exception e){
     myPort.write(9); 
   }
}

void sendToLed(String label, String type){
  
  if (label.equals("pessimistic") && ledState == false ) { 
    myPort.write(0); 
    println("send");
    ledState = true;  
  } else if(label.equals("optimistic") && ledState == false ) {                   
    myPort.write(1);  
    println("send  ");
    ledState = true;
  }else if( label.equals("neutral") && ledState == false ) {                   
    myPort.write(2); 
    println("send");
    ledState = true;
    //也记录netural出现的次数值
    sourceCount[6] += 0.7;
  }
ledState = false;
  if(type != " "){
    for(int i = 0; i <= 5; i++){
      if (type.equals(emotionType[i]) && ledState == false ) { 
        int index = i + 3;
        myPort.write(index); 
        println("send");
        //caculate how many times the emotion has been recorded
        sourceCount[i] += emotionValue;
        ledState = true;
      }
    }
  }  
}

int findMax(){
  float num = 0;
  int index = 0;
  for(int i = 0; i <= 6; i++){     //这里是6，因为要包括最后一个neutral
      if (sourceCount[i] > num ) { 
        num = sourceCount[i];
        index = i;
      }
    }
  if(num <= 0) return 6; 
  return index;
}
