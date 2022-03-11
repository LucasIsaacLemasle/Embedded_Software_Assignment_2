// ==========================================
//  B31DG - Embedded Software - Assignment 2
// Lucas Isaac
// ==========================================

/* include the Ticker library */

#include <Ticker.h>

/* Definition of all the pins */

#define LED 15 // output
#define error_LED 16 // output
#define push_button 23 // input
#define square_reader 35 // input
#define analog_reader 14 // input

/* Initialisation of the variables for all the tasks */

int monitor_task2 = false;
float frecuencyTask3 = 0;
int periodTask3 = 1; // not 0 to avoid a division by zero just in case
float analog_value_task4[4] = {0,0,0,0};
float average_task5 = 0;
int error_code = 0;
float half_max = 3.3/2;

/* One ticker to manage everything */
Ticker cycleTicker;

/* Prototypes of the functions */
void task1();
void task2();
void task3();
void task4();
void task5();
void task6();
void task7();
void task8();
void task9();
void Cycle();

// Period of each task in milliseconds
const int TimeTask1 = 9; // time to wait signal B
const int TimeTask2 = 200;
const int TimeTask3 = 1000;
const int TimeTask4 = 42;
const int TimeTask5 = 42;
const int TimeTask6 = 100;
const int TimeTask7 = 333;
const int TimeTask8 = 333;
const int TimeTask9 = 5000;

int Counter = 0;

int previousTime = micros();
int currentTime = micros();


/* ============================ */
/*    Functions of the tasks    */
/* ============================ */

// --------- TASK 1 --------- // 

void task1(){

  /* 
   *  Just turn on a led during 50 µs 
   *  and turn it off
   */
  digitalWrite(LED, HIGH);
  delayMicroseconds(50);
  digitalWrite(LED, LOW);
}

// --------- TASK 2 --------- //

void task2(){

  /* Read if the button is pressed or not 
   *  and change the variable depending of the 
   *  results
   */
  if (digitalRead(push_button)){
    monitor_task2 = true;
  }
  else{
    monitor_task2 = false;
  }
}

// --------- TASK 3 --------- //

void task3(){

  /* calculate the average of a square wave signal
   */
   
  int square_state = 0;
  int count_time_square = 0;
  
  square_state = digitalRead(square_reader);
  while(digitalRead(square_reader)==square_state){}
  square_state = digitalRead(square_reader);
  while(digitalRead(square_reader)==square_state){
    delayMicroseconds(1);
    count_time_square ++;
  }
  periodTask3 = 2*count_time_square;  
  frecuencyTask3 = (float) (1000000/periodTask3);
}

// --------- TASK 4 --------- //

void task4(){

  /* We read the value of an analog entrance, 
   *  and save it in an array of 4 elements 
   *  to be used in the next task
   */
  
  for(int i=1; i<4; i++){
    analog_value_task4[i] = analog_value_task4[i+1];
  }
  analog_value_task4[4] = analogRead(analog_reader);
}

// --------- TASK 5 --------- //

void task5(){

  /* We calculate the average of the four values 
   *  saved in an array
   */
  
  average_task5 = 0;
  for(int i=1; i<5; i++){
    average_task5 = average_task5 + analog_value_task4[i];
  }
  average_task5 = average_task5 / 4;
}

// --------- TASK 6 --------- //

void task6(){
 
  /* We repeat 1000 time the the command
   *  __asm__ __volatile__ ("nop");
   */
  
  for(int i=0; i<1000; i++){
    __asm__ __volatile__ ("nop");
  }
}

// --------- TASK 7 --------- //

void task7(){
 
  /*If the value of the readed tension
   * is more than 3.3/2, we activate the variable
   * error_code
   */
  
  if(analogRead(analog_reader) > half_max){
    error_code = 1;
  }
  else{
    error_code = 0;
  }

}

// --------- TASK 8 --------- //

void task8(){ 

  /* If there the variable error_code is activated
   *  then we turn on a LED
   */
   
  if(error_code){
    digitalWrite(error_LED,HIGH);
  }
  else{
    digitalWrite(error_LED,LOW);
  }
}

// --------- TASK 9 --------- //

void task9(){ 

  /*  We print all the data we register 
   *  in the previous tasks  
   */   
  
  Serial.print("Pushbutton value : ");
  Serial.println(monitor_task2);
  Serial.print("Frecuency of the square signal = ");
  Serial.println(frecuencyTask3);
  Serial.print("Filtered analogue input value = ");
  Serial.println(average_task5);

}

/* ================================================================== */
/*    Function called every 1 ms by a Ticker to schedule the tasks    */
/* ================================================================== */

void Cycle(){
  Counter++;
  if(Counter % TimeTask1 == 0) task1(); 
  if(Counter % TimeTask2 == 0) task2();
  if(Counter % TimeTask3 == 0) task3();
  if(Counter % TimeTask4 == 0) task4(); 
  if(Counter % TimeTask5 == 0) task5();
  if(Counter % TimeTask6 == 0) task6();
  if(Counter % TimeTask7 == 0) task7();
  if(Counter % TimeTask8 == 0) task8();
  if(Counter % TimeTask9 == 0) task9();
}


/* =================================================== */
/*    Definition of INPUT and OUTPUT and the TICKER    */
/* =================================================== */

void setup() {
  Serial.begin(115200); 
  pinMode(LED,OUTPUT);
  pinMode(push_button,INPUT);
  pinMode(square_reader,INPUT);
  pinMode(analog_reader,INPUT);
  pinMode(error_LED,OUTPUT);
  cycleTicker.attach_ms(1,Cycle);
}

void loop() {}
