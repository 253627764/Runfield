#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "cocos2d.h"
#include <vector>

#define GAP_UNDEFINED -1
#define GAP_FALSE 0
#define GAP_TRUE 1

struct TerrainFactor {
	float _phase;
	float _amplitude;
	float _waveLength;
};

struct TerrainPlatform {
	float _width;
	float _gap;
	float _offset;std::vector<TerrainFactor> _factors;
};

class Terrain {
public:
	Terrain();
	~Terrain();
	float fn(float x, int gap, bool straightGap);
	float forestY(float f, float t, int gap);
	void reset();
	float getDistance();

	std::vector<TerrainPlatform> getPlatforms();

private:
	std::vector<TerrainFactor> randomFactors();
	TerrainPlatform randomPlatform();

	/**
	 * 各个山坡的水平距离总和
	 */
	float _po;
	/**
	 * 山坡地形数组
	 */
	std::vector<TerrainPlatform> _platforms;
};

#endif
