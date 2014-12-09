/*
		BME 400 - Uroflex System
 		Arduino Code
 		
 		created by: Andrew Vamos, Samual Lines
                            Shawn Patel, Michael Simonson
 		
 
 		Last updated: 12/8/14
 
         Content: This code samples, filters, and outputs two channel data
         at a rate of 1,000 Hz. The filter is currently set as a 200 sample
         moving average. This can easily be adjusted by changing the numSamps
         value. Before uploading this code, it is necessary to install the 
         TimerThree library developed by Paul Stroffegen. This can be found
         at https://github.com/PaulStoffregen/TimerThree . 
 */

#include <TimerThree.h>

const int led = 13;    //The pin with a LED for troubleshooting
const int pFloor = A0; //Input pin for pelvic floor
const int abDom = A3;  //Input pin for abdominals

int pFloorMax = 0; //Thresholds set by user
int abDomMin = 0;  //Thresholds to be set by user

int pFCutoff = 0; //Cutoff values set later
int abDomCutOff = 0;

boolean playing = false; //used in loop

volatile int pFFlex = 0;      //Variable for pf sample value
volatile int abDomFlex = 0;   //Variable for abdom sample value

//This section establishes the moving average variables
const int numSamps = 200;
int pFVect [numSamps];             //samples from pelvic foor
int abDomVect [numSamps];          //samples from abdominals
int index = 0;                     //index of current sample
unsigned long pFTotal = 0;         //running total for pelvic floor
unsigned long abDomTotal = 0;      //running total for abdominals
float pFAve = 0;                   //average for pelvic floor
float abDomAve = 0;                //average for abdominals


void setup(void)
{
  Timer3.initialize(1000);           //Sets timer to overflow every millisecond for 1,000 Hz sampling
  Timer3.attachInterrupt(sample);    //Runs sample function each overflow
  Serial.begin(9600);
  
  for (int pFFlex = 0; pFFlex < numSamps; pFFlex++)    //These initialize all array values to zero
  pFVect[pFFlex] = 0;
  for (int abDomFlex = 0; abDomFlex < numSamps; abDomFlex++)   
  abDomVect[abDomFlex] = 0;
  
  
  
}

void sample() 
{
  Timer3.stop();
  pFFlex = analogRead(pFloor);                   //Read current pf
  abDomFlex = analogRead(abDom);                 //Read current abdom
  pFTotal = pFTotal - pFVect[index];             //Subtracts last pf reading
  abDomTotal = abDomTotal - abDomVect[index];    //Subtracts last abdom reading
  pFVect[index] = pFFlex;                        //Put pf value in vector
  abDomVect[index] =  abDomFlex;                 //Put abdom value vector
  pFTotal = pFTotal + pFVect[index];             //Add pf samp to total
  abDomTotal = abDomTotal + abDomVect[index];    //Add abdom samp to total
  index = index + 1;                             //Advances to next spot in array
  
  if (index >= numSamps)        //This resets the index when it reaches the end
  index = 0;
  
  
  
  pFAve = pFTotal / numSamps;          //Calculates pelvic floor average
  abDomAve = abDomTotal / numSamps;    //Calculates abdominal average
  
  
  Timer3.resume();                     //Resumes interrupt timer
  Serial.print(pFFlex);                //Prints current pelvic sample value
  Serial.print('\t');
  Serial.print(pFTotal);               //Prints current pelvic average value
  Serial.print('\t');
  Serial.print(abDomFlex);             //Prints current abdominal sample value
  Serial.print('\t');
  Serial.print(abDomAve);              //Prints current abdominal average value
  Serial.print('\n');                  //Moves to next line for next iteration
}  
  

void loop()      //Necessary to include in all Arduino scripts
{

}
