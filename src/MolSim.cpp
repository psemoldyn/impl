
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "FileReader.h"
#include "ParticleContainer.h"
#include "ParticleGenerator.h"

#include <list>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <math.h>

using namespace std;

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
void testForce();

/**
 * Tests the functionality of ParticleContainer
 */
void testParticleContainer();


double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;

ParticleContainer particles;


int main(int argc, char* argsv[]) {
	cout << "Hello from MolSim for PSE!" << endl;
	if (argc < 4) {
		cout << "Errounous programme call! " << endl;
		cout << "./molsym filename" << endl;
	}
	else if (argc == 4){
		end_time = atof(argsv[2]);
		delta_t = atof(argsv[3]);
	}
	else {
		end_time = atof(argsv[2]);
		delta_t = atof(argsv[3]);
		utils::Vector<double, 3> posFirstParticle;
		int lengthX;
		int lengthY;
		int lengthZ;
		double distance;
		double mass;
		utils::Vector<double, 3> velocity;
		double bm;

		for (int i=0; i<3; i++){
			posFirstParticle[i]=atof(argsv[4+i]);
		}

		lengthX = atoi(argsv[7]);
		lengthY = atoi(argsv[8]);
		lengthZ = atoi(argsv[9]);

		distance = atof(argsv[10]);
		mass = atof(argsv[11]);

		for (int i=0; i<3; i++){
			velocity[i] = atof(argsv[12+i]);
		}

		if (argc == 16){
			bm = atof(argsv[15]);
		}

		else{
			//is 1 a reasonable choice?
			bm = 1;
		}

		ParticleGenerator pg(particles, posFirstParticle,lengthX,lengthY,lengthZ,distance,mass,velocity,bm);
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
	for (size_t i=0; i<particles.size();i++){

		Particle& p1 = particles[i];
		p1.getOldF() = p1.getF();
		p1.getF() = 0;

		for (size_t j = 0; j<particles.size();j++){
			if (i != j){
				Particle& p2 = particles[j];

				double denominator = pow((p2.getX()-p1.getX()).L2Norm(),3);

				p1.getF() = p1.getF() + p1.getM()*p2.getM()/denominator*(p2.getX()-p1.getX());

			}
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

void testParticleContainer(){
	ParticleContainer pa;
	double x1[3] = {0,3,4};
	double v1[3] = {5,1,2};
	double m1 = 10;
	Particle p1(x1,v1,m1);
	double x2[3] = {10,9,8};
	double v2[3] = {7,6,5};
	double m2 = 20;
	Particle p2(x2,v2,m2);
	pa.add(Particle(p1));

	pa.add(Particle(p2));

	std::vector<Particle> p = std::vector<Particle>();
	p.push_back(p1);
	p.push_back(p2);

	ParticleContainer part(p);

	assert(pa.size()==part.size());
	assert(pa.size()==(size_t)2);
	assert(pa[1]==part[1]);
	assert(pa[1]==p2);

}

