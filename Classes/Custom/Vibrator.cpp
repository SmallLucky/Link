#include "Vibrator.h"

using namespace Custom;

//开始震动
void Vibrator::vibrate(int time)
{
	if (!time)
	{
		return;
	}
	log("Vibrate %dms", time);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "vibrate", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, time);
		t.env->DeleteLocalRef(t.classID);
	}
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

}

//停止震动
void Vibrator::cancelVibrate()
{
	log("Cancel vibrate");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "cancelVibrate", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#endif 
}