/*
 * ParseInput.cpp
 *
 *  Created on: Dec 20, 2015
 *      Author: jana
 */

#include "ParseInput.h"

//#include "SimulationInput.h"

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
		si = sim(this->filename);
	}
	catch (const xml_schema::exception& e){
		cerr << e << endl;
		exit(1);
	}

	const globalVals& vals = si->global();
	this->end_time = *vals.endTime();
	this->delta_t = *vals.deltaT();
	this->boundaryType = *vals.boundaryType();
	if (vals.gravity().present()){
		this->gravity = *vals.gravity();
	}
	else {
		this->gravity = 0;
	}
	this->simType = *vals.simType();
	this->w3d = *vals.use3d();

/*	if (simType == 1){

	}*/

	if (w3d){
		this->numDim = 3;
	}
	else{
		this->numDim = 2;
	}

	if (simType == 0){
		::particles = new ParticleContainerN();
		::fc = new ForceCalcN(gravity);
	}
	else {
		const linkedCellInput& lci = si->linkedCell();
		vector<double>* dS = new vector<double>(3);

		r_cut = *lci.rcut();
		domain[0] = *lci.domainSizeX();
		domain[1] = *lci.domainSizeY();
		domain[2] = *lci.domainSizeZ();

		(*dS)[0] = domain[0];
		(*dS)[1] = domain[1];
		(*dS)[2] = domain[2];
		::particles = new ParticleContainerLC(r_cut, *dS);
		::fc = new ForceCalcLC(r_cut, gravity);
		cout << "created LC cont" << endl;
	}

	this->particles = new vector<Particle>();

	const particleInput& particles = si->particles();
	int t = 0;
	for (particleInput::cuboid_const_iterator i = particles.cuboid().begin();
			i != particles.cuboid().end(); i++){
		double firstPos[3] = {*i->firstPosX(),*i->firstPosY(),*i->firstPosZ()};
		double startVel[3] = {*i->startVelX(), *i->startVelY(),*i->startVelZ()};
		int n1 = *i->lengthX();
		int n2 = *i->lengthY();
		int n3 = *i->lengthZ();
		double mass = *i->mass();
		double h = *i->dist();
		double sigma = *i->sigma();
		double epsilon = *i->epsilon();

		cout << "generating cuboid" << endl;

		ParticleGeneratorCuboid c(*this->particles, firstPos, n1, n2, n3, h,
				mass, startVel, sigma, epsilon, t);

		cout << "generated cuboid" << t << endl;

		t++;
	}

	cout << "read cuboids" << endl;


	t = 0;
	for (particleInput::sphere_const_iterator i = particles.sphere().begin();
			i != particles.sphere().end(); i++){
		double center[3] = {*i->centerX(), *i->centerY(), *i->centerZ()};
		double startVel[3] = {*i->startVelX(), *i->startVelY(), *i->startVelZ()};
		int radius = *i->radius();
		double h = *i->dist();
		double mass = *i->mass();
		double sigma = *i->sigma();
		double epsilon = *i->epsilon();

		ParticleGeneratorSphere s(*this->particles, center, radius, h, mass, startVel,
				w3d, sigma, epsilon, t);
		t++;
	}

	cout << "read spheres" << endl;

//	for (particleInput::list_const_iterator i = particles.list().begin();
//			i != particles.list().end(); i++){
//		char* filename = i->listParticleInput()->c_str();
//		FileReader fr;
//		fr.readFile(particles, filename);
//	}

	const tempInput& temp = si->temp();
	this->initTemp = *temp.initTemp();
	this->timeSteps = *temp.stepsApply();
	if (temp.brown().present()){
		this->brown = *temp.brown();
	}
	else{
		this->brown = false;
	}
	if (temp.deltaTemp().present()){
		this->tempDiff = *temp.deltaTemp();
		this->timeStepsChange = *temp.deltaTempSteps();
		this->targetTemp = *temp.targetTemp();
		this->varTemp = true;
	}

	cout << "read temp" << endl;

	const outputData& od = si->outData();
	this->writeTo = od.filename()->c_str();

	cout << "read outputfilename" << endl;

	this->writeFreq = *od.writeFreq();
	if (od.writeType().present()){
		this->writeType = *od.writeType();
	}
	else{
		this->writeType = 0;
	}

}

void ParseInput::configure(){
	readXML();

	cout << "read file" << endl;

	::end_time = this->end_time;
	::delta_t = this->delta_t;
	::boundaryType = this->boundaryType;
	::gravity = this->gravity;
	::numDim = this->numDim;
	::vc = new VelocityCalc(delta_t, numDim);
	::pc = new PositionCalc(delta_t);
	::simType = this->simType;

	vector<Particle>::iterator i;

	for (i = this->particles->begin(); i != this->particles->end(); i++){
		::particles->add(*i);
	}

	if (varTemp){
		::thermostat = new Thermostat(*::particles, initTemp, timeSteps, brown, targetTemp, tempDiff, timeStepsChange, numDim);
	}
	else{
		::thermostat = new Thermostat(*::particles, initTemp, timeSteps, brown, numDim);
	}
}









