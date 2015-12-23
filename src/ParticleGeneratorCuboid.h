/*
 * ParticleGeneratorCuboid.h
 *
 *  Created on: Dec 7, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLEGENERATORCUBOID_H_
#define SRC_PARTICLEGENERATORCUBOID_H_

#include "ParticleContainer.h"
#include "ParticleContainerLC.h"
#include "utils/Vector.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

class ParticleGeneratorCuboid{
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
	 * Lennard-Jones parameter sigma
	 */
	double sigma;

	/**
	 * Lennard-Jones parameter epsilon
	 */
	double epsilon;

	/**
	 * Position of the first particle in a given row
	 */
	utils::Vector<double, 3> currentParticle;

	/**
	 * Generates particles on the x-axis
	 */
	void generateParticlesX(vector<Particle>& particles, int n, int type);


	/**
	 * Generates the particles
	 */
	void generateParticles(vector<Particle>& particles, int type);


public:

	ParticleGeneratorCuboid();
	virtual ~ParticleGeneratorCuboid();

	/**
	 * Reads the input parameters from a file
	 */
	ParticleGeneratorCuboid(vector<Particle>& particles, char* filename);

	/**
	 * Uses the input parameters from the command line
	 */
	ParticleGeneratorCuboid(vector<Particle>& particles,
			utils::Vector<double, 3> start, int x, int y, int z, double h,
			double mass, utils::Vector<double, 3> v, double sigma,
			double epsilon, int type);

};

#endif /* SRC_PARTICLEGENERATORCUBOID_H_ */
