
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
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX();

/**
 * calculate the position for all particles
 */
void calculateV();

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);

//void plotParticlesXYZ(int iteration);


double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;

ParticleContainer particles;


int main(int argc, char* argsv[]) {

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
//			plotParticlesXYZ(iteration);
		}
		cout << "Iteration " << iteration << " finished." << endl;

		current_time += delta_t;
	}

	cout << "output written. Terminating..." << endl;
	return 0;
}


void calculateF() {
//	list<Particle>::iterator iterator;
//	iterator = particles.begin();

//	while (iterator != particles.end()) {
	for (size_t i=0; i<particles.size();i++){
//		list<Particle>::iterator innerIterator = particles.begin();

//		Particle& p = *iterator;
		Particle& p1 = particles[i];
		p1.getOldF() = p1.getF();
		p1.getF() = 0;

		for (size_t j = 0; j<particles.size();j++){
//		while (innerIterator != particles.end()) {
//			if (innerIterator != iterator) {
			if (i != j){
	//			Particle& p1 = *iterator;
	//			Particle& p2 = *innerIterator;

				Particle& p2 = particles[j];

				double denominator = pow((p2.getX()-p1.getX()).L2Norm(),3);

				p1.getF()[0] += p1.getM()*p2.getM()*(p2.getX()[0]-p1.getX()[0])/denominator;
				p1.getF()[1] += p1.getM()*p2.getM()*(p2.getX()[1]-p1.getX()[1])/denominator;
				p1.getF()[2] += p1.getM()*p2.getM()*(p2.getX()[2]-p1.getX()[2])/denominator;

				cout <<  p1 << endl;

			}
	//		++innerIterator;
		}
	//	++iterator;
	}
}


void calculateX() {
//	list<Particle>::iterator iterator = particles.begin();
//	while (iterator != particles.end()) {
		for (size_t i = 0; i < particles.size(); i++){
//		Particle& p = *iterator;
		Particle& p = particles[i];
		// calculate X
		// this function is called before calculateF() (except for initial forces), so it uses f and not old_f (since f hasn"t been updated yet)
		p.getX()[0] = p.getX()[0]+delta_t*p.getV()[0]+pow(delta_t,2)*p.getF()[0]/(2*p.getM());
		p.getX()[1] = p.getX()[1]+delta_t*p.getV()[1]+pow(delta_t,2)*p.getF()[1]/(2*p.getM());
		p.getX()[2] = p.getX()[2]+delta_t*p.getV()[2]+pow(delta_t,2)*p.getF()[2]/(2*p.getM());

//		++iterator;
	}
}


void calculateV() {
//	list<Particle>::iterator iterator = particles.begin();
//	while (iterator != particles.end()) {
	for (size_t i = 0; i < particles.size(); i++){
//		Particle& p = *iterator;
		Particle& p = particles[i];
		// insert calculation of velocity here!
		p.getV() = p.getV() + delta_t*(p.getOldF()[0]+p.getF()[0])/(2*p.getM());
		p.getV() = p.getV() + delta_t*(p.getOldF()[1]+p.getF()[1])/(2*p.getM());
		p.getV() = p.getV() + delta_t*(p.getOldF()[2]+p.getF()[2])/(2*p.getM());

//		++iterator;
	}
}

/*
void plotParticlesXYZ(int iteration) {

	string out_name("MD_vtk");

	outputWriter::XYZWriter writer;
	writer.plotParticles(particles, out_name, iteration);
}
*/

void plotParticles(int iteration){

	string out_name("MD_vtk");

	outputWriter::VTKWriter writer;
	writer.initializeOutput(particles.size());

//	list<Particle>::iterator iterator = particles.begin();
//	while (iterator != particles.end()) {
	for (size_t i = 0; i < particles.size(); i++){
	//	Particle& p = *iterator;
		Particle& p = particles[i];
		writer.plotParticle(p);
	//	++iterator;
	}

	writer.writeFile(out_name, iteration);

}
