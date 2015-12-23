/*
 * ParticleContainer.h
 *
 *  Created on: Oct 26, 2015
 *      Author: jana
 */

#ifndef PARTICLECONTAINER_H_
#define PARTICLECONTAINER_H_

#include "Particle.h"
#include "utils/Vector.h"
#include "Calculator.h"

#include <vector>
#include <list>
#include <iostream>

using namespace std;

class ParticleContainer {

protected:
	vector<Particle> particles;

public:

	/**
	 * Creates an empty container
	 */
	virtual ~ParticleContainer(){

	};

	/**
	 * Adds an element to the container
	 */
	virtual void add(Particle& p) = 0;

	/**
	 * Returns the number of elements in the container
	 */
	virtual size_t size() = 0;

	virtual vector<Particle> getParticles() = 0;

	/**
	 * Returns a pointer to the i-th element
	 */
	virtual Particle& operator[](size_t i) = 0;

	/**
	 * Returns an iterator to the beginning of the container
	 */
	virtual vector<Particle>::iterator begin() = 0;

	/**
	 * Returns an iterator to the next element after the end of the container
	 */
	virtual vector<Particle>::iterator end() = 0;

	virtual void iterSingles(Calculator& calc) = 0;

	virtual void iterPairs(Calculator& calc) = 0;

	virtual void updateGrid() = 0;

};





#endif /* PARTICLECONTAINER_H_ */
