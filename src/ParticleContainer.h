/*
 * ParticleContainer.h
 *
 *  Created on: Oct 26, 2015
 *      Author: jana
 */

#ifndef PARTICLECONTAINER_H_
#define PARTICLECONTAINER_H_

#include "Particle.h"

#include <vector>
#include <iostream>

using namespace std;

class ParticleContainer {

private:
	vector<Particle> particles;

public:

	/**
	 * Creates an empty container
	 */
	ParticleContainer();

	/**
	 * Creates a container with n elements
	 */
	ParticleContainer(int n);

	/**
	 * Creates a copy of the passed container
	 */
	ParticleContainer(const ParticleContainer& pc);

	/**
	 * Creates a ParticleContainer of the passed list
	 */
	ParticleContainer(const vector<Particle> pvector);

	/**
	 * Adds an element to the container
	 */
	void add(const Particle& p);

	/**
	 * Returns the number of elements in the container
	 */
	size_t size();

	/**
	 * Returns a pointer to the i-th element
	 */
	Particle& operator[](size_t i);

	/**
	 * Returns an iterator to the beginning of the container
	 */
	vector<Particle>::iterator begin();

	/**
	 * Returns an iterator to the next element after the end of the container
	 */
	vector<Particle>::iterator end();

};





#endif /* PARTICLECONTAINER_H_ */
