/* Start Header ------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

File Name:      FluidParticle.h
Purpose:        This is the Culling class implementations.
Language:       C++
Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/

#include "math.hpp"
#include "mesh.hpp"
#ifndef	FLUID_PARTICLE_H
#define FLUID_PARTICLE_H

#define DAMPING 0.1f // how much to damp the cloth simulation each frame
#define TIME_STEPSIZE2 0.5f*0.5f	 // how large time step each particle takes each frame
/* The particle class represents a particle of mass that can move around in 3D space*/
class FluidParticle
{
private:
	bool movable; // can the particle move or not ? used to pin parts of the cloth
	f32 radius;
	f32 mass; // the mass of the particle (is always 1 in this example)
	f32 invMass; // the mass of the particle (is always 1 in this example)
	Vec3 pos; // the current position of the particle in 3D space
	Vec3 old_pos; // the position of the particle in the previous time step, used as part of the verlet numerical integration scheme
	Vec3 acceleration; // a vector representing the current acceleration of the particle
	Vec3 accumulated_normal; // an accumulated normal (i.e. non normalized), used for OpenGL soft shading
	Vec3 accumulated_tangent;
public:
	bool isCollided;
	FluidParticle(Vec3 pos, f32 radius) 
		: pos(pos), old_pos(pos)
		, acceleration(Vec3(0, 0, 0))
		, mass(1.f)
		, invMass(1.f/mass)
		, movable(true)
		, accumulated_normal(Vec3(0, 0, 0))
		, accumulated_tangent(Vec3(0, 0, 0))
		, radius(radius)
		, isCollided(false)
	{}
	FluidParticle()
		: pos(Vec3(0, 0, 0)), old_pos(Vec3(0, 0, 0))
		, acceleration(Vec3(0, 0, 0))
		, mass(1.f)
		, invMass(1.f / mass)
		, movable(true)
		, accumulated_normal(Vec3(0, 0, 0))
		, accumulated_tangent(Vec3(0, 0, 0))
		, radius(0.f)
		, isCollided(false)
	{}

	FluidParticle(const FluidParticle& r)
		: pos(r.pos)
		, old_pos(r.old_pos)
		, acceleration(r.acceleration)
		, mass(r.mass)
		, invMass(r.invMass)
		, movable(r.movable)
		, accumulated_normal(r.accumulated_normal)
		, accumulated_tangent(r.accumulated_tangent)
		, radius(r.radius)
		, isCollided(r.isCollided)
	{}

	void addForce(Vec3 f)
	{
		if (f.x != f.x || f.y != f.y || f.z != f.z) return;
		acceleration += f * invMass;
	}

	/* This is one of the important methods, where the time is progressed a single step size (TIME_STEPSIZE)
	The method is called by Cloth.time_step()
	Given the equation "force = mass * acceleration" the next position is found through verlet integration*/
	void timeStep(f32 dt)
	{
		if (movable)
		{
			Vec3 temp = pos;
			vec3 tpos = pos - old_pos;
			if (tpos.x != tpos.x || tpos.y != tpos.y || tpos.z != tpos.z) return;
			pos += (pos - old_pos)*(1.0f - DAMPING) + acceleration*dt*dt;
			acceleration = Vec3(0, 0, 0); // acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)
			if (pos.x != pos.x || pos.y != pos.y || pos.z != pos.z || std::isinf(pos.x) || std::isinf(pos.y) || std::isinf(pos.z))
			{
				pos = temp;
				return;
			}
			old_pos = temp;

		}
	}

	Vec3& getPos() { return pos; }

	void resetAcceleration() { acceleration = Vec3(0, 0, 0); }

	void offsetPos(const Vec3 v) { 
			pos += (movable) ? v : vec3(0,0,0);
	}

	void makeUnmovable() { movable = false; }

	void addToNormal(const Vec3& normal)
	{
		vec3 n = Normalise(normal);
		if (n.x != n.x || n.y != n.y || n.z != n.z) return;
		if (n.x == n.y && n.x == n.z && n.x == 0) return;
		accumulated_normal += n;
	}

	void addToTangent(const Vec3& normal)
	{
		vec3 n = Normalise(normal);
		if (n.x != n.x || n.y != n.y || n.z != n.z) return;
		if (n.x == n.y && n.x == n.z && n.x == 0) return;
		accumulated_tangent += n;
	}

	Vec3& getNormal() { return accumulated_normal; } // notice, the normal is not unit length

	void resetNormal() { accumulated_normal = Vec3(0, 0, 0); }

	Vertex& getVertex(Vertex& v)
	{
		v.pos = pos;
		vec3 n = Normalise(accumulated_normal);
		if (n.x != n.x || n.y != n.y || n.z != n.z) 
			return v;
		if (n.x == n.y && n.x == n.z && n.x == 0) 
			return v;
		this->accumulated_normal = n;
		
		v.nrm = n;



		return v;
	}

	f32 GetRadius()
	{
		return radius;
	}
};
#endif