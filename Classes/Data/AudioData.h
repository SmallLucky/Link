#pragma once

#include "Global.h"

//关于音乐音效的游戏数据

//extern const char* BGM; //背景音乐
//
//extern const char* START_LINK_EFFECT; //开始连接时的音效
//extern const char* LINK_EFFECT; //连接时的音效
//extern const char* UNDO_EFFECT; //撤销连接时的音效
//extern const char* REMOVE_EFFECT; //消除时的音效
//extern const char* WRONG_REMOVE_EFFECT; //错误消除时的音效
//
//extern const char* GAME_OVER_EFFECT; //游戏结束的音效
//extern const char* NEW_RECORD_EFFECT; //新纪录的音效
//extern const char* CLICK_BUTTON; //点击button音效
//extern const char* ELEMENT_BOOM; //元素爆炸音效
//
//extern const char* ELEMENT_LINE; //特殊元素元素音效
//extern const char* ELEMENT_ROW; //元素音效
//extern const char* ELEMENT_ROUND; //元素音效
//extern const char* SHOP_MONEY;

class AudioData
{
private:
	AudioData();
	~AudioData();
	static AudioData* _instance;
	 //
public:
	void addBgMusic(int num);
	void stopBgMusic();

	void addButtonEffect(int num);
	void addOtherEffect(int num);
	void addSpecialEffect(int num);
	void addLinkEffect(int num);

public:

	static AudioData* getInstance();
};