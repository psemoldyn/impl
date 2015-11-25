/*
 * ParticleContainer.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: jana
 */

#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(){
	particles = vector<Particle>();
}

ParticleContainer::ParticleContainer(int n){
	particles = vector<Particle>(n);
}

ParticleContainer::ParticleContainer(const ParticleContainer& pc){
	particles = pc.particles;
}

ParticleContainer::ParticleContainer(const vector<Particle> pvector){
	particles = pvector;
}



void ParticleContainer::add(const Particle& p){
	particles.push_back(p);
}



size_t ParticleContainer::size(){
	return particles.size();
}

Particle& ParticleContainer::operator[](size_t i){
	return particles[i];
}

vector<Particle>::iterator ParticleContainer::begin(){
	return particles.begin();
}

vector<Particle>::iterator ParticleContainer::end(){
	return particles.end();
}
