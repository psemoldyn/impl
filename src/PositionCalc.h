/*
 * PositionCalc.h
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#ifndef SRC_POSITIONCALC_H_
#define SRC_POSITIONCALC_H_

#include "Calculator.h"
#include "Particle.h"

class PositionCalc: public Calculator {
private:
	double delta_t;
public:
	PositionCalc(double delta_t);
	virtual ~PositionCalc();

	void calc(Particle& p);
};

#endif /* SRC_POSITIONCALC_H_ */
