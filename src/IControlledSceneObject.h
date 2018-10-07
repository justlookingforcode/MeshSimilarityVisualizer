
#ifndef I_CONTROLLED_SCENE_ENTITY_H
#define I_CONTROLLED_SCENE_ENTITY_H

class IControlledSceneObject
{
public:
	virtual void MoveUp() = 0;
	virtual void MoveDown() = 0;
	virtual void MoveLeft() = 0;
	virtual void MoveRight() = 0;
	virtual void MoveCloser() = 0;
	virtual void MoveFarther() = 0;

	virtual void RotateUp() = 0;
	virtual void RotateDown() = 0;
	virtual void RotateLeft() = 0;
	virtual void RotateRight() = 0;


	virtual bool isMoved() = 0;
};

#endif