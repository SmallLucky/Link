#pragma once

#include "Global.h"

//����������Ч����Ϸ����

//extern const char* BGM; //��������
//
//extern const char* START_LINK_EFFECT; //��ʼ����ʱ����Ч
//extern const char* LINK_EFFECT; //����ʱ����Ч
//extern const char* UNDO_EFFECT; //��������ʱ����Ч
//extern const char* REMOVE_EFFECT; //����ʱ����Ч
//extern const char* WRONG_REMOVE_EFFECT; //��������ʱ����Ч
//
//extern const char* GAME_OVER_EFFECT; //��Ϸ��������Ч
//extern const char* NEW_RECORD_EFFECT; //�¼�¼����Ч
//extern const char* CLICK_BUTTON; //���button��Ч
//extern const char* ELEMENT_BOOM; //Ԫ�ر�ը��Ч
//
//extern const char* ELEMENT_LINE; //����Ԫ��Ԫ����Ч
//extern const char* ELEMENT_ROW; //Ԫ����Ч
//extern const char* ELEMENT_ROUND; //Ԫ����Ч
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