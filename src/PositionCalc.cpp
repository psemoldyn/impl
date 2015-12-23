/*
 * PositionCalc.cpp
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#include "PositionCalc.h"

PositionCalc::PositionCalc(double delta_t):delta_t(delta_t) {
	// TODO Auto-generated constructor stub

}

PositionCalc::~PositionCalc() {
	// TODO Auto-generated destructor stub
}

void PositionCalc::calc(Particle& p){
		if (!p.getSkip()){
			p.getOldX() = p.getX();
			p.getX() = p.getX()+delta_t*p.getV()+pow(delta_t,2)/(2*p.getM())*p.getF();
	}
}
