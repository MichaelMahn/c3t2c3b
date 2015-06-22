#define COCOS2D_DEBUG 1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <fbxsdk.h>
#include <iostream>
#include <fstream>
#include <3d/CCBundle3D.h>

#include "../Classes/src/Settings.h"
#include "../Classes/src/FbxConv.h"
#include "../Classes/src/FbxConvCommand.h"
#include "../Classes/src/json2/JSONWriter.h"
#include "../Classes/src/json2/UBJSONWriter.h"
#include "../Classes/src/log/messages.h"

#include "../Classes/AppDelegate.h"



USING_NS_CC;

int main(int argc, char **argv)
{

	char const *filePath = ((std::string) cocos2d::FileUtils::getInstance()->getWritablePath() + "orc.c3t").c_str();

	fbxconv::log::Log log(new fbxconv::log::DefaultMessages, -1);
	fbxconv::FbxConv conv(&log);
	int argc1 = 3;
	char const *argv1[3] = {"", "-a", filePath};
	conv.execute(argc1, argv1);
	/*AppDelegate app;
	return Application::getInstance()->run();*/
}
