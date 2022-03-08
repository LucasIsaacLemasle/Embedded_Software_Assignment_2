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
#define analog_reader 34 // input

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
void PeriodSquare();
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

int loopBegin = 0;
int loopEnd = 0;


/* ============================ */
/*    Functions of the tasks    */
/* ============================ */

// --------- TASK 1 --------- // 

void task1(){
  //Serial.println("Task 1");
  digitalWrite(LED, HIGH);
  delayMicroseconds(50);
  digitalWrite(LED, LOW);
}

// --------- TASK 2 --------- //

void task2(){
  //Serial.println("Task 2");
  if (digitalRead(push_button)){
    monitor_task2 = true;
  }
  else{
    monitor_task2 = false;
  }
}

// --------- TASK 3 --------- //

void task3(){
  //Serial.println("Task 3");
  /*
  int high_state = pulseIn(square_reader,HIGH);
  int low_state = pulseIn(square_reader,LOW);
  periodTask3 = (high_state + low_state);
  */
  
  frecuencyTask3 = (float) (1000000/periodTask3);
  Serial.print("Period = ");
  Serial.println(periodTask3);
  Serial.print("Frecuency = ");
  Serial.println(frecuencyTask3);

}

void PeriodSquare(){ 

  // use an interruption to calculate the time when the 
  // square begins to rise and save the previous to obtain the time
  // difference for those two
  
  previousTime = currentTime;
  currentTime = micros();
  Serial.println("The interrupt is activated !!! ");
  periodTask3 = (currentTime - previousTime);
}


// --------- TASK 4 --------- //

void task4(){
  //Serial.println("Task 4");
  //Serial.println(analogRead(analog_reader));
  for(int i=1; i<4; i++){
    analog_value_task4[i] = analog_value_task4[i+1];
  }
  analog_value_task4[4] = analogRead(analog_reader);
  
  Serial.print("Analog value = ");
  Serial.println(analogRead(analog_reader));
  
}

// --------- TASK 5 --------- //

void task5(){
  //Serial.println("Task 5");
  average_task5 = 0;
  for(int i=1; i<5; i++){
    average_task5 = average_task5 + analog_value_task4[i];
  }
  average_task5 = average_task5 / 4;
}

// --------- TASK 6 --------- //

void task6(){
  //Serial.println("Task 6");
  for(int i=0; i<1000; i++){
    __asm__ __volatile__ ("nop");
  }
}

// --------- TASK 7 --------- //

void task7(){
  //Serial.println("Task 7");
  if(analogRead(analog_reader) > half_max){
    error_code = 1;
  }
  else{
    error_code = 0;
  }

}

// --------- TASK 8 --------- //

void task8(){ 
  //Serial.println("Task 8");
  if(error_code){
    digitalWrite(error_LED,HIGH);
  }
  else{
    digitalWrite(error_LED,LOW);
  }
}

// --------- TASK 9 --------- //

void task9(){ 
  /*Serial.println("Task 9");
  
  Serial.print("Pushbutton value : ");
  Serial.println(monitor_task2);
  Serial.print("Frecuency of the square signal = ");
  Serial.println(frecuencyTask3);
  Serial.print("Filtered analogue input value = ");
  Serial.println(average_task5);

  */
}

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


/* ==================================== */
/*    Definition of INPUT and OUTPUT    */
/* ==================================== */

void setup() {
  Serial.begin(115200); 
  pinMode(LED,OUTPUT);
  pinMode(push_button,INPUT);
  pinMode(square_reader,INPUT);
  pinMode(analog_reader,INPUT);
  pinMode(error_LED,OUTPUT);
  cycleTicker.attach_ms(1,Cycle);
  attachInterrupt(digitalPinToInterrupt(square_reader),PeriodSquare,CHANGE);
}

/* ====================================== */
/*    Call and Scheduling of the tasks    */
/* ====================================== */

void loop() {}
