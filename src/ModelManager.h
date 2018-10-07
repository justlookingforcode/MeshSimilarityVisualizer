/*  Start Header -------------------------------------------------------
    File Name:      ModelMgr.h
    Author:         Tan Wen De Kevin
    Creation Date:  24/Sep/2016
    - End Header -------------------------------------------------------*/

#ifndef _MODELMGR_H_
#define _MODELMGR_H_

// ==========================
// includes
// ==========================

#include <unordered_map>
#include "Model.h"

// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
	class Model;

    typedef std::unordered_map<str, Model *>    MODEL_CONT;
    typedef MODEL_CONT::value_type              MODELINFO;
    typedef MODEL_CONT::iterator                MODEL_ITER;

    class ModelManager
    {
        public:

            static  ModelManager &  GetInstance();
			MODEL_CONT &        GetModelContainer();
            MODEL_CONT &        GetAtbModelContainer();
            bool                LoadAllModels(const str & t_FileName);
			void                AddModel(const str & t_ModelID, Model* t_Model);
            void                UnloadAllModels();
            Model *             GetModel(const str & t_ModelID);

        private:

            ModelManager();
            ~ModelManager();

            static ModelManager s_Instance;
            MODEL_CONT      m_ModelContainer;
            MODEL_CONT      m_AtbModelContainer;
    };
}

#define MODELMGR GAM300::ModelMgr::GetInstance()

#endif
