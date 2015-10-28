
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
 * Writes the properties of the particles at a given iteration to a .vtu file
 * which allows visualization in ParaView.
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
//	vector<Particle>::iterator iterator;
//	iterator = particles.begin();

//	while (iterator != particles.end()) {
	for (size_t i=0; i<particles.size();i++){
//		vector<Particle>::iterator innerIterator = particles.begin();

//		Particle& p1 = *iterator;
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

				p1.getF() = p1.getF() + p1.getM()*p2.getM()/denominator*(p2.getX()-p1.getX());
				cout <<  p1 << endl;

			}
//			++innerIterator;
		}
//		++iterator;
	}
}

void calculateX() {
//	vector<Particle>::iterator iterator = particles.begin();
//	while (iterator != particles.end()) {
	for (size_t i = 0; i < particles.size(); i++){
//		Particle& p = *iterator;
		Particle& p = particles[i];
		// calculate X
		// this function is called before calculateF() (except for initial forces), so it uses f and not old_f (since f hasn"t been updated yet)
		p.getX() = p.getX()+delta_t*p.getV()+pow(delta_t,2)/(2*p.getM())*p.getF();

//		++iterator;
	}
}

void calculateV() {
//	vector<Particle>::iterator iterator = particles.begin();
//	while (iterator != particles.end()) {
	for (size_t i = 0; i < particles.size(); i++){
//		Particle& p = *iterator;
		Particle& p = particles[i];
		// calculate velocity
		p.getV() = p.getV()+delta_t/(2*p.getM())*(p.getOldF()+p.getF());
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

//	vector<Particle>::iterator iterator = particles.begin();
//	while (iterator != particles.end()) {
	for (size_t i = 0; i < particles.size(); i++){
//		Particle& p = *iterator;
		Particle& p = particles[i];
		writer.plotParticle(p);
//		++iterator;
	}

	writer.writeFile(out_name, iteration);

}
