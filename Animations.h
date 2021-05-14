#pragma once
#define NUMZOMBIEWALKKEYFRAMES  14
#define NUMZOMBIEIDLEKEYFRAMES  12
#define NUMZOMBIEATTACKKEYFRAMES  12
#define NUMZOMBIECHASEKEYFRAMES  12
#define NUMZOMBIES 40
#define NUMPLAYERWALKEYFRAMES   12
#define NUMPLAYERIDLEKEYFRAMES 12
#define NUMZOMBIEATTACKKEYFRAMES 14
#define NUMPLAYERDEATHKEYFRAMES 32
#define NUMZOMBIEDANCE2KEYFRAMES 24
#define NUMPLAYERATTACKKEYFRAMES 20
#define NUMZOMBIEDEATHKEYFRAMES 18
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#include <GL/glu.h>
#include "glut.h"



class AnimationDisplayLists {

 public:
	AnimationDisplayLists() {}
	GLuint ZombieWalkKeyFrames[NUMZOMBIEWALKKEYFRAMES];
	GLuint ZombieChaseKeyFrames[NUMZOMBIECHASEKEYFRAMES];
	GLuint ZombieIdleKeyFrames[NUMZOMBIEIDLEKEYFRAMES];
	GLuint ZombieAttackKeyFrames[NUMZOMBIEATTACKKEYFRAMES];
	GLuint PlayerWalkKeyFrames[NUMPLAYERWALKEYFRAMES];
	GLuint PlayerIdleKeyFrames[NUMPLAYERIDLEKEYFRAMES];
	GLuint PlayerDeathKeyFrames[NUMPLAYERDEATHKEYFRAMES];
	GLuint ZombieDance2KeyFrames[NUMZOMBIEDANCE2KEYFRAMES];
	GLuint PlayerAttackKeyFrames[NUMPLAYERATTACKKEYFRAMES];
	GLuint ZombieDeathKeyFrames[NUMZOMBIEDEATHKEYFRAMES];
};

