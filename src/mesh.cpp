/******************************************************************************/
/*!
\file   mesh.cpp
\par    Purpose: Library for generating polygonal meshes.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   28/3/2015
*/
/******************************************************************************/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "math.hpp"
#include "mesh.hpp"
#include "Assimp/aiscene.h"        // Output data structure
#include "Assimp/assimp.hpp"
#include "Assimp/aipostprocess.h"
#include <iostream>
#include <fstream> //file io stream

/*  Function prototype(s) */
void BuildIndexBuffer(int stacks, int slices, Mesh &mesh);
void addVertex(Mesh &mesh, const Vertex &v);
void addIndex(Mesh &mesh, int index);
void ComputeTangentsBitangents(VertexBufferType &vertices, const IndexBufferType &indices);


/******************************************************************************/
/*!
\fn     Mesh CreatePlane(int stacks, int slices)
\brief
        Create a plane of dimension 1*1. 
        The plane x/y-coordinates range from -0.5 to 0.5.
        The plane z-coordinate is 0.
        The normal of the plane is z-vector.
        The uv of each vertex on the plane should allow repeating texture,
        instead of being capped between 0 and 1.
\param  stacks
        Number of stacks on the plane
\param  slices
        Number of slices on the plane
\return
        The generated plane
*/
/******************************************************************************/
Mesh CreatePlane(int stacks, int slices)
{
    Mesh mesh;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            Vertex v;

            v.pos = Vec3(col - 0.5f, 0.5f - row, 0.0f);
            v.nrm = Vec3(0.0f, 0.0f, 1.0f);
			v.uv = Vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);
    ComputeTangentsBitangents(mesh.vertexBuffer, mesh.indexBuffer);

    return mesh;
}


/******************************************************************************/
/*!
\fn     Mesh CreateCube(int height, int length, int width)
\brief
        Create a cube of dimension 1*1*1 that is subdivided into
        "height" stacks, "length" slices along the x-axis and
        "width" slices along the z-axis.
        The x/y/z-coordinates still range from -0.5 to 0.5.
\param  height
        Number of stacks on the cube
\param  length
        Number of slices along the x-axis of the cube
\param  width
        Number of slices along the z-axis of the cube
\return
        The generated cube
*/
/******************************************************************************/
Mesh CreateCube(int length, int height, int width)
{
    /*  Initial planes and their transformations to form the cube faces */
    Mesh planeMesh[3];
    planeMesh[0] = CreatePlane(height, length);     /*  XY-plane, used for +Z and -Z faces */
    planeMesh[1] = CreatePlane(height, width);      /*  YZ-plane, used for +X and -X faces */
    planeMesh[2] = CreatePlane(width, length);      /*  XZ-plane, used for +Y and -Y faces */
    Mesh mesh;

    Vec3 const translateArray[] =
    {
        Vec3(+0.0f, +0.0f, +0.5f), // Z+
        Vec3(+0.0f, +0.0f, -0.5f), // Z-
        Vec3(+0.5f, +0.0f, +0.0f), // X+
        Vec3(-0.5f, +0.0f, +0.0f), // X-
        Vec3(+0.0f, +0.5f, +0.0f), // Y+
        Vec3(+0.0f, -0.5f, +0.0f), // Y-
    };

    Vec2 const rotateArray[] =
    {
        Vec2(+0.0f, +0.0f),        // Z+
        Vec2(+0.0f, +PI),          // Z-
        Vec2(+0.0f, +HALF_PI),     // X+
        Vec2(+0.0f, -HALF_PI),     // X-
        Vec2(-HALF_PI, +0.0f),     // Y+
        Vec2(+HALF_PI, +0.0f)      // Y-
    };


    /*  Transform the initial planes and use their transformed vertices to create vertices of the cube */
    /*  The vertices should be pushed into vertexBuffer while their indices are pushed into indexBuffer */
    int numIndices = 0;
    for (int i = 0; i < 6; i++)
    {
        Mat4 rotateXMat = Rotate(rotateArray[i][X], BASIS[X]);
        Mat4 rotateYMat = Rotate(rotateArray[i][Y], BASIS[Y]);
        Mat4 translateMat = Translate(translateArray[i]);

        Mat4 transformMat = translateMat * rotateYMat * rotateXMat;

        int planeIndex = i / 2;
        for (int j = 0; j < planeMesh[planeIndex].numVertices; j++)
        {
            Vertex v;
            v.pos = Vec3(transformMat * Vec4(planeMesh[planeIndex].vertexBuffer[j].pos, 1.0f));
            v.nrm = Vec3(transformMat * Vec4(planeMesh[planeIndex].vertexBuffer[j].nrm, 0.0f));
            v.uv = planeMesh[planeIndex].vertexBuffer[j].uv;

            mesh.vertexBuffer.push_back(v);
        }

        for (int j = 0; j < planeMesh[planeIndex].numIndices; j++)
        {
            mesh.indexBuffer.push_back(planeMesh[planeIndex].indexBuffer[j] + numIndices);
        }
        numIndices += planeMesh[planeIndex].numVertices;
    }

    ComputeTangentsBitangents(mesh.vertexBuffer, mesh.indexBuffer);

    /*  Compute the number of vertices, indices and triangles */
    mesh.numVertices = mesh.vertexBuffer.size();
    mesh.numIndices = mesh.indexBuffer.size();
    mesh.numTris = mesh.numIndices / 3;

    return mesh;
}


/******************************************************************************/
/*!
\fn     Mesh CreateSphere(int stacks, int slices)
\brief
        Create a sphere of radius 0.5.
        The center of the sphere is (0, 0, 0).
\param  stacks
        Number of stacks on the sphere
\param  slices
        Number of slices on the sphere
\return
        The generated sphere
*/
/******************************************************************************/
Mesh CreateSphere(int stacks, int slices)
{
    Mesh mesh;
    
    float radius = 0.5f;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;
        float beta = (float)HALF_PI - row * (float)PI;

        float cos_b = cosf(beta);
        float sin_b = sinf(beta);

        /*  Don't use a single vertex to store the two poles because we need multiple uv coords */
        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;
            float alpha = col * (float)TWO_PI;

            float cos_a = cosf(alpha);
            float sin_a = sinf(alpha);

            Vertex v;

            v.pos = Vec3(radius * cos_b * sin_a, radius * sin_b, radius * cos_b * cos_a);
            v.nrm = Normalize(Vec3(v.pos[0], v.pos[1], v.pos[2]));
            v.uv = Vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);
    ComputeTangentsBitangents(mesh.vertexBuffer, mesh.indexBuffer);

    return mesh;
}


/******************************************************************************/
/*!
\fn     void BuildIndexBuffer(int stacks, int slices, Mesh &mesh)
\brief
        Generate the index buffer for the mesh.
\param  stacks
        Number of stacks on the mesh
\param  slices
        Number of slices on the mesh
\param  mesh
        The mesh whose index buffer will be generated.
*/
/******************************************************************************/
void BuildIndexBuffer(int stacks, int slices, Mesh &mesh)
{
    int row0, row1, i0, i1, i2;
    int stride = slices + 1;    /*  We count both the first and last vertices since they have different uvs */

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            row0 = i * stride;
            row1 = row0 + stride;

            /*  First triangle */
            i0 = row0 + j;
            i1 = row1 + j;
            i2 = row0 + j + 1;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri( mesh.vertexBuffer[i0].pos, 
                                mesh.vertexBuffer[i1].pos, 
                                mesh.vertexBuffer[i2].pos))
            {
                addIndex(mesh, i0);
                addIndex(mesh, i1);
                addIndex(mesh, i2);
            }


            /*  Second triangle */
            i0 = row0 + j + 1;
            i1 = row1 + j;
            i2 = row1 + j + 1;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri( mesh.vertexBuffer[i0].pos,
                                mesh.vertexBuffer[i1].pos,
                                mesh.vertexBuffer[i2].pos))
            {
                addIndex(mesh, i0);
                addIndex(mesh, i1);
                addIndex(mesh, i2);
            }
        }
    }
}


/******************************************************************************/
/*!
\fn     void addVertex(Mesh &mesh, const Vertex &v)
\brief
        Add a vertex to a mesh
\param  mesh
        The mesh to be updated.
\param  v
        The vertex to be added.
*/
/******************************************************************************/
void addVertex(Mesh &mesh, const Vertex &v)
{
    mesh.vertexBuffer.push_back(v);
    ++mesh.numVertices;
}


/******************************************************************************/
/*!
\fn     void addIndex(Mesh &mesh, int index)
\brief
        Add an index to a mesh
\param  mesh
        The mesh to be updated.
\param  index
        The vertex index to be added.
*/
/******************************************************************************/
void addIndex(Mesh &mesh, int index)
{
    mesh.indexBuffer.push_back(index);
    ++mesh.numIndices;

    if (mesh.numIndices % 3 == 0)
        ++mesh.numTris;
}


/******************************************************************************/
/*!
\fn     void ComputeTangentsBitangents(VertexBufferType &vertices, const IndexBufferType &indices)
\brief
        Compute the tangent and bitangent at each vertex of the mesh
\param  vertices
        The vertex list of the mesh
\param  indices
        The index list of the mesh
*/
/******************************************************************************/
void ComputeTangentsBitangents(VertexBufferType &vertices, const IndexBufferType &indices)
{
    int numVertices = vertices.size();
    int numIndices = indices.size();

    int *AverageCounter = new int[numVertices];

    std::memset(AverageCounter, 0, numVertices*sizeof(int));

    for (int index = 0; index < numIndices; index += 3)
    {
        int p0 = indices[index + 0];
        int p1 = indices[index + 1];
        int p2 = indices[index + 2];

        Vertex &A = vertices[p0];
        Vertex &B = vertices[p1];
        Vertex &C = vertices[p2];

        // Get our two vectors across the triangles
        Vec3 P = B.pos - A.pos;
        Vec3 Q = C.pos - A.pos;

        // Get our texture components of those vectors
        Vec2 Puv = B.uv - A.uv;
        Vec2 Quv = C.uv - A.uv;

        float texCoord = Puv.x * Quv.y - Quv.x * Puv.y;

        // This would handle the case of a small (possibly zero)
        // reciprocal, just skip this vertex, Tan and Bin aren't incremented
        // and the counter isn't incremented.
        if (texCoord < EPSILON && texCoord > -EPSILON)
            continue;

        float reciprocal = 1.0f / texCoord;

        Vec3 Tangent((P * Quv.y - Q * Puv.y) * reciprocal);
        Vec3 Bitangent((Q * Puv.x - P * Quv.x) * reciprocal);

        A.tan += Tangent;
        B.tan += Tangent;
        C.tan += Tangent;

        A.bitan += Bitangent;
        B.bitan += Bitangent;
        C.bitan += Bitangent;

        ++AverageCounter[p0];
        ++AverageCounter[p1];
        ++AverageCounter[p2];
    }

    // For each vertex, divide by the number of triangles shared by it
    for (int vert = 0; vert < numVertices; ++vert)
    {
        if (AverageCounter[vert])
        {
            vertices[vert].tan /= static_cast<float>(AverageCounter[vert]);
            vertices[vert].bitan /= static_cast<float>(AverageCounter[vert]);
        }
    }

    delete[] AverageCounter;
}

//@MSMS:TODO http://ogldev.atspace.co.uk/www/tutorial22/tutorial22.html
bool Mesh::LoadModelFromFile(const str & path)
{	
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(path.c_str(), aiProcessPreset_TargetRealtime_Quality);

	if (pScene == nullptr)
	{
        std::cout << "Error parsing file " << path << " : " << Importer.GetErrorString() << std::endl;
		return false;
	}

    vertexBuffer.resize(pScene->mNumMeshes * 3);
    indexBuffer.resize(pScene->mNumMeshes * 3);

    //loop per mesh
    for (int i = 0; i < pScene->mNumMeshes; ++i)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];

        //per vertex
        LoadVertices(paiMesh);

        //per index
        LoadIndices(paiMesh);
    }

	//Update GPU
	UpdateGPUVertexBuffer();

	return true;
}

void Mesh::UpdateGPUVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBuffer.size() * sizeof(vertexBuffer[0]), &vertexBuffer[0]);
	//glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(vertexBuffer[0]), &vertexBuffer[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//@MSMS:TODO
void Mesh::LoadVertices(const aiMesh * t_Mesh)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (int j = 0; j < t_Mesh->mNumVertices; ++j)
    {
        const aiVector3D* pPos = &(t_Mesh->mVertices[j]);
        const aiVector3D* pNormal = &(t_Mesh->mNormals[j]);
        const aiVector3D* pTexCoord = t_Mesh->HasTextureCoords(0) ? &(t_Mesh->mTextureCoords[0][j]) : &Zero3D;
        const aiVector3D* ptan = &Zero3D;
        const aiVector3D* pbitan = &Zero3D;
        if (t_Mesh->HasTangentsAndBitangents())
        {
            ptan = t_Mesh->mTangents;
            pbitan = t_Mesh->mBitangents;
        }
        Vertex v(
            Vec3(pPos->x, pPos->y, pPos->z),            //pos
            Vec3(pNormal->x, pNormal->y, pNormal->z),   //normal
            Vec3(ptan->x, ptan->y, ptan->z),            //tangent
            Vec3(pbitan->x, pbitan->y, pbitan->z),      //bitangent
            Vec2(pTexCoord->x, pTexCoord->y)            //tex,uv
            );

        vertexBuffer.push_back(v);
    }
	numVertices = t_Mesh->mNumVertices;
	numTris = t_Mesh->mNumFaces;

}
//@MSMS:TODO
void Mesh::LoadNormal(const aiMesh * t_Mesh)
{
}
//@MSMS:TODO
void Mesh::LoadTangent(const aiMesh * t_Mesh)
{
}
//@MSMS:TODO
void Mesh::LoadTextureUV(const aiMesh * t_Mesh)
{
}
//@MSMS:TODO
void Mesh::LoadIndices(const aiMesh * t_Mesh)
{
    for (int k = 0; k < t_Mesh->mNumFaces; ++k)
    {
        const aiFace& face = t_Mesh->mFaces[k];
        if (face.mNumIndices == 3)
        {
            indexBuffer.push_back(face.mIndices[0]);
            indexBuffer.push_back(face.mIndices[1]);
            indexBuffer.push_back(face.mIndices[2]);
        }
    }
	numIndices = indexBuffer.size();
}
