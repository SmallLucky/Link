#include "AudioData.h" //����������Ч����Ϸ����

//const char* BGM = "Audio/other/hammer.ogg"; //��������
//
//const char* START_LINK_EFFECT = "Audio/other/hammer.ogg"; //��ʼ����ʱ����ЧlineXiaochu  start
//const char* LINK_EFFECT = "Audio/other/hammer.ogg"; //����ʱ����ЧpopBoom.mp3link.mp3
//const char* UNDO_EFFECT = "Audio/other/hammer.ogg"; //��������ʱ����Чundo
//const char* REMOVE_EFFECT = "Audio/other/hammer.ogg"; //����ʱ����Чdododod.mp3 remove2.mp3 
//const char* WRONG_REMOVE_EFFECT = "Audio/other/hammer.ogg"; //��������ʱ����Чwrong
//
//const char* GAME_OVER_EFFECT = "Audio/other/hammer.ogg"; //��Ϸ��������Ч
//const char* NEW_RECORD_EFFECT = "Audio/other/hammer.ogg"; //�¼�¼����Ч
//const char* CLICK_BUTTON = "Audio/other/hammer.ogg"; //���button��Ч
//
//const char* ELEMENT_BOOM = "Audio/other/hammer.ogg"; //Ԫ�ر�ը��Ч
//
//const char* ELEMENT_LINE = "Audio/other/hammer.ogg"; //����Ԫ��Ԫ����Ч
//const char* ELEMENT_ROW = "Audio/other/hammer.ogg"; //Ԫ����Ч
//const char* ELEMENT_ROUND = "Audio/other/hammer.ogg"; //Ԫ����ЧshopMoney.ogg
//const char* SHOP_MONEY = "Audio/other/hammer.ogg";


AudioData* AudioData::_instance = nullptr;
AudioData::AudioData()
{

}
AudioData::~AudioData()
{

}

AudioData* AudioData::getInstance()
{
	if (!_instance)
	{
		_instance = new AudioData();
	}
	return _instance;
}

void AudioData::addBgMusic(int num)
{
	FileUtils *fu = FileUtils::getInstance();
	log("addBgMusic(int num)");
	switch (num)
	{
	case 1://start
		if (fu->isFileExist("Audio/bgmusic/fight_bgMusic.mp3"))
		{
			log("addBgMusic(int 1)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/bgmusic/fight_bgMusic.mp3", true);
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
		}
		break;
	case 2://level
		if (fu->isFileExist("Audio/bgmusic/BGM_bat_2.mp3"))
		{
			log("addBgMusic(int 2)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/bgmusic/BGM_bat_2.mp3", true);
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8);
		}
		break;
	case 3://game
		if (fu->isFileExist("Audio/bgmusic/BGM_2.mp3"))
		{
			log("addBgMusic(int 3)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/bgmusic/BGM_2.mp3", true);
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8);
		}

		break;
	case 4://win
		if (fu->isFileExist("Audio/bgmusic/battle_win.mp3"))
		{
			log("addBgMusic(int 4)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/bgmusic/battle_win.mp3", true);
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
		}
		break;
	case 5://loser
		if (fu->isFileExist("Audio/bgmusic/battle_failed.mp3"))
		{
			log("addBgMusic(int 5)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/bgmusic/battle_failed.mp3", true);
			CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
		}
		break;
	default:
		break;
	}
}

void AudioData::addButtonEffect(int num)
{
	FileUtils *fu = FileUtils::getInstance();
	log("addButtonEffect(int num)");
	switch (num)
	{
	case 1:
		if (fu->isFileExist("Audio/button/button_pressed.ogg"))
		{
			log("addButtonEffect(int 1)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/button/button_pressed.ogg");
		}
		break;
	case 2:
		if (fu->isFileExist("Audio/button/buyKey.ogg"))
		{
			log("addButtonEffect(int 2)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/button/buyKey.ogg");
		}
		break;
	case 3:
		if (fu->isFileExist("Audio/button/shopMoney.ogg"))
		{
			log("addButtonEffect(int 3)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/button/shopMoney.ogg");
		}
		break;
	default:
		break;
	}
}

void  AudioData::addOtherEffect(int num)
{
	FileUtils *fu = FileUtils::getInstance();
	log("addOtherEffect(int num)");
	switch (num)
	{
	case 1:
		if (fu->isFileExist("Audio/other/chest_main.ogg"))
		{
			log("addOtherEffect(int 1)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/other/chest_main.ogg");
		}
		break;
	case 2:
		if (fu->isFileExist("Audio/other/go.ogg"))
		{
			log("addOtherEffect(int 2)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/other/go.ogg");
		}
		break;
	case 3:
		if (fu->isFileExist("Audio/other/hammer.ogg"))
		{
			log("addOtherEffect(int 3)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/other/hammer.ogg");
		}
		break;
	default:
		break;
	}
}
void  AudioData::addSpecialEffect(int num)
{
	FileUtils *fu = FileUtils::getInstance();
	log("addSpecialEffect(int num)");
	switch (num)
	{
	case 1:
		if (fu->isFileExist("Audio/special/bomb.ogg"))
		{
			log("addSpecialEffect(int 1)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/special/bomb.ogg");
		}
		break;
	case 2:
		if (fu->isFileExist("Audio/special/good.ogg"))
		{
			log("addSpecialEffect(int 2)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/special/good.ogg");
		}
		break;
	case 3:
		if (fu->isFileExist("Audio/special/great.ogg"))
		{
			log("addSpecialEffect(int 3)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/special/great.ogg");
		}
		break;
	case 4:
		if (fu->isFileExist("Audio/special/refresh.ogg"))
		{
			log("addSpecialEffect(int 4)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/special/refresh.ogg");
		}
		break;
	case 5:
		if (fu->isFileExist("Audio/special/sword.ogg"))
		{
			log("addSpecialEffect(int 5)");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/special/sword.ogg");
		}
		break;
	default:
		break;
	}
}
void  AudioData::addLinkEffect(int num)
{
	FileUtils *fu = FileUtils::getInstance();
	log("addLinkEffect(int num)");
	if (fu->isFileExist(StringUtils::format("Audio/link/%d.ogg", num)))
	{
		log("addLinkEffect(int %d)",num);
		string str = StringUtils::format("Audio/link/%d.ogg", num);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(str.c_str());
	}
}


void AudioData::stopBgMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}