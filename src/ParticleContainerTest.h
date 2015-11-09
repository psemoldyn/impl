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
#include "utils/Vector.h"

using namespace CppUnit;
using namespace std;

class ParticleContainerTest : public CppUnit::TestFixture{

	//Declaring the suite, passing the class name to macro
	CPPUNIT_TEST_SUITE(ParticleContainerTest);
	//Method is named after the class name, declaration of each test case of the fixture.
    CPPUNIT_TEST(testadd);
	//End of the suite declaration
	CPPUNIT_TEST_SUITE_END();

private:
	ParticleContainer pc;
	Particle p1;
	Particle p2;

public:
	ParticleContainerTest();
    virtual ~ParticleContainerTest();

    void setUp(void)
    	{
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
};

/*
static CppUnit::Test *suite(){

		ParticleContainerTest t;

		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("ParticleContainerTest");

		CppUnit::Test *test2 = new CppUnit::TestCaller<ParticleContainerTest> ("testadd", &ParticleContainerTest::testadd());
        suiteOfTests->addTest(test2);
		return suiteOfTests;
	}


};

*/


#endif /* PARTICLECONTAINERTEST_H_ */
