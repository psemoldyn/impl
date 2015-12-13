/*
 * Thermostat.h
 *
 *  Created on: Dec 13, 2015
 *      Author: jana
 */

#ifndef SRC_THERMOSTAT_H_
#define SRC_THERMOSTAT_H_

#include <cmath>

#include "ParticleContainer.h"
#include "MaxwellBoltzmannDistribution.h"
#include "Particle.h"

class Thermostat {
private:
	double initialTemp;
	double targetTemp;
	double tempDiff;
	int timeSteps;
	int timeStepsChange;
	double nextTemp;
	bool brown;
	bool constant;
	int numDimensions;
	ParticleContainer& particles;

	static const double k_B = 1.3806488;

	static const double m = 1;

	//sets the next temperature in case of non-constant temperature
	void updateTemp();

	//updates the temperature of the particles
	void changeTempParticles();

	//calculates the kinetic energy of the system
	double calcKineticEnergy();

public:
//	Thermostat();

	Thermostat(ParticleContainer& particles);

	Thermostat(ParticleContainer& particles, double initialTemp, int timeSteps, bool applyBrown, int numDimensions);

	Thermostat(ParticleContainer& particles, double initialTemp, int timeSteps, bool applyBrown, double targetTemp,
			double tempDiff, int timeStepsChange, int numDimensions);

	virtual ~Thermostat();

	void applyThermostat(int iteration);

	void operator=(const Thermostat& other);

};

#endif /* SRC_THERMOSTAT_H_ */
