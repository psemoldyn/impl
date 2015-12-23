/*
 * ParticleContainerN.cpp
 *
 *  Created on: Dec 23, 2015
 *      Author: jana
 */

#include "ParticleContainerN.h"

ParticleContainerN::ParticleContainerN() {
	// TODO Auto-generated constructor stub

}

ParticleContainerN::~ParticleContainerN() {
	// TODO Auto-generated destructor stub
}

ParticleContainerN::ParticleContainerN(int n){
	particles = vector<Particle>(n);
}

ParticleContainerN::ParticleContainerN(const ParticleContainerN& pc){
	particles = pc.particles;
}

ParticleContainerN::ParticleContainerN(const vector<Particle> pvector){
	particles = pvector;
}

vector<Particle> ParticleContainerN::getParticles(){
	return particles;
}

void ParticleContainerN::add(Particle& p){
	particles.push_back(p);
}



size_t ParticleContainerN::size(){
	return particles.size();
}

Particle& ParticleContainerN::operator[](size_t i){
	return particles[i];
}

vector<Particle>::iterator ParticleContainerN::begin(){
	return particles.begin();
}

vector<Particle>::iterator ParticleContainerN::end(){
	return particles.end();
}

void ParticleContainerN::iterSingles(Calculator& calc){
	vector<Particle>::iterator iter;

	for (iter = particles.begin(); iter != particles.end(); iter++){
		Particle& p = *iter;
		calc.calc(p);
	}

}

void ParticleContainerN::iterPairs(Calculator& calc){
	for (size_t i=0; i<particles.size();i++){
		Particle& p1 = particles[i];
		for (size_t j = i+1; j<particles.size();j++){
				Particle& p2 = particles[j];
				calc.calcForce(p1, p2);
		}
	}


}

void ParticleContainerN::updateGrid(){

}

