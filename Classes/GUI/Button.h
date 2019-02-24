#include "cocos2d.h"
#include "ui/UIButton.h"
using namespace cocos2d;

#ifndef BUTTON_H
#define BUTTON_H

class Button
{
public:
	ui::Button* button;
	Button();
	Button(int typeButton, std::string title);
	Button(int typeButton, std::string title, Color3B color);
	void createButton(int typeButton, std::string title);
	ui::Button* getButton();
	static const int LIGHT_BROWN = 1;
	static const int DARK_BROWN = 2;
	static const int LIGHT_GRAY = 3;
	static const int DARK_GRAY = 4;
	static const int BACK_BIG = 5;
	static const int BACK_YELLOW = 6;
	void setTextButton(std::string titleButton);
	void setColorTitle(Color3B color);
	void setPosition(Vec2 v);
	Vec2 getPosition();
	void setTitleText(std::string title);
	void setVisible(bool isVisible);
	void setEnabled(bool isEnable);
	bool isEnabled();
private:
	
};

#endif