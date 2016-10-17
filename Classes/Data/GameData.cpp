
#include "GameData.h"


GameData* GameData::_instance = nullptr;
GameData::GameData():
cur_level(0),
cur_score(0),
history_score(0),
diamond_num(0)
{

}

GameData::~GameData()
{
	m_lock.clear();
	m_targetScore.clear();
	m_offsetX.clear();
	m_offsetY.clear();
}

GameData* GameData::getInstance()
{
	if (!_instance)
	{
		_instance = new GameData();
	}
	return _instance;
}

bool GameData::purgeInstance()
{
	CCASSERT(_instance, "instance can not emptoy");
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
	return true;
}

rapidjson::Document GameData::preparePrase()
{
	FileUtils * fileUtils = FileUtils::getInstance();
	path = fileUtils->getWritablePath();
	string fileName = "tollgate.json";
	path += fileName;
	this->copyFileToPath(fileName);
	
	rapidjson::Document docment;
	string data = fileUtils->getStringFromFile(path);
	string clearData(data);
	size_t pos = clearData.rfind("}");
	clearData = clearData.substr(0,pos+1);
	
	docment.Parse<4>(clearData.c_str());

	if (docment.HasParseError()) {
		log("GetParaseErro %s\n", docment.GetParseError());
		return NULL;
	}

	return docment;

}

void GameData::copyFileToPath(std::string path)
{
	//获取文件复制的原路径和目录路径
	FileUtils *fu = FileUtils::getInstance();
	std::string wrpath = fu->getWritablePath();
	wrpath += path;
	log("%s", wrpath.c_str());
	if (!fu->isFileExist(wrpath)) {
		//获取app里面的路径
		std::string datapath = fu->fullPathForFilename(path.c_str());
		log("datapath=%s", datapath.c_str());
		//根据这个路径来获取到文件
		std::string pFileContent = fu->getStringFromFile(datapath);
		CCLOG("%s", pFileContent.c_str());
		//将文件内容读出，写入到沙盒中
		FILE *file = fopen(wrpath.c_str(), "w");
		if (file) {
			fputs(pFileContent.c_str(), file);
			fclose(file);
		}
	}

}

void GameData::praseFinish(rapidjson::Document &document)
{
	////修改json内容之后重新写入
	rapidjson::StringBuffer buffer; //初始化缓冲区
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);//初始化写入器
	document.Accept(writer);		//将doc里面的内容放到writer里面的buffer

	FILE* file = fopen(path.c_str(), "wb"); //二进制写入
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
		cout << "解析完成" << endl;
	}
}

void GameData::praseJsonData()
{
	auto doc = preparePrase();

	m_lock.clear();
	m_targetScore.clear();

	rapidjson::Value & json_array = doc["Tollgate"];

	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value & frist = json_array[i];
		rapidjson::Value & chapter = frist["chapter"];
		//
		rapidjson::Value & lock = chapter["lock"];
		rapidjson::Value & level = chapter["level"];
		rapidjson::Value & targetScore = chapter["targetScore"];
		rapidjson::Value & offsetX = chapter["offsetX"];
		rapidjson::Value & offsetY = chapter["offsetY"];
		rapidjson::Value & power = chapter["power"];
		rapidjson::Value & diamond = chapter["diamond"];
		rapidjson::Value & love = chapter["love"];
		rapidjson::Value & money = chapter["money"];
		rapidjson::Value & currScore = chapter["currScore"];
		rapidjson::Value & count = chapter["count"];
		rapidjson::Value & refresh = chapter["refresh"];
		rapidjson::Value & boom = chapter["boom"];

		for (unsigned int j = 0; j < lock.Size(); j++)
		{
			rapidjson::Value & value = lock[j];
			m_lock.push_back(value.GetInt());
		}
		for (unsigned int k = 0; k < targetScore.Size(); k++)
		{
			rapidjson::Value& value = targetScore[k];
			m_targetScore.push_back(value.GetInt());
		}
		for (unsigned int x = 0; x < offsetX.Size(); x++)
		{
			rapidjson::Value & value = offsetX[x];
			m_offsetX.push_back(value.GetInt());
		}
		for (unsigned int y = 0; y < offsetY.Size(); y++)
		{
			rapidjson::Value& value = offsetY[y];
			m_offsetY.push_back(value.GetInt());
		}
		for (unsigned int l = 0; l < count.Size(); l++)
		{
			rapidjson::Value & value = count[l];
			m_count.push_back(value.GetInt());
		}
		cur_level = level.GetInt();
		cur_score = currScore.GetInt();
		diamond_num = diamond.GetInt();	//钻石数量
		love_num = love.GetInt();
		money_num = money.GetInt();
		power_num = power.GetInt();
		refresh_num = refresh.GetInt();
		boom_num = boom.GetInt();
		cout << "---------------------------" << endl;
		printf("level = %d\n", level.GetInt());
		for (int w = 0; w < m_lock.size(); w++)
		{
			printf("lock:%i \n",m_lock[w]);
		}
		for (int n = 0; n < m_targetScore.size(); n++)
		{
			printf("targetScore:%i \n",m_targetScore[n]);
		}
		cout << "---------------------------" << endl;
	}

	praseFinish(doc);
}

//解锁设置
void GameData::setLock(int level_index)
{
	auto doc = preparePrase();

	rapidjson::Value & json_array = doc["Tollgate"];

	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value & frist = json_array[i];
		rapidjson::Value & chapter = frist["chapter"];
		rapidjson::Value & lock = chapter["lock"];
		lock[level_index] = 0;
	}
	praseFinish(doc);
}

int  GameData::getLock(int level)
{
	if (level < 0)
	{
		return -1;
	}
	return m_lock[level];
}

int GameData:: getTargetScore(int level)
{
	if (level < 0)
	{
		return -1;
	}
	return m_targetScore[level];
}

int GameData::getCount(int level)
{
	if (level < 0)
	{
		return -1;
	}
	return m_count[level];
}

//获得x，y的偏移量
int  GameData::getOffsetX(int level)
{
	if (level < 0)
	{
		return 0;
	}
	return m_offsetX[level];
}
int  GameData::getOffsetY(int level)
{
	if (level < 0 )
	{
		return 0;
	}
	return m_offsetY[level];
}

void GameData::setCurLevel(int level)
{
	auto doc = preparePrase();
	cur_level = level;

	rapidjson::Value &json_array = doc["Tollgate"];

	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value & first = json_array[i];
		rapidjson::Value & chapter = first["chapter"];
		rapidjson::Value & level = chapter["level"];
		level.SetInt(cur_level);
	}
	praseFinish(doc);
	//cout << "设置当前等级:" << cur_level << endl;
}

void GameData::setCurScore(int _score)
{
	auto doc = preparePrase();
	cur_score = _score;
	rapidjson::Value & json_array = doc["Tollgate"];
	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value & first = json_array[i];
		rapidjson::Value & chapter = first["chapter"];

		rapidjson::Value & curScore = chapter["currScore"];
		curScore.SetInt(cur_score);
	}
	praseFinish(doc);
	//cout << "设置当前分数:" << cur_score << endl;
}

void GameData::setDiamondNum(int _num)
{
	auto doc = preparePrase();
	diamond_num = _num;

	rapidjson::Value &json_array = doc["Tollgate"];
	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value & first = json_array[i];
		rapidjson::Value & chapter = first["chapter"];

		rapidjson::Value & diamondNum = chapter["diamondNum"];
		diamondNum.SetInt(diamond_num);
	}
	praseFinish(doc);
	//cout << "设置钻石数量:" << diamond_num << endl;
}

void GameData::setLoveNum(int _num)
{
	auto doc = preparePrase();
	love_num = _num;

	rapidjson::Value & json_array = doc["Tollgate"];
	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value & frist = json_array[i];
		rapidjson::Value & chapter = frist["chapter"];

		rapidjson::Value& love = chapter["love"];
		love.SetInt(love_num);
	}
	praseFinish(doc);
}

void GameData::setPowerNum(int _num)
{
	auto doc = preparePrase();
	power_num = _num;

	rapidjson::Value& json_array = doc["Tollgate"];
	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value& frist = json_array[i];
		rapidjson::Value& chapter = frist["chapter"];

		rapidjson::Value& power = chapter["power"];
		power.SetInt(power_num);
	}
	praseFinish(doc);
}

void GameData::setMoneyNum(int _num)
{
	auto doc = preparePrase();
	money_num = _num;

	rapidjson::Value& json_array = doc["Tollgate"];
	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value& frist = json_array[i];
		rapidjson::Value& chapter = frist["chapter"];

		rapidjson::Value& money = chapter["money"];
		money.SetInt(money_num);
	}
	praseFinish(doc);
}