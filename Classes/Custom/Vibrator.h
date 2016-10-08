
#pragma once

#include "cocos2d.h"
USING_NS_CC;
/*
目前只实现了Android端的震动功能，需要通过JNI调用Java函数，要在Android文件中进行修改
在游戏目录下 proj.android\src\org\cocos2dx\lib\Cocos2dxHelper.java 文件中修改
在 public class Cocos2dxHelper 类中添加，并引入对应包
public static void vibrate(int vibrateTime) {
Vibrator vib = (Vibrator) sActivity.getSystemService(Service.VIBRATOR_SERVICE);
vib.vibrate(vibrateTime);
}
public static void cancelVibrate() {
Vibrator vib = (Vibrator) sActivity.getSystemService(Service.VIBRATOR_SERVICE);
vib.cancel();
}

在游戏目录下 proj.android\AndroidManifest.xml 中添加
<uses-permission android:name="android.permission.VIBRATE"/>
*/

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"

namespace Custom
{
	class Vibrator
	{
	public:
		static void vibrate(int time); //开始震动（毫秒）
		static void cancelVibrate();	//停止震动
	};
}