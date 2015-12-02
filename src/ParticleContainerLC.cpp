/*
 * ParticleContainerLC.cpp
 *
 *  Created on: Nov 22, 2015
 *      Author: jana
 */

#include "ParticleContainerLC.h"
#include <iterator>

static LoggerPtr logger(Logger::getLogger("global"));


ParticleContainerLC::ParticleContainerLC() {
	// TODO Auto-generated constructor stub

}

ParticleContainerLC::~ParticleContainerLC() {
	// TODO Auto-generated destructor stub
}

ParticleContainerLC::ParticleContainerLC(double r_cut, vector<int> domainSize){
	particles = vector<Particle>();

	this->r_cut = r_cut;

	this->domainSize[0] = domainSize[0];
	this->domainSize[1] = domainSize[1];
	this->domainSize[2] = domainSize[2];

	n_x = domainSize[0]/(r_cut)+4;
	n_y = domainSize[1]/(r_cut)+4;
	n_z = 1;
	dim = 2;
	if (domainSize[2] != 0){
		n_z = domainSize[2]/(r_cut);
		dim = 3;
//		grid[n_x*n_y*n_z];
	}

	grid= vector<list<Particle*>*>(n_x*n_y*n_z);

}

void ParticleContainerLC::init(double r_cut, vector<int> domainSize){
	this->r_cut = r_cut;


	if (domainSize[2] == 0){
		dim = 2;
	}
	else {
		dim = 3;
	}

	n_x = domainSize[0]/(r_cut)+4;
	n_y = domainSize[1]/(r_cut)+4;
	n_z = 1;
	if (dim == 3){
		n_z = domainSize[2]/(r_cut);
//		grid[n_x*n_y*n_z];
	}

	this->domainSize[0]=domainSize[0];
	LOG4CXX_INFO(logger,this->domainSize[0]);

	this->domainSize[1]=domainSize[1];
	LOG4CXX_INFO(logger, this->domainSize[1]);

	this->domainSize[2] = domainSize[2];
	LOG4CXX_INFO(logger, this->domainSize[2]);


//	this->domainSize = domainSize;




	grid= vector<list<Particle*>*>(n_x*n_y*n_z);


//	for (int i = 0; i < grid.size(); i++){
//	}


	LOG4CXX_INFO(logger, "Grid addr " << &grid << " " << &grid[0]);

	dims[0] = n_x;
	dims[1] = n_y;
	dims[2] = n_z;

	LOG4CXX_INFO(logger, "LCC created");

	vector<Particle>::iterator iter;

	for (iter = particles.begin(); iter != particles.end(); iter++){
		Particle& p = *iter;
		this->add(p);
	}



}

void ParticleContainerLC::add(Particle& p){
	utils::Vector<double, 3> position = p.getX();
	LOG4CXX_INFO(logger, dim);
	LOG4CXX_INFO(logger,this->r_cut);
	LOG4CXX_INFO(logger,&(this->domainSize[0]));
	LOG4CXX_INFO(logger, &n_x);
	LOG4CXX_INFO(logger, this->domainSize[1]);
	LOG4CXX_INFO(logger, this->domainSize[2]);
	LOG4CXX_INFO(logger, "Adding " + p.toString());

	if ((dim == 2 && position[0]<domainSize[0] && position[1]<domainSize[1]) ||
		(dim == 3 && position[0]<domainSize[0] && position[1]<domainSize[1] && position[2]<domainSize[2])){
	//each particle lands in cell position(coord)/factor_coord
	LOG4CXX_INFO(logger, "Adding " + p.toString());

	int cell_x = position[0]/r_cut;
	int cell_y = position[1]/r_cut;
	int cell_z = position[2]/r_cut;
	LOG4CXX_INFO(logger, "Adding at " << (n_x*n_y*cell_z + n_x*(cell_y+2) + cell_x + 2));
	LOG4CXX_INFO(logger, "grid size " << grid.size());

	if (!grid[n_x*n_y*cell_z + n_x*(cell_y+2) + cell_x+2]){
		grid[n_x*n_y*cell_z + n_x*(cell_y+2) + cell_x+2] = new list<Particle*>();
	}

	grid[n_x*n_y*cell_z + n_x*(cell_y+2) + cell_x+2]->push_back(&p);

	LOG4CXX_INFO(logger, "Added " + p.toString());
			}
	else {
		LOG4CXX_INFO(logger, "Particle out of bounds");
	}


}

list<list<Particle*>*> ParticleContainerLC::findNeighbors(int cell){
	list<list<Particle*>*> neighbors;

	//position in grid (a,b,c) or (a,b)
	int a_pos;
	int b_pos;
	int c_pos;


	if (dim == 2){
//		neighbors[8];
		a_pos = cell / n_x;
		b_pos = cell % n_x;

		int i = 0;

		//index in grid
		int ig;

		//coordinates of neighbor in grid
		int a = a_pos;
		int b = b_pos;

		while (i < 9){
			//coordinates of the neighbor in the grid
			a = a_pos;
			b = b_pos;


			//set a and b
			if (i / 3 == 0){
				a = a-1;
			}

			//if i/3==1 -> a = a

			else if (i / 3 == 2){
				a = a+1;
			}

			if (i % 3 == 0){
				b = b-1;
			}

			//if i%3==1 -> b=b

			else if (i % 3 == 2){
				b = b+1;
			}


				//convert to linearized index
				ig = n_x*(a+2)+b+2;


				if (a >= 0 && a < n_y && b >= 0 && b < n_x){
					neighbors.push_back(grid[ig]);
				}
/*				if (i < 4){
					neighbors[i]=&grid[ig];
				}
				else if (i > 4){
					neighbors[i-1]=&grid[ig];
				}
				*/
	//		}

			i++;
		}
	}
//	}


	if (dim == 3){
//		neighbors[26];
		a_pos = cell / (n_x*n_y);
		b_pos = (cell - a_pos*n_x*n_y) / n_x;
		c_pos = cell % n_x;

		int i = 0;

		//linearized index in grid array
		int ig;

		//coordinates of neighbor in grid
		int a;
		int b;
		int c;

		while (i < 27){
			//must be reset every iteration
			a = a_pos;
			b = b_pos;
			c = c_pos;

			if (i / 9 == 0){
				a = a-1;
			}

			//if i/9==1 -> a=a

			else if (i/9==2){
				a=a+1;
			}

			if ((i / 3) % 3 == 0){
				b=b-1;
			}

			//if (i/3)%3==1 -> b=b

			else if ((i / 3) % 3 == 2){
				b=b+1;
			}

			if (i % 3 == 0){
				c=c-1;
			}

			//if i%3==1 -> c=c

			else if (i % 3 == 2){
				c=c+1;
			}

			ig = a*n_y*n_x + (b+2)*n_x + c + 2;

			if (a >= 0 && a < n_z && b >= 0 && b < n_y && c >= 0 && c < n_x){
				neighbors.push_back(grid[ig]);

			}
			i++;

		}
	/*		if (i < 13){
				neighbors[i] = &grid[ig];
			}
			else if (i > 13){
				neighbors[i-1]= &grid[ig];
			}
			*/
	//		}


			}
		return neighbors;
}

vector<list<Particle*>*>* ParticleContainerLC::getGrid(){
	return &grid;
}

int* ParticleContainerLC::getGridDim(){
	return dims;
}

void ParticleContainerLC::updateGrid(){
	vector<Particle>::iterator iter;

	int oldCell;
	int newCell;

	utils::Vector<double, 3> oldPosition;
	utils::Vector<double, 3> newPosition;

	int old_cell_x;
	int old_cell_y;
	int old_cell_z;

	int new_cell_x;
	int new_cell_y;
	int new_cell_z;


	for (iter = particles.begin(); iter != particles.end(); iter++){
		Particle& p = *iter;

		oldPosition = p.getOldX();
		newPosition = p.getX();

		old_cell_x = oldPosition[0]/r_cut;
		old_cell_y = oldPosition[1]/r_cut;
		old_cell_z = oldPosition[2]/r_cut;

		oldCell = n_x*n_y*old_cell_z + n_x*(old_cell_y+2) + old_cell_x + 2;

		new_cell_x = newPosition[0]/r_cut;
		new_cell_y = newPosition[1]/r_cut;
		new_cell_z = newPosition[2]/r_cut;

		newCell = n_x*n_y*new_cell_z + n_x*(new_cell_y+2) + new_cell_x + 2;

		if(oldCell != newCell){
			list<Particle*>::iterator i;
			for(i = grid[oldCell]->begin(); i != grid[oldCell]->end(); i++){
				Particle& p2 = **i;

				LOG4CXX_INFO(logger, &p2);
				LOG4CXX_INFO(logger, &p);

				if (p == p2){
					grid[oldCell]->erase(i);
				}

			}

		if ( newPosition[0] < domainSize[0] && newPosition[1] < domainSize[1] && (dim == 2 || newPosition[2] < domainSize[2])){

			if (!grid[newCell]){
				grid[newCell] = new list<Particle*>();
			}

			grid[newCell]->push_back(&p);

		}
		}

	}
}

list<Particle*> ParticleContainerLC::getBoundaryParticles(){
	list<Particle*> boundary = list<Particle*>();

	list<Particle*>::iterator iter;

	//if there are 3 dimensions add the boundary for each rectangle in the z-direction
	for (int r = 0; r < n_z; r++){

	for (int i = 2; i < n_x-2; i++){
		//a == 0
		if (grid[i+2*n_x+r*n_x*n_y]){
		for (iter = grid[i+2*n_x+r*n_x*n_y]->begin(); iter != grid[i+2*n_x+r*n_x*n_y]->end(); iter++){
			boundary.push_back(*iter);
		}
		}
		LOG4CXX_INFO(logger,"a=0");

		//a == 1
		if(grid[i+n_x*3+r*n_x*n_y]){
		for (iter = grid[i+n_x+r*n_x*n_y]->begin(); iter != grid[i+n_x+r*n_x*n_y]->end(); iter++){
			boundary.push_back(*iter);
		}
		}
		LOG4CXX_INFO(logger,"a=1");


		//a == n_y-2
		if(grid[i+n_x*(n_y-4)]){
		for (iter = grid[i+n_x*(n_y-4)]->begin(); iter != grid[i+n_x*(n_y-4)+r*n_x*n_y]->end(); iter++){
			boundary.push_back(*iter);
		}
		}
		LOG4CXX_INFO(logger,"a=y-2");

		//a == n_y-1
		if(grid[i+n_x*(n_y-3)+r*n_x*n_y]){
		for (iter = grid[i+n_x*(n_y-3)+r*n_x*n_y]->begin(); iter != grid[i+n_x*(n_y-3)+r*n_x*n_y]->end(); iter++){
			boundary.push_back(*iter);
		}
		}
		LOG4CXX_INFO(logger,"a=y-1");

	}

	//a==2 to a==n_y-3, since a==0,a==1,a==n_y-2,n_y-1 already handled
	for (int i = 4; i < n_y-4; i++){
		//b == 0
		if (grid[i*n_x+2+r*n_x*n_y]){
		for (iter = grid[i*n_x+2+r*n_x*n_y]->begin(); iter != grid[i*n_x+2+r*n_x*n_y]->end(); iter++){
			boundary.push_back(*iter);
		}
		}
		LOG4CXX_INFO(logger,"b=0");

		//b == 1
		if(grid[i*n_x+3+r*n_x*n_y]){
		for (iter = grid[i*n_x+3+r*n_x*n_y]->begin(); iter != grid[i*n_x+3+r*n_x*n_y]->end(); iter++){
			boundary.push_back(*iter);
		}
		}
		LOG4CXX_INFO(logger,"b=1");

		//b == n_x-2
		if(grid[(i+1)*n_x-4+r*n_x*n_y]){
		for (iter = grid[(i+1)*n_x-4+r*n_x*n_y]->begin(); iter != grid[(i+1)*n_x-4+r*n_x*n_y]->end(); iter++){
			boundary.push_back(*iter);
		}
		}
		LOG4CXX_INFO(logger,"b=x-2");

		//b == n_x-1
		if(grid[(i+1)*n_x-3+r*n_x*n_y]){
		for (iter = grid[(i+1)*n_x-3+r*n_x*n_y]->begin(); iter != grid[(i+1)*n_x-3+r*n_x*n_y]->end(); iter++){
			boundary.push_back(*iter);
		}
		}
		LOG4CXX_INFO(logger,"b=x-1");

	}
	}


	return boundary;
}

list<Particle*> ParticleContainerLC::getHalo(){
	list<Particle*> halo = list<Particle*>();

	list<Particle*>::iterator iter;

	//if there are 3 dimensions add the boundary for each rectangle in the z-direction
	for (int r = 0; r < n_z; r++){

	for (int i = 0; i < n_x; i++){
		//a == 0
		for (iter = grid[i+r*n_x*n_y]->begin(); iter != grid[i+r*n_x*n_y]->end(); iter++){
			halo.push_back(*iter);
		}

		//a == 1
		for (iter = grid[i+n_x+r*n_x*n_y]->begin(); iter != grid[i+n_x+r*n_x*n_y]->end(); iter++){
			halo.push_back(*iter);
		}

		//a == n_y-2
		for (iter = grid[i+n_x*(n_y-2)]->begin(); iter != grid[i+n_x*(n_y-2)+r*n_x*n_y]->end(); iter++){
			halo.push_back(*iter);
		}

		//a == n_y-1
		for (iter = grid[i+n_x*(n_y-1)+r*n_x*n_y]->begin(); iter != grid[i+n_x*(n_y-1)+r*n_x*n_y]->end(); iter++){
			halo.push_back(*iter);
		}

	}

	//a==2 to a==n_y-3, since a==0,a==1,a==n_y-2,n_y-1 already handled
	for (int i = 2; i < n_y-2; i++){
		//b == 0
		for (iter = grid[i*n_x+r*n_x*n_y]->begin(); iter != grid[i*n_x+r*n_x*n_y]->end(); iter++){
			halo.push_back(*iter);
		}

		//b == 1
		for (iter = grid[i*n_x+1+r*n_x*n_y]->begin(); iter != grid[i*n_x+1+r*n_x*n_y]->end(); iter++){
			halo.push_back(*iter);
		}

		//b == n_x-2
		for (iter = grid[(i+1)*n_x-2+r*n_x*n_y]->begin(); iter != grid[(i+1)*n_x-2+r*n_x*n_y]->end(); iter++){
			halo.push_back(*iter);
		}

		//b == n_x-1
		for (iter = grid[(i+1)*n_x-1+r*n_x*n_y]->begin(); iter != grid[(i+1)*n_x-1+r*n_x*n_y]->end(); iter++){
			halo.push_back(*iter);
		}
	}
	}


	return halo;
}

