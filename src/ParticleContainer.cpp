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

ParticleContainer::~ParticleContainer(){

}

//void ParticleContainer::add(const Particle& p){
//	particles.push_back(p);
//}

vector<Particle> ParticleContainer::getParticles(){

};


size_t ParticleContainer::size(){
//	return particles.size();
}

Particle& ParticleContainer::operator[](size_t i){
//	return particles[i];
}

vector<Particle>::iterator ParticleContainer::begin(){
//	return particles.begin();
}

vector<Particle>::iterator ParticleContainer::end(){
//	return particles.end();
}

void ParticleContainer::iterSingles(Calculator& calc){
/*	vector<Particle>::iterator iter;

	for (iter = particles.begin(); iter != particles.end(); iter++){
		Particle& p = *iter;
		calc.calc(p);
	}*/

}

void ParticleContainer::iterPairs(Calculator& calc){
/*	for (size_t i=0; i<particles.size();i++){
		Particle& p1 = particles[i];
		for (size_t j = i+1; j<particles.size();j++){
				Particle& p2 = particles[j];
				calc.calcForce(p1, p2);
		}
	}*/


}
