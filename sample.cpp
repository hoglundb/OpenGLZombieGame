#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include<iostream>
#include<string>
#include<math.h>
#include"Zombie.h"
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

#include "Particle.cpp"
#include "Random.h"
#include <string.h>
#include <ctype.h>
#include <vector>
#include"Keyframe.h"
#include "Player.h"
#include "Animations.h"
#include "LightingFunctions.h"
#include "ObjLoaderFunctions.h"
#include "AnimationFunctions.h"
#include "Geometries.h"
#include "KeyboardFunctions.h"
#include "Camera.h"

const char *WINDOWTITLE = { "OpenGL / Final Project:  Brian Hoglund" };
const char *GLUITITLE   = { "User Interface Window" };

// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};

char * ColorNames[ ] =
{
	"Red",
	"Yellow",
	"Green",
	"Cyan",
	"Blue",
	"Magenta",
	"White",
	"Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to use the z-buffer
GLuint	BoxList;				// object display list
GLuint  GroundList;
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
float White[] = { 1.,1.,1.,1. };
//Animation globals 


int CurPlayerIdleKeyFrame = 0;
int CurZombieWalkKeyFrame = 0;
int CurPlayerWalkKeyFrame = 0;
bool   PlayAnimation = true;
int NumAliveZombies;
int IsGameOver = false;
Zombie *ZombieGameObjects;
Player *PlayerObject;
AnimationDisplayLists * HumaniodAnimations;
int CurrentRound = 1;


float  Time = 0.0f;


//tracks the state of the keyboard input every frame
struct KeyboardStateObject {
		bool wKey = false;
		bool sKey = false;
		bool aKey = false;
		bool dKey = false;
};



KeyboardStateObject KeyboardState;

// function prototypes:
void    KeyboardRelease(unsigned char c, int x, int y);
void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );
void DrawScene();
void InitLighting();
void InitGameObjects();
void KeyboardRelease(unsigned char c, int x, int y);
void InitZombieObjects();
void SetCameraPosition();
void DoStartMenu(int);
void RestartGame(bool resetRound);


// main program:

int main( int argc, char *argv[ ] )
{

	glutInit(&argc, argv);

	//initalize game objects
	InitGameObjects();


	// setup all the graphics stuff:
	InitGraphics();
	InitLighting();
	
	// create the display structures that will not change:

	InitLists();


	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset();


	// setup all the user interface stuff:

	InitMenus();


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow(MainWindow);
	glutMainLoop();
	return 0;
}


float * Array3(float a, float b, float c)
{
	static float array[4];
	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}


void KeyboardRelease(unsigned char c, int x, int y) {
	KeyboardReleaseHandler(c, x, y, PlayerObject, KeyboardState.aKey, KeyboardState.dKey, KeyboardState.sKey, KeyboardState.wKey);
}

void DisplayText(float x, float y, float z, char* s) {


	glPushMatrix();
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
	float sf = .6 / (119.05f + 33.33f);
	glScalef((GLfloat)sf, (GLfloat)sf, (GLfloat)sf);
	glRotatef(-90.0f,  0, 1, 0);
	char c;			// one character to print
	for (; (c = *s) != '\0'; s++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
	}
	glPopMatrix();
}

void InitZombieObjects() {
	NumAliveZombies = 3;
	ZombieGameObjects = new Zombie[NUMZOMBIES];

	//the zombies that will initailly be alive 
	for (int i = 0; i < NumAliveZombies; i++) {
		ZombieGameObjects[i].ZombieStatus = ZombieAive;
		ZombieGameObjects[i].ZombieAnimationState = ZombieIdle; //randomize this later on 	
		ZombieGameObjects[i].ZombieId = (i + 1);
	}

	//round 1+ zombie positions
	ZombieGameObjects[0].SetPosition(2.0f, 0.0f, -5.0f);
	ZombieGameObjects[1].SetPosition(8.0f, 0.0f, 5.0f);
	ZombieGameObjects[2].SetPosition(6.0f, 0.0f, -5.0f);

	//round 2+ zombie positions
	ZombieGameObjects[3].SetPosition(2.0f, 0.0f, 5.0f);
	ZombieGameObjects[4].SetPosition(8.0f, 0.0f, -5.0f);
	ZombieGameObjects[5].SetPosition(6.0f, 0.0f, 5.0f);
	ZombieGameObjects[6].SetPosition(2.0f, 0.0f, -8.0f);
	ZombieGameObjects[7].SetPosition(10.0f, 0.0f, 7.0f);

	// rounds 3+ zombie positions
	ZombieGameObjects[8].SetPosition(-8.0f, 0.0f, -5.0f);
	ZombieGameObjects[9].SetPosition(-6.0f, 0.0f, 5.0f);
	ZombieGameObjects[10].SetPosition(-10.0f, 0.0f, 5.0f);
	ZombieGameObjects[11].SetPosition(-10.0f, 0.0f, -5.0f);
	ZombieGameObjects[11].SetPosition(-10.0f, 0.0f, -2.0f);
	ZombieGameObjects[11].SetPosition(-10.0f, 0.0f, 10.0f);

	//round 4 zombie positions
	ZombieGameObjects[8].SetPosition(-15.0f, 0.0f, -5.0f);
	ZombieGameObjects[9].SetPosition(-15.0f, 0.0f, 5.0f);
	ZombieGameObjects[10].SetPosition(-15.0f, 0.0f, 5.0f);
	ZombieGameObjects[11].SetPosition(-15.0f, 0.0f, -5.0f);
	ZombieGameObjects[12].SetPosition(-15.0f, 0.0f, -2.0f);
	ZombieGameObjects[13].SetPosition(-15.0f, 0.0f, 10.0f);
	ZombieGameObjects[14].SetPosition(-15.0f, 0.0f, -15.0f);
	ZombieGameObjects[15].SetPosition(-15.0f, 0.0f, 15.0f);
	ZombieGameObjects[16].SetPosition(-15.0f, 0.0f, 12.0f);
	ZombieGameObjects[17].SetPosition(-15.0f, 0.0f, -18.0f);
	ZombieGameObjects[18].SetPosition(-12.0f, 0.0f, -4.0f);
	ZombieGameObjects[19].SetPosition(12.0f, 0.0f, 20.0f);
	ZombieGameObjects[20].SetPosition(-20.0f, 0.0f, -2.0f);
	ZombieGameObjects[21].SetPosition(-20.0f, 0.0f, 10.0f);
	ZombieGameObjects[22].SetPosition(20.0f, 0.0f, -15.0f);
	ZombieGameObjects[23].SetPosition(-15.0f, 0.0f, 0.0f);
	ZombieGameObjects[24].SetPosition(15.0f, 0.0f, 0.0f);
	ZombieGameObjects[25].SetPosition(-22.0f, 0.0f, -3.0f);
	ZombieGameObjects[26].SetPosition(22.0f, 0.0f, -4.0f);
	ZombieGameObjects[27].SetPosition(12.0f, 0.0f, -20.0f);



}


void InitGameObjects() {
	PlayerObject = new Player();
	InitZombieObjects();
}


float TimeSinceLastZombieRunAnimate = 0.0f;
void AnimateZombieWalk() {

	//update the player walk animation cycle
	float tSpan = glutGet(GLUT_ELAPSED_TIME) - TimeSinceLastZombieRunAnimate;
	if (tSpan > 50) {
		CurPlayerWalkKeyFrame += 1;

		if (CurPlayerWalkKeyFrame >= NUMPLAYERWALKEYFRAMES) {
			CurPlayerWalkKeyFrame = 0;
		}

		TimeSinceLastZombieRunAnimate = glutGet(GLUT_ELAPSED_TIME);
	}
	
}


int TimeSinceLastZombieAnimation = 0;


//returns the index of the zombie 
int GetIndexOfZombieClosestToPlayer() {
	const float minDist = 3.5f;
	const float minAngle = 12.0f;
	float distOfClosestZombie = 100;
	int indexOfClosestZombie = -1;
	for (int i = 0; i < NUMZOMBIES; i++) {
		Zombie* curZombie = &ZombieGameObjects[i];
		if (curZombie->ZombieStatus == ZombieDead) continue;

		//get the square of the distance from the zombie to the player. return if zombie out of range
		float distSquared = (PlayerObject->position.x - curZombie->xPos) * (PlayerObject->position.x - curZombie->xPos);
		distSquared += (PlayerObject->position.y - curZombie->yPos) * (PlayerObject->position.y - curZombie->yPos);
		distSquared += (PlayerObject->position.z - curZombie->zPos) * (PlayerObject->position.z - curZombie->zPos);

		if (distSquared > minDist) continue;

		//get the angle between the player and the zombie. Player can only attack zombie if angle is smaller than min angle
		Vector3 vZombieToPlayer;
		vZombieToPlayer.x = curZombie->xPos - PlayerObject->position.x;
		vZombieToPlayer.y = curZombie->yPos - PlayerObject->position.y;
		vZombieToPlayer.z = curZombie->zPos - PlayerObject->position.z;

		float angle = (180.0f / 3.1415) * Vector3::AngleBetweenVectors(PlayerObject->movementDirection.x, PlayerObject->movementDirection.y, PlayerObject->movementDirection.z, vZombieToPlayer.x, vZombieToPlayer.y, vZombieToPlayer.z);
	//F	if (angle < 0) angle = -angle;
		if (angle > minAngle ) continue;

		if (distSquared < distOfClosestZombie) {
			indexOfClosestZombie = i;
			distOfClosestZombie = distSquared;
		}
	}

	return indexOfClosestZombie;
}


void Animate( )
{
	//animate the player
	PlayerObject->Animate(ZombieGameObjects);

	//check if the player has attacked any zombie
	int zIndex = -1;
	if (PlayerObject->ZombieDeathTrigger) {
		PlayerObject->ZombieDeathTrigger = false;
		int zIndex = GetIndexOfZombieClosestToPlayer();
		if (zIndex >= 0) {
			ZombieGameObjects[zIndex].ZombieDeathTrigger = true;
		}
	}

	//animate all spawned zombies FIXME
	for (int i = 0; i < NumAliveZombies; i++) {
		if (ZombieGameObjects[i].ZombieStatus == ZombieAive) {
			ZombieGameObjects[i].Animate(ZombieGameObjects, PlayerObject, CurrentRound);
		}
	}

	//force call to Display()
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void Display( )
{
	if (PlayAnimation) {
		glutIdleFunc(Animate);
	}
	else {
		glutIdleFunc(NULL);
	}

	if (DebugOn != 0)
	{
		fprintf(stderr, "Display\n");
	}

	// set which window we want to do the graphics into:

	glutSetWindow(MainWindow);


	// erase the background:

	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (DepthBufferOn != 0)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);


	// specify shading to be flat:

	glShadeModel(GL_FLAT);


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = (vx - v) / 2;
	GLint yb = (vy - v) / 2;
	glViewport(xl, yb, v, v);


	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (WhichProjection == ORTHO)
		glOrtho(-3., 3., -3., 3., 0.1, 1000.);
	else
		gluPerspective(90., 1., 0.1, 1000.);


	// place the objects into the scene:

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// set the eye position, look-at position, and up-vector:
	SetCameraPosition();
	//gluLookAt(-3., 3., 0., 0., 0., 0., 0., 1., 0.);


	// rotate the scene:

	glRotatef((GLfloat)Yrot, 0., 1., 0.);
	glRotatef((GLfloat)Xrot, 1., 0., 0.);


	// uniformly scale the scene:

	if (Scale < MINSCALE)
		Scale = MINSCALE;
	glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);


	// set the fog parameters:

	if (DepthCueOn != 0)
	{
		glFogi(GL_FOG_MODE, FOGMODE);
		glFogfv(GL_FOG_COLOR, FOGCOLOR);
		glFogf(GL_FOG_DENSITY, FOGDENSITY);
		glFogf(GL_FOG_START, FOGSTART);
		glFogf(GL_FOG_END, FOGEND);
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}


	// since we are using glScalef( ), be sure normals get unitized:
	glEnable(GL_NORMALIZE);

	if (AxesOn != 0)
	{
		glColor3fv(&Colors[WhichColor][0]);
		glCallList(AxesList);
	}


	//draw the scene
	DrawScene();
	

	if (DepthFightingOn != 0)
	{
		glPushMatrix();
		glRotatef(90., 0., 1., 0.);
		glCallList(BoxList);
		glPopMatrix();
	}

	// swap the double-buffered framebuffers:

	glutSwapBuffers();


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush();

}


void SetPlayerAnimationFromKeyboardState() {
	if (KeyboardState.dKey || KeyboardState.aKey || KeyboardState.sKey || KeyboardState.wKey) {
		PlayerObject->CurrentAnimation = PlayerRunAnimation;
	}
	else {
		PlayerObject->CurrentAnimation = PlayerIdleAnimation;
	}
}


void DrawPlayer()
{
	    //Set walk or run animations based on the keyboard keys that are currently held down 
	  //  SetPlayerAnimationFromKeyboardState(KeyboardState.a);
	    PlayerObject->SetAnimationFromKeyboardState(KeyboardState.dKey, KeyboardState.aKey , KeyboardState.sKey ,KeyboardState.wKey);
		PlayerObject->DrawPlayer(HumaniodAnimations);
}

void DrawZombies() {
	for (int i = 0; i < NumAliveZombies; i++) { //FIXME
		ZombieGameObjects[i].DrawZombie(ZombieGameObjects, PlayerObject, HumaniodAnimations);
	}
}


void DrawGround() {
	glPushMatrix();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	Lighting::SetPointLight(GL_LIGHT0, 0, 2, 0, .5, .5, .5);
	glCallList(GroundList);
	glDisable(GL_LIGHTING);
	glPopMatrix();
}


void DrawPlayerTest() {
	DrawPlayer();
	DrawZombies();
	DrawGround();
}


void DrawHealthBar(int health) {
	if (health < 0) health = 0;
	const float inc = .2f;
	const float width = 0.3f;
	const float max = PlayerObject->MaxHealth * inc;
	float curY = 0.0f;
	float buff = 0.01f;


	Vector3 playerPos;
	playerPos.x = PlayerObject->position.x;
	playerPos.y = PlayerObject->position.y;
	playerPos.z = PlayerObject->position.z;

	glPushMatrix();

	glTranslatef(playerPos.x -4.6 , .8, playerPos.z -2.8);
	glRotatef(-45., 0., 0., 1);
	glColor3f(36./255.,176./255., 235./255.);
	glBegin(GL_QUADS);
	for (int i = 0; i < health; i++) {
		glVertex3f(0.0f, curY, 0.0f);
		glVertex3f(0.0f, curY, width);
		glVertex3f(0.0f, curY + inc, width);
		glVertex3f(0.0f, curY + inc, 0.0f);
		curY += inc;
	}
	glEnd();

	glColor3f(0., 0., 0.);
	glLineWidth(3.);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f , 0 - buff, 0.0f - buff);
	glVertex3f(0.0f , 0 - buff, width + buff);
	glVertex3f(0.0f , max + buff, width + buff);
	glVertex3f(0.0f , max + buff, 0.0f - buff);
	glVertex3f(0.0f, 0 - buff, 0.0f - buff);
	glEnd();

	glPopMatrix();
}

//camera always at a fixed distance from the player in the -x direction 
void SetCameraPosition() {
	Vector3 playerPos;
	playerPos.x = PlayerObject->position.x;
	playerPos.y = PlayerObject->position.y;
	playerPos.z = PlayerObject->position.z;

	Vector3 CameraPos = playerPos;
	CameraPos.x = CameraPos.x - 5;
	
	gluLookAt(CameraPos.x, 5, CameraPos.z,      playerPos.x, 0., playerPos.z,       0.0f, 1.0f, 0.0f);
}


//returns true if there is at least one alive zombie left. Returns false otherwise 
bool RoundOver() {
	for (int i = 0; i < NUMZOMBIES; i++) {
		if (ZombieGameObjects[i].ZombieStatus == ZombieAive) {
			return false;
		}
	}
	return true;
}

bool ShowRoundAnimation = true;
float TimeOfRoundAnimationStart = 0.0f;
void StartRound() {
	if (CurrentRound == 2) {
		NumAliveZombies += 4;
	}
	else if (CurrentRound == 3) {
		NumAliveZombies += 7;
	}
	else if (CurrentRound == 4) {
		NumAliveZombies += 20;
	}
	RestartGame(false);
	PlayerObject->Health = PlayerObject->MaxHealth;
	TimeOfRoundAnimationStart = glutGet(GLUT_ELAPSED_TIME);
	ShowRoundAnimation = true;
}

bool FirstDraw = true;
void DrawScene() {

	if (FirstDraw) {
		TimeOfRoundAnimationStart = glutGet(GLUT_ELAPSED_TIME);
		FirstDraw = false;
	}


	

	if (RoundOver()) {
		CurrentRound++;
		StartRound();
	}

		DrawPlayerTest();
		DrawHealthBar(PlayerObject->Health);

	glPushMatrix();
    if (PlayerObject->IsAlive == false) {
		glColor3f(1, 1, 1);
		
		DisplayText(PlayerObject->position.x, 4, PlayerObject->position.z - .5 , "Game Over");
		glScalef(2, 2, 2);
	} 
	glPopMatrix();

	if (ShowRoundAnimation) {
		float tSpan = glutGet(GLUT_ELAPSED_TIME) - TimeOfRoundAnimationStart;
		if (tSpan < 2000) {
			glPushMatrix();
			glColor3f(1, 1, 1);
			std::string roundString = "Round " + std::to_string(CurrentRound);
			char *cstr = new char[roundString.length() + 1];
			strcpy(cstr, roundString.c_str());
			DisplayText(PlayerObject->position.x, 4, PlayerObject->position.z - .5, cstr);
			glPopMatrix();
		}
		else {
			ShowRoundAnimation = false;
		}
		tSpan = glutGet(GLUT_ELAPSED_TIME);
	}
}


void DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:
void DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// return the number of seconds since the start of the program:
float ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:
void InitMenus( )
{
	glutSetWindow( MainWindow );
	int startMenu = glutCreateMenu(DoStartMenu);
	glutAddMenuEntry("Restart", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void DoStartMenu(int id) {
	glutSetWindow(MainWindow);
	glutPostRedisplay();
	RestartGame(false);
}


void RestartGame(bool resetRound) {
	IsGameOver = false;
	PlayerObject->Reset();
	for (int i = 0; i < NumAliveZombies; i++) {
		ZombieGameObjects[i].Reset();
	}
	if(resetRound)
	CurrentRound = 1;
	ShowRoundAnimation = true;
	FirstDraw = true;
}


void InitLighting() {
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Lighting::MulArray3(.3f, White));
}


// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions
void InitGraphics( )
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	//glutMotionFunc( MouseMotion );
	glutMotionFunc(NULL);
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( NULL );
	glutSpecialUpFunc(NULL);
	glutKeyboardUpFunc(KeyboardRelease);
	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

}


void InitGround() {
	GroundList = glGenLists(1);
	glNewList(GroundList, GL_COMPILE);
	Geometries::DrawPlane(100, 100, .7, .7, .7, 0, 0, 1);
	glEndList();
}


void InitAxes() {
	// create the axes:
	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(AXES_WIDTH);
	Geometries::Axes(1.5);
	glLineWidth(1.);
	glEndList();
}


void InitLists( )
{
	glutSetWindow( MainWindow );


	InitGround();
	InitAxes();

	//Create display lists for player animations
	HumaniodAnimations = new AnimationDisplayLists();
	AnimationFunctions::InitPlayerWalkAnimation(HumaniodAnimations);
	AnimationFunctions::InitPlayerIdleAnimation(HumaniodAnimations);
	AnimationFunctions::InitZombieWalkAnimation(HumaniodAnimations);
	AnimationFunctions::InitZombieAttackAnimation(HumaniodAnimations);
	AnimationFunctions::InitPlayerDeathAnimation(HumaniodAnimations);
	AnimationFunctions::InitPlayerAttackAnimation(HumaniodAnimations);
	AnimationFunctions::InitZombieDeathAnimation(HumaniodAnimations);

}


// the keyboard callback:
void Keyboard( unsigned char c, int x, int y )
{
	KeyboardHandler(c, x, y, PlayerObject, KeyboardState.aKey, KeyboardState.dKey, KeyboardState.sKey, KeyboardState.wKey, PlayAnimation);
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:
void MouseButton( int button, int state, int x, int y )
{

	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

			//code for middle mouse scroll wheel
		//case 3:
		//	b = MIDDLE;
		//	Scale += SCLFACT * (float)(5);

		//	// keep object from turning inside-out or disappearing:
		//	if (Scale < MINSCALE)
		//		Scale = MINSCALE;

		//	glutSetWindow(MainWindow);
		//	glutPostRedisplay();
		//	break;

		//case 4:
		//	b = MIDDLE;
		//	Scale -= SCLFACT * (float)(5);

		//	glutSetWindow(MainWindow);
		//	glutPostRedisplay();
		//	break;


		//case GLUT_MIDDLE_BUTTON:
		//	b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		
		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}


// called when the mouse moves while a button is down:
void MouseMotion( int x, int y )
{
	//if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy/10 );
		Yrot += ( ANGFACT*dx/10 );
	}


	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene
void Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:
void Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:
void Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}










