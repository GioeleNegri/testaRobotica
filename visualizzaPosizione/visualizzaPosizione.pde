//LIBRERIE
import processing.serial.*;

//VARIABILI
Serial myPort = null;
int angolo = 0;
void setup(){
  size(600,600);
  myPort = new Serial(this, Serial.list()[0], 9600);  
}  


void draw(){
  String str = myPort.readString();
  
  //centro
  translate(width/2,height/2);
  background(40);
  //assi cartesiani
  stroke(0,255,255,50);
  line(-width,0,width,0);
  line(0,-height/2,0,height/2);
  //linea motore
    
  stroke(255,0,0,255);
  rotate(-radians(angolo));
  line(0,0,10,500);
  
  
  
  if(str != null){
      angolo = int(str.substring(0,str.length()-1));
      println(str);
  }else{  }
}