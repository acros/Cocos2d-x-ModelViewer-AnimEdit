#ifndef __UI_MANANGER_H__
#define __UI_MANANGER_H__

#include "cocos2d.h"
#include "ui/UIText.h"
#include "ui/UIListView.h"
#include "ui/UIButton.h"

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
	void	setAnimName(const std::string&	title);

	void	addModelToViewList(const std::string& modelName);
	void	addAnimToViewList(const std::string& animName);

	void	clearAnimViewList(){
		_animListView->removeAllItems();
	}

protected:
	void selectedModelEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);
	void selectedAnimEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);

	cocos2d::RefPtr<cocos2d::ui::Text>	_titleLabel;
	cocos2d::RefPtr<cocos2d::ui::Text>	_animLabel;
	cocos2d::RefPtr<cocos2d::ui::Text>	_modelLabel;

	cocos2d::RefPtr<cocos2d::ui::ListView>	_modelListView;
	cocos2d::RefPtr<cocos2d::ui::ListView>	_animListView;

private:
	cocos2d::RefPtr<cocos2d::ui::Button>	_defaultBtninListView;

	static cocos2d::RefPtr<UiHandler>	sInstance;
};

#endif