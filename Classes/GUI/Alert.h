#include "cocos2d.h"
#include "ui/UIScrollView.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "GUI\Button.h"
using namespace cocos2d;

#ifndef ALERT_H
#define ALERT_H

class Alert
{
private:
	Sprite* alertWindow;
	Label* titleLabel;
	ui::Text* contentLabel;
	Layer* contentContainer;
	ui::ScrollView* scrollContainer;
	Button* button;
	Button* button1;
	Layer* layer;
public:
	
	Alert();
	Alert(std::string title, std::string contentText, int typeFormSize);
	Layer* getAlertLayer();
	Layer* getLayerContent();
	Sprite* getAlertWindow();
	Label* getTitleLabel();
	void setTitleAlert(std::string title);
	void setTitleAlert(std::string title, float rate);
	void setContentText(std::string content);
	ui::Text* getContentLabel();
	ui::ScrollView* getScroll();
	Layer* getLayerInner();
	void setLayerContent(Layer* newLayerContent);
	Button* getActionButton();
	Button* getRightActionButton();
	Button* getLeftActionButton();
	void setVisible(bool isVisible);
	bool isVisible();
	static const int MEDIUM_SIZE = 2;
	static const int LARGE_SIZE = 3;
	static const int CONFIRM_SIZE = 4;
};

#endif