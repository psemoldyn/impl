
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "FileReader.h"
#include "ParticleContainer.h"
#include "ParticleGenerator.h"
//#include "ParticleContainerTest.h"

#include <list>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>

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

using namespace std;
using namespace CppUnit;
using namespace log4cxx;
using namespace log4cxx::helpers;

/**** forward declaration of the calculation functions ****/

/**
 * Calculates the force for all particles
 */
void calculateF();

/**
 * Calculates the position for all particles
 */
void calculateX();

/**
 * Calculates the velocity for all particles
 */
void calculateV();

/**
 * Writes the properties of the particles at a given iteration
 */
void plotParticles(int iteration);

/**
 * Writes the coordinates of the particles at a given iteration
 */
void plotParticlesXYZ(int iteration);

/**
 * Tests the functionality of calculateF()
 */
//void testForce();

/**
 * Tests the functionality of ParticleContainer
 */
//void testParticleContainer();

LoggerPtr loggerMain(log4cxx::Logger::getLogger("main"));


double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;

ParticleContainer particles;


int main(int argc, char* argsv[]) {
log4cxx::xml::DOMConfigurator::configure("Log4cxxConfig.xml");

	//	PropertyConfigurator::configure("log.cfg");

//	BasicConfigurator::configure();


	LOG4CXX_INFO (loggerMain, "Hello from MolSim for PSE!" );
/*	if (argc == 2 && argsv[1]=="-test"){
//		CPPUNIT_TEST_SUITE_REGISTRATION(ParticleContainerTest);
		CppUnit::TestSuite suite;
		CppUnit::TestRunner runner;
		CppUnit:: TestResult result;
		runner.addTest(ParticleContainerTest::suite());
		runner.run(result);
	}
	*/
	if (argc < 5) {
		cout << "Erroneous program call! " << endl;
//		cout << "./molsym filename" << endl;
	}
	//pass "l" for a list of particles, "c" for a cuboid as the scond argument
	else if (argc == 5){
		if (*argsv[1] == 'l'){
			FileReader fileReader;
			fileReader.readFile(particles, argsv[2]);
		}
		else if (*argsv[1]=='c'){
			ParticleGenerator pg(particles, argsv[2]);
		}
		else{
			cout << "Erroneous program call! " << endl;
		}
		end_time = atof(argsv[3]);
		delta_t = atof(argsv[4]);
	}
	else {
		end_time = atof(argsv[1]);
		delta_t = atof(argsv[2]);
		utils::Vector<double, 3> posFirstParticle;
		int lengthX;
		int lengthY;
		int lengthZ;
		double distance;
		double mass;
		utils::Vector<double, 3> velocity;
		double bm;

		//11 arguments for each cuboid
		for (int c=0; c<(argc-3)/11; c++){
			for (int i=0; i<3; i++){
				posFirstParticle[i]=atof(argsv[11*c+3+i]);
			}

			lengthX = atoi(argsv[11*c+6]);
			lengthY = atoi(argsv[11*c+7]);
			lengthZ = atoi(argsv[11*c+8]);

			distance = atof(argsv[11*c+9]);
			mass = atof(argsv[13*c+10]);

			for (int i=0; i<3; i++){
				velocity[i] = atof(argsv[11*c+i+11]);
			}

/*			if (argc == 16){
				bm = atof(argsv[15]);
			}
*/
//			else{
				bm = 0.1;
//			}

			ParticleGenerator pg(particles, posFirstParticle,lengthX,lengthY,lengthZ,distance,mass,velocity,bm);
		}
	}


	//differentiate between two types of input files!
//	FileReader fileReader;
//	fileReader.readFile(particles, argsv[1]);
	// the forces are needed to calculate x, but are not given in the input file.
	calculateF();

	double current_time = start_time;

	int iteration = 0;

	 // for this loop, we assume: current x, current f and current v are known
	while (current_time < end_time) {
		// calculate new x
		calculateX();

		// calculate new f
		calculateF();
		// calculate new v
		calculateV();

		iteration++;
		if (iteration % 10 == 0) {
			plotParticles(iteration);
			plotParticlesXYZ(iteration);
		}
		cout << "Iteration " << iteration << " finished." << endl;

		current_time += delta_t;
	}

	cout << "output written. Terminating..." << endl;
	return 0;
}


void calculateF() {
	float epsilon = 5.0;
	float sigma = 1.0;
	//set force for all particles to 0
	for(size_t i=0; i<particles.size(); i++){
		Particle& p1 = particles[i];
		p1.getOldF() = p1.getF();
		p1.getF() = 0;
	}

	//calculate new force
	for (size_t i=0; i<particles.size();i++){
		Particle& p1 = particles[i];
		for (size_t j = i+1; j<particles.size();j++){
				Particle& p2 = particles[j];

				double norm = (p1.getX()-p2.getX()).L2Norm();
				p1.getF() = p1.getF() + 24.0*epsilon/norm*(pow(sigma/norm,6)-2*pow(sigma/norm,12))*(p2.getX()-p1.getX());
				p2.getF() = p2.getF() - 24.0*epsilon/norm*(pow(sigma/norm,6)-2*pow(sigma/norm,12))*(p2.getX()-p1.getX());
		}
	}
}

void calculateX() {
	for (size_t i = 0; i < particles.size(); i++){
		Particle& p = particles[i];
		// calculate X
		// this function is called before calculateF() (except for initial forces), so it uses f and not old_f (since f hasn"t been updated yet)
		p.getX() = p.getX()+delta_t*p.getV()+pow(delta_t,2)/(2*p.getM())*p.getF();
	}
}

void calculateV() {
	for (size_t i = 0; i < particles.size(); i++){
		Particle& p = particles[i];
		// calculate velocity
		p.getV() = p.getV()+delta_t/(2*p.getM())*(p.getOldF()+p.getF());
	}
}


void plotParticlesXYZ(int iteration) {

	string out_name("MD_vtk");

	outputWriter::XYZWriter writer;
	writer.plotParticles(particles, out_name, iteration);
}



void plotParticles(int iteration){

	string out_name("MD_vtk");

	outputWriter::VTKWriter writer;
	writer.initializeOutput(particles.size());

	for (size_t i = 0; i < particles.size(); i++){
		Particle& p = particles[i];
		writer.plotParticle(p);
	}

	writer.writeFile(out_name, iteration);

}

