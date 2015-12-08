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

	//domain
	double domainSize[3];

	//minimal cell size
	double r_cut;

	//actual cell sizes
	double r_cut_x;
	double r_cut_y;
	double r_cut_z;

	//dimension of the grid
	int dim;

	//number of cells in each direction
	int n_x;
	int n_y;
	int n_z;


	//moves a particle from one cell to another
	void moveParticle(Particle& p, int oldCell, int newCell);




public:
	ParticleContainerLC();
	virtual ~ParticleContainerLC();

	ParticleContainerLC(double r_cut, vector<double> domainSize);

	/**
	 * Creates a data structure for the Linked Cell algorithm
	 */
	void init(double r_cut, vector<double> domainSize);

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


	//removes a given particle from a cell
	void delFromCell(Particle& p, int cell);

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

	/**
	 * Adds a particle to the ghost layer
	 */
	void addToHalo(Particle& p);

	/**
	 * Deletes a partcile from the halo
	 */
	void removeFromHalo(Particle& p);

	vector<double> getDomain();

	double getCutoff();

	double getCutX();

	double getCutY();

	double getCutZ();

};

#endif /* SRC_PARTICLECONTAINERLC_H_ */
