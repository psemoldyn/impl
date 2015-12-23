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
#include "ParticleContainerN.h"
#include "ParticleContainerLC.h"
#include "utils/Vector.h"

using namespace CppUnit;
using namespace std;

class ParticleContainerTest : public CppUnit::TestFixture{

	//Declaring the suite, passing the class name to macro
	CPPUNIT_TEST_SUITE(ParticleContainerTest);
	//Method is named after the class name, declaration of each test case of the fixture.
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testSize);
    CPPUNIT_TEST(testIndex);
	//End of the suite declaration
	CPPUNIT_TEST_SUITE_END();

private:
	ParticleContainerN* pc;
	Particle p1;
	Particle p2;

public:
	ParticleContainerTest();
	virtual ~ParticleContainerTest();

    void setUp();
    void testConstructor();
    void testSize();
    void testIndex();
};




#endif /* PARTICLECONTAINERTEST_H_ */
