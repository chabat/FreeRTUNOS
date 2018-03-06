import processing.serial.*;

Serial myPort;
String val;

void Setup(){
  String portName = Serial.list()[32];
  //String portName = "/dev/ttyUSB0";
  myPort = new Serial(this, portName, 9600);
}

void draw(){
  //println(Serial.list()); 
  myPort.available();
  //if (myPort.available() > 0)
    //val = myPort.readStringUntil('\n');
  //if(val != null)
    //println(val);
}