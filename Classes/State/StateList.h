#pragma once
#include "Global.h"
#include "State.h"

//DEFINE_STATE(XXState)和END_DEFINE_STATE定义XXState，之间定义个性化参数

DEFINE_STATE(StartState) //开始状态

END_DEFINE_STATE

DEFINE_STATE(InitState) //初始化状态
float matrixTime;
bool matrixFinishFlag;
END_DEFINE_STATE

DEFINE_STATE(LinkState) //等待连线状态
bool gameOverFlag;
bool refreshElement;
END_DEFINE_STATE

DEFINE_STATE(RemoveState) //消除状态
float matrixTime;
bool matrixFinishFlag;
bool gameOverFlag;
bool gameStartFlag;
bool IsLineBoom;
END_DEFINE_STATE

DEFINE_STATE(LineBoomState) //开始状态
float matrixTime;
bool matrixFinishFlag;
bool gameStartFlag;
END_DEFINE_STATE

DEFINE_STATE(GameOverState) //游戏结束状态
END_DEFINE_STATE