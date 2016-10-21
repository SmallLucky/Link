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
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "vibrator", "(I)V"))
	{
		LOGD("void Vibrator::vibrate(int 2)");
		t.env->CallStaticVoidMethod(t.classID, t.methodID, time);
		//t.env->DeleteLocalRef(t.classID);
	}
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

}

//ֹͣ��
void Vibrator::cancelVibrate()
{
	log("Cancel vibrate");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "onStop", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		//t.env->DeleteLocalRef(t.classID);
	}
#endif 
}