/*
 * ParseInput.cpp
 *
 *  Created on: Dec 20, 2015
 *      Author: jana
 */

#include "ParseInput.h"
#include "SimulationInput.hxx"

ParseInput::ParseInput() {
	// TODO Auto-generated constructor stub

}

ParseInput::ParseInput(const char* filename):
	filename(filename){
}

ParseInput::~ParseInput() {
	// TODO Auto-generated destructor stub
}

void ParseInput::readXML(){
	auto_ptr<simulationInput> si;
	try {
		si = simulationInput(filename);
	}
	catch (const xml_schema::exception& e){
		cerr << "Error reading file" << endl;
		exit(1);
	}

	const globalVals& vals = si->global();
	this->end_time = vals.endTime();
	this->delta_t = vals.deltaT();
//	this->applyTemp = vals.applyTemp();
	this->boundaryType = vals.boundaryType();
	this->gravity = vals.gravity();
	this->simType = vals.simType();
	this->w3d = vals.use3d();

	if (simType == 1){
		const linkedCellInput& lci = si->linkedCell();
		r_cut = lci.rcut();
		domain[3] = {lci.domainSizeX(), lci.domainSizeY(), lci.domainSizeZ()};
	}

	if (w3d){
		this->numDim = 3;
	}
	else{
		this->numDim = 2;
	}

	const particleInput& particles = si->particles();
	int t = 0;
	for (particleInput::cuboid_const_iterator i = particles.cuboid().begin();
			i != particles.cuboid().end(); i++){
		double firstPos[3] = {i->firstPosX(),i->firstPosY(),i->firstPosZ()};
		double startVel[3] = {i->startVelX(), i->startVelY(),i->startVelZ()};
		int n1 = i->lengthX();
		int n2 = i->lengthY();
		int n3 = i->lengthZ();
		double mass = i->mass();
		double h = i->dist();
		double sigma = i->sigma();
		double epsilon = i->epsilon();

		ParticleGeneratorCuboid c(particles, firstPos, n1, n2, n3, h,
				mass, startVel, sigma, epsilon, t);
		t++;
	}

	t = 0;
	for (particleInput::sphere_const_iterator i = particles.sphere().begin();
			i != particles.cuboid().end(); i++){
		double center[3] = {i->centerX(), i->centerY(), i->centerZ()};
		double startVel[3] = {i->startVelX(), i->startVelY(), i->startVelZ()};
		int radius = i->radius();
		double h = i->dist();
		double mass = i->mass();
		double sigma = i->sigma();
		double epsilon = i->epsilon();

		ParticleGeneratorSphere s(particles, center, radius, h, mass, startVel,
				w3d, sigma, epsilon, t);
		t++;
	}

	for (particleInput::list_const_iterator i = particles.list().begin();
			i != particles.list().end(); i++){
		char* filename = i->listParticleInput();
		FileReader fr;
//		fr.readFile(particles, filename);
	}

	const tempInput& temp = si->temp();
	this->initTemp = temp.initTemp();
	this->timeSteps = temp.stepsApply();
	if (temp.brown().present()){
		this->brown = temp.brown();
	}
	else{
		this->brown = false;
	}
	if (temp.deltaTemp().present()){
		this->tempDiff = temp.deltaTemp();
		this->timeStepsChange = temp.deltaTempSteps();
		this->targetTemp = temp.targetTemp();
		this->varTemp = true;
	}

	const outputData& od = si->outData();
	this->writeTo = od.filename();
	this->writeFreq = od.writeFreq();
	if (od.writeType().present()){
		this->writeType = od.writeType();
	}
	else{
		this->writeType = 0;
	}

}

void ParseInput::configure(){
	::end_time = this->end_time;
	::delta_t = this->delta_t;
	::boundaryType = this->boundaryType;
	::gravity = this->gravity;
	::numDim = this->numDim;


	if (simType == 0){
		::particles = new ParticleContainer();
	}
	else {
		::particles = new ParticleContainerLC(r_cut, domain);
	}

	if (varTemp){
		::thermostat(particles, initTemp, timeSteps, brown, targetTemp, tempDiff, timeStepsChange, numDim);
	}
	else{
		::thermostat(particles, initTemp, timeSteps, brown, numDim);
	}
}









