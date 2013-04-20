#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
byte mac[] = { 0xDF, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 0, 22 }; // ip in lan
byte gateway[] = { 192, 168, 0, 254 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port
File webFile;   // the web page file on the SD card
String HTTP_req; // stores the HTTP request

byte isOn;

DeviceAddress deviceAddress;
int numberOfDevices;
float sensorTemps[3];


#define ONE_WIRE_BUS 34  //Data wire for Sensors
#define TEMPERATURE_PRECISION 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//-----Set these to the correct pins-----
int pumpPin = 42; //pump pin on arduino
int rimsPin = 40; //rims pin on arduino
int liquorPin = 38; //liquor pin on arduino
int boilPin = 44; //boil pin on arduino

//-----Set these to the correct sensors-----
int rimsSensor = 1; //rims sensor number
int liquorSensor = 0; //liquor sensor number
int boilSensor = 2; //boil sensor number

int rimsSet = 150; //temp the rims is set at
int liquorSet = 170; //temp the liquor is set at
int boilSet = 208; //temp the boil is set at

String pumpStatus = "0"; //is the pump on or off
String rimsStatus = "0"; //is the rims on or off
String liquorStatus = "0"; //is the liquor on or off
String boilStatus = "0"; //is the boil on or off

String pump = "pump=";
String rims = "rims=";
String liquor = "liquor=";
String boil = "boil=";

int readLength = 0; //length of HTTP_req
int beginLocation = 0; //Begining location of search string
String inputString = ""; //Substring to be converted into char array
char carry [10]; //Array of characters to be converted to int for temp set

//////////////////////
 
void setup(){

  pinMode(pumpPin, OUTPUT); //set pump pin as digital output
  pinMode(rimsPin, OUTPUT); //set rims pin as digital output
  pinMode(liquorPin, OUTPUT); //set liquor pin as digital output
  pinMode(boilPin, OUTPUT); //set boil pin as digital output
  digitalWrite(pumpPin, LOW); //set pump off on first start
  digitalWrite(rimsPin, LOW); //set rims off on first start
  digitalWrite(liquorPin, LOW); //set liquor off on first start
  digitalWrite(boilPin, LOW); //set boil off on first start
  
  Serial.begin(9600);
  Serial.println("Serial Output:");  
  sensors.begin();
 
  Ethernet.begin(mac, ip, gateway, subnet); //Start Ethernet
  server.begin();
  
  numberOfDevices = sensors.getDeviceCount(); //get number of temp sensors
  
  // initialize SD card
  Serial.println("Initializing SD card ...");
 
  if(!SD.begin(4)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;
  }
  
  Serial.println("SUCCESS - SD card initialized.");
  // check for index.htm file
  if (!SD.exists("index.htm")) {
    Serial.println("ERROR - Can't find index.htm file!");
    return;
  }
  
  Serial.println("SUCCESS - Found index.htm file."); 
}


void loop(){
  /////////////////////Get Sensor data
//  sensors.requestTemperatures(); // Send the command to get temperatures
//  
//  for(int i=0;i<numberOfDevices; i++) // Loop through each device, print out temperature data
//  {
//    if(sensors.getAddress(deviceAddress, i))
//  {
//           sensorTemps[i] = sensors.getTempF(deviceAddress);
//	} 
//  }
//  delay(25);
///////////////////////Checking Pin Status
//            if(HTTP_req.indexOf("pump=1") >0) //Check Pump Status
//            {
//              pumpStatus = "1";
//            }
//            else
//            if(HTTP_req.indexOf("pump=0") >0) //Check Pump Status
//            {
//               pumpStatus = "0";
//            }
//            
//            if(HTTP_req.indexOf("rims=1") >0) // Check RIMS Status
//           {
//              rimsStatus = "1";
//            }
//            else
//            if(HTTP_req.indexOf("rims=0") >0) // Check RIMS Status
//            {
//               rimsStatus = "0";
//            }
//            
//            if(HTTP_req.indexOf("liquor=1") >0) // Check Liqour Status
//            {
//              liquorStatus = "1";
//            }
//            else
//            if(HTTP_req.indexOf("liquor=0") >0) // Check Liqour Status
//            {
//               liquorStatus = "0";
//            }
//            
//            if(HTTP_req.indexOf("boil=1") >0) // Check Boil Status
//            {
//              boilStatus = "1";
//            }
//            else
//            if(HTTP_req.indexOf("boil=0") >0) // Check Boil Status
//            {
//               boilStatus = "0";
//            } 
//            
//            
//            if(HTTP_req.indexOf("rimsTemp=") >0)
//            {
//            readLength = HTTP_req.length() - 11; //find end of string and remove http end 
//            beginLocation = HTTP_req.indexOf("rimsTemp=") + 9; //find rimsTemp= and add 9 to find value after =
//            inputString = HTTP_req.substring(beginLocation, readLength); //inputSting is the substring of the HTTP_req containing the set vale
//            inputString.toCharArray(carry, sizeof(carry)); //convert string value to char array
//            rimsSet = atoi(carry); //convert char array to rimsSet value
//            }
//            if(HTTP_req.indexOf("liquorTemp=") >0)
//            {
//            readLength = HTTP_req.length() - 11; //find end of string and remove http end
//            beginLocation = HTTP_req.indexOf("liquorTemp=") + 11; //find liquorTemp= and add 11 to find value after =
//            inputString = HTTP_req.substring(beginLocation, readLength); //inputSting is the substring of the HTTP_req containing the set vale
//            inputString.toCharArray(carry, sizeof(carry)); //convert string value to char array
//            liquorSet = atoi(carry); //convert char array to liquorSet value
//            }
//            if(HTTP_req.indexOf("boilTemp=") >0)
//            {
//            readLength = HTTP_req.length() - 11; //find end of string and remove http end
//            beginLocation  = HTTP_req.indexOf("boilTemp=") + 9; //find boilTemp= and add 9 to find value after =
//            inputString = HTTP_req.substring(beginLocation, readLength); //inputSting is the substring of the HTTP_req containing the set vale
//            inputString.toCharArray(carry, sizeof(carry)); //convert string value to char array
//            boilSet = atoi(carry); //convert char array to boilSet value
//            }
//            
//           HTTP_req=""; //clearing string for next read
//           
//          ///////////////////// Control Arduino Pins
//          if(pumpStatus == "1") //checks for on
//          {
//            digitalWrite(pumpPin, HIGH);    // set pin pumpPin high
//          }
//          if(pumpStatus == "0")//checks for off
//          {
//            //digitalWrite(rimsPin, LOW);    // set pin rimsPin low
//            digitalWrite(pumpPin, LOW);    // set pin pumpPin low
//          }
//          if(rimsStatus == "1" && sensorTemps[rimsSensor] < rimsSet)//checks for on
//          {
//            digitalWrite(rimsPin, HIGH);    // set pin rimsPin high
//            //digitalWrite(pumpPin, HIGH);    // set pin pumpPin high
//          }
//          if((rimsStatus == "0" ) || (rimsStatus == "1" && sensorTemps[rimsSensor] > rimsSet))//checks for off
//          {
//            digitalWrite(rimsPin, LOW);    // set pin rimsPin low
//          }
//          if(liquorStatus == "1" && sensorTemps[liquorSensor] < liquorSet)//checks for on
//          {
//            digitalWrite(liquorPin, HIGH);    // set pin liquorPin high
//          }
//          if((liquorStatus == "0" ) || (liquorStatus == "1" && sensorTemps[liquorSensor] > liquorSet))//checks for off
//          {
//            digitalWrite(liquorPin, LOW);    // set pin liquorPin low
//          }
//          if(boilStatus == "1" && sensorTemps[boilSensor] < boilSet)//checks for on
//          {
//            digitalWrite(boilPin, HIGH);    // set pin boilPin high
//          }
//          if((boilStatus == "0" ) || (boilStatus == "1" && sensorTemps[boilSensor] > boilSet))//checks for off
//          {
//            digitalWrite(boilPin, LOW);    // set pin boilPin low
//          }

if (!server.available()){
  //Serial.println("Server unavailable...");
}

  EthernetClient client = server.available();  // Create a client connection
 // Serial.println("Initiating http client...");
  if (client) {
    while (client.connected()) {
      Serial.println("Client Connected ...");
      if (client.available()) {
        char c = client.read();
 
        if (HTTP_req.length() < 100) { //read char by char HTTP request
 
          HTTP_req += c; //store characters to string
          //Serial.print(HTTP_req);
        }
        if (c == '\n') { //if HTTP request has ended
           
          // send a standard http response header
          // begin header
          client.println("HTTP/1.1 200 OK"); //send new page
          // conte-type changes based on request
          // remainder of header sends below, depending on condition
          
          //Ajax request
//          if (HTTP_req.indexOf("ajax_switch") > -1) 
//          {
//            client.println("Content-Type: text/xml");
//            client.println("Connection: keep-alive");
//            client.println();
//             //send XML file containing input states
//            XML_response(client);           
//          }
//          
//          else {
            // send rest of HTTP header
            client.println("Content-Type: text/html");
            client.println("Connection: keep-alive");
            client.println();
            // send web page
            Serial.println("Opening file...");
            webFile = SD.open("index.htm"); // open web page file
            if (webFile) {
              while(webFile.available()){
                client.write(webFile.read());  //send web page to client
                Serial.println("Reading file...");
              }
              webFile.close();
              Serial.println("Closing file...");
            }
//          }
          
          // display received HTTP request on serial port
          //Serial.print(HTTP_req);
          HTTP_req = "";  //finished with the request, empty the string
          break;        
          // end header
          
//          
//          client.println("<a href=" + buildURL("1", rimsStatus, liquorStatus, boilStatus) + ">Turn On Pump</a>");
//          client.println("<a href=" + buildURL("0", rimsStatus, liquorStatus, boilStatus) + ">Turn Off Pump</a><br />");
//          client.print(getOnOff(pumpPin));
//    
//          ///////////////Liquor
//          client.println("<a href=" + buildURL(pumpStatus, rimsStatus, "1", boilStatus) + ">Turn On Liquor</a>");
//          client.println("<a href=" + buildURL(pumpStatus, rimsStatus, "0", boilStatus) + ">Turn Off Liquor</a><br />");
//          client.print(getOnOff(liquorPin));
//          client.print(getOnOffset(liquorStatus));
//          client.print(sensorTemps[liquorSensor]);
//          client.print(liquorSet);  
//          client.println("<form name='input' action='/' method='get'>");
//          client.println("<input type='hidden' name='rims' value='" + rimsStatus + "'>");
//          client.println("<input type='hidden' name='pump' value='" + pumpStatus +"'>");
//          client.println("<input type='hidden' name='liquor' value='" + liquorStatus +"'>");          
//          client.println("<input type='hidden' name='boil' value='" + boilStatus +"'>");                    
//          client.println("<input type='text' name='liquorTemp'>");
//          client.println("<input type='submit' value='Submit'>");
//                   
//          ///////////////RIMS
//          client.println("<a href=" + buildURL(pumpStatus, "1", liquorStatus, boilStatus) + "> Turn On RIMS</a>");
//          client.println("<a href=" + buildURL(pumpStatus, "0", liquorStatus, boilStatus) + ">Turn Off RIMS</a><br />");
//          client.print(getOnOff(rimsPin));
//          client.print(getOnOffset(rimsStatus));
//          client.print(sensorTemps[rimsSensor]);
//          client.print(rimsSet);          
//          client.println("<form name='input' action='/' method='get'>");
//          client.println("<input type='hidden' name='rims' value='" + rimsStatus + "'>");
//          client.println("<input type='hidden' name='pump' value='" + pumpStatus +"'>");
//          client.println("<input type='hidden' name='liquor' value='" + liquorStatus +"'>");          
//          client.println("<input type='hidden' name='boil' value='" + boilStatus +"'>");                    
//          client.println("<input type='text' name='rimsTemp'>");
//          client.println("<input type='submit' value='Submit'>");
//          client.println("<a href=" + buildURL(pumpStatus, rimsStatus, liquorStatus, "1") + ">Turn On Boil</a>");
//          client.println("<a href=" + buildURL(pumpStatus, rimsStatus, liquorStatus, "0") + ">Turn Off Boil</a><br />");
//
//          client.print(getOnOff(boilPin));
//          client.print(getOnOffset(boilStatus));
//          client.print(sensorTemps[boilSensor]);
//          client.print(boilSet);
//          client.println("<form name='input' action='/' method='get'>");
//          client.println("<input type='hidden' name='rims' value='" + rimsStatus + "'>");
//          client.println("<input type='hidden' name='pump' value='" + pumpStatus +"'>");
//          client.println("<input type='hidden' name='liquor' value='" + liquorStatus +"'>");          
//          client.println("<input type='hidden' name='boil' value='" + boilStatus +"'>");                   
 
          delay(1);
          Serial.println("Stopping client...");
          client.stop(); //stopping client
        }
      }
    }
  }
}

String getOnOff(int val){ 
  isOn = digitalRead(val); //get bit val of the pin
  if (isOn)
  {  
     return "<FONT COLOR=""green"">ON</FONT>";
  }
  else
  {
     return "<FONT COLOR=""red"">OFF</FONT>";
  }
}
  
float getTempF(DeviceAddress tempSensor)
{
  sensors.requestTemperatures();
  float temperatureF = (sensors.getTempF(tempSensor));
 
   return temperatureF;  
}

String getOnOffset(String val){ 
  if(val == "1")
  {  
     return "<FONT COLOR=""green"">ON</FONT>";
  }
  else
  if(val == "0")
  {
     return "<FONT COLOR=""red"">OFF</FONT>";
  }
}

String buildURL (String myPumpStatus, String myRimsStatus, String myLiquorStatus, String myBoilStatus)
{
  String constructor = "?";
  constructor += pump + myPumpStatus;
  constructor += "&";
  constructor += rims + myRimsStatus;  
  constructor += "&";
  constructor += liquor + myLiquorStatus;  
  constructor += "&";  
  constructor += boil + myBoilStatus;  
  return constructor;
}

void XML_response(EthernetClient cl)
{
  cl.print("<?xml version = \"1.0\" ?>");
  cl.print("<inputs>");
  cl.print("</inputs");
 
}
