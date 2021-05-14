#pragma once
#include"Player.h"
enum ZombieAnimationState{
    ZombieIdle,
	ZombieWalk,
	ZombieChase, 
	ZombieAttack
};

enum ZombieAnimations {
	ZombieWalkAnimation,
	ZombieAttackAnimation,
	ZombieDeathAnimation,
	ZombieDance1Animation,
	ZombieDance2Animation,
};

enum ZombieStatus {
	ZombieAive,
	ZombieDead,
	ZombieDying
};



class Zombie
{

private: 
	float TimeSinceLastAnimate;
	float TimeSinceLastPositionChange;
	float AngleFromPlayer;
	Vector3 VectorToPlayer;
	int CurrentAnimation;
	bool IsAttacking;
	void SetVectorToPlayer(Player* p);
	Vector3 CrossProduct(float, float, float, float, float, float);
	float DotProduct(float, float, float, float, float, float);
	float AngleBetweenVectors(float, float, float, float, float, float);
	float VectorMagnitude(float, float , float);
	void MakeUnitVector(Vector3& v);
	bool CanMoveToPosition(Zombie* ZombieGameObjects, float x, float y, float z);
	float initialXPos;
	float initialYPos;
	float initialZPos;

	const float MinDamageDistance = 1.5f; //the closest the zombie can ever get to the player
	const float MinAttackDistance = 1.5f;
public:
	Zombie();
	~Zombie();
	void Reset();
	void SetZombieSpawnPosition(Zombie* zombiList, int zombieListLength, float playerXPos, float playerYPos, float playerZPos);
	void SetPosition(float x, float y, float z);
	void IncrementKeyFrame();
	void DrawZombie(Zombie* ZombieGameObject, Player* playerObject, AnimationDisplayLists* animations);
	void Animate(Zombie* ZombieGameObject, Player *p, int curRound);
	void ComputeZombieDirection(Zombie zombie, Zombie*zombieList);
	float xPos, yPos, zPos, xDir, yDir, zDir;
	int ZombieAnimationState;
	int ZombieStatus;
	int CurKeyFrame;
	int ZombieId;
	bool ZombieDeathTrigger = false;
	bool IsDying = false;
};

