/******************************************************************************/
/*!
\file   input.hpp
\par    Purpose: Handle events upon user inputs
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   8/2/2015
*/
/******************************************************************************/

#ifndef INPUT_HPP
#define INPUT_HPP

#include "graphics.hpp"

/*  Public functions */
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);

void MouseClick(int button, int state, int x, int y);
void MousePan( int x, int y);

#endif

