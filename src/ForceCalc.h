/*
 * ForceCalc.h
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#ifndef SRC_FORCECALC_H_
#define SRC_FORCECALC_H_

#include "Particle.h"
#include "Calculator.h"


class ForceCalc : public Calculator {
public:
	ForceCalc();
	virtual ~ForceCalc();

	virtual void calcForce(Particle& p1, Particle& p2);

	virtual void calc(Particle& p);
};



#endif /* SRC_FORCECALC_H_ */
