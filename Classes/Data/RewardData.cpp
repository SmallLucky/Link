
#include "RewardData.h"

RewardData* RewardData::_instance = nullptr;

RewardData::RewardData()
{

}

RewardData::~RewardData()
{
	m_power.clear();
	m_love.clear();
	m_coins.clear();
	m_offsetX.clear();
	m_offsetY.clear();
	m_refresh.clear();
	m_boom.clear();

	m_blue.clear();
	m_purple.clear();
	m_green.clear();
	m_rad.clear();
	m_yellor.clear();

	m_isFristPlay.clear();
}

RewardData* RewardData:: getInstance()
{
	if (!_instance)
	{
		_instance = new RewardData();
	}
	return _instance;
}
bool	RewardData::purgeInstance()//清除实例
{
	CCASSERT(_instance, "instance can not emptoy");
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
	return true;
}

rapidjson::Document RewardData::preparePrase()
{
	FileUtils * fileUtils = FileUtils::getInstance();
	path = fileUtils->getWritablePath();
	string fileName = "reward.json";
	path += fileName;
	this->copyFileToPath(fileName);

	rapidjson::Document docment;
	string data = fileUtils->getStringFromFile(path);
	string clearData(data);
	size_t pos = clearData.rfind("}");
	clearData = clearData.substr(0, pos + 1);

	docment.Parse<4>(clearData.c_str());

	if (docment.HasParseError()) {
		log("GetParaseErro %s\n", docment.GetParseError());
		return nullptr;
	}

	return docment;
}
void	RewardData::copyFileToPath(std::string path)
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

void	RewardData::praseFinish(rapidjson::Document &document)
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

void	RewardData::praseJsonData()
{
	auto doc = preparePrase();

	m_power.clear();
	m_love.clear();
	m_coins.clear();
	m_offsetX.clear();
	m_offsetY.clear();
	m_refresh.clear();
	m_boom.clear();

	m_blue.clear();
	m_purple.clear();
	m_green.clear();
	m_rad.clear();
	m_yellor.clear();

	m_isFristPlay.clear();
	rapidjson::Value & json_array = doc["Tollgate"];

	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value & frist = json_array[i];
		rapidjson::Value & chapter = frist["chapter"];

		rapidjson::Value & power = chapter["power"];
		rapidjson::Value & love = chapter["love"];
		rapidjson::Value & coins = chapter["coins"];
		rapidjson::Value & offsetX = chapter["offsetX"];
		rapidjson::Value & offsetY = chapter["offsetY"];
		rapidjson::Value & refresh = chapter["refresh"];
		rapidjson::Value & boom = chapter["boom"]; 

		rapidjson::Value & blue = chapter["blue"];
		rapidjson::Value & purple = chapter["purple"];
		rapidjson::Value & green = chapter["green"];
		rapidjson::Value & rad = chapter["rad"];
		rapidjson::Value & yellor = chapter["yellor"];
		rapidjson::Value & isFristPlay = chapter["isFristPlay"];

		for (unsigned int a = 0; a < blue.Size(); a++)
		{
			rapidjson::Value & value = blue[a];
			m_blue.push_back(value.GetInt());
		}

		for (unsigned int b = 0; b < purple.Size(); b++)
		{
			rapidjson::Value & value = purple[b];
			m_purple.push_back(value.GetInt());
		}
		for (unsigned int c = 0; c < green.Size(); c++)
		{
			rapidjson::Value & value = green[c];
			m_green.push_back(value.GetInt());
		}
		for (unsigned int d = 0; d < rad.Size(); d++)
		{
			rapidjson::Value & value = rad[d];
			m_rad.push_back(value.GetInt());
		}
		for (unsigned int e = 0; e < yellor.Size(); e++)
		{
			rapidjson::Value & value = yellor[e];
			m_yellor.push_back(value.GetInt());
		}

		for (unsigned int f = 0; f < isFristPlay.Size(); f++)
		{
			rapidjson::Value & value = isFristPlay[f];
			m_isFristPlay.push_back(value.GetInt());
		}

		for (unsigned int l = 0; l < power.Size(); l++)
		{
			rapidjson::Value & value = power[l];
			m_power.push_back(value.GetInt());
		}
		for (unsigned int j = 0; j < love.Size(); j++)
		{
			rapidjson::Value & value = love[j];
			m_love.push_back(value.GetInt());
		}
		for (unsigned int k = 0; k < coins.Size(); k++)
		{
			rapidjson::Value & value = coins[k];
			m_coins.push_back(value.GetInt());
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
		for (unsigned int n = 0; n < refresh.Size(); n++)
		{
			rapidjson::Value & value = refresh[n];
			m_refresh.push_back(value.GetInt());
		}
		for (unsigned int m = 0; m < boom.Size(); m++)
		{
			rapidjson::Value & value = boom[m];
			m_boom.push_back(value.GetInt());
		}
	}
	praseFinish(doc);
}

int		RewardData::getIsFristPlay(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_isFristPlay[index];
}
void	RewardData::setIsFristPlay(int level)
{
	auto doc = preparePrase();

	rapidjson::Value & json_array = doc["Tollgate"];

	for (unsigned int i = 0; i < json_array.Size(); i++)
	{
		rapidjson::Value & frist = json_array[i];
		rapidjson::Value & chapter = frist["chapter"];
		rapidjson::Value & isFristPlay = chapter["isFristPlay"];
		isFristPlay[level] = 2;
	}
	praseFinish(doc);
}

int		RewardData::getPower(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_power[index];
}
int		RewardData::getLove(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_love[index];
}
int		RewardData::getCoins(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_coins[index];
}
int		RewardData::getOffsetX(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_offsetX[index];
}
int		RewardData::getOffsetY(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_offsetY[index];
}
int		RewardData::getRefresh(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_refresh[index];
}
int		RewardData::getBoom(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_boom[index];
}

int RewardData::getBlue(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_blue[index];
}
int RewardData::getPurple(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_purple[index];
}
int RewardData::getGreen(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_green[index];
}
int RewardData::getRad(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_rad[index];
}
int RewardData::getYellor(int index)
{
	if (index < 0)
	{
		return 0;
	}
	return m_yellor[index];
}