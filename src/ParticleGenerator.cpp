/*
 * ParticleGenerator.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: jana
 */

#include "ParticleGenerator.h"
#include <sstream>

using namespace std;

static LoggerPtr logger(Logger::getLogger("global"));

ParticleGenerator::ParticleGenerator(ParticleContainer& particles, char* filename){
	std::ifstream inputFile(filename);
	string input;
	int numCuboids;

    if (inputFile.is_open()) {

    	getline(inputFile, input);
    	LOG4CXX_INFO(logger, "Read line: " + input);

    	while (input.size() == 0 || input[0] == '#') {
    		getline(inputFile, input);
    		LOG4CXX_INFO(logger, "Read line: " + input);
    	}

       	istringstream numstream(input);
        numstream >> numCuboids;


        for (int i = 0; i < numCuboids; i++) {
        	getline(inputFile, input);
           	LOG4CXX_INFO(logger, "Read line: " + input);

       		istringstream datastream(input);

       		// read position of lower left particle
       		for (int j=0; j<3; j++){
       			datastream >> firstParticle[j];
       			currentParticle[j] = firstParticle[j];
       		}

       		//read number of particles on x-axis
       		datastream >> x;

       		//read number of particles on y-axis
       		datastream >> y;

       		//read number of particles on z-axis
        	datastream >> z;

          	//read distance
         	datastream >> h;

          	//read mass
          	datastream >> mass;

          	//read velocity
          	for (int j=0; j<3; j++){
          		datastream >> v[j];
        		}

    //      		if (datastream.eof()){
          	bm = 0.1;
    //      		}

     //     		datastream >> bm;

          	//maybe use arrays for the class attributes so all cuboids can be saved?
          	generateParticles(particles);

        	}


    }



}

ParticleGenerator::ParticleGenerator(ParticleContainer& particles, utils::Vector<double, 3> start, int x, int y,
								int z, double h, double m,
								utils::Vector<double, 3> v, double bm):
//										particles(particles),
										firstParticle(start),
										x(x),
										y(y),
										z(z),
										h(h),
										mass(m),
										v(v),
										bm(bm),
										currentParticle(start)
								{
	if (z == 1){
		dim = 2;
	}
	else{
		dim = 3;
	}
	generateParticles(particles);
								}

void ParticleGenerator::generateParticlesX(ParticleContainer& particles, int n){

	for (int i = 0; i < n; i++){
		utils::Vector<double, 3> cP = currentParticle;
		cP[0] += h*i;
		Particle p(cP,v,mass);
		cout << p << endl;
		MaxwellBoltzmannDistribution(p,bm,dim);
		particles.add(p);
	}

}

void ParticleGenerator::generateParticles(ParticleContainer& particles){
	for (int i= 0; i < z; i++){
		for (int j = 0; j < y; j++){
			generateParticlesX(particles, x);
			currentParticle[1] += h;
			stringstream ss;
			ss << j+1;
	    	LOG4CXX_INFO(logger, "Generated row: " + ss.str());

		}
		currentParticle[2] += h;
	}
}
