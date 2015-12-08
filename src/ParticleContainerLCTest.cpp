/*
 * ParticleContainerLCTest.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: jana
 */

#include "ParticleContainerLCTest.h"

#include "ParticleContainerTest.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>

using namespace CppUnit;

static LoggerPtr logger(Logger::getLogger("global"));

ParticleContainerLCTest::ParticleContainerLCTest() {
	// TODO Auto-generated constructor stub

}

ParticleContainerLCTest::~ParticleContainerLCTest() {
	// TODO Auto-generated destructor stub
}

void ParticleContainerLCTest::setUp(){
	vector<double> domain2 = vector<double> (3);
	vector<double> domain3 = vector<double> (3);
	domain2[0] = 10;
	domain2[1] = 10;
	domain2[2] = 0;

	domain3[0] = 10;
	domain3[1] = 10;
	domain3[2] = 10;

	ParticleContainer2x2 = ParticleContainerLC(4.5, domain2);
	double x[3] = {0,0,0};
	double v[3] = {0,0,0};
	Particle p1(x,v,1);
	double x2[3] = {50,0,0};
	Particle p2(x2,v,0.5);
	double x3[3] = {9,2,0};
	Particle p3(x3,v,0.3);
	double x4[3] = {3,4,0};
	Particle p4(x4,v,1);
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;
	ParticleContainer3x3x3 = ParticleContainerLC(3.0,domain3);
	grid = ParticleContainer2x2.getGrid();

}

void ParticleContainerLCTest::testAdd(){
	LOG4CXX_INFO(logger, "in ta?");

	//add particle at (0,0)
	ParticleContainer2x2.add(p1);
	CPPUNIT_ASSERT((*((*grid)[14])->front()) == p1);
	//add particle out of bounds
	ParticleContainer2x2.add(p2);

	vector<list<Particle*>*>::iterator i;
	list<Particle*>::iterator listi;


	for (i = grid->begin(); i != grid->end(); i++){
		if (*i){
		for (listi = (*i)->begin(); listi != (*i)->end(); listi++){
			CPPUNIT_ASSERT(!(**listi == p2));
		}
		}
	}

	LOG4CXX_INFO(logger, "Passed testAdd!");

}

void ParticleContainerLCTest::testBoundary(){
	//create a (2,2) or (1,1) grid
	list<Particle*> boundary;
	ParticleContainer2x2.add(p3);
	ParticleContainer2x2.add(p4);
	boundary = ParticleContainer2x2.getBoundaryParticles();

	size_t size = 0;

	for (int j = 0; j < grid->size(); j++){
		if ((*grid)[j]){
		size += (*grid)[j]->size();
		}
	}

	LOG4CXX_INFO(logger, boundary.size());
	CPPUNIT_ASSERT(boundary.size() == size);

	LOG4CXX_INFO(logger, "Passed testBoundary!");


}

void ParticleContainerLCTest::testHalo(){
	//check if no particles from grid in halo
	//check if no particles from halo in grid
}

void ParticleContainerLCTest::testNeighbors(){
	//check if number of neighbors correct
	list<list<Particle*>*> neighbors = ParticleContainer2x2.findNeighbors(0);
	list<list<Particle*>*> neighbors2 = ParticleContainer3x3x3.findNeighbors(64);

	CPPUNIT_ASSERT(neighbors.size() == 4);
	CPPUNIT_ASSERT(neighbors2.size() == 27);

	LOG4CXX_INFO(logger, "Passed testNeighbors!");

}

void ParticleContainerLCTest::testUpdateGrid(){
	//move one particle and see if ok
//	((ParticleContainer) ParticleContainer2x2).add(p1);
	double fp[3];
	fp[0] = 0;
	fp[1] = 0;
	fp[2] = 0;
	utils::Vector<double, 3> fp1(fp);
	LOG4CXX_INFO(logger, "before pg");
	ParticleGeneratorCuboid pg(ParticleContainer2x2, fp1, 1, 1, 1, 0, 1, fp1 ,0.1,0);
	LOG4CXX_INFO(logger, "after pg");


	p1.getOldX() = p1.getX();
	p1.getX()[0] = 7;

	vector<double> domain2 = vector<double> (3);
	domain2[0] = 10;
	domain2[1] = 10;
	domain2[2] = 0;
	ParticleContainer2x2.init(4.5,domain2);
	for (int i = 0; i < ParticleContainer2x2.size(); i++){
		ParticleContainer2x2[i].getOldX() = ParticleContainer2x2[i].getX();
		ParticleContainer2x2[i].getX()[0] = 7;

	}
	ParticleContainer2x2.updateGrid();

	bool particleInCell = false;
	list<Particle*>::iterator i;
	for (i = (*grid)[15]->begin(); i != (*grid)[15]->end(); i++){
		if (**i == p1){
			particleInCell = true;
		}
	}


	CPPUNIT_ASSERT(particleInCell == true);

	LOG4CXX_INFO(logger, "Passed testUpdateGrid!");

}

