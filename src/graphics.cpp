/******************************************************************************/
/*!
\file   graphics.cpp
\par    Purpose: Implement the graphics-related functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   27/3/2015
*/
/******************************************************************************/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <ctime>
#include "graphics.hpp"
#include "Camera.hpp"
#include "textfile.h"
#include "image_io.h"
#include "Model.h"
#include "ModelManager.h"
#include "GFXComponent.h"
#include "SceneObject.h"
#include "SceneObjectManager.h"
#include "Collision.h"
#include "HierachicalAABB.h"


/******************************************************************************/
/*  Graphics-related variables                                                */
/******************************************************************************/


/*  For displaying FPS */
clock_t currTime, prevTime;
int frameCount;
float secCount;        /*  Num of seconds from prevTime to currTime */
float fps;

bool drawWireFrame(false);
bool drawBoundingVolumes(false);
bool rotateModels (false);
bool renderingModeChanged(false);
size_t windowHeight, windowWidth;
/*  For clearing depth buffer */
const GLfloat one = 1.0f;
const GLfloat bgColor[] = { 0.3f, 0.3f, 0.3f, 1.f };

/*  For color texture */
GLuint texID[4];
const char *objTexFile[4] = {"../images/pottery.png", "../images/stone.png", "../images/normal_map.fw.png", "../images/normal_map2.fw.png" };

//link up default object to be controlled by the keyboard
Camera mainCam;

Proto::SceneObject sceneObject0;
Proto::SceneObject sceneObject1;
Proto::SceneObject sceneObject2;
std::vector<Proto::SceneObject> vecSceneObjects;

const int maxObjCount = 50;
const int maxj = 5;
const int maxi = maxObjCount / maxj;

IControlledSceneObject* activeControlledObject(&mainCam);
u32 activeShaderProgram = ProgType::MAIN_PROG;

//debug meshes
Mesh dSphereMesh;
Mesh dSphereMesh2;
Mesh dCubeMesh;

/*  Matrices for view/projetion transformations */
/*  Viewer camera */
Mat4 mainCamViewMat, mainCamProjMat, mainCamMVMat[1], mainCamNormalMVMat[1];

double elapsedtime(0);
clock_t pTime(0);

/*  Location of light data */
GLint numLightsLoc, lightPosLoc;
GLint ambientLoc, diffuseLoc, specularLoc, specularPowerLoc;
GLint lightOnLoc;

/*  Location of color textures */
GLint textureLoc;                       /*  Normal object texture */
GLint textureOnLoc;

/*  Location of bump/normal textures */
GLint normalTexLoc, bumpTexLoc;

/*  For indicating whether object has normal map, and parallax mapping status */
GLint normalMappingOnLoc, parallaxMappingOnLoc;


struct ShaderType
{
    enum { VERTEX_SHADER = 0, FRAGMENT_SHADER, NUM_SHADERTYPES };
};

/*  Shader filenames */
const char file[ProgType::NUM_PROGTYPES][ShaderType::NUM_SHADERTYPES][100] =
{
	{
		{ "../shaders/main.vs" },
		{ "../shaders/main.fs" }
	},
	{
		{ "../shaders/heatmap.vs" },
		{ "../shaders/heatmap.fs" }
	}
};

/*  ID of the shader programs that we'll use */
GLuint prog[ProgType::NUM_PROGTYPES];


/*  Locations of the variables in the shader. */
/*  Locations of transform matrices */
GLint mainMVMatLoc, mainNMVMatLoc, mainProjMatLoc, objectColorLoc;  /*  used for main program */

void updateHeatMap(Proto::SceneObject* shadedObject, Proto::SceneObject* opposingObject);

/******************************************************************************/
/*!
\fn     void ValidateShader(GLuint shader, const char *file)
\brief
        Check whether shader files can be compiled successfully.
\param  shader
        ID of the shader
\param  file
        Shader file name
*/
/******************************************************************************/
void ValidateShader(GLuint shader, const char *file)
{
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];

    GLsizei length = 0;
    GLint result;

    glGetShaderInfoLog(shader, 512, &length, buffer);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE && length > 0)
    {
        std::cerr << "Shader " << file << " compilation error: " << buffer << "\n";
        exit(1);
    }
    else
        std::cout << "Shader " << file << " compilation successful.\n";
}


/******************************************************************************/
/*!
\fn     void ValidateProgram(GLuint program)
\brief
        Check whether shader program can be linked successfully.
\param  program
        ID of the shader program
*/
/******************************************************************************/
void ValidateProgram(GLuint program)
{
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;
    GLint status;

    /*  Ask OpenGL to give us the log associated with the program */
    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer); 
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status != GL_TRUE && length > 0)
    {
        std::cerr << "Program " << program << " link error: " << buffer << "\n";
        exit(1);
    }
    else
        std::cout << "Program " << program << " link successful.\n";

    /*  Ask OpenGL to validate the program */
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cerr << "Error validating shader " << program << ".\n";
        exit(1);
    }
    else
    {
        std::cout << "Program " << program << " validation successful.\n";
    }
}


/******************************************************************************/
/*!
\fn     GLuint CompileShaders(char vsFilename[], char fsFilename[])
\brief
        Read the shader files, compile and link them into a program for render.
\param  const char vsFilename[]
        Vertex shader filename.
\param  const char fsFilename[]
        Fragment shader filename.
\return
        The rendering program ID.
*/
/******************************************************************************/
GLuint CompileShaders(const char vsFilename[], const char fsFilename[])
{
    char *vsSource = ReadTextFile(vsFilename);
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsSource, NULL);
    glCompileShader(vertexShader);
    ValidateShader(vertexShader, vsFilename);           /*  Prints any errors */

    char * fsSource = ReadTextFile(fsFilename);
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsSource, NULL);
    glCompileShader(fragmentShader);
    ValidateShader(vertexShader, fsFilename);           /*  Prints any errors */

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    ValidateProgram(program);                           /*  Print any errors */

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	free(vsSource);
	free(fsSource);

    return program;
}


/******************************************************************************/
/*!
\fn     void SetUpMainUniformLocations(GLuint prog)
\brief
        Look up the locations of uniform variables in the main shader program.
\param  prog
        The given shader program ID.
*/
/******************************************************************************/
void SetUpMainUniformLocations(GLuint prog)
{
    mainMVMatLoc	= glGetUniformLocation(prog, "mvMat");
    mainNMVMatLoc	= glGetUniformLocation(prog, "nmvMat");
    mainProjMatLoc	= glGetUniformLocation(prog, "projMat");

    textureLoc		= glGetUniformLocation(prog, "colorTex");
	textureOnLoc	= glGetUniformLocation(prog, "textureOn");
    numLightsLoc	= glGetUniformLocation(prog, "numLights");
    lightPosLoc		= glGetUniformLocation(prog, "lightPosVF");
	lightOnLoc		= glGetUniformLocation(prog, "lightOn");

    ambientLoc			= glGetUniformLocation(prog, "ambient");
    diffuseLoc			= glGetUniformLocation(prog, "diffuse");
    specularLoc			= glGetUniformLocation(prog, "specular");
    specularPowerLoc	= glGetUniformLocation(prog, "specularPower");

	bumpTexLoc				= glGetUniformLocation(prog, "bumpTex");
	normalTexLoc			= glGetUniformLocation(prog, "normalTex");
	normalMappingOnLoc		= glGetUniformLocation(prog, "normalMappingOn");
	parallaxMappingOnLoc	= glGetUniformLocation(prog, "parallaxMappingOn");
    objectColorLoc			= glGetUniformLocation(prog, "uOutColor");
}



/******************************************************************************/
/*!
\fn     void SetUpVertexData(Mesh &mesh)
\brief
        Create VAO (graphics state), VBO (vertex data state) and 
        IBO (vertex index state) for the mesh, and feed vertex data into 
        the shaders.
\param  mesh
        The input mesh, whose VAO/VBO/IBO members will be updated and data
        will be sent to shaders.
*/
/******************************************************************************/
void SetUpVertexData(Mesh &mesh)
{

    glGenVertexArrays(1, &mesh.VAO);
    glBindVertexArray(mesh.VAO);

    glGenBuffers(1, &mesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    /*  Copy vertex attributes to GPU */
    glBufferData(GL_ARRAY_BUFFER, 
                    mesh.numVertices * vertexSize, &mesh.vertexBuffer[0], 
                    GL_STATIC_DRAW);

    glGenBuffers(1, &mesh.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
    /*  Copy vertex indices to GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                    mesh.numIndices * indexSize, &mesh.indexBuffer[0], 
                    GL_STATIC_DRAW);

    /*  Send vertex attributes to shaders */
    for (int i = 0; i < numAttribs; ++i)
    {
        glEnableVertexAttribArray(vLayout[i].location);
        glVertexAttribPointer(  vLayout[i].location, vLayout[i].size, vLayout[i].type, 
                                vLayout[i].normalized, vertexSize, (void*)vLayout[i].offset);
    }
}

void UpdateGPUMesh(const Mesh &mesh)
{
	glBindVertexArray(mesh.VAO);


	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	/*  Copy vertex attributes to GPU */
	glBufferData(GL_ARRAY_BUFFER,
		mesh.numVertices * vertexSize, &mesh.vertexBuffer[0],
		GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
	/*  Copy vertex indices to GPU */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mesh.numIndices * indexSize, &mesh.indexBuffer[0],
		GL_STATIC_DRAW);
}

/******************************************************************************/
/*!
\fn     void UpdateLightPosViewFrame()
\brief
        Compute view-frame light positions and send them to shader when needed.
*/
/******************************************************************************/
void UpdateLightPosViewFrame()
{
	if (activeControlledObject->isMoved())
    {
        for (int i = 0; i < NUM_LIGHTS; ++i)
            lightPosVF[i] = Vec3(mainCamViewMat * Vec4(lightPosWF[i], 1.0f));

        glUniform3fv(lightPosLoc, NUM_LIGHTS, &lightPosVF[0][0]);
    }
}


/******************************************************************************/
/*!
\fn     void SendLightProperties()
\brief
        Send numLights and intensities to the rendering program.
*/
/******************************************************************************/
void SendLightProperties()
{
    glUniform1i(numLightsLoc, NUM_LIGHTS);

    /*  ambient, diffuse, specular are now reflected components on the object
        surface and can be used directly as intensities in the lighting equation.
    */
    Vec4 ambient, diffuse, specular;
    ambient  = I * ambientAlbedo;
    diffuse  = I * diffuseAlbedo;
    specular = I * specularAlbedo;

    glUniform4fv(ambientLoc, 1, &ambient[0]);
    glUniform4fv(diffuseLoc, 1, &diffuse[0]);
    glUniform4fv(specularLoc, 1, &specular[0]);

    glUniform1i(specularPowerLoc, specularPower);
}


/******************************************************************************/
/*!
\fn     ComputeObjMVMats(Mat4 MVMat[], Mat4 NMVMat[], Mat4 viewMat)
\brief
        Compute the modelview matrices for positions and normals.
\param  MVMat
        Modelview matrix for positions to be computed.
\param  NMVMat
        Modelview matrix for normals to be computed.
\param  viewMat
        Given view matrix.
*/
/******************************************************************************/
void ComputeObjMVMats(Mat4 MVMat[], Mat4 NMVMat[], Mat4 viewMat)
{
    for (int i = 0; i < ObjID::NUM_OBJS; ++i)
    {
        MVMat[i] = viewMat * obj[i].modelMat;
        NMVMat[i] = Transpose(Inverse(MVMat[i]));
    }
}


void ComputeObjMVMat(Mat4& MVMat, Mat4& NMVMat,const Mat4& viewMat,const Mat4& modelMat)
{
        MVMat = viewMat * modelMat;
        NMVMat = Transpose(Inverse(MVMat));
}




/******************************************************************************/
/*!
\fn     void ComputeMainCamMats()
\brief
        Compute the view/projection and other related matrices for user camera.
*/
/******************************************************************************/
void ComputeMainCamMats()
{
    /*  Update view transform matrix */
	if (mainCam.isMoved())
    {
		mainCamViewMat = mainCam.ViewMat();
        ComputeObjMVMats(mainCamMVMat, mainCamNormalMVMat, mainCamViewMat);
    }

    /*  Update projection matrix */
	if (mainCam.resized)
		mainCamProjMat = mainCam.ProjMat();
}


/******************************************************************************/
/*!
\fn     void SendMVMat(const Object &obj)
\brief
        Send object's model-view matrices to the shader.
\param  obj
        The object whose model-view matrices we want to send.
*/
/******************************************************************************/
void SendMVMat(const Mat4 &mvMat, const Mat4 &nmvMat, GLint mvMatLoc, GLint nmvMatLoc)
{
	/* Send per-object transformation matrices to shaders */
	glUniformMatrix4fv(mvMatLoc, 1, GL_FALSE, &mvMat[0][0]);
	glUniformMatrix4fv(nmvMatLoc, 1, GL_FALSE, &nmvMat[0][0]);
}


void SendObjectColor(const vec3& color, GLint objectColorLoc)
{
    glUniform3fv(objectColorLoc, 1, &color[0]);
}


/******************************************************************************/
/*!
\fn     void SendViewMat(Mat4 viewMat, GLint viewMatLoc)
\brief
        Send view matrix to the corresponding variable.
        We send this separately because the skybox rendering only needs 
        view matrix but not modeling or projection matrix.
\param  viewMat
        The view matrix to be sent.
\param  viewMatLoc
        The location of the variable to send to.
*/
/******************************************************************************/
void SendViewMat(Mat4 viewMat, GLint viewMatLoc)
{
	if (mainCam.isMoved())
		glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, &viewMat[0][0]);
}


/******************************************************************************/
/*!
\fn     void SendProjMat()
\brief
        Send projection matrix to the shader.
\param  obj
        The object whose projection matrix we want to send.
*/
/******************************************************************************/
void SendProjMat(Mat4 projMat, GLint projMatLoc)
{
	if (mainCam.resized)
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, &projMat[0][0]);
}


/******************************************************************************/
/*!
\fn     void SetUpObjTextures()
\brief
        Read texture images from files, then copy them to graphics memory.
        These textures will be combined with light colors for the objects 
        in the scene.
*/
/******************************************************************************/
void SetUpObjTextures()
{
    glGenTextures(ImageID::NUM_IMAGES, texID);

    unsigned char *imgData;
    int imgWidth, imgHeight, numComponents;

    /*  Mirror and sphere will not use existing textures so we'll set them up separately */
    for (int i = 0; i < ImageID::NUM_IMAGES; ++i)
    {
        if (ReadImageFile(objTexFile[i], &imgData, &imgWidth, &imgHeight, &numComponents) == 0)
        {
            std::cerr << "Reading " << objTexFile[i] << " failed.\n";
            exit(1);
        }

        /*  Bind corresponding texture ID */
        glBindTexture(GL_TEXTURE_2D, texID[i]);

        /*  Copy image data to graphics memory */
        if (numComponents == 3)
            glTexImage2D(   GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0, 
                            GL_RGB, GL_UNSIGNED_BYTE, imgData);
        else
            glTexImage2D(   GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight, 0, 
                            GL_RGBA, GL_UNSIGNED_BYTE, imgData);

        /*  Done with raw image data so delete it */
        free(imgData);

        /*  Generate texture mipmaps. */
        glGenerateMipmap(GL_TEXTURE_2D);

        /*  Set up texture behaviors */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
}


/******************************************************************************/
/*!
\fn     void SendObjTexID(GLuint texID, int activeTex)
\brief
        Send object's texture ID to the shader.
\param  texID
        The texture object we want to bind to.
\param  activeTex
        The active texture handler we want to use to map with the sampler variable.
\param  texLoc
        The location of the uniform sampler in the shader.
*/
/******************************************************************************/
void SendObjTexID(GLuint texID, int activeTex, GLint texLoc)
{
    glActiveTexture(GL_TEXTURE0 + activeTex);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(texLoc, activeTex);
}




/******************************************************************************/
/*!
\fn     void SetUp()
\brief
        Set up the render program and graphics-related data for rendering.
*/
/******************************************************************************/
void SetUp()
{
    /*  Initialization for fps estimation */
    currTime = clock();
    frameCount = 0;
    secCount = 0;

    /*  Set Up Scene:
        - Obj attributes, which include mesh type, size, pos, color/texture ...
        - Light positions
    */
    SetUpScene();
	for (int i = 0; i < ProgType::NUM_PROGTYPES; ++i)
		prog[i] = CompileShaders(file[i][ShaderType::VERTEX_SHADER], file[i][ShaderType::FRAGMENT_SHADER]);

	//glUseProgram(prog[ProgType::MAIN_PROG]);

    /*  Send mesh data only */
    for (int i = 0; i < MeshID::NUM_MESHES; ++i)
        SetUpVertexData(mesh[i]);


    /*  Set up textures for objects in the scene */
    SetUpObjTextures();
	
	glUseProgram(prog[ProgType::HEAT_MAP_PROG]);
	SetUpMainUniformLocations(prog[ProgType::HEAT_MAP_PROG]);
	SendLightProperties();
	SendObjectColor(vec3(255.f, 255.f, 255.f), objectColorLoc);


	glUseProgram(prog[ProgType::MAIN_PROG]);
	SetUpMainUniformLocations(prog[ProgType::MAIN_PROG]);
    SendLightProperties();
    SendObjectColor(vec3(255.f, 255.f, 255.f), objectColorLoc);

	glUseProgram(prog[activeShaderProgram]);
	SetUpMainUniformLocations(prog[activeShaderProgram]);

    /*  Drawing using filled mode */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    /*  Hidden surface removal */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    /*  Enable writing to depth buffer */
    glDepthMask(GL_TRUE);


    glDisable(GL_CULL_FACE);     /*  For efficiency, not drawing back-face */

	glViewport(0, 0, mainCam.width, mainCam.height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/******************************************************************************/
/*!
\fn     void CleanUp()
\brief
        Clean up the graphics-related stuffs before shutting down.
*/
/******************************************************************************/
void CleanUp()
{
    TwTerminate();      /*  Terminate AntTweakBar */

    glBindVertexArray(0);
    
    for (int i = 0; i < MeshID::NUM_MESHES; ++i)
    {
        glDeleteVertexArrays(1, &mesh[i].VAO);
        glDeleteBuffers(1, &mesh[i].VBO);
        glDeleteBuffers(1, &mesh[i].IBO);
    }

    glDeleteTextures(ImageID::NUM_IMAGES, texID);

	glDeleteProgram(prog[ProgType::MAIN_PROG]);
	glDeleteProgram(prog[ProgType::HEAT_MAP_PROG]);


    exit(0);    /*  successful run */
}


/******************************************************************************/
/*!
\fn     void Resize(int w, int h)
\brief
        Update viewport and projection matrix upon window resize.
\param  w
        The new width of the window
\param  h
        The new height of the window
*/
/******************************************************************************/
void Resize(int w, int h)
{
    mainCam.width = w;
    mainCam.height = h;

	windowHeight = h;
	windowWidth = w;
	TwWindowSize(mainCam.width, mainCam.height);    /*  Send AntTweakBar updated window size */
    glutPostRedisplay();    /*  Set flag to force re-rendering */
}


/******************************************************************************/
/*!
\fn     void EstimateFPS()
\brief
        Estimating FPS. This only updates the FPS about once per second.
*/
/******************************************************************************/
void EstimateFPS()
{
    ++frameCount;

    prevTime = currTime;
    currTime = clock();
    secCount += 1.0f * (currTime - prevTime) / CLOCKS_PER_SEC;

    if (secCount > 1.0f)
    {
        fps = frameCount / secCount;

        frameCount = 0;
        secCount = 0;
    }
}


/******************************************************************************/
/*!
\fn     void RenderObj(const Object &obj)
\brief
        Render an object.
\param  obj
        The object that we want to render.
*/
/******************************************************************************/
void RenderObj(const Object &obj, bool wireframe = false)
{
    /*  Tell shader to use obj's VAO for rendering */
	unsigned drawMode = (wireframe) ? GL_LINES : GL_TRIANGLES;
    glBindVertexArray(mesh[obj.meshID].VAO);
	glDrawElements(drawMode, mesh[obj.meshID].numIndices, GL_UNSIGNED_INT, nullptr);
}





void RenderMeshObj(const Mesh &obj, bool wireframe)
{
	/*  Tell shader to use obj's VAO for rendering */
	unsigned drawMode = (wireframe) ? GL_LINES : GL_TRIANGLES;
	glBindVertexArray(obj.VAO);
	glDrawElements(drawMode, obj.numIndices, GL_UNSIGNED_INT, nullptr);
}

void RenderMeshObjs()
{
    Proto::SceneObjectManager::GetInstance().DrawAll(mainCamViewMat);
}

/******************************************************************************/
/*!
\fn     void RenderObjs(	Mat4 MVMat[], Mat4 normalMVMat[], 
                            Mat4 viewMat, Mat4 projMat, 
                            int viewportWidth, int viewportHeight,
                            int renderPass)
\brief
        Render background and objects in the scene, except the sphere.
\param  MVMat
        The mode-view matrices of the objects.
\param  normalMVMat
        The normal mode-view matrices of the normals on the objects.
\param  viewMat
        The view transform matrix of the camera to render the skybox background.
\param  projMat
        The projection matrix of the camera.
\param  viewportWidth
        The width of the viewport to render to.
\param  viewportHeight
        The height of the viewport to render to.
\param  renderPass
        Whether we are rendering textures for sphere reflection/refraction, or
        mirror reflection, or rendering objects onto the screen.
        We need this flag because each pass only render certain objects.
*/
/******************************************************************************/
void RenderObjs(	Mat4 MVMat[], Mat4 normalMVMat[])
{
    /*  Send object texture and render them */

    for (int i = 0; i < ObjID::NUM_OBJS; ++i)
    {
        SendObjTexID(texID[obj[i].imageID], 0, textureLoc);

        SendMVMat(MVMat[i], normalMVMat[i], mainMVMatLoc, mainNMVMatLoc);

        RenderObj(obj[i]);
    }

}


/******************************************************************************/
/*!
\fn     void Render()
\brief
        Render function for update & drawing.
        This function will perform forward shading if deferredShading == false.
        Otherwise it will perform geometry pass followed by light pass of 
        deferred shading.
*/
/******************************************************************************/
void Render()
{
    Proto::SceneObjectManager& gom = Proto::SceneObjectManager::GetInstance();
    clock_t  cTime;
     
    cTime = clock();
    f32 deltaTime = (float)(cTime - pTime)/1000.f;
	pTime = cTime;
	elapsedtime += deltaTime;


	Proto::SceneObject* pSphere = gom.m_AllActiveObj["sphere"];
    //@TODO for loop recalculating too many things lol
	
	if (renderingModeChanged)
	{
		switch (activeShaderProgram)
		{
		case ProgType::HEAT_MAP_PROG:
			activeShaderProgram = ProgType::MAIN_PROG;
			break;

		case ProgType::MAIN_PROG:
			for (auto it = gom.m_AllActiveObj.begin(); it != gom.m_AllActiveObj.end(); ++it)
			{
				if (it->first == "sphere")
					continue;

				Proto::SceneObject* pOther = it->second;

				activeShaderProgram = ProgType::HEAT_MAP_PROG;
				updateHeatMap(pSphere, pOther);
				updateHeatMap(pOther, pSphere);
			}
			break;

		default:
			break;
		}

		glUseProgram(prog[activeShaderProgram]);
		SetUpMainUniformLocations(prog[activeShaderProgram]);
		mainCam.moved = true;
		mainCam.resized = true;
		renderingModeChanged = false;
	}
	

    glClearBufferfv(GL_COLOR, 0, bgColor);
    glClearBufferfv(GL_DEPTH, 0, &one);

    ComputeMainCamMats();
    bool hasChanged(activeControlledObject->isMoved());
    gom.UpdateAll(deltaTime);

    //recalculate heatmap when needed
	//optimization using spatial partitioning required here
    if (hasChanged && activeControlledObject != &mainCam && activeShaderProgram == ProgType::HEAT_MAP_PROG)
    {
		for (auto it = gom.m_AllActiveObj.begin(); it != gom.m_AllActiveObj.end(); ++it)
		{
			if (it->first == "sphere")
				continue;

			Proto::SceneObject* pOther = it->second;

			updateHeatMap(pSphere, pOther);
			updateHeatMap(pOther, pSphere);


			//Proto::AABB aabb = pOther->GetMeshRenderer()->GetWorldSpaceAABB();
			//Proto::BS bs = pSphere->GetMeshRenderer()->GetWorldSpaceBS();
		}
    }
   
    
    
	UpdateLightPosViewFrame();

	SendProjMat(mainCamProjMat, mainProjMatLoc);

    RenderMeshObjs();
    
    /*  Reset */
	mainCam.moved = false;
	mainCam.resized = false;

    EstimateFPS();
    TwDraw();   /*  Update AntTweakBar */

    glutSwapBuffers();
}



void LoadResources()
{
    Proto::ModelManager& mm = Proto::ModelManager::GetInstance();
    Proto::SceneObject* p_go;
    Proto::GFXComponent*c;
    Proto::SceneObjectManager& gom = Proto::SceneObjectManager::GetInstance();

    dSphereMesh = CreateSphere(8, 8);
    dSphereMesh2 = CreateSphere(8, 8);
    dCubeMesh = CreateCube(1, 1, 1);

    Proto::Model* dSphere = new Proto::Model(dSphereMesh);
    Proto::Model* dSphere2 = new Proto::Model(dSphereMesh2);
    Proto::Model* dMesh = new Proto::Model(dCubeMesh);

    mm.AddModel("MODEL_DSPHERE", dSphere);
    mm.AddModel("MODEL_DSPHERE2", dSphere2);
    mm.AddModel("MODEL_DCUBE", dMesh);

    mm.GetModel("MODEL_DSPHERE")->BuildSphere(mm.GetModel("MODEL_DSPHERE")->GetModelMesh());
    mm.GetModel("MODEL_DSPHERE")->BuildAABB(mm.GetModel("MODEL_DCUBE")->GetModelMesh());
    mm.GetModel("MODEL_DCUBE")->BuildSphere(mm.GetModel("MODEL_DSPHERE")->GetModelMesh());
    mm.GetModel("MODEL_DCUBE")->BuildAABB(mm.GetModel("MODEL_DCUBE")->GetModelMesh());

#if(TEST_NUM == TEST_0)
    
    //str path("config//ModelFileList.txt");
    //mm.LoadAllModels(path);

	//first item
	p_go = &sceneObject0;
	p_go->SetSoInstID(std::string("sphere"));
	p_go->SetPosVec(vec3(-10, 0, -50));
	p_go->SetScaleVec(vec3(5.5f, 5.5f, 5.5f));
	c = p_go->GetMeshRenderer();
	c->SetColorTexture(texID[ImageID::STONE_TEX]);
	c->SetModel("MODEL_DSPHERE2");
	gom.AddSceneObject(p_go);

	//second item
	p_go = &sceneObject1;
	p_go->SetSoInstID(std::string("box"));
	p_go->SetPosVec(vec3(10, 0, -50));
	p_go->SetScaleVec(vec3(10.5f,10.5f, 10.5f));
	c = p_go->GetMeshRenderer();
	c->SetColorTexture(texID[ImageID::POTTERY_TEX]);
	c->SetModel("MODEL_DSPHERE");
	gom.AddSceneObject(p_go);

    //3rd item
    p_go = &sceneObject2;
    p_go->SetSoInstID(std::string("box1"));
    p_go->SetPosVec(vec3(10, 15, -50));
    p_go->SetScaleVec(vec3(10.5f, 10.5f, 10.5f));
    c = p_go->GetMeshRenderer();
    c->SetColorTexture(texID[ImageID::POTTERY_TEX]);
    c->SetModel("MODEL_DCUBE");
    gom.AddSceneObject(p_go);
#elif(TEST_NUM == TEST_1)

    //first item
    p_go = &sceneObject0;
    p_go->SetSoInstID(std::string("sphere"));
    p_go->SetPosVec(vec3(-10, 0, -30));
    p_go->SetScaleVec(vec3(5.5f, 5.5f, 5.5f));
    c = p_go->GetMeshRenderer();
    c->SetColorTexture(texID[ImageID::STONE_TEX]);
    c->SetModel("MODEL_DSPHERE2");
    gom.AddSceneObject(p_go);

    vecSceneObjects.reserve(maxObjCount);
    //@TODO create multiple objects, place them in Octree data struct
    const float startX = -30;
    const float startY = -30;
    const float startZ = -50;
    float curX = startX;
    float curY = startY;
    float curZ = startZ;
    bool toggleCubeModel = true;
    std::string curModelStr;
    std::string instIDStr;
    int texType = 0;
    for (int i = 0; i < maxi; ++i)
    {
        curY = startY + (10.f* i);
        if (toggleCubeModel)
        {
            curModelStr = "MODEL_DCUBE";
            texType = ImageID::STONE_TEX;
            toggleCubeModel = false;
        }
        else
        {
            curModelStr = "MODEL_DSPHERE";
            texType = ImageID::POTTERY_TEX;
            toggleCubeModel = true;
        }
        for (int j = 0; j < maxj; ++j)
        {
            curX = startX + (10.f * j);
            vecSceneObjects.push_back(Proto::SceneObject());
            p_go = &vecSceneObjects.back();
            if (toggleCubeModel == false)
            {
                instIDStr = "cube_" + std::to_string(i) + "_" + std::to_string(j);
            }
            else
            {
                instIDStr = "sphere_" + std::to_string(i) + "_" + std::to_string(j);
            }
            p_go->SetSoInstID(std::string(instIDStr));
            p_go->SetPosVec(vec3(curX, curY, curZ));
            std::cout << curX << " " << curY << " \n";
            p_go->SetScaleVec(vec3(5.5f, 5.5f, 5.5f));
            c = p_go->GetMeshRenderer();
            c->SetColorTexture(texID[texType]);
            c->SetModel(curModelStr);
            gom.AddSceneObject(p_go);
        }
		gom.incrementRenderObjCount();
    }
#endif
}



void TW_CALL ToggleBoundingVolumeVisibility(void *)
{
	drawBoundingVolumes = !drawBoundingVolumes;
}



void TW_CALL SetControlledObjAsCamera(void *)
{

	activeControlledObject = &mainCam;
}


void TW_CALL SetControlledObjAsSceneObj0(void *)
{

	activeControlledObject = &sceneObject0;
}



void TW_CALL SetControlledObjAsSceneObj1(void *)
{
	activeControlledObject = &sceneObject1;
}





void TW_CALL ToggleDrawWireFrame(void *)
{
	drawWireFrame = !drawWireFrame;
}

void TW_CALL ToggleRenderingMode(void *)
{
	renderingModeChanged = true;
}





void updateHeatMap(Proto::SceneObject* shadedObject, Proto::SceneObject* opposingObject)
{
	if (shadedObject == nullptr || opposingObject == nullptr) return;

	if (!shadedObject->GetMeshRenderer()  || !opposingObject->GetMeshRenderer()) return;

	auto& shadedMeshRenderer	= *shadedObject->GetMeshRenderer();
	auto& opposingMeshRenderer  = *opposingObject->GetMeshRenderer();

	if (shadedMeshRenderer.GetModel() == nullptr || opposingMeshRenderer.GetModel() == nullptr) return;

	auto& shadedModel   = *shadedMeshRenderer.GetModel();
	auto& opposingModel = *opposingMeshRenderer.GetModel();
	auto& shadedMesh	= shadedModel.GetModelMesh();
	auto& opposingMesh  = opposingModel.GetModelMesh();
	const auto& shadedObjectMTW	= shadedObject->GetMWMatrix();
	const auto shadedNormalMTW	= Mat3(Transpose(Inverse(shadedObjectMTW)));
	//const auto shadedNormalMTW = Mat3(shadedObjectMTW);

	const auto& opposingObjectMTW = opposingObject->GetMWMatrix();

	auto& shadedVertices = shadedMesh.vertexBuffer;
	auto& opposingVertices = opposingMesh.vertexBuffer;

	u32 total = shadedVertices.size();
	f32 timeOfIntersection(std::numeric_limits<f32>::max());
	
	//for each shaded vertex
	for (u32 i = 0; i < total; ++i)
	{
		auto& shadedVertex = shadedMesh.vertexBuffer[i];
		//reset heatmap value for vertex
		shadedVertex.heatmap.x = 0.5f;
		shadedVertex.heatmap.y = 1.f;
		//get world space position of vertex and normal direction of shaded vertex
		Vec3 worldSpacePosition = Vec3(shadedObjectMTW * Vec4(shadedVertex.pos, 1.f));
		Vec3 worldSpaceNormal = shadedNormalMTW * shadedVertex.nrm;
		worldSpaceNormal = Normalise(worldSpaceNormal);

		//spatial partioning traversal decision function
		auto IsInPartition = [worldSpacePosition, worldSpaceNormal](const HierachicalAABBNode& a)->bool
		{
			Vec3 intersectionPoint;
			f32 timeOfIntersection;
			bool flag = IntersectRayAABB(worldSpacePosition, worldSpaceNormal, a.m_AABB, timeOfIntersection, intersectionPoint);
			//ray cast in the opposite direction if there is no collision detected
			if (!flag)
				flag = IntersectRayAABB(worldSpacePosition, -worldSpaceNormal, a.m_AABB, timeOfIntersection, intersectionPoint);
			return flag;
		};


		bool hasCollision(false);
		f32 bestTime(timeOfIntersection);
		bool isTriangleBehindVertex(false);

		auto Visitor = [&hasCollision, &bestTime, &isTriangleBehindVertex, 
			&shadedVertex, &opposingVertices, &opposingObjectMTW, 
			worldSpacePosition, worldSpaceNormal](const HierachicalAABBNode& a)->void
		{
			//is in leaf node
			if (a.m_LeftChild == -1 || a.m_RightChild == -1)
			{
				u32 total2 = a.triangleIndices.size();
				

				for (u32 j = 0; j < total2; ++j)
				{
					f32 distance(std::numeric_limits<f32>::max());

					auto& triangle = a.triangleIndices[j];
					auto v0 = opposingVertices[triangle[0]].pos;
					auto v1 = opposingVertices[triangle[1]].pos;
					auto v2 = opposingVertices[triangle[2]].pos;

					v0 = Vec3(opposingObjectMTW * Vec4(v0, 1.f));
					v1 = Vec3(opposingObjectMTW * Vec4(v1, 1.f));
					v2 = Vec3(opposingObjectMTW * Vec4(v2, 1.f));
					
					if (Proto::IntersectRayTriangle(worldSpacePosition, worldSpaceNormal, v0, v1, v2, distance))
					{
						if (bestTime > distance)
						{
							bestTime = distance;
							isTriangleBehindVertex = false;
							hasCollision = true;
						}
					}
					//check the opposite of the normal direction as well
					if (Proto::IntersectRayTriangle(worldSpacePosition, -worldSpaceNormal, v0, v1, v2, distance))
					{
						if (bestTime > distance)
						{
							bestTime = distance;
							isTriangleBehindVertex = true;
							hasCollision = true;
						}
					}
				}

			}
		};

		
		opposingMeshRenderer.GetWorldSpaceHAABB().VisitNodes(Visitor, IsInPartition);
		if (hasCollision)
		{
			//rescale value to a range
			//bestTime *=50;
			//clamp value
			bestTime = (bestTime > 0.5f) ? 0.5f : bestTime;
			//invert value if behind the vertex
			bestTime = (isTriangleBehindVertex) ? bestTime + 0.5f: 0.5f - bestTime;
			shadedVertex.heatmap.x = bestTime; 
			shadedVertex.heatmap.y = 0.f;
		}
		else
		{
			shadedVertex.heatmap.x = 0.5f;
			shadedVertex.heatmap.y = 1.f;
		}
	}
	UpdateGPUMesh(shadedMesh);
}




/*  YAY ! END OF THE SUPER TEDIOUS DOCUMENTATION !!! */