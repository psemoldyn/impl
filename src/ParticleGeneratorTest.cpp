/*
 * ParticleGeneratorTest.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: jana
 */

#include "ParticleGeneratorTest.h"
#include "ParticleContainerN.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>

CPPUNIT_TEST_SUITE_REGISTRATION(ParticleGeneratorTest);

ParticleGeneratorTest::ParticleGeneratorTest(){
//	pc=new ParticleContainer();
}

ParticleGeneratorTest::~ParticleGeneratorTest(){
	// TODO Auto-generated destructor stub
}

void ParticleGeneratorTest::setUp(){
	*pc = ParticleContainerN();
	double fp[3];
	fp[0] = 0;
	fp[1] = 1;
	fp[2] = 0;
	utils::Vector<double, 3> fp1(fp);
	vector<Particle> pcv;
	ParticleGeneratorCuboid pg(pcv, fp1, 12, 8, 5, 1, 1, fp1 ,1,1,0);
	for (vector<Particle>::iterator it = pcv.begin(); it != pcv.end(); it++){
		pc->add(*it);
	}
}

void ParticleGeneratorTest::testNumParticles(){
	CPPUNIT_ASSERT(pc->size()==(size_t)12*8*5);
}
