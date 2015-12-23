/*
 * ParseInput.h
 *
 *  Created on: Dec 20, 2015
 *      Author: jana
 */

#ifndef SRC_XMLINPUT_PARSEINPUT_H_
#define SRC_XMLINPUT_PARSEINPUT_H_

#include "Particle.h"
#include "ParticleGeneratorCuboid.h"
#include "ParticleGeneratorSphere.h"
#include "BoundaryConditions.h"
#include "ParticleContainer.h"
#include "ParticleContainerN.h"
#include "ParticleContainerLC.h"
#include "FileReader.h"
#include "outputWriter/ParticleWriter.h"
#include "Thermostat.h"
#include "Calculator.h"
#include "ForceCalc.h"
#include "VelocityCalc.h"
#include "PositionCalc.h"
#include "globals.h"
#include "ForceCalcLC.h"
#include "ForceCalcN.h"
#include "utils/Vector.h"
#include "SimulationInput.h"

#include <vector>
#include <auto_ptr.h>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include "SimulationInput.h"

class ParseInput {
private:
	const char* filename;
	double end_time;
	double delta_t;
	int simType;
	bool varTemp;
	double r_cut;
	int boundaryType;
	double initTemp;
	double targetTemp;
	int timeSteps;
	int timeStepsChange;
	double tempDiff;
	bool brown;
	int numDim;
	double gravity;
	bool w3d;
	bool eq;
	const char* writeTo;
	int writeFreq;
	int writeType;
	double domain[3];

	vector<Particle>* particles;

	void readXML();

public:
	ParseInput();

	ParseInput(const char* filename);

	virtual ~ParseInput();

	void configure();
};

#endif /* SRC_XMLINPUT_PARSEINPUT_H_ */
