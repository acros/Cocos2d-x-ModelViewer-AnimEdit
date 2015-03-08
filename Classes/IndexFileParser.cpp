#include "IndexFileParser.h"
#include "json/reader.h"
#include "json/document.h"

USING_NS_CC;

AnimFileIndexList* IndexFileParser::parseIndexFile(const std::string& filePath)
{
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);

	rapidjson::Document doc;
	doc.Parse<0>(contentStr.c_str());

	if ( doc.HasParseError() || !doc.HasMember("list"))
		return nullptr;

	auto animIndexList = new AnimFileIndexList();
	rapidjson::Value&	na = doc["list"];
	int nodeSize = na.Size();
	for (int i = 0; i < nodeSize; ++i)
	{
		rapidjson::Value& nodeValue = na[i];

		//Parse node context
		AnimFileIndex	t;
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
				AnimFileIndex::AnimFrames secFrame;
				secFrame.name = itr->name.GetString();
				if (itr->value.IsArray()){
					secFrame.start = itr->value[0u].GetInt();
					secFrame.end = itr->value[1].GetInt();
				}
				t.animList.push_back(secFrame);
			}
		}

		animIndexList->push_back(t);
	}

	return animIndexList;
}
