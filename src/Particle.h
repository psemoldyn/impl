/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "utils/Vector.h"

#include <log4cxx/logger.h>

using namespace log4cxx;

class Particle {

private:

	/** the position of the particle */
	utils::Vector<double, 3> x;

	/** the position of the particle in the previous time step */
	utils::Vector<double, 3> old_x;

	/** the velocity of the particle */
	utils::Vector<double, 3> v;

	/** the force effective on this particle */
	utils::Vector<double, 3> f;

	/** the force which was effective on this particle */
	utils::Vector<double, 3> old_f;

	/** the mass of this particle */
	double m;

	/** type of the particle. Use it for whatever you want (e.g. to seperate
	 * molecules belonging to different bodies, matters, and so on)
	 */
	int type;

	/**
	 * shows whether the particle is a halo particle
	 */
	bool halo;

	/**
	 * whether to skip the particle in calculations
	 */
	//only temporary, think of a better way to remove particles that go oob
	bool skip;

public:

	Particle(int type = 0);

	Particle(const Particle& other);

	Particle(
			// for visualization, we need always 3 coordinates
			// -> in case of 2d, we use only the first and the second
			utils::Vector<double, 3> x_arg,
	        utils::Vector<double, 3> v_arg,
	        double m_arg,
	        int type = 0
	);

	virtual ~Particle();

	utils::Vector<double, 3>& getX();

	utils::Vector<double, 3>& getOldX();

	utils::Vector<double, 3>& getF();

	utils::Vector<double, 3>& getOldF();

	utils::Vector<double, 3>& getV();

	bool& getHalo();

	bool& getSkip();

	double getM();

	int getType();

	bool operator==(Particle& other);

	std::string toString();
};

std::ostream& operator<<(std::ostream& stream, Particle& p);

#endif /* PARTICLE_H_ */
