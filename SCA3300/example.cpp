/**
 * @author Nicolas SALMIN
 * @date 4 Apr 2019
 * @file example.cpp
 * @brief Simple Hello World to use SCA3300 library
 *
 */

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
/* ******** Includes/System ************************************************* */
#include <iostream>
#include <stdio.h>
#include <armadillo>
#include <cmath>

/* ******** Includes/Consts ************************************************* */

/* ******** Includes/Types ************************************************** */

/* ******** Includes/Globals ************************************************ */

/* *********Includes/functions prototypes *********************************** */
/* external */

/* local */
#include "sca3300.h"
#include "sca3300-tools.h"
#include "sca3300def.h"

/*============================================================================*/
/*                                DEFINITIONS                                 */
/*============================================================================*/
/* ******** Definitions/Consts ********************************************** */

/* ******** Definitions/Types *********************************************** */

/* ******** Definitions/Variables ******************************************* */

/* ******** Definitions/Functions ******************************************* */

/*============================================================================*/
/*                                NAMESPACES                                  */
/*============================================================================*/
using namespace std;
using namespace sca3300d01;

extern int crap;

int main(int argc, char** argv)
{
    /* Create sca3300 Object */
    sca3300 sca3300Chip("/dev/spidev1.0", SPI_MODE_0, 50000, 8);

    bool ret = true;

    float temp  = 0.0;
    float Acc_X = 0.0;
    float Acc_Y = 0.0;
    float Acc_Z = 0.0;

    /* Call temperature and acceleration */
  //  for (int i = 0; i < 100000; ++i)
	int i=0;

	/*
	// Initialize the random generator	
	arma::arm_rng::set_seed_random();

	// Create a 4x4 random matrix and print it on the screen
	arma::Mat<double> A = arma::randu(4,4);
	cout << "A:\n" << A << "\n";

	//Multiply A with its transpose
	cout << "A * A.t() = \n";
	cout << A * A.t() << "\n";

	// Access/Modify rows and columns from the array:
	A.row(0) = A.row(1) + A.row(3);
	A.col(3).zeros();
	cout << "add rows 1 and 3, store result in row 0, also fill 4th column with zeros:\n";
	cout << "A:\n" << A << "\n";

	// Create a new diagonal matrix using the main diagonal of A:
	arma::Mat<double>B = arma::diagmat(A);
	cout << "B\n" << B << "\n";
	*/

	while(1)
    {
        /* Ask for acceleration */
         ret &= sca3300Chip.GetAng( ACCEL_X, Acc_X );
        ret &= sca3300Chip.GetAng( ACCEL_Y, Acc_Y );
/*        ret &= sca3300Chip.GetAng( ACCEL_Z, Acc_Z );
*/
        /* Ask for temperature */
/*        ret &= sca3300Chip.GetTemperature( temp );*/
		/*if( !ret)
			break;*/


        /* Print accelerations */
//        printf (" Temperature: %f\n",temp);
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


			//TODO : T2 and T3 values don't match MATLAB output
			printf (" Ang_XY: %7.2f %7.2f %10u %d Hip Torque: %7.2f | Knee Torque: %7.2f \n", Acc_X, Acc_Y, i++, crap, T2, T3);
//        printf (" Ang_Y: %f\n",Acc_Y);
//        printf (" Ang_Z: %f\n",Acc_Z);

//        sleep(1);
    }

    return ret;
}
