
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "FileReader.h"
#include "ParticleContainer.h"
#include "ParticleContainerLC.h"
#include "ParticleGenerator.h"
#include "ParticleGeneratorCuboid.h"
#include "ParticleGeneratorSphere.h"
#include "ParticleContainerTest.h"
#include "ParticleGeneratorTest.h"
#include "ParticleContainerLCTest.h"
#include "MaxwellBoltzmannDistribution.h"
#include "BoundaryConditions.h"
#include "Thermostat.h"
#include "outputWriter/ParticleWriter.h"

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
vector<double> domainSize = vector<double>(3);
//type of boundary, 0 for outflow, 1 for reflecting
int boundaryType;
BoundaryConditions bc = BoundaryConditions();
double initTemp;
double targetTemp;
int timeSteps;
int timeStepsChange;
double tempDiff;
bool brown;
int numDim;
double gravity;
bool eq;
char* writeTo;
ParticleWriter pw = ParticleWriter();
ParticleContainerLC particles;
//ParticleContainer particlesLC;
vector< vector<int> > dims;
//Thermostat thermostat(particles);
Thermostat* thermostat = new Thermostat(particles);

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
			runner.addTest(ParticleGeneratorTest::suite());
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


	else if (argc < 9) {
		LOG4CXX_FATAL(logger,"Erroneous program call! Please type the end time and time step, then c for a file "
				"containing cuboids or l for a file containing a list of particles, followed by the filename. ALternatively, "
				"type the parameters of the cuboids you want to create. ");
		return 0;
	}

	//pass "l" for a list of particles, "c" for a cuboid as the second argument
	else if (argc >= 13 && argc <= 19){

		LOG4CXX_INFO(logger, "Start reading args");


		end_time = atof(argsv[1]);

		delta_t = atof(argsv[2]);



		r_cut = atof(argsv[5]);

		boundaryType = atoi(argsv[6]);

		domainSize[0]=atoi(argsv[7]);

		domainSize[1]=atoi(argsv[8]);

		domainSize[2]=atoi(argsv[9]);

		if (domainSize[2] == 0){
			numDim = 2;
		}
		else{
			numDim = 3;
		}

		gravity = atof(argsv[10]);

		initTemp = atof(argsv[11]);

		timeSteps = atof(argsv[12]);

		if (argc == 16){
			if (*argsv[13] == 't'){
				brown = true;
			}
			else if (*argsv[13] == 'f'){
				brown = false;
			}
			if (argc == strcmp(argsv[14],"-eq")==0){
				eq = true;
				writeTo = argsv[15];
			}
		}

		else if (argc == 19){
			targetTemp = atof(argsv[13]);
			tempDiff = atof(argsv[14]);
			timeStepsChange = atoi(argsv[15]);
			if (*argsv[16] == 't'){
				brown = true;
			}
			else if (*argsv[16] == 'f'){
				brown = false;
			}
			if (argc == strcmp(argsv[17],"-eq")==0){
				eq = true;
				writeTo = argsv[18];
			}
		}

	//	particles = ParticleContainerLC(r_cut, domainSize);

		if (*argsv[3] == 'l'){
			FileReader fileReader;
			fileReader.readFile(particles, argsv[4]);
	/*		dims = vector< vector<int> >(1);
			dims[0] = vector<int>(2);
			dims[0][0] = particles.size();
			dims[0][1] = 3; */
		}
		else if (*argsv[3]=='c'){

			ParticleGeneratorCuboid pg(particles, argsv[4]);
/*
			//HORRIBLE! use a pointer to pg.dims, this is way too inefficient. i couldn't figure it out.
			dims = vector< vector<int> >(pg.dims.size());
			for (int c = 0; c<pg.dims.size();c++){
				dims[c]=vector<int>(2);
				dims[c][0]=pg.dims[c][0];
				dims[c][1]=pg.dims[c][1];
//				LOG4CXX_INFO(logger, pg.dims[c][1]);
			}
*/
		}

		else if (*argsv[3]=='s'){

			ParticleGeneratorSphere pg(particles, argsv[4]);
/*
			//HORRIBLE! use a pointer to pg.dims, this is way too inefficient. i couldn't figure it out.
			dims = vector< vector<int> >(pg.dims.size());
			for (int c = 0; c<pg.dims.size();c++){
				dims[c]=vector<int>(2);
				dims[c][0]=pg.dims[c][0];
				dims[c][1]=pg.dims[c][1];
//				LOG4CXX_INFO(logger, pg.dims[c][1]);

			}*/
		}


		LOG4CXX_INFO(logger, "before thermo");
		if (argc == 12 || argc == 13){
			*thermostat = Thermostat(particles, initTemp, timeSteps, brown, numDim);
			LOG4CXX_INFO(logger, "after thermo");

		}
		else if (argc == 15 || argc == 16){
			*thermostat = Thermostat(particles, initTemp, timeSteps, targetTemp,
					tempDiff,timeStepsChange, brown, numDim);
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



	// the forces are needed to calculate x, but are not given in the input file.
	calculateF();

	double current_time = start_time;

	int iteration = 0;

	 // for this loop, we assume: current x, current f and current v are known
	while (current_time < end_time) {
		// calculate new x
		calculateX();

		if (boundaryType == 0){
			bc.applyOutflowBoundary(particles);
		}
		else if (boundaryType == 1){
			bc.applyReflectingBoundary(particles, 5, 1);
		}
		else if (boundaryType == 2){
			bc.applyPeriodicBoundary(particles);
		}
	//	LOG4CXX_INFO(logger, "boundaries");
		particles.updateGrid();
	//	LOG4CXX_INFO(logger, "updated grid");
		// calculate new f
		calculateF();
//		LOG4CXX_INFO(logger, "force");
		// calculate new v
		calculateV();
		LOG4CXX_INFO(logger, "vel");
		if (thermostat != NULL && iteration != 0){
			thermostat->applyThermostat(iteration);
		}

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

	if (eq){
		pw.writeParticles(particles, writeTo);
	}

	LOG4CXX_INFO(logger, "Output written. Terminating...");
	return 0;
}


void calculateF() {
	list<Particle*>::iterator iter;
	list<Particle*>::iterator innerIter;
	list<int>::iterator neighborsIter;

	double epsilon;
	double sigma;
	double cutoff;

	int length = particles.getGridDim()[0]*
			particles.getGridDim()[1]*
			particles.getGridDim()[2];

	for (int i = 0; i < length; i++){
	//	LOG4CXX_INFO(logger, i);
		if ((*grid)[i] && (*grid)[i]->size() > 0){
		for (iter = (*grid)[i]->begin(); iter != (*grid)[i]->end(); iter++){
			Particle& p1 = **iter;
			p1.getOldF() = p1.getF();
			p1.getF() = 0;
	//		cout << p1 << endl;
			if (!p1.getSkip()){
			list<int> neighbors = particles.findNeighbors(i);
	//		LOG4CXX_INFO(logger, "neighbors " << neighbors.size());
			for (neighborsIter = neighbors.begin(); neighborsIter != neighbors.end(); neighborsIter++){
				if ((*grid)[*neighborsIter] && (*grid)[*neighborsIter]->size() > 0){
				list<Particle*>& neighbor = *(*grid)[*neighborsIter];
	//			LOG4CXX_INFO(logger, "neighbor " << *neighborsIter << " " << (*grid)[*neighborsIter]->size());
				for (innerIter = neighbor.begin(); innerIter != neighbor.end(); innerIter++){
					if (*innerIter){
					Particle& p2 = **innerIter;

	//				LOG4CXX_INFO(logger, "neighbor " << p2.toString());

					if (!(p1== p2) && !p2.getSkip()){

						double distance = (p1.getX()-p2.getX()).L2Norm();
//						LOG4CXX_INFO(logger, "dist " << distance);

						sigma = (p1.getSigma() + p2.getSigma())/2;
						epsilon = sqrt(p1.getEpsilon()*p2.getEpsilon());

						cutoff = r_cut*sigma;

						if (distance <= cutoff){
							p1.getF() = p1.getF() + 24.0*epsilon/(distance*distance)*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(p2.getX()-p1.getX());
						}

					}
				}
				}
				}
			}

			p1.getF()[1] += p1.getM()*gravity;
	//		LOG4CXX_INFO(logger, "Calculated force of " << p1.toString());

		}
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
	for (size_t i = 0; i < particles.size(); i++){
		Particle& p = particles[i];
		if (!p.getSkip()){
		p.getOldX() = p.getX();
		// calculate X
		// this function is called before calculateF() (except for initial forces), so it uses f and not old_f (since f hasn"t been updated yet)
		p.getX() = p.getX()+delta_t*p.getV()+pow(delta_t,2)/(2*p.getM())*p.getF();
	}
	}
}

void calculateV() {
	int cuboid=0;
	for (size_t i = 0; i < particles.size(); i++){
		Particle& p = particles[i];
		if (!p.getSkip()){
		// calculate velocity
		p.getV() = p.getV()+delta_t/(2*p.getM())*(p.getOldF()+p.getF());
		//save dimension of each cuboid somewhere
		// i mod number particles in first cuboid = 0 => first cuboid, read dim
		// i mod -||- = 1 => second cuboid. remove frist for simpler comparison
/*		int dim;
		if (i % dims[cuboid][0] < dims[cuboid][0]){
			dim = dims[cuboid][1];
		}
		else{
			cuboid++;
			dim = dims[cuboid][1];
		}*/
		//should this be here??
		MaxwellBoltzmannDistribution(p,0.1,numDim);
	}
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

