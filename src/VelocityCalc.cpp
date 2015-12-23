/*
 * VelocityCalc.cpp
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#include "VelocityCalc.h"

VelocityCalc::VelocityCalc(double delta_t, int numDims):delta_t(delta_t),dims(numDims) {
	// TODO Auto-generated constructor stub

}

VelocityCalc::~VelocityCalc() {
	// TODO Auto-generated destructor stub
}

void VelocityCalc::calc(Particle& p){
//	cout << "before " << p << endl;
//	cout << delta_t << endl;
	if (!p.getSkip()){
		// calculate velocity
		p.getV() = p.getV()+delta_t/(2*p.getM())*(p.getOldF()+p.getF());
		//should this be here??
		MaxwellBoltzmannDistribution(p,0.1,dims);
	}

//	cout << "after " << p << endl;

}
