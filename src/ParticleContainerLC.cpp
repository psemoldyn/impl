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

void ParticleContainerLC::init(double r_cut, vector<int> domainSize){
	this->r_cut = r_cut;
	if (domainSize.size() == 2){
		dim = 2;
	}
	else if (domainSize.size() == 3){
		dim = 3;
	}

	this->domainSize = domainSize;

	n_x = domainSize[0]/(r_cut);
	n_y = domainSize[1]/(r_cut);
	if (dim == 3){
		n_z = domainSize[2]/(r_cut);
//		grid[n_x*n_y*n_z];
		grid = vector<list<Particle>*>(n_x*n_y*n_z);
	}
	//if dim !=3 then it must be dim=2, set grid accordingly
	else {
		n_z = 1;
//		grid[n_x*n_y];
		grid= vector<list<Particle>*>(n_x*n_y);
	}

//	for (int i = 0; i < grid.size(); i++){
//	}


	LOG4CXX_INFO(logger, "Grid addr " << &grid << " " << &grid[0]);

	dims[0] = n_x;
	dims[1] = n_y;
	dims[2] = n_z;

	LOG4CXX_INFO(logger, "LCC created");

	vector<Particle>::iterator iter;

	for (iter = particles.begin(); iter < particles.end(); iter++){
		Particle& p = *iter;
		this->add(p);
	}



}

void ParticleContainerLC::add(Particle& p){
	//just a basic implementation: think of a way to handle negative coordinates
	utils::Vector<double, 3> position = p.getX();
	//each particle lands in cell position(coord)/factor_coord
	LOG4CXX_INFO(logger, "Adding " + p.toString());

	int cell_x = position[0]/r_cut;
	int cell_y = position[1]/r_cut;
	int cell_z = position[2]/r_cut;
	LOG4CXX_INFO(logger, "Adding at " << (n_x*n_y*cell_z + n_x*cell_y + cell_x));
	LOG4CXX_INFO(logger, "grid size " << grid.size());

	if (!grid[n_x*n_y*cell_z + n_x*cell_y + cell_x]){
		grid[n_x*n_y*cell_z + n_x*cell_y + cell_x] = new list<Particle>();
	}

	grid[n_x*n_y*cell_z + n_x*cell_y + cell_x]->push_back(p);

	LOG4CXX_INFO(logger, "Added " + p.toString());


}

list<list<Particle>*> ParticleContainerLC::findNeighbors(int cell){
	list<list<Particle>*> neighbors;

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
				ig = n_x*a+b;


				if (grid[ig]){
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
		a_pos = cell / n_x*n_y;
		b_pos = (cell / n_x) % n_y;
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

			ig = c*n_y*n_x + b*n_x + a;

			if (grid[ig]){
				neighbors.push_back(grid[ig]);

			}
			}
	/*		if (i < 13){
				neighbors[i] = &grid[ig];
			}
			else if (i > 13){
				neighbors[i-1]= &grid[ig];
			}
			*/
	//		}

			i++;

			}
		return neighbors;
}

vector<list<Particle>*> ParticleContainerLC::getGrid(){
	return grid;
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

		oldCell = n_z*n_y*old_cell_z + n_x*old_cell_y + old_cell_x;

		new_cell_x = newPosition[0]/r_cut;
		new_cell_y = newPosition[1]/r_cut;
		new_cell_z = newPosition[2]/r_cut;

		newCell = n_z*n_y*new_cell_z + n_x*new_cell_y + new_cell_x;

		if (oldCell != newCell){
			list<Particle>::iterator i;
			for(i = grid[oldCell]->begin(); i != grid[oldCell]->end(); i++){
				Particle& p2 = *i;
				if (p == p2){
					grid[oldCell]->erase(i);
				}

			}

			if (!grid[newCell]){
				grid[newCell] = new list<Particle>();
			}

			grid[newCell]->push_back(p);
		}

	}
}
