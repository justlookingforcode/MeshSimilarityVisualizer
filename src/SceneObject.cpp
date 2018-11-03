#include "graphics.hpp"
#include "SceneObject.h"
#include "GFXComponent.h"
namespace Proto
{
    SceneObject::GOINST_VEC SceneObject::s_SceneObjects;

    SceneObject::SceneObject() : 
        m_IsAlive(false),

        m_AlphaVal(1.f),
        m_ScaleVec(vec3(1.f, 1.f, 1.f)),
        m_RotVec(vec3(0.f, 0.f, 0.f)),
		m_IsDirty(true)

    {
        this->m_MeshRenderer = new GFXComponent();
    }

    SceneObject::~SceneObject()
    {
        GOINST_ITER t_i = s_SceneObjects.begin();
        GOINST_ITER t_e = s_SceneObjects.end();

        if (this->m_MeshRenderer)
            delete(this->m_MeshRenderer);

    }

	SceneObject::SceneObject(const SceneObject& r)
	{
		m_IsAlive = r.m_IsAlive;

		m_AlphaVal = r.m_AlphaVal;
		m_ScaleVec = r.m_ScaleVec;
		m_RotVec = r.m_RotVec;
		m_IsDirty = r.m_IsDirty;
		this->m_MeshRenderer = new GFXComponent(*r.m_MeshRenderer);
	}

    const str & SceneObject::GetSoInstID() const
    {
        return this->m_SOInstID;
    }

    void SceneObject::SetSoInstID(const str& t_SOInstID)
    {
        this->m_SOInstID = t_SOInstID;
    }

    const mat4 & SceneObject::GetMWMatrix()
    {
        return this->m_MWMatrix;
    }

    void SceneObject::SetScaleVec(const vec3 & t_ScaleVec)
    {
		m_IsDirty = true;
        this->m_ScaleVec = t_ScaleVec;
    }

    const vec3 & SceneObject::GetScaleVec()
    {

        return this->m_ScaleVec;
    }

    void SceneObject::SetRotVec(const vec3 & t_RotVec)
    {
		m_IsDirty = true;
        this->m_RotVec = t_RotVec;
    }

    const vec3 & SceneObject::GetRotVec()
    {
        return this->m_RotVec;
    }

    void SceneObject::SetPosVec(const vec3 & t_PosVec)
    {
        this->m_PosVec = t_PosVec;
    }

    const vec3 & SceneObject::GetPosVec()
    {
		m_IsDirty = true;
        return this->m_PosVec;
    }

    GFXComponent * SceneObject::GetMeshRenderer()
    {
        if (this->m_MeshRenderer)
            return this->m_MeshRenderer;

        return nullptr;
    }


    void SceneObject::Update(f32 t_DeltaTime)
    {

		if (m_IsDirty)
		{
			m_IsDirty = false;
			this->m_MeshRenderer->Update(this->m_MWMatrix,
				this->m_PosVec,
				this->m_RotVec,
				this->m_ScaleVec);
		}
    }





    void SceneObject::Draw(const mat4& viewMatrix, bool wireframe)
    {
        mat4 MVMat, normalMVMat; 
        this->m_MeshRenderer->Draw(viewMatrix, this->m_MWMatrix,  wireframe);
    }

	void SceneObject::DrawDebugSphere(const mat4& viewMatrix)
	{
		mat4 MVMat, normalMVMat; 
		this->m_MeshRenderer->DrawDebugSphere(viewMatrix);
	}

	void SceneObject::DrawDebugAABB(const mat4& viewMatrix)
	{
		mat4 MVMat, normalMVMat; 
		this->m_MeshRenderer->DrawDebugAABB(viewMatrix);
	}

    void SceneObject::DrawDebugHierachicalAABB(const mat4& viewMatrix)
    {
        //if (this->m_IsInView == CS350::OUTSIDE)
        //    return;

        mat4 MVMat, normalMVMat;
        this->m_MeshRenderer->DrawDebugHierachicalAABB(viewMatrix);
    }
	


	void SceneObject::MoveUp()
	{
		m_IsDirty = true;
		m_PosVec.y += 1;
	}
	
	void SceneObject::MoveDown()
	{
		m_IsDirty = true;
		m_PosVec.y -= 1;
	}
	void SceneObject::MoveLeft()
	{
		m_IsDirty = true;
		m_PosVec.x -= 1;
	}
	
	void SceneObject::MoveRight()
	{
		m_IsDirty = true;
		m_PosVec.x += 1;
	}
	void SceneObject::MoveCloser()
	{
		m_IsDirty = true;
		m_PosVec.z -= 1;

	}
	void SceneObject::MoveFarther()
	{
		m_IsDirty = true;
		m_PosVec.z += 1;
	}

	void SceneObject::RotateUp()
	{
		m_IsDirty = true;

		m_RotVec.x += 1;


	}
	void SceneObject::RotateDown()
	{
		m_IsDirty = true;
		m_RotVec.x -= 1;
	}
	void SceneObject::RotateLeft()
	{
		m_IsDirty = true; 
		m_RotVec.y -= 1;
	}
	void SceneObject::RotateRight()
	{
		m_IsDirty = true;
		m_RotVec.y += 1;
	}


	bool SceneObject::isMoved()
	{
		return m_IsDirty;
	}

};