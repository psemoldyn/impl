/*
 * ParticleGeneratorCuboid.cpp
 *
 *  Created on: Dec 7, 2015
 *      Author: jana
 */

#include "ParticleGenerator.h"
#include "ParticleGeneratorCuboid.h"

#include <sstream>

using namespace std;

static LoggerPtr logger(Logger::getLogger("global"));

ParticleGeneratorCuboid::ParticleGeneratorCuboid() {
	// TODO Auto-generated constructor stub

}

ParticleGeneratorCuboid::~ParticleGeneratorCuboid() {
	// TODO Auto-generated destructor stub
}

ParticleGeneratorCuboid::ParticleGeneratorCuboid(ParticleContainer& particles, char* filename){
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

    	dims = vector< vector<int> >(numCuboids);
    	for (int c = 0; c<numCuboids; c++){
    		dims[c] = vector<int>(2);
    	}


        //dims[numCuboids];
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
          	generateParticles(particles, i);

          	//save dimension of each cuboid
          	dims[i][0]=x*y*z;

          	if (z==1){
          		dims[i][1] = 2;
          	}

          	else{
          		dims[i][1] = 3;
          	}
        }
    }
}


ParticleGeneratorCuboid::ParticleGeneratorCuboid(ParticleContainer& particles, utils::Vector<double, 3> start, int x, int y,
								int z, double h, double m,
								utils::Vector<double, 3> v, double bm, int type):
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
	int d;
	if (z==1){
		d=2;
	}
	else{
		d=3;
	}

/*	dims = vector< vector<int> >();
//	dims[cuboid][0]=x*y*z;
//	dims[cuboid++][1] = d;
//	int toadd[]={x*y*z,d};
	vector<int> toadd;
	toadd[0]=x*y*z;
	toadd[1]=d;
	dims.push_back(toadd);
	*/
	generateParticles(particles, type);
								}

void ParticleGeneratorCuboid::generateParticlesX(ParticleContainer& particles, int n, int type){

	for (int i = 0; i < n; i++){
		utils::Vector<double, 3> cP = currentParticle;
		cP[0] += h*i;
		Particle p(cP,v,mass,type);
		LOG4CXX_INFO(logger, "Generated particle: " + p.toString());
		particles.add(p);
		LOG4CXX_INFO(logger, "Added particle: " + p.toString());

		}
}

void ParticleGeneratorCuboid::generateParticles(ParticleContainer& particles, int type){
	for (int i= 0; i < z; i++){
		for (int j = 0; j < y; j++){
			generateParticlesX(particles, x, type);
			currentParticle[1] += h;
			stringstream ss;
			ss << j+1;
	    	LOG4CXX_INFO(logger, "Generated row: " + ss.str());

		}
		currentParticle[2] += h;
	}
}

