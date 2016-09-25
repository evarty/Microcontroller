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
#define LEGMASS 10.
#define BODYMASS 10.

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
void CalculateLegCOM(struct Angle, double *COMX, double *COMY);
void FootPositionToMotorAngle(struct Position, struct Angle);
void CalculateCOM(struct Leg Legs[4], double COM[2]);

int main(void){

  PCA9685Init();


  struct Leg Legs[4];

  double COM[2] = {0,0};

  //Initial Leg Positioning
  for(int i = 0; i < 4; i++){
    Legs[i].PlannedPosition.XPosition = 10.;
    Legs[i].PlannedPosition.YPosition = 10.;
    Legs[i].PlannedPosition.ZPosition = 10.;
  }

  for (int i = 0; i < 4; i++){

    Legs[i].PlannedAngle.CoxaAngle = AlphaCalculate(Legs[i].PlannedPosition.XPosition, Legs[i].PlannedPosition.YPosition);
    Legs[i].PlannedAngle.FemurAngle = BetaCalculate(Legs[i].PlannedPosition.XPosition, Legs[i].PlannedPosition.YPosition, Legs[i].PlannedPosition.ZPosition);
    Legs[i].PlannedAngle.TibiaAngle = GammaCalculate(Legs[i].PlannedPosition.XPosition, Legs[i].PlannedPosition.YPosition, Legs[i].PlannedPosition.ZPosition);

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

void FootPositionToMotorAngle(struct Position Foot, struct Angle Angles){


  Angles.CoxaAngle = AlphaCalculate (Foot.XPosition, Foot.YPosition);
  Angles.FemurAngle = BetaCalculate(Foot.XPosition, Foot.YPosition, Foot.ZPosition);
  Angles.TibiaAngle = GammaCalculate(Foot.XPosition, Foot.YPosition, Foot.ZPosition);

}

void CalculateLegCOM(struct Angle Angles, double *COMX, double *COMY){

  *COMX = cos(Angles.CoxaAngle) * ((COXACOM * COXAMASS) + FEMURMASS * (COXALENGTH + FEMURCOM * cos (Angles.FemurAngle)) + TIBIAMASS * (COXALENGTH + FEMURLENGTH * cos(Angles.FemurAngle) + TIBIACOM * cos (Angles.TibiaAngle - Angles.FemurAngle)));
  *COMY = sin(Angles.CoxaAngle) * ((COXACOM * COXAMASS) + FEMURMASS * (COXALENGTH + FEMURCOM * cos (Angles.FemurAngle)) + TIBIAMASS * (COXALENGTH + FEMURLENGTH * cos(Angles.CoxaAngle) + TIBIACOM * cos (Angles.TibiaAngle - Angles.FemurAngle)));

}

void CalculateCOM(struct Leg Legs[4], double COM[2]){

  COM[0] = ((Legs[0].COMX * (-1) + Legs[1].COMX * (-1) + Legs[2].COMX + Legs[3].COMX) * 4 * LEGMASS) / (LEGMASS + BODYMASS);
  COM[1] = ((Legs[0].COMY + Legs[1].COMY * (-1) + Legs[2].COMY * (-1) + Legs[3].COMY) * 4 * LEGMASS) / (LEGMASS + BODYMASS);

}

uint8_t TestCOMInPoly(struct Leg Legs[4], double COM[2]){

  double holdx = COM[0];
  double holdy = COM[1];
  uint8_t count[50];
  for(int i = 0; i < 50; i++){
    count[i] = 0;
  }
  uint8_t inpoly = 0;
  uint8_t isdown[4] = {0,0,0,0};
  double vector[4][2] = {{0,0},{0,0},{0,0},{0,0}};

  for(int i = 0; i < 4; i++){
    if(Legs[i].PlannedPosition.ZPosition == DOWNZ){
      isdown[i] = 1;
    }
  }

  for(int i = 0; i < 4; i++){
    if((isdown[i] == 1) & (isdown[((i+1) % 4)] == 1)){
      vector[i][0] = Legs[((i+1) % 4)].PlannedPosition.XPosition - Legs[i].PlannedPosition.XPosition;
      vector[i][1] = Legs[((i+1) % 4)].PlannedPosition.YPosition - Legs[i].PlannedPosition.XPosition;
    }else if((isdown[i] == 1) & (isdown[((i+2) % 4)] == 1)){
      vector[i][0] = Legs[((i+2) % 4)].PlannedPosition.XPosition - Legs[i].PlannedPosition.XPosition;
      vector[i][1] = Legs[((i+2) % 4)].PlannedPosition.YPosition - Legs[i].PlannedPosition.XPosition;
    }else {
      vector[i][0] = 0;
      vector[i][1] = 0;
    }
  }

  for(int j = 0; j < 4; j++){
    for(int i = 0; i < 50; i++){
      holdx += i;
      for(int k = 0; k < 10; k++){
        if(((Legs[j].PlannedPosition.XPosition + (k * vector[j][0] / 10.) - holdx) < 3) & ((Legs[j].PlannedPosition.YPosition + (k * vector[j][1] / 10. ) - holdy) < 3)){
          count[i] = 1;
        }
      }
    }
  }
  
  for(int i = 0; i < 49; i++){

    if((count[i] == 1) & (count[i+1] != 1)){
      inpoly += 1;
    }
  }

  return (inpoly % 2);

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
