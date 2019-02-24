#ifndef __DOOR_H__
#define __DOOR_H__

#include "Objects\DynamicGameObject.h"

class DynamicGameObject;

class Door : public DynamicGameObject {
private:
	static const int OPEN;
	bool m_isOpened;
	bool m_isResetPB;
public:
	Door(string animationFileDirect);

	void open();
	void update(float deltaTime);

	std::string className() { return "Door"; }
};

#endif