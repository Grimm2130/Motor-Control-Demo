// load the wifi library
#include <WiFi.h>

// Define motor Control pins
// EN F B
int motor_1Pins[3] = {0,12,17};
int motor_2Pins[3] = {0,21,23};

// Replace with your network credentials
const char* ssid     = "ESP32-Motor-Control";
const char* password = NULL;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String MotorState = "off";

//-----------Global functions-------------
// Turns motor on
void motorForward(){
  // Configure to move forward
  digitalWrite(motor_1Pins[1], LOW);
  digitalWrite(motor_1Pins[2], HIGH);
  digitalWrite(motor_2Pins[1], LOW);
  digitalWrite(motor_2Pins[2], HIGH);
}

void motorBackwards(){
  // Configure to move forward
  digitalWrite(motor_1Pins[1], HIGH);
  digitalWrite(motor_1Pins[2], LOW);
  digitalWrite(motor_2Pins[1], HIGH);
  digitalWrite(motor_2Pins[2], LOW);
}

// Turns motor off
void motorOff(){
  // disable the chip
  digitalWrite(motor_1Pins[0], LOW);
}

// Turns motor On
void motorOn(){
  // disable the chip
  digitalWrite(motor_1Pins[0], 150);
}

// Turn left
void motorLeft(){
  digitalWrite(motor_1Pins[1], HIGH);
  digitalWrite(motor_1Pins[2], LOW);
  digitalWrite(motor_2Pins[1], LOW);
  digitalWrite(motor_2Pins[2], HIGH);
}

// Turn right
void motorRight(){
  digitalWrite(motor_1Pins[1], LOW);
  digitalWrite(motor_1Pins[2], HIGH);
  digitalWrite(motor_2Pins[1], HIGH);
  digitalWrite(motor_2Pins[2], LOW);
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Define the Access point
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
  // Initialize the output variables as outputs
  for(int i = 0; i < 3; i++){
    pinMode(motor_1Pins[i], OUTPUT);  
  }
  for(int i = 0; i < 3; i++){
    pinMode(motor_2Pins[i], OUTPUT);  
  }
  // Initialize the built-in led as an output
  pinMode(LED_BUILTIN, OUTPUT);
  // Set outputs to LOW
  for(int i = 0; i < 3; i++){
    digitalWrite(motor_1Pins[i], LOW);  
  }
  for(int i = 0; i < 3; i++){
    digitalWrite(motor_2Pins[i], LOW);  
  }
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off via http requests
            // Turn on
            if (header.indexOf("GET /motor/on") >= 0) {          // If request is to turn on
              Serial.println("Motor on");     
              // Set motor state as on                 
              MotorState = "on";                                 
              // Turn the motor on
              motorOn();                                
            }
            // Turn off 
            else if (header.indexOf("GET /motor/off") >= 0) {     // if request is to turn off
              Serial.println("Motor off");
              // Set motor state as off
              MotorState = "off";                                 
              // Turn the 
              motorOff();                                         
            }
            // Controls Direction of body based on commands
            // Forward
            else if(header.indexOf("GET /motor/Forward") >= 0) {     // if request is to turn off
              Serial.println("Motor Forward");
              // Move forward
              motorForward();
            }
            // Backwards
            else if(header.indexOf("GET /motor/Backward") >= 0) {     // if request is to turn off
              Serial.println("Motor Backward");
              // Move Backward
              motorBackwards();
            }
            // Turn right
            else if(header.indexOf("GET /motor/right") >= 0) {     // if request is to turn off
              Serial.println("Motor right");
              // Move Backward
              motorRight();
            }
            // Turn left
            else if(header.indexOf("GET /motor/left") >= 0) {     // if request is to turn off
              Serial.println("Motor left");
              // Move Backward
              motorLeft();
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            Serial.println("Got to html header");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".buttonON { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // Forward button css config
            client.println(".Forward {background-color: #FFFFFF; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;  position: absolute;");
            client.println("top: 200px;right: 500px;}");
            // Backward button config
            client.println(".Backward {background-color: #FFFFFF; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;  position: absolute;");
            client.println("top: 500px;right: 500px;}");
            // Left button css config
            client.println(".right-Container {background-color: #FFFFFF; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;  position: absolute;");
            client.println("top: 300px;left: 350px;}");
            // right button css config
            client.println(".left-Container {background-color: #FFFFFF; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;  position: absolute;");
            client.println("top: 300px;right: 350px}");
            client.println("</style></head>");
            // Web Page Heading
            client.println("<body><h1>ESP32 Motor Controller Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>Motor " + MotorState + "</p>");
            // If the MotorState is off, it displays the ON button       
            if (MotorState=="off") {
              client.println("<p><a href=\"/motor/on\"><button class=\"button\">ON</button></a></p></body>");
            } 
            // Only if the motor state is on display the following
            else {
              // Turn motor off
              client.println("<p><a href=\"/motor/off\"><button class=\"button button2\">OFF</button></a></p>");
              // Move forward button
              client.println("<div class = \"Forward\">");
              client.println("<p  style=\"color:black\" ><a href=\"/motor/Forward\"><button class=\"button_F\">Forward</button></a></p>");
              client.println("</div>");
              // Move backwards button
              client.println("<div class = \"Backward\">");
              client.println("<p style=\"color:black\"><a href=\"/motor/Backward\"><button class=\"button_B\">Backward</button></a></p>");
              client.println("</div>");
              // Move left button
              client.println("<div class = \"right-Container\">");
              client.println("<p><a href=\"/motor/left\"><button class=\"button2\"><----</button></a></p>");
              client.println(" </div>");
              // Move right button
              client.println("<div class = \"left-Container\">");
              client.println("<p><a href=\"/motor/right\"><button class=\"button3\">----></button></a></p></body>");
              client.println("</div>");
            }
             
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } 
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
