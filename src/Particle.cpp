/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"

#include <sstream>
#include <iostream>

static LoggerPtr logger(Logger::getLogger("global"));


Particle::Particle(int type_arg) {
	type = type_arg;
	LOG4CXX_INFO(logger, "Particle generated!");
	f = 0.0;
	old_f = 0.0;
	old_x = 0;
}

Particle::Particle(const Particle& other) {
	x = other.x;
	old_x = other.old_x;
	v = other.v;
	f = other.f;
	old_f = other.old_f;
	m = other.m;
	type = other.type;
	LOG4CXX_INFO(logger,"Particle generated by copy!");
}

Particle::Particle(	utils::Vector<double, 3> x_arg,
	        utils::Vector<double, 3> v_arg,
	        double m_arg,
	        int type_arg
):x(x_arg),
  v(v_arg),
  m(m_arg),
  type(type_arg)

{
    f = 0.0;
    old_f = 0.0;
    old_x = 0;

//    loggerParticle =
    LOG4CXX_INFO(logger, "Particle generated!");
//    std::cout << "Particle generated!" << std::endl;
}

Particle::~Particle() {
	LOG4CXX_INFO(logger, "Particle destructed!");
}

utils::Vector<double, 3>& Particle::getX() {
	return x;
}

utils::Vector<double, 3>& Particle::getOldX(){
	return old_x;
}

utils::Vector<double, 3>& Particle::getV() {
	return v;
}

utils::Vector<double, 3>& Particle::getF() {
	return f;
}

utils::Vector<double, 3>& Particle::getOldF() {
	return old_f;
}

double Particle::getM() {
	return m;
}

int Particle::getType() {
	return type;
}

std::string Particle::toString() {
	std::stringstream stream;
	stream << "Particle: X:" << x <<  " v: " << v << " f: " << f << " old_f: " << old_f << " type: " << type;
	return stream.str();
}

bool Particle::operator ==(Particle& other) {
	if ( (x == other.x) && (v == other.v) && (f == other.f) &&
			(type == other.type) & (m == other.m) && (old_f == other.old_f)) {
		return true;
	}

	return false;
}

std::ostream& operator<<(std::ostream& stream, Particle& p) {
	stream << p.toString();
	return stream;
}
