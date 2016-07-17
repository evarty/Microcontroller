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

  double DesiredFootPosition[4][3] = {{2,4,5},{2,8,5},{3,2,1},{5,6,7}};
  double MotorAngles[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  //double CenterOfMass = 0;

  while(1){


    MotorAngles[0][0] = GammaCalculate(DesiredFootPosition[0][0], DesiredFootPosition[0][1]);
    MotorAngles[0][1] = AlphaCalculate(DesiredFootPosition[0][0], DesiredFootPosition[0][1], DesiredFootPosition[0][2]);
    MotorAngles[0][1] = BetaCalculate(DesiredFootPosition[0][0], DesiredFootPosition[0][1], DesiredFootPosition[0][2]);

    for(int i = 0; i < 4; i++){
      for(int j = 0; j < 2; j++){
        DesiredFootPosition[i][j] += 1;
      }
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
