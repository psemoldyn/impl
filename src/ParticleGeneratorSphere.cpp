/*
 * ParticleGeneratorSphere.cpp
 *
 *  Created on: Dec 7, 2015
 *      Author: jana
 */

#include "ParticleGeneratorSphere.h"


static LoggerPtr logger(Logger::getLogger("global"));

ParticleGeneratorSphere::ParticleGeneratorSphere() {
	// TODO Auto-generated constructor stub

}

ParticleGeneratorSphere::~ParticleGeneratorSphere() {
	// TODO Auto-generated destructor stub
}

ParticleGeneratorSphere::ParticleGeneratorSphere(ParticleContainer& particles, char* filename){
	std::ifstream inputFile(filename);
	string input;
	int numSpheres;

    if (inputFile.is_open()) {

    	getline(inputFile, input);
    	LOG4CXX_INFO(logger, "Read line: " + input);

    	while (input.size() == 0 || input[0] == '#') {
    		getline(inputFile, input);
    		LOG4CXX_INFO(logger, "Read line: " + input);
    	}

       	istringstream numstream(input);
        numstream >> numSpheres;
/*
   	 	dims = vector< vector<int> >(numSpheres);
   	    	for (int c = 0; c<numSpheres; c++){
   	    		dims[c] = vector<int>(2);
   	    	}
*/

        //dims[numCuboids];
        for (int i = 0; i < numSpheres; i++) {
        	getline(inputFile, input);
           	LOG4CXX_INFO(logger, "Read line: " + input);

       		istringstream datastream(input);


       		// read position of lower left particle
       		for (int j=0; j<3; j++){
       			datastream >> firstParticle[j];
       			currentParticle[j] = firstParticle[j];
       		}

          	//read velocity
          	for (int j=0; j<3; j++){
          		datastream >> v[j];
        		}


       		//read number of particles along radius
       		datastream >> x;

          	//read distance
         	datastream >> h;

          	//read mass
          	datastream >> mass;

          	datastream >> sigma;

          	datastream >> epsilon;

          	//maybe use arrays for the class attributes so all cuboids can be saved?
          	generateParticles(particles, i);

          	//save dimension of each cuboid
/*          	dims[i][0]=x*y*z;

          	if (z==1){
          		dims[i][1] = 2;
          	}

          	else{
          		dims[i][1] = 3;
          	}
          	*/
        }
    }
}


void ParticleGeneratorSphere::generateParticles(ParticleContainer& particles, int type){
	generateCircle(particles, x, firstParticle, type);

	utils::Vector<double, 3> currentZFront = currentParticle;
	utils::Vector<double, 3> currentZBack = currentParticle;

	currentZFront[2] = firstParticle[2] - h/2;
	currentZBack[2] = firstParticle[2] - h/2;

	generateCircle(particles, x, currentZFront, type);
	generateCircle(particles, x, currentZBack, type);


	for (int i= 1; i < x-1; i++){
		currentZFront[2] -= h;
		currentZBack[2] += h;

		generateCircle(particles, x-i, currentZFront, type);
		generateCircle(particles, x-i, currentZBack, type);

	}
}


void ParticleGeneratorSphere::generateCircle(ParticleContainer& particles, int radius, utils::Vector<double, 3> currentZ, int type){
	generateParticlesX(particles, 0, firstParticle, type);

	utils::Vector<double, 3> currentRowUp = currentZ;
	utils::Vector<double, 3> currentRowDown = currentZ;

	currentRowUp[1] = firstParticle[1] + h/2;
	currentRowDown[1] = firstParticle[1] - h/2;

	generateParticlesX(particles, radius, currentRowUp, type);
	generateParticlesX(particles, radius, currentRowDown, type);

	for (int j = 1; j < radius-1; j++){
		currentRowUp[1] += h;
		currentRowDown[1] -= h;

		Particle p1(currentRowUp, v, mass, type, false, false, sigma, epsilon);
		Particle p2(currentRowDown, v, mass, type, false, false, sigma, epsilon);

		particles.add(p1);
		particles.add(p2);

		generateParticlesX(particles, j, currentRowUp, type);
		generateParticlesX(particles, j, currentRowDown, type);

		stringstream ss;
		ss << j+1;
    	LOG4CXX_INFO(logger, "Generated row: " + ss.str());

	}

	currentRowUp[1] += h;
	currentRowDown[1] -= h;

	Particle p1(currentRowUp, v, mass, type, false, false, sigma, epsilon);
	Particle p2(currentRowDown, v, mass, type, false, false, sigma, epsilon);

	particles.add(p1);
	particles.add(p2);
}

void ParticleGeneratorSphere::generateParticlesX(ParticleContainer& particles, int n, utils::Vector<double, 3> currentRow, int type){
	utils::Vector<double, 3> firstPosRight = currentRow;
	firstPosRight[0] = firstParticle[0] + h/2;
	Particle p1(firstPosRight, v, mass, type, false, false, sigma, epsilon);
	particles.add(p1);

	utils::Vector<double, 3> firstPosLeft = currentRow;
	firstPosLeft[0] = firstParticle[0] - h/2;
	Particle p2(firstPosLeft, v, mass, type, false, false, sigma, epsilon);

	utils::Vector<double, 3> cP1 = firstPosRight;
	utils::Vector<double, 3> cP2 = firstPosLeft;

	for (int i = 1; i < x-(n+1); i++){
		firstPosRight[0] += h*i;
		Particle p1(cP1, v, mass, type, false, false, sigma, epsilon);
		particles.add(p1);

		firstPosLeft[0] -= h*i;
		Particle p2(cP2, v, mass, type, false, false, sigma, epsilon);
		particles.add(p2);

	}
}
