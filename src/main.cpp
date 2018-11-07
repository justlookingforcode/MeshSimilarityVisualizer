/******************************************************************************/
/*!
\file   main.cpp
\par    Purpose: Main file of the program
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   27/3/2015
*/
/******************************************************************************/

#include <iostream>
#include <iomanip>
#include <ctime>
#include "input.hpp"
#include "graphics.hpp"

#include "DumbSceneGraph.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


/******************************************************************************/
/*!
@fn     void CreateGLContext(int argc, char **argv)
@brief
        Create the context for OpenGL commands to work.
@param  argc
        argc from main function
@param  argv
        argv from main function
*/
/******************************************************************************/

void DummyTimerFunc(int value){}

void CreateGLContext(int argc, char **argv)
{
    /* GLUT Window Creation & Initialization */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(mainCam.width, mainCam.height);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("Room");

    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

}


/******************************************************************************/
/*!
@fn     void InitGLExtensions()
@brief
        Initialize the OpenGL extensions supported on the current platform.
*/
/******************************************************************************/
void InitGLExtensions()
{
    /* GL3W Initialization */
    if (gl3wInit())
    {
        std::cerr << "Failed to initialize OpenGL.\n";
        exit(1);
    }

    if (!gl3wIsSupported(3, 3))
    {
        std::cerr << "OpenGL 3.3 not supported.\n";
        exit(1);
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL version  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}


/******************************************************************************/
/*!
@fn     void CreateAntTweakBar()
@brief
        Create the AntTweakBar to display information and receive user inputs.
*/
/******************************************************************************/
void CreateAntTweakBar()
{
    TwInit(TW_OPENGL_CORE, NULL);

    TwBar *myBar;
    myBar = TwNewBar("Parameters");
    TwDefine("Parameters color='50 50 50' alpha=10 fontSize=3 position='10 10' size='300 500' valuesWidth=50 ");

    /*  Displaying FPS */
	TwAddVarRO(myBar, "Frame Rate", TW_TYPE_FLOAT, &fps, ""); 
	TwAddButton(myBar, "ToggleHeatMap", ToggleRenderingMode, NULL, " label='Toggle Heat Map' group='' ");
	TwAddButton(myBar, "ControlCamera", SetControlledObjAsCamera, NULL, " label='Control camera' oup='' ");
	TwAddButton(myBar, "ControlSceneObject0", SetControlledObjAsSceneObj0, NULL, " label='Control scene obj 0' group='' ");
	TwAddButton(myBar, "ControlSceneObject1", SetControlledObjAsSceneObj1, NULL, " label='Control scene obj 1' group='' ");
    TwAddButton(myBar, "ControlSceneObject2", SetControlledObjAsSceneObj2, NULL, " label='Control scene bunny' group='' ");

    TwAddSeparator(myBar, "misc", "group='Other'");
    TwAddButton(myBar, "ToggleRotateModel", ToggleRotateModel, NULL, " label='Toggle Rotate Model' group='' ");
    TwAddButton(myBar, "ToggleWireFrame", ToggleDrawWireFrame, NULL, " label='Toggle Wire Frame' group='' ");
    TwAddButton(myBar, "BoundingVolumesUsed", ToggleBoundingVolumeVisibility, NULL, " label='Toggle Draw BV' group='Bounding_Volumes' ");
    TwAddVarRO(myBar, "RenderedDeptha", TW_TYPE_UINT32, &u8CurrentBSPDepth, " min=1 max=7 step=1 group='Bounding_Volumes' label='Rendered Depth' ");
    TwAddButton(myBar, "RaiseDepth", IncrementDepth, NULL, " label='Increase Depth' group='Bounding_Volumes' ");
    TwAddButton(myBar, "LowerDepth", DecrementDepth, NULL, " label='DecreaseDepth' group='Bounding_Volumes' ");
}


/******************************************************************************/
/*!
@fn     void MainLoop()
@brief
        The main loop of OpenGL rendering and user interactions.
*/
/******************************************************************************/
void MainLoop()
{
    /* GLUT callback loop */
    glutDisplayFunc(Render);
    glutIdleFunc(Render);

    glutReshapeFunc(Resize);

    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeyboard);

    glutMouseFunc(MouseClick);
	glutMotionFunc(MousePan);
    glutMainLoop();
}


/******************************************************************************/
/*!
@fn     main
@brief
        Entry point of the program.
@param  argc
        Number of arguments when the program is executed.
@param  argv
        The array of arguments when the program is executed.
*/
/******************************************************************************/
void main(int argc, char **argv)
{
    /*  This function can be used in Windows to disable the console.
        Currently, we still keep our console for debugging purpose. */
    //FreeConsole();


    CreateGLContext(argc, argv);
    InitGLExtensions();

    SetUp();

    CreateAntTweakBar();
    LoadResources();
    /*  The main loop for user drawing and interactions */
    MainLoop();




	return;
}