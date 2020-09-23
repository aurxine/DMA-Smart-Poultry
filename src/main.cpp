#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "nodeMCUpins.h"
#include "setupDevice.h"
#include "setupDevice.cpp"
#include "Farm.h"
#include "Farm.cpp"
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <MQTT.h>
#include <EspMQTTClient.h>
#include <ESP8266WebServer.h>
#include<RTClib.h>

bool flag = false;
bool serial_debug = true;

DHT_Unified dht(D5, DHT22); // dht(pin attached, sensor type)

RTC_DS3231 rtc;
DateTime initial_Date;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600);
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", 
                  "July", "August", "September", "October", "November", "December"};


Farm farm;

setupDevice nodeMCU;

String Topic = "DMA/Poultry/" + nodeMCU.ID();

EspMQTTClient client("broker.hivemq.com", 1883);// https://github.com/plapointe6/EspMQTTClient/blob/master/src/EspMQTTClient.h

void onConnectionEstablished()//send a confirmation mesage
{
  client.publish(Topic, "Connection Established");
}

const int capacity = 300;
DynamicJsonDocument doc(1024);

char json[] = R"(
  {
   "Device_ID": "",
   "Time_Stamp":{
      "Hour":"",
      "Minute":"",
      "Second":""
   },
   "Week":"",
   "Sensor_Data":{
      "Temperature":"",
      "Humidity":"",
      "Ammonia":""
   },
   "Temperature_Status":"",
   "Humidity_Status":"",
   "Ammonia_Status":""
}
)";


const char* ssid     = "Smart_Poultry"; //AP SSID
const char* password = "12345678"; // AP Password


String DEVICE_ID = nodeMCU.ID();
String SSID; 
String PASSWORD;
int NUMBER_OF_CONTACTS = 0;
String PHONEBOOK[max_number_of_contacts];
String CONTACT1, CONTACT2, CONTACT3, CONTACT4, CONTACT5;
String FARM_TYPE;
String INITIAL_AGE;
bool parameters_loaded = false; // is true when all parameters are set from the webserver
// These are the initial setup inputs from the webserver.

/*IP Address details */
IPAddress local_ip(192,168,2,1);
IPAddress gateway(192,168,2,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);


String checkUserID(String ID)
{
  String html = R"(
  <!DOCTYPE html>
  <html>
  <form name="loginform" onSubmit="return validateForm();" action="/form" method="post">
    <label>User name</label>
    <input type="text" name="usr" placeholder="username">
    <input type="submit" value="Login"/>
  </form>
  )";
  html += R"(
    <script>
      function validateForm() {
          var un = document.loginform.usr.value;
  )";

  html += "        var username = ";
  html += "\"";
  html += ID;
  html += "\";\n";

  html += R"(
        if ((un == username)) {
            return true;
        }
        else {
            alert ("Login was unsuccessful, please check your username");
            return false;
        }
      }
  </script>
  </html>
  )";
  return html;
}

String InputData()
{
  String html = R"(
    <!DOCTYPE html>
    <html lang=\"en\">
    <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <body>
            <h1>Device Setup</h1>
      <form name="configuration" onSubmit="return validateForm();" action="/save" method="post">
      WiFi SSID:
            <input type='text' name='ssid' id='ssid' size=16 autofocus> <br><br>
      WiFi Password:
            <input type='text' name='password' id='password' size=16 autofocus> <br><br> 
      Contact number 1:
            <input type='text' name='contact1' id='contact1' size=16 autofocus> <br><br>
      Contact number 2:  
      <input type='text' name='contact2' id='contact2' size=16 autofocus> <br><br>
      Contact number 3:
            <input type='text' name='contact3' id='contact3' size=16 autofocus> <br><br>
      Contact number 4:  
      <input type='text' name='contact4' id='contact4' size=16 autofocus> <br><br>
      Contact number 5:  
      <input type='text' name='contact5' id='contact5' size=16 autofocus> <br><br>
      Farm Type:<br>
      <input type='radio' name='farm' id='Brooder' value='Brooder'>
      <label for='Brooder'>Brooder</label><br>

      <input type='radio' name='farm' id='Grower' value='Grower'>
      <label for='Grower'>Grower</label><br>

      <input type='radio' name='farm' id='Parent' value='Parent'>
      <label for='Parent'>Parent</label><br><br>
      Bird age in weeks:
      <input type='text' name='age' id='age' size=16 autofocus> <br><br>
            <div>
            <br><button id="save_button">Save</button><br><br>
            </div>
      </form>
      <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>    
      <script>
        var ssid;
        var password;
        var contact1;
        var contact2;
        var contact3;
        var contact4;
        var contact5;
        var age;
        var type;
        $('#save_button').click(function(e){
          e.preventDefault();
          ssid = $('#ssid').val();
          password = $('#password').val();
          contact1 = $('#contact1').val();
          contact2 = $('#contact2').val();
          contact3 = $('#contact3').val();
          contact4 = $('#contact4').val();
          contact5 = $('#contact5').val();
          age = $('#age').val();  
          type = $("input[name='farm']:checked").val();
    
          $.get('/save?ssid=' + ssid + '&password=' + password + '&contact1=' + 
          contact1 + '&contact2=' + contact2 + '&contact3=' + contact3 + '&contact4=' + contact4 + 
          '&contact5=' + contact5 + '&age=' + age + '&type=' + type, function(data){
            console.log(data);
          });
        });      
      </script>
    </body>
  </html>
  )";
  return html;
}

void handle_OnConnect() {
  server.send(200, "text/html", checkUserID(DEVICE_ID)); 
}

void handle_Form()
{
  if(serial_debug)Serial.println("Handle form");
  server.send ( 200, "text/html", InputData());
}

void handle_Save() {
  if(serial_debug)Serial.println("Handle Save");
  if(serial_debug)Serial.println(server.hasArg("farm"));
  
  if (server.arg("ssid")!= ""){
    SSID = server.arg("ssid");
    if(serial_debug)Serial.println("SSID: " + server.arg("ssid"));
  }

  if (server.arg("password")!= ""){
    PASSWORD = server.arg("password");
    if(serial_debug)Serial.println("Password: " + server.arg("password"));
  }

  if (server.arg("contact1")!= ""){
    PHONEBOOK[NUMBER_OF_CONTACTS++] = server.arg("contact1");
    if(serial_debug)Serial.println("Contact 1: " + server.arg("contact1"));
    
  }

  if (server.arg("contact2")!= ""){
    PHONEBOOK[NUMBER_OF_CONTACTS++] = server.arg("contact2");
    if(serial_debug)Serial.println("Contact 2: " + server.arg("contact2"));
  }

  if (server.arg("contact3")!= ""){
    PHONEBOOK[NUMBER_OF_CONTACTS++] = server.arg("contact3");
    if(serial_debug)Serial.println("Contact 3: " + server.arg("contact3"));
  }

  if (server.arg("contact4")!= ""){
    PHONEBOOK[NUMBER_OF_CONTACTS++] = server.arg("contact4");
    if(serial_debug)Serial.println("Contact 4: " + server.arg("contact4"));
  }

  if (server.arg("contact5")!= ""){
    PHONEBOOK[NUMBER_OF_CONTACTS++] = server.arg("contact5");
    if(serial_debug)Serial.println("Contact 5: " + server.arg("contact5"));
  }

  if (server.arg("age")!= ""){
    INITIAL_AGE = server.arg("age");
    if(serial_debug)Serial.println("Age: " + server.arg("age"));
  }

  if (server.arg("type")!= ""){
    FARM_TYPE = server.arg("type");
    if(serial_debug)Serial.println("Farm type: " + server.arg("type"));
  }
  parameters_loaded = true;
  server.send(200, "text/html", "Saved Succesfully!");

}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void SendMessage(String message, String number)
{
  unsigned int len = number.length() + 1;
  char Number[len];
  number.toCharArray(Number, len);

  Serial.println(Number);
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  char outString[19];
  sprintf_P(outString, PSTR("AT+CMGS=\"%s\"\r"), Number);
  Serial.println(outString);
  delay(1000);
  Serial.println(message);// The SMS text you want to send
  delay(100);
  Serial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

String generateMessage()
{
  String Temperature_message = farm.getErrorMessageInParameters("temperature");
  String Humidity_message = farm.getErrorMessageInParameters("humidity");
  String Ammonia_message = farm.getErrorMessageInParameters("ammonia");


  // Serial.println(Temperature_message);
  // Serial.println(Humidity_message);
  // Serial.println(Ammonia_message);

  String error_message = "Temperature is " + Temperature_message + "\n"
                        + "Humidity is " + Humidity_message + "\n"
                        + "Ammonia is " + Ammonia_message;
  return error_message;
}

void sendMessagesToAllContacts()
{
  String msg = generateMessage();
  for(int i = 0; i < nodeMCU.NumberOfContacts(); i++)
  {
    SendMessage(msg, nodeMCU.Contact(i));
    delay(5000);
  }
}



void ICACHE_RAM_ATTR reset()
{
  nodeMCU.reset(1);
  //flag = true;
}

void showDateTime(DateTime now)
{
  char t[20];
  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  
  if(serial_debug)Serial.print(F("Date/Time: "));
  if(serial_debug)Serial.println(t);
}

int daysPassed()
{
  DateTime now = rtc.now();
  TimeSpan span = now - initial_Date;
  // showDateTime(initial_Date);
  // Serial.println(span.days());
  return span.days();
}

int weeksPassed()
{
  int days = daysPassed();
  return (int)days/7;
}

void setup() {
  Serial.begin(9600);
  rtc.begin();
  //Serial.println(nodeMCU.resetState());
  pinMode(D4, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D4), reset, FALLING);
  timeClient.begin();
  dht.begin();
  delay(5000);
  //nodeMCU.reset(1);
  if(nodeMCU.resetState())
  {
    // Set nodeMCU as Access Point
    if(serial_debug)Serial.print("Setting soft-AP configuration ... ");

    if(WiFi.softAPConfig(local_ip, gateway, subnet))
      if(serial_debug)Serial.println("Ready");

    if(serial_debug)Serial.print("Setting soft-AP ... ");

    if(WiFi.softAP(ssid, password))
      if(serial_debug)Serial.println("Ready");

    if(serial_debug)Serial.print("Soft-AP IP address = ");

    if(serial_debug)Serial.println(WiFi.softAPIP());

    delay(100);

    //Run an Web server which will provide a form for initial setup
    server.on("/", handle_OnConnect);
    server.on ("/form", handle_Form);
    server.on("/save", handle_Save);
    server.onNotFound(handle_NotFound);

    server.begin();
    //Run untill the form is filled.
    while(!parameters_loaded)
    {
      server.handleClient();
      yield();
    }

    nodeMCU.getSSID(SSID);
    nodeMCU.showSSID();
    nodeMCU.getPassword(PASSWORD);
    nodeMCU.showPassword();

    nodeMCU.getNumberOfContacts(NUMBER_OF_CONTACTS);

    for(int i = 0; i < NUMBER_OF_CONTACTS; i++)
    nodeMCU.getContact(PHONEBOOK[i]);

    farm.getFarmType(FARM_TYPE);
    farm.getInitAge(INITIAL_AGE);

    server.close(); // close the server
    WiFi.softAPdisconnect (true);// disconnect AP

    WiFi.begin(nodeMCU.SSID(), nodeMCU.Password());

    while ( WiFi.status() != WL_CONNECTED ) 
    {
      delay ( 500 );
      if(serial_debug)Serial.print ( "." );
    }

    if(serial_debug)Serial.println("Wifi connected!");
    if(serial_debug)Serial.print("Wifi Mac address: ");
    if(serial_debug)Serial.println(WiFi.macAddress());
    if(serial_debug)Serial.print("IP address: ");
    if(serial_debug)Serial.println(WiFi.localIP());

    //Set and save starting date
    timeClient.update();

    unsigned long epochTime = timeClient.getEpochTime();
    if(serial_debug)Serial.print("Epoch Time: ");
    if(serial_debug)Serial.println(epochTime);
    
    String formattedTime = timeClient.getFormattedTime();
    if(serial_debug)Serial.print("Formatted Time: ");
    if(serial_debug)Serial.println(formattedTime);  

    byte currentHour = timeClient.getHours() + 5; //should be +6, but it seems +5 works
    if(serial_debug)Serial.print("Hour: ");
    if(serial_debug)Serial.println(currentHour);  

    byte currentMinute = timeClient.getMinutes();
    if(serial_debug)Serial.print("Minutes: ");
    if(serial_debug)Serial.println(currentMinute); 
    
    byte currentSecond = timeClient.getSeconds();
    if(serial_debug)Serial.print("Seconds: ");
    if(serial_debug)Serial.println(currentSecond);  
    
    byte dayOfWeek = timeClient.getDay() ;
    String weekDay = weekDays[dayOfWeek]; // ntp uses 0 as sunday
    if(serial_debug)Serial.print("Week Day: ");
    if(serial_debug)Serial.println(weekDay);    

    //Get a time structure
    struct tm *ptm = gmtime ((time_t *)&epochTime); 

    byte monthDay = ptm->tm_mday;
    if(serial_debug)Serial.print("Month day: ");
    if(serial_debug)Serial.println(monthDay);

    byte currentMonth = ptm->tm_mon+1;
    if(serial_debug)Serial.print("Month: ");
    if(serial_debug)Serial.println(currentMonth);

    String currentMonthName = months[currentMonth-1];
    if(serial_debug)Serial.print("Month name: ");
    if(serial_debug)Serial.println(currentMonthName);

    int currentYear = ptm->tm_year+1900;
    if(serial_debug)Serial.print("Year: ");
    if(serial_debug)Serial.println(currentYear);

    //Print complete date:
    String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
    if(serial_debug)Serial.print("Current date: ");
    if(serial_debug)Serial.println(currentDate);

    if(serial_debug)Serial.println("");
    
    nodeMCU.setStartingDate(currentSecond, currentMinute, currentHour, dayOfWeek, 
                            monthDay, currentMonth, currentYear);
    if(serial_debug)Serial.println(nodeMCU.Second());
    if(serial_debug)Serial.println(nodeMCU.Minute());
    if(serial_debug)Serial.println(nodeMCU.Hour());
    if(serial_debug)Serial.println(nodeMCU.DayOfWeek());
    if(serial_debug)Serial.println(nodeMCU.DayOfMonth());
    if(serial_debug)Serial.println(nodeMCU.Month());
    if(serial_debug)Serial.println(nodeMCU.Year());
    
    rtc.adjust(DateTime(nodeMCU.Year(), nodeMCU.Month(), nodeMCU.DayOfMonth(),
                        nodeMCU.Hour(), nodeMCU.Minute(), nodeMCU.Second()));
    nodeMCU.reset(0);
  }

  initial_Date = DateTime(nodeMCU.Year(), nodeMCU.Month(), nodeMCU.DayOfMonth(),
                        nodeMCU.Hour(), nodeMCU.Minute(), nodeMCU.Second());
  if(serial_debug)
  {
    Serial.println("Showing Data...");
    nodeMCU.showID();
    nodeMCU.showSSID();
    nodeMCU.showPassword();
    nodeMCU.showContacts();
  }

  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    if(serial_debug)Serial.print(F("deserializeJson() failed: "));
    if(serial_debug)Serial.println(error.c_str());
  }
  doc["Device_ID"] = nodeMCU.ID();

  delay(5000);
}


void loop() {
  DateTime now = rtc.now();
  //showDateTime(now);
  sensors_event_t event;

  dht.temperature().getEvent(&event); //calls an event of the sensor
  float temp = event.temperature;
  doc["Sensor_Data"]["Temperature"] = temp;

  dht.humidity().getEvent(&event); // calls an event of the sensor
  float hum = event.relative_humidity ; // sensor event records humidity
  doc["Sensor_Data"]["Humidity"] = hum;

  float NH3 = analogRead(A0);
  doc["Sensor_Data"]["Ammonia"] = NH3 / 200;

  int temp_status = farm.check_Temperature(now.month(), weeksPassed() + 1, temp);
  //farm.show_Temperature_status();

  int hum_status = farm.check_Humidity(weeksPassed() + 1, hum);
  //farm.show_Humidity_status();

  int NH3_status = farm.check_Ammonia(weeksPassed() + 1, NH3 / 200);
  //farm.show_Ammonia_status();
  if(serial_debug)Serial.println("Weeks Passed: " + (String)weeksPassed());

  doc["Time_Stamp"]["Hour"] = now.hour();
  doc["Time_Stamp"]["Minute"] = now.minute();
  doc["Time_Stamp"]["Second"] = now.second();
  doc["Week"] = weeksPassed();

  doc["Temperature_Status"] = farm.Temperature_status();
  doc["Humidity_Status"] = farm.Humidity_status();
  doc["Ammonia_Status"] = farm.Ammonia_status();

  char payload[capacity];
  serializeJson(doc, payload);
  //Serial.println(payload);

  if(client.publish(Topic, payload))
  {
    if(serial_debug)Serial.println("Send Success!");
  }
  else
  {
    if(serial_debug)Serial.println("Send Failed!");
    // if(temp_status + hum_status + NH3_status > 3)
    // {
    //   sendMessagesToAllContacts();
    // }
  }
  
  delay(3000);
  client.loop();
}