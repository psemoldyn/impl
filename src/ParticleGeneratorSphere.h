/*
 * ParticleGeneratorSphere.h
 *
 *  Created on: Dec 7, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLEGENERATORSPHERE_H_
#define SRC_PARTICLEGENERATORSPHERE_H_

#include "ParticleContainer.h"
#include "ParticleContainerLC.h"
#include "utils/Vector.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

class ParticleGeneratorSphere {
private:
	/**
	 * Position of the center
	 */
	utils::Vector<double, 3> firstParticle;

	/**
	 * Radius of the sphere
	 */
	int radius;

	/**
	 * If the object to generate is a sphere or a circle
	 */
	bool w3d;

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

	double sigma;

	double epsilon;

	/**
	 * Generates the particles in one row
	 */
	void generateParticlesX(vector<Particle>& particles, int n,
			utils::Vector<double, 3> currentRow, int type);

	/**
	 * Generates the particles in one row
	 */
	void generateCircle(vector<Particle>& particles, int radius,
			utils::Vector<double, 3> currentZ, int type);


	/**
	 * Generates the particles
	 */
	void generateParticles(vector<Particle>& particles, int type);


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
	ParticleGeneratorSphere(vector<Particle>& partciles, char* filename);

	/**
	 * Uses the input parameters from the command line
	 */
	ParticleGeneratorSphere(vector<Particle>& particles,
			utils::Vector<double, 3> start, int radius, double h,
			double mass, utils::Vector<double, 3> v, bool w3d,
			double sigma, double epsilon, int type);


};

#endif /* SRC_PARTICLEGENERATORSPHERE_H_ */
