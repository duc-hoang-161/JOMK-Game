#include "GUI\Alert.h"

Alert::Alert()
{
	this->layer = Layer::create();
}

Alert::Alert(std::string title, std::string contentText, int typeFormSize)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//CREATE LAYER
	this->layer = Layer::create();
	layer->setContentSize(visibleSize);
	int rateSize = 2;
	switch (typeFormSize)
	{
	case MEDIUM_SIZE:
	{
		rateSize = 2;
		break;
	}
	case LARGE_SIZE:
	{
		rateSize = 3;
		break;
	}
		
	case CONFIRM_SIZE:
	{
		rateSize = 2;
		break;
	}
		
	}
	//create window 
	alertWindow = Sprite::create("UI/button/alert-background.png");
	alertWindow->setName("alert");
	alertWindow->setPosition(visibleSize / 2);
	alertWindow->setScale(visibleSize.width * rateSize / (4 * alertWindow->getContentSize().width), visibleSize.height * rateSize / (4 * alertWindow->getContentSize().height));
	layer->addChild(alertWindow, 2);
	auto windowSize = alertWindow->getBoundingBox().size;
	//create title window
	titleLabel = Label::createWithTTF(title, "UI/font/SVN-Prima.ttf", 30);
	titleLabel->setAnchorPoint(Vec2(0.5, 1));
	titleLabel->setAlignment(TextHAlignment::CENTER);
	//titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*43/64));
	titleLabel->setName("title");
	layer->addChild(titleLabel, 3);
	if (typeFormSize != CONFIRM_SIZE)
	{
		titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + windowSize.height * 11 / 32));
		//create content containner
		contentContainer = Layer::create();
		scrollContainer = ui::ScrollView::create();
		scrollContainer->setDirection(ui::ScrollView::Direction::VERTICAL);
		scrollContainer->setBackGroundColor(Color3B::BLUE);
		scrollContainer->setContentSize(Size(windowSize.width * 4 / 5, windowSize.height * 5 / 7));
		scrollContainer->setScrollBarAutoHideEnabled(true);
		scrollContainer->setScrollBarAutoHideTime(-1);
		scrollContainer->setClippingEnabled(true);
		scrollContainer->setAnchorPoint(Vec2(0.5, 0.5));
		scrollContainer->setLayoutType(ui::Layout::Type::VERTICAL);
		scrollContainer->setInnerContainerSize(visibleSize/2);
		scrollContainer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 15 / 32));
		//create content text
		contentLabel = ui::Text::create();
		contentLabel->setFontName("UI/font/SVN-Prima.ttf");
		contentLabel->setFontSize(20);
		contentLabel->ignoreContentAdaptWithSize(true);
		contentLabel->setString(contentText);
		contentLabel->setTextHorizontalAlignment(TextHAlignment::LEFT);
		contentLabel->setTextVerticalAlignment(TextVAlignment::CENTER);
		auto param1 = ui::LinearLayoutParameter::create();
		param1->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
		param1->setMargin(ui::Margin(0, 5, 0, 25));
		contentLabel->setLayoutParameter(param1);
		contentLabel->setTextAreaSize(Size(scrollContainer->getContentSize().width, 0));
		contentLabel->setPosition(Vec2(scrollContainer->getContentSize().width/2, scrollContainer->getContentSize().height -100));
		contentLabel->setName("content");
		scrollContainer->setInnerContainerSize(contentLabel->getContentSize());
		scrollContainer->addChild(contentLabel, 4);
		scrollContainer->setName("scroll");
		contentContainer->addChild(scrollContainer, 3);
		layer->addChild(contentContainer, 3);
	
		button = new Button(Button::DARK_GRAY, "BACK");
		button->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - windowSize.height / 2));
		layer->addChild(button->getButton(), 5);
		button->getButton()->setName("button");
	}
	else
	{
		titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + windowSize.height * 2 / 10));
		titleLabel->setTextColor(Color4B::BLACK);
		alertWindow->setScaleY(0.5);
		button = new Button(Button::DARK_GRAY, "CANCEL");
		button->getButton()->setPosition(Vec2(visibleSize.width * 5 / 8, visibleSize.height / 2 - windowSize.height / 5));
		layer->addChild(button->getButton(), 5);
		button->getButton()->setName("buttoncancel");

		button1 = new Button(Button::DARK_BROWN, "OK", Color3B::BLACK);
		button1->getButton()->setPosition(Vec2(visibleSize.width * 3 / 8, visibleSize.height / 2 - windowSize.height / 5));
		layer->addChild(button1->getButton(), 5);
		button1->getButton()->setName("buttonok");
	}
	layer->setVisible(false);

	/*
	log("CONTENT SIZE %f | %f", layer->getContentSize().width, layer->getContentSize().height);
	log("LAYER POSITION IN ITSEFL %f | %f", titleLabel->getPositionX(), titleLabel->getPositionY());*/
}

Layer* Alert::getAlertLayer()
{
	return this->layer;
}

Layer* Alert::getLayerContent()
{
	return this->contentContainer;
}


Sprite* Alert::getAlertWindow()
{
	return this->alertWindow;
}
void Alert::setTitleAlert(std::string title)
{
	this->titleLabel->setString(title);
}
void Alert::setTitleAlert(std::string title, float rate)
{
	this->titleLabel->setString(title);
	this->titleLabel->setScale(rate);
}
void Alert::setContentText(std::string content)
{
	this->contentLabel->setString(content);
}
void Alert::setLayerContent(Layer* newLayerContent)
{
	this->contentContainer = newLayerContent;
}

Button* Alert::getActionButton()
{
	return this->button;
}

Button* Alert::getRightActionButton()
{
	return this->button;
}

Button* Alert::getLeftActionButton()
{
	return this->button1;
}

Label* Alert::getTitleLabel()
{
	return this->titleLabel;
}

void Alert::setVisible(bool isVisible)
{
	this->layer->setVisible(isVisible);
}

bool Alert::isVisible()
{
	return this->layer->isVisible();
}

ui::Text* Alert::getContentLabel()
{
	return this->contentLabel;
}


ui::ScrollView* Alert::getScroll()
{
	return this->scrollContainer;
}

Layer* Alert::getLayerInner()
{
	return this->layer;
}