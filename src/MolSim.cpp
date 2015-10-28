
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "FileReader.h"
#include "ParticleContainer.h"

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
	testForce();
	testParticleContainer();

	cout << "Hello from MolSim for PSE!" << endl;
	if (argc != 4) {
		cout << "Errounous programme call! " << endl;
		cout << "./molsym filename" << endl;
	}
	else {
		end_time = atof(argsv[2]);
		delta_t = atof(argsv[3]);
	}


	FileReader fileReader;
	fileReader.readFile(particles, argsv[1]);
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

void testForce(){
	std::vector<Particle> p = std::vector<Particle>();
	double x1[3] = {0,3,4};
	double v1[3] = {5,1,2};
	double m1 = 10;
	Particle p1(x1,v1,m1);
	double x2[3] = {10,9,8};
	double v2[3] = {7,6,5};
	double m2 = 20;
	Particle p2(x2,v2,m2);
	p.push_back(p1);
	p.push_back(p2);
	ParticleContainer particles(p);

	// test if forces have changed and are opposite (F_ij = -F_ji and there are only two particles, so F_i = F_ij)
	calculateF();

	utils::Vector<double, 3> p1f = p1.getF();
	utils::Vector<double, 3> p2f = p2.getF();
	assert(p1f == (-1)*p2f);

	//test if old_f corresponds to the f in previous iteration
	calculateF();

	assert(p1f == p1.getOldF());
	assert(p2f == p2.getOldF());

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

