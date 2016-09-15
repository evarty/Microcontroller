#include <avr/io.h> //allows more human readable stuff
#include <avr/interrupt.h>  //allows interrupts
#include <avr/power.h> //power reduction management
#include <math.h>
#include "IIC.h"
#include "PCA9685.h"

#define PCA9685ADDRESS 0x80

#define COXALENGTH 10.
#define FEMURLENGTH 10.
#define TIBIALENGTH 10.
#define DOWNZ 10.

double GammaCalculate(double, double);
double AlphaCalculate(double, double, double);
double BetaCalculate(double, double, double);
uint16_t MotorAngleToDutyCycle(double angle);
void CalculateCOM(double foot[4][3], COM[3]);

void FootPositionToMotorAngle(double foot[4][3], double angle[4][3]);

int main(void){

  PCA9685Init();

  double PlannedFootPosition[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double ActualFootPosition[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double DesiredFootPosition[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

  double PlannedMotorAngles[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double ActualMotorAngles[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double DesiredMotorAngles[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

  uint16_t DutyCycle[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double COM[3] = {0,0,0};

  //Initial Leg Positioning
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){
      PlannedFootPosition[i][j] = 10;
    }
  }

  FootPositionToMotorAngle(PlannedFootPosition, PlannedMotorAngles);
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){

      DutyCycle[i][j] = MotorAngleToDutyCycle(PlannedMotorAngles[i][j]);
      PCA9685OutputNoPhase(i + j, DutyCycle[i][j]);
      ActualMotorAngles[i][j] = PlannedMotorAngles[i][j];
      ActualFootPosition[i][j] = PlannedFootPosition[i][j];

    }

  }
  


  //double CenterOfMass = 0;



  while(1){





  } 
}

double GammaCalculate(double X, double Y){
  return atan2(Y, X);
}

double AlphaCalculate(double X, double Y, double Z){
  return acos(Z / (sqrt(square(Z) + square(sqrt(square(X) + square(Y)) - COXALENGTH)))) + acos((square(TIBIALENGTH) - square(FEMURLENGTH) - (square(Z) + square(sqrt(square(X) + square(Y)) - COXALENGTH)))) / (-2 * FEMURLENGTH * sqrt(square(Z) + square(sqrt(square(X) + square(Y)) - square(COXALENGTH))));
}

double BetaCalculate(double X, double Y, double Z){
  return acos((square(Z) + square(sqrt(square(X) + square(Y)) - COXALENGTH) - square(TIBIALENGTH) - square(FEMURLENGTH)) / (-2 * TIBIALENGTH * FEMURLENGTH));
}

/*double CenterOfMassCalculate(){

  }*/

uint16_t MotorAngleToDutyCycle(double angle){

  uint16_t DutyCycle;

  DutyCycle =  angle / 90.;
  DutyCycle *= (0.5);
  DutyCycle += 1.5;
  DutyCycle /= 20.;
  return DutyCycle;

}

void FootPositionToMotorAngle(double foot[4][3], double angle[4][3]){

  for(int i = 0; i < 4; i++){

    angle[i][0] = GammaCalculate (foot[i][0], foot[i][1]);
    angle[i][1] = AlphaCalculate(foot[i][0], foot[i][1], foot[i][2]);
    angle[i][1] = BetaCalculate(foot[i][0], foot[i][1], foot[i][2]);
  }

}

void CalculateCOM(double angle[4][3], double COM[3]){

    COM[0] = 
    COM[1] =
    COM[2] =
    
}
/*
 * Forward direction is +Y. Using standard XY plane looking down.
 *
 * Set legs to initial positions. In this case just on a square.
 * Save this as actual foot position
 * Compute current COM
 * Compute current support polygon
 *
 * Compute desired change of position of the center as a 3-vector. 
 * For the moment assume the distance is only one step cycle total
 * Compute the leg positions needed for the final position. The legs will move from their current position via one airborne spot to this final position. Save as desired leg positions. 
 * Compute the intermediate airborne position for leg 1. Save as planned leg position.
 * 
 * Compute resulting COM. Save as planned COM.
 * Compute resulting support poly. Save as planned support poly.
 * Determine if COM is in support poly by ~5cm
 *  if yes
 *    then move leg 1 to airborne intermediate/planned position
 *    save as actual leg position
 *  if no
 *    move body via body IK away from leg 1, but only in the x direction by ~5cm
 *    then go back to start of this block
 * Move leg 1 to desired position. Save as actual leg position
 *
 * Now leg 1 is in the final position for this step cycle. The other 3 are still where they started.
 *
 * Using current leg positions, compute COM and support poly. Check that COM is in the support poly. It should be since it was in the support poly with only three legs. Save as actual COM and support poly.
 *
 * Using body IK, compute needed leg positions to move body forward by 5cm. Save as planned leg positions.
 * Compute resulting COM and support poly. Save as planned COM and support poly.
 *  if COM is in support poly and leg angles within bounds.
 *    then go back to top of block
 *  if COM not in support poly or leg angles out of bounds.
 *    subtract 5cm from body position using body IK
 * Move legs/body to needed positions. Save as actual position.
 * Subtract the distance the center has moved from the desired change of position
 *
 * If this puts the center at the desired center position, then done.
 * If it puts the center past the desired position, take the difference and move the body back that much.
 * If not at the desired position yet, then move to next leg and repeat. Leg order is 1, 2, 4, 3.
 * */
