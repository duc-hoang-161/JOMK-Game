#ifndef BASE_UI_H
#define BASE_UI_H

#include "Import\AudioConfig.h"
#include "cocos2d.h"
#include "GUI\Button.h"
#include "ui/UIScrollView.h"
#include "ui/UIText.h"
using namespace cocos2d;

class BaseUI
{
protected:
	Layer* mainLayer;
	Sprite* background;
	Layer* headerLayer;
	Sprite* headerBackground;
	Sprite* iconUnit;
	Label* numberOfCoin;
	Label* headerLabel;
	Layer* rightSideLayer;
	Button* btnBack;
	Button* firstActionButton;
	Button* secActionButton;
	Button* thirdActionButton;
	Label* detailLabelTitle;
	Sprite* detailBackground;
	ui::ScrollView* scrollDetail;
	ui::Text* detailText;
	Sprite* contentBackground;
	Layer* contentLayer;

public:
	BaseUI();
	void addListener();
	//set function
	void setBackground(std::string src);
	void setTitleHeader(std::string titleHeader);
	void setIconUnit(std::string src);
	void setNumberValue(int number);
	void setActionFirstButton(std::string title, std::function<void()> callback);
	void setActionSecButton(std::string title, std::function<void()> callback);
	void setActionThirdButton(std::string title, std::function<void()> callback);
	//get function
	Label* getCoinLabel();
	ui::ScrollView* getDetailPanel();


	
};

#endif