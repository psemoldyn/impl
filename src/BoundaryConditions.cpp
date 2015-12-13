/*
 * BoundaryConditions.cpp
 *
 *  Created on: Dec 7, 2015
 *      Author: jana
 */

#include "BoundaryConditions.h"

BoundaryConditions::BoundaryConditions() {
	// TODO Auto-generated constructor stub

}

BoundaryConditions::~BoundaryConditions() {
	// TODO Auto-generated destructor stub
}

void BoundaryConditions::applyOutflowBoundary(ParticleContainerLC& pc){
	vector<Particle>::iterator i;

	int n_x = pc.getGridDim()[0];
	int n_y = pc.getGridDim()[1];
	int n_z = pc.getGridDim()[2];

	int r_cut = pc.getCutoff();

	double r_cut_x = pc.getCutX();
	double r_cut_y = pc.getCutY();
	double r_cut_z = pc.getCutZ();

	vector<double> domainSize = vector<double>(3);
	domainSize[0] = pc.getDomain()[0];
	domainSize[1] = pc.getDomain()[1];
	domainSize[2] = pc.getDomain()[2];



	for (i = pc.begin(); i != pc.end(); i++){
		Particle& p = *i;

		utils::Vector<double, 3>& pos = p.getX();
		utils::Vector<double, 3>& oldPos = p.getOldX();

		int oldCell;

		if (!p.getSkip() &&
			(pos[0] < 0 || pos[0] > domainSize[0] ||
			 pos[1] < 0 || pos[1] > domainSize[1] ||
			 pos[2] < 0 || pos[2] > domainSize[2])){

			int old_cell_x = oldPos[0]/r_cut_x;
			int old_cell_y = oldPos[1]/r_cut_y;
			int old_cell_z = oldPos[2]/r_cut_z;

			oldCell = n_x*n_y*old_cell_z + n_x*(old_cell_y+2) + old_cell_x + 2;

			pc.delFromCell(p, oldCell);
			p.getSkip() = true;

		}

	}

}

void BoundaryConditions::applyReflectingBoundary(ParticleContainerLC& pc, double epsilon, double sigma){
	vector<Particle>::iterator i;

	for (i = pc.begin(); i != pc.end(); i++){
	Particle& p1 = *i;

	vector<double> domainSize = vector<double>(3);
	domainSize[0] = pc.getDomain()[0];
	domainSize[1] = pc.getDomain()[1];
	domainSize[2] = pc.getDomain()[2];

	//right boundary
		if (abs(p1.getX()[0]-domainSize[0])< pow(2,1/6.0)*sigma && abs(p1.getX()[0]-domainSize[0]) !=0){
			double x[3] = {domainSize[0], p1.getX()[1], p1.getX()[2]};
			utils::Vector<double, 3> v = p1.getV();
			Particle counterParticle(x,v,p1.getM());

			double distance = (p1.getX()-counterParticle.getX()).L2Norm();

			p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

		}

	//left boundary
		if (abs(p1.getX()[0])< pow(2,1/6.0)*sigma && abs(p1.getX()[0]) !=0){
			double x[3] = {0, p1.getX()[1], p1.getX()[2]};
			utils::Vector<double, 3> v = p1.getV();
			Particle counterParticle(x,v,p1.getM());

			double distance = (p1.getX()-counterParticle.getX()).L2Norm();

			p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

		}

	//upper boundary
		if (abs(p1.getX()[1]-domainSize[1])< pow(2,1/6.0)*sigma && abs(p1.getX()[1] - domainSize[1]) !=0){
			double x[3] = {p1.getX()[0], domainSize[1], p1.getX()[2]};
			utils::Vector<double, 3> v = p1.getV();
			Particle counterParticle(x,v,p1.getM());

			double distance = (p1.getX()-counterParticle.getX()).L2Norm();

			p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

		}

	//lower boundary
		if (abs(p1.getX()[1])< pow(2,1/6.0)*sigma && abs(p1.getX()[1]) !=0){
			double x[3] = {p1.getX()[0], 0, p1.getX()[2]};
			utils::Vector<double, 3> v = p1.getV();
			Particle counterParticle(x,v,p1.getM());

			double distance = (p1.getX()-counterParticle.getX()).L2Norm();

			p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

		}
		if (domainSize[2] != 0){

			//back boundary
			if (abs(p1.getX()[2]-domainSize[2])< pow(2,1/6.0)*sigma && abs(p1.getX()[2]-domainSize[2]) !=0){
				double x[3] = {p1.getX()[0], p1.getX()[1], domainSize[2]};
				utils::Vector<double, 3> v = p1.getV();
				Particle counterParticle(x,v,p1.getM());

				double distance = (p1.getX()-counterParticle.getX()).L2Norm();

				p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

			}

	//front boundary
			if (abs(p1.getX()[2])< pow(2,1/6.0)*sigma && abs(p1.getX()[2]) !=0){
				double x[3] = {p1.getX()[0], p1.getX()[1], 0};
				utils::Vector<double, 3> v = p1.getV();
				Particle counterParticle(x,v,p1.getM());

				double distance = (p1.getX()-counterParticle.getX()).L2Norm();

				p1.getF() = p1.getF() + 24.0*epsilon/(pow(distance,2))*(pow(sigma/distance,6)-2*pow(sigma/distance,12))*(counterParticle.getX()-p1.getX());

			}
		}
	}
}
