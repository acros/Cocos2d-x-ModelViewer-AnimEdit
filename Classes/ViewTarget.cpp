#include "ViewTarget.h"

USING_NS_CC;

bool ViewTarget::init()
{
	return true;
}

ViewTarget::ViewTarget():
_orginDistance(0.0f)
{

}

ViewTarget::~ViewTarget()
{

}

bool ViewTarget::load(const AnimFileIndex& fileIdx)
{
	if (fileIdx.texFile.empty())
		_Sprite3d = Sprite3D::create(fileIdx.modelFile);
	else
		_Sprite3d = Sprite3D::create(fileIdx.modelFile, fileIdx.texFile);

	if (_Sprite3d)
	{
		auto animation = Animation3D::create(fileIdx.animFile);
		if (animation)
		{
			auto animate = Animate3D::create(animation);
			_Sprite3d->runAction(RepeatForever::create(animate));
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
