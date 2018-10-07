/******************************************************************************/
/*!
\file   camera.cpp
\par    Purpose: Implementation of camera class
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   27/3/2015
*/
/******************************************************************************/

#include "camera.hpp"


/*  Pre-defined camera */




/******************************************************************************/
/*!
\fn     void Camera::Reset()
\brief
        Reset the camera to its original position and orientation.
*/
/******************************************************************************/
void Camera::
Reset()
{
    radius = INIT_RADIUS;
    alpha = INIT_ALPHA;
    beta = INIT_BETA;
    upVec = BASIS[Y];

    moved = true;
}


/******************************************************************************/
/*!
\fn     void Camera::MoveUp()
\brief
        Increase cam alpha by CAM_ANGLE_STEP and rewind if needed, so that
        -NUM_STEPS_HALF_PI <= alpha < NUM_STEPS_ONEANDHALF_PI
*/
/******************************************************************************/
void Camera::RotateUp()
{
    /*  Move camera up */
    alpha -= CAM_ANGLE_STEP;

    /*  Keep alpha within [-90, 270) */
    while (alpha >= NUM_STEPS_ONEANDHALF_PI)
        alpha -= NUM_STEPS_TWO_PI;

    moved = true;
}

void Camera::RotateUp(f32 d)
{
	/*  Move camera up */
	alpha -= d;

	/*  Keep alpha within [-90, 270) */
	while (alpha >= NUM_STEPS_ONEANDHALF_PI)
		alpha -= NUM_STEPS_TWO_PI;

	moved = true;
}


/******************************************************************************/
/*!
\fn     void Camera::MoveDown()
\brief
        Decrease cam alpha by CAM_ANGLE_STEP and rewind if needed, so that
        -NUM_STEPS_HALF_PI <= alpha < NUM_STEPS_ONEANDHALF_PI
*/
/******************************************************************************/
void Camera::
RotateDown()
{
    /*  Move camera down */
    alpha += CAM_ANGLE_STEP;

    /*  Keep alpha within [-90, 270) */
    while (alpha < -NUM_STEPS_HALF_PI)
        alpha += NUM_STEPS_TWO_PI;

    moved = true;
}

void Camera::
RotateDown(f32 d)
{
	/*  Move camera down */
	alpha += d;

	/*  Keep alpha within [-90, 270) */
	while (alpha < -NUM_STEPS_HALF_PI)
		alpha += NUM_STEPS_TWO_PI;

	moved = true;
}


/******************************************************************************/
/*!
\fn     void Camera::MoveLeft()
\brief
        Increase cam beta by CAM_ANGLE_STEP and rewind if needed, so that
        0 <= alpha < NUM_STEPS_TWO_PI
*/
/******************************************************************************/
void Camera::
RotateLeft()
{
    /*  Move camera left */
    beta -= CAM_ANGLE_STEP;

    /*  Keep beta within [0, 360) */
    while (beta >= NUM_STEPS_TWO_PI)
        beta -= NUM_STEPS_TWO_PI;

    moved = true;
}

void Camera::
RotateLeft(f32 d)
{
	/*  Move camera left */
	beta -= d;

	/*  Keep beta within [0, 360) */
	while (beta >= NUM_STEPS_TWO_PI)
		beta -= NUM_STEPS_TWO_PI;

	moved = true;
}


/******************************************************************************/
/*!
\fn     void Camera::MoveRight()
\brief
        Decrease cam beta by CAM_ANGLE_STEP and rewind if needed, so that
        0 <= alpha < NUM_STEPS_TWO_PI
*/
/******************************************************************************/
void Camera::
RotateRight()
{
    /*  Move camera right */
    beta += CAM_ANGLE_STEP;

    /*  Keep beta within [0, 360) */
    while (beta < 0)
        beta += NUM_STEPS_TWO_PI;

    moved = true;
}


void Camera::
RotateRight(f32 d)
{
	/*  Move camera left */
	beta += d;

	/*  Keep beta within [0, 360) */
	while (beta >= NUM_STEPS_TWO_PI)
		beta -= NUM_STEPS_TWO_PI;

	moved = true;
}


/******************************************************************************/
/*!
\fn     void Camera::MoveCloser()
\brief
        Decrease radius by CAM_RADIUS_STEP but cap it at CAM_MIN_RADIUS
*/
/******************************************************************************/
void Camera::
MoveCloser()
{
	pos -= camViewVec * 1.f;
	moved = true;
}


/******************************************************************************/
/*!
\fn     void Camera::MoveFarther()
\brief
        Increase radius by CAM_RADIUS_STEP but cap it at CAM_MAX_RADIUS
*/
/******************************************************************************/
void Camera::
MoveFarther()
{
	pos += camViewVec * 1.f;

	moved = true;
}



void Camera::MoveUp()
{
	this->pos += this->camUpVec * 1.f;
	moved = true;
}
void Camera::MoveDown()
{

	this->pos -= this->camUpVec * 1.f;
	moved = true;
}

void Camera::MoveLeft()
{
	this->pos -= this->camSideVec * 1.f;
	moved = true;
}
void Camera::MoveRight()
{
	this->pos += this->camSideVec * 1.f;
	moved = true;
}


bool Camera::isMoved()
{
	return moved;
}

/******************************************************************************/
/*!
\fn     void Camera::ComputeUpVec()
\brief
        Compute a reasonable up vector depending on the current alpha and beta,
        so that the camera coordinate frame is constructed correctly.
*/
/******************************************************************************/
void Camera::
ComputeUpVec()
{
    if (alpha == NUM_STEPS_HALF_PI)
        upVec = Mat3(Rotate(-ONE_STEP * beta, BASIS[Y])) * -BASIS[X];
    else
    if (alpha == -NUM_STEPS_HALF_PI)
        upVec = Mat3(Rotate(-ONE_STEP * beta, BASIS[Y])) * BASIS[X];
    else
    if (alpha < NUM_STEPS_HALF_PI)
        upVec = BASIS[Y];
    else
        upVec = -BASIS[Y];
}


/******************************************************************************/
/*!
\fn     Mat4 Camera::ViewMat()
\brief
        Compute view transformation matrix for a camera that looks at the origin,
        based on the camera radius (distance to the origin), alpha (angle between
        origin-camera vector and xz-plane), and beta (angle between origin-camera
        vector and xy-plane).
\return
        The resulting view transformation matrix
*/
/******************************************************************************/
const Mat4& Camera::
ViewMat()
{
    ComputeUpVec();
    
    float alphaAng = ONE_STEP * alpha;
    float betaAng = ONE_STEP * beta;

    float ca = std::cosf(alphaAng);
    float sa = std::sinf(alphaAng);
    float cb = std::cosf(betaAng);
    float sb = std::sinf(betaAng);

    float eyeProjXZ = radius * ca;
	lookAt = pos  -Vec3(eyeProjXZ * cb, radius * sa, eyeProjXZ * sb);
	viewMat = LookAt(pos, lookAt, upVec);


	invViewMat = Inverse(viewMat);
	camSideVec = vec3(invViewMat[0]);
	camUpVec = vec3(invViewMat[1]);
	camViewVec = vec3(invViewMat[2]);
	return viewMat;
}



const Mat4& Camera::
InvViewMat()
{
	return invViewMat;
}


/******************************************************************************/
/*!
\fn     Mat4 Camera::ProjMat() const
\brief
        Wrapper for glm::Frustum function.
\return
        The resulting perspective transformation matrix
*/
/******************************************************************************/
Mat4 Camera::
ProjMat() const
{
    return Frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
}