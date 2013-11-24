#ifndef __FOREST_H__
#define __FOREST_H__

#include "cocos2d.h"
#include "Terrain.h"

#define kMaxForestVertice 6000
#define kForestSegment 4

class Forest: public cocos2d::CCNode {
public:
	Forest();
	~Forest();

	virtual bool init();
	virtual void draw();

	CREATE_FUNC(Forest)
	;CC_SYNTHESIZE_RETAIN(cocos2d::CCTexture2D*, _forestTexture, ForestTexture)
	;
	void updateForest(float factor, float offset, float yoff, int maxH, int gap, int segWidth,
			Terrain* terrain);
private:
	cocos2d::CCPoint _forestVertices[kMaxForestVertice];
	cocos2d::CCPoint _forestTexCoords[kMaxForestVertice];
	int _nForestVertices;

};

#endif
