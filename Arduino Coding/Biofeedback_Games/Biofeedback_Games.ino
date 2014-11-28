#include <TimerThree.h>

const int led = 13;  // the pin with a LED
const int pFloor = A0; // Input pin for pelvic floor
const int abDom = A3; //Input pin for abdominals

int pFloorMax = 0; //Thresholds set by user
int abDomMin = 0;

int pFCutoff = 0; //Cutoff values set later
int abDomCutOff = 0;

boolean playing = false; //used in loop

volatile int pFFlex = 0;      //Variable for pf sample value
volatile int abDomFlex = 0;   //Variable for abdom sample value

//This section establishes the moving average variables
const int numSamps = 200;
int pFVect [numSamps];      //samples from pelvic foor
int abDomVect [numSamps];   //samples from abdominals
int index = 0;              //index of current sample
int index2 = 1;
int pFTotal = 0;  //running total for pelvic floor
int abDomTotal = 0;         //running total for abdominals
float pFAve = 0;              //average for pelvic floor
float abDomAve = 0;           //average for abdominals


void setup(void)
{
  Timer3.initialize(1000);
  Timer3.attachInterrupt(sample); // blinkLED to run every 0.15 seconds
  Serial.begin(9600);
  
  for (int pFFlex = 0; pFFlex < numSamps; pFFlex++)            //This initializes all array values as zero
  pFVect[pFFlex] = 0;
  for (int abDomFlex = 0; abDomFlex < numSamps; abDomFlex++)   //This initializes all array values as zero
  abDomVect[abDomFlex] = 0;
}

void sample() 
{
  Timer3.stop();
  pFFlex = analogRead(pFloor);                   //Read current pf
  abDomFlex = analogRead(abDom);                 //Read current abdom
  pFTotal = pFTotal - pFVect[index];             //Subtracts last pf reading
  abDomTotal = abDomTotal - abDomVect[index];   // Subtracts last abdom reading
  pFVect[index] = pFFlex;                        //Put pf value in vector
  abDomVect[index] =  abDomFlex;                 //Put abdom value vector
  pFTotal = pFTotal + pFVect[index];             //Add pf samp to total
  abDomTotal = abDomTotal + abDomVect[index];    //Add abdom samp to total
  index = index + 1;                             //Advances to next spot in array
  index2 = index2 + 1;
  
  if (index >= numSamps)    // This resets the index if it's at the end
  index = 0;
  
  if(index2 >= numSamps)    // This resets the index2 if it's at the end
  index2 = 0;
  
  
  
  pFAve = pFTotal / numSamps;          //Calculates pelvic floor average
  abDomAve = abDomTotal / numSamps;    //Calculates abdominal average
  
  
  Timer3.resume();
  Serial.println(pFAve);
  //Serial.println(abDomAve);
}  
  


void loop()
{

}
