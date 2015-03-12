#include "ViewTarget.h"
#include "UiHandler.h"

USING_NS_CC;

bool ViewTarget::init()
{
	return true;
}

ViewTarget::ViewTarget():
_orginDistance(0.0f)
, _MaxAnimFrame(0)
{

}

ViewTarget::~ViewTarget()
{

}

bool ViewTarget::load(AnimFileData& fileIdx)
{
	if (fileIdx.texFile.empty())
		_Sprite3d = Sprite3D::create(fileIdx.modelFile);
	else
		_Sprite3d = Sprite3D::create(fileIdx.modelFile, fileIdx.texFile);

	if (_Sprite3d)
	{
		_name = fileIdx.name;
		_modelName = fileIdx.modelFile;
		auto animation = Animation3D::create(fileIdx.animFile);
		if (animation)
		{
			auto animate = Animate3D::create(animation);
			_AnimList.insert(IndexFileParser::s_DefaultAnim, animate);
			_MaxAnimFrame = animation->getDuration() * IndexFileParser::sFrameRate;
			UiHandler::getInstance()->setAnimName(IndexFileParser::s_DefaultAnim, 0, _MaxAnimFrame);
			_Sprite3d->runAction(RepeatForever::create(animate));

			parseAnimSection(fileIdx,animation);
		}

		AABB aabb = _Sprite3d->getAABB();
		Vec3 corners[8];
		aabb.getCorners(corners);
		//temporary method, replace it
		if (abs(corners[3].x) == 99999.0f && abs(corners[3].y) == 99999.0f && abs(corners[3].z) == 99999.0f)
		{
			_orginCenter = Vec3(0.0f, 0.0f, 0.0f);
			_orginDistance = 100.0f;
		}
		else
		{
			float radius = (corners[0] - corners[5]).length();
			_orginCenter = aabb.getCenter();
			_orginDistance = radius;
		}

		_Sprite3d->setCameraMask((unsigned short)CameraFlag::USER1);
	}

	return true;
}

cocos2d::Node* ViewTarget::getNode() const
{
	return _Sprite3d;
}

void ViewTarget::switchAnim(int step)
{
	//if (animName != _currAnim->first)
	if (step == 0)
		return;

	assert(step == 1 || step == -1);
	if (step > 0 )
	{
		_currAnim++;
		if (_currAnim == _AnimList.end()){
			_currAnim = _AnimList.begin();
		}		
	}
	else {
		if (_currAnim == _AnimList.begin()){
			_currAnim = (--_AnimList.end());
		}
		else
			--_currAnim;
	}

	_Sprite3d->stopAllActions();
	_Sprite3d->runAction(RepeatForever::create(_currAnim->second));

	if (_currAnim == _AnimList.begin()){
		UiHandler::getInstance()->setAnimName(_currAnim->first, 0,_MaxAnimFrame);
	}
	else{
		auto animTarget = IndexFileParser::findAnim(_name,_currAnim->first);
		if (animTarget != nullptr)
			UiHandler::getInstance()->setAnimName(_currAnim->first, animTarget->start, animTarget->end);
	}
}

void ViewTarget::switchAnim(const std::string& animName)
{
	for (auto itr = _AnimList.begin(); itr != _AnimList.end();	++itr){
		if (itr->first == animName)	{
			_currAnim = itr;
			_Sprite3d->stopAllActions();
			_Sprite3d->runAction(RepeatForever::create(_currAnim->second));

			if (itr == _AnimList.begin())
				UiHandler::getInstance()->setAnimName(_currAnim->first,0,_MaxAnimFrame);
			else{
				auto animTarget = IndexFileParser::findAnim(_name, animName);
				if (animTarget != nullptr)
					UiHandler::getInstance()->setAnimName(animName, animTarget->start, animTarget->end);
			}

			break;
		}
	}
}

void ViewTarget::parseAnimSection(const AnimFileData& animFile, Animation3D* anim)
{
	for (auto it = animFile.animList.begin(); it != animFile.animList.end(); ++it){
		auto animate = Animate3D::createWithFrames(anim, it->start, it->end);
		_AnimList.insert(it->name, animate);
	}

	_currAnim = _AnimList.begin();
}

const std::string& ViewTarget::getCurrAnimName() const
{
	return _currAnim->first;
}

const std::string& ViewTarget::getTitle() const
{
	return _name;
}

const std::string& ViewTarget::getModelName() const
{
	return _modelName;
}
