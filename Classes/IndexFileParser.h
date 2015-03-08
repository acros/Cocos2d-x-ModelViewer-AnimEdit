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

struct AnimFileIndex{
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
typedef std::vector<AnimFileIndex>	AnimFileIndexList;


class IndexFileParser{
protected:
	IndexFileParser();
	~IndexFileParser();

public:

	static AnimFileIndexList*	parseIndexFile(const std::string&	filePath);
};

#endif