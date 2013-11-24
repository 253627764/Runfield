#include "Path.h"

USING_NS_CC;

Path::Path(void) {
	_nPathVertices = 0;
}

Path::~Path(void) {
}

bool Path::init(void) {
	bool bRet = false;
	do {
		CC_BREAK_IF(!CCNode::init());
		this->setShaderProgram(
				CCShaderCache::sharedShaderCache()->programForKey(
						kCCShader_PositionTexture));
		bRet = true;
	} while (0);
	return bRet;
}

void Path::updatePath(float offset, float yoff, int gap, int segW,
		Terrain* terrain) {
	_nPathVertices = 0;
	int screenH = CCDirector::sharedDirector()->getWinSize().height;
	int screenW = CCDirector::sharedDirector()->getWinSize().width;
	float off = -(((int) floorf(offset)) % 512);
	const float h = _pathTexture->getContentSize().height;
	const float w = _pathTexture->getContentSize().width;

	for (int i = 0; i < 1536 + 256; i += segW) {
		float x = off + i;
		if (x >= screenW) {
			continue;
		}
		//		float y = floorf(terrain->fn(offset + off + i, gap, true) + yoff);
//		float y = floorf(terrain->fn(offset + off + i, gap, true) - yoff) + 200.f;
		float y = 512 - floorf(terrain->fn(offset + off + i, gap, true) - yoff);
		//		float sh = MAX(0, h - MAX(0, y + h - 512));
		float sw = MAX(0, MIN(w - (i % (int)w), segW));
		if (sw <= 0) {
			continue;
		}
		if (x + sw <= 0) {
			continue;
		}
		float sh = 0.f;
		if (y > 0) {
			if (y - h < screenH) {
				if (y < h) {
					sh = y;
				} else {
					sh = h;
				}
			} else {
				sh = 0;
				continue;
			}
		} else {
			sh = 0;
			continue;
		}

		_pathVertices[_nPathVertices] = ccp(x, y);
		_pathTexCoords[_nPathVertices++] = ccp(i / w, 0);
		_pathVertices[_nPathVertices] = ccp(x, y - sh);
		_pathTexCoords[_nPathVertices++] = ccp(i / w, sh / 512.f);
		_pathVertices[_nPathVertices] = ccp(x + sw, y -sh);
		_pathTexCoords[_nPathVertices++] = ccp((i + sw) / w, sh / 512.f);

		_pathVertices[_nPathVertices] = ccp(x + sw, y - sh);
		_pathTexCoords[_nPathVertices++] = ccp((i + sw) / w, sh / 512.f);
		_pathVertices[_nPathVertices] = ccp(x + sw, y);
		_pathTexCoords[_nPathVertices++] = ccp((i + sw) / w, 0);
		_pathVertices[_nPathVertices] = ccp(x, y);
		_pathTexCoords[_nPathVertices++] = ccp(i / w, 0);

		//		if (y + ih < 512) {
		//			for (var j = 0; j < 512 - y + ih; j += 32) {
		//				ctx.drawImage(img, (i % w), ih - 32, sw, 32, x, y + ih + j, sw,
		//						32);
		//			}
		//		}
	}
}

void Path::draw() {
	CCNode::draw();

	CC_NODE_DRAW_SETUP();

	ccGLBindTexture2D(_pathTexture->getName());
	ccGLEnableVertexAttribs(
			kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);

	ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0,
			_pathVertices);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0,
			_pathTexCoords);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) _nPathVertices);
}
