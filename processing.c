import processing.serial.*;     // import serial library
String xString = null ;
String yString = null;

String getV = null;
String getX = null;

String getP = null;
String getA = null;
String getB = null;
String getC = null;
String getD = null;

Serial myPort;                      // declare a serial port
// variable to store x and y co-ordinates for vertices
float x, y;                         

float xA, yA;                       
float xB, yB;                       
float xC, yC;                       
float xD, yD;                       

int radius = 400; 
int ln = 13;
int w = 300;                    // set an arbitary width value
int degree = 0;                 // servo position in degrees
int nilai_motion = 50;//35;
int skala = 5;//1;

int cacah = 0;
float waktu_rotasi = 0;
float waktu_rotasi_mulai = 0;
float waktu_rotasi_akhir = 0;

int value  = 0;                  // value from sensor
int valueA = 0;                  // value from sensor
int valueB = 0;                  // value from sensor
int valueC = 0;                  // value from sensor
int valueD = 0;                  // value from sensor

int motion = 0;                 
int[] newValue = new int[181];  // create an array to store each new sensor value for each servo position
int[] oldValue = new int[181];  // create an array to store the previous values.

// create an array to store each new sensor value for each servo position
int[] newValueA = new int[91];  
int[] oldValueA = new int[91];  
int[] newValueB = new int[91]; 
int[] oldValueB = new int[91];  
int[] newValueC = new int[91];  
int[] oldValueC = new int[91];  
int[] newValueD = new int[91];  
int[] oldValueD = new int[91];  

PFont myFont;                   // setup fonts in Processing
PFont RadarFont;                   
PFont JudulFont;                   
PFont SubJudulFont;                   
float radarDist = 0;              // set value to configure Radar 
int firstRun = 0;               // value to ignore triggering motion on the first 2 servo sweeps

/* create background and serial buffer */
void setup(){


  size(1100, 720); // yang ini lingkaran penuh
  
  
  background (0); // 0 = black warna hitam 
  myFont = createFont("verdana", 12);
  RadarFont = createFont("verdana", 9);
  JudulFont = createFont("verdana", 24);
  SubJudulFont = createFont("verdana", 18);
  textFont(myFont);
  myPort = new Serial(this, Serial.list()[2], 9600);
  myPort.bufferUntil(ln);
}

/* draw the screen */
void draw(){

  fill(0);                              // set the following shapes to be black
  noStroke();                           // set the following shapes 
  ellipse(radius, radius, 800,800);//750, 750);    // draw a circle with a width/ height = 750 with its center position (x and y) set by the radius
  
 
  if (degree >= 89) {                  // if at the far right then set motion = 1/ true we're about to go right to left
    motion = 1;                         // this changes the animation to run right to left
  }
  if (degree <= 1) {                    // if servo at 0 degrees then we're about to go left to right
    motion = 0;                         // this sets the animation to run left to right
  }
  fill(0);
  rect(0,20,1000,200);
  
  strokeWeight(7); 
  if (motion == 0) 
  {                    // if going left to right

// draw 20 lines with fading colour each 1 degree further round than the last
    for (int i = 0; i <= 20; i++) 
    {     
      stroke(0, (10*i), 0);             
line(radius, radius, radius + cos(radians(degree+(180+i)))*w, radius + sin(radians(degree+(180+i)))*w); // line(start x, start y, end x, end y) A
line(radius, radius, radius + cos(radians(degree+(90+i)))*w, radius + sin(radians(degree+(90+i)))*w); // line(start x, start y, end x, end y) B
line(radius, radius, radius + cos(radians(degree+(i)))*w, radius + sin(radians(degree+(i)))*w); // line(start x, start y, end x, end y)     C
line(radius, radius, radius + cos(radians(degree+(270+i)))*w, radius + sin(radians(degree+(270+i)))*w); // line(start x, start y, end x, end y)  D
    }  
  } 

else 

{                              // if going right to left
    for (int i = 20; i >= 0; i--) {     // draw 20 lines with fading colour
      stroke(0,200-(10*i), 0);          // using standard RGB values, each between 0 and 255
line(radius, radius, radius + cos(radians(degree+(180+i)))*w, radius + sin(radians(degree+(180+i)))*w); // A
line(radius, radius, radius + cos(radians(degree+(90+i)))*w, radius + sin(radians(degree+(90+i)))*w); // line(start x, start y, end x, end y) B
line(radius, radius, radius + cos(radians(degree+(i)))*w, radius + sin(radians(degree+(i)))*w); // line(start x, start y, end x, end y)     C
line(radius, radius, radius + cos(radians(degree+(270+i)))*w, radius + sin(radians(degree+(270+i)))*w); // line(start x, start y, end x, end y)  D
    }
  }

  noStroke();                           // no outline
  /* first sweep */
  fill(0,50,0);   
    beginShape();                         // start drawing shape
    for (int i = 0; i <= 90; i++) {     // for each degree in the array
    
      xA = radius + cos(radians((180+i)))*((oldValueA[i]) * skala); // create x coordinate 
      yA = radius + sin(radians((180+i)))*((oldValueA[i]) * skala); // create y coordinate
      vertex(xA, yA);                     // plot vertices
   
    }
    vertex(radius, radius);
  endShape();                           // end shape
  
   noStroke();                           // no outline
  /* first sweep */
  fill(0,50,0);  
  beginShape();
  
   for (int i = 0; i <= 90; i++) {     // for each degree in the array
    
      xB = radius + cos(radians((270+i)))*((oldValueB[i]) * skala); // create x coordinate 
      yB = radius + sin(radians((270+i)))*((oldValueB[i]) * skala); // create y coordinate
      vertex(xB, yB);                     // plot vertices
    
    }
    vertex(radius, radius);
  endShape();                           // end shape
  
noStroke();                           // no outline
  /* first sweep */
  fill(0,50,0);     
beginShape();
//  
   for (int i = 0; i <= 90; i++) {     // for each degree in the array
    
      xC = radius + cos(radians((i)))*((oldValueC[i]) * skala); // create x coordinate 
      yC = radius + sin(radians((i)))*((oldValueC[i]) * skala); // create y coordinate
      vertex(xC, yC);                    // plot vertices
    
    }
    vertex(radius, radius);
  endShape(); 
//  
//   
 noStroke();                           // no outline
  /* first sweep */
  fill(0,50,0);  
beginShape();
//  
   for (int i = 0; i <= 90; i++) {     // for each degree in the array
    
      xD = radius + cos(radians((i+90)))*((oldValueD[i]) * skala); // create x coordinate 
      yD = radius + sin(radians((i+90)))*((oldValueD[i]) * skala); // create y coordinate
      vertex(xD, yD);                      // plot vertices
    
    }
    vertex(radius, radius);
  endShape(); 
  
  
    /* second sweep */
  fill(0,110,0);
  beginShape();
    for (int i = 0; i <= 90; i++) {
      xA = radius + cos(radians((180+i)))*(newValueA[i]  * skala);
      yA = radius + sin(radians((180+i)))*(newValueA[i]  * skala);
      vertex(xA, yA);

    }
    vertex(radius, radius);
  endShape();
  
  ///////////
    fill(0,110,0);
  beginShape();
    for (int i = 0; i <= 90; i++) {
    xB = radius + cos(radians((270+i)))*(newValueB[i]  * skala);
    yB = radius + sin(radians((270+i)))*(newValueB[i] * skala);
    vertex(xB, yB);

    }
    vertex(radius, radius);
  endShape();
  
    fill(0,110,0);
  beginShape();
    for (int i = 0; i <= 90; i++) {
    xC = radius + cos(radians((i)))*(newValueC[i] * skala);
      yC = radius + sin(radians((i)))*(newValueC[i] * skala);
      vertex(xC, yC);

    }
    vertex(radius, radius);
  endShape();
  
    fill(0,110,0);
  beginShape();
    for (int i = 0; i <= 90; i++) {
    xD = radius + cos(radians((i+90)))*(newValueD[i] * skala);
      yD = radius + sin(radians((i+90)))*(newValueD[i] * skala);
      vertex(xD, yD);

    }
    vertex(radius, radius);
  endShape();
    /* average */
  fill(0,170,0);
  beginShape();
      vertex(radius, radius);
    for (int i = 0; i <= 90; i++) {
      xA = radius + cos(radians((180+i)))*((newValueA[i]+oldValueA[i]) * skala/2); // create average
      yA = radius + sin(radians((180+i)))*((newValueA[i]+oldValueA[i]) * skala/2);
      vertex(xA, yA);
   
    }
  endShape();
  
    fill(0,170,0);
  beginShape();
      vertex(radius, radius);
    for (int i = 0; i <= 90; i++) {
      xB = radius + cos(radians((270+i)))*((newValueB[i]+oldValueB[i]) * skala/2); // create average
      yB = radius + sin(radians((270+i)))*((newValueB[i]+oldValueB[i]) * skala/2);
      vertex(xB, yB);
       
    }
//    vertex(radius, radius);
  endShape();

////      

  fill(0,170,0);
  beginShape();
      vertex(radius, radius);
    for (int i = 0; i <= 90; i++) {
      xC = radius + cos(radians((i)))*((newValueC[i]+oldValueC[i]) * skala/2); // create average
      yC = radius + sin(radians((i)))*((newValueC[i]+oldValueC[i]) * skala/2);
      vertex(xC, yC);
       
    }
//    vertex(radius, radius);
  endShape();
  
    fill(0,170,0);
  beginShape();
      vertex(radius, radius);
    for (int i = 0; i <= 90; i++) {
      xD = radius + cos(radians((90+i)))*((newValueD[i]+oldValueD[i]) * skala/2); // create average
      yD = radius + sin(radians((90+i)))*((newValueD[i]+oldValueD[i]) * skala/2);
      vertex(xD, yD); 
       
    }
//    vertex(radius, radius);
  endShape();

    /* if after first 2 sweeps, highlight motion with red circle*/
      noStroke();
fill(0);
rect(800,20,200,1000);
//  fill(0, 100, 0);
  fill(0, 255, 0);
  float m = millis();
 int s = second();  // Values from 0 - 59
 waktu_rotasi = waktu_rotasi_akhir - waktu_rotasi_mulai;

 text("waktu: " + Float.toString(m) ,800,80,250,50);
 text("waktu rotasi : " + Float.toString(waktu_rotasi)+ "ms" , 800,100,250,50);
  text("Distance PING 1 :  "+Integer.toString(valueA), 800, 180, 250, 40);         // text(string, x, y, width, height)
  text("Distance PING 2 :  "+Integer.toString(valueB), 800, 200, 250, 40);         // text(string, x, y, width, height)
  text("Distance PING 3 :  "+Integer.toString(valueC), 800, 220, 250, 40);         // text(string, x, y, width, height)
  text("Distance PING 4 :  "+Integer.toString(valueD), 800, 240, 250, 40);         // text(string, x, y, width, height)
  text("Degrees: "          +Integer.toString(degree), 800, 260, 100, 50);         // use Integet.toString to convert numeric to string as text() only outputs strings
     textFont(JudulFont); 
  text("Radar screen Dipta", 280, 20, 250, 50);
       textFont(SubJudulFont); 
  text("Waktu Sweeping", 800, 50, 250, 50);
  text("Data Sensor", 800, 150, 250, 50);
     textFont(myFont);
    
  text("PING 1", 820, 300, 300, 40);         // text(string, x, y, width, height)
  text("PING 2", 870, 300, 300, 40);         // text(string, x, y, width, height)
  text("PING 3", 920, 300, 300, 40);         // text(string, x, y, width, height)
  text("PING 4", 970, 300, 250, 40);         // text(string, x, y, width, height)   
//  for(int i=0; i<=90){}
  for (int i=0; i <=90 ; i+= 5)
  {
  text(Integer.toString(i), 800, (320 + i*4), 100, 40);         // text(string, x, y, width, height)
  text(Integer.toString(newValueA[i]), 840, (320 + i*4), 100, 40);         // text(string, x, y, width, height)
  text(Integer.toString(newValueB[i]), 890, (320 + i*4), 100, 40);
  text(Integer.toString(newValueC[i]), 940, (320 + i*4), 100, 40);
  text(Integer.toString(newValueD[i]), 990, (320 + i*4), 100, 40);
  
  }
  

  if (firstRun >= 360) {
    stroke(150,0,0);
    strokeWeight(1);
    noFill();
      for (int i = 0; i <= 90; i++) {
        if (oldValueA[i] - newValueA[i] > nilai_motion || newValueA[i] - oldValueA[i] > nilai_motion) {
          xA = radius + cos(radians((180+i)))*(newValueA[i] * skala);
          yA = radius + sin(radians((180+i)))*(newValueA[i] * skala);
          ellipse(xA, yA, 10, 10);
        }
        
        if (oldValueB[i] - newValueB[i] > nilai_motion || newValueB[i] - oldValueB[i] > nilai_motion) {
          xB = radius + cos(radians((270+i)))*(newValueB[i] * skala);
          yB = radius + sin(radians((270+i)))*(newValueB[i] * skala);
          ellipse(xB, yB, 10, 10);
        }

        if (oldValueC[i] - newValueC[i] > nilai_motion || newValueC[i] - oldValueC[i] > nilai_motion) {
          xC = radius + cos(radians((i)))*(newValueC[i] * skala);
          yC = radius + sin(radians((i)))*(newValueC[i] * skala);
          ellipse(xA, yB, 10, 10);
        }

        if (oldValueD[i] - newValueA[i] > nilai_motion || newValueA[i] - oldValueA[i] > nilai_motion) {
          xD = radius + cos(radians((90+i)))*(newValueD[i] * skala);
          yD = radius + sin(radians((90+i)))*(newValueD[i] * skala);
          ellipse(xD, yD, 10, 10);
        }
        
      }
  }
  textFont(RadarFont);
  for (int i = 0; i <=30; i++){
    noFill();
    strokeWeight(1);
    stroke(0, 255-(6*i), 0);
    ellipse(radius, radius, (20*i), (20*i));
//    fill(0, 150, 0);
    fill(255);
    noStroke();
//    text(Integer.toString((radarDist+10)/skala), 380, (375-radarDist), 50, 50);
    text(Float.toString((radarDist+10)/skala), 380, (383-radarDist), 50, 50);
    text(Float.toString((radarDist+10)/skala), 380, (403+radarDist), 50, 50);
    radarDist+=10;
  }
  radarDist = 0;
   textFont(myFont); 
  
  
    /* draw the grid lines on the radar every 30 degrees and write their values 180, 210, 240 etc.. */
  for (int i = 0; i <= 11; i++) {
    strokeWeight(1);
    stroke(0, 75, 0);
    line(radius, radius, radius + cos(radians(180+(30*i)))*w, radius + sin(radians(180+(30*i)))*w);
    fill(0, 55, 0);
    
    noStroke();
    text(Integer.toString(30*i), radius + cos(radians(180+(30*i)))*w, radius + sin(radians(180+(30*i)))*w, 60,40);
    
  
  
   fill(0, 100, 0);
      textFont(JudulFont); 
  text("PING 2", 660, 200, 250, 50);
  text("PING 3", 660, 600, 250, 50);
  text("PING 4", 50, 600, 250, 50);
  text("PING 1", 50, 200, 250, 50);
     textFont(myFont); 
//  fill(0);
//  rect(70,60,150,100);
  fill(0, 200, 0);
  text("Screen Key:", 30, 50, 150, 50);
  fill(0,50,0);
  rect(30,75,10,10);
  text("First sweep", 45, 70, 150, 50);
  fill(0,110,0);
  rect(30,95,10,10);
  text("Second sweep", 45, 90, 150, 50);
  fill(0,170,0);
  rect(30,115,10,10);
  text("Average", 45, 110, 150, 50);
  noFill();
  stroke(150,0,0);
  strokeWeight(1);
  ellipse(35, 140, 10, 10);
  fill(150,0,0);
  text("Motion", 45, 130, 150, 50);
  println(xString);
}

/* get values from serial port */
void serialEvent (Serial myPort) {
   xString = myPort.readStringUntil(ln);  // read the serial port until a new line
    if (xString != null) {  // if theres data in between the new lines
        xString = trim(xString); // get rid of any whitespace just in case
        
        
        getP = xString.substring(1, xString.indexOf("A")); // get the value of the servo position
        getA = xString.substring(xString.indexOf("A")+1, xString.indexOf("B")); // get the value of the sensor reading
        getB = xString.substring(xString.indexOf("B")+1, xString.indexOf("C")); // get the value of the sensor reading
        getC = xString.substring(xString.indexOf("C")+1, xString.indexOf("D")); // get the value of the sensor reading
        getD = xString.substring(xString.indexOf("D")+1, xString.length()); // get the value of the sensor reading
        

        
        degree = Integer.parseInt(getP); // set the values to variables
        valueA = Integer.parseInt(getA);
        valueB = Integer.parseInt(getB);
        valueC = Integer.parseInt(getC);
        valueD = Integer.parseInt(getD);
        
        oldValueA[degree] = newValueA[degree]; 
        newValueA[degree] = valueA;
        
        oldValueB[degree] = newValueB[degree]; 
        newValueB[degree] = valueB;
        
        oldValueC[degree] = newValueC[degree]; 
        newValueC[degree] = valueC;
        
        oldValueD[degree] = newValueD[degree];
        newValueD[degree] = valueD;
        
        firstRun++;
        cacah++;
        if (cacah == 1)
        {
        waktu_rotasi_mulai = millis();
        }
        if (firstRun > 180) {
          firstRun = 180; // keep the value at 360
        }
        if(cacah == 180)
        {
        waktu_rotasi_akhir = millis();
        }
  }
} 
