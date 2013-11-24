#ifndef __PATH_H__
#define __PATH_H__

#include "cocos2d.h"
#include "Terrain.h"

#define kMaxPathVertice 6000	//路径顶点个数
#define kPathSegment 4	//每4个像素取一个点模拟曲线

class Path: public cocos2d::CCNode {
public:
	Path();
	~Path();

	virtual bool init();
	virtual void draw();

	CREATE_FUNC(Path)
	;

	CC_SYNTHESIZE_RETAIN(cocos2d::CCTexture2D*, _pathTexture, PathTexture);
	void updatePath(float offset, float yoff, int gap, int segWidth, Terrain* terrain);
private:
	cocos2d::CCPoint _pathVertices[kMaxPathVertice];
	cocos2d::CCPoint _pathTexCoords[kMaxPathVertice];
	int _nPathVertices;
};

#endif
