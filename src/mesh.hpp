/******************************************************************************/
/*!
\file   mesh.hpp
\par    Purpose: Library for generating polygonal meshes.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   28/3/2015
*/
/******************************************************************************/

#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "gl/gl3w.h"
#include "math.hpp"

#include "defines.h"

/*  Vertex format */
struct aiMesh;
struct Vertex
{
    Vertex(Vec3 pos, Vec3 nrm, Vec3 tan, Vec3 bitan, Vec2 uv, vec2 heatmap = Vec2(0,0)) 
		: pos(pos)
		, nrm(nrm)
		, tan(tan)
		, bitan(bitan)
		, uv(uv)
		, heatmap(heatmap)
	{}

	Vertex() : pos(Vec3()), nrm(Vec3()), tan(Vec3()), bitan(Vec3()), uv(Vec2()), heatmap(Vec2(0,0)) { }

    Vec3 pos, nrm, tan, bitan;
    Vec2 uv, heatmap;
};

const int vertexSize = sizeof(Vertex);
const int indexSize = sizeof(int);


/*  The layouts for specifying the offsets of a vertex 
    when it is copied into the graphics pipeline. */
struct VertexLayout
{
    int location;
    int size;
    int type;
    bool normalized;
    int offset;
};



const VertexLayout vLayout[] =
{
    { 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos) },
    { 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm) },
    { 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tan) },
    { 3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, bitan)},
    { 4, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv) },
	{ 5, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, heatmap) }
};

const int layoutSize = sizeof(VertexLayout);
const int numAttribs = sizeof(vLayout) / layoutSize;


typedef std::vector<Vertex> VertexBufferType;
typedef std::vector<int> IndexBufferType;

/*  Mesh format, only contains geometric data but not color/texture */
struct Mesh
{
    Mesh() 
		: numVertices(0)
		, numTris(0)
		, numIndices(0)
    {
        vertexBuffer.clear(); 
        indexBuffer.clear();
    }


	virtual ~Mesh(){}
    /*  Storing the actual vertex/index data */
    VertexBufferType vertexBuffer;
    IndexBufferType indexBuffer;

    int numVertices;
    int numTris;
    int numIndices;

    /*  Once the buffer data has been copied into the pipeline, these array object and 
        buffer objects will store  its "states".
        Later on if we want to render a mesh, we just need to bind the VAO. 
        We don't need to copy the buffer data again.
    */
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

    //type of mesh
    enMeshType enMT;

    bool LoadModelFromFile(const str& path);
	void UpdateGPUVertexBuffer();
private:
    void            LoadVertices(const aiMesh * t_Mesh);
    void            LoadNormal(const aiMesh * t_Mesh);
    void            LoadTangent(const aiMesh * t_Mesh);
    void            LoadTextureUV(const aiMesh * t_Mesh);
    void            LoadIndices(const aiMesh * t_Mesh);
};


/*  Mesh function(s) */
Mesh CreatePlane(int stacks, int slices);
Mesh CreateCube(int length, int height, int width);
Mesh CreateSphere(int stacks, int slices);


#endif