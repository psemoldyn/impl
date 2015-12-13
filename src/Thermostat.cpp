/*
 * Thermostat.cpp
 *
 *  Created on: Dec 13, 2015
 *      Author: jana
 */

#include "Thermostat.h"

/*
Thermostat::Thermostat(){
	particles = ParticleContainer();
}
*/
Thermostat::Thermostat(ParticleContainer& particles):particles(particles){

}


Thermostat::Thermostat(ParticleContainer& particles, double initialTemp, int timeSteps,
		bool applyBrown, int numDimensions):
			particles(particles),
			initialTemp(initialTemp),
			nextTemp(initialTemp),
			timeSteps(timeSteps),
			brown(applyBrown),
			numDimensions(numDimensions),
			constant(true),
			tempDiff(0),
			timeStepsChange(0),
			targetTemp(initialTemp)
{
	if (applyBrown){
		double initV = sqrt(k_B * initialTemp / m);

		vector<Particle>::iterator i;

		for (i = particles.begin(); i != particles.end(); i++){
			Particle& p = *i;
			if (!p.getSkip()){
				MaxwellBoltzmannDistribution(p,initV,numDimensions);
			}
		}
	}

	changeTempParticles();
}

Thermostat::Thermostat(ParticleContainer& particles, double initialTemp,
		int timeSteps, bool applyBrown, double targetTemp,
		double tempDiff, int timeStepsChange, int numDimensions):
				particles(particles),
				initialTemp(initialTemp),
				nextTemp(initialTemp),
				timeSteps(timeSteps),
				brown(applyBrown),
				targetTemp(targetTemp),
				tempDiff(tempDiff),
				timeStepsChange(timeStepsChange),
				numDimensions(numDimensions),
				constant(false)
{
	if (applyBrown){
		double initV = sqrt(k_B * initialTemp / m);

		vector<Particle>::iterator i;

		for (i = particles.begin(); i != particles.end(); i++){
			Particle& p = *i;
			if (!p.getSkip()){
				MaxwellBoltzmannDistribution(p,initV,numDimensions);
			}
		}
	}

	if ((targetTemp < initialTemp && tempDiff > 0)
			|| (targetTemp > initialTemp && tempDiff < 0)){
		this->tempDiff = -tempDiff;
	}


	changeTempParticles();
}


Thermostat::~Thermostat() {
	// TODO Auto-generated destructor stub
}

double Thermostat::calcKineticEnergy(){
	double Ekin = 0;

	vector<Particle>::iterator i;

	for (i = particles.begin(); i != particles.end(); i++){
		Particle& p = *i;


		if (!p.getSkip()){
			double v_2 = p.getV().L2Norm()*p.getV().L2Norm();
			Ekin += p.getM()*v_2;
		}
	}

	return Ekin;
}

void Thermostat::updateTemp(){
	if (abs(targetTemp - nextTemp) <= abs(tempDiff)){
		nextTemp = targetTemp;
	}
	else{
		nextTemp += tempDiff;
	}
}

void Thermostat::changeTempParticles(){
	double targetKineticEnergy = numDimensions*particles.size()*nextTemp*k_B/2;
	double currentKineticEnergy = calcKineticEnergy();
	if (currentKineticEnergy > 0){
		double beta = sqrt(targetKineticEnergy/currentKineticEnergy);

		vector<Particle>::iterator i;

		for (i = particles.begin(); i != particles.end(); i++){
			Particle& p  = *i;
			if (!p.getSkip()){
				p.getV() = beta*p.getV();
			}
		}
	}


}

void Thermostat::applyThermostat(int iteration){
	if (!constant && iteration % timeStepsChange == 0){
		updateTemp();
	}
	if(iteration % timeSteps == 0){
		changeTempParticles();
	}
}

void Thermostat::operator=(const Thermostat& other){
	particles = other.particles;
	this->brown=other.brown;
	constant=other.constant;
	initialTemp=other.initialTemp;
	nextTemp = other.nextTemp;
	numDimensions = other.numDimensions;
	targetTemp = other.targetTemp;
	tempDiff = other.tempDiff;
	timeSteps = other.timeSteps;
	timeStepsChange = other.timeStepsChange;
}
