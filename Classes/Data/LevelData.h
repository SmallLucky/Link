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

	int getLock(int level); // ����Ƿ����� 0 and 1
	void setLock(int level);

	Point getButtonPoint(int level); //ͨ���ؿ����button������

	void createJson(); //����һ��json�ļ�
	 

private:
	static LevelData* _instance;

private:
	LevelData();
	~LevelData();

};

