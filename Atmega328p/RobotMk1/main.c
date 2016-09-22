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
#define COXAMASS 10.
#define FEMURMASS 10.
#define TIBIAMASS 10.
#define COXACOM 10.
#define TIBIACOM 10.
#define FEMURCOM 10.

struct Angle{

  double CoxaAngle;
  double FemurAngle;
  double TibiaAngle;

};

struct Position{

  double XPosition;
  double YPosition;
  double ZPosition;

};

struct Leg{

  uint8_t LegNumber;
  struct Angle DesiredAngle;
  struct Angle PlannedAngle;
  struct Angle ActualAngle;
  struct Position DesiredPosition;
  struct Position PlannedPosition;
  struct Position ActualPosition;
  double OffsetX; //Offset from the center of the robot, which is chosen as (0,0)
  double OffsetY;
  double COMX;
  double COMY;
  uint8_t Quadrant;
  uint16_t DutyCycle;

};


double AlphaCalculate(double, double);
double BetaCalculate(double, double, double);
double GammaCalculate(double, double, double);
uint16_t MotorAngleToDutyCycle(double angle);
void CalculateCOM(double foot[4][3], double COM[2]);

void FootPositionToMotorAngle(double foot[4][3], double angle[4][3]);

int main(void){

  PCA9685Init();


  struct Leg Legs[3];

  double COM[2] = {0,0};

  //Initial Leg Positioning
  for(int i = 0; i < 3; i++){
    Legs[i].ActualPosition.XPosition = 10.;
    Legs[i].ActualPosition.YPosition = 10.;
    Legs[i].ActualPosition.ZPosition = 10.;
  }

  for (int i = 0; i < 3; i++){

    Legs[i].ActualAngle.CoxaAngle = AlphaCalculate(Legs[i].ActualPosition.XPosition, Legs[i].ActualPosition.YPosition);
    Legs[i].ActualAngle.FemurAngle = BetaCalculate(Legs[i].ActualPosition.XPosition, Legs[i].ActualPosition.YPosition, Legs[i].ActualPosition.ZPosition);
    Legs[i].ActualAngle.TibiaAngle = GammaCalculate(Legs[i].ActualPosition.XPosition, Legs[i].ActualPosition.YPosition, Legs[i].ActualPosition.ZPosition);

  }
/*
  FootPositionToMotorAngle(Legs, PlannedMotorAngles);
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){

      DutyCycle[i][j] = MotorAngleToDutyCycle(PlannedMotorAngles[i][j]);
      PCA9685OutputNoPhase(i + j, DutyCycle[i][j]);
      ActualMotorAngles[i][j] = PlannedMotorAngles[i][j];
      ActualFootPosition[i][j] = PlannedFootPosition[i][j];

    }

  }
*/


  //double CenterOfMass = 0;



  while(1){





  } 
}

double AlphaCalculate(double X, double Y){
  return atan2(Y, X);
}

double BetaCalculate(double X, double Y, double Z){

  double HoldL = sqrt( square(Z) + square (sqrt (square (X) + square (Y)) - COXALENGTH));

  return (acos(Z / HoldL) + acos((square(FEMURLENGTH) - square(TIBIALENGTH) + (square(HoldL)))) / (2 * FEMURLENGTH * HoldL)) - 1.5707963268;
}

double GammaCalculate(double X, double Y, double Z){

  double HoldL = sqrt( square(Z) + square (sqrt (square (X) + square (Y)) - COXALENGTH));

  return 3.14159 - (acos( square(TIBIALENGTH) + square(FEMURLENGTH) + square (HoldL)) / (2 * TIBIALENGTH * FEMURLENGTH));
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

    angle[i][0] = AlphaCalculate (foot[i][0], foot[i][1]);
    angle[i][1] = BetaCalculate(foot[i][0], foot[i][1], foot[i][2]);
    angle[i][2] = GammaCalculate(foot[i][0], foot[i][1], foot[i][2]);
  }

}

void CalculateCOM(double angle[4][3], double COM[3]){

  for(int i = 0; i < 2; i ++){
    COM[0] = cos(angle[i][0]) * ((COXACOM * COXAMASS) + FEMURMASS * (COXALENGTH + FEMURCOM * cos (angle[i][1])) + TIBIAMASS * (COXALENGTH + FEMURLENGTH * cos(angle[i][1]) + TIBIACOM * cos (angle[i][2] - angle[i][1])));
    COM[1] = sin(angle[i][0]) * ((COXACOM * COXAMASS) + FEMURMASS * (COXALENGTH + FEMURCOM * cos (angle[i][1])) + TIBIAMASS * (COXALENGTH + FEMURLENGTH * cos(angle[i][1]) + TIBIACOM * cos (angle[i][2] - angle[i][1])));
  }

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
