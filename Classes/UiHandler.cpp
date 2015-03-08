#include "UiHandler.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;

cocos2d::RefPtr<UiHandler> UiHandler::sInstance = nullptr;

UiHandler::UiHandler()
{
	assert(sInstance == nullptr);
	sInstance = this;
}

UiHandler::~UiHandler()
{

}

UiHandler* UiHandler::getInstance()
{
	assert(sInstance != nullptr);
	return sInstance;
}

bool UiHandler::init()
{
	Layer::init();

	auto hud = CSLoader::createNode("ModelViewSelect.csb");
	hud->setCameraMask((unsigned short)CameraFlag::DEFAULT);

	addChild(hud);
	if (hud != nullptr)
	{
		_titleLabel = dynamic_cast<ui::Text*>(hud->getChildByName("global_anchor_lt")->getChildByName("title"));
		_modelLabel = dynamic_cast<ui::Text*>(_titleLabel->getChildByName("modelName"));
		_animLabel  = dynamic_cast<ui::Text*>(_modelLabel->getChildByName("animName"));
	}

	return true;
}

void UiHandler::setTitle(const std::string& title)
{
	_titleLabel->setString(title);
}

void UiHandler::setModelName(const std::string& modelName)
{
	_modelLabel->setString(modelName);
}

void UiHandler::setAnimName(const std::string& animName)
{
	_animLabel->setString(animName);
}

