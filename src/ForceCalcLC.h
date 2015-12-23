/*
 * ForceCalc.h
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#ifndef SRC_FORCECALCLC_H_
#define SRC_FORCECALCLC_H_

#include "ForceCalc.h"
#include "Particle.h"

class ForceCalcLC: public ForceCalc {

private:
	double r_cut;
	double gravity;

public:


	ForceCalcLC(double r_cut, double gravity);

	void calcForce(Particle& p1, Particle& p2);

	void calc(Particle& p);
};



#endif /* SRC_FORCECALCLC_H_ */
