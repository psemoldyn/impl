/*
 * ParticleContainerTest.h
 *
 *  Created on: Nov 9, 2015
 *      Author: nivetha
 */

#ifndef PARTICLECONTAINERTEST_H_
#define PARTICLECONTAINERTEST_H_


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>

#include "ParticleContainer.h"

using namespace CppUnit;
using namespace std;

class ParticleContainerTest : public CppUnit::TestFixture{

	//Declaring the suite, passing the class name to macro
	CPPUNIT_TEST_SUITE(ParticleContainerTest);
	//Method is named after the class name, declaration of each test case of the fixture.
    CPPUNIT_TEST(testadd);
	CPPUNIT_TEST(setUP);
	//End of the suite declaration
	CPPUNIT_TEST_SUITE_END();


public:
	ParticleContainerTest();
    virtual ~ParticleContainerTest();

protected:
  void testadd(void);
  void setUP(void);

private:
	ParticleContainer pc;
	Particle p1;
	Particle p2;


void setUP(void)
	{
	double x1[3] = {0,3,4};
	double v1[3] = {5,1,2};
	double m1 = 10;
	p1(x1,v1,m1);
	double x2[3] = {10,9,8};
	double v2[3] = {7,6,5};
	double m2 = 20;
	p2(x2,v2,m2);
	pc.add(Particle(p1));
	pc.add(Particle(p2));
	}

void testadd()
	{
	pc.add(p1);
	pc.add(p2);

	ParticleContainer part(pc);

	CPPUNIT_ASSERT(pc.size()==part.size());
	CPPUNIT_ASSERT(pc.size()==(size_t)2);
	CPPUNIT_ASSERT(pc[1]==part[1]);
	CPPUNIT_ASSERT(pc[1]==p2);

		}


static CppUnit::Test *suite(){

		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("ParticleContainerTest");
		suiteOfTests->addTest(CppUnit::TestCaller *ParticleContainerTest(setUP));
        suiteOfTests->addTest(CppUnit::TestCaller *ParticleContainerTest (testadd));
		return suiteOfTests;
	}


};




#endif /* PARTICLECONTAINERTEST_H_ */
