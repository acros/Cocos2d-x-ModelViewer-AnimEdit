#ifndef __VIEW_TARGET_H__
#define __VIEW_TARGET_H__

#include "cocos2d.h"
#include "IndexFileParser.h"

typedef cocos2d::Map<std::string, cocos2d::Animate3D*>::iterator	AnimMapIter;
typedef cocos2d::Map<std::string, cocos2d::Animate3D*>		AnimMap;

class ViewTarget : public cocos2d::Ref{
CC_CONSTRUCTOR_ACCESS:
	ViewTarget();
	virtual ~ViewTarget();

	virtual bool init();

public:
	CREATE_FUNC(ViewTarget);

	bool	load(const AnimFileIndex&	animFile);

	cocos2d::Node*	getNode()const;

	float	getCamDistance()const	{ return _orginDistance; }
	const	cocos2d::Vec3&	getCamCenter()const	{ return _orginCenter; }

	void	switchAnim(int step);
	void	switchAnim(const std::string& animName);

	const std::string& getTitle()const;
	const std::string& getModelName()const;
	const std::string& getCurrAnimName()const;

	const AnimMap&	getAnimMap()const	{ return _AnimList; }

protected:
	void	parseAnimSection(const AnimFileIndex&	animFile, cocos2d::Animation3D* anim);

	float			_orginDistance;
	cocos2d::Vec3	_orginCenter;

	cocos2d::RefPtr<cocos2d::Sprite3D>	_Sprite3d;

	std::string	_name;
	std::string _modelName;

	AnimMap	_AnimList;
	AnimMapIter _currAnim;
};

#endif