/*
 * ParticleWriter.cpp
 *
 *  Created on: Dec 14, 2015
 *      Author: jana
 */

#include "ParticleWriter.h"

ParticleWriter::ParticleWriter() {
	// TODO Auto-generated constructor stub

}

ParticleWriter::~ParticleWriter() {
	// TODO Auto-generated destructor stub
}

void ParticleWriter::writeParticles(ParticleContainer& particles, char* filename){
	ofstream file;
	file.open(filename);

	file << particles.size() << endl;

	vector<Particle>::iterator i;

	for (i = particles.begin(); i != particles.end(); i++){
		Particle& p = *i;

		file << p.getX()[0] << " " << p.getX()[1] << " " << p.getX()[2] << " "
				<< p.getOldX()[0] << " " << p.getOldX()[1] << " " << p.getOldX()[2] << " "
				<< p.getV()[0] << " " << p.getV()[1] << " " << p.getV()[2] << " "
				<< p.getF()[0] << " " << p.getF()[1] << " " << p.getF()[2] << " "
				<< p.getOldF()[0] << " " << p.getOldF()[1] << " " << p.getOldF()[2] << " "
				<< p.getM() << p.getType() << " " << p.getHalo() << " "
				<< p.getSkip() << " " << p.getSigma() << " " << p.getEpsilon();
		file << endl;
	}

	file.close();
}
