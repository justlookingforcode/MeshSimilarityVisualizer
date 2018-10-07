/*  Start Header -------------------------------------------------------
File Name:      GFXComponent.cpp
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/

#include "GFXComponent.h"

#include "graphics.hpp"
#include <algorithm>
namespace Proto
{
    /*************************************************************************/
    /*!
    \fn GFXComponent::GFXComponent()

    \brief
    This is the default constructor for the GFXComponent class.
	*/
    /*************************************************************************/
	GFXComponent::GFXComponent() 
		: m_Model(nullptr)
		, colorTexID(-1)
		, normalTexID(-1)
		, normalTexID2(-1)
		, m_RenderedTreeDepth(0)
    {}

    /*************************************************************************/
    /*!
    \fn GFXComponent::~GFXComponent()

    \brief
    This is the default destructor for the GFXComponent class.
    */
    /*************************************************************************/
    GFXComponent::~GFXComponent()
    {}

    /*************************************************************************/
    /*!
    \fn void GFXComponent::Init(const str & t_ModelID,
    Model *     t_Model,
    const str & t_TextureID)

    \brief
    This function initialises the graphics component object.

    \param t_ModelID
    This is the model object pointer.

    \param t_Model
    This is the texture ID.

    \param t_TextureID
    This is the texture ID.
    */
    /*************************************************************************/
    void GFXComponent::Init(const str & t_ModelID)
    {


        m_ModelID = t_ModelID;
        m_Model = ModelManager::GetInstance().GetModel(t_ModelID);

		this->m_WorldSpaceHierachicalAABB = m_Model->GetHierachicalAABB();



    }


	void GFXComponent::UpdateWorldSpaceBoundingSphere(const vec3 &  t_translationVec, const vec3& t_ScaleVec, const vec3& t_RotVec, BS& t_worldSpaceBS)
	{
		f32 scale = std::max(std::max(t_ScaleVec.x, t_ScaleVec.y), t_ScaleVec.z);
		t_worldSpaceBS.m_Radius = m_Model->GetBS().m_Radius * scale;
		mat4 t_RotMatrix = RotationMatrix(t_RotVec.x, t_RotVec.y, t_RotVec.z);
		mat4 sphereCenterTransform = t_RotMatrix * ScaleMatrix(t_ScaleVec);
		sphereCenterTransform[3][0] = t_translationVec.x;
		sphereCenterTransform[3][1] = t_translationVec.y;
		sphereCenterTransform[3][2] = t_translationVec.z;
		t_worldSpaceBS.m_Center = vec3(sphereCenterTransform * vec4(m_Model->GetBS().m_Center, 1));

	}


	void GFXComponent::UpdateWorldSpaceAABB(const mat4 &  t_MWMatrix, AABB& t_worldSpaceAABB)
	{
		t_worldSpaceAABB.UpdateAABB(t_MWMatrix, m_Model->GetAABB());
	}




	void GFXComponent::UpdateWorldSpaceHierachicalAABB(const mat4 &  t_MWMatrix, HierachicalAABB& t_hAABB)
	{
		auto& src(m_Model->GetModelMesh());
		//copy out vertex buffer
		auto vertices = src.vertexBuffer;
		u32 total = vertices.size();
		//transform vertices into world space
		for (u32 i = 0; i < total; ++i)
		{
			auto& vertex = vertices[i];
			vertex.pos = Vec3(t_MWMatrix * Vec4(vertex.pos, 1.f));
		}
		HierachicalAABB hAABB;
		t_hAABB = hAABB;
		t_hAABB.BuildFromModel(vertices, src.indexBuffer, t_hAABB.getMaxDepth());
		
	}

    /*************************************************************************/
    /*!
    \fn void GFXComponent::Update(mat4       & t_MatResult,
    const vec3 & t_Pos,
    const vec3 & t_RotVec,
    const vec3 & t_ScaleVec)

    \brief
    This function performs the updating of the graphics component object.

    \param t_MatResult
    This is the matrix result.

    \param t_Pos
    This is the object position.

    \param t_RotVec
    This is the object rotation vector.

    \param t_ScaleVec
    This is the object scaling vector.
    */
    /*************************************************************************/
    void GFXComponent::Update(mat4 & t_MatResult,
        const vec3 & t_Pos,
        const vec3 & t_RotVec,
        const vec3 & t_ScaleVec)
    {
        UpdateModelWorldMatrix(t_MatResult, t_Pos, t_RotVec, t_ScaleVec);
		UpdateWorldSpaceBoundingSphere(t_Pos, t_ScaleVec, t_RotVec, this->m_WorldSpaceBS);
		UpdateWorldSpaceAABB(t_MatResult, this->m_WorldSpaceAABB);


		UpdateWorldSpaceHierachicalAABB(t_MatResult, this->m_WorldSpaceHierachicalAABB);

    }



	const BS& GFXComponent::GetWorldSpaceBS()
	{
		return m_WorldSpaceBS;
	}


	const AABB& GFXComponent::GetWorldSpaceAABB()
	{
		return m_WorldSpaceAABB;
	}

    /*************************************************************************/
    /*!
    \fn void GFXComponent::Draw(const mat4 & t_MWMatrix)

    \brief
    This function performs the drawing of the object.

    \param t_MWMatrix
    This is the model-world matrix.
    */
    /*************************************************************************/
    void GFXComponent::Draw(const mat4& t_VMatrix, const mat4 &  t_MWMatrix, bool wireframe)
    {

        mat4 MVMat, normalMVMat;
        ComputeObjMVMat(MVMat, normalMVMat, t_VMatrix, t_MWMatrix);

        SendMVMat(MVMat, normalMVMat, mainMVMatLoc, mainNMVMatLoc);

		if (wireframe || colorTexID == -1)
		{
			glUniform1i(textureOnLoc, false);
		}
		else
		{
			glUniform1i(textureOnLoc, true);
			SendObjTexID(colorTexID, ActiveTexID::COLOR, textureLoc);
		}

        RenderMeshObj(this->m_Model->GetModelMesh(), wireframe);
    }


	void GFXComponent::DrawDebugSphere(const mat4 &  t_VMatrix)
	{

		mat4 MVMat, normalMVMat, t_SphereMWMatrix, t_SphereTransform;

		
		t_SphereTransform = ScaleMatrix(vec3(m_WorldSpaceBS.m_Radius * 2, m_WorldSpaceBS.m_Radius * 2, m_WorldSpaceBS.m_Radius * 2));
		t_SphereTransform[3][0] = m_WorldSpaceBS.m_Center.x;
		t_SphereTransform[3][1] = m_WorldSpaceBS.m_Center.y;
		t_SphereTransform[3][2] = m_WorldSpaceBS.m_Center.z;

		
		ComputeObjMVMat(MVMat, normalMVMat, t_VMatrix, t_SphereTransform);

		Model* t_sphereModel = ModelManager::GetInstance().GetModel("MODEL_DSPHERE");

		SendMVMat(MVMat, normalMVMat, mainMVMatLoc, mainNMVMatLoc);
		RenderMeshObj(t_sphereModel->GetModelMesh(), true);
	}


	void GFXComponent::DrawDebugAABB(const mat4 &  t_VMatrix)
	{

		mat4 MVMat, normalMVMat, t_AABBMWMatrix;

		t_AABBMWMatrix = ScaleMatrix(vec3(m_WorldSpaceAABB.m_Radius[0] * 2, m_WorldSpaceAABB.m_Radius[1] * 2, m_WorldSpaceAABB.m_Radius[2] * 2));
		t_AABBMWMatrix[3][0] = m_WorldSpaceAABB.m_Center.x;
		t_AABBMWMatrix[3][1] = m_WorldSpaceAABB.m_Center.y;
		t_AABBMWMatrix[3][2] = m_WorldSpaceAABB.m_Center.z;
       

		Model* t_sphereModel = ModelManager::GetInstance().GetModel("MODEL_DCUBE");
        ComputeObjMVMat(MVMat, normalMVMat, t_VMatrix, t_AABBMWMatrix);
		SendMVMat(MVMat, normalMVMat, mainMVMatLoc, mainNMVMatLoc);
		RenderMeshObj(t_sphereModel->GetModelMesh(), true);
	}





    /*************************************************************************/
    /*!
    \fn Model * GFXComponent::GetModel()

    \brief
    This function retrieves the pointer to the model object.

    \return
    This function returns the pointer to the model object.
    */
    /*************************************************************************/
    Model * GFXComponent::GetModel()
    {
        if (this->m_Model)
            return this->m_Model;

        return nullptr;
    }

    /*************************************************************************/
    /*!
    \fn void GFXComponent::SetModel(const str & t_ModelID)

    \brief
    This function sets the model object based on the model ID specified.

    \param t_ModelID
    This is the model ID.
    */
    /*************************************************************************/
    void GFXComponent::SetModel(const str & t_ModelID)
    {
        this->m_ModelID = t_ModelID;
        this->m_Model = ModelManager::GetInstance().GetModel(t_ModelID);
		this->m_WorldSpaceHierachicalAABB = m_Model->GetHierachicalAABB();

    }

    /*************************************************************************/
    /*!
    \fn const str &	GFXComponent::GetModelID()

    \brief
    This function retrieves the model ID.

    \return
    This function returns the model ID.
    */
    /*************************************************************************/
    const str &	GFXComponent::GetModelID()
    {
        return this->m_ModelID;
    }




    /*************************************************************************/
    /*!
    \fn void GFXComponent::UpdateModelWorldMatrix(mat4 & t_MatResult,
    const vec3 & t_Pos,
    const vec3 & t_RotVec,
    const vec3 & t_ScaleVec)

    \brief
    This function updates the model-world matrix per frame.

    \param t_MatResult
    This is the matrix result.

    \param t_Pos
    This is the position of the object.

    \param t_RotVec
    This is the rotation vector of the object.

    \param t_ScaleVec
    This is the scaling vector of the object.
    */
    /*************************************************************************/
    void GFXComponent::UpdateModelWorldMatrix(mat4 & t_MatResult,
        const vec3 & t_Pos,
        const vec3 & t_RotVec,
        const vec3 & t_ScaleVec)
    {
        mat4 t_RotMatrix = RotationMatrix(t_RotVec.x, t_RotVec.y, t_RotVec.z);
        mat4 t_TransMatrix = TranslationMatrix(t_Pos);
        mat4 t_ScaleMatrix = ScaleMatrix(t_ScaleVec);
        t_MatResult = TransformationMatrix(t_TransMatrix, t_RotMatrix, t_ScaleMatrix);
    }









	HierachicalAABB&	GFXComponent::GetWorldSpaceHAABB()
	{
		return m_WorldSpaceHierachicalAABB;
	}


	void GFXComponent::SetColorTexture(s32 ctID)
	{
		colorTexID = ctID;
	}
	void GFXComponent::SetNormalMapTexture(s32 ctID)
	{
		normalTexID = ctID;
	}

	void GFXComponent::SetNormalMapTexture2(s32 ctID)
	{
		normalTexID2 = ctID;
	}


	void GFXComponent::SetDefaultColor(const vec3& c)
	{
		color = c;
	}
}
