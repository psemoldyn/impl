/*
 * ParticleContainerLC.h
 *
 *  Created on: Nov 22, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLECONTAINERLC_H_
#define SRC_PARTICLECONTAINERLC_H_

#include "ParticleContainer.h"

#include <iterator>
#include <vector>
#include <list>

class ParticleContainerLC: public ParticleContainer {
private:
	//array is good enough for the grid, as it doesn't change
//	list<Particle>* grid[];
	vector<list<Particle*>*> grid;

	//mesh size
	double r_cut;

	//dimension of the grid
	int dim;

	//number of cells in each direction
	int n_x;
	int n_y;
	int n_z;

	//grid dimension
	int dims[];

	//domain
	int domainSize[3];


public:
	ParticleContainerLC();
	virtual ~ParticleContainerLC();

	ParticleContainerLC(double r_cut, vector<int> domainSize);

	/**
	 * Creates a data structure for the Linked Cell algorithm
	 */
	void init(double r_cut, vector<int> domainSize);

	/**
	 * Adds a particle to the Linked Cell grid
	 */
	void add(Particle& p);

	/**
	 * Returns a list of the cells neighboring the given
	 */
	list<list<Particle*>*> findNeighbors(int cell);

	/**
	 * Returns the grid of cells
	 */
	vector<list<Particle*>*>* getGrid();

	/**
	 * Returns the dimension of the grid
	 */
	int* getGridDim();

	/**
	 * Updates the positions of the particles in the grid
	 */
	void updateGrid();

	/**
	 * Returns a list of the boundary particles
	 */
	list<Particle*> getBoundaryParticles();

	/**
	 * Returns a list of the particles in the ghost layer
	 */
	list<Particle*> getHalo();
};

#endif /* SRC_PARTICLECONTAINERLC_H_ */
