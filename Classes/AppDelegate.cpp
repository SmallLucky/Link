#include "AppDelegate.h"
#include "Scene/StartScene.h"
#include "Data/Data.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

static cocos2d::Size mydesignResolutionSize = cocos2d::Size(640, 1136);
float DESIGN_RATIO = 0.56;  //  640/1136 = 0.56

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	//HttpClient::destroyInstance();
	//Configuration::getInstance()->destroyInstance();
	//ScriptEngineManager::getInstance()->destroyInstance();
	//PoolManager::getInstance()->destroyInstance();

}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::create("My Game");
		director->setOpenGLView(glview);
	}

	Size size = Director::getInstance()->getWinSize();

	float ratio = size.width / size.height;

	if (ratio >= DESIGN_RATIO)
	{
		float height = mydesignResolutionSize.height;
		float width = height * ratio;
		glview->setDesignResolutionSize(width, height, ResolutionPolicy::EXACT_FIT);
	}
	else
	{
		float width = mydesignResolutionSize.width;
		float height = width / ratio;
		glview->setDesignResolutionSize(width, height, ResolutionPolicy::EXACT_FIT);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//glview->setFrameSize(640, 1136);
	glview->setFrameSize(640, 960);
	//glview->setFrameSize(768, 1024);
	//glview->setFrameSize(320, 480);

#endif

	director->setDisplayStats(true); //不显示FPS
	director->setAnimationInterval(1.0 / 40); //设置FPS

	srand(time(nullptr)); //设置随机数种子

	//加载cocos studio导出的文件
	FileUtils::getInstance()->addSearchPath("res");

	//GameData::getInstance()->praseJsonData();
	//RewardData::getInstance()->praseJsonData();

	//TextureCache::getInstance()->addImage("bg/bg_1.png");

	//addMusic();

	//进入Start场景
	auto scene = StartScene::create();
	director->runWithScene(scene);

	return true;
}

void AppDelegate::addMusic()
{
	//预加载音效
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audio/bgmusic/battle_failed.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audio/bgmusic/battle_win.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audio/bgmusic/BGM_2.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audio/bgmusic/BGM_bat_2.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audio/bgmusic/fight_bgMusic.mp3");

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/button/button_pressed.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/button/buyKey.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/button/shopMoney.ogg");

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/1.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/2.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/3.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/4.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/5.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/6.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/7.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/8.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/9.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/10.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/11.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/12.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/13.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/14.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/link/15.ogg");

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/other/chest_main.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/other/go.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/other/hammer.ogg");

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/special/bomb.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/special/good.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/special/great.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/special/refresh.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/special/sword.ogg");
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
