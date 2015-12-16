/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"
#include "utils/Vector.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace std;

static LoggerPtr logger(Logger::getLogger("global"));

FileReader::FileReader() {
}

FileReader::~FileReader() {
}


void FileReader::readFile(ParticleContainer& particles, char* filename) {
	double x[] = {0,0,0};
	double old_x[] = {0,0,0};
	double v[] = {1,1,1};
	double f[] = {0,0,0};
	double old_f[] = {0,0,0};
	double m = 1;
	int type;
	bool halo;
	bool skip;
	double sigma;
	double epsilon;
    int num_particles = 0;

    std::ifstream input_file(filename);
    string tmp_string;

    if (input_file.is_open()) {

    	getline(input_file, tmp_string);
    	LOG4CXX_INFO(logger, "Read line: " + tmp_string);

    	while (tmp_string.size() == 0 || tmp_string[0] == '#') {
    		getline(input_file, tmp_string);
//    		cout << "Read line: " << tmp_string << endl;
        	LOG4CXX_INFO(logger, "Read line: " + tmp_string);
    	}

    	istringstream numstream(tmp_string);
    	numstream >> num_particles;
    	LOG4CXX_INFO(logger, "Readng " + numstream.str() + " particles.");
    	getline(input_file, tmp_string);
    	LOG4CXX_INFO(logger, "Read line: " + tmp_string);

    	for (int i = 0; i < num_particles; i++) {
    		istringstream datastream(tmp_string);

    		for (int j = 0; j < 3; j++) {
    			datastream >> x[j];

    		}

    		for (int j = 0; j < 3; j++){
    			datastream >> old_x[j];
    		}
    		for (int j = 0; j < 3; j++) {
    			datastream >> v[j];
    		}
    		for (int j = 0; j < 3; j++){
    			datastream >> f[j];
    		}
    		for (int j = 0; j < 3; j++){
    			datastream >> old_f[j];
    		}
    		if (datastream.eof()) {
    			LOG4CXX_FATAL(logger,"Error reading file: eof reached unexpectedly reading from line " + i)
    			exit(-1);
    		}
    		datastream >> m;
    		datastream >> type;
    		datastream >> halo;
    		datastream >> skip;
    		datastream >> sigma;
    		datastream >> epsilon;
    		Particle p(x, v, m, type, halo, skip, sigma, epsilon);
    		particles.add(p);

    		getline(input_file, tmp_string);
        	LOG4CXX_INFO(logger, "Read line: " + tmp_string);
    	}
    } else {
    	LOG4CXX_FATAL(logger, "Error: could not open file");
    	exit(-1);
    }

}

