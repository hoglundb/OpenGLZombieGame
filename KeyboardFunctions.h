#pragma once
void KeyboardReleaseHandler(unsigned char c, int x, int y, Player* p, bool &aKey, bool &dKey, bool &sKey, bool &wKey) {
	switch (c)
	{
	case 'w':
	case 'W':
		p->movementKeys.forward = false;
		wKey = false;
		break;

	case 's':
	case 'S':
		p->movementKeys.backward = false;
		sKey = false;
		break;

	case 'a':
	case 'A':
		p->movementKeys.left = false;
		aKey = false;
		break;

	case 'd':
	case 'D':
		p->movementKeys.right = false;
		dKey = false;
		break;

	}
}


void KeyboardHandler(unsigned char c, int x, int y, Player* p, bool &aKey, bool &dKey, bool &sKey, bool &wKey, bool &playAnimation) {
	if (false)
		fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

	switch (c)
	{

	case 'w':
	case 'W':
		p->movementKeys.forward = true;
		if (p->IsAttackingZombie) {
			p->IsAttackingZombie = false;
		}
		p->IsAttackingZombie = false;
		wKey = true;
		break;

	case 's':
	case 'S':
		p->movementKeys.backward = true;
		if (p->IsAttackingZombie) {
			p->IsAttackingZombie = false;
		}
		sKey = true;
		break;

	case 'a':
	case 'A':
		p->movementKeys.left = true;
		if (p->IsAttackingZombie) {
			p->IsAttackingZombie = false;
		}
		aKey = true;
		break;

	case 'd':
	case 'D':
		p->movementKeys.right = true;
		if (p->IsAttackingZombie) {
			p->IsAttackingZombie = false;
		}
		dKey = true;
		break;

	case 'g':
	case 'G':
		playAnimation = !playAnimation;
		break;
	case 'f':
	case 'F':
		if (p->IsAttackingZombie == false) {
			p->IsAttackingZombie = true;
			p->CurKeyFrame = 0;
		}
		
		// happy compiler

	default:
		fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c);
	}
}

