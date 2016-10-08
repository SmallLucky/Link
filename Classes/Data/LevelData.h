#pragma  once

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;
using namespace rapidjson;

class LevelData
{
public:
	static LevelData* getInstance();

	int getLock(int level); // 获得是否上锁 0 and 1
	void setLock(int level);

	Point getButtonPoint(int level); //通过关卡获得button的坐标

	void createJson(); //创建一个json文件
	 

private:
	static LevelData* _instance;

private:
	LevelData();
	~LevelData();

};

