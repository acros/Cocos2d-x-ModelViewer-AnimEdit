#ifndef __UI_MANANGER_H__
#define __UI_MANANGER_H__

#include "cocos2d.h"
#include "ui/UIText.h"

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

protected:
	cocos2d::RefPtr<cocos2d::ui::Text>	_titleLabel;
	cocos2d::RefPtr<cocos2d::ui::Text>	_animLabel;
	cocos2d::RefPtr<cocos2d::ui::Text>	_modelLabel;

// 	ui::Text* _titleLabel;
// 	cocos2d::Label* _animLabel;
// 	cocos2d::Label* _modelLabel;

private:
	static cocos2d::RefPtr<UiHandler>	sInstance;
};

#endif