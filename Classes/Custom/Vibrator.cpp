#include "Vibrator.h"
#include "CommonFunction.h"

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
	LOGD("void Vibrator::vibrate(int time)");
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getInstance", "()Ljava/lang/Object;");
	//先获得类的对象，然后用这个对象去调用它的非静态函数
	jobject jobj;
	if (ret)
	{
		LOGD("call static method");
		jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
		//getMethodInfo判断java定义的类非静态函数是否存在，返回bool
		bool re = JniHelper::getMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "vibrator", "(I)V");
		if (re)
		{
			LOGD("call no-static method");
			//非静态函数调用的时候，需要的是对象，所以与静态函数调用的第一个参数不同
			info.env->CallVoidMethod(jobj, info.methodID, time);
		}
	}
	else{
		LOGD("cant call static method");
	}

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

}

//停止震动
void Vibrator::cancelVibrate()
{
	log("Cancel vibrate");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "getInstance", "()Ljava/lang/Object;");
	//先获得类的对象，然后用这个对象去调用它的非静态函数
	jobject jobj;
	if (ret)
	{
		LOGD("call static method");
		jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
		//getMethodInfo判断java定义的类非静态函数是否存在，返回bool
		bool re = JniHelper::getMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "onStop", "()V");
		if (re)
		{
			//非静态函数调用的时候，需要的是对象，所以与静态函数调用的第一个参数不同
			info.env->CallVoidMethod(jobj, info.methodID);
		}
	}
	else{
		LOGD("cant call static method");
	}
#endif 
}