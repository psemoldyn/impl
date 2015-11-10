
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "FileReader.h"
#include "ParticleContainer.h"
#include "ParticleGenerator.h"
#include "ParticleContainerTest.h"
#include "ParticleGeneratorTest.h"

#include <list>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>

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


static LoggerPtr logger(Logger::getLogger("global"));

double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;

ParticleContainer particles;


int main(int argc, char* argsv[]) {
	PropertyConfigurator::configure("log.cfg");
	LOG4CXX_INFO (logger, "Hello from MolSim for PSE!" );

	if (strcmp(argsv[1],"-test")==0){
		if (argc == 2){
			CppUnit::TestRunner runner;
			TestResult result;
			runner.addTest(ParticleContainerTest::suite());
			runner.addTest(ParticleGeneratorTest::suite());
			runner.run(result);
		}

		//to add in future: choose function and unit test to run
		else{
			if (strcmp(argsv[2],"testConstructor")==0)
				CppUnit::TestCaller<ParticleContainerTest> testPC(argsv[2],&ParticleContainerTest::testConstructor);
			if (strcmp(argsv[2], "testSize")==0)
				CppUnit::TestCaller<ParticleContainerTest> testPC(argsv[2],&ParticleContainerTest::testSize);
			if (strcmp(argsv[2], "testIndex")==0)
				CppUnit::TestCaller<ParticleContainerTest> testPC(argsv[2],&ParticleContainerTest::testIndex);
		}

		return 0;
	}

	else if (argc < 5) {
		LOG4CXX_FATAL(logger,"Erroneous program call! Please type the end time and time step, then c for a file "
				"containing cuboids or l for a file containing a list of particles, followed by the filename. ALternatively, "
				"type the parameters of the cuboids you want to create. ");
		return 0;
	}

	//pass "l" for a list of particles, "c" for a cuboid as the second argument
	else if (argc == 5){
		if (*argsv[1] == 'l'){
			FileReader fileReader;
			fileReader.readFile(particles, argsv[2]);
		}
		else if (*argsv[1]=='c'){
			ParticleGenerator pg(particles, argsv[2]);
		}
		else{
			LOG4CXX_FATAL(logger, "Erroneous program call! Please type the end time and time step, then c for a file "
				"containing cuboids or l for a file containing a list of particles, followed by the filename. ALternatively, "
				"type the parameters of the cuboids you want to create. ");
			return 0;
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
		stringstream ss;
		ss << iteration;

		LOG4CXX_INFO(logger, "Iteration " + ss.str() + " finished.");

		current_time += delta_t;
	}

	LOG4CXX_INFO(logger, "Output written. Terminating...");
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

