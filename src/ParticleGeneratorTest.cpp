/*
 * ParticleGeneratorTest.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: jana
 */

#include "ParticleGeneratorTest.h"
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
//	pc=new ParticleContainer();
	double fp[3];
	fp[0] = 0;
	fp[1] = 1;
	fp[2] = 0;
	utils::Vector<double, 3> fp1(fp);
	ParticleGenerator pg(pc, fp1, 12, 8, 5, 1, 1, fp1 ,0.1);
}

void ParticleGeneratorTest::testNumParticles(){
	CPPUNIT_ASSERT(pc.size()==(size_t)12*8*5);

}
