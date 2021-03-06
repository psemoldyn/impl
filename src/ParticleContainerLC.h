/*
 * ParticleContainerLC.h
 *
 *  Created on: Nov 22, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLECONTAINERLC_H_
#define SRC_PARTICLECONTAINERLC_H_

#include "ParticleContainer.h"
#include "Calculator.h"

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
	list<int> findNeighbors(int cell);

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
	 * Returns a list of the coundary cells
	 */
	list<int> getBoundaryCells();


	/**
	 * Returns a list of the particles in the ghost layer
	 */
	list<Particle*> getHalo();

	/**
	 * Returns a list of the cells in the ghost layer
	 */
	list<int> getHaloCells();

	/**
	 * Adds a particle to the ghost layer
	 */
	void addToHalo(Particle& p, int cell);

	/**
	 * Deletes a partcile from the halo
	 */
	void removeFromHalo(Particle& p, int cell);

	vector<double> getDomain();

	double getCutoff();

	double getCutX();

	double getCutY();

	double getCutZ();

	void iterPairs(Calculator& calc);

	void iterSingles(Calculator& calc);


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

	vector<Particle> getParticles();


};

#endif /* SRC_PARTICLECONTAINERLC_H_ */
