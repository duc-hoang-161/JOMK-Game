#include "SettingOverlay.h"
#include "Import\AudioConfig.h"
#include "LoadingScene.h"
#include "ui\UIRichText.h"
Layer* SettingOverlay::settingOverlay = NULL;
int SettingOverlay::TYPE = 1;

Color3B def3BColor = Color3B(96, 57, 19);
Color4B def4BColor = Color4B(96, 57, 19, 255);

SettingOverlay::SettingOverlay()
{
	this->settingOverlay = Layer::create();
	this->settingOverlay->retain();
	this->TYPE = LITE_OPERATION;
	init();
	this->changeLayout(LITE_OPERATION);
}

SettingOverlay::SettingOverlay(int mode)
{
	this->settingOverlay = Layer::create();
	this->settingOverlay->retain();
	this->TYPE = mode;
	init();
	this->changeLayout(mode);
}

void SettingOverlay::setCallBack(std::function<void(bool)> callback)
{

	this->callback = callback;
}

void SettingOverlay::init()
{
	createLayout();
	changeLayout(TYPE);
	addListener();
}

void SettingOverlay::createLayout()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	settingOverlay->setContentSize(visibleSize);
	settingOverlay->setAnchorPoint(Vec2(0, 0));

	//add layer contain menu
	layerMenu = Layer::create();
	layerMenu->setContentSize(visibleSize);
	layerMenu->setPosition(Vec2(0, 0));
	layerMenu->setAnchorPoint(Vec2(0.5, 0.5));
	layerMenu->setVisible(false);
	settingOverlay->addChild(layerMenu, 0);
	//add background transparent gray
	auto gray = Sprite::create("UI/gray.png");
	gray->setScale(visibleSize.width / gray->getContentSize().width, visibleSize.height / gray->getContentSize().height);
	gray->setPosition(visibleSize / 2);
	gray->setOpacity(60);
	gray->setName("graybg");
	gray->setVisible(false);
	settingOverlay->addChild(gray, -1);


	//add window layout
	auto window = Sprite::create("UI/button/alert-background.png");
	window->setScale(visibleSize.width / (2 * window->getContentSize().width), visibleSize.height / (2 * window->getContentSize().height));
	layerMenu->addChild(window, 0);
	layerMenu->setName("main");
	window->setPosition(visibleSize / 2);
	Size windowSize = Size(window->getContentSize().width*window->getScaleX(), window->getContentSize().height*window->getScaleY());

	btnReturnMain = new Button(Button::DARK_BROWN, "Main Menu");
	btnReturnMain->setColorTitle(Color3B(96, 57, 19));
	btnReturnMain->getButton()->setAnchorPoint(Vec2(0.5, 0.5));
	btnReturnMain->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 13 / 20));
	layerMenu->addChild(btnReturnMain->getButton(), 1);

	btnAudioSetting = new Button(Button::DARK_BROWN, "Audio");
	btnAudioSetting->setColorTitle(Color3B(96, 57, 19));
	btnAudioSetting->getButton()->setAnchorPoint(Vec2(0.5, 0.5));
	btnAudioSetting->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 11 / 20));
	layerMenu->addChild(btnAudioSetting->getButton(), 1);

	btnTutorial = new Button(Button::DARK_BROWN, "Tutorial");
	btnTutorial->setColorTitle(Color3B(96, 57, 19));
	btnTutorial->getButton()->setAnchorPoint(Vec2(0.5, 0.5));
	btnTutorial->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 9 / 20));
	layerMenu->addChild(btnTutorial->getButton(), 1);

	btnAbout = new Button(Button::DARK_BROWN, "About");
	btnAbout->setColorTitle(Color3B(96, 57, 19));
	btnAbout->getButton()->setAnchorPoint(Vec2(0.5, 0.5));
	btnAbout->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 7 / 20));
	layerMenu->addChild(btnAbout->getButton(), 1);

	btnBack = new Button(Button::BACK_BIG, "BACK");
	btnBack->setColorTitle(Color3B(96, 57, 19));
	btnBack->getButton()->setAnchorPoint(Vec2(0.5, 0.5));
	btnBack->getButton()->setPosition(Vec2(visibleSize.width * 5 / 8, visibleSize.height / 4));
	btnBack->getButton()->setTitleFontSize(50);
	btnBack->getButton()->setScale(0.6);
	layerMenu->addChild(btnBack->getButton(), 1);

	Button* btnExit = new Button(Button::BACK_BIG, "EXIT");
	btnExit->setColorTitle(Color3B(158, 11, 15));
	btnExit->getButton()->setAnchorPoint(Vec2(0.5, 0.5));
	btnExit->getButton()->setPosition(Vec2(visibleSize.width * 3 / 8, visibleSize.height / 4));
	btnExit->getButton()->setTitleFontSize(50);
	btnExit->getButton()->setScale(0.6);
	layerMenu->addChild(btnExit->getButton(), 1);
	btnExit->getButton()->addClickEventListener([](Ref* event){
		Director::getInstance()->end();
	});
	//create Audio layer
	createAudioSettingLayout();

	//add tutorial alert with default invisible
	this->tutorial = new Alert("Tutorial", "The increasing popularity of solo travel has trained the spotlight on safe destinations. The U.S. News newspaper recommends a solo vacation in Vietnam, which it describes as a safe destination with friendly locals and outstanding cuisine.", Alert::LARGE_SIZE);
	Layer* layerTut = this->tutorial->getAlertLayer();
	this->tutorial->getTitleLabel()->setPositionY(this->tutorial->getTitleLabel()->getPositionY() + 25);
	this->tutorial->getTitleLabel()->setTextColor(Color4B::BLACK);
	
	this->tutorial->getScroll()->removeChild(this->tutorial->getContentLabel());
	Sprite* tutPic = Sprite::create("UI/tut.png");
	tutPic->setPosition(tutPic->getContentSize() / 2);
	this->tutorial->getScroll()->setInnerContainerSize(tutPic->getContentSize());
	this->tutorial->getScroll()->addChild(tutPic);
	settingOverlay->addChild(layerTut, 5);

	//add about alert with default invisible
	this->about = new Alert("About", "This is about game", Alert::LARGE_SIZE);
	Layer* layerAbout = this->about->getAlertLayer();
	this->about->getTitleLabel()->setPositionY(this->about->getTitleLabel()->getPositionY() + 25);
	this->about->getTitleLabel()->setTextColor(Color4B::BLACK);
	this->about->getContentLabel()->setTextColor(def4BColor);
	this->about->getContentLabel()->setString(FileUtils::getInstance()->getStringFromFile("res/about.txt").c_str());
	//ui::RichText* aboutText = ui::RichText::createWithXML("<b>JOU</b>");
	//this->about->getLayerContent()->addChild(aboutText);
	settingOverlay->addChild(layerAbout, 5);
}

void SettingOverlay::changeLayout(int type)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	this->TYPE = type;
	switch (type)
	{
	case FULL_OPERATION:
	{
						  
				btnReturnMain->getButton()->setVisible(true);
				//btnReturnMain->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 13 / 20));
				btnAudioSetting->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 11 / 20));
				btnTutorial->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 9 / 20));
				btnAbout->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 7 / 20));
				this->layerMenu->setVisible(false);

				//pause alert
				Label* pauseText = Label::createWithTTF("GAME PAUSED", "UI/font/UTM Karate.ttf", 45);
				pauseText->setTextColor(Color4B(158, 11, 15, 255));
				pauseText->enableOutline(Color4B::ORANGE, 1);
				pauseText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 4 / 5));
				pauseText->setVisible(true);
				layerMenu->addChild(pauseText);
				break;
	}
		//LITE_OPERATION
	default:
	{
			   btnReturnMain->getButton()->setVisible(false);
			   //btnReturnMain->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 13 / 20));
			   btnAudioSetting->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 13 / 20));
			   btnTutorial->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 10 / 20));
			   btnAbout->getButton()->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 7 / 20));
			   this->layerMenu->setVisible(false);
			   break;
	}
	
	}

}

void SettingOverlay::addListener()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//ADD LISTENER RETURN MAIN MENU SCENE
	btnReturnMain->getButton()->addClickEventListener([this](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		this->callback(true);
		if (Director::getInstance()->isPaused())
		{
			Director::getInstance()->resume();
			
		}
		Director::getInstance()->replaceScene(TransitionFade::create(1.5, MainScene::createScene()));
	});

	Layer* layerSetting = setting->getAlertLayer();

	//ADD LISTENER FOR SETTING AUDIO
	//show alert
	btnAudioSetting->getButton()->addClickEventListener([&, layerSetting](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		layerSetting->setVisible(true);
		layerMenu->setVisible(false);
	});


	//ADD LISTENER FOR TUTORIAL
	Layer* layerTut = tutorial->getAlertLayer();
	//show alert
	btnTutorial->getButton()->addClickEventListener([&, layerTut](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		layerMenu->setVisible(false);
		layerTut->setVisible(true);
	});
	//close alert
	tutorial->getActionButton()->getButton()->addClickEventListener([&, layerTut](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		layerTut->setVisible(false); layerMenu->setVisible(true); 
	});

	//ADD LISTENER FOR ABOUT
	Layer* layerAbout = about->getAlertLayer();
	//show alert
	btnAbout->getButton()->addClickEventListener([&, layerAbout](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		layerAbout->setVisible(true);
		layerMenu->setVisible(false);
	});
	//close alert
	about->getActionButton()->getButton()->addClickEventListener([&, layerAbout](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		layerAbout->setVisible(false); layerMenu->setVisible(true); 
	});

	//ADD ICON SETTING
	if (this->TYPE == FULL_OPERATION)
	{
		btnSetting = ui::Button::create("UI/main screen/icon-setting-ingame.png", "UI/main screen/icon-setting-ingame-pressed.png", "UI/main screen/icon-setting-ingame-pressed.png");
	}
	else
	{
		btnSetting = ui::Button::create("UI/main screen/icon-setting.png", "UI/main screen/icon-setting-pressed.png", "UI/main screen/icon-setting.png");
	}
	settingOverlay->addChild(btnSetting, 0);
	if (this->TYPE == FULL_OPERATION)
	{
		this->btnSetting->setOpacity(95);
	}
	btnSetting->setPosition(Vec2(visibleSize.width - btnSetting->boundingBox().size.width / 2, visibleSize.height - btnSetting->boundingBox().size.height / 2));
	Layer* m_layerMenu = layerMenu;
	//show setting menu
	btnSetting->addClickEventListener([&, m_layerMenu](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		//show setting
		m_layerMenu->setVisible(true);
		callback(false);						//disable building button on main scene
		//hide setting icon
		settingOverlay->getChildByName("graybg")->setVisible(true);
		btnSetting->setEnabled(false);
		/*btnSetting->stopAllActions();
		if (this->TYPE != FULL_OPERATION)
			btnSetting->runAction(Repeat::create(FadeOut::create(0.5), 1));
		else
			btnSetting->setVisible(false);*/
	});

	//hide setting menu
	Layer *overlay = settingOverlay;
	btnBack->getButton()->addClickEventListener([&, overlay](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		auto fadeOut = FadeOut::create(0.2f);
		Layer* layer = layerMenu;
		//hide setting
		auto hide = CallFunc::create([&, layer, overlay](){
			layer->setVisible(false);
			overlay->getChildByName("graybg")->setVisible(false);
			if (this->TYPE == FULL_OPERATION)
			{
				this->btnSetting->setOpacity(95);
			}
		});
		auto sequence =
			Sequence::createWithTwoActions(Repeat::create(fadeOut, 0.2), hide);
		overlay->stopAllActions();
		overlay->runAction(sequence);

		//enable setting icon
		btnSetting->setEnabled(true);
		btnSetting->setVisible(true);
		btnSetting->stopAllActions();
		/*if (this->TYPE == LITE_OPERATION)
			btnSetting->runAction(Repeat::create(FadeIn::create(0.5), 1));
		else
			btnSetting->setVisible(true);*/
		//enable building button on main scene
		callback(true);
	});
}

void SettingOverlay::createAudioSettingLayout()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//add setting alert
	this->setting = new Alert("Setting audio", "", Alert::LARGE_SIZE);
	this->setting->getTitleLabel()->setTextColor(def4BColor);
	this->setting->getTitleLabel()->setPosition(Vec2(visibleSize.width/2, visibleSize.height*4/5));
	this->setting->getTitleLabel()->setScale(1.4);

	Layer* layerSetting = this->setting->getAlertLayer();
	Layer* controlLayer = Layer::create();
	Size settingContentSize = this->setting->getLayerContent()->getContentSize();
	controlLayer->setContentSize(settingContentSize);
	this->setting->getLayerContent()->removeAllChildren();
	controlLayer->setPosition(controlLayer->getPosition() + Vec2(0, 25));
	//add label total volume
	Label* labelTotalAudio = Label::createWithTTF("Total volume", "UI/font/SVN-Prima.ttf", 30);
	labelTotalAudio->setPosition(Vec2(settingContentSize.width * 3 / 10, settingContentSize.height * 12 / 20));
	controlLayer->addChild(labelTotalAudio, 5);
	labelTotalAudio->setTextColor(def4BColor);
	//add check box total volume
	ui::CheckBox* checkBox = ui::CheckBox::create("UI/button/check-base.png", "UI/button/check-base.png", "UI/button/check-cc.png", "UI/button/check-base.png", "UI/button/check-cc.png");
	checkBox->setSelected(true);
	checkBox->setPosition(Vec2(Vec2(settingContentSize.width / 2, settingContentSize.height * 12 / 20)));
	controlLayer->addChild(checkBox, 5);

	//label background sound volume
	Label* labelBgVolume = Label::createWithTTF("Background", "UI/font/SVN-Prima.ttf", 25);
	labelBgVolume->setAnchorPoint(Vec2(1, 0));
	labelBgVolume->setPosition(Vec2(settingContentSize.width * 3 / 10, settingContentSize.height * 10 / 20));
	controlLayer->addChild(labelBgVolume, 5);
	labelBgVolume->setTextColor(def4BColor);
	//label percent background sound volume 
	Label* labelBgVolumePercent = Label::createWithTTF(CCString::createWithFormat("%d", AudioConfig::getInstance()->getBackgroundVolume())->getCString(), "UI/font/SVN-Prima.ttf", 25);
	labelBgVolumePercent->setAnchorPoint(Vec2(0, 0));
	labelBgVolumePercent->setPosition(Vec2(settingContentSize.width * 4 / 11, settingContentSize.height * 10 / 20));
	controlLayer->addChild(labelBgVolumePercent, 5);
	labelBgVolumePercent->setTextColor(def4BColor);
	//background volume slider
	ui::Slider* bgSlider = ui::Slider::create();
	Sprite* bgSliderImage = Sprite::create("UI/button/bg-slider-b.png");
	bgSliderImage->setPosition(Vec2(settingContentSize.width / 2, settingContentSize.height * 9 / 20));
	controlLayer->addChild(bgSliderImage, 4);
	bgSlider->loadBarTexture("UI/button/bg-slider.png");
	bgSlider->loadSlidBallTextures("UI/button/ball.png", "UI/button/ball-pressed.png", "UI/button/ball.png");
	bgSlider->loadProgressBarTexture("UI/button/bg-slider-load.png");
	bgSlider->setPercent(AudioConfig::getInstance()->getBackgroundVolume());
	bgSlider->setPosition(Vec2(settingContentSize.width / 2, settingContentSize.height * 9 / 20));
	controlLayer->addChild(bgSlider, 5);

	//label effect sound volume
	Label* labelEffect = Label::createWithTTF("Effect", "UI/font/SVN-Prima.ttf", 25);
	labelEffect->setAnchorPoint(Vec2(1, 0));
	labelEffect->setPosition(Vec2(settingContentSize.width * 3 / 10, settingContentSize.height * 7 / 20));
	controlLayer->addChild(labelEffect, 5);
	labelEffect->setTextColor(def4BColor);
	//label percent effect sound volume 
	Label* labelEffectPercent = Label::createWithTTF(CCString::createWithFormat("%d", AudioConfig::getInstance()->getEffectVolume())->getCString(), "UI/font/SVN-Prima.ttf", 25);
	labelEffectPercent->setAnchorPoint(Vec2(0, 0));
	labelEffectPercent->setPosition(Vec2(settingContentSize.width * 4 / 11, settingContentSize.height * 7 / 20));
	controlLayer->addChild(labelEffectPercent, 5);
	labelEffectPercent->setTextColor(def4BColor);
	//effect volume slider
	ui::Slider* effSlider = ui::Slider::create();
	Sprite* effSliderImage = Sprite::create("UI/button/bg-slider-b.png");
	effSliderImage->setPosition(Vec2(settingContentSize.width / 2, settingContentSize.height * 6 / 20));
	controlLayer->addChild(effSliderImage, 4);
	effSlider->loadBarTexture("UI/button/bg-slider.png");
	effSlider->loadSlidBallTextures("UI/button/ball.png", "UI/button/ball-pressed.png", "UI/button/ball.png");
	effSlider->loadProgressBarTexture("UI/button/bg-slider-load.png");
	effSlider->setPercent(AudioConfig::getInstance()->getEffectVolume());
	effSlider->setPosition(Vec2(settingContentSize.width / 2, settingContentSize.height * 6 / 20));
	controlLayer->addChild(effSlider, 5);

	//init config audio from config file
	if (AudioConfig::getInstance()->isAudioEnable())
	{
		checkBox->setSelected(true);
		bgSlider->setEnabled(true);
		effSlider->setEnabled(true);
	}
	else
	{
		checkBox->setSelected(false);
		bgSlider->setEnabled(false);
		effSlider->setEnabled(false);
		labelBgVolumePercent->setString("MUTED");
		labelEffectPercent->setString("MUTED");
	}

	//ADD LAYOUT TO OVERLAY
	this->setting->getLayerContent()->addChild(controlLayer, 4);
	settingOverlay->addChild(layerSetting, 5);

	//listener for checkbox and slider
	checkBox->addClickEventListener([checkBox, bgSlider, effSlider, labelBgVolumePercent, labelEffectPercent](Ref* event){
		if (checkBox->isSelected())
		{
			AudioConfig::getInstance()->playEffect("UIOFF");
			bgSlider->setEnabled(false);
			effSlider->setEnabled(false);
			labelBgVolumePercent->setString("MUTED");
			labelEffectPercent->setString("MUTED");
		}
		else
		{
			AudioConfig::getInstance()->playEffect("UION");
			bgSlider->setEnabled(true);
			effSlider->setEnabled(true);
			if (labelBgVolumePercent->getStringLength() == 5)
			{
				labelBgVolumePercent->setString("0");
				labelEffectPercent->setString("0");
				bgSlider->setPercent(0);
				effSlider->setPercent(0);
			}
		}
	});

	//listener for slider background volume
	bgSlider->addClickEventListener([&, labelBgVolumePercent, bgSlider, checkBox](Ref* event){
		if (checkBox->isSelected())
		{
			labelBgVolumePercent->setString(CCString::createWithFormat("%d", bgSlider->getPercent())->getCString());
		}
	});
	//listener for slider effect volume
	effSlider->addClickEventListener([&, labelEffectPercent, effSlider, checkBox](Ref* event){
		if (checkBox->isSelected())
		{
			labelEffectPercent->setString(CCString::createWithFormat("%d", effSlider->getPercent())->getCString());
		}
	});

	//add button default audio and listener
	Button* btnDefAudio = new Button(Button::LIGHT_GRAY, "Default");
	btnDefAudio->getButton()->setTitleColor(def3BColor);
	btnDefAudio->getButton()->setPosition(Vec2(visibleSize.width * 5 / 16, visibleSize.height * 2 / 9));
	setting->getAlertLayer()->addChild(btnDefAudio->getButton(), 5);
	btnDefAudio->getButton()->addClickEventListener([&, checkBox, labelBgVolume, labelBgVolumePercent, bgSlider, labelEffectPercent, effSlider](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		checkBox->setSelected(true);
		labelBgVolumePercent->setString("100");
		bgSlider->setPercent(100);
		bgSlider->setEnabled(true);
		labelEffectPercent->setString("100");
		effSlider->setPercent(100);
		effSlider->setEnabled(true);
		AudioConfig::getInstance()->updateConfig(checkBox->isSelected(), bgSlider->getPercent(), effSlider->getPercent());
	});
	//add button apply setting and listener
	Button* btnApplyAudio = new Button(Button::DARK_BROWN, "Apply");
	btnApplyAudio->getButton()->setTitleColor(def3BColor);
	btnApplyAudio->getButton()->setPosition(Vec2(visibleSize.width * 8 / 16, visibleSize.height * 2 / 9));
	setting->getAlertLayer()->addChild(btnApplyAudio->getButton(), 5);
	btnApplyAudio->getButton()->addClickEventListener([&, checkBox, labelBgVolumePercent, bgSlider, labelEffect, effSlider](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		AudioConfig::getInstance()->updateConfig(checkBox->isSelected(), bgSlider->getPercent(), effSlider->getPercent());
	});

	//hide alert
	setting->getActionButton()->getButton()->addClickEventListener([&, layerSetting, checkBox, labelBgVolumePercent, bgSlider, labelEffectPercent, effSlider](Ref* event){
		AudioConfig::getInstance()->playEffect("UI1");
		layerSetting->setVisible(false);
		layerMenu->setVisible(true);
		if (AudioConfig::getInstance()->isAudioEnable() != checkBox->isSelected() || AudioConfig::getInstance()->getBackgroundVolume() != bgSlider->getPercent() || AudioConfig::getInstance()->getEffectVolume() != effSlider->getPercent())
		{
			if (!AudioConfig::getInstance()->isAudioEnable())
			{
				checkBox->setSelected(false);
				bgSlider->setEnabled(false);
				effSlider->setEnabled(false);
				labelBgVolumePercent->setString("MUTED");
				labelEffectPercent->setString("MUTED");
				bgSlider->setPercent(AudioConfig::getInstance()->getBackgroundVolume());
				effSlider->setPercent(AudioConfig::getInstance()->getEffectVolume());
			}
			else
			{
				checkBox->setSelected(true);
				bgSlider->setEnabled(true);
				effSlider->setEnabled(true);
				labelBgVolumePercent->setString(CCString::createWithFormat("%d", AudioConfig::getInstance()->getBackgroundVolume())->getCString());
				labelEffectPercent->setString(CCString::createWithFormat("%d", AudioConfig::getInstance()->getEffectVolume())->getCString());
				bgSlider->setPercent(AudioConfig::getInstance()->getBackgroundVolume());
				effSlider->setPercent(AudioConfig::getInstance()->getEffectVolume());
			}
		}
	});
	setting->getActionButton()->getButton()->setTitleColor(def3BColor);
	setting->getActionButton()->getButton()->setPosition(Vec2(visibleSize.width * 11 / 16, visibleSize.height * 2 / 9));

}

Layer* SettingOverlay::getOverlay()
{
	return this->settingOverlay;
}

void SettingOverlay::close()
{
	//settingOverlay->setVisible(false);
	layerMenu->setVisible(false);
	btnSetting->setEnabled(true);
	//callback(true);
}

SettingOverlay::~SettingOverlay()
{
	this->settingOverlay->release();
}

void SettingOverlay::setEnable(bool isEnable)
{
	this->btnSetting->setEnabled(isEnable);
}