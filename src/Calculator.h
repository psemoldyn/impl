/*
 * Calculator.h
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#ifndef SRC_CALCULATOR_H_
#define SRC_CALCULATOR_H_

#include "Particle.h"

#include <iostream>

using namespace std;


class Calculator {
public:
	Calculator();
	virtual ~Calculator();

	virtual void calcForce(Particle& p1, Particle& p2);
	virtual void calc(Particle& p);
};

#endif /* SRC_CALCULATOR_H_ */
