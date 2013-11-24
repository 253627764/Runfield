#include "Terrain.h"

USING_NS_CC;

Terrain::Terrain() :
	_platforms() {
}

Terrain::~Terrain() {
}

float Terrain::forestY(float f, float t, int gap) {
	float off = (t - 256) / f + 256;
	//	return fn(off, false, straightGap) / f + 100 * f - 50;
//	return 50 - 100 * f + fn(off, false, gap == false) * f;
	return 50 - 100 * f + fn(off, false, gap == GAP_FALSE) * f;
}

float Terrain::fn(float x, int gap, bool straightGap) {
	x = ((int) x) % ((int) _po);
	TerrainPlatform* p = NULL;
	int i = _platforms.size() - 1;
	int min = 0;
	int max = _platforms.size() - 1;
	while (min <= max) {
		int mid = min + floor((max - min) / 2);
		p = &_platforms[mid];
		if (p->_offset <= x && p->_offset + p->_width + p->_gap > x) {
			i = mid;
			break;
		} else if (p->_offset <= x) {
			min = mid + 1;
		} else {
			max = mid - 1;
		}
	}
	x = x - p->_offset;
	float y = 110.f;
	//	float y = 408.f;
	for (int j = 0; j < p->_factors.size(); ++j) {
		TerrainFactor f = p->_factors[j];
		y += sinf(f._phase + x / f._waveLength) * f._amplitude;
		//		y -= sinf(f._phase + x / f._waveLength) * f._amplitude;
	}
	float d = x - p->_width;
	if (d > 0) {
		if (gap != GAP_FALSE) {
			return 9999.f;
		}
		TerrainPlatform* next = NULL;
		if (i + 1 < _platforms.size()) {
			next = &_platforms[i + 1];
		} else {
			next = &_platforms[0];
		}
		float nextY = 110.f;
		//		float nextY = 408.f;
		float dp = d - p->_gap;
		for (int j = 0; j < next->_factors.size(); ++j) {
			TerrainFactor* f = &next->_factors[j];
			nextY += sinf(f->_phase + dp / f->_waveLength) * f->_amplitude;
			//			nextY -= sinf(f->_phase + dp / f->_waveLength) * f->_amplitude;
		}
		float f = d / p->_gap;
		float df = straightGap ? 0 : 1 - (cosf(f * 2 * M_PI) * 0.5f + 0.5f);
		y = (1 - f) * y + f * nextY + df * 96;
	} else if (gap == GAP_TRUE) {
		float xd = 0;
		float taper = 64;
		if (x < taper) {
			xd = 1 - (x / taper);
		} else if (-d < taper) {
			xd = 1 - (-d / taper);
		}
		y += xd * xd * xd * 48;
		//		y -= xd * xd * xd * 48;
	}
	return y;
}

void Terrain::reset() {
	_po = 0.f;
	_platforms.clear();
	std::vector<TerrainFactor> factors;
	TerrainFactor factor = { -1, 100, (float) (5000 / (M_PI + 1.5f)) };
	factors.push_back(factor);
	TerrainPlatform p = { 5000, 200, 0, factors };
	_platforms.push_back(p);
	_po += p._width + p._gap;
	for (int i = 0; i < 100; ++i) {
		TerrainPlatform p = randomPlatform();
		bool last = false;
		if (_po > 35000) {
			last = true;
		}
		p._offset = _po;
		_po += p._width + p._gap;
		_platforms.push_back(p);
		if (last) {
			break;
		}
	}
}

std::vector<TerrainFactor> Terrain::randomFactors() {
	std::vector<TerrainFactor> factorVec;
	for (int i = 1; i < 5; ++i) {
		float p = powf(5.f, i);
		TerrainFactor factor = { (float) ((rand() % 11) / 10.f * M_PI * 2),
				(float) (i * 11), 200 + p + (rand() % 11) / 10.f * p };
		factorVec.push_back(factor);
	}
	return factorVec;
}

TerrainPlatform Terrain::randomPlatform() {
	TerrainPlatform platform = { 800 + 256 * ((rand() % 11) / 10.f), 300, 0.f,
			randomFactors() };
	return platform;
}

float Terrain::getDistance() {
	return _po;
}

std::vector<TerrainPlatform> Terrain::getPlatforms() {
	return _platforms;
}
