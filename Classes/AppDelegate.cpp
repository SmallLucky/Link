#include "AppDelegate.h"
#include "Scene/StartScene.h"

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

	srand(time(NULL)); //设置随机数种子

	////预加载音乐音效
	//auto audio = SimpleAudioEngine::getInstance();
	//audio->preloadBackgroundMusic(BGM);
	//audio->preloadEffect(START_LINK_EFFECT);
	//audio->preloadEffect(LINK_EFFECT);
	//audio->preloadEffect(UNDO_EFFECT);
	//audio->preloadEffect(REMOVE_EFFECT);
	//audio->preloadEffect(WRONG_REMOVE_EFFECT);
	//audio->preloadEffect(GAME_OVER_EFFECT);
	//audio->preloadEffect(NEW_RECORD_EFFECT);

	//加载cocos studio导出的文件
	FileUtils::getInstance()->addSearchPath("res");

	//进入Start场景
	auto scene = StartScene::create();
	director->runWithScene(scene);

	return true;
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
