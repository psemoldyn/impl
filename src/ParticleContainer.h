/*
 * ParticleContainer.h
 *
 *  Created on: Oct 26, 2015
 *      Author: jana
 */

#ifndef PARTICLECONTAINER_H_
#define PARTICLECONTAINER_H_

#include <vector>
#include "Particle.h"

class ParticleContainer {

private:
	std::vector<Particle> particles;

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
	ParticleContainer(ParticleContainer& pc);

	/**
	 * Adds an element to the container
	 */
	void add(Particle p);

	/**
	 * Returns the number of elements in the container
	 */
	size_t size();

	/**
	 * Returns a pointer to the i-th element
	 */
	Particle& operator[](size_t i);
};





#endif /* PARTICLECONTAINER_H_ */
