/*
 * ParticleContainer.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: jana
 */

#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(){
	particles = std::vector<Particle>();
}

ParticleContainer::ParticleContainer(int n){
	particles = std::vector<Particle>(n);
}

ParticleContainer::ParticleContainer(ParticleContainer& pc){
	particles = pc.particles;
}

void ParticleContainer::add(Particle p){
	particles.push_back(p);
}

size_t ParticleContainer::size(){
	return particles.size();
}

Particle& ParticleContainer::operator[](size_t i){
	return particles[i];
}

