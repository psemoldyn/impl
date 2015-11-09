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
	void setUp(){
		double x1[3] = {0,3,4};
		double v1[3] = {5,1,2};
		double m1 = 10;
		p1(x1,v1,m1);
		double x2[3] = {10,9,8};
		double v2[3] = {7,6,5};
		double m2 = 20;
		p2(x2,v2,m2);
		pa.add(Particle(p1));

		pa.add(Particle(p2));
	}
	void testParticleContainer()
	{
			std::vector<Particle> p = std::vector<Particle>();
			p.push_back(p1);
			p.push_back(p2);

			ParticleContainer part(p);

			CPPUNIT_ASSERT(pa.size()==part.size());
			CPPUNIT_ASSERT(pa.size()==(size_t)2);
			CPPUNIT_ASSERT(pa[1]==part[1]);
			CPPUNIT_ASSERT(pa[1]==p2);

		}

	static CppUnit::Test *suite(){
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("ParticleContainerTest");
		suiteOfTests->addTest(new CppUnit::TestCaller<ParticleContainerTest>("testParticleContainer", &testParticleContainer));

		return suiteOfTests;
	}

};




#endif /* SRC_PARTICLECONTAINERTEST_H_ */
