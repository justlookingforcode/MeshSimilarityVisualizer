/*  Start Header -------------------------------------------------------
File Name:      GameObjMgr.cpp
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/


#include "SceneObjectManager.h"
#include "graphics.hpp"
#include <algorithm>
#include "Collision.h"
namespace Proto
{
	/*************************************************************************/
	/*!
	\fn GameObjMgr & GameObjMgr::GetInstance()

	\brief
	This function retrieves the instance of this GOMgr object.

	\return
	This function returns the instance of this GOMgr object.
	*/
	/*************************************************************************/
	SceneObjectManager & SceneObjectManager::GetInstance()
	{
		static SceneObjectManager s_Instance;
		return s_Instance;
	}

	/*************************************************************************/
	/*!
	\fn GameObjMgr::GameObjMgr()

	\brief
	This is the default construcctor of GOMgr.
	*/
	/*************************************************************************/
	SceneObjectManager::SceneObjectManager()
		: renderObjectCount(9)
		, currentRadiusUnitCount(8)

	{



	}

	/*************************************************************************/
	/*!
	\fn GameObjMgr::~GameObjMgr()

	\brief
	This is the destrucctor of GOMgr.
	*/
	/*************************************************************************/
	SceneObjectManager::~SceneObjectManager()
	{
	}


	size_t	SceneObjectManager::GetNumberofSceneObjects()
	{
		return m_AllActiveObj.size();
	}

	/*************************************************************************/
	/*!
	\fn void GameObjMgr::UpdateAllObjects(const f32 & t_DeltaTime,
	Frustum & t_ViewFrustum)

	\brief
	This function performs an update to all game objects.

	\param t_DeltaTime
	This is the delta time.

	\param t_ViewFrustum
	This is the view frustum.
	*/
	/*************************************************************************/
	void SceneObjectManager::UpdateAll(const f32 & t_DeltaTime)
	{

		UpdateObj<GOINST_CONT, SceneObject>(this->m_AllActiveObj, t_DeltaTime);



	}

	void SceneObjectManager::UpdateRotation(const vec3& rotVec)
	{
		auto t_Start = this->m_RenderList.begin();
		auto t_End = this->m_RenderList.end();
		for (size_t renderCount = 0; t_Start != t_End && renderCount <renderObjectCount; ++t_Start, ++renderCount)
			if ((*t_Start))
			{
				(*t_Start)->SetRotVec(rotVec);
			}
	}

	/*************************************************************************/
	/*!
	\fn void GameObjMgr::DrawAllObjects(bool t_ShowBV)

	\brief
	This function performs drawing to all game objects.

	\param t_ShowBV
	This is the flag which indicates if bounding volume should be shown.
	*/
	/*************************************************************************/
	void SceneObjectManager::DrawAll(const mat4& viewMatrix)
	{

		//<! draw objects
		auto t_Start = this->m_RenderList.begin();
		auto t_End = this->m_RenderList.end();
		vec3 c(255.f, 255.f, 255.f);
#if 1
		SendObjectColor(c, objectColorLoc);
		for (size_t renderCount = 0; t_Start != t_End && renderCount <renderObjectCount; ++t_Start, ++renderCount)
			if (*t_Start)
				(*t_Start)->Draw(viewMatrix, drawWireFrame);
#endif





	}


	void   SceneObjectManager::AddSceneObject(SceneObject * t_Inst)
	{
		GOINST_CONT::iterator i = m_AllActiveObj.find(t_Inst->GetSoInstID());
		if (i != m_AllActiveObj.end())
		{
			delete m_AllActiveObj[t_Inst->GetSoInstID()];
			auto j = std::find(m_RenderList.begin(), m_RenderList.end(), t_Inst);
			m_RenderList.erase(j);
		}

		m_RenderList.push_back(t_Inst);
		m_AllActiveObj[t_Inst->GetSoInstID()] = t_Inst;

	}
	void  SceneObjectManager::RemoveSceneObject(const str & t_InstName)
	{
		GOINST_CONT::iterator t_Inst = this->m_AllActiveObj.find(t_InstName);
		GOINST_CONT::iterator t_End = this->m_AllActiveObj.end();

		// return the pointer to the instance if found. o.w., return null.
		if ((t_Inst != t_End))
		{
			delete t_Inst->second;
			m_AllActiveObj.erase(t_Inst);
			auto j = std::find(m_RenderList.begin(), m_RenderList.end(), t_Inst->second);
			m_RenderList.erase(j);
		}
	}

	size_t SceneObjectManager::GetRenderObjCount()
	{
		return renderObjectCount;
	}
	void SceneObjectManager::incrementRenderObjCount()
	{
		if (renderObjectCount + currentRadiusUnitCount + 8 < m_AllActiveObj.size())
		{
			currentRadiusUnitCount += 8;
			renderObjectCount += currentRadiusUnitCount;
		}
	}
	void SceneObjectManager::decrementRenderObjCount()
	{
		if (renderObjectCount >= 9)
		{
			renderObjectCount -= currentRadiusUnitCount;
			currentRadiusUnitCount -= 8;
			
		}
	}



}