#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Terrain.h"
#include "Path.h"
#include "Shadow.h"
#include "Forest.h"
#include<vector>

class HelloWorld: public cocos2d::CCLayer {
public:
	HelloWorld();
	~HelloWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld)
	;
	virtual void keyBackClicked();
	void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
	void ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
	void ccTouchesCancelled(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);

private:
	void loadTextures();

	void tick(float dt);
	void updateLevel(long st, float offset, float yoff, int gap);
	void updateSky(long st, float yoff);
	void updateMountain(float offset, float yoff);
	void reset();
	long getCurTimeInMilliSeconds();
	void playMusic(float volume, bool isLooped, std::list<std::string> musicList);
	/**
	 * 狐狸的动画做到第几帧。默认为第一帧
	 */
	int animFrame;
	/**
	 * 当前动画持续的时间
	 */
	int animTime;
	/**
	 * 当狐狸的关键帧改变时，记录当前的动画时间
	 */
	int lastAnimTime;
	/**
	 * 上一帧动画开始的时间
	 */
	long lastAnimFrameTime;
	float lastY;
	float lastYoff;std::list<cocos2d::CCPoint> locations;
	int gameOverTime;
	long titleScreenTime;
	/**
	 * 上一帧绘制完成的时间
	 */
	long lastFrameTime;
	/**
	 * 当前帧率
	 */
	int fps;
	/**
	 * 平均帧率
	 */
	int avgFps;
	/**
	 * 当前帧率是否较低
	 */
	bool slow;
	int totalFrameTime;
	/**
	 * 已经绘制的帧数
	 */
	int frame;
	/**
	 * 是否发生点击事件
	 */
	bool down;
	/**
	 * 地面水平偏移距离
	 */
	float offset;
	/**
	 * 地面水平移动速度
	 */
	float speed;
	/**
	 * 狐狸的y坐标
	 */
	float y;
	/**
	 * 地面垂直移动速度
	 */
	float vy;
	/**
	 * 跃起动作的持续时长
	 */
	int jump;
	/**
	 * 狐狸是否已经落地
	 */
	bool onground;
	/**
	 * 当前取得的分数
	 */
	int points;
	/**
	 * 游戏是否已经开始
	 */
	bool gameStarted;
	/**
	 * 游戏是否结束
	 */
	bool gameOver;
	/**
	 * 游戏分数记录
	 */
	int record;
	/**
	 * 旧的游戏分数记录
	 */
	int oldRecord;
	/**
	 * 是否正在启动加速
	 */
	bool wasBoosting;
	/**
	 * 是否正在加速中
	 */
	bool wasBooming;

	Terrain* _terrain;

	cocos2d::CCSprite* _sky;
	cocos2d::CCSprite* _mountain;
	cocos2d::CCLabelTTF* _pointsText;
	cocos2d::CCLabelTTF* _recordText;
	cocos2d::CCLabelTTF* _instText;
	Path* _path;
	cocos2d::CCSprite* _fox;
	cocos2d::CCSprite* _inst;
	Shadow* _shadow;
	Forest* _forest;
	Forest* _forest2;
	Forest* _fgFront;
	cocos2d::CCSprite* _title;
	cocos2d::CCLabelTTF* _failedTipText;
	cocos2d::CCLabelTTF* _newRecordText;
	cocos2d::CCLayerColor* _mask;
	std::list<std::string> _jumpMusic;
	std::list<std::string> _failMusic;
	std::list<std::string> _landingMusic;
	std::list<std::string> _boostsMusic;
	std::list<std::string> _boomMusic;
};

#endif // __HELLOWORLD_SCENE_H__
