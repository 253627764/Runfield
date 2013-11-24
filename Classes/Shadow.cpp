#include "Shadow.h"

USING_NS_CC;

Shadow::Shadow(void) {
	_nShadowVertices = 0;
}

Shadow::~Shadow(void) {
}

bool Shadow::init(void) {
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

void Shadow::updateShadow(float offset, float yoff, int gap, Terrain* terrain) {
	_nShadowVertices = 0;

	float screenH = CCDirector::sharedDirector()->getWinSize().height;
	float w = _shadowTexture->getContentSize().width;
	float h = _shadowTexture->getContentSize().height;
	int segW = kShadowSegment;
	float sw = 0.f;
	float sh = 0.f;
	for (int i = 0; i < w; i += segW) {
		float x = i + 25;
		//var y = 264+Math.floor(yFunc(offset+x, gap)-yoff);
		float y = 512 - (264 + floorf(
				terrain->fn(offset + x, gap, false) - yoff));
		if (y < 0 || y > screenH + h) {
			continue;
		}
		sw = MIN(w - i, segW);
		sh = h;

		_shadowVertices[_nShadowVertices] = ccp(x, y);
		_shadowTexCoords[_nShadowVertices++] = ccp(i / w, 0);
		_shadowVertices[_nShadowVertices] = ccp(x, y - sh);
		_shadowTexCoords[_nShadowVertices++] = ccp(i / w, sh / h);
		_shadowVertices[_nShadowVertices] = ccp(x + sw, y -sh);
		_shadowTexCoords[_nShadowVertices++] = ccp((i + sw) / w, sh / h);

		_shadowVertices[_nShadowVertices] = ccp(x + sw, y - sh);
		_shadowTexCoords[_nShadowVertices++] = ccp((i + sw) / w, sh / h);
		_shadowVertices[_nShadowVertices] = ccp(x + sw, y);
		_shadowTexCoords[_nShadowVertices++] = ccp((i + sw) / w, 0);
		_shadowVertices[_nShadowVertices] = ccp(x, y);
		_shadowTexCoords[_nShadowVertices++] = ccp(i / w, 0);
	}
}

void Shadow::draw() {
	CCNode::draw();

	CC_NODE_DRAW_SETUP();

	ccGLBindTexture2D(_shadowTexture->getName());
	ccGLEnableVertexAttribs(
			kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);

	ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0,
			_shadowVertices);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0,
			_shadowTexCoords);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) _nShadowVertices);
}
