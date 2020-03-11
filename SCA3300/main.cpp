/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
/* ******** Includes/System ************************************************* */
#include <iostream>
#include <stdio.h>
#include <armadillo>
#include <cmath>

/* local */
#include "sca3300.h"
#include "sca3300-tools.h"
#include "sca3300def.h"
/*============================================================================*/
/*                                NAMESPACES                                  */
/*============================================================================*/
using namespace std;
using namespace sca3300d01;

extern int error;

int main(int argc, char** argv)
{
    /* Create sca3300 Object */
    sca3300 sca3300Chip("/dev/spidev1.0", SPI_MODE_0, 50000, 8);

    bool ret = true;

    float Acc_X = 0.0;
    float Acc_Y = 0.0;
    float Acc_Z = 0.0;

	 int i=0;

	while(1) 
	{
		ret &= sca3300Chip.GetAng( ACCEL_X, Acc_X ); //Get X-angle
      ret &= sca3300Chip.GetAng( ACCEL_Y, Acc_Y ); //Get Y-angle

		Acc_X -= 88;	//Calibrate it so that x-vertical = 0 degrees

		double l2 = 24; //thigh length (inches)
		double l3 = 24; //calf length (inches)
		double F = 50;  //lbs

		Acc_X = Acc_X * (3.141592 / 180); //Convert from degrees to radians
		Acc_Y = Acc_Y * (3.141592 / 180); //Convert from degrees to radians
			
		//Equations assume angles in radians, length in inches
		double x1 = l2*sin(Acc_X) + l3*sin(Acc_X + Acc_Y);
		double x3 = l3*sin(Acc_X + Acc_Y);

		//Torque is in inch*lbs
		double T2 = -x1*F; //Torque at the hip
		double T3 = -x3*F; //Torque at the knee

		printf (" Ang_XY: %7.2f %7.2f %10u %d Hip Torque: %7.2f | Knee Torque: %7.2f \n", Acc_X, Acc_Y, i++, error, T2, T3);

	}
    return ret;
}
