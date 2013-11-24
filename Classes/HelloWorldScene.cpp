#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

HelloWorld::HelloWorld() :
	locations(), _jumpMusic(), _failMusic(), _landingMusic(), _boostsMusic(),
			_boomMusic() {
	offset = 1000.f;
	speed = 1.5f;
	y = 0.f;
	vy = 0.f;
	jump = 0;
	onground = false;
	points = 0;
	gameStarted = false;
	gameOver = false;
	record = CCUserDefault::sharedUserDefault()->getIntegerForKey("record", 0);
	oldRecord = record;
	wasBoosting = false;
	wasBooming = false;

	lastFrameTime = getCurTimeInMilliSeconds();
	fps = 0;
	avgFps = 0;
	slow = false;
	totalFrameTime = 0;
	frame = 0;
	down = false;

	animFrame = 0;
	animTime = 0;
	lastAnimTime = 0;
	lastAnimFrameTime = getCurTimeInMilliSeconds();
	lastY = 0.f;
	lastYoff = 0.f;
	gameOverTime = 0;
	titleScreenTime = 0;

	_terrain = new Terrain();

	_jumpMusic.push_back("hyppy.ogg.mp3");
	_jumpMusic.push_back("hyppy2.ogg.mp3");
	_jumpMusic.push_back("hyppy3.ogg.mp3");
	_landingMusic.push_back("lasku2.ogg.mp3");
	_failMusic.push_back("delaa.ogg.mp3");
	_boostsMusic.push_back("turbo3_2.ogg.mp3");
	_boomMusic.push_back("snoicboom.ogg.mp3");
}

long HelloWorld::getCurTimeInMilliSeconds() {
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	return now.tv_sec * 1000 + now.tv_usec / 1000;
}

HelloWorld::~HelloWorld() {
}

CCScene* HelloWorld::scene() {
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();
	layer->setTouchEnabled(true);
	layer->setKeypadEnabled(true);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}

	//	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//
	//	/////////////////////////////
	//	// 2. add a menu item with "X" image, which is clicked to quit the program
	//	//    you may modify it.
	//
	//	// add a "close" icon to exit the progress. it's an autorelease object
	//	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png",
	//			"CloseSelected.png", this,
	//			menu_selector(HelloWorld::menuCloseCallback));
	//
	//	pCloseItem->setPosition(
	//			ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
	//					origin.y + pCloseItem->getContentSize().height/2));
	//
	//	// create menu, it's an autorelease object
	//	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	//	pMenu->setPosition(CCPointZero);
	//	this->addChild(pMenu, 1);
	//
	//	/////////////////////////////
	//	// 3. add your codes below...
	//
	//	// add a label shows "Hello World"
	//	// create and initialize a label
	//
	//	CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
	//
	//	// position the label on the center of the screen
	//	pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
	//			origin.y + visibleSize.height - pLabel->getContentSize().height));
	//
	//	// add the label as a child to this layer
	//	this->addChild(pLabel, 1);
	//
	//	// add "HelloWorld" splash screen"
	//	CCSprite* pSprite = CCSprite::create("HelloWorld.png");
	//
	//	// position the sprite on the center of the screen
	//	pSprite->setPosition(
	//			ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	//
	//	// add the sprite as a child to this layer
	//	this->addChild(pSprite, 0);

	loadTextures();
	reset();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	ccTexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE };

	CCTexture2D* skyTexture =
			CCTextureCache::sharedTextureCache()->textureForKey("sky.png");
	skyTexture->setTexParameters(&tp);
	_sky = CCSprite::createWithTexture(skyTexture,
			CCRectMake(0, 0, winSize.width, 512));
	_sky->setPosition(ccp(winSize.width / 2, winSize.height - 256));
	this->addChild(_sky, 0);

	CCTexture2D* mountainTexture =
			CCTextureCache::sharedTextureCache()->textureForKey("horizon.png");
	mountainTexture->setTexParameters(&tp);
	_mountain = CCSprite::createWithTexture(mountainTexture,
			CCRectMake(0, 0, winSize.width, 512));
	_mountain->setPosition(ccp(winSize.width / 2, 0));
	this->addChild(_mountain, 0);

	CCTexture2D* forestTexture =
			CCTextureCache::sharedTextureCache()->textureForKey(
					"background.png");
	forestTexture->setTexParameters(&tp);
	_forest2 = Forest::create();
	_forest2->setForestTexture(forestTexture);
	this->addChild(_forest2, 0);

	_forest = Forest::create();
	_forest->setForestTexture(forestTexture);
	this->addChild(_forest, 0);

	CCTexture2D* pathTexture =
			CCTextureCache::sharedTextureCache()->textureForKey("path.png");
	pathTexture->setTexParameters(&tp);
	_path = Path::create();
	_path->setPathTexture(pathTexture);
	this->addChild(_path, 0);

	//	CCLOG("begin add shadow");
	//	CCTexture2D* shadowTexture =
	//			CCTextureCache::sharedTextureCache()->textureForKey("shadow.png");
	//	_shadow = Shadow::create();
	//	_shadow->setShadowTexture(shadowTexture);
	//	this->addChild(_shadow, 1);
	//	CCLOG("end add shadow");

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(
			"run_frames.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(
			"boost_frames.plist");
	_fox = CCSprite::createWithSpriteFrame(
			CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
					"fox_1_1.png"));
	_fox->setPosition(ccp(-1000, -1000));
	this->addChild(_fox, 1);

	_inst = CCSprite::createWithTexture(
			CCTextureCache::sharedTextureCache()->textureForKey(
					"instructions.png"));
	_inst->setPosition(ccp(-1000, -1000));
	this->addChild(_inst, 1);

	CCTexture2D* foregrounTexture =
			CCTextureCache::sharedTextureCache()->textureForKey(
					"foreground.png");
	foregrounTexture->setTexParameters(&tp);
	_fgFront = Forest::create();
	_fgFront->setForestTexture(foregrounTexture);
	this->addChild(_fgFront, 1);

	_pointsText = CCLabelTTF::create("", "Arial", 48);
	_pointsText->setPosition(ccp(-1000, 0));
	_pointsText->setColor(ccc3(81, 65, 30));
	this->addChild(_pointsText, 2);

	_recordText = CCLabelTTF::create("", "Arial", 48);
	_recordText->setPosition(ccp(-1000, 0));
	this->addChild(_recordText, 2);

	_newRecordText = CCLabelTTF::create("", "Arial", 48);
	_newRecordText->setPosition(ccp(-1000, 0));
	this->addChild(_newRecordText, 2);

	_failedTipText = CCLabelTTF::create("", "Arial", 48);
	_failedTipText->setPosition(ccp(-1000, 0));
	this->addChild(_failedTipText, 2);

	_instText = CCLabelTTF::create("", "Arial", 60);
	_instText->setPosition(ccp(-1000, 0));
	this->addChild(_instText, 2);

	CCTexture2D* titleTexture =
			CCTextureCache::sharedTextureCache()->textureForKey("Runfield.png");
	_title = CCSprite::createWithTexture(titleTexture);
	_title->setPosition(ccp(-1000, 0));
	this->addChild(_title, 3);

	_mask = CCLayerColor::create(ccc4(0, 0, 0, 255), winSize.width,
			winSize.height);
	_mask->setPosition(ccp(0, 0));
	this->addChild(_mask, 4);

	this->schedule(schedule_selector(HelloWorld::tick));

	return true;
}

void HelloWorld::onEnter() {
	CCLayer::onEnter();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(
			0.f);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
			"musa.ogg.mp3", true);
}

void HelloWorld::onExit() {
	CCLayer::onExit();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
}

void HelloWorld::loadTextures() {
	CCTextureCache::sharedTextureCache()->addImage("Runfield.png");

	CCTextureCache::sharedTextureCache()->addImage("sky.png");

	CCTextureCache::sharedTextureCache()->addImage("background.png");
	CCTextureCache::sharedTextureCache()->addImage("boost_frames.png");

	CCTextureCache::sharedTextureCache()->addImage("foreground.png");
	CCTextureCache::sharedTextureCache()->addImage("horizon.png");

	CCTextureCache::sharedTextureCache()->addImage("path.png");
	CCTextureCache::sharedTextureCache()->addImage("run_frames.png");

	CCTextureCache::sharedTextureCache()->addImage("shadow.png");
	CCTextureCache::sharedTextureCache()->addImage("instructions.png");
}

void HelloWorld::tick(float dt) {
	float screenW = CCDirector::sharedDirector()->getWinSize().width;
	float screenH = CCDirector::sharedDirector()->getWinSize().height;
	long st = getCurTimeInMilliSeconds();
	animTime += st - lastAnimFrameTime;
	long animElapsed = st - lastAnimFrameTime;
	lastAnimFrameTime = st;
	float timeF = (animElapsed / 16.f);
	long t = 0;
	long elapsed = 0;
	long drawTime = 0;
	if (!gameStarted) {
		if (avgFps > 0) {
			slow = (slow && avgFps < 50) || (!slow && avgFps < 25); //当前帧率是否较低
		}
		float ground = (_terrain->fn(offset + 512, GAP_FALSE, true) + 40);
		float yoff = ground - 160 - (512 - 480);
		if (ground > 400) {
			yoff = lastYoff;
		}
		yoff = lastYoff + (yoff - lastYoff) * 0.05f;
		lastYoff = yoff;
		updateLevel(st, offset, yoff, GAP_FALSE);
		float sb = (sinf(st / 150.f) + 1) * 0.5f;
		float sb4 = powf(sb, 4.f);
		if (sb4 < 1 / 256.f) {
			sb4 = 0.f;
		} else if (sb4 > 1.f) {
			sb4 = 1.f;
		}
		if (gameOver) {
			if (gameOverTime == 0) {
				gameOverTime = st;
			}
			if (st - gameOverTime > 30000) {
				gameOver = false;
				titleScreenTime = st - 1000;
			}
			if (points > record) {
				record = points;
				CCUserDefault::sharedUserDefault()->setIntegerForKey("record",
						points);
			}
			gameStarted = false;
			//drawBg(ctx, fg_front.getImage(lowres), offset, 1.15, yoff-30, fn, null, false);
			_fgFront->updateForest(1.15f, offset, yoff - 30, 0, GAP_FALSE, 32,
					_terrain);
			float a = MAX(0, MIN(1, (29000 - (st - gameOverTime)) / 1000.f));
			a = 0.5f - cosf(a * M_PI) * 0.5f;
			float b = MAX(0, MIN(1, (30000 - (st - gameOverTime)) / 1000.f));
			b = 0.5f - cosf(b * M_PI) * 0.5f;
			char text[50];
			sprintf(text, "Points: %d", points);
			_pointsText->setColor(ccc3(255, 255, 255));
			_pointsText->setOpacity((GLubyte)(a * 255));
			_pointsText->setString(text);
			_pointsText->setPosition(ccp(200, screenH - 45));
			memset(text, 0, 50);
			sprintf(text, "Record: %d", record);
			_recordText->setString(text);
			_recordText->setColor(ccc3(255, 255, 255));
			_recordText->setOpacity((GLubyte)(a * 255));
			_recordText->setPosition(ccp(screenW - 200, screenH - 45));
			char overText[] = { "You fell in a ditch" };
			_failedTipText->setString(overText);
			_failedTipText->setColor(ccc3(255, 255, 255));
			_failedTipText->setOpacity((GLubyte)(a * 255));
			_failedTipText->setPosition(ccp(screenW * 0.5f, screenH - 240));
			char tip[] = "Click to Retry";
			_instText->setString(tip);
			_instText->setOpacity((GLubyte)(a * 255));
			_instText->setColor(ccc3(255, 255, 255));
			_instText->setPosition(ccp(screenW * 0.5f, screenH - 420));

			if (points > oldRecord) {
				_newRecordText->setColor(ccc3(255, 0, 0));
				_newRecordText->setOpacity((GLubyte)((1 - sb4) * a * 255));
				char newRecordText[] = "New Record!";
				_newRecordText->setString(newRecordText);
				_newRecordText->setPosition(ccp(screenW * 0.5f, screenH - 288));
			}
		} else {
			if (titleScreenTime == 0) {
				titleScreenTime = st;
			}
			speed = 0.2f;
			float titleY = 120.f;
			float titleA = 1.f;
			long e = st - titleScreenTime;
			if (e < 2500) {
				titleA = 0.f;
				if (e > 1000) {
					float toff = MIN(1, (e - 1000) / 1000.f);
					toff = 0.5f - cosf(toff * M_PI) * 0.5f;
					titleY = titleY + (1 - toff) * 30;
					titleA = toff; //全局透明度Alpha
				}
			}
			//			ctx.globalAlpha = titleA;
			//			if (titleA)
			//				ctx.drawImage(title.getImage(lowres),
			//						512 - title.getImage().width / 2, titleY);
			_title->setOpacity((short) (titleA * 255));
			if (titleA != 0) {
				_title->setPosition(
						ccp(screenW * 0.5f,
								screenH - titleY - _title->getContentSize().height * 0.5f));
				CCLog("title pos x = %f, y = %f", _title->getPosition().x,
						_title->getPosition().y);
			}
			//			ctx.globalAlpha = 1;
			//			drawBg(ctx, fg_front.getImage(lowres), offset, 1.15, yoff - 30, fn,
			//					null, false);
			_fgFront->updateForest(1.15f, offset, yoff - 30, 0, GAP_FALSE, 32,
					_terrain);
			if (e >= 2500) {
				float toff = MIN(1, (e - 2500) / 500.f);
				toff = 0.5f - cosf(toff * M_PI) * 0.5f;
				float s = (sinf(M_PI_2 + (e - 2500) / 300) + 1) * 0.5f;
				float s4 = pow(s, 4);
				if (s4 < (1 / 256.f)) {
					s4 = 0.f;
				} else if (s4 > 1.f) {
					s4 = 1.f;
				}
				char tip[] = "Click to Play";
				_instText->setPosition(
						ccp(0.5f * screenW, screenH - (420 - 10 * (1 - toff))));
				_instText->setString(tip);
			}
		}
		if (frame <= 90) {
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(
					frame / 90.f);
			_mask->setColor(ccc3(0, 0, 0));
			_mask->setOpacity(
					(GLubyte)((1 - MAX(0.f, (frame - 30) / 60.F)) * 255));
			_mask->setPosition(ccp(0, 0));
		}
		t = getCurTimeInMilliSeconds();
		long drawTime = t - st;
		elapsed = t - lastFrameTime;
		offset += elapsed * speed;
		speed = speed + (0.2f - speed) * 0.02f;
		//		CCLog("GAME NOT STARTED speed = %f", speed);
	} else {
		this->removeChild(_title);
		const int maxHeight = 512;
		float ground = 40 + _terrain->fn(offset + 50 + 116, GAP_UNDEFINED,
				false);
		if (y > maxHeight) { //狐狸掉进坑里
			//		if (y < 0) {
			//画面蒙上一层灰色
			//			ctx.fillStyle = 'rgba(255,255,255,' + ((y - 512) / 1024) + ')';
			//			ctx.fillRect(0, 0, 1024, 512);

			_mask->setColor(ccc3(0, 0, 0));
			_mask->setOpacity((GLubyte)((y - 512) / 1024.f * 255));
			_mask->setPosition(ccp(0, 0));
		}
		if (y > maxHeight + maxHeight) {
			//游戏结束
			//			setSfxVolume(0.4);
			//			playSfx(sfx.fall);
			playMusic(0.4f, false, _failMusic);
			gameOver = true;
			gameOverTime = st;
			gameStarted = false;
		} else {
			bool landing = false;
			//			CCLog("ground = %f, y = %f", ground, y);
			if (y - ground < 30 && ground - y < 10 && vy > 0) {
				//			if (ground - y < 30 && y - ground < 10 && vy > 0) {
				y = ground;
				landing = landing || vy > 10;
				vy = 0; //着陆后狐狸下落速度清零，防止穿透地面
			}
			bool wasAbove = y < ground;
			//			bool wasAbove = y > ground; //狐狸在地面上
			if (jump > 0) {
				//点击事件使狐狸往上跳
				vy = -jump;
				jump -= timeF;
			}
			y += vy * timeF;
			vy += timeF * speed;
			bool isBelow = y >= ground;
			if ((y - ground < 50 && ground - y < 10 && jump <= 0) || (isBelow
					&& wasAbove) || (onground && jump <= 0 && ground < 1024)) {
				y = ground;
				landing = landing || vy > 10;
				vy = 0;
			}
			if (landing) {
				playMusic(1.0f, false, _landingMusic);
			}
			onground = abs(y - ground) < 30 && jump <= 0;
		}
		lastY = y;
		float boostOff = ceilf(60000.f / _terrain->getDistance())
				* _terrain->getDistance();
		float boomOff = ceilf(90000.f / _terrain->getDistance())
				* _terrain->getDistance();
		bool trailers = offset > boostOff + 800;
		bool boosting = offset > boostOff + 200;
		bool booming = offset > boomOff + 200;
		if (!wasBoosting && boosting) {
			playMusic(1.0f, false, _boostsMusic);
		}
		if (!wasBooming && booming && y <= 512 + 512) {
			//					stopAllSfx( sfx);
			//					playSfx(sfx.boom);
			playMusic(1.f, false, _boomMusic);
		}
		//		if (booming && sfx.boom[0].currentTime > 3 && y <= 512+512) {
		//		                setSfxVolume(0);
		//		              }
		wasBoosting = boosting;
		wasBooming = booming;
		float xoff = boosting ? 1 : 0.2f;
		float af = boosting ? 1 : 0.2f;
		long animFrameDuration = boosting ? 160 : 160;
		long framesElapsed = (animTime - lastAnimTime) / (animFrameDuration
				* af / speed);
		if (framesElapsed >= 1) {
			lastAnimTime = animTime;
			animFrame += floorl(framesElapsed);
		}
		int fr = onground ? animFrame % ((int) (4 / af)) : 0;
		int f = (int) ((fr / (1 / af)));
		int hf = (int) (fr % ((int) (1 / af)));
		float yoff = y - 160 - (512 - 480);
		yoff = lastYoff + (yoff - lastYoff) * 0.2f;
		yoff = MIN(MAX(yoff, -50), 50);
		lastYoff = yoff;
		updateLevel(st, offset, yoff, GAP_TRUE);
		points = floorf(offset - 1000);
		float lap = floorf(MAX(0, (offset-1500)/ _terrain->getDistance())) + 1;
		char text[50];
		sprintf(text, "Points: %d", points);
		_pointsText->setString(text);
		_pointsText->setColor(ccc3(81, 65, 30));
		_pointsText->setOpacity(255);
		_pointsText->setPosition(ccp(200, screenH - 45));
		memset(text, 0, 50);
		sprintf(text, "Record: %d", record);
		_recordText->setString(text);
		_recordText->setColor(ccc3(81, 65, 30));
		_recordText->setOpacity(255);
		_recordText->setPosition(ccp(screenW - 200, screenH - 45));
		if (y <= ground) {
			//绘制狐狸的投影
			//阴影图片有问题
			//			_shadow->updateShadow(offset, yoff, GAP_UNDEFINED, _terrain);
		}
		//绘制狐狸
		if (boosting) {
			char boostFoxFrameName[20] = { 0 };
			sprintf(boostFoxFrameName, "boost_frames_%d.png", (f + 1));
			_fox->setDisplayFrame(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							boostFoxFrameName));
		} else {
			char foxFrameName[10] = { 0 };
			sprintf(foxFrameName, "fox_%d_%d.png", (hf + 1), (f + 1));
			_fox->setDisplayFrame(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
							foxFrameName));
		}
		_fox->setPosition(
				ccp(xoff + 192 * 0.5f, 512 - (y - yoff + 80) - 160 * 0.5f));
		if (locations.size() == 20) {
			for (int k = 0; k < 10; ++k) {
				locations.pop_front();
			}
		}
		locations.push_back(ccp(offset, y + (f % 2) * 5));
		//		if (trailers) {
		//			ctx.strokeStyle = trailergradient;
		//			drawTrail(ctx, locations, 180, 74, 5, offset, xoff, yoff);
		//			drawTrail(ctx, locations, 197, 46, 3, offset, xoff, yoff);
		//			drawTrail(ctx, locations, 205, 56, 2, offset, xoff, yoff);
		//		}
		if (((int) offset) % ((int) _terrain->getDistance()) < 1500) {
			// draw starting line
			//ctx.fillStyle = 'rgba(255,255,255,0.5)';
			//ctx.fillRect(1500 - (offset % po) - 1, 0, 3, 512);
		}
		_fgFront->updateForest(1.15f, offset, yoff - 30, 0, GAP_UNDEFINED, 32,
				_terrain);
		if (offset < 6000) {
			float a = MAX(0, offset - 5000) / 1000.F;
			a = 0.5f - 0.5f * cosf(a * M_PI);
			_inst->setOpacity((GLubyte)((1 - a) * 255));
			_inst->setPosition(
					ccp(screenW * 0.5f ,
							screenH - (512 - _inst->getContentSize().height - 120) - 0.5f * _inst->getContentSize().height));
			char tip[] = "Click to jump over ditches";
			_instText->setString(tip);
			_instText->setOpacity((GLubyte)((1 - a) * 255));
			_instText->setColor(ccc3(255, 255, 255));
			_instText->setPosition(ccp(screenW * 0.5f, screenH - 420));
		}
		t = getCurTimeInMilliSeconds();
		drawTime = t - st;
		elapsed = t - lastFrameTime;
		if (speed < 1.f) {
			speed += elapsed / 30000.f;
		} else if (speed < 1.25f) {
			speed += elapsed / 50000.f;
		} else if (speed < 1.5f) {
			speed += elapsed / 60000.f;
		} else if (((int) offset) % ((int) _terrain->getDistance()) < 6000) {
			speed += elapsed / 7000.f;
		} else {
			speed += elapsed / 70000.f;
		}
		offset += elapsed * speed;
	}
	lastFrameTime = t;
	totalFrameTime += elapsed;
	if (frame % 30 == 0 || frame == 11) {
		fps = (int) (1000 / elapsed);
		if (frame == 11) {
			totalFrameTime *= 3;
		} else if (frame == 0) {
			totalFrameTime *= 30;
		}
		avgFps = 1000 / (totalFrameTime / 30);
		totalFrameTime = 0;
	}
	frame++;
	//	if (booming && musicOn) {
	//		music.volume = 0;
	//	} else if (!music.paused) {
	//		if (musicOn)
	//			music.volume = Math.min(
	//					(0.01 + music.volume) * (1.0 + elapsed / 1000), 1);
	//		else
	//			music.volume = Math.max(music.volume * (1.0 - elapsed / 200), 0);
	//		if (music.volume < 0.01)
	//			music.volume = 0;
	//		if (!musicOn && music.volume == 0)
	//			music.pause();
	//	}
	//	if (musicOn && (music.paused || music.currentTime == music.duration
	//			|| music.currentTime == 0))
	//		music.play();
	//	if (window.requestAnimationFrame) {
	//		window.requestAnimationFrame(draw);
	//	} else if (window.mozRequestAnimationFrame) {
	//		window.mozRequestAnimationFrame(draw);
	//	} else if (window.webkitRequestAnimationFrame) {
	//		window.webkitRequestAnimationFrame(draw);
	//	} else {
	//		setTimeout(draw, Math.floor(15 - (t % 15)));
	//	}
}

void HelloWorld::updateLevel(long st, float offset, float yoff, int gap) {
	updateSky(st, yoff);
	if (!slow) {
		updateMountain(offset, yoff);
		_forest2->updateForest(0.5f, offset, yoff, 400, gap, 32, _terrain);
	}
	_forest->updateForest(0.75f, offset, yoff, 0, GAP_UNDEFINED, 32, _terrain);
	_path->updatePath(offset, yoff, gap, slow ? 16 : 4, _terrain);
}

void HelloWorld::updateSky(long st, float yoff) {
	const float f = 0.001f;
	CCSize textureSize = _sky->getTextureRect().size;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float x = offset * f;
	float y = winSize.height + yoff * f - 256;
	_sky->setPosition(ccp(_sky->getPosition().x, y));
	_sky->setTextureRect(
			CCRectMake(x, 0, textureSize.width, textureSize.height));
}

void HelloWorld::updateMountain(float offset, float yoff) {
	const float f = 0.01f;
	CCSize textureSize = _mountain->getTextureRect().size;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float x = offset * f;
	float y = winSize.height + yoff * f - 256;
	_mountain->setPosition(ccp(_mountain->getPosition().x, y));
	_mountain->setTextureRect(
			CCRectMake(x, 0, textureSize.width, textureSize.height));
}

void HelloWorld::reset() {
	if (points > record) {
		record = points;
		CCUserDefault::sharedUserDefault()->setIntegerForKey("record", points);
	}
	oldRecord = record;
	points = 0;
	offset = 1000.f;
	speed = 0.7f;
	y = 0.f;
	//	y = CCDirector::sharedDirector()->getWinSize().height;
	vy = 0.f;
	jump = 0;
	onground = false;
	wasBoosting = false;

	_terrain->reset();
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* pTouches,
		cocos2d::CCEvent* pEvent) {
	if (!gameStarted) {
		gameOver = false;
		gameStarted = true;
		reset();
		_failedTipText->setPosition(ccp(-1000, 0));
		_newRecordText->setPosition(ccp(-1000, 0));
		_mask->setPosition(ccp(-10000, -1000));
	} else if (onground && !down) {
		jump = 20;
		onground = false;
		down = true;
		playMusic(0.4f, false, _jumpMusic);
	}
}

void HelloWorld::playMusic(float volume, bool isLooped,
		std::list<std::string> musicList) {
	std::string music = musicList.front();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(music.c_str(),
			isLooped);
	musicList.pop_front();
	musicList.push_back(music);
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet* pTouches,
		cocos2d::CCEvent* pEvent) {
	if (down) {
		//		jump = 0;
		down = false;
	}
}
void HelloWorld::ccTouchesCancelled(cocos2d::CCSet* pTouches,
		cocos2d::CCEvent* pEvent) {
	if (down) {
		//		jump = 0;
		down = false;
	}
}

void HelloWorld::keyBackClicked() {
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::menuCloseCallback(CCObject* pSender) {
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
