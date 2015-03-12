#include "IndexFileParser.h"
#include "json/reader.h"
#include "json/document.h"

USING_NS_CC;

const float IndexFileParser::sFrameRate = 30.f;
const std::string IndexFileParser::s_DefaultAnim = "Default Animation";

AnimFileDataList IndexFileParser::s_AnimFileData;

AnimFileDataList* IndexFileParser::parseIndexFile(const std::string& filePath)
{
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);

	rapidjson::Document doc;
	doc.Parse<0>(contentStr.c_str());

	if ( doc.HasParseError() || !doc.HasMember("list"))
		return nullptr;

	s_AnimFileData.clear();

	rapidjson::Value&	na = doc["list"];
	int nodeSize = na.Size();
	for (int i = 0; i < nodeSize; ++i)
	{
		rapidjson::Value& nodeValue = na[i];

		//Parse node context
		AnimFileData	t;
		if (nodeValue.HasMember("model")){
			t.modelFile = nodeValue["model"].GetString();
		}

		if (nodeValue.HasMember("name")) {
			t.name = nodeValue["name"].GetString();
		}
		if (nodeValue.HasMember("tex")){
			t.texFile = nodeValue["tex"].GetString();
		}
		if (nodeValue.HasMember("anim")){
			t.animFile = nodeValue["anim"].GetString();
		}else{
			t.animFile = t.modelFile;
		}

		if (nodeValue.HasMember("sec") && nodeValue["sec"].IsArray()){
			rapidjson::Value& secValue = nodeValue["sec"][0u];
			for (auto itr = secValue.MemberonBegin(); itr != secValue.MemberonEnd(); ++itr){
				AnimFileData::AnimFrames secFrame;
				secFrame.name = itr->name.GetString();
				if (itr->value.IsArray()){
					secFrame.start = itr->value[0u].GetInt();
					secFrame.end = itr->value[1].GetInt();
				}
				t.animList.push_back(secFrame);
			}
		}

		s_AnimFileData.push_back(t);
	}

	return &s_AnimFileData;
}

// AnimFileData* IndexFileParser::find(const std::string& name)
// {
// 	for (auto itr = s_AnimFileData.begin(); itr != s_AnimFileData.end(); ++itr)
// 	{
// 		if (itr->name == name)
// 		{
// 			return &(*itr);
// 		}
// 	}
// }

AnimFileData::AnimFrames* IndexFileParser::findAnim(const std::string& modelName, const std::string& animName)
{
	auto itr = s_AnimFileData.begin();
	for (; itr != s_AnimFileData.end(); ++itr)
	{
		if (itr->name == modelName)
			break;
	}

	if (itr != s_AnimFileData.end())
	{
		for (auto animItr = itr->animList.begin(); animItr != itr->animList.end(); ++animItr){
			if (animItr->name == animName)
				return &(*animItr);
		}
	}

	return nullptr;
}


