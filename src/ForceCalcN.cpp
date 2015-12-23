/*
 * ForceCalcN.cpp
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#include "ForceCalcN.h"

ForceCalcN::ForceCalcN(double gravity):gravity(gravity) {

}

ForceCalcN::~ForceCalcN() {
	// TODO Auto-generated destructor stub
}

void ForceCalcN::calcForce(Particle& p1, Particle& p2){
	p1.getOldF() = p1.getF();
	p1.getF() = 0;

	if (!p1.getSkip() && !p2.getSkip() && !(p1==p2)){
		double distance = (p1.getX()-p2.getX()).L2Norm();

		double sigma = (p1.getSigma() + p2.getSigma())/2;
		double epsilon = sqrt(p1.getEpsilon()*p2.getEpsilon());

		p1.getF() = p1.getF() + 24.0*epsilon/(distance*distance)*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(p2.getX()-p1.getX());
		p2.getF() = p2.getF() - 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(p2.getX()-p1.getX());

	}
}

void ForceCalcN::calc(Particle& p){
	p.getF()[1] += p.getM()*gravity;
}
