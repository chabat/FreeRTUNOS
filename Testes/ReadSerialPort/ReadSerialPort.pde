/* 
ARDUINO TO PROCESSING
Read Serial messages from Arduino for use in Processing. 
*Even though Serial Library comes with install of Processing, upon first usage, you may be prompted to execute two sudo Terminal 
commands after entering your user password*
Created by Daniel Christopher 10/27/12
Public Domain
*/
/****************************************************************************
modified to write data coming over the serial port (from an Arduino) into a file
Margaret Johnson 11/02/2014
*****************************************************************************/

import processing.serial.*; //import the Serial library

int end = 10;    // the number 10 is ASCII for linefeed (end of serial.println), later we will look for this to break up individual messages
String serial;   // declare a new string called 'serial' . A string is a sequence of characters (data type know as "char")
Serial port;  // The serial port, this is a new instance of the Serial class (an Object)
PrintWriter output;

void setup() {
 //  println(Serial.list());
 // port = new Serial(this, Serial.list()[0], 9600); // initializing the object by assigning a port and baud rate (must match that of Arduino)
 //MJ: BAD PRACTICE - hard coded serial port.  Most of the Processing sketches I saw used the line above to associate a variable to a port
 //this isn't a great idea either because as is my case - the serial port used by the ARduino IDE is not the first one...I don't know the
 //Processing code well enough to generalize.
  String portName = "/dev/ttyUSB0";
  port = new Serial(this, portName, 9600);
  port.clear();  // function from serial library that throws out the first reading, in case we started reading in the middle of a string from Arduino
  //MJ: HARD CODED the path and name of the output file to hold data.  The idea is this is throwaway code where the UI IS the code.
  //output = createWriter("/Users/margaret/Documents/Lettuce Buddy HW/HealthypH/DataFiles/ASContinuousRead/ASBakngSoda.csv");
  //serial = port.readStringUntil(end); // function that reads the string from serial port until a println and then assigns string to our string variable (called 'serial')
  //serial = null; // initially, the string will be null (empty)
}

void draw() {
  port.available();
  while (port.available() > 0) { //as long as there is data coming from serial port, read it and store it 
    serial = port.readStringUntil(end);
  }
    if (serial != null) {  //if the string is not empty, print the following
 
      print(serial);
      //output.print(serial);
    }
}
//There's a little window that gets opened up (I think from draw().  When this window is active and any key is pressed, the file will be closed 
//correctly.  Once closed, stop the sketch.  File is ready.
void keyPressed() {
    output.flush();  // Writes the remaining data to the file
    output.close();  // Finishes the file
    exit();  // Stops the program
}