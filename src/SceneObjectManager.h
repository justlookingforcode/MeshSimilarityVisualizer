/*  Start Header -------------------------------------------------------
File Name:      GameObjMgr.h
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/
#ifndef SCENE_OBJECT_MANAGER_H_
#define SCENE_OBJECT_MANAGER_H_

// ==========================
// includes
// ==========================

#include <unordered_map>
#include "defines.h"
#include "SceneObject.h"
#include "Plane.h"



// ==========================
// defines
// ==========================



// ==========================
// class/ function prototypes
// ==========================

typedef std::unordered_map<str, Proto::SceneObject*>           GOINST_CONT;
typedef GOINST_CONT::iterator                                   m_AllActiveObj;

//typedef std::unordered_map<str, GAM300::TransitionScreen *> ScreenItems;




namespace Proto
{
	class SceneObjectManager
	{
	public:

        static SceneObjectManager & GetInstance();

		// load objects from xml file
		void                CreateAndInitSceneObjects(const str & t_XmlPath);
		void                SaveSceneObjectsToXml(const str & t_XmlPath);

		// operations to be performed on all objects
		void                CreateAll();
		void                DeleteAll();
		void                UpdateAll(const f32 & t_DeltaTime);
        void                DrawAll(const mat4& viewMatrix);
		void                UpdateRotation(const vec3& rotVec);
 

        void                AddSceneObject(SceneObject * t_Inst);
        void                RemoveSceneObject(const str & t_InstName);

		size_t				GetNumberofSceneObjects();

		// Update screen items
		void updateScreenItems(float deltaTime);

		// Draw screen items
		void drawScreenItems();
		size_t GetRenderObjCount();
		void incrementRenderObjCount();
		void decrementRenderObjCount();
		GOINST_CONT         m_AllActiveObj;
		std::vector<SceneObject*> m_RenderList;
	private:
        
        SceneObjectManager();
        ~SceneObjectManager();

		size_t renderObjectCount;
		size_t currentRadiusUnitCount;
		// Level loaded
		bool isLevelLoaded;
		bool isLevelStarted;
		// Remove screen items check
		void removeScreenItemsCheck();


     

        template<typename Container, typename T>
        void UpdateObj(Container & t_Container, const f32 & t_DeltaTime)
        {
            Container::iterator t_Iter = t_Container.begin();
            Container::iterator t_End = t_Container.end();

            for (; t_Iter != t_End; ++t_Iter)
            {
                T * t_Obj = t_Iter->second;
                t_Obj->Update(t_DeltaTime);
                

                  
            }
        }
	};
}

#define SOMGR Proto::SceneObjectManager::GetInstance()

#endif
