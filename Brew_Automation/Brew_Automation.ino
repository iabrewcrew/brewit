/*--------------------------------------------------------------
  Program:      eth_websrv_SD_Ajax_XML

  Description:  Arduino web server that serves up a web page
                that displays the status of two switches and
                one analog input.
                The web page is stored on the SD card.
                The web page contains JavaScript code that uses
                Ajax and XML to get the states of the switches
                and value of the analog input.
  
  Hardware:     Arduino Uno and official Arduino Ethernet
                shield. Should work with other Arduinos and
                compatible Ethernet shields.
                2Gb micro SD card formatted FAT16.
                Push button switches interfaced to pin 7 and 8
                of the Arduino. Potentiometer interfaced to A2
                analog input.
                
  Software:     Developed using Arduino 1.0.3 software
                Should be compatible with Arduino 1.0 +
                SD card contains web page called index.htm
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - SD card examples by David A. Mellis and
                  Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet
                - SD Card library documentation:
                  http://arduino.cc/en/Reference/SD

  Date:         27 March 2013
 
  Author:       W.A. Smith, http://startingelectronics.com
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

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

byte isOn;

int readLength = 0; //length of HTTP_req
int beginLocation = 0; //Begining location of search string
String inputString = ""; //Substring to be converted into char array
char carry [10]; //Array of characters to be converted to int for temp set

//////////////////////
 

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 20); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80
File webFile;               // the web page file on the SD card
String HTTP_req;            // stores the HTTP request

void setup()
{
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
    Serial.begin(9600);       // for debugging
    
    // initialize SD card
    Serial.println("Initializing SD card...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
    }
    Serial.println("SUCCESS - SD card initialized.");
    // check for index.htm file
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return;  // can't find index file
    }
    Serial.println("SUCCESS - Found index.htm file.");
    pinMode(7, INPUT);        // switch is attached to Arduino pin 7
    pinMode(8, INPUT);        // switch is attached to Arduino pin 8
}

void loop()
{
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                HTTP_req += c;  // save the HTTP request 1 char at a time
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    // remainder of header follows below, depending on if
                    // web page or XML page is requested
                    // Ajax request - send XML file
                    if (HTTP_req.indexOf("ajax_inputs") > -1) {
                        // send rest of HTTP header
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        // send XML file containing input states
                        XML_response(client);
                    }
                    else {  // web page request
                        // send rest of HTTP header
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");
                        client.println();
                        // send web page
                        webFile = SD.open("index.htm");        // open web page file
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read()); // send web page to client
                            }
                            webFile.close();
                        }
                    }
                    // display received HTTP request on serial port
                    Serial.print(HTTP_req);
                    HTTP_req = "";            // finished with request, empty string
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}

// send the XML file with switch statuses and analog value
void XML_response(EthernetClient cl)
{
    int analog_val;
    
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    cl.print("<liquorStatus>");
    cl.print(getOnOff(liquorPin));
//    if (digitalRead(7)) {
//        cl.print("ON");
//    }
//    else {
//        cl.print("OFF");
//    }
    cl.print("</liquorStatus>");
    cl.print("<button2>");
    if (digitalRead(8)) {
        cl.print("ON");
    }
    else {
        cl.print("OFF");
    }
    cl.print("</button2>");
    // read analog pin A2
    analog_val = analogRead(2);
    cl.print("<analog1>");
    cl.print(analog_val);
    cl.print("</analog1>");
    cl.print("</inputs>");
}

String getOnOff(int val){ 
  isOn = digitalRead(val); //get bit val of the pin
  if (isOn)
  {  
     return "ON";
  }
  else
  {
     return "OFF";
  }
}

