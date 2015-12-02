
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "FileReader.h"
#include "ParticleContainer.h"
#include "ParticleContainerLC.h"
#include "ParticleGenerator.h"
#include "ParticleContainerTest.h"
#include "ParticleGeneratorTest.h"
#include "ParticleContainerLCTest.h"
#include "MaxwellBoltzmannDistribution.h"

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
double r_cut;
vector<int> domainSize = vector<int>(3);

ParticleContainerLC particles;
//ParticleContainer particlesLC;
vector< vector<int> > dims;

vector<list<Particle*>*>* grid;

int main(int argc, char* argsv[]) {
	PropertyConfigurator::configure("log.cfg");
	LOG4CXX_INFO (logger, "Hello from MolSim for PSE!" );

	if (argc < 2){
		LOG4CXX_FATAL(logger,"Erroneous program call! Please type the end time and time step, then c for a file "
				"containing cuboids or l for a file containing a list of particles, followed by the filename. ALternatively, "
				"type the parameters of the cuboids you want to create. ");
		return 0;
	}

	if (strcmp(argsv[1],"-test")==0){
		if (argc == 2){
			CppUnit::TestRunner runner;
			TestResult result;
			runner.addTest(ParticleContainerTest::suite());
//			runner.addTest(ParticleGeneratorTest::suite());
			runner.addTest(ParticleContainerLCTest::suite());
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


	else if (argc < 8) {
		LOG4CXX_FATAL(logger,"Erroneous program call! Please type the end time and time step, then c for a file "
				"containing cuboids or l for a file containing a list of particles, followed by the filename. ALternatively, "
				"type the parameters of the cuboids you want to create. ");
		return 0;
	}

	//pass "l" for a list of particles, "c" for a cuboid as the second argument
	else if (argc == 8 || argc == 9){

		LOG4CXX_INFO(logger, "Start reading args");


		end_time = atof(argsv[1]);

		delta_t = atof(argsv[2]);



		r_cut = atof(argsv[5]);

		domainSize[0]=atoi(argsv[6]);

		domainSize[1]=atoi(argsv[7]);

		domainSize[2] = 0;


		if (argc == 9){
			domainSize[2]=atoi(argsv[8]);
		}

	//	particles = ParticleContainerLC(r_cut, domainSize);

		if (*argsv[3] == 'l'){
			FileReader fileReader;
			fileReader.readFile(particles, argsv[4]);
		}
		else if (*argsv[3]=='c'){

			ParticleGenerator pg(particles, argsv[4]);

			//HORRIBLE! use a pointer to pg.dims, this is way too inefficient. i couldn't figure it out.
			dims = vector< vector<int> >(pg.dims.size());
			for (int c = 0; c<pg.dims.size();c++){
				dims[c]=vector<int>(2);
				dims[c][0]=pg.dims[c][0];
				dims[c][1]=pg.dims[c][1];
//				LOG4CXX_INFO(logger, pg.dims[c][1]);
			}

		}

		else{
			LOG4CXX_FATAL(logger, "Erroneous program call! Please type the end time and time step, then c for a file "
				"containing cuboids or l for a file containing a list of particles, followed by the filename. ALternatively, "
				"type the parameters of the cuboids you want to create. ");
			return 0;
		}

	}

	//set up for use of Linked Cell Method
	particles.init(r_cut, domainSize);
	LOG4CXX_INFO(logger, "LCC created in MolSim");

	grid = particles.getGrid();


	LOG4CXX_INFO(logger, "Particles " << particles.size());


	// the forces are needed to calculate x, but are not given in the input file.
	calculateF();

	double current_time = start_time;

	int iteration = 0;

	 // for this loop, we assume: current x, current f and current v are known
	while (current_time < end_time) {
		// calculate new x
		calculateX();
		particles.updateGrid();

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
	list<Particle*>::iterator iter;
	list<Particle*>::iterator innerIter;
	list<list<Particle*>*>::iterator neighborsIter;

	float epsilon = 5.0;
	float sigma = 1.0;


	LOG4CXX_INFO(logger, "Particles " << particles.size());

	int length = particles.getGridDim()[0]*
			particles.getGridDim()[1]*
			particles.getGridDim()[2];
	LOG4CXX_INFO(logger, "got length");

	for (int i = 0; i < length; i++){
		if ((*grid)[i]){
		for (iter = (*grid)[i]->begin(); iter != (*grid)[i]->end(); iter++){
			Particle& p1 = **iter;
			p1.getOldF() = p1.getF();
			p1.getF() = 0;

			//reflecting boundary conditions
			//right boundary
			if (abs(p1.getX()[0]-domainSize[0])< pow(2,1/6.0)*sigma && abs(p1.getX()[0]-domainSize[0]) !=0){
				double x[3] = {domainSize[0], p1.getX()[1], p1.getX()[2]};
				utils::Vector<double, 3> v = p1.getV();
				Particle counterParticle(x,v,p1.getM());

				double distance = (p1.getX()-counterParticle.getX()).L2Norm();

				p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

			}

			//left boundary
			if (abs(p1.getX()[0])< pow(2,1/6.0)*sigma && abs(p1.getX()[0]) !=0){
				double x[3] = {0, p1.getX()[1], p1.getX()[2]};
				utils::Vector<double, 3> v = p1.getV();
				Particle counterParticle(x,v,p1.getM());

				double distance = (p1.getX()-counterParticle.getX()).L2Norm();

				p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

			}

			//upper boundary
			if (abs(p1.getX()[1]-domainSize[1])< pow(2,1/6.0)*sigma && abs(p1.getX()[1] - domainSize[1]) !=0){
				double x[3] = {p1.getX()[0], domainSize[1], p1.getX()[2]};
				utils::Vector<double, 3> v = p1.getV();
				Particle counterParticle(x,v,p1.getM());

				double distance = (p1.getX()-counterParticle.getX()).L2Norm();

				p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

			}

			//lower boundary
			if (abs(p1.getX()[1])< pow(2,1/6.0)*sigma && abs(p1.getX()[1]) !=0){
				double x[3] = {p1.getX()[0], 0, p1.getX()[2]};
				utils::Vector<double, 3> v = p1.getV();
				Particle counterParticle(x,v,p1.getM());

				double distance = (p1.getX()-counterParticle.getX()).L2Norm();

				p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

			}
			if (domainSize[2] != 0){

			//back boundary
			if (abs(p1.getX()[2]-domainSize[2])< pow(2,1/6.0)*sigma && abs(p1.getX()[2]-domainSize[2]) !=0){
				double x[3] = {p1.getX()[0], p1.getX()[1], domainSize[2]};
				utils::Vector<double, 3> v = p1.getV();
				Particle counterParticle(x,v,p1.getM());

				double distance = (p1.getX()-counterParticle.getX()).L2Norm();

				p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

			}

			//front boundary
			if (abs(p1.getX()[2])< pow(2,1/6.0)*sigma && abs(p1.getX()[2]) !=0){
				double x[3] = {p1.getX()[0], p1.getX()[1], 0};
				utils::Vector<double, 3> v = p1.getV();
				Particle counterParticle(x,v,p1.getM());

				double distance = (p1.getX()-counterParticle.getX()).L2Norm();

				p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

			}
			}

			list<list<Particle*>*> neighbors = particles.findNeighbors(i);
//			LOG4CXX_INFO(logger, "neighbors " << neighbors.size());
			for (neighborsIter = neighbors.begin(); neighborsIter != neighbors.end(); neighborsIter++){
				list<Particle*>& neighbor = **neighborsIter;
//				LOG4CXX_INFO(logger, "neighbor " << neighbor.size());
				for (innerIter = neighbor.begin(); innerIter != neighbor.end(); innerIter++){
					Particle& p2 = **innerIter;

//					LOG4CXX_INFO(logger, "neighbor " << p2.toString());

					if (!(p1== p2)){

						double distance = (p1.getX()-p2.getX()).L2Norm();
//						LOG4CXX_INFO(logger, "dist " << distance);


						if (distance <= r_cut){
							p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(p2.getX()-p1.getX());
						}
					}

				}
			}

			LOG4CXX_INFO(logger, "calculated force" << p1.toString());

		}
		}
	}
}

/*
void calcF(){
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
				p1.getF() = p1.getF() + 24.0*epsilon/(pow(norm,2))*(pow(sigma/norm,6)-2*pow(sigma/norm,12))*(p2.getX()-p1.getX());
				p2.getF() = p2.getF() - 24.0*epsilon/(pow(norm,2))*(pow(sigma/norm,6)-2*pow(sigma/norm,12))*(p2.getX()-p1.getX());
		}
	}

}

*/

void calculateX() {
	LOG4CXX_INFO(logger, "calculated force");

	for (size_t i = 0; i < particles.size(); i++){
		Particle& p = particles[i];
		p.getOldX() = p.getX();
		// calculate X
		// this function is called before calculateF() (except for initial forces), so it uses f and not old_f (since f hasn"t been updated yet)
		p.getX() = p.getX()+delta_t*p.getV()+pow(delta_t,2)/(2*p.getM())*p.getF();
	}
}

void calculateV() {
	int cuboid=0;
	for (size_t i = 0; i < particles.size(); i++){
		Particle& p = particles[i];

		// calculate velocity
		p.getV() = p.getV()+delta_t/(2*p.getM())*(p.getOldF()+p.getF());
		//save dimension of each cuboid somewhere
		// i mod number particles in first cuboid = 0 => first cuboid, read dim
		// i mod -||- = 1 => second cuboid. remove frist for simpler comparison
		int dim;
		if (i % dims[cuboid][0] < dims[cuboid][0]){
			dim = dims[cuboid][1];
		}
		else{
			cuboid++;
			dim = dims[cuboid][1];
		}
		MaxwellBoltzmannDistribution(p,0.1,dim);
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

