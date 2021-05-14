#pragma once

#include "Vector3.h"
#include "glew.h"
#include "Animations.h"

class Zombie;
//the movement as determined by the w,s,a,d keys
struct Movement {
	bool forward;
	bool backward;
	bool left;
	bool right;
};

enum PlayerColors {
	PlayerColorRed,
	PlayerColorNormal
};

enum PlayerAnimations {
	PlayerIdleAnimation,
	PlayerRunAnimation,
	PlayerDeathAnimation,
	PlayerAttackAnimation
};

class Player
{

private:
	const Vector3 ForwardDir = { 1.0f, 0.0f, 0.0f };
	const Vector3 BackwardDir = { -1.0f, 0.0f, 0.0f };
	const Vector3 LeftDir = { 0.0f, 0.0f, -1.0f };
	const Vector3 RightDir = { 0.0f, 0.0f, 1.0f };
	const Vector3 ForwardLeftDir = { 0.70711f, 0.0f, -0.70711f };
	const Vector3 ForwardRightDir = { 0.70711f, 0.0f, 0.70711f };
	const Vector3 BackwardLeftDir = { -0.70711, 0.0f, -0.70711f };
	const Vector3 BackwardRightDir = { -0.70711, 0.0f, 0.70711f };
	const Vector3 Stationary = { 0.0f, 0.0f, 0.0f };
	const float ColorBinkAnimationTime = 3.0f;
	float TimeSinceLastAnimate;
	float TimeSinceLastMove;

	float PlayAtackColorBlink;
	float TimeSinceBeingAttacked;
	float CurPlayerColor;

	Vector3 SetPlayerColor();
	void PlayColorAnimation();
	float TimeSinceLastColorAnimate;  //tracks the time between
	float TimeOfLastAttack = 0;
	bool IsPlayingColorAnimation = false;
	Vector3 initialPosition;
	float initialRotAng;
	Vector3 initialMovementDirection;

public:
	int PlayerColor; //three and we are done playing the animation
	bool IsAttackingZombie;
	bool IsAlive;
	int Health;
	const int MaxHealth = 5;
	Vector3 position;
	Vector3 movementDirection;
	float rotAngle = 0.0f;
	Movement movementKeys;
	float speed;
	int CurrentAnimation;
	bool IsBeingAttacked;

	void CalcPlayerMovementDirection();
	void SetAnimationFromKeyboardState(bool aKey, bool dKey, bool sKey, bool wKey);
	void DrawPlayer(AnimationDisplayLists* callList);
	void Animate(Zombie* zombieList );
	void Reset();
	int CurKeyFrame;
	bool FinishedDying;
	bool ZombieDeathTrigger = false;
	Player();
	~Player();
};

