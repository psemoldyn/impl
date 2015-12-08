/*
 * BoundaryConditions.h
 *
 *  Created on: Dec 7, 2015
 *      Author: jana
 */

#ifndef SRC_BOUNDARYCONDITIONS_H_
#define SRC_BOUNDARYCONDITIONS_H_

#include <vector>
#include "utils/Vector.h"
#include "ParticleContainerLC.h"

class BoundaryConditions {
public:
	BoundaryConditions();
	virtual ~BoundaryConditions();

	void applyOutflowBoundary(ParticleContainerLC& pc);
	void applyReflectingBoundary(ParticleContainerLC& pc, double epsilon, double sigma);
};

#endif /* SRC_BOUNDARYCONDITIONS_H_ */
