/*  Start Header -------------------------------------------------------
    File Name:      ModelMgr.cpp
    Author:         Tan Wen De Kevin
    Creation Date:  24/Sep/2016
    - End Header -------------------------------------------------------*/

#include <fstream>

#include "ModelManager.h"


namespace Proto
{
    bool LoadConfigurationFrom(const char * t_Filename,
        STRCONT & t_FileCont)
    {
        std::ifstream t_ConfigFile(t_Filename, std::ios_base::in);
        if (!t_ConfigFile.is_open())
            return false;

        str t_Line = "";

        // Read config file
        while (getline(t_ConfigFile, t_Line))
            t_FileCont.push_back(t_Line);

        t_ConfigFile.close();
        return true;
    }


    ModelManager ModelManager::s_Instance = ModelManager();

    /*************************************************************************/
    /*!
    \fn ModelMgr::ModelMgr()

    \brief
        This is the constructor of the ModelMgr class.
    */
    /*************************************************************************/
    ModelManager::ModelManager()
    {
    }
    
    /*************************************************************************/
    /*!
    \fn ModelMgr::~ModelMgr()

    \brief
        This is the destructor of the ModelMgr class.
    */
    /*************************************************************************/
    ModelManager::~ModelManager()
    {

    }

    /*************************************************************************/
    /*!
    \fn ModelMgr & ModelMgr::GetInstance()

    \brief
        This function retrieves the instance of this class.
    
    \return
        This function returns the instance of this class.
    */
    /*************************************************************************/
    ModelManager & ModelManager::GetInstance()
    {
        return s_Instance;
    }

    /*************************************************************************/
    /*!
    \fn bool ModelMgr::LoadAllModels(const str & t_FileName)

    \brief
        This function loads all the model objects required for the game.
    
    \param t_FileName
        This is the file name of the model object.

    \return
        This function returns TRUE if loaded successfully.
    */
    /*************************************************************************/
    bool ModelManager::LoadAllModels(const str & t_FileName)
    {
        STRCONT t_FileCont;
        
        // load file
        if(!LoadConfigurationFrom(t_FileName.c_str(), t_FileCont))
            return false;

        u32 t_Size = t_FileCont.size();
        for(u32 i = 0; i < t_Size; ++i)
        {
            str    t_Line           = t_FileCont[i];
            size_t t_DelimiterPos   = t_Line.find_first_of(" ");
            str    t_ID             = t_Line.substr(0, t_DelimiterPos);
            str    t_ModelFileName  = t_Line.substr(t_DelimiterPos + 1);

            Model * t_Model = new Model(t_ModelFileName);
            this->m_ModelContainer.insert(MODELINFO(t_ID.c_str(), t_Model));

            if(t_ID.compare("MODEL_DSPHERE") != 0 && t_ID.compare("MODEL_DCUBE")  != 0 && t_Model)
            {

				t_Model->BuildSphere(this->m_ModelContainer["MODEL_DSPHERE"]->GetModelMesh());
	            t_Model->BuildAABB(this->m_ModelContainer["MODEL_DCUBE"]->GetModelMesh());
				//t_Model->BuildHierachicalOBB();
				t_Model->BuildHierachicalAABB();
            }
            
        }

        t_FileCont.clear();
        std::cout << "Models loaded successfully." << std::endl;
        return true;
    }


	void ModelManager::AddModel(const str & t_ModelID, Model* t_Model)
	{
		this->m_ModelContainer.insert(MODELINFO(t_ModelID.c_str(), t_Model));
	}


    /*************************************************************************/
    /*!
    \fn void ModelMgr::UnloadAllModels()

    \brief
        This function unloads all the model objects.
    */
    /*************************************************************************/
    void ModelManager::UnloadAllModels()
    {
        MODEL_ITER t_Iter   = this->m_ModelContainer.begin();
        MODEL_ITER t_End    = this->m_ModelContainer.end();

        for (; t_Iter != t_End;)
            t_Iter = this->m_ModelContainer.erase(t_Iter);

        // clear the container
        this->m_ModelContainer.clear();
    }

    /*************************************************************************/
    /*!
    \fn Model * ModelMgr::GetModel(const str & t_ModelID)

    \brief
        This function retrieves the specified model object.

    \param t_ModelID
        This is the model string ID.

    \return
        This function returns a pointer to the model object.
    */
    /*************************************************************************/
    Model * ModelManager::GetModel(const str & t_ModelID)
    {
        return this->m_ModelContainer[t_ModelID];
    }

    MODEL_CONT& ModelManager::GetModelContainer()
	{
		return m_ModelContainer;
	}

    MODEL_CONT& ModelManager::GetAtbModelContainer()
    {
        return m_AtbModelContainer;
    }
}
