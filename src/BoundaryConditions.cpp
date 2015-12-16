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


void BoundaryConditions::applyPeriodicBoundary(ParticleContainerLC& pc){
	vector<Particle>::iterator i;
	list<Particle*>::iterator iter;

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

	vector<list<Particle*>*>* grid = pc.getGrid();

	for (i = pc.begin(); i != pc.end(); i++){
		Particle& p = *i;

		utils::Vector<double, 3>& pos = p.getX();
		utils::Vector<double, 3>& oldPos = p.getOldX();

		int old_cell_x = oldPos[0]/r_cut_x;
		int old_cell_y = oldPos[1]/r_cut_y;
		int old_cell_z = oldPos[2]/r_cut_z;

		int halo_cell_x = old_cell_x;
		int halo_cell_y = old_cell_y;
		int halo_cell_z = old_cell_z;

		int oldCell = n_x*n_y*old_cell_z + n_x*(old_cell_y+2) + old_cell_x + 2;
		int haloCell = oldCell;

		if (pos[0] < 0){
			pos[0] = pc.getDomain()[0];
			//move to halo
			halo_cell_x = pc.getDomain()[0]/r_cut_x;
			halo_cell_x++;
			haloCell = n_x*n_y*old_cell_z + n_x*(old_cell_y+2) + halo_cell_x + 2;

		}

		if (pos[0] > pc.getDomain()[0]){
			pos[0] = 0;
			halo_cell_x = 0/r_cut_x;
			halo_cell_x--;
			haloCell =/* n_x*n_y*old_cell_z +*/ n_x*(old_cell_y+2) + halo_cell_x +2;
		}

		if (pos[1] < 0){
			pos[1] = pc.getDomain()[1];
			halo_cell_y = pc.getDomain()[1]/r_cut_y;
			halo_cell_y++;
			haloCell = n_x*n_y*old_cell_z + n_x*(old_cell_y+2) + halo_cell_x +2;
		}

		if (pos[1] > pc.getDomain()[1]){
			pos[1] = 0;
			halo_cell_y = 0/r_cut_y;
			halo_cell_y--;
			haloCell =/* n_x*n_y*old_cell_z + */ n_x*(old_cell_y+2) + halo_cell_x +2;
		}
/*
		if (pos[2] < 0){
			pos[2] += pc.getDomain()[2];
			halo_cell_z = pc.getDomain()[2]/r_cut_z;
			halo_cell_z++;
			haloCell = n_x*n_y*old_cell_z + n_x*(old_cell_y+2) + halo_cell_x +2;
		}

		if (pos[2] > pc.getDomain()[2]){
			pos[2] -= pc.getDomain()[2];
			halo_cell_z = 0/r_cut_z;
			halo_cell_z--;
			haloCell = n_x*n_y*old_cell_z + n_x*(old_cell_y+2) + halo_cell_x +2;

		} */

		for (iter = (*grid)[oldCell]->begin(); iter != (*grid)[oldCell]->end(); iter++){
			Particle& p2 = **iter;
			if (!(p2==p) && oldCell != haloCell){
				Particle copy(p2);
				copy.getHalo() = true;
				copy.getSkip() = true;
				if (!(*grid)[haloCell]){
					(*grid)[haloCell] = new list<Particle*>();
				}
				if (pos[0] < 0){
				copy.getX()[0] += domainSize[0];
				}
				if (pos[1] < 0){
				copy.getX()[1] += domainSize[1];
				}
//				copy.getX()[2] = halo_cell_z*r_cut_z;
				if (pos[0] > domainSize[0]){
				copy.getX()[0] -= domainSize[0];
				}
				if (pos[1] > domainSize[1]){
				copy.getX()[1] -= domainSize[1];
				}

				(*grid)[haloCell]->push_back(&copy);
				pc.getParticles().push_back(copy);
			}
		}


	}
}
