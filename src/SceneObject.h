#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

// ==========================
// includes
// ==========================

#include "defines.h"
#include "IControlledSceneObject.h"
#include "GFXComponent.h"
/* Start Header ------------------------------------------------------


File Name:      Culling.h
Purpose:        This is the Culling class implementations.
Language:       C++
Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/




// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
	class GFXComponent;
	class SceneObject : public IControlledSceneObject
	{
		typedef std::vector<SceneObject*>   GOINST_VEC;
		typedef GOINST_VEC::iterator        GOINST_ITER;
		typedef std::string					str;

	public:



		SceneObject();
		virtual ~SceneObject();
		SceneObject(const SceneObject& rhs);
		//virtual void        Update(const double & t_DeltaTime) = 0;
        virtual void        Draw(const mat4& viewMatrix, bool wireframe = false);
		virtual void        DrawDebugSphere(const mat4& viewMatrix);
		virtual void        DrawDebugAABB(const mat4& viewMatrix);


		const str &         GetSoInstID() const;
		void                SetSoInstID(const str & t_GoInstID);

		const mat4 &        GetMWMatrix();

		const vec3 &        GetScaleVec();
		void                SetScaleVec(const vec3 & t_ScaleVec);
		const vec3 &        GetRotVec();
		void                SetRotVec(const vec3 & t_RotVec);
        const vec3 &        GetPosVec();
        void                SetPosVec(const vec3 & t_PosVec);

        void                Update(f32 t_DeltaTime);

        GFXComponent * GetMeshRenderer();

		virtual void MoveUp();
		virtual void MoveDown();
		virtual void MoveLeft();
		virtual void MoveRight();
		virtual void MoveCloser();
		virtual void MoveFarther();

		virtual void RotateUp();
		virtual void RotateDown();
		virtual void RotateLeft();
		virtual void RotateRight();


		virtual bool isMoved();

		// init and deinit GOInst info
		//virtual void        Init(void);
		//void                Init(SceneObject* t_inst);
		//virtual void        DeInit();

		protected:



		static GOINST_VEC   s_SceneObjects;
		bool                m_IsAlive;
		bool                m_IsDirty;
		f32                 m_AlphaVal;
        GFXComponent*       m_MeshRenderer;
		str                 m_SOClassType;
		str                 m_SOInstID;
		vec3                m_ScaleVec;
		vec3                m_RotVec;
        vec3                m_PosVec;
		mat4                m_MWMatrix;

		

	};

	//typedef SceneObject * (__stdcall * CreateGOFn)(void);


}

#endif
