#include "GUI\BaseUI.h"


BaseUI::BaseUI()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	this->mainLayer = Layer::create();
	mainLayer->setContentSize(visibleSize);
	mainLayer->setAnchorPoint(Vec2(0, 0));

	this->headerBackground = Sprite::create("UI/button/header-coin.png");
	this->headerBackground->setScaleX(1000 / this->headerBackground->getContentSize().width);
	this->headerBackground->setScaleY(120 / this->headerBackground->getContentSize().height);
	this->headerBackground->setPosition(Vec2(visibleSize.width*5/12, visibleSize.height * 10 / 11));
	mainLayer->addChild(this->headerBackground, 0);


	this->numberOfCoin = Label::createWithTTF("98765", "UI/font/SVN-Prima.ttf", 25);
	this->numberOfCoin->setTextColor(Color4B(255, 83, 0, 255));
	this->numberOfCoin->setPosition(Vec2(visibleSize.width * 1.5 / 12, visibleSize.height * 10 / 11));
	this->numberOfCoin->setAnchorPoint(Vec2(0, 0.5));
	mainLayer->addChild(this->numberOfCoin, 1);
	

	this->headerLabel = Label::createWithTTF("NAME BUILDING", "UI/font/UTM Ong Do Gia.ttf", 35);
	this->headerLabel->setTextColor(Color4B::BLACK);
	this->headerLabel->setPosition(Vec2(visibleSize.width * 4 / 9, visibleSize.height * 10 / 11));
	mainLayer->addChild(headerLabel, 1);

	this->btnBack = new Button(Button::BACK_YELLOW, "    Back", Color3B(234, 107, 10));
	this->btnBack->getButton()->setPosition(Vec2(visibleSize.width*9/10, visibleSize.height*10/11));
	mainLayer->addChild(this->btnBack->getButton(), 0);

	this->contentBackground = Sprite::create("UI/button/containner-bg.png");
	this->contentBackground->setScaleX(1000 / this->contentBackground->getContentSize().width);
	this->contentBackground->setScaleY(visibleSize.height * 8 / (10 * this->contentBackground->getContentSize().height));
	this->contentBackground->setPosition(Vec2(visibleSize.width * 5 / 12, visibleSize.height * 5 / 12));
	mainLayer->addChild(contentBackground, 0);

	this->contentLayer = Layer::create();
	this->contentLayer->setContentSize(Size(this->contentBackground->getBoundingBox().size.width * 7.8 / 10, this->contentBackground->getBoundingBox().size.height * 7.8 / 10));
	this->contentLayer->setPositionX(this->contentBackground->getBoundingBox().size.width/48);
	this->contentLayer->setPositionY(this->contentBackground->getBoundingBox().size.height / 24);
	//this->contentLayer->setPosition(Vec2(this->contentBackground->getBoundingBox().size.width / 2 - contentLayer->getContentSize().width / 2, this->contentBackground->getBoundingBox().size.height / 2 - contentLayer->getContentSize().height / 2)/2);
	this->contentBackground->addChild(contentLayer, 1);

	this->firstActionButton = new Button(Button::DARK_BROWN, "First", Color3B(96, 57, 19));
	this->firstActionButton->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 9 / 12));
	mainLayer->addChild(this->firstActionButton->getButton(), 2);

	this->secActionButton = new Button(Button::DARK_GRAY, "Second", Color3B(96, 57, 19));
	this->secActionButton->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 8 / 12));
	mainLayer->addChild(this->secActionButton->getButton(), 2);

	this->thirdActionButton = new Button(Button::LIGHT_GRAY, "Third", Color3B(96, 57, 19));
	this->thirdActionButton->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 7 / 12));
	mainLayer->addChild(this->thirdActionButton->getButton(), 2);

	this->detailBackground = Sprite::create("UI/button/background-detail.png");
	this->detailBackground->setContentSize(Size(this->firstActionButton->getButton()->getBoundingBox().size.width, visibleSize.height*6/12));
	this->detailBackground->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 3 / 11));
	mainLayer->addChild(this->detailBackground, 1);

	this->detailLabelTitle = Label::createWithTTF("Information", "UI/font/000 VTCSuperMarketSaleSC [TeddyBear].ttf", 25);
	this->detailLabelTitle->setTextColor(Color4B::BLACK);
	this->detailLabelTitle->setPosition(Vec2(detailBackground->getBoundingBox().size.width / 2, detailBackground->getBoundingBox().size.height *9/10));
	detailBackground->addChild(this->detailLabelTitle, 2);

	this->scrollDetail = ui::ScrollView::create();
	scrollDetail->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollDetail->setContentSize(Size(detailBackground->getContentSize().width, detailBackground->getContentSize().height * 7 / 10));
	scrollDetail->setScrollBarAutoHideEnabled(true);
	scrollDetail->setScrollBarAutoHideTime(-1);
	scrollDetail->setClippingEnabled(true);
	scrollDetail->setAnchorPoint(Vec2(0, 0));
	scrollDetail->setLayoutType(ui::Layout::Type::VERTICAL);
	scrollDetail->setInnerContainerSize(Size(detailBackground->getContentSize().width * 9 / 10, detailBackground->getContentSize().height * 2));
	scrollDetail->setPosition(Vec2(0,50));
	detailBackground->addChild(scrollDetail);

	this->detailText = ui::Text::create();
	detailText->setFontName("UI/font/000 Meanwhile.ttf");
	detailText->setFontSize(18);
	detailText->setTextColor(Color4B::BLACK);
	detailText->ignoreContentAdaptWithSize(true);
	detailText->setTextHorizontalAlignment(TextHAlignment::LEFT);
	detailText->setTextVerticalAlignment(TextVAlignment::CENTER);
	detailText->setContentSize(scrollDetail->getInnerContainerSize());
	auto param1 = ui::LinearLayoutParameter::create();
	param1->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
	param1->setMargin(ui::Margin(20, 5, 20, 25));
	detailText->setLayoutParameter(param1);
	detailText->setTextAreaSize(Size(scrollDetail->getContentSize().width*9/10, 0));
	detailText->setPosition(Vec2(scrollDetail->getContentSize().width / 2, scrollDetail->getContentSize().height - 100));
	detailText->setName("content");
	detailText->setString("THIS WILL SHOW YOU INFORMATION OF ITEM HAD SELECTED");
	this->scrollDetail->addChild(detailText);

	addListener();

}

void BaseUI::addListener()
{
	btnBack->getButton()->addClickEventListener([](Ref* event){
		AudioConfig::getInstance()->playEffect("UI2");
		Director::getInstance()->popScene();
	});
}

void BaseUI::setBackground(std::string src)
{
	this->background = Sprite::create(src);
	this->background->setScaleX(Director::getInstance()->getVisibleSize().width / this->background->getContentSize().width);
	this->background->setScaleY(Director::getInstance()->getVisibleSize().height / this->background->getContentSize().height);
	this->background->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->mainLayer->addChild(background, -1);
}

void BaseUI::setTitleHeader(std::string titleHeader)
{
	this->headerLabel->setString(titleHeader);
}

void BaseUI::setIconUnit(std::string src)
{
	this->iconUnit = Sprite::create(src);
	this->iconUnit->setPosition(Vec2(this->headerBackground->getBoundingBox().size.width *1.2 / 11, this->headerBackground->getBoundingBox().size.height *1.2 / 2));
	this->iconUnit->setContentSize(Size(this->numberOfCoin->getContentSize().height, this->numberOfCoin->getContentSize().height));
	this->headerBackground->addChild(iconUnit, 1);
}

void BaseUI::setNumberValue(int number)
{
	this->numberOfCoin->setString(CCString::createWithFormat("%d", number)->getCString());
}

void BaseUI::setActionFirstButton(std::string title, std::function<void()> callback)
{
	this->firstActionButton->setTextButton(title);
	this->firstActionButton->getButton()->addClickEventListener([&, callback](Ref* event){
		callback();
	});
}

void BaseUI::setActionSecButton(std::string title, std::function<void()> callback)
{
	this->secActionButton->setTextButton(title);
	this->secActionButton->getButton()->addClickEventListener([&, callback](Ref* event){
		callback();
	});
}
void BaseUI::setActionThirdButton(std::string title, std::function<void()> callback)
{
	this->thirdActionButton->setTextButton(title);
	this->thirdActionButton->getButton()->addClickEventListener([&, callback](Ref* event){
		callback();
	});
}

//get function
Label* BaseUI::getCoinLabel()
{
	return this->numberOfCoin;
}
ui::ScrollView* BaseUI::getDetailPanel()
{
	return this->scrollDetail;
}