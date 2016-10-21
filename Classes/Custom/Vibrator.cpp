#include "Vibrator.h"
#include "CommonFunction.h"

using namespace Custom;

//��ʼ��
void Vibrator::vibrate(int time)
{
	if (!time)
	{
		return;
	}
	log("Vibrate %dms", time);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("void Vibrator::vibrate(int time)");
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getInstance", "()Ljava/lang/Object;");
	//�Ȼ����Ķ���Ȼ�����������ȥ�������ķǾ�̬����
	jobject jobj;
	if (ret)
	{
		LOGD("call static method");
		jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
		//getMethodInfo�ж�java�������Ǿ�̬�����Ƿ���ڣ�����bool
		bool re = JniHelper::getMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "vibrator", "(I)V");
		if (re)
		{
			LOGD("call no-static method");
			//�Ǿ�̬�������õ�ʱ����Ҫ���Ƕ��������뾲̬�������õĵ�һ��������ͬ
			info.env->CallVoidMethod(jobj, info.methodID, time);
		}
	}
	else{
		LOGD("cant call static method");
	}

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

}

//ֹͣ��
void Vibrator::cancelVibrate()
{
	log("Cancel vibrate");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getInstance", "()Ljava/lang/Object;");
	//�Ȼ����Ķ���Ȼ�����������ȥ�������ķǾ�̬����
	jobject jobj;
	if (ret)
	{
		LOGD("call static method");
		jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
		//getMethodInfo�ж�java�������Ǿ�̬�����Ƿ���ڣ�����bool
		bool re = JniHelper::getMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "onStop", "()V");
		if (re)
		{
			//�Ǿ�̬�������õ�ʱ����Ҫ���Ƕ��������뾲̬�������õĵ�һ��������ͬ
			info.env->CallVoidMethod(jobj, info.methodID);
		}
	}
	else{
		LOGD("cant call static method");
	}
#endif 
}