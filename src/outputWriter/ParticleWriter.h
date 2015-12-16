/*
 * ParticleWriter.h
 *
 *  Created on: Dec 14, 2015
 *      Author: jana
 */

#ifndef SRC_OUTPUTWRITER_PARTICLEWRITER_H_
#define SRC_OUTPUTWRITER_PARTICLEWRITER_H_

#include "ParticleContainer.h"
#include "Particle.h"

#include <iostream>
#include <fstream>

using namespace std;

class ParticleWriter {
public:
	ParticleWriter();
	virtual ~ParticleWriter();

	void writeParticles(ParticleContainer& particles, char* filename);
};

#endif /* SRC_OUTPUTWRITER_PARTICLEWRITER_H_ */
