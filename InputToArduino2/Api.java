import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.Scanner;

public class Api{
    private String FILENAME;
    // 文件格式, wav
    private final String FORMAT = "wav";
    private String CUID = "1c:57:dc:70:c8:0a";  //"E0-0A-F6-6E-6F-B3";
    // 采样率固定值
    private final int RATE = 16000;
    private String URL = "http://vop.baidu.com/server_api";
    //private String SCOPE = "audio_voice_assistant_get";
    
    public Api(String recordUrl){
      FILENAME = "/Users/yenny/Downloads/physical computing/PComp/processing/InputToArduino2/"+recordUrl+".wav";
      URL = "http://vop.baidu.com/server_api";
      //SCOPE = "audio_voice_assistant_get";
    }
    
    public String getResult(String token, int DEV_PID) {
        String result  = recordToTextPost(token, DEV_PID);      
        return result;
    }
    
    private String recordToTextPost(String token, int DEV_PID){         
      ConnUtil ConnUtil = new ConnUtil();
      String url2 = URL + "?cuid=" + ConnUtil.urlEncode(CUID) + "&dev_pid=" + DEV_PID + "&token=" + token;
      //System.out.println(ConnUtil.urlEncode(CUID));
      String contentTypeStr = "audio/" + FORMAT + "; rate=" + RATE;
      //System.out.println(url2);
      byte[] content = getFileContent(FILENAME);
      
      String result = "no value";
      try{
        HttpURLConnection conn = (HttpURLConnection) new URL(url2).openConnection();
        conn.setConnectTimeout(5000);
        conn.setRequestProperty("Content-Type", contentTypeStr);
        conn.setRequestMethod("POST");
        conn.setDoOutput(true);
        conn.getOutputStream().write(content);
        conn.getOutputStream().close();
        byte[] re = ConnUtil.getResponseBytes(conn);
        result =  new String(re, "UTF-8");     
        System.out.println(result); 
      }catch (Exception e) {
        //e.printStackTrace();
      } 
      return result;
    }
    
    public String emotionAnalysePostMethod(String token, String rawText){         
      ConnUtil ConnUtil = new ConnUtil();
      String url2 = "https://aip.baidubce.com/rpc/2.0/nlp/v1/emotion?access_token="+ token +"&charset=GBK";
      String data1 = "{\"scene\":\"talk\",\"text\":\"";
      String data2 = " \" }";
      String data = data1 + rawText + data2;
      String textUrl = "/Users/yenny/Downloads/physical computing/PComp/processing/InputToArduino2/postEmotionAnaBaidu.txt";
      try{
        File file = new File(textUrl);
        FileWriter fo = new FileWriter(file);
        fo.write(data);
        fo.close();
     }catch(Exception e){
     }
      byte[] content = getFileContent(textUrl);
      String result = "no value";
      try{
        HttpURLConnection conn = (HttpURLConnection) new URL(url2).openConnection();
        conn.setConnectTimeout(5000);
        conn.setRequestProperty("Content-Type", "application/json");
        conn.setRequestMethod("POST");
        conn.setDoOutput(true);
        conn.getOutputStream().write(content);
        conn.getOutputStream().close();  
        byte[] re = ConnUtil.getResponseBytes(conn);
        result =  new String(re, "GBK");
        System.out.println(result);     
      }catch (Exception e) {
        //e.printStackTrace();
      } 
      return result;
    }
    
    public String transApiPostMethod(String token, String rawText){         
      ConnUtil ConnUtil = new ConnUtil();
      String url2 = "https://aip.baidubce.com/rpc/2.0/mt/texttrans/v1?access_token="+ token;
      String data1 = "{\"from\":\"en\",\"to\":\"zh\",\"q\":\"";
      String data2 = "\"}";
      String data = data1 + rawText + data2;
      String textUrl = "/Users/yenny/Downloads/physical computing/PComp/processing/InputToArduino2/textToTrans.txt";
      try{
        File file = new File(textUrl);
        FileWriter fo = new FileWriter(file);
        fo.write(data);
        fo.close();
     }catch(Exception e){
     }
      byte[] content = getFileContent(textUrl);
      String result = "no value";
      try{
        HttpURLConnection conn = (HttpURLConnection) new URL(url2).openConnection();
        conn.setConnectTimeout(5000);
        conn.setRequestProperty("Content-Type", "application/json");
        conn.setRequestMethod("POST");
        conn.setDoOutput(true);
        conn.getOutputStream().write(content);
        conn.getOutputStream().close();  
        byte[] re = ConnUtil.getResponseBytes(conn);
        result =  new String(re, "UTF-8");
        //System.out.println(result);     
      }catch (Exception e) {
        //e.printStackTrace();
      } 
      return result;
    }


 byte[] getFileContent(String filename) {
      File file = new File(filename);
      byte[] re = {};
      if (!file.canRead()) {
          System.err.println("文件不存在或者不可读: " + file.getAbsolutePath());
          //throw new DemoException("file cannot read: " + file.getAbsolutePath());
      }
      FileInputStream is = null;
      try {
        try{
          is = new FileInputStream(file);
          re = ConnUtil.getInputStreamContent(is);
        }catch (Exception e) {
      } 
         
      } finally {
          if (is != null) {
              try {
                  is.close();
          
              } catch (IOException e) {
                  e.printStackTrace();
              }
          }
      }
      return re; 
    }
}
