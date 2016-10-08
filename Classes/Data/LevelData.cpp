
#include "LevelData.h"

LevelData* LevelData::_instance = nullptr;

LevelData::LevelData()
{

}

LevelData::~LevelData()
{

}
LevelData* LevelData::getInstance()
{
	if (!_instance)
	{
		_instance = new LevelData();
	}
	return _instance;
}

//创建一个json文件
void  LevelData::createJson()
{

}