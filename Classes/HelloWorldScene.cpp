#include "HelloWorldScene.h"

#include <string>
#include <iostream>
#include <fstream>

#include <stdio.h>
#include <3d/CCBundle3D.h>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto label = Label::createWithTTF("", "fonts/Arial.ttf", 34);
	label->setPosition(100, 100);

	this->addChild(label);

	/*char const *filePath = ((std::string) cocos2d::FileUtils::getInstance()->getWritablePath() + "orc.c3t").c_str();

	//CCLOG(filePath);

	fbxconv::log::Log log(new fbxconv::log::DefaultMessages(), -1);
	fbxconv::FbxConv conv(&log);
	int argc = 3;
	char const *argv[3] = {"", "-a", filePath};
	conv.execute(argc, argv);*/

	/*std::string _data = "C3B\0";

	std::string filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "data";

	CCLOG(filePath.c_str());

	FILE *fp = fopen(filePath.c_str(), "wb");

	fputs(_data.c_str(), fp);
	fclose(fp);*/

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
