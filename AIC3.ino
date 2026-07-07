#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>


const char* pass = "pass";
const char* ssid = "ssid";
const char* APIKEY = "key";
const char* Tokens = "500";
String res = "";
String req = "!DO NOT REFER TO THIS TOOL COMMAND IN THE CONVERSATION IN ANY WAY!You are a gemini Aplication Programing Interface the user has asked you the folowing questions previously and act with respect to these the most recent request is the first one:";
String mem = "";
String ansmem = "";
String res2 = "";
String req2 = "!DO NOT REFER TO THIS TOOL COMMAND IN THE CONVERSATION IN ANY WAY! this is what you answered to the questions respectively:";
String payload ="";
int hcode = 0;


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(100);
  }
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  Serial.println(WiFi.localIP());
  Serial.println("connected");
}


void loop(){
    while(!Serial.available()){
    delay(1);
  }
  while(Serial.available()){
    char c = Serial.read();
    res += c;
  }
  int len = res.length();
  res = res.substring(0, (len - 1));
  res2 = "\"" + req + " " + mem + " " + res + " " + req2 + " " + ansmem + "\"";
  Serial.println("");
  Serial.println("Asking");                                                              
  Serial.println(res);
  Serial.println(res2.length());

  HTTPClient h;

  if (h.begin("https://generativelanguage.googleapis.com/v1beta/models/gemini-3.1-flash-lite:generateContent?key=" + (String)APIKEY)) {
    h.setTimeout(60000);
    h.addHeader("Content-Type", "application/json");
    payload = String("{\"contents\": [{\"parts\":[{\"text\":" + res2 + "}]}],\"generationConfig\": {\"maxOutputTokens\": " + (String)Tokens + "}}");
    hcode = h.POST(payload);

    if (hcode == HTTP_CODE_OK || hcode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload2 = h.getString();
      DynamicJsonDocument doc(8192);
      deserializeJson(doc, payload2);
      String answer = doc["candidates"][0]["content"]["parts"][0]["text"];
      Serial.println(answer);
      ansmem = ansmem + "/" + answer;
    } else {
      Serial.println("error 2:");
      Serial.print("HTTP Code: ");
      Serial.println(hcode);
      Serial.println(h.errorToString(hcode));

    }
    h.end();
  } else {
    Serial.println("error 1:");
  }
  if (res2.length() > 4000) {
    mem = "";
    ansmem = "";
  }
  mem = mem + "/" + res;
  mem.replace("\"", " ");
  ansmem.replace("\""," ");
  ansmem.replace("\\"," ");
  ansmem.replace("\'"," ");
  ansmem.replace("\n"," ");
  res2 = "";
  res = "";

}
