/*

 * ParticleContainerTest.h
 *
 *  Created on: Nov 9, 2015
 *      Author: jana
 */

#ifndef SRC_PARTICLECONTAINERTEST_H_
#define SRC_PARTICLECONTAINERTEST_H_

#include "ParticleContainer.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
//#include <cppunit/XmlOutputter.h>
//#include <netinet/in.h>

using namespace CppUnit;

class PartcileContainerTest : public CppUnit::TestFixture{

private:
	ParticleContainer pa;
	Particle p1;
	Particle p2;
public:
	void setUp();
	void testParticleContainer();
	static CppUnit::Test *suite();
};




#endif /* SRC_PARTICLECONTAINERTEST_H_ */
