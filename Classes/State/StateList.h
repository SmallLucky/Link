#pragma once
#include "Global.h"
#include "State.h"

//DEFINE_STATE(XXState)��END_DEFINE_STATE����XXState��֮�䶨����Ի�����

DEFINE_STATE(StartState) //��ʼ״̬

END_DEFINE_STATE

DEFINE_STATE(InitState) //��ʼ��״̬
float matrixTime;
bool matrixFinishFlag;
END_DEFINE_STATE

DEFINE_STATE(LinkState) //�ȴ�����״̬
bool gameOverFlag;
bool refreshElement;
END_DEFINE_STATE

DEFINE_STATE(RemoveState) //����״̬
float matrixTime;
bool matrixFinishFlag;
bool gameOverFlag;
bool gameStartFlag;
bool IsLineBoom;
END_DEFINE_STATE

DEFINE_STATE(LineBoomState) //��ʼ״̬
float matrixTime;
bool matrixFinishFlag;
bool gameStartFlag;
END_DEFINE_STATE

DEFINE_STATE(GameOverState) //��Ϸ����״̬
END_DEFINE_STATE