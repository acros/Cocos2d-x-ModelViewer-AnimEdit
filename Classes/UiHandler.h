#ifndef __UI_MANANGER_H__
#define __UI_MANANGER_H__

#include "cocos2d.h"
#include "ui/UIText.h"
#include "ui/UIListView.h"
#include "ui/UIButton.h"
#include "ui/UITextField.h"

enum class UiCustomEventType{
	UCE_SELECT_MODEL,
	UCE_SELECT_ANIM,

};

class UiCustomEventData{
public:
	static std::string	sUiCustomEventName;

	UiCustomEventData(UiCustomEventType	uiType);

	UiCustomEventType		_type;
	int						_idx;
	std::string				_info;
};


class UiHandler : public cocos2d::Layer{
CC_CONSTRUCTOR_ACCESS:
	UiHandler();
	~UiHandler();

	virtual bool init()override;
public:
	CREATE_FUNC(UiHandler);

	static UiHandler*	getInstance();

	void	setTitle(const std::string&	title);
	void	setModelName(const std::string&	title);
	void	setAnimName(const std::string& animName, int from, int to);

	void	addModelToViewList(const std::string& modelName);
	void	addAnimToViewList(const std::string& animName);

	void	clearAnimViewList(){
		_animListView->removeAllItems();
	}

	void update(float t)override;

protected:
	void selectedModelEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);
	void selectedAnimEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);

	void showUserMsg(const std::string&	msg);

private:
	//Left top
	cocos2d::RefPtr<cocos2d::ui::Text>	_titleLabel;
	cocos2d::RefPtr<cocos2d::ui::Text>	_animLabel;
	cocos2d::RefPtr<cocos2d::ui::Text>	_modelLabel;

	//Right top
	cocos2d::RefPtr<cocos2d::ui::ListView>	_modelListView;
	cocos2d::RefPtr<cocos2d::ui::ListView>	_animListView;

	//Right bottom
	cocos2d::RefPtr<cocos2d::ui::Button>	_SaveBtn;
	cocos2d::RefPtr<cocos2d::ui::TextField>	_FromFrame;
	cocos2d::RefPtr<cocos2d::ui::TextField>	_ToFrame;

	//Middle bottom
	cocos2d::RefPtr<cocos2d::ui::Text>		_MsgToUser;

	float _MsgToUserAlpha;

	cocos2d::RefPtr<cocos2d::ui::Button>	_defaultBtninListView;	//Dummy for clone

	static cocos2d::RefPtr<UiHandler>	sInstance;
};

#endif