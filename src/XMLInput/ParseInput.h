/*
 * ParseInput.h
 *
 *  Created on: Dec 20, 2015
 *      Author: jana
 */

#ifndef SRC_XMLINPUT_PARSEINPUT_H_
#define SRC_XMLINPUT_PARSEINPUT_H_

#include "ParticleGeneratorCuboid.h"
#include "ParticleGeneratorSphere.h"
#include "BoundaryConditions.h"
#include "ParticleContainer.h"
#include "ParticleContainerLC.h"
#include "FileReader.h"
#include "Thermostat.h"
#include "SimulationInput.hxx"

#include <vector>
#include <auto_ptr.h>
#include <iostream>
#include <iterator>
#include <list>

class ParseInput {
private:
	char* filename;
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
	char* writeTo;
	int writeFreq;
	int writeType;
	double domain[3];
	ParticleWriter pw = ParticleWriter();
	ParticleContainer particles;

	void readXML();

public:
	ParseInput();

	ParseInput(const char* filename);

	virtual ~ParseInput();

	void configure();
};

#endif /* SRC_XMLINPUT_PARSEINPUT_H_ */
