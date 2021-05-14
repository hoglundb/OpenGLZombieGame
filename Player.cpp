#include "Player.h"
#include "Animations.h"
#include "LightingFunctions.h"

Player::Player()
{
	
	this->movementDirection.x = 0.0f;
	this->movementDirection.y = 0.0f;
	this->movementDirection.z = 1.0f;
	this->position.x = 0.0f;
	this->position.y = 0.0f;
	this->position.z = 0.0f;
	this->speed = 0.0f;
	this->CurKeyFrame = 0;
	this->CurrentAnimation = PlayerIdleAnimation;
	this->TimeSinceLastAnimate = 0.0f;
	this->movementDirection.x = 0.0f;
	this->movementDirection.y = 0.0f;
	this->movementDirection.z = 0.0f;
	this->movementKeys.backward = false;
	this->movementKeys.forward = false;
	this->movementKeys.left = false;
	this->movementKeys.right = false;
	this->IsBeingAttacked = false;
	this->Health = this->MaxHealth;
	this->PlayAtackColorBlink = false;
	this->TimeSinceBeingAttacked = 10000.0f;
	this->CurPlayerColor = PlayerColorNormal;
	this->IsAlive = true;
	this->TimeSinceLastColorAnimate = 0.0f;
	this->PlayerColor = PlayerColorNormal;
	this->FinishedDying = false;

	this->initialPosition = this->position;
	this->initialRotAng =  this->rotAngle;
	this->initialMovementDirection = this->movementDirection;
	this->IsAttackingZombie = false;

}


void Player::Reset() {
	this->rotAngle = this->initialRotAng;
	this->position = this->initialPosition;
	this->movementDirection = this->initialMovementDirection;
	this->Health = this->MaxHealth;
	this->CurrentAnimation = PlayerIdleAnimation;
	this->FinishedDying = false;
	this->IsBeingAttacked = false;
	this->IsAlive = true;
	this->PlayAtackColorBlink = false;
	this->CurKeyFrame = 0;
	this->IsAttackingZombie = false;
}


Player::~Player()
{
}


//
Vector3 Player::SetPlayerColor() {
	Vector3 c;
	if (this->CurPlayerColor == PlayerColorRed) {
		c.x = .8;
		c.y = .1;
		c.z = .1;
	}
	else {
		c.x = .5;
		c.y = .5;
		c.z = .5;
	}
	return c;
}


//plays the animation that blinks the player red after a hit 
void Player::PlayColorAnimation() {
	if (this->IsPlayingColorAnimation) {
		this->CurPlayerColor = PlayerColorRed;
		float curTime = glutGet(GLUT_ELAPSED_TIME);
		if (curTime - this->TimeOfLastAttack > 400.0f) {
			IsPlayingColorAnimation = false;
			this->CurPlayerColor = PlayerColorNormal;
		}
	}

}





void Player::DrawPlayer(AnimationDisplayLists* callList) {

	//determine if the player is reciving an attack at this instant 

	if (this->IsBeingAttacked && this->Health > 0) {
		this->IsBeingAttacked = false;
		this->Health -= 1.0f;
    	this->TimeSinceLastAnimate = 0.0f;
		this->CurKeyFrame = 0;
		this->TimeOfLastAttack = glutGet(GLUT_ELAPSED_TIME);
		this->IsPlayingColorAnimation = true;
	
	}
	this->PlayColorAnimation();



	glPushMatrix();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	Vector3 playerColor = SetPlayerColor();
	Lighting::SetPointLight(GL_LIGHT0, position.x - 4, 100, position.z, playerColor.x, playerColor.y, playerColor.z);
	glTranslatef(this->position.x, this->position.y, this->position.z);
	glRotatef(this->rotAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

	//Call the list for the current keyframe of the current animation  
	if (this->CurrentAnimation == PlayerRunAnimation) {
		glCallList(callList->PlayerWalkKeyFrames[this->CurKeyFrame]);
	}
	else if (this->CurrentAnimation == PlayerIdleAnimation) {
		glCallList(callList->PlayerIdleKeyFrames[this->CurKeyFrame]);
	}
	else if (this->CurrentAnimation == PlayerDeathAnimation) {
		glCallList(callList->PlayerDeathKeyFrames[this->CurKeyFrame]);
	}
	else if (this->CurrentAnimation == PlayerAttackAnimation) {
		glCallList(callList->PlayerAttackKeyFrames[this->CurKeyFrame]);
	}

	glDisable(GL_LIGHTING);
	glPopMatrix();
}


void Player::CalcPlayerMovementDirection() {
	//forward or forward + sideways
	if (this->movementKeys.forward) {
		if (this->movementKeys.left) {
			this->movementDirection = ForwardLeftDir;
			this->rotAngle = 45.0f;
		}
		else if (this->movementKeys.right) {
			this->movementDirection = ForwardRightDir;
			this->rotAngle = -45.0f;
		}
		else {
			this->movementDirection = ForwardDir;
			this->rotAngle = 0.0f;
		}
	}

	//backward or backward + sideways
	else if (this->movementKeys.backward) {
		if (this->movementKeys.left) {
			this->movementDirection = BackwardLeftDir;
			this->rotAngle = 135.0f;
		}
		else if (this->movementKeys.right) {
			this->movementDirection = BackwardRightDir;
			this->rotAngle = -135.0f;
		}
		else {
			this->movementDirection = BackwardDir;
			this->rotAngle = 180.0f;
		}
	}

	//only moving left
	else if (this->movementKeys.left) {
		this->movementDirection = LeftDir;
		this->rotAngle = 90.0f;
	}

	//only moving right
	else if (this->movementKeys.right) {
		this->movementDirection = RightDir;
		this->rotAngle = -90.0f;
	}

	else {
		this->movementDirection = Stationary;
	}
}

 

void Player::Animate(Zombie* zombieList) {
	this->ZombieDeathTrigger = false;
	//play the death animation 
	if (this->Health <= 0) {
		this->CurrentAnimation = PlayerDeathAnimation;
		float tSpanRun = glutGet(GLUT_ELAPSED_TIME) - this->TimeSinceLastAnimate;
		if (tSpanRun > 100) {
			this->CurKeyFrame += 1;
			if (this->CurKeyFrame >= NUMPLAYERDEATHKEYFRAMES - 1) {
				this->CurKeyFrame = NUMPLAYERDEATHKEYFRAMES - 2;
				this->FinishedDying = true;
				this->IsAlive = false;
			}
			this->TimeSinceLastAnimate = glutGet(GLUT_ELAPSED_TIME);
		}
	}

	else if (this->IsAttackingZombie) {
		this->CurrentAnimation = PlayerAttackAnimation;
		float tSpan = glutGet(GLUT_ELAPSED_TIME) - this->TimeSinceLastAnimate;
		if (tSpan > 40) {
			this->CurKeyFrame++;
			if (this->CurKeyFrame == NUMPLAYERATTACKKEYFRAMES - 1) {
				this->IsAttackingZombie = false;
				this->CurKeyFrame = 0;
				this->CurrentAnimation = PlayerIdleAnimation;
				return;
			}
			this->TimeSinceLastAnimate = glutGet(GLUT_ELAPSED_TIME);

			//if a zombie is in front of the player and close enough, set the zombie to die when zombie->animate() is called next frame 
			if (this->CurKeyFrame == NUMPLAYERATTACKKEYFRAMES - 5) {
				this->ZombieDeathTrigger = true;
			}
			
		}
	}

	else if (this->CurrentAnimation == PlayerRunAnimation) {
		//update the players pose based on the elapsed time 
		float tSpanRun = glutGet(GLUT_ELAPSED_TIME) - this->TimeSinceLastAnimate;
		if (tSpanRun > 70) {
			this->CurKeyFrame += 1;
			if (this->CurKeyFrame >= NUMPLAYERWALKEYFRAMES - 1) {
				this->CurKeyFrame = 0;
			}
			this->TimeSinceLastAnimate = glutGet(GLUT_ELAPSED_TIME);
		}

		//update the players position based on the movement direction parameters 
		tSpanRun = glutGet(GLUT_ELAPSED_TIME) - this->TimeSinceLastMove;
		float speed = .035f;
		if (tSpanRun > 8) {
			this->CalcPlayerMovementDirection();
			Vector3 playerDir = this->movementDirection;
			this->position.x = this->position.x + this->movementDirection.x * speed;
			this->position.y = this->position.y + this->movementDirection.y * speed;
			this->position.z = this->position.z + this->movementDirection.z * speed;
			this->TimeSinceLastMove = glutGet(GLUT_ELAPSED_TIME);
		}
	}
	else if (this->CurrentAnimation == PlayerIdleAnimation) {
		float tSpanIdle = glutGet(GLUT_ELAPSED_TIME) - this->TimeSinceLastAnimate;
		if (tSpanIdle > 150) {
			this->CurKeyFrame += 1;
			if (this->CurKeyFrame >= NUMPLAYERIDLEKEYFRAMES) {
				this->CurKeyFrame = 0;
			}
			this->TimeSinceLastAnimate = glutGet(GLUT_ELAPSED_TIME);
		}
	}
}



void Player::SetAnimationFromKeyboardState(bool aKey, bool dKey, bool sKey, bool wKey) {
	if (this->CurrentAnimation == PlayerDeathAnimation) return;
	if (dKey || aKey || sKey || wKey) {
		this->CurrentAnimation = PlayerRunAnimation;
	}
	else {
		if (this->IsAttackingZombie) {
			this->CurrentAnimation = PlayerAttackAnimation;
		}
		else {
			this->CurrentAnimation = PlayerIdleAnimation;
		}
		
	}
}