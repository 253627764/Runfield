#ifndef __SHADOW_H__
#define __SHADOW_H__

#include "cocos2d.h"
#include "Terrain.h"

#define kMaxShadowVertice 1000 //路径顶点个数
#define kShadowSegment 4

class Shadow: public cocos2d::CCNode {
public:
	Shadow();
	~Shadow();

	virtual bool init();
	virtual void draw();

	CREATE_FUNC(Shadow)
	;

	CC_SYNTHESIZE_RETAIN(cocos2d::CCTexture2D*, _shadowTexture, ShadowTexture);
	void updateShadow(float offset, float yoff, int gap, Terrain* terrain);
private:
	cocos2d::CCPoint _shadowVertices[kMaxShadowVertice];
	cocos2d::CCPoint _shadowTexCoords[kMaxShadowVertice];
	int _nShadowVertices;
};

#endif
