#include "Zombie.h"
#include"Animations.h"
#include "LightingFunctions.h"
#include <math.h>
//randomly sets the position of the zombie at a fixed distance from the player and from the other alive zombies 
void Zombie::SetZombieSpawnPosition(Zombie* zombiList, int zombieListLength, float playerXPos, float playerYPos, float playerZPos) {

}

void Zombie::SetPosition(float x, float y, float z) {
	this->xPos = x;
	this->yPos = y;
	this->zPos = z;
	this->initialXPos = x;
	this->initialYPos = y;
	this->initialZPos = z;
}


void Zombie::Reset() {
	this->xPos = this->initialXPos;
	this->yPos = this->initialYPos;
	this->zPos = this->initialZPos;
	this->ZombieStatus = ZombieAive;
	this->CurrentAnimation = ZombieWalkAnimation;
	this->IsAttacking = false;
	this->CurKeyFrame = 0;
	this->IsDying = false;
	this->TimeSinceLastAnimate = 0.0f;

}



void Zombie::IncrementKeyFrame() {
	this->CurKeyFrame++;
	if (this->CurKeyFrame == NUMZOMBIEWALKKEYFRAMES) {
		this->CurKeyFrame = 0;
	}
}


void Zombie::DrawZombie(Zombie* ZombieGameObjects, Player* playerObject, AnimationDisplayLists* HumaniodAnimations) {
	glPushMatrix();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	Lighting::SetPointLight(GL_LIGHT0, playerObject->position.x - 4, 100, playerObject->position.z, .5,.5,.5);
	for (int i = 0; i < NUMZOMBIES; i++) {
		if (this->ZombieStatus == ZombieDead) continue;
		glPushMatrix();
		glTranslatef(this->xPos, this->yPos, this->zPos);
		float a = this->AngleFromPlayer;
		glRotatef(a, 0.0f, 1.0f, 0.0f);
		int cur = this->CurKeyFrame;
		if (this->CurrentAnimation == ZombieWalkAnimation) {
			glCallList(HumaniodAnimations->ZombieWalkKeyFrames[this->CurKeyFrame]);
		}
		else if (this->CurrentAnimation == ZombieAttackAnimation) {
			glCallList(HumaniodAnimations->ZombieAttackKeyFrames[this->CurKeyFrame]);
		}
		else if (this->CurrentAnimation == ZombieDeathAnimation) {
			glCallList(HumaniodAnimations->ZombieDeathKeyFrames[this->CurKeyFrame]);
		}
		glPopMatrix();

	}
	glDisable(GL_LIGHTING);
	glPopMatrix();
}



//set the current keyframe to animate, the animation to play, the position and roation of the zombie 
void Zombie::Animate(Zombie* ZombieGameObjects, Player* p, int curRound) {

	//if the game is over just play the zombie dance animation and return
	if (p->FinishedDying) {
		this->CurrentAnimation = ZombieWalkAnimation;
		return;
	}


	if (this->ZombieDeathTrigger) {
		int test = 1;
		this->ZombieDeathTrigger = false;
		this->CurrentAnimation = ZombieDeathAnimation;
		this->CurKeyFrame = 0;
	}


	float tSpanDeath = glutGet(GLUT_ELAPSED_TIME) - this->TimeSinceLastAnimate;
	if (this->CurrentAnimation == ZombieDeathAnimation) {
		if (tSpanDeath > 100) {
			this->CurKeyFrame++;
			if (this->CurKeyFrame >= NUMZOMBIEDEATHKEYFRAMES - 1) {
				this->CurKeyFrame = NUMZOMBIEDEATHKEYFRAMES - 1;
				this->ZombieStatus = ZombieDead;
			}
			this->TimeSinceLastAnimate = glutGet(GLUT_ELAPSED_TIME);
		}
		return;
	}


	//set the current animation and keyframe
	float tSpanZombieWalk = glutGet(GLUT_ELAPSED_TIME) - this->TimeSinceLastAnimate;
	
	//set the animation speed based on the animation that is playing 
	float animationSpeed = 0.0f;
	if (this->CurrentAnimation == ZombieWalkAnimation) animationSpeed = 90.0f;
	else if (this->CurrentAnimation == ZombieAttackAnimation) animationSpeed = 120.0f;
     if (tSpanZombieWalk > animationSpeed) {

		 if (this->ZombieStatus == ZombieDead) return;
		 this->CurKeyFrame++;

		 //if near the end of the animation, the player takes a hit (only if within min attack distance )
		 if (this->IsAttacking && this->CurKeyFrame == NUMZOMBIEATTACKKEYFRAMES - 5) {
			 SetVectorToPlayer(p);
			 float distToPlayer = this->VectorMagnitude(this->VectorToPlayer.x, this->VectorToPlayer.y, this->VectorToPlayer.z);
			 if (distToPlayer <= this->MinAttackDistance) {
				 p->IsBeingAttacked = true;
			 }			
		 }

		 //go back to walk animation by defualt at the end of animation cycle
		 if (this->CurKeyFrame == NUMZOMBIEWALKKEYFRAMES) {
			 this->CurKeyFrame = 0;
			 this->IsAttacking = false;
		 }
		 this->TimeSinceLastAnimate = glutGet(GLUT_ELAPSED_TIME);
	}
	
	//set the vector between the player and the zombie
	 SetVectorToPlayer(p);
	 float curDistToPlayer = this->VectorMagnitude(this->VectorToPlayer.x, this->VectorToPlayer.y, this->VectorToPlayer.z);
	 //compute the angle between the player and the zombie 
	 this->AngleFromPlayer = this->AngleBetweenVectors(VectorToPlayer.x, VectorToPlayer.y, VectorToPlayer.z, this->xDir, this->yDir, this->zDir);
	 this->AngleFromPlayer = 180.0f * this->AngleFromPlayer / (3.14f);
	 Vector3 dir = this->CrossProduct(VectorToPlayer.x, VectorToPlayer.y, VectorToPlayer.z, this->xDir, this->yDir, this->zDir);
	 float d = dir.y;
	 if (dir.y > 0) this->AngleFromPlayer = -this->AngleFromPlayer;

	 //compute the new direction of the zombie now that it has rotated towards the player 
    MakeUnitVector(VectorToPlayer);


	//update the animation to play based on if the zombie is close enough to the player to attack
	 if (curDistToPlayer <= this->MinAttackDistance) {

		//when beginning an attack, start at the first keyframe in the animation
		if (this->IsAttacking == false) {
			this->CurKeyFrame = 0;
		}
		this->CurrentAnimation = ZombieAttackAnimation;
		this->IsAttacking = true; 
	}
	else {
		if (!this->IsAttacking) {
			this->CurrentAnimation = ZombieWalkAnimation;
		}
	}

    //compute zombies next x,y,z position based on its speed and the position of the other zombies 
	 //update the players position based on the movement direction parameters 
	 if (this->CurrentAnimation == ZombieWalkAnimation && curDistToPlayer > this->MinAttackDistance) {
		 float tSpan = glutGet(GLUT_ELAPSED_TIME) - this->TimeSinceLastPositionChange;
		 float speed = .035f;
		 if (curRound == 2) speed = 0.076;
		 if (curRound == 3) speed == .12;
		 if (curRound == 4) speed == .29;
		 if (tSpan > 50) {

			 float nextXPos = this->xPos + this->VectorToPlayer.x * speed;
			 float nextYPos = this->yPos + this->VectorToPlayer.y * speed;
			 float nextZPos = this->zPos + this->VectorToPlayer.z * speed;

			 if (CanMoveToPosition(ZombieGameObjects, nextXPos, nextYPos, nextZPos)) {
				 this->xPos = nextXPos;
				 this->yPos = nextYPos;
				 this->zPos = nextZPos;
			 }

			 this->TimeSinceLastPositionChange = glutGet(GLUT_ELAPSED_TIME);
		 }
	 }
	
}


//returns true if the position is less than the specified distance from each other zombie 
bool Zombie::CanMoveToPosition(Zombie* ZombieGameObjects, float x, float y, float z) {

	const float squareOfMinDist = .5;
	for (int i = 0; i < NUMZOMBIES; i++) {
		Zombie * curNeighbor = &ZombieGameObjects[i];

		//not spawned zombies and the current zombie get skipped 
		if (curNeighbor->ZombieStatus != ZombieAive || curNeighbor->ZombieId == this->ZombieId) continue;

		float distSquared = (x - curNeighbor->xPos) * (x - curNeighbor->xPos) + (y - curNeighbor->yPos) * (y - curNeighbor->yPos) + (z - curNeighbor->zPos) * (z - curNeighbor->zPos);
		if (distSquared <= squareOfMinDist) {
			return false;
		}

	}
	return true;
}


void Zombie::MakeUnitVector(Vector3 &v) {
	float mag = this->VectorMagnitude(v.x, v.y, v.z);
	if(v.x != 0.0f)	v.x = v.x / mag;
	if(v.y != 0.0f)	v.y = v.y / mag;
	if(v.z != 0.0f)	v.z = v.z / mag;
}

Vector3 Zombie::CrossProduct(float x1, float y1, float z1, float x2, float y2, float z2) {
	float xComp = (y1 * z2 - z1 * y2);
	float yComp = (z2 * x1 - x2 * z1);
	float zComp = (x1 * y2 - x2 * y1);
	Vector3 CrossProd;
	CrossProd.x = xComp;
	CrossProd.y = -yComp;
	CrossProd.z = zComp;
	return CrossProd;
}

float Zombie::AngleBetweenVectors(float x1, float y1, float z1, float x2, float y2, float z2) {
	float dot = this->DotProduct( x1,  x2,  y1,  y2,  z1,  z2);
	float mag1 = this->VectorMagnitude(x1, y1, z1);
	float mag2 = this->VectorMagnitude(x2, y2, z2);
	return acos(dot / (mag1 * mag2));
}


float Zombie::DotProduct(float x1, float x2, float y1, float y2, float z1, float z2) {
	return(x1 * x2 + y1 * y2 + z1 * z2);
}


float Zombie::VectorMagnitude(float x, float y, float z) {
	return sqrt(x * x + y * y + z * z);
}


//computes the vector from the zombie to the player. 
void Zombie::SetVectorToPlayer(Player* p) {
	float x = p->position.x - this->xPos;
	float y = 0.0f;
	float z = p->position.z - this->zPos;
	this->VectorToPlayer.x = x;
	this->VectorToPlayer.y = y;
	this->VectorToPlayer.z = z;
}


Zombie::Zombie()
{
	this->ZombieStatus = ZombieDead;
	this->ZombieAnimationState = ZombieIdle;
	this->CurrentAnimation = ZombieWalkAnimation;
	this->CurKeyFrame = 0;
	this->ZombieId = 0;
	this->TimeSinceLastAnimate = 0.0f;
	this->TimeSinceLastPositionChange = 0.0f;
	this->VectorToPlayer.x = 0.0f;
	this->VectorToPlayer.y = 0.0f;
	this->VectorToPlayer.z = 0.0f;
	this->xDir = 0.0f; 
	this->yDir = 0.0f;
	this->zDir = 1.0f;
	this->IsAttacking = false;
}


Zombie::~Zombie()
{
}
