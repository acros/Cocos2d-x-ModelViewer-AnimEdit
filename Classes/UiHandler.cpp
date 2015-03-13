#include "UiHandler.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIHelper.h"
#include "IndexFileParser.h"
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
: _MsgToUserAlpha(1.f)
, _addState(AddingState::AS_NONE)
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

	auto hud = static_cast<Widget*>(CSLoader::createNode("ModelViewSelect.csb"));

	if (hud != nullptr)
	{
		hud->setCameraMask((unsigned short)CameraFlag::DEFAULT);
		addChild(hud);

		_titleLabel = static_cast<ui::Text*>(hud->getChildByName("global_anchor_lt")->getChildByName("title"));
		_modelLabel = static_cast<ui::Text*>(_titleLabel->getChildByName("modelName"));
		_animLabel = static_cast<ui::Text*>(_modelLabel->getChildByName("animName"));

		//////////////////////////////////////////////////////////////////////////
		_addModelBtn = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(hud, "AddModelBtn"));
		_addModelBtn->addClickEventListener(CC_CALLBACK_1(UiHandler::showAdding, this,AddingState::AS_ADD_MODEL));
		_modelListView = static_cast<ui::ListView*>(hud->getChildByName("global_anchor_rt")->getChildByName("modelListView"));
		_modelListView->setClippingEnabled(true);
		_modelListView->setTouchEnabled(true);
		_modelListView->setSwallowTouches(true);
		_modelListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(UiHandler::selectedModelEvent, this));

		_addAnimBtn = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(hud, "AddAnimBtn"));
		_addAnimBtn->addClickEventListener(CC_CALLBACK_1(UiHandler::showAdding, this,AddingState::AS_ADD_ANIM));
		_animListView = static_cast<ui::ListView*>(hud->getChildByName("global_anchor_rt")->getChildByName("animListView"));
		_animListView->setClippingEnabled(true);
		_animListView->setTouchEnabled(true);
		_animListView->setSwallowTouches(true);
		_animListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(UiHandler::selectedAnimEvent, this));
		//////////////////////////////////////////////////////////////////////////

		_addDialogImage = ui::Helper::seekWidgetByName(hud, "addDialog");		
		_addDialogImage->setVisible(false);
		_addFileName = static_cast<ui::TextField*>(ui::Helper::seekWidgetByName(hud, "addNameText"));
		_addOkBtn = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(hud, "addGO"));
		_addOkBtn->addClickEventListener(CC_CALLBACK_1(UiHandler::AddNewItem, this));

		//////////////////////////////////////////////////////////////////////////

		_FromFrame  = static_cast<ui::TextField*>(ui::Helper::seekWidgetByName(hud,"FromFrame"));
		_ToFrame = static_cast<ui::TextField*>(ui::Helper::seekWidgetByName(hud,"ToFrame"));
		
		_EnsureModifyAnimBtn = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(hud, "changeAnim"));
		_EnsureModifyAnimBtn->addClickEventListener(CC_CALLBACK_1(UiHandler::modifyAnim, this));

		_MsgToUser = static_cast<ui::Text*>(ui::Helper::seekWidgetByName(hud, "info"));

		_SaveBtn = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(hud,"saveToFile"));
		_SaveBtn->addClickEventListener(CC_CALLBACK_1(UiHandler::serializeToJson, this));

		showUserMsg("Arrow key to switch Model/Animation.\nUse MOUSE to control view. Key SPACE to reset camera.");
		scheduleUpdate();
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

void UiHandler::setAnimName(const std::string& animName,int from,int to)
{
	_animLabel->setString(animName);

	char text[25];
	_itoa(from, text, 10);
	_FromFrame->setString(text);

	_itoa(to, text, 10);
	_ToFrame->setString(text);
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

void UiHandler::update(float t)
{
	if (_MsgToUser->isVisible())
	{
		_MsgToUserAlpha -= t*0.2f;
		_MsgToUser->setOpacity(_MsgToUserAlpha * 255);
		if (_MsgToUserAlpha < 0.f)
		{
			_MsgToUser->setVisible(false);
			_MsgToUserAlpha = 0;
		}
	}
}

void UiHandler::showUserMsg(const std::string& msg,Color3B	c)
{
	_MsgToUser->setVisible(true);
	_MsgToUserAlpha = 1.f;

	_MsgToUser->setString(msg);
	_MsgToUser->setColor(c);
}

void UiHandler::modifyAnim(cocos2d::Ref* pSender)
{
	if (_animLabel->getString() == IndexFileParser::s_DefaultAnim)	{
		showUserMsg("Can't modify the default animation", Color3B::RED);
	}
	else{
		showUserMsg("Animation [" + _animLabel->getString() + "] modified.", Color3B::GREEN);

		//TODO:
		//change the anim
		assert(false);
	}
}

void UiHandler::AddNewItem(cocos2d::Ref* pSender)
{
	bool canAdd = true;

	_addDialogImage->setVisible(false);

	if (!_addFileName->getString().empty()){
		//Check if the name exist
		if (_addState == AddingState::AS_ADD_MODEL)
		{
			for (auto& items : IndexFileParser::s_AnimFileData){
				string newName = _addFileName->getString();
				string oldName(items.name);
				std::transform(newName.begin(), newName.end(), newName.begin(), tolower);
				std::transform(oldName.begin(), oldName.end(), oldName.begin(), tolower);

				if (oldName == newName){
					canAdd = false;
					showUserMsg("Model name conflict!", Color3B::RED);
					break;
				}

			}
		}else if (_addState == AddingState::AS_ADD_ANIM){
			for (auto& items : IndexFileParser::s_AnimFileData){
				if (items.name == _titleLabel->getString())	{
					for (auto& anim : items.animList){
						string newName = _addFileName->getString();
						string oldName(anim.name);
						std::transform(newName.begin(), newName.end(), newName.begin(), tolower);
						std::transform(oldName.begin(), oldName.end(), oldName.begin(), tolower);
						if (oldName == newName){
							canAdd = false;
							showUserMsg("Animation name conflict!", Color3B::RED);
							break;
						}
					}
					if (canAdd == false)
						break;
				}
			}
		}
	}

	if (canAdd)
	{
		showUserMsg("Add this to list!", Color3B::GREEN);
		//TODO:Find if exist
		assert(false);
	}

	_addFileName->setString("");
	_addState = AddingState::AS_NONE;
}

void UiHandler::showAdding(cocos2d::Ref* pSender, AddingState state)
{
	if (_addState != AddingState::AS_NONE)
		return;

	_addState = state;
	_addDialogImage->setVisible(true);
}

void UiHandler::serializeToJson(cocos2d::Ref* pSender)
{
	showUserMsg("Function not finished yet.", Color3B::YELLOW);

	//TODO:
	assert(false);
}
