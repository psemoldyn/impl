/*
 * ForceCalcN.h
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#ifndef SRC_FORCECALCN_H_
#define SRC_FORCECALCN_H_

#include "ForceCalc.h"
#include "Particle.h"

class ForceCalcN: public ForceCalc {
private:
	double gravity;

public:
	ForceCalcN(double gravity);
	virtual ~ForceCalcN();

	void calcForce(Particle& p1, Particle& p2);

	void calc(Particle& p);
};

#endif /* SRC_FORCECALCN_H_ */
