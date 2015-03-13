#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include <iostream>

USING_NS_CC;

int main(int argc, char *argv[])
{
	std::string path = "config.json";
	if (argc > 1)
		path = argv[1];

    AppDelegate app(path);
    return Application::getInstance()->run();
}