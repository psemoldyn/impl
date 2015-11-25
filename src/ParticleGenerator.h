/*
 * ParticleGenerator.h
 *
 *  Created on: Nov 3, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLEGENERATOR_H_
#define SRC_PARTICLEGENERATOR_H_

#include "ParticleContainer.h"
#include "ParticleContainerLC.h"
#include "utils/Vector.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace std;

class ParticleGenerator{
private:
	/**
	 * Position of particle in lower left corner
	 */
	utils::Vector<double, 3> firstParticle;

	/**
	 * Number of particles on the x-axis
	 */
	int x;

	/**
	 * Number of particles on the y-axis
	 */
	int y;

	/**
	 * Number of particles on the z-axis
	 */
	int z;

	/**
	 * Distance between the particles
	 */
	double h;

	/**
	 * Mass of the particles
	 */
	double mass;

	/**
	 * Initial velocity of the particles
	 */
	utils::Vector<double, 3> v;

	/**
	 * Mean of velocity of Brownian Motion
	 */
	double bm;

	/**
	 * Position of the first particle in a given row
	 */
	utils::Vector<double, 3> currentParticle;


	/**
	 * Generates particles on the x-axis
	 */
	void generateParticlesX(ParticleContainer& particles, int n, int type);

	/**
	 * Generates the particles
	 */
	void generateParticles(ParticleContainer& particles, int type);

public:
	/**
	 * Dimensions of the cuboids
	 */
	vector< vector<int> > dims;

	/**
	 * Reads the input parameters from a file
	 */
	ParticleGenerator(ParticleContainer& partciles, char* filename);

	/**
	 * Uses the input parameters from the command line
	 */
	ParticleGenerator(ParticleContainer& particles, utils::Vector<double, 3> start, int x, int y, int z, double h, double mass, utils::Vector<double, 3> v, double bm, int type);


};




#endif /* SRC_PARTICLEGENERATOR_H_ */
