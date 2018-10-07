/* Start Header ------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

File Name:      ClothParticleConstraint.h
Purpose:        This is the Culling class implementations.
Language:       C++
Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/


#include "FluidParticle.h"
#ifndef CLOTH_PARTICLE_CONSTRAINT_H
#define	CLOTH_PARTICLE_CONSTRAINT_H
class ClothParticleConstraint
{
private:
	f32 rest_distance; // the length between particle p1 and p2 in rest configuration

public:
	u32  i1, i2; // the two particles that are connected through this constraint

	ClothParticleConstraint(std::vector<FluidParticle>& p, u32 i1, u32 i2)
		: i1(i1)
		, i2(i2)
	{
		FluidParticle& p1(p[i1]);
		FluidParticle& p2(p[i2]);

		Vec3 vec = p1.getPos() - p2.getPos();
		rest_distance = glm::length(vec);

	}

	ClothParticleConstraint(const ClothParticleConstraint& r)
		: i1(r.i1)
		, i2(r.i2)
		, rest_distance(r.rest_distance)
	{

	}

	/* This is one of the important methods, where a single constraint between two particles p1 and p2 is solved
	the method is called by Cloth.time_step() many times per frame*/
	void satisfyConstraint(std::vector<FluidParticle>& p)
	{
		FluidParticle& p1(p[i1]);
		FluidParticle& p2(p[i2]);
		Vec3 p1_to_p2 = p2.getPos() - p1.getPos(); // vector from p1 to p2
		f32 current_distance = glm::length(p1_to_p2); // current distance between p1 and p2
		if (current_distance != current_distance)
		{
			return;
		}
		if (current_distance > rest_distance)
		{

		}
		current_distance = (std::isinf(current_distance)) ? rest_distance : current_distance;
		float factor = (current_distance <= EPSILON) ? 0.0f : rest_distance/current_distance ;
		Vec3 correctionVector = p1_to_p2*((1.f - factor) *0.5f); // The offset vector that could moves p1 into a distance of rest_distance to p2

		p1.offsetPos(correctionVector); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
		p2.offsetPos(-correctionVector); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.	


	}

};
#endif