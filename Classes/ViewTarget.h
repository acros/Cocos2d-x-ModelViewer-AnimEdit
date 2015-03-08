#ifndef __VIEW_TARGET_H__
#define __VIEW_TARGET_H__

#include "cocos2d.h"
#include "IndexFileParser.h"

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
	const cocos2d::Vec3&	getCamCenter()const	{ return _orginCenter; }
protected:

	float			_orginDistance;
	cocos2d::Vec3	_orginCenter;

	cocos2d::RefPtr<cocos2d::Sprite3D>	_Sprite3d;

};

#endif