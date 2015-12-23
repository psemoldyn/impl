/*
 * ParticleContainerN.h
 *
 *  Created on: Dec 23, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLECONTAINERN_H_
#define SRC_PARTICLECONTAINERN_H_

#include "ParticleContainer.h"

class ParticleContainerN: public ParticleContainer {
public:
	ParticleContainerN();
	virtual ~ParticleContainerN();

	/**
	 * Creates a container with n elements
	 */
	ParticleContainerN(int n);

	/**
	 * Creates a copy of the passed container
	 */
	ParticleContainerN(const ParticleContainerN& pc);

	/**
	 * Creates a ParticleContainer of the passed list
	 */
	ParticleContainerN(const vector<Particle> pvector);


	vector<Particle> getParticles();

	/**
	 * Adds an element to the container
	 */
	void add(Particle& p);

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

	void iterSingles(Calculator& calc);

	void iterPairs(Calculator& calc);

	void updateGrid();


};

#endif /* SRC_PARTICLECONTAINERN_H_ */
