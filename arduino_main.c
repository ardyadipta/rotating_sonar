#define TIMER_US_PER_TICK 4 // 16MHz / 64 cycles per tick
#define TIMER_OVERFLOW_US TIMER_US_PER_TICK * 65536 // timer1 16bit
#define SUHU 28 //mendefinisikan nilai suhu untuk perhitungan kecepatan suara

//#define CM_PER_SEC 34792//pada suhu 27 derajat, rumusnya V = (33145 + 61*Tc)cm/s //34000

////////// variabel untuk servo
#include <Servo.h>            // include standard servo library
Servo leftRightServo;         // buat variabel untuk mendefinisikan sebuah servo
int pos = 0;//servo position

/////////////////////////////
// variabel PING

volatile long ofStart[] = {0,0,0,0};
volatile long timerStart[] = {0,0,0,0};// = [0,0];
volatile long ultrasoundValue[] = {0,0,0,0}; // = [0,0];
int val[] = {0,0,0,0};
boolean done_PING[] = {false, false, false, false};
//variabel untuk memastikan apakah sensor PING sudah selesai bekerja atau belum
const int pingSig[] = {46,47,48,49} ; // pin SIG pada sensor PING)))
long cm[] = {0,0,0,0};// variabel untuk nilai sensor
long offset[] = {8,8,8,7};// offset untuk sensor, berdasarkan percobaan, sensor 4 diberikan offset -1. 
//jarak dari pusat robot ke ujung sensor (jarak 0 sensor) adalah 8 cm, jadi masing-masing sensor diberikan offset 8 cm kecuali sensor 4, karena dia diberikan offset -1 juga.

volatile int timer1_overflow = 0; //variabel pencacah berapa kali timer 1 sudah overflow, diberikan tipe volatile karena data dalam tipe ini disimpan dalam RAM, bukan dalam register, hal ini supaya nilainya tidak terganggu, hanya dapat diakses dan diubah pada kondisi tertentu.

long CM_PER_SEC = 33145 + (61* SUHU); //rumus menentukan kecepatan suara berdasarkan suhu ruangan

ISR(SIG_OVERFLOW1) {

  timer1_overflow += 1;
};


void setup() {

  leftRightServo.attach(5);

  Serial.begin(9600);                // Sets the baud rate to 9600
  TCNT1 = 0; // reset timer 1
  TCCR1A = 0x00;          // COM1A1=0, COM1A0=0 => Disconnect Pin OC1 from Timer/Counter 1 -- PWM11=0,
                          // PWM10=0 => PWM Operation disabled                                                                      
  TCCR1B = 0x03; // clock/64 = increments every 3.2us  - overflows in 209.7mS  
//  TCCR1B = 0x02; // clock/8 = increments every 0.5us  - overflows in 32.768mS  
  TIMSK1 |= _BV(0);   // timer1 overflow interrupt enable TOIE1=1. atmel_doc2545 p. 135      

}

void markTime(int i) {

 cli(); // disable global interrupts
 timerStart[i] = TCNT1;  
 ofStart[i] = timer1_overflow;
 sei(); // enable global interrupts
}

long diffTime(int i) {
  // return diff in microsec between now and the last mark
  long us;
  us = (timer1_overflow - ofStart[i]) * 65536;
  us += (TCNT1 - timerStart[i]);
  us *= TIMER_US_PER_TICK;
  return us;
}

void ultrasound_response(int i) {
/* Listening for echo pulse
 * -------------------------------------------------------------------
 * response should come withing ~.5ms (6") and 10ms(20")
 * handles falling pin interrupt signal
 */
 if (ofStart[i] != -1) {
   // check ofStart and return if a new ping has started.
   ultrasoundValue[i] = diffTime(i);
 }
}

void printValue() {


cm[0] = ((ultrasoundValue[0] * CM_PER_SEC) /2000000) + offset[0];
cm[1] = ((ultrasoundValue[1] * CM_PER_SEC) /2000000) + offset[1];
cm[2] = ((ultrasoundValue[2] * CM_PER_SEC) /2000000) + offset[2];
cm[3] = ((ultrasoundValue[3] * CM_PER_SEC) /2000000) + offset[3];

  Serial.print("P");
  Serial.print(pos);
  Serial.print("A");
  Serial.print(cm[0]);
  Serial.print("B");
  Serial.print(cm[1]);
  Serial.print("C");
  Serial.print(cm[2]);
  Serial.print("D");
  Serial.println(cm[3]);

}

void triggerpulse(){

// inisiasi nilai done_PING 
done_PING[0] = false ;
done_PING[1] = false ;
done_PING[2] = false ;
done_PING[3] = false ;

// reset any previous value 
ultrasoundValue[0] = -1;
ultrasoundValue[1] = -1; 
ultrasoundValue[2] = -1;
ultrasoundValue[3] = -1;

// indicator to INT1 interrupt to ignore this LOW
ofStart[0] = -1;
ofStart[1] = -1;
ofStart[2] = -1;
ofStart[3] = -1;

// semua pinSig dijadikan output untuk trigger
pinMode(pingSig[0],OUTPUT);
pinMode(pingSig[1],OUTPUT);
pinMode(pingSig[2],OUTPUT);
pinMode(pingSig[3],OUTPUT);

// hold off terlebih dahulu
digitalWrite(pingSig[0], LOW);
digitalWrite(pingSig[1], LOW);
digitalWrite(pingSig[2], LOW);
digitalWrite(pingSig[3], LOW);
delayMicroseconds(2);

// trigger dimulai, setelah trigger selesai dilakukan, pinSig kembali menjadi input untuk mendengarkan sinyal echo

//trigger ping 0
digitalWrite(pingSig[0], HIGH);
delayMicroseconds(5);
digitalWrite(pingSig[0], LOW);
pinMode(pingSig[0],INPUT);

//trigger ping 1
digitalWrite(pingSig[1], HIGH);
delayMicroseconds(5);
digitalWrite(pingSig[1], LOW);
pinMode(pingSig[1],INPUT);

//trigger ping 2
digitalWrite(pingSig[2], HIGH);
delayMicroseconds(5);
digitalWrite(pingSig[2], LOW); // Holdoff  
pinMode(pingSig[2],INPUT);

//trigger ping 3 
digitalWrite(pingSig[3], HIGH);
delayMicroseconds(5);
digitalWrite(pingSig[3], LOW); // Holdoff  
pinMode(pingSig[3],INPUT);

} 
// Append signal value to val

void readsignal()
{
	val[0] = digitalRead(pingSig[0]);        
	val[1] = digitalRead(pingSig[1]); 
        val[2] = digitalRead(pingSig[2]); 
        val[3] = digitalRead(pingSig[3]); 

	while(val[0] == LOW || val[1] == LOW || val[2] == LOW || val[3] == LOW ) // ______________________
	{ // Loop until pin reads a high value
	  val[0] = digitalRead(pingSig[0]);
          val[1] = digitalRead(pingSig[1]);
          val[2] = digitalRead(pingSig[2]); 
          val[3] = digitalRead(pingSig[3]);
        }

        

       while (!done_PING[0] || !done_PING[1] || !done_PING[2] || !done_PING[3] )
       {
          val[0] = digitalRead(pingSig[0]);
          val[1] = digitalRead(pingSig[1]);
          val[2] = digitalRead(pingSig[2]); 
          val[3] = digitalRead(pingSig[3]); 

         if (!done_PING[0])
         {
          if(val[0]==HIGH &&  ofStart[0] == -1){
            markTime(0);
          }
          else if(val[0] == LOW && ofStart[0] != -1)
          {
            ultrasound_response(0) ;
	    done_PING[0] = true;
          }
          else {
          val[0] = digitalRead(pingSig[0]);
          }

          }

          //        
        if (!done_PING[1]){
          if(val[1]==HIGH &&  ofStart[1] == -1){
            markTime(1);
          }
          else if(val[1] == LOW && ofStart[1] != -1){
            ultrasound_response(1) ;
	    done_PING[1] = true;
          }
          else {
          val[1] = digitalRead(pingSig[1]);}

        }

                if (!done_PING[2]){
          if(val[2]==HIGH &&  ofStart[2] == -1){
            markTime(2);
          }
          else if(val[2] == LOW && ofStart[2] != -1){
            ultrasound_response(2) ;
	    done_PING[2] = true;
          }
          else {
          val[2] = digitalRead(pingSig[2]);}

        }

                if (!done_PING[3]){
          if(val[3]==HIGH &&  ofStart[3] == -1){
            markTime(3);
          }
          else if(val[3] == LOW && ofStart[3] != -1){
            ultrasound_response(3) ;
	    done_PING[3] = true;
          }
          else {
          val[3] = digitalRead(pingSig[3]);}

        }

        
}// akhir readsignal


void loop()
{

    for (pos = 0; pos<90 ;pos +=1)// pos++)
    {
        sei();
        leftRightServo.write(pos);
        triggerpulse();
        readsignal();
        delayMicroseconds(500);
        printValue();       
        cli();      
        timer1_overflow = 0;// reset timer overflow
        TCNT1 = 0; // reset timer 1
        sei();

    }

        for (pos = 90; pos>0 ;pos -= 1)// pos--)
    {
        sei();
        leftRightServo.write(pos);
        triggerpulse();
        readsignal();
        delayMicroseconds(500);
        printValue();  
        cli();
        timer1_overflow = 0;
        TCNT1 = 0; // reset timer 1 
        sei();
  }


}
