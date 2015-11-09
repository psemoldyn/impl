/*
 * ParticleGeneratorTest.h
 *
 *  Created on: Nov 9, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLEGENERATORTEST_H_
#define SRC_PARTICLEGENERATORTEST_H_

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

#include "ParticleGenerator.h"
#include "utils/Vector.h"

using namespace CppUnit;

class ParticleGeneratorTest : public CppUnit::TestFixture{

	//Declaring the suite, passing the class name to macro
	CPPUNIT_TEST_SUITE(ParticleGeneratorTest);
	//Method is named after the class name, declaration of each test case of the fixture.
    CPPUNIT_TEST(testNumParticles);
	//End of the suite declaration
	CPPUNIT_TEST_SUITE_END();

private:
	ParticleContainer pc;

public:
	ParticleGeneratorTest();
	~ParticleGeneratorTest();

    void setUp();
    void testNumParticles();
};



#endif /* SRC_PARTICLEGENERATORTEST_H_ */
