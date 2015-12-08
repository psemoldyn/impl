/*
 * ParticleContainerLCTest.h
 *
 *  Created on: Nov 30, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLECONTAINERLCTEST_H_
#define SRC_PARTICLECONTAINERLCTEST_H_

#include "ParticleContainer.h"
#include "ParticleContainerLC.h"
#include "ParticleGenerator.h"
#include "ParticleGeneratorCuboid.h"
#include "utils/Vector.h"

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

#include <vector>
#include <list>

#include <log4cxx/logstring.h>
#include <log4cxx/logger.h>
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/consoleappender.h"
#include "log4cxx/propertyconfigurator.h"
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/helpers/inputstreamreader.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/helpers/bytebuffer.h>

using namespace CppUnit;
using namespace std;
using namespace log4cxx;

class ParticleContainerLCTest : public CppUnit::TestFixture{

	//Declaring the suite, passing the class name to macro
	CPPUNIT_TEST_SUITE(ParticleContainerLCTest);
	//Method is named after the class name, declaration of each test case of the fixture.
    CPPUNIT_TEST(testAdd);
	CPPUNIT_TEST(testBoundary);
    CPPUNIT_TEST(testHalo);
    CPPUNIT_TEST(testNeighbors);
    CPPUNIT_TEST(testUpdateGrid);
    //End of the suite declaration
	CPPUNIT_TEST_SUITE_END();

private:
	ParticleContainerLC ParticleContainer2x2;
	ParticleContainerLC ParticleContainer4x4;
	ParticleContainerLC ParticleContainer3x3x3;
	Particle p1;
	Particle p2;
	Particle p3;
	Particle p4;
	vector<list<Particle*>*>* grid;

public:
	ParticleContainerLCTest();
	virtual ~ParticleContainerLCTest();

    void setUp();
    void testAdd();
    void testBoundary();
    void testHalo();
    void testNeighbors();
    void testUpdateGrid();

};

#endif /* SRC_PARTICLECONTAINERLCTEST_H_ */
