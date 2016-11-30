
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
	m_count.clear();
	m_coins.clear();
	m_special.clear();
	m_lovesmoneyJiao.clear();
	m_coinsmoneyJiao.clear();
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
		return nullptr;
	}

	return docment;

}

void GameData::copyFileToPath(std::string path)
{
	//��ȡ�ļ����Ƶ�ԭ·����Ŀ¼·��
	FileUtils *fu = FileUtils::getInstance();
	std::string wrpath = fu->getWritablePath();
	wrpath += path;
	log("%s", wrpath.c_str());
	if (!fu->isFileExist(wrpath)) {
		//��ȡapp�����·��
		std::string datapath = fu->fullPathForFilename(path.c_str());
		log("datapath=%s", datapath.c_str());
		//�������·������ȡ���ļ�
		std::string pFileContent = fu->getStringFromFile(datapath);
		CCLOG("%s", pFileContent.c_str());
		//���ļ����ݶ�����д�뵽ɳ����
		FILE *file = fopen(wrpath.c_str(), "w");
		if (file) {
			fputs(pFileContent.c_str(), file);
			fclose(file);
		}
	}

}

void GameData::praseFinish(rapidjson::Document &document)
{
	////�޸�json����֮������д��
	rapidjson::StringBuffer buffer; //��ʼ��������
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);//��ʼ��д����
	document.Accept(writer);		//��doc��������ݷŵ�writer�����buffer

	FILE* file = fopen(path.c_str(), "wb"); //������д��
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
		cout << "�������" << endl;
	}
}

void GameData::praseJsonData()
{
	auto doc = preparePrase();

	m_lock.clear();
	m_targetScore.clear();
	m_offsetX.clear();
	m_offsetY.clear();
	m_count.clear();
	m_coins.clear();
	m_special.clear();
	m_lovesmoneyJiao.clear();
	m_coinsmoneyJiao.clear();

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
		rapidjson::Value & shopcoins = chapter["shopcoins"];
		rapidjson::Value & special = chapter["special"];
		rapidjson::Value & shoplove = chapter["shoplove"];
		rapidjson::Value & shopcoinsM = chapter["shopcoinsM"];
		rapidjson::Value & shoploveM = chapter["shoploveM"];
		rapidjson::Value & shopcoinsJ = chapter["shopcoinsJ"];
		rapidjson::Value & shoploveJ = chapter["shoploveJ"];

		for (unsigned int a = 0; a < shoplove.Size(); a++)
		{
			rapidjson::Value & value = shoplove[a];
			m_loves.push_back(value.GetInt());
		}
		for (unsigned int b = 0; b < shopcoinsM.Size(); b++)
		{
			rapidjson::Value & value = shopcoinsM[b];
			m_coinsmoney.push_back(value.GetInt());
		}
		for (unsigned int d = 0; d < shoploveM.Size(); d++)
		{
			rapidjson::Value & value = shoploveM[d];
			m_lovesmoney.push_back(value.GetInt());
		}

		for (unsigned int c = 0; c < shopcoins.Size(); c++)
		{
			rapidjson::Value & value = shopcoins[c];
			m_coins.push_back(value.GetInt());
		}

		for (unsigned int e = 0; e < shopcoinsJ.Size(); e++)
		{
			rapidjson::Value & value = shopcoinsJ[e];
			m_coinsmoneyJiao.push_back(value.GetInt());
		}

		for (unsigned int f = 0; f < shoploveJ.Size(); f++)
		{
			rapidjson::Value & value = shoploveJ[f];
			m_lovesmoneyJiao.push_back(value.GetInt());
		}

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
		for (int m = 0; m < special.Size(); m++)
		{
			rapidjson::Value & value = special[m];
			m_special.push_back(value.GetInt());
		}

		cur_level = level.GetInt();
		cur_score = currScore.GetInt();
		diamond_num = diamond.GetInt();	//��ʯ����
		love_num = love.GetInt();
		money_num = money.GetInt();
		power_num = power.GetInt();
		refresh_num = refresh.GetInt();
		boom_num = boom.GetInt();
		//cout << "---------------------------" << endl;
		//printf("level = %d\n", level.GetInt());
		//for (int w = 0; w < m_lock.size(); w++)
		//{
		//	printf("lock:%i \n",m_lock[w]);
		//}
		//for (int n = 0; n < m_targetScore.size(); n++)
		//{
		//	printf("targetScore:%i \n",m_targetScore[n]);
		//}
		//cout << "---------------------------" << endl;
	}

	praseFinish(doc);
}

//��������
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

int GameData::getCoinsMoneyJiao(int index)
{
	if (index < 0)
	{
		return -1;
	}
	return m_coinsmoneyJiao[index];
}

int GameData::getLovesMoneyJiao(int index)
{
	if (index < 0)
	{
		return -1;
	}
	return m_lovesmoneyJiao[index];
}

int  GameData::getLock(int level)
{
	if (level < 0)
	{
		return -1;
	}
	return m_lock[level];
}

int  GameData::getCoinsMoney(int index)
{
	if (index < 0)
	{
		return -1;
	}
	return m_coinsmoney[index];
}
int  GameData::getLovesNum(int index)
{
	if (index < 0)
	{
		return -1;
	}
	return m_loves[index];
}
int  GameData::getLovesMoney(int index)
{
	if (index < 0)
	{
		return -1;
	}
	return m_lovesmoney[index];
}

int GameData:: getTargetScore(int level)
{
	if (level < 0)
	{
		return -1;
	}
	return m_targetScore[level];
}

int GameData::getShopCoins(int index)
{
	return m_coins[index];
}

int GameData::getSpecialNum(int level)
{
	if (level < 0 )
	{
		return -1;
	}
	return m_special[level];
}

int GameData::getCount(int level)
{
	if (level < 0)
	{
		return -1;
	}
	return m_count[level];
}

//���x��y��ƫ����
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
	//cout << "���õ�ǰ�ȼ�:" << cur_level << endl;
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
	//cout << "���õ�ǰ����:" << cur_score << endl;
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
	//cout << "������ʯ����:" << diamond_num << endl;
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

void GameData::setRefresh(int num)
{
	auto doc = preparePrase();
	refresh_num = num;

	rapidjson::Value& json_array = doc["Tollgate"];
	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value& frist = json_array[i];
		rapidjson::Value& chapter = frist["chapter"];

		rapidjson::Value& power = chapter["refresh"];
		power.SetInt(refresh_num);
	}
	praseFinish(doc);
}

void GameData::setBoomb(int num)
{
	auto doc = preparePrase();
	boom_num = num;

	rapidjson::Value& json_array = doc["Tollgate"];
	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value& frist = json_array[i];
		rapidjson::Value& chapter = frist["chapter"];

		rapidjson::Value& power = chapter["boom"];
		power.SetInt(boom_num);
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