#include "UiHandler.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;

using namespace ui;

cocos2d::RefPtr<UiHandler> UiHandler::sInstance = nullptr;


std::string UiCustomEventData::sUiCustomEventName = "UiCustomEvent";


UiCustomEventData::UiCustomEventData(UiCustomEventType uiType)
	: _idx(0)
	, _type(uiType)
{

}


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
		_titleLabel = static_cast<ui::Text*>(hud->getChildByName("global_anchor_lt")->getChildByName("title"));
		_modelLabel = static_cast<ui::Text*>(_titleLabel->getChildByName("modelName"));
		_animLabel = static_cast<ui::Text*>(_modelLabel->getChildByName("animName"));

		_modelListView = static_cast<ui::ListView*>(hud->getChildByName("global_anchor_rt")->getChildByName("modelListView"));
		_modelListView->setClippingEnabled(true);
		_modelListView->setTouchEnabled(true);
		_modelListView->setSwallowTouches(true);
		_modelListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(UiHandler::selectedModelEvent, this));

		_animListView = static_cast<ui::ListView*>(hud->getChildByName("global_anchor_rt")->getChildByName("animListView"));
		_animListView->setClippingEnabled(true);
		_animListView->setTouchEnabled(true);
		_animListView->setSwallowTouches(true);
		_animListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(UiHandler::selectedAnimEvent, this));
	}

	return true;
}

void UiHandler::selectedModelEvent(cocos2d::Ref *pSender, ListView::EventType type)
{
	if (type == ListView::EventType::ON_SELECTED_ITEM_START) {

		ui::ListView* listView = static_cast<ui::ListView*>(pSender);
		UiCustomEventData	d(UiCustomEventType::UCE_SELECT_MODEL);
		d._idx = listView->getCurSelectedIndex();
		d._info = (static_cast<Button*>(listView->getItem(d._idx)))->getTitleText();
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(UiCustomEventData::sUiCustomEventName,&d);
		// cocos2d::ui::LISTVIEW_ONSELECTEDITEM_END:

	}
}

void UiHandler::selectedAnimEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type)
{
	if (type == ListView::EventType::ON_SELECTED_ITEM_START) {
		ui::ListView* listView = static_cast<ui::ListView*>(pSender);

		UiCustomEventData	d(UiCustomEventType::UCE_SELECT_ANIM);
		d._idx = listView->getCurSelectedIndex();
		d._info = (static_cast<Button*>(listView->getItem(d._idx)))->getTitleText();
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(UiCustomEventData::sUiCustomEventName, &d);
	}
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

void UiHandler::addModelToViewList(const std::string& modelName)
{
	if (_defaultBtninListView == nullptr){
		_defaultBtninListView = static_cast<ui::Button*>(_modelListView->getItem(0)->clone());
		static_cast<ui::Button*>(_modelListView->getItem(0))->setEnabled(false);
	}

	auto newWidget = static_cast<ui::Button*>(_defaultBtninListView->clone());
	newWidget->setTitleText(modelName);
	_modelListView->insertCustomItem(newWidget, 1);

}

void UiHandler::addAnimToViewList(const std::string& animName)
{
	if (_defaultBtninListView == nullptr){
		_defaultBtninListView = static_cast<ui::Button*>(_modelListView->getItem(0)->clone());
		static_cast<ui::Button*>(_modelListView->getItem(0))->setEnabled(false);
	}

	auto newWidget = static_cast<ui::Button*>(_defaultBtninListView->clone());
	newWidget->setTitleText(animName);

	if (_animListView->getItems().empty())
	{
		_animListView->insertCustomItem(newWidget, 0);
	}
	else
		_animListView->insertCustomItem(newWidget, 1);

}

