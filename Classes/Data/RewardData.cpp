
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
}

RewardData* RewardData:: getInstance()
{
	if (!_instance)
	{
		_instance = new RewardData();
	}
	return _instance;
}
bool	RewardData::purgeInstance()//���ʵ��
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
		return NULL;
	}

	return docment;
}
void	RewardData::copyFileToPath(std::string path)
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

void	RewardData::praseFinish(rapidjson::Document &document)
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