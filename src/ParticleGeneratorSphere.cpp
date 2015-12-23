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

ParticleGeneratorSphere::ParticleGeneratorSphere(vector<Particle>& particles, char* filename){
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
       		datastream >> radius;

          	//read distance
         	datastream >> h;

          	//read mass
          	datastream >> mass;

          	datastream >> sigma;

          	datastream >> epsilon;

          	if (w3d){
          		generateParticles(particles, i);
          	}
          	else {
          		generateCircle(particles, radius, firstParticle, i);
          	}
        }
    }
}


ParticleGeneratorSphere::ParticleGeneratorSphere(vector<Particle>& particles, utils::Vector<double, 3> start, int radius, double h,
		double mass, utils::Vector<double, 3> v, bool w3d, double sigma, double epsilon, int type):
			firstParticle(start),
			radius(radius),
			h(h),
			mass(mass),
			v(v),
			w3d(w3d),
			sigma(sigma),
			epsilon(epsilon)
{
	if (w3d){
		generateParticles(particles, type);
	}
	else{
		generateCircle(particles, radius, firstParticle, type);
	}
}


void ParticleGeneratorSphere::generateParticles(vector<Particle>& particles, int type){
	generateCircle(particles, radius, firstParticle, type);

	utils::Vector<double, 3> currentZFront = currentParticle;
	utils::Vector<double, 3> currentZBack = currentParticle;

	currentZFront[2] = firstParticle[2] - h/2;
	currentZBack[2] = firstParticle[2] - h/2;

	generateCircle(particles, radius, currentZFront, type);
	generateCircle(particles, radius, currentZBack, type);


	for (int i= 1; i < radius-1; i++){
		currentZFront[2] -= h;
		currentZBack[2] += h;

		generateCircle(particles, radius-i, currentZFront, type);
		generateCircle(particles, radius-i, currentZBack, type);

	}
}


void ParticleGeneratorSphere::generateCircle(vector<Particle>& particles, int radius, utils::Vector<double, 3> currentZ, int type){
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

		particles.push_back(p1);
		particles.push_back(p2);

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

	particles.push_back(p1);
	particles.push_back(p2);
}

void ParticleGeneratorSphere::generateParticlesX(vector<Particle>& particles, int n, utils::Vector<double, 3> currentRow, int type){
	utils::Vector<double, 3> firstPosRight = currentRow;
	firstPosRight[0] = firstParticle[0] + h/2;
	Particle p1(firstPosRight, v, mass, type, false, false, sigma, epsilon);
	particles.push_back(p1);

	utils::Vector<double, 3> firstPosLeft = currentRow;
	firstPosLeft[0] = firstParticle[0] - h/2;
	Particle p2(firstPosLeft, v, mass, type, false, false, sigma, epsilon);

	utils::Vector<double, 3> cP1 = firstPosRight;
	utils::Vector<double, 3> cP2 = firstPosLeft;

	for (int i = 1; i < radius-(n+1); i++){
		firstPosRight[0] += h*i;
		Particle p1(cP1, v, mass, type, false, false, sigma, epsilon);
		particles.push_back(p1);

		firstPosLeft[0] -= h*i;
		Particle p2(cP2, v, mass, type, false, false, sigma, epsilon);
		particles.push_back(p2);

	}
}
