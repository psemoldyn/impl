/*
 * ForceCalc.cpp
 *
 *  Created on: Dec 21, 2015
 *      Author: jana
 */

#include "ForceCalcLC.h"

ForceCalcLC::ForceCalcLC(double r_cut, double gravity)
							:r_cut(r_cut),
							 gravity(gravity)

{
}

void ForceCalcLC::calcForce(Particle& p1, Particle& p2){
//	p1.getOldF() = p1.getF();
//	p1.getF() = 0;

	if (!(p1== p2) && !p1.getSkip() && !p2.getSkip()){

		double distance = (p1.getX()-p2.getX()).L2Norm();

		double sigma = (p1.getSigma() + p2.getSigma())/2;
		double epsilon = sqrt(p1.getEpsilon()*p2.getEpsilon());

//		double cutoff = r_cut*sigma;

		double cutoff = r_cut;
		double pow6 = sigma/distance*sigma/distance*sigma/distance;
		pow6 = pow6*pow6;
		double pow12 = pow6*pow6;


/*
		cout << distance << endl;
		cout << p1 << endl;
		cout << p2 << endl;

		cout << distance << endl;
		cout << 24.0*epsilon/(distance*distance) << endl;
		cout << pow6 << endl;
		cout << 2*pow12 << endl;
		cout << (p2.getX()-p1.getX()) << endl;
	*/

		if (distance <= cutoff){
			p1.getF() = p1.getF() + 24.0*epsilon/(distance*distance)*(pow6-2*pow12)*(p2.getX()-p1.getX());
		}

	}

}

void ForceCalcLC::calc(Particle& p){
	p.getF()[1] += p.getM()*gravity;
}
