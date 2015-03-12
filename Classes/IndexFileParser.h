/*
	Author: Zhao Zhihui
	Data:	2015.3.3
	github:	https://github.com/acros
	Email:	inary88@gmail.com
*/

#ifndef __ANIM_INDEX_FILE_PARSER_H__
#define __ANIM_INDEX_FILE_PARSER_H__

#include "cocos2d.h"

using std::string;

struct AnimFileData{
	string name;
	string modelFile;
	string texFile;
	string animFile;

	struct AnimFrames{
		AnimFrames() :start(0), end(0)	{}
		string name;
		int start;
		int end;
	};
	std::vector<AnimFrames>	animList;
};
typedef std::vector<AnimFileData>	AnimFileDataList;


class IndexFileParser{
protected:
	IndexFileParser();
	~IndexFileParser();

public:

	static AnimFileDataList*	parseIndexFile(const std::string&	filePath);

	static AnimFileData::AnimFrames*	findAnim(const std::string&	modelName, const std::string& animName);


	static AnimFileDataList		s_AnimFileData;
	static const float			sFrameRate;
	static const std::string	s_DefaultAnim;

};

#endif