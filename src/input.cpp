/******************************************************************************/
/*!
\file   input.cpp
\par    Purpose: Handle events upon user inputs
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   8/2/2015
*/
/******************************************************************************/

#include "input.hpp"
#include "SceneObjectManager.h"
#include "graphics.hpp"
#include "GFXComponent.h"
#include "SceneObject.h"
#include "math.hpp"
/******************************************************************************/
/*!
\fn     void Keyboard(unsigned char key, int x, int y)
\brief
        Events when a key is pressed.
\param  key
        Pressed key.
\param  x
        Current x-position of the mouse cursor.
\param  y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/

extern IControlledSceneObject* activeControlledObject;
void Keyboard(unsigned char key, int x, int y)
{

	if (TwEventKeyboardGLUT(key, x, y))
	{
		return;
	}

	if (activeControlledObject == nullptr) return;


    switch (key)
    {
        case 27:    /*  ESC */
            CleanUp();


		case 'w':
		case 'W':
			activeControlledObject->MoveCloser();

			break;

		case 's':
		case 'S':
			activeControlledObject->MoveFarther();

			break;

		case 'a':
		case 'A':
			activeControlledObject->MoveLeft();

			break;

		case 'd':
		case 'D':
			activeControlledObject->MoveRight();

			break;


		case 'q':
		case 'Q':
			activeControlledObject->MoveDown();

			break;


		case 'e':
		case 'E':
			activeControlledObject->MoveUp();

			break;


		case 'r':
		case 'R':
			activeControlledObject->RotateLeft();

			break;


		case 't':
		case 'T':
			activeControlledObject->RotateRight();

			break;


        case ' ':
			//activeControlledCamera->Reset();
            break;
    }

    glutPostRedisplay();    /*  Set flag to force re-rendering */
}

/******************************************************************************/
/*!
\fn     void SpecialKeyboard(int key, int x, int y)
\brief
        Events when a special key is pressed. This include CTRL+ ..., ALT+ ...
\param  key
        Pressed key.
\param  x
        Current x-position of the mouse cursor.
\param  y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void SpecialKeyboard(int key, int x, int y)
{
    /* ALT+F4: Quit */
    if (key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        CleanUp();
    }
    else
    /*  For now we're not handling any other special keys */
    if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT)
    {
    }
}


/******************************************************************************/
/*!
\fn     void MouseClick(int button, int state, int x, int y)
\brief
        Events when a mouse button is clicked. 
        These include AntTweakBar behaviors too.
\param  button
        Clicked button.
\param  state
        Up/down state of the button.
\param  x
        Current x-position of the mouse cursor.
\param  y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void MouseClick(int button, int state, int x, int y)
{
    /*  If the mouse is clicked when it is over the AntTweakBar */
    if (TwEventMouseButtonGLUT(button, state, x, y))
    {

        glutPostRedisplay();
    }
	else
	{

	}
    /*  Otherwise don't do anything for now */
}

void MousePan(int x, int y)
{


}