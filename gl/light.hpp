#pragma once

#include <cstring>
#include <GL/glew.h>

class Light {
public:
	float position[4] = {0, 0, 0, 1};
	float color[4] = {1, 1, 1, 1};
	
	void setPosition(const float p[4]) {
		memcpy(position, p, 4*sizeof(float));
	}
	void setColor(const float c[4]) {
		memcpy(color, c, 4*sizeof(float));
	}
	
	void apply(int n) {
		static const float black[4] = {0,0,0,1};
		GLenum lid = GL_LIGHT0 + n;
		glEnable(lid);
		glLightfv(lid, GL_POSITION, position);
		glLightfv(lid, GL_AMBIENT,  black);
		glLightfv(lid, GL_DIFFUSE,  color);
		glLightfv(lid, GL_SPECULAR, black);
	}
};
