/* Start Header ------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

File Name:      Cloth.h
Purpose:        This is the Culling class implementations.
Language:       C++
Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/


#include "ClothParticleConstraint.h"
#include <vector>

#include "mesh.hpp"
#ifndef CLOTH_H
#define	CLOTH_H
#define CONSTRAINT_ITERATIONS 15 // how many iterations of constraint satisfaction each frame (more is rigid, less is soft)
#include "BS.h"
#include "HierachicalAABB.h"

#include "Collision.h"
class Cloth : public Mesh
{
private:

	u32 num_particles_width; // number of particles in "width" direction
	u32 num_particles_height; // number of particles in "height" direction
	f32 width;
	f32 height;
	// total number of particles is num_particles_width*num_particles_height

	std::vector<FluidParticle> particles; // all particles that are part of this cloth
	std::vector<ClothParticleConstraint> constraints; // alle constraints between particles as part of this cloth
	std::vector<u32> pivolts;
	HierachicalAABB hAABB;

	FluidParticle* getParticle(u32 x, u32 y) {
		u32 index = y*num_particles_width + x;
		if (index >= particles.size())
		{
			return nullptr;
		}
		return &particles[index]; 
	}

	u32 GetParticleIndex(u32 x, u32 y)
	{
		return y*num_particles_width + x;
	}


	void makeConstraint(u32 i1, u32 i2) 
	{ 
		ClothParticleConstraint c(particles, i1, i2);
		constraints.push_back(c);
	}



	/* A private method used by drawShaded() and addWindForcesForTriangle() to retrieve the
	normal vector of the triangle defined by the position of the particles p1, p2, and p3.
	The magnitude of the normal vector is equal to the area of the parallelogram defined by p1, p2 and p3
	*/
	Vec3 calcTriangleNormal(FluidParticle *p1, FluidParticle *p2, FluidParticle *p3)
	{
		Vec3 pos1 = p1->getPos();
		Vec3 pos2 = p2->getPos();
		Vec3 pos3 = p3->getPos();

		Vec3 v1 = pos2 - pos1;
		Vec3 v2 = pos3 - pos1;

		return Cross(v1,v2);
	}

	/* A private method used by windForce() to calcualte the wind force for a single triangle
	defined by p1,p2,p3*/
	void addWindForcesForTriangle(FluidParticle *p1, FluidParticle *p2, FluidParticle *p3, const Vec3 direction)
	{
		Vec3 normal = calcTriangleNormal(p1, p2, p3);
		
		Vec3 d = Normalise(normal);
		if (normal.x != normal.x || normal.y != normal.y || normal.z != normal.z) return;
		Vec3 force = normal*Dot(d,direction);
		p1->addForce(force);
		p2->addForce(force);
		p3->addForce(force);
	}



public:

	Cloth()
		: width(0)
		, height(0)
		, num_particles_width(0)
		, num_particles_height(0)

	{}
	/* This is a important constructor for the entire system of particles and constraints*/
	Cloth(float width, float height, int num_particles_width, int num_particles_height) 
		: num_particles_width(num_particles_width)
		, num_particles_height(num_particles_height)
		, width(width)
		, height(height)
	{

		particles.resize(num_particles_width*num_particles_height); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles
		f32 hWidth = width*0.5f;
		f32 hHeight = height*0.5f;
		f32 radius = glm::length(vec2((hWidth / num_particles_width) * 0.7f , (hHeight / num_particles_height) * 0.7f));
		// creating particles in a grid of particles from (0,0,0) to (width,-height,0
		for (int y = 0; y<num_particles_height; y++)
		{
			for (int x = 0; x<num_particles_width; x++)
			{



				Vec3 pos = Vec3(width * (x / (float)num_particles_width) - hWidth,
									hHeight -height * (y / (float)num_particles_height),
									0);
				particles[y*num_particles_width + x] = FluidParticle(pos, radius); // insert particle in column x at y'th row
			}
		}

		// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
		for (int y = 0; y<num_particles_height; y++)
		{
			for (int x = 0; x<num_particles_width; x++)
			{

				if (x<num_particles_width - 1) 
					makeConstraint(GetParticleIndex(x, y), GetParticleIndex(x + 1, y));
				if (y<num_particles_height - 1) 
					makeConstraint(GetParticleIndex(x, y), GetParticleIndex(x, y + 1));

				if (x < num_particles_width - 1 && y < num_particles_height - 1)
				{
					makeConstraint(GetParticleIndex(x, y), GetParticleIndex(x + 1, y + 1));
					makeConstraint(GetParticleIndex(x + 1, y), GetParticleIndex(x, y + 1));
				}
				//if (x<num_particles_width - 1 && y<num_particles_height - 1) 
			}
		}


		// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
#if 1
		for (int y = 0; y<num_particles_height; y++)
		{
			for (int x = 0; x<num_particles_width; x++)
			{



				if (x<num_particles_width - 2)
					makeConstraint(GetParticleIndex(x, y), GetParticleIndex(x + 2, y));
				if (y<num_particles_height - 2) 
					makeConstraint(GetParticleIndex(x, y), GetParticleIndex(x, y + 2));
				if (x < num_particles_width - 2 && y < num_particles_height - 2)
				{
					makeConstraint(GetParticleIndex(x, y), GetParticleIndex(x + 2, y + 2));
					makeConstraint(GetParticleIndex(x + 2, y), GetParticleIndex(x, y + 2));
				}

			}
		}
#endif

		// making the upper left most three and right most three particles unmovable
		for (int i = 0; i<3; i++)
		{
			getParticle(0 + i, 0)->offsetPos(Vec3(0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(0 + i, 0)->makeUnmovable();

			getParticle(num_particles_width - 1 +i, 0)->offsetPos(Vec3(-0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(num_particles_width - 1 + i, 0)->makeUnmovable();

			pivolts.push_back(i);
			pivolts.push_back(num_particles_width - 1 + i);

		}




		Mesh mesh = CreatePlane(num_particles_height - 1, num_particles_width - 1);
		vertexBuffer = std::move(mesh.vertexBuffer);
		indexBuffer = std::move(mesh.indexBuffer);
		IBO = mesh.IBO;
		VBO = mesh.VBO;
		VAO = mesh.VAO;
		numIndices = mesh.numIndices;
		numVertices = mesh.numVertices;
		numTris = mesh.numTris;
		UpdateMesh();


	}


	void PivoltReset1()
	{
		f32 hWidth = width*0.5f;
		f32 hHeight = height*0.5f;
		f32 radius = glm::length(vec2((hWidth / num_particles_width) * 0.7f, (hHeight / num_particles_height) * 0.7f));
		// creating particles in a grid of particles from (0,0,0) to (width,-height,0
		for (u32 y = 0; y<num_particles_height; y++)
		{
			for (u32 x = 0; x<num_particles_width; x++)
			{
				Vec3 pos = Vec3(width * (x / (float)num_particles_width) - hWidth,
					hHeight - height * (y / (float)num_particles_height),
					0);
				particles[y*num_particles_width + x] = FluidParticle(pos, radius); // insert particle in column x at y'th row
			}
		}

		// making the upper left most three and right most three particles unmovable
		pivolts.clear();
		for (u32 i = 0; i<3; i++)
		{
			getParticle(0 + i, 0)->offsetPos(Vec3(0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(0 + i, 0)->makeUnmovable();

			getParticle(num_particles_width - 1 + i, 0)->offsetPos(Vec3(-0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(num_particles_width - 1 + i, 0)->makeUnmovable();

			pivolts.push_back(i);
			pivolts.push_back(num_particles_width - 1 + i);

		}

	}

	void PivoltReset2()
	{
		f32 hWidth = width*0.5f;
		f32 hHeight = height*0.5f;
		f32 radius = glm::length(vec2((hWidth / num_particles_width) * 0.7f, (hHeight / num_particles_height) * 0.7f));
		// creating particles in a grid of particles from (0,0,0) to (width,-height,0
		for (u32 y = 0; y<num_particles_height; y++)
		{
			for (u32 x = 0; x<num_particles_width; x++)
			{



				Vec3 pos = Vec3(width * (x / (float)num_particles_width) - hWidth,
					hHeight - height * (y / (float)num_particles_height),
					0);
				particles[y*num_particles_width + x] = FluidParticle(pos, radius); // insert particle in column x at y'th row
			}
		}

		// making the upper left most three and right most three particles unmovable
		pivolts.clear();
		for (int i = 0; i<3; i++)
		{
			getParticle(0 + i, 0)->offsetPos(Vec3(0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(0 + i, 0)->makeUnmovable();

			//getParticle(num_particles_width - 1 + i, 0)->offsetPos(Vec3(-0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			//getParticle(num_particles_width - 1 + i, 0)->makeUnmovable();

			pivolts.push_back(i);
			//pivolts.push_back(num_particles_width - 1 + i);

		}

	}

	void PivoltReset3()
	{
		f32 hWidth = width*0.5f;
		f32 hHeight = height*0.5f;
		f32 radius = glm::length(vec2((hWidth / num_particles_width) * 0.7f, (hHeight / num_particles_height) * 0.7f));
		// creating particles in a grid of particles from (0,0,0) to (width,-height,0
		for (u32  y = 0; y<num_particles_height; y++)
		{
			for (u32 x = 0; x<num_particles_width; x++)
			{



				Vec3 pos = Vec3(width * (x / (float)num_particles_width) - hWidth,
					hHeight - height * (y / (float)num_particles_height),
					0);
				particles[y*num_particles_width + x] = FluidParticle(pos, radius); // insert particle in column x at y'th row
			}
		}

		pivolts.clear();
		// making the upper left most three and right most three particles unmovable
		for (u32 i = 0; i<3; i++)
		{

			getParticle(num_particles_width / 2 + i, num_particles_height / 2 + i)->offsetPos(Vec3(0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			getParticle(num_particles_width / 2 + i, num_particles_height / 2 + i)->makeUnmovable();

			//getParticle(num_particles_width - 1 + i, 0)->offsetPos(Vec3(-0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
			//getParticle(num_particles_width - 1 + i, 0)->makeUnmovable();
			u32 index = (num_particles_height / 2 + i) * num_particles_width + num_particles_width / 2 + i;
			pivolts.push_back(index);
			//pivolts.push_back(num_particles_width - 1 + i);

		}

	}

	void RebuildBV()
	{
		hAABB.BuildFromModel(vertexBuffer, indexBuffer, 8);
	}
	/* drawing the cloth as a smooth shaded (and colored according to column) OpenGL triangular mesh
	Called from the display() method
	The cloth is seen as consisting of triangles for four particles in the grid as follows:

	(x,y)   *--* (x+1,y)
	| /|
	|/ |
	(x,y+1) *--* (x+1,y+1)

	*/
	void UpdateMesh()
	{
		// reset normals (which where written to last frame)
		std::vector<FluidParticle>::iterator particle;
		u32 total = particles.size();
		for (u32 i = 0; i < total; ++i)
		{
			(particles[i]).resetNormal();
		}

		//create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
		for (u32 y = 0; y<num_particles_height -1; y++)
		{
			for (u32 x = 0; x<num_particles_width -1; x++)
			{

				Vec3 normal = calcTriangleNormal(
					getParticle(x + 1, y), 
					getParticle(x, y), 
					getParticle(x, y + 1));
				if (normal.x != normal.x || normal.y != normal.y || normal.z != normal.z) ;
				else{
					getParticle(x + 1, y)->addToNormal(normal);
					getParticle(x, y)->addToNormal(normal);
					getParticle(x, y + 1)->addToNormal(normal);
				}
				normal = calcTriangleNormal(
					getParticle(x + 1, y + 1), 
					getParticle(x + 1, y), 
					getParticle(x, y + 1));
				if (normal.x != normal.x || normal.y != normal.y || normal.z != normal.z);
				else
				{
					getParticle(x + 1, y + 1)->addToNormal(normal);
					getParticle(x + 1, y)->addToNormal(normal);
					getParticle(x, y + 1)->addToNormal(normal);
				}
			}
		}
		//<! update the positions and normals within the vertex buffer
		
		//mesh.vertexBuffer.resize(num_particles_width * num_particles_height);

		for (u32 y = 0; y < num_particles_height; y++)
		{
			for (u32 x = 0; x < num_particles_width; x++)
			{
				Vertex& v = vertexBuffer[y * num_particles_width + x];
				FluidParticle* p = getParticle(x, y);
				p->getVertex(v);

				vertexBuffer[y * num_particles_width + x] = v;

			}
		}

		//mesh.UpdateGPUVertexBuffer();


	}

	/* this is an important methods where the time is progressed one time step for the entire cloth.
	This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
	*/
	void timeStep(f32 dt)
	{
		u32 total;
#if 1

		total = constraints.size();
		for (int i = 0; i<CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
		{
			for (u32 j = 0; j < total; ++j)
			{
				ClothParticleConstraint& c = constraints[j];
				c.satisfyConstraint(particles); // satisfy constraint.
			}
		}
#endif
#if 1

		total = particles.size();
		for (u32 i = 0; i < total; ++i)
		{
			(particles[i]).timeStep(dt); // calculate the position of each particle at the next time step.
			particles[i].isCollided = false;
		}
#endif
	}



	/* used to add gravity (or any other arbitrary vector) to all particles*/
	void addForce(const Vec3& direction)
	{
		std::vector<FluidParticle>::iterator particle;
		u32 total = particles.size();
		for (u32 i = 0; i < total; ++i)
		{
			particles[i].addForce(direction); // add the forces to each particle
		}

	}

	/* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
	void windForce(const Vec3 direction)
	{
		for (u32 y = 0; y<num_particles_height - 1; y++)
		{
			for (u32 x = 0; x<num_particles_width - 1; x++)
			{

				addWindForcesForTriangle(	getParticle(x + 1, y), 
											getParticle(x, y), 
											getParticle(x, y + 1), 
											direction);
				addWindForcesForTriangle(	getParticle(x + 1, y + 1), 
											getParticle(x + 1, y), 
											getParticle(x, y + 1), direction);
			}
		}
	}

	/* used to detect and resolve the collision of the cloth with the ball.
	This is based on a very simples scheme where the position of each particle is simply compared to the sphere and corrected.
	This also means that the sphere can "slip through" if the ball is small enough compared to the distance in the grid bewteen particles
	*/
	void ballCollision(const Proto::BS& bs)
	{

		auto a = [&](const HierachicalAABBNode& a)->void
		{
			if (a.m_LeftChild == -1 && a.m_RightChild== - 1)
			{
				f32 buffer(1.f);
	
				f32 d = (bs.m_Radius) * (bs.m_Radius);
				u32 total = a.indices.size();

				for (u32 i = 0; i < total; ++i)
				{
					u32 index = a.indices[i];
					Vec3 v = particles[index].getPos() - bs.m_Center;
					f32 l2 = Dot(v, v);
					if (l2 < d)
					{
						f32 l = glm::length(v);
						vec3 vOff = Normalise(v)*(bs.m_Radius - l);
						particles[index].offsetPos(vOff);
					}
				}
			}
		};

		auto b = [&](const HierachicalAABBNode& a)->bool
		{
			bool flag = SphereAABBCollision(bs, a.m_AABB);
			return flag;
		};

		hAABB.VisitNodes(a, b);

	}





	void Cloth::cubeCollision(const Proto::AABB& aabb)
	{
		Vec3 halfwidth = aabb.m_Radius * 1.1f ;
		Vec3 minPt(aabb.m_Center - halfwidth);
		Vec3 maxPt(aabb.m_Center + halfwidth);

		auto a = [&](const HierachicalAABBNode& a)->void
		{
			if (a.m_LeftChild == -1 && a.m_RightChild == -1)
			{
				u32 total = a.indices.size();
				for (u32 i = 0; i < total; ++i)
				{
					u32 index = a.indices[i];
					FluidParticle& ptc = particles[index];
					Vec3& pos = ptc.getPos();
					if (pos.x > minPt.x && pos.x < maxPt.x &&
						pos.y > minPt.y && pos.y < maxPt.y &&
						pos.z > minPt.z && pos.z < maxPt.z) // if the particle is inside the aabb
					{
						Vec3 v = pos - aabb.m_Center;
						float closestaxis = (std::max)((std::max)(std::fabs(v.x), std::fabs(v.y)), std::fabs(v.z));
						//Vec3 ints = Normalize(v) * radius;
						float ratio = (halfwidth.x - closestaxis) / closestaxis;
						//float r = Length(radius);

						particles[index].isCollided = true;
						particles[index].offsetPos(v * ratio);
					}
				}
			}
		};

		auto b = [&](const HierachicalAABBNode& a)->bool
		{
			bool flag = AABBAABBCollision(aabb, a.m_AABB);
			return flag;
		};
		hAABB.VisitNodes(a, b);

	}


	void SelfCollision()
	{


	u32 total = particles.size();
		u32 j;
		for (j = 0; j < total; j++)
		{
			FluidParticle& ptc1 = particles[j];
			Proto::BS bs;
			bs.m_Center = ptc1.getPos();
			bs.m_Radius = ptc1.GetRadius();
			auto a = [&](const HierachicalAABBNode& a)->void
			{
				if (a.m_LeftChild == -1 && a.m_RightChild == -1)
				{
					u32 total = a.indices.size();
					for (u32 i = 0; i < total; ++i)
					{
						u32 index2 = a.indices[i];
						if (index2 == j)
							continue;
						FluidParticle& ptc2 = particles[index2];

						Vec3 ptc1pos = ptc1.getPos();
						Vec3 ptc2pos = ptc2.getPos();

						float length = glm::length(ptc1pos - ptc2pos);
						float totalrad = ptc1.GetRadius() + ptc2.GetRadius();

						if (length < totalrad)
						{
							float ratio = (totalrad - length) / length * 0.5f;

							Vec3 offset((ptc1pos - ptc2pos) * ratio);

							ptc1.isCollided = true;
							ptc2.isCollided = true;

							ptc1.offsetPos(offset);
							ptc2.offsetPos(-offset);
						}
					}
				}
			};

			auto b = [&](const HierachicalAABBNode& a)->bool
			{
				bool flag = SphereAABBCollision(bs, a.m_AABB);
				return flag;
			};
			hAABB.VisitNodes(a, b);
		}

	}

	void moveLeft()
	{
		u32 total = pivolts.size();
		for (u32 i = 0; i < total; ++i)
		{
			particles[pivolts[i]].getPos().x -= 1.f;
		}
	}

	void moveRight()
	{
		u32 total = pivolts.size();
		for (u32 i = 0; i < total; ++i)
		{
			particles[pivolts[i]].getPos().x += 1.f;
		}
	}

	void moveForward()
	{
		u32 total = pivolts.size();
		for (u32 i = 0; i < total; ++i)
		{
			particles[pivolts[i]].getPos().z -= 1.f;
		}
	}


	void moveBackward()
	{
		u32 total = pivolts.size();
		for (u32 i = 0; i < total; ++i)
		{
			particles[pivolts[i]].getPos().z += 1.f;
		}
	}


	void moveUp()
	{
		u32 total = pivolts.size();
		for (u32 i = 0; i < total; ++i)
		{
			particles[pivolts[i]].getPos().y += 1.f;
		}
	}

	void moveDown()
	{
		u32 total = pivolts.size();
		for (u32 i = 0; i < total; ++i)
		{
			particles[pivolts[i]].getPos().y -= 1.f;
		}
	
	}
};

#endif