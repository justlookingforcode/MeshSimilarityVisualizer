/*  Start Header -------------------------------------------------------
    File Name:      Model.h
    Author:         Tan Wen De Kevin
    Creation Date:  24/Sep/2016
    - End Header -------------------------------------------------------*/

#ifndef _MODEL_H_
#define _MODEL_H_

// ==========================
// includes
// ==========================

#include "Assimp/aiscene.h"        // Output data structure

#include "BS.h"
#include "AABB.h"
#include "Mesh.hpp"

#include "HierachicalBS.h"
#include "HierachicalAABB.h"
#include "SceneObject.h"

// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
    class Model
    {
        public:
            
            Model	(const str & t_FileName);
            Model	(Mesh & t_Mesh);
            ~Model	();

            Mesh&		    GetModelMesh();
            void            SetModel(Model & t_Model);

            const GLuint &  GetModelTexture();
            const str &     GetModelTextureStringID();
			bool			LoadModel();
            const BS &      GetBS();
            const AABB &    GetAABB();
		
			const HierachicalAABB &     GetHierachicalAABB();
			const HierachicalBS &     GetHierachicalBS();


            void            BuildSphere(Mesh & t_ModelMesh);
            void            BuildAABB(Mesh & t_ModelMesh);
            void            BuildHierachicalOBB();
			void			BuildHierachicalAABB();
			void			UpdateGPUVertexBuffer();
        private:

			void            LoadVertices(const aiMesh * t_Mesh);
			void            LoadNormal(const aiMesh * t_Mesh);
			void            LoadTangent(const aiMesh * t_Mesh);
			void            LoadTextureUV(const aiMesh * t_Mesh);
			void            LoadIndices(const aiMesh * t_Mesh);
			void            BindModelVAO();
			void            DeleteModel();

            str             m_FileName;
            bool            m_IsLoaded;


            Mesh*            m_ObjMesh;   // model mesh
            BS              m_ModelBS;   // model BS
            AABB            m_ModelAABB; // model AABB    

			HierachicalBS m_hBS;
			HierachicalAABB m_hAABB;


    };
}

#endif
