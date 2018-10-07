/*  Start Header -------------------------------------------------------
    File Name:      Model.cpp
    Author:         Tan Wen De Kevin
    Creation Date:  24/Sep/2016
    - End Header -------------------------------------------------------*/

#include "Assimp/assimp.hpp"       // C++ importer interface
#include "Assimp/aipostprocess.h"
#include "Model.h"


namespace Proto
{
    /*************************************************************************/
    /*!
    \fn Model::Model(const str & t_FileName)

    \brief
        This is the constructor of the Model class.

    \param t_FileName
        This is the file name of the object model.
    */
    /*************************************************************************/
    Model::Model(const str & t_FileName):   m_FileName(t_FileName)
    {
		m_IsLoaded = LoadModel();
        if(!m_IsLoaded)
            std::cout << "Model.cpp: Unable to load "<<t_FileName<<"!\n";
    }

    /*************************************************************************/
    /*!
    \fn Model::Model(Mesh & t_Mesh)

    \brief
        This is the copy constructor of the Model class.

    \param t_Mesh
        This is the model mesh to copy over.
    */
    /*************************************************************************/
    Model::Model(Mesh & t_Mesh)
    {
		m_ObjMesh     = &t_Mesh;
		m_IsLoaded    = true;
		BindModelVAO();
		BuildHierachicalAABB();
    }

    /*************************************************************************/
    /*!
    \fn Model::~Model()

    \brief
        This is the destructor of the Model class.
    */
    /*************************************************************************/
    Model::~Model()
    {
        DeleteModel();
    }

    /*************************************************************************/
    /*!
    \fn void Model::BindModelVAO()

    \brief
        This function binds the model object VAO.
    */
    /*************************************************************************/
    void Model::BindModelVAO()
    {
		// Bind the states of the buffers for rendering later
		//SHADERSMGR.GetShader().BindStates_CopyBuffers(&this->m_ObjMesh);
		//SHADERSMGR.GetShader().UnBindVAO();
        glGenVertexArrays(1, &m_ObjMesh->VAO);
		glBindVertexArray(m_ObjMesh->VAO);

		glGenBuffers(1, &m_ObjMesh->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_ObjMesh->VBO);
		glBufferData(GL_ARRAY_BUFFER, m_ObjMesh->vertexBuffer.size() * sizeof(m_ObjMesh->vertexBuffer[0]), &m_ObjMesh->vertexBuffer[0], GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_ObjMesh->IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ObjMesh->IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_ObjMesh->indexBuffer.size() * sizeof(m_ObjMesh->indexBuffer[0]), &m_ObjMesh->indexBuffer[0], GL_STATIC_DRAW);

		const int stride = sizeof(Vertex);
		const VertexLayout *layout = vLayout;

		while (layout->size)
		{
            glEnableVertexAttribArray(layout->location);
            glVertexAttribPointer(layout->location, layout->size, layout->type, layout->normalized, stride, (void*)layout->offset);
			layout++;
		}
		glBindVertexArray(0);
    }



    /*************************************************************************/
    /*!
    \fn bool Model::LoadModel()

    \brief
        This function loads the model object.
    */
    /*************************************************************************/
    bool Model::LoadModel()
    {

		std::cout << "Loading asset :" << this->m_FileName << std::endl;
        Assimp::Importer t_ModelImporter;
        
        const aiScene * t_Scene = t_ModelImporter.ReadFile(this->m_FileName, 
                                                           aiProcess_CalcTangentSpace         | 
                                                           aiProcess_Triangulate              |
                                                           aiProcess_JoinIdenticalVertices    |
                                                           aiProcess_SortByPType);
		if (this->m_ObjMesh)
			delete m_ObjMesh;

		this->m_ObjMesh = new Mesh();
        // if unable to create scene ptr obj
        if(!t_Scene)
        {
            str s=this->m_FileName;
            s+=" ";
            s+=t_ModelImporter.GetErrorString();

           //GE_ASSERT_MSG(s.c_str());
           return false;
        }
    
        // if the file has no mesh, return false
        if(!t_Scene->HasMeshes())
        {
            //GE_ASSERT_MSG("no vertices");
            return false;
        }
        // ==========================
        // load all the obj data
        // ==========================
        
        // store mesh object for future accessing
        aiMesh * t_Mesh = (*(t_Scene->mMeshes));

        if(t_Mesh->HasPositions())  // load vertices
        {
            LoadVertices(t_Mesh);
        }
        else
        {
            str s=this->m_FileName;
            s+=" no Vertices found";
            //GE_WARNING_MSG(s.c_str());
            
        }
        if(t_Mesh->HasNormals())    // load normals
        {
            LoadNormal(t_Mesh);
        }
        else
        {
            str s=this->m_FileName;
            s+=" no Normals found";
            //GE_WARNING_MSG(s.c_str());
            
        }
        if(t_Mesh->HasTangentsAndBitangents())
        {
            LoadTangent(t_Mesh);
        }
        else
        {
            str s=this->m_FileName;
            s+=" no Tangent and Bitangent found";
            //GE_WARNING_MSG(s.c_str());
            
        }
        if(t_Mesh->HasFaces())      // load indices
        {
            LoadIndices(t_Mesh);
        }
        else
        {
            str s=this->m_FileName;
            s+=" no Face Indices found";
            //GE_WARNING_MSG(s.c_str());
            
        }
        if(t_Mesh->HasTextureCoords(0))
        {
            LoadTextureUV(t_Mesh);
        }
        else
        {
            str s=this->m_FileName;
            s+=" no UVCords found";
            //GE_WARNING_MSG(s.c_str());
            
        }


        BindModelVAO();


		BuildHierachicalAABB();
        return this->m_IsLoaded = true;
    }

    /*************************************************************************/
    /*!
    \fn void Model::BuildSphere(Mesh & t_Mesh)

    \brief
        This function creates the model BS mesh of this model object.

    \param t_Mesh
        This is the model object mesh.
    */
    /*************************************************************************/
    void Model::BuildSphere(Mesh & t_BSModelMesh)
    {
        //this->m_ModelBS.m_BVMesh = t_BSModelMesh;
        this->m_ModelBS.RitterSphere(this->m_ModelBS, 
			this->m_ObjMesh->vertexBuffer,
			this->m_ObjMesh->vertexBuffer.size());
    }

    /*************************************************************************/
    /*!
    \fn void Model::BuildAABB(Mesh & t_Mesh)

    \brief
        This function creates the model AABB mesh of this model object.

    \param t_Mesh
        This is the model object mesh.
    */
    /*************************************************************************/
    void Model::BuildAABB(Mesh & t_AABBModelMesh)
    {
        //this->m_ModelAABB.m_BVMesh = t_AABBModelMesh;
		this->m_ModelAABB.Create(this->m_ObjMesh->vertexBuffer);
    }



	void Model::BuildHierachicalAABB()
	{
		this->m_hAABB.BuildFromModel(m_ObjMesh->vertexBuffer, m_ObjMesh->indexBuffer, 7);
	}
    
    /*************************************************************************/
    /*************************************************************************/
    /*!
    \fn const BS & Model::GetBS()

    \brief
        This function retrieves the BS model mesh of this model object.

    \return
        This function returns the BS model mesh of this model object.
    */
    /*************************************************************************/
    const BS & Model::GetBS()
    {
        return m_ModelBS;
    }

    /*************************************************************************/
    /*!
    \fn const AABB & Model::GetAABB()

    \brief
        This function retrieves the AABB model mesh of this model object.

    \return
        This function returns the AABB model mesh of this model object.
    */
    /*************************************************************************/
    const AABB & Model::GetAABB()
    {
        return m_ModelAABB;
    }
    
    /*************************************************************************/
    /*!
    \fn const OBB & Model::GetOBB()

    \brief
        This function retrieves the OBB model mesh of this model object.

    \return
        This function returns the OBB model mesh of this model object.
    */
    /*************************************************************************/
    /*
    const OBB & Model::GetOBB()
    {
        return m_ModelOBB;
    }
    */
    /*************************************************************************/
    /*!
    \fn void Model::DeleteModel()

    \brief
        This function deletes the created models.
    */
    /*************************************************************************/
    void Model::DeleteModel()
    {
		if(!this->m_IsLoaded)
		    return;

		// delete the meshes used/allocated
		//SHADERSMGR.GetShader().DeleteStates(&this->m_ObjMesh);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_ObjMesh->VAO);
		glDeleteBuffers(1, &m_ObjMesh->VBO);
		glDeleteBuffers(1, &m_ObjMesh->IBO);

		m_IsLoaded = false;
		m_FileName.clear();
    }


    /*************************************************************************/
    /*!
    \fn void Model::LoadVertices(const aiMesh * t_Mesh)

    \brief
        This function loads the vertices of the model object.

    \param t_Mesh
        This is the mesh object.
    */
    /*************************************************************************/
    void Model::LoadVertices(const aiMesh * t_Mesh)
    {
        // store size for future accessing
        u32 t_Size = t_Mesh->mNumVertices;
        for(u32 i = 0; i < t_Size; ++i)
        {
            // store the current vertex for future accessing
            aiVector3D t_Vert = t_Mesh->mVertices[i];
                
            Vertex v;
            v.pos.x = t_Vert.x;
            v.pos.y = t_Vert.y;
            v.pos.z = t_Vert.z;
            //v.pos.w = 1;
                
            // push into container
			m_ObjMesh->vertexBuffer.push_back(v);
        }
            
		m_ObjMesh->numVertices = t_Size;
    }
    
    /*************************************************************************/
    /*!
    \fn void Model::LoadNormal(const aiMesh * t_Mesh)

    \brief
        This function loads the normal values of the model object.

    \param t_Mesh
        This is the mesh object.
    */
    /*************************************************************************/
    void Model::LoadNormal(const aiMesh * t_Mesh)
    {
		u32 t_Size = m_ObjMesh->numVertices;
        for(u32 i = 0; i < t_Size;++i)
        {
            aiVector3D t_Normal = t_Mesh->mNormals[i];
            
            Vertex v;
            v.nrm.x = t_Normal.x;
            v.nrm.y = t_Normal.y;
            v.nrm.z = t_Normal.z;
            
			m_ObjMesh->vertexBuffer[i].nrm = v.nrm;
        }
    }
    
    /*************************************************************************/
    /*!
    \fn void Model::LoadTangent(const aiMesh * t_Mesh)

    \brief
        This function loads the tangent and bitangent values of the model object.

    \param t_Mesh
        This is the mesh object.
    */
    /*************************************************************************/
    void Model::LoadTangent(const aiMesh * t_Mesh)
    {
		u32 t_Size = m_ObjMesh->numVertices;
        for(u32 i = 0; i < t_Size; ++i)
        {
            Vertex v;

            aiVector3D t_Tan = t_Mesh->mTangents[i];
            v.tan.x = t_Tan.x;
            v.tan.y = t_Tan.y;
            v.tan.z = t_Tan.z;

            aiVector3D t_Bitan = t_Mesh->mBitangents[i];
            v.bitan.x = t_Bitan.x;
            v.bitan.y = t_Bitan.y;
            v.bitan.z = t_Bitan.z;
            
			m_ObjMesh->vertexBuffer[i].tan = v.tan;
			m_ObjMesh->vertexBuffer[i].bitan = v.bitan;
        }
    }
    
    /*************************************************************************/
    /*!
    \fn void Model::LoadTextureUV(const aiMesh * t_Mesh)

    \brief
        This function loads the UV values of the model object.

    \param t_Mesh
        This is the mesh object.
    */
    /*************************************************************************/
    void Model::LoadTextureUV(const aiMesh * t_Mesh)
    {
		u32 t_Size = m_ObjMesh->numVertices;
        for(u32 i = 0; i < t_Size; ++i)
        {
            //int num = t_Mesh->GetNumUVChannels();
            aiVector3D t_Tex = t_Mesh->mTextureCoords[0][i];
            
            Vertex v;
            v.uv.x = t_Tex.x;
            v.uv.y = t_Tex.y;

			m_ObjMesh->vertexBuffer[i].uv = v.uv;
        }
    }

    /*************************************************************************/
    /*!
    \fn void Model::LoadIndices(const aiMesh * t_Mesh)

    \brief
        This function loads the indices of the model object.

    \param t_Mesh
        This is the mesh object.
    */
    /*************************************************************************/
    void Model::LoadIndices(const aiMesh * t_Mesh)
    {
        m_ObjMesh->numTris      = t_Mesh->mNumFaces;
		m_ObjMesh->numIndices = 0;
		u32 t_TotalTri = m_ObjMesh->numTris;

        for(u32 i = 0; i < t_TotalTri; ++i)
        {
          aiFace t_Face         = t_Mesh->mFaces[i];
          int t_NoOfIndices     = t_Face.mNumIndices;
		  m_ObjMesh->numIndices += t_NoOfIndices;

          for(int j = 0; j < t_NoOfIndices; ++j)
			  m_ObjMesh->indexBuffer.push_back(t_Face.mIndices[j]);
        }

    }



    /*************************************************************************/
    /*!
    \fn Mesh& Model::GetModelMesh()

    \brief
        This is the getter of the Model class.
    */
    /*************************************************************************/
    Mesh& Model::GetModelMesh()
    {
        return *m_ObjMesh;
    }

    /*************************************************************************/
    /*!
    \fn void Model::SetModelMesh(Mesh & t_Mesh)

    \brief
        This is the setter function of the Model class.

    \param t_Mesh
        This is the mesh to copy over to.
    */
    /*************************************************************************/
    void Model::SetModel(Model & t_Model)
    {
        this->m_ObjMesh     = &t_Model.GetModelMesh();
        this->m_FileName    = t_Model.m_FileName;
    }





	const HierachicalAABB &  Model::GetHierachicalAABB()
	{
		return this->m_hAABB;
	}


	const HierachicalBS &  Model::GetHierachicalBS()
	{
		return this->m_hBS;
	}



}
