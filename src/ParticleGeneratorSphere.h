/*
 * ParticleGeneratorSphere.h
 *
 *  Created on: Dec 7, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLEGENERATORSPHERE_H_
#define SRC_PARTICLEGENERATORSPHERE_H_

#include "ParticleGenerator.h"

#include "ParticleContainer.h"
#include "ParticleContainerLC.h"
#include "utils/Vector.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

class ParticleGeneratorSphere: public ParticleGenerator {
private:
	/**
	 * Position of particle in lower left corner
	 */
	utils::Vector<double, 3> firstParticle;

	/**
	 * Radius of the sphere
	 */
	int x;


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
	 * Position of the first particle in a given row
	 */
	utils::Vector<double, 3> currentParticle;

	/**
	 * Generates the particles in one row
	 */
	void generateParticlesX(ParticleContainer& particles, int n, utils::Vector<double, 3> currentRow, int type);

	/**
	 * Generates the particles in one row
	 */
	void generateCircle(ParticleContainer& particles, int radius, utils::Vector<double, 3> currentZ, int type);


	/**
	 * Generates the particles
	 */
	void generateParticles(ParticleContainer& particles, int type);


public:
	/**
	 * Dimensions of the cuboids
	 */
	vector< vector<int> > dims;

	ParticleGeneratorSphere();
	virtual ~ParticleGeneratorSphere();

	/**
	 * Reads the input parameters from a file
	 */
	ParticleGeneratorSphere(ParticleContainer& partciles, char* filename);

	/**
	 * Uses the input parameters from the command line
	 */
	ParticleGeneratorSphere(ParticleContainer& particles, utils::Vector<double, 3> start, int x, int y, int z, double h, double mass, utils::Vector<double, 3> v, double bm, int type);


};

#endif /* SRC_PARTICLEGENERATORSPHERE_H_ */
