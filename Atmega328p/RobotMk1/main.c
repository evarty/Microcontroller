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

double GammaCalculate(double, double);
double AlphaCalculate(double, double, double);
double BetaCalculate(double, double, double);
double AngleToDutyCycle(double);

int main(void){

  PCA9685Init();

  double PlannedFootPosition[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double ActualFootPosition[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double DesiredFootPosition[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double PlannedMotorAngles[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double ActualMotorAngles[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  double DesiredMotorAngles[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  //double CenterOfMass = 0;
  uint16_t DutyCycle[4][3];

  InitialLegPosistioning();

  while(1){

    for(int i = 0; i < 4; i++){

    MotorAngles[i][0] = GammaCalculate(DesiredFootPosition[i][0], DesiredFootPosition[i][1]);
    MotorAngles[i][1] = AlphaCalculate(DesiredFootPosition[i][0], DesiredFootPosition[i][1], DesiredFootPosition[i][2]);
    MotorAngles[i][1] = BetaCalculate(DesiredFootPosition[i][0], DesiredFootPosition[i][1], DesiredFootPosition[i][2]);
    }

    for(int l = 0; l < 4; l++){
      for(int k = 0; k < 3; k++){
        DutyCycle[l][k] = AngleToDutyCycle(MotorAngles[l][k]);
        PCA9685OutputNoPhase(l + k, DutyCycle[l][k]);
      }
    }

    for(int i = 0; i < 4; i++){
        DesiredFootPosition[i][0] += 1;
    }


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

double AngleToDutyCycle(double Angle){

  double DutyCycle;

    DutyCycle =  Angle / 90;
    DutyCycle *= (0.5);
    DutyCycle += 1.5;
    DutyCycle /= 20;
    return DutyCycle;
    
}

void InitialLegPosistions(){

  

}


/*
 * Forward direction is +Y. Using standard XY plane looking down.
 *
 * Set legs to initial positions
 * Save this as current foot position
 * Compute current COM
 * Compute current support polygon
 *
 * Compute desired change of position of the center as a 3-vector. 
 * For the moment assume the distance is only one step cycle total
 * Compute the leg positions needed for the final position. The legs will move from their current position via one airborne spot to this final position. 
 * Compute the intermediate airborne position for leg 1. 
 * 
 * Compute resulting COM
 * Compute resulting support poly
 * Determine if COM is in support poly by ~5cm
 *  if yes
 *    then move leg 1 to airborne intermediate position
 *    save as current leg position
 *  if no
 *    move body via body IK away from leg 1, but only in the x direction by ~5cm
 *    then go back to start of this block
 * Move leg 1 to desired position. Save as current leg position
 *
 * Now leg 1 is in the final position for this step cycle. The other 3 are still where they started.
 *
 * Using current leg positions, compute COM and support poly. Check that COM is in the support poly. It should be since it was in the support poly with only three legs.
 *
 * Using body IK, compute needed leg positions to move body forward by 5cm. Add this distance to a variable representing how far the center is moving
 * Compute resulting COM and support poly
 *  if COM is in support poly
 *    then go back to top of block
 *  if COM not in support poly
 *    subtract 5cm from body position using body IK
 * Move legs/body to needed positions.
 * Subtract the distance the center has moved from the desired change of position
 *
 * If this puts the center at the desired center position, then done.
 * If it puts the center past the desired position, take the difference and move the body back that much.
 * If not at the desired position yet, then move to next leg and repeat. Leg order is 1, 2, 4, 3.
