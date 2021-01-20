#include <Arduino.h>

// Import required libraries
#include "mywifi.h"
const char* local_ssid="your_LocalAP_SSID";
const char* mDNS_name="yours_DNS_name";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";
const char* PARAM_INPUT_SSID = "SSID";
const char* PARAM_INPUT_Password = "Password";


uint8_t wifi_ssid_count=0;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


String outputState(int output);
/**
  * @brief	WebServer Root Page	
  */
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Controller Page</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;
              text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
    .button2 {background-color: #555555;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Controller Page</h2>
  %BUTTONPLACEHOLDER%
  <h3><font color="red">Press Submit will reboot</font></h3>

<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/led?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/led?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

/**
  * @brief	AP mode WebServer Root Page	
  */
const char ap_mode_index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>WiFi Config</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;
              text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
    .button2 {background-color: #555555;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Wi-Fi Configuration</h2>
  %BUTTONPLACEHOLDER%

<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/led?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/led?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

/**
	* @brief	scan SSID and Print out
	*/
void myWIFI_scan_ssid(void)
{
  delay(100);
  wifi_ssid_count = WiFi.scanNetworks();
  Serial.println("scan done");
  if (wifi_ssid_count == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(wifi_ssid_count);
    Serial.println(" networks found");

    for (int i = 0; i < wifi_ssid_count; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i)));
      delay(10);
     }
  }
  Serial.println(""); 
}
/**
	* @brief	Connect to Wi-Fi and return IP
    * @retval	IPAddress 
	*/
bool  myWIFI_connect(System_infor_t *me)
{ 
  uint8_t SSID_len=me->SSID_len;
  uint8_t PW_len=me->Password_len;

  char SSID_buf[SSID_len+1];
  char PW_buf[PW_len+1];

  me->SSID.toCharArray(SSID_buf,SSID_len+1);
  me->Password.toCharArray(PW_buf,PW_len+1);

  
  WiFi.disconnect();//reset wifi
  WiFi.begin(SSID_buf, PW_buf);
  while (WiFi.status() != WL_CONNECTED) 
  {
    static uint8_t try_time=0;
    delay(1000);
    Serial.println("Connecting to WiFi..");
    try_time++;
    if(try_time%3==0)
    {
      WiFi.disconnect();//reset wifi
      WiFi.begin(SSID_buf, PW_buf);
    }
    if(try_time==15)
    {
      return false;
    }
  }
  // Print ESP Local IP Address
  IPAddress ip=WiFi.localIP();
  Serial.println(ip);
  return true;
}
/**
    @brief	Replaces placeholder with  web page
*/
String root_page_placeholder(const String& var)
{
  if(var == "BUTTONPLACEHOLDER")
  {
    String hander = "";    

    hander += "<h4>Output - LED B</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    hander += "<h4>Output - LED W</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    hander += "<h4>Output - LED G</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
    hander += "<h2>Chaging WIFI</h2>";
    hander +="<form action=\"/wificonf\">";
    hander +="<h4>SSID: <select name=\"SSID\" ></h4>";
    for(int i=0;i<wifi_ssid_count;i++)
    {
      hander +="<option >"+WiFi.SSID(i)+"</option>";
    }
    hander +="</select>";
    hander +="<h4>Password: <input type=\"text\"   name=\"Password\"></h4>";
    hander +="<input type=\"submit\" value=\"Submit\">";
    hander +="</form>";
    return hander;
  }
  return String();
}
/**
    @brief	Replaces placeholder with  web page in ap mode
*/
String ap_root_page_placeholder(const String& var)
{
  if(var == "BUTTONPLACEHOLDER")
  {
    String hander = "";   
    hander +="<form action=\"/wificonf\">";
    hander +="<h4>SSID: <select name=\"SSID\" ></h4>";
    for(int i=0;i<wifi_ssid_count;i++)
    {
      hander +="<option >"+WiFi.SSID(i)+"</option>";
    }
    hander +="</select>";
    hander +="<h4>Password: <input type=\"text\"   name=\"Password\"></h4>";
    hander +="<input type=\"submit\" value=\"Submit\">";
    hander +="</form>";
    hander +="<h3><font color=\"red\">Press Submit will reboot</font></h3>";
    hander +="<h2>Controller Page</h2>";
    hander += "<h4>Output - LED B</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    hander += "<h4>Output - LED W</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    hander += "<h4>Output - LED G</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";

    return hander;
  }
  return String();
}
String outputState(int output)
{
  if(digitalRead(output))
  {
    return "checked";
  }
  else 
  {
    return "";
  }
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
/**
  * @brief	/led  handler
  */
void __attribute__((weak)) Web_led_handler(String inputMessage1,String inputMessage2) 
{;}
/**
  * @brief	/wificonf  handler
  */
void __attribute__((weak)) Web_wificonf_handler(String mySSID,String myPassword) 
{;}
/**
    @brief	Create asynchronous web server 
            and Setting mDNS : autoff.local
	@note   Page :  / (root)
            Page :  /led    -> control LED form page
            Page :  /get    ->
*/
void myWIFI_Webserver(void)
{
// Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", index_html, root_page_placeholder);
  });

  // Send a GET request to <ESP_IP>/led?output=<inputMessage1>&state=<inputMessage2>
  server.on("/led", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    String inputMessage1;
    String inputMessage2;

    // GET input1 value on <ESP_IP>/led?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) 
    {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
    }
    else 
    {
      inputMessage1 = "None";
      inputMessage2 = "None";
    }
    Web_led_handler(inputMessage1,inputMessage2);
    request->send(200, "text/plain", "OK");
  }
  );

  // Send a GET request to <ESP_IP>/wificonf?input1=<inputMessage>
  server.on("/wificonf", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String input_SSID;
    String input_Password;
    if (request->hasParam(PARAM_INPUT_SSID) && request->hasParam(PARAM_INPUT_Password)) 
    {
      input_SSID = request->getParam(PARAM_INPUT_SSID)->value();
      input_Password = request->getParam(PARAM_INPUT_Password)->value();
    }
    else 
    {
      input_SSID = "none";
      input_Password = "none";
    }
    Web_wificonf_handler(input_SSID,input_Password);
    request->send(200, "text/html", "<P>Wi-Fi Setting successful</P> <P>SSID:  "+ 
                 input_SSID+"</P><P>Password :"+input_Password+"</P><a href=\"/\">Return to Home Page</a>");
  });

  if(!MDNS.begin(mDNS_name))
  {
    Serial.println("Error Starting mDNS");
  } 
  //server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP

  server.onNotFound(notFound);
  // Start server
  server.begin();


}
/**
    @brief	Create asynchronous web server 
            and Setting mDNS : autoff.local
	@note   Page :  / (root)
            Page :  /led    -> control LED form page
            Page :  /get    ->
*/
void myWIFI_AP_Webserver(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  myWIFI_scan_ssid();
  WiFi.softAP(local_ssid);
  Serial.print("Setting AP (Access Point)…");
  WiFi.mode(WIFI_MODE_AP);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
// Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send_P(200, "text/html", ap_mode_index_html, ap_root_page_placeholder);
  });

  // Send a GET request to <ESP_IP>/led?output=<inputMessage1>&state=<inputMessage2>
  server.on("/led", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    String inputMessage1;
    String inputMessage2;

    // GET input1 value on <ESP_IP>/led?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) 
    {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
    }
    else 
    {
      inputMessage1 = "None";
      inputMessage2 = "None";
    }
    Web_led_handler(inputMessage1,inputMessage2);
    request->send(200, "text/plain", "OK");
  }
  );

  // Send a GET request to <ESP_IP>/wificonf?input1=<inputMessage>
  server.on("/wificonf", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String input_SSID;
    String input_Password;
    if (request->hasParam(PARAM_INPUT_SSID) && request->hasParam(PARAM_INPUT_Password)) 
    {
      input_SSID = request->getParam(PARAM_INPUT_SSID)->value();
      input_Password = request->getParam(PARAM_INPUT_Password)->value();
    }
    else 
    {
      input_SSID = "none";
      input_Password = "none";
    }
    Web_wificonf_handler(input_SSID,input_Password);
    request->send(200, "text/html", "<P>Wi-Fi Setting successful</P> <P>SSID:  "+ 
                 input_SSID+"</P><P>Password :"+input_Password+"</P><a href=\"/\">Return to Home Page</a>");
  });

  if(!MDNS.begin(mDNS_name))
  {
    Serial.println("Error Starting mDNS");
  } 
  //server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP

  server.onNotFound(notFound);
  // Start server
  server.begin();


}
