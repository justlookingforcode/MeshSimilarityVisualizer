/*  Start Header -------------------------------------------------------
File Name:      GFXComponent.h
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/

#ifndef GFXCOMPONENT_H_
#define GFXCOMPONENT_H_

// ==========================
// includes
// ==========================


#include "ModelManager.h"
#include "AABB.h"
#include "HierachicalAABB.h"
#include "BS.h"
#include "HierachicalBS.h"

// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
	class Model;


    class GFXComponent
    {
    public:

        GFXComponent();
        ~GFXComponent();

        void				Init(const str &    t_ModelID);

        void				Update( mat4         & t_MatResult,
									const vec3   & t_Pos,
									const vec3   & t_RotVec,
									const vec3   & t_ScaleVec);

        void				Draw(	const mat4 &  t_VMatrix, 
									const mat4 &  t_MWMatrix ,
									bool wireframe = false);
		void				DrawDebugSphere(const mat4 &  t_VMatrix);
		void				DrawDebugAABB(const mat4 &  t_VMatrix);

		void				DrawDebugHierachicalAABB(const mat4 &  t_VMatrix);

		void				UpdateWorldSpaceBoundingSphere(const vec3 &  t_translationVec, const vec3& t_ScaleVec, const vec3& t_RotVec, BS& t_worldSpaceBS);
		void				UpdateWorldSpaceAABB(const mat4 &  t_MWMatrix, AABB& t_worldSpaceAABB);
		void				UpdateWorldSpaceHierachicalAABB(const mat4 &  t_MWMatrix, HierachicalAABB& t_hAABB);

		void				SetColorTexture(s32 colorTexID);
		void				SetNormalMapTexture(s32 colorTexID);
		void				SetNormalMapTexture2(s32 colorTexID);
		void				SetDefaultColor(const vec3& c);

        HierachicalAABB&	GetHAABB();

		HierachicalAABB&	GetWorldSpaceHAABB();
		const AABB&			GetWorldSpaceAABB();
		const BS&			GetWorldSpaceBS();

        Model *             GetModel();
        void                SetModel(const str & t_ModelID);
        const str &			GetModelID();

        void				setShaderProgramID(u32 id);
        void                UpdateModelWorldMatrix( mat4       & t_MatResult,
													const vec3 & t_Pos,
													const vec3 & t_RotVec,
													const vec3 & t_ScaleVec);

    private:

        Model *				m_Model;
        mat4                m_MWMatrix;
        str					m_ModelID;
		BS					m_WorldSpaceBS;
		AABB				m_WorldSpaceAABB;
		HierachicalBS		m_WorldSpaceHierachicalBS;
		HierachicalAABB		m_WorldSpaceHierachicalAABB;
		u32					m_RenderedTreeDepth;
		s32 colorTexID;
		s32 normalTexID;
		s32 normalTexID2;
		vec3 color;


    };
}

#endif
