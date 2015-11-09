/*
 * ParticleContainerTest.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: nivetha
 */

#include "ParticleContainerTest.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>

using namespace CppUnit;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ParticleContainerTest);


ParticleContainerTest::ParticleContainerTest() {
	// TODO Auto-generated constructor stub

}

ParticleContainerTest::~ParticleContainerTest() {
	// TODO Auto-generated destructor stub
}

void ParticleContainerTest::setUp()   	{
	//    	utils::Vector<double, 3> x1({0,3,4});
	//    	utils::Vector<double, 3> v1({5,1,2});
	    	double x1[3] = {0,3,4};
	    	double v1[3] = {5,1,2};
	    	double m1 = 10;
	    	Particle p1(x1,v1,m1);
	//    	utils::Vector<double, 3> x2({10,9,8});
	//    	utils::Vector<double, 3> v2({7,6,5});
	    	double x2[3] = {0,3,4};
	    	double v2[3] = {5,1,2};

	    	double m2 = 20;
	    	Particle p2(x2,v2,m2);
	    	pc.add(p1);
	    	pc.add(p2);
	   }

void ParticleContainerTest::testConstructor(){
//	pc.add(p1);
//	pc.add(p2);

	ParticleContainer part(pc);

	CPPUNIT_ASSERT(pc.size()==part.size());
	CPPUNIT_ASSERT(pc[1]==part[1]);

}

void ParticleContainerTest::testSize(){
	CPPUNIT_ASSERT(pc.size()==(size_t)2);
};

void ParticleContainerTest::testIndex(){
	CPPUNIT_ASSERT(pc[1]==p2);
}




