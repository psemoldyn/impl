/*
 * VelocityCalc.h
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#ifndef SRC_VELOCITYCALC_H_
#define SRC_VELOCITYCALC_H_

#include "Calculator.h"
#include "Particle.h"
#include "MaxwellBoltzmannDistribution.h"

class VelocityCalc: public Calculator {
private:
	double delta_t;
	int dims;
public:
	VelocityCalc(double delta_t, int numDim);
	virtual ~VelocityCalc();

	void calc(Particle& p);
};

#endif /* SRC_VELOCITYCALC_H_ */
