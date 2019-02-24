#include "GUI\Button.h"

Button::Button()
{
	this->button = ui::Button::create();
}

Button::Button(int typeButton, std::string title)
{
	
	createButton(typeButton, title);
	this->button->setTitleColor(Color3B::BLACK);
}

Button::Button(int typeButton, std::string title, Color3B color)
{
	createButton(typeButton, title);
	this->button->setTitleColor(color);
}

void Button::createButton(int typeButton, std::string title)
{
	switch (typeButton)
	{
	case 1:
	{
			  this->button = ui::Button::create("UI/button/b1.png", "UI/button/b1-pressed.png");
			  break;

	}
	case 2:
	{
			  this->button = ui::Button::create("UI/button/b3.png", "UI/button/b3-pressed.png");

			  break;
	}
	case 3:
	{
			  this->button = ui::Button::create("UI/button/b4.png", "UI/button/b4-pressed.png");
			  break;
	}
	case 4:
	{
			  this->button = ui::Button::create("UI/button/b2.png", "UI/button/b2-pressed.png");
			  break;
	}
	case 5:
	{
			  this->button = ui::Button::create("UI/button/btn-back-1.png", "UI/button/btn-back-1-pressed.png");
			  break;
	}
	case 6:
	{
			  this->button = ui::Button::create("UI/button/btn-back.png", "UI/button/btn-back-pressed.png");
			  break;
	}
	default:
		this->button = ui::Button::create("UI/button/b1.png", "UI/button/b1-pressed.png");
		break;
	}
	this->button->setTitleText(title);
	this->button->setTitleFontSize(25);
	this->button->setTitleFontName("UI/font/SVN-Prima.ttf");
}

ui::Button* Button::getButton()
{
	return this->button;
}

void Button::setTextButton(std::string titleButton)
{
	this->button->setTitleText(titleButton);
}

void Button::setColorTitle(Color3B color)
{
	this->button->setTitleColor(color);
}

void Button::setPosition(Vec2 v)
{
	this->button->setPosition(v);
}

Vec2 Button::getPosition()
{
	return this->button->getPosition();
}

void Button::setTitleText(std::string title)
{
	this->button->setTitleText(title);
}

void Button::setVisible(bool isVisible)
{
	this->button->setVisible(isVisible);
}

void Button::setEnabled(bool isEnable)
{
	this->button->setEnabled(isEnable);
}

bool Button::isEnabled()
{
	return this->button->isEnabled();
}