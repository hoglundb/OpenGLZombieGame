#pragma once
#include"Keyframe.h"

static class AnimationFunctions {
public:

	static void DrawPlayerMesh(Keyframe k, float r, float g, float b) {

		glPushMatrix();
		glShadeModel(GL_SMOOTH);
		Lighting::SetMaterial(r, g, b, 1);
		glBegin(GL_TRIANGLES);


		for (int i = 0; i < k.vertices->size(); i++) {

			glNormal3f(k.vertices->at(i).xNorm, k.vertices->at(i).yNorm, k.vertices->at(i).zNorm);
			glVertex3f(k.vertices->at(i).xPos, k.vertices->at(i).yPos, k.vertices->at(i).zPos);
		}
		glEnd();
		glPopMatrix();
	}

	static Keyframe* LoadKeyframesFromObj(std::string baseFileName, std::string * fileNames, int numKeyFrames) {
		std::string fileName = "";
		std::string fp = baseFileName;
		Keyframe* Keyframes = new Keyframe[numKeyFrames / 2];
		for (int i = 0; i < numKeyFrames / 2; i++) {
			fileName = fp + fileNames[i] + ".obj";
			char *cstr = new char[fileName.length() + 1];
			strcpy(cstr, fileName.c_str());
			Keyframes[i] = *ObjLoaderFunctions::LoadObjFile(cstr);
		}
		return Keyframes;
	}



	static void InitPlayerWalkAnimation(AnimationDisplayLists* d) {
		//load the keyframes from the file 
		std::string fileNames[NUMPLAYERWALKEYFRAMES / 2] = { "001",  "004", "007",  "010",  "013" , "016" };
		Keyframe* keyframes = LoadKeyframesFromObj("Animations/HumanWalk/run_000", fileNames, NUMPLAYERWALKEYFRAMES);

		//get inbetween frames 
		Keyframe* allAnimationFrames = AnimationFunctions::InterpolateKeyFrames(keyframes, NUMPLAYERWALKEYFRAMES);

		//load all frames into the global display list object 
		for (int i = 0; i < NUMPLAYERWALKEYFRAMES; i++) {
			d->PlayerWalkKeyFrames[i] = glGenLists(1);
			glNewList(d->PlayerWalkKeyFrames[i], GL_COMPILE);
			glPushMatrix();
			DrawPlayerMesh(allAnimationFrames[i], .3, .3, .3);
			glPopMatrix();
			glEndList();
		}
	}


	static void InitPlayerAttackAnimation(AnimationDisplayLists* d) {
		//load the keyframes from the file 
		std::string fileNames[NUMPLAYERATTACKKEYFRAMES / 2] = { "001",  "004", "008",  "012",  "016" , "020", "024", "028", "132" , "136"};
		Keyframe* keyframes = LoadKeyframesFromObj("Animations/PlayerAttack/playerAttack_000", fileNames, NUMPLAYERATTACKKEYFRAMES);

		//get inbetween frames 
		Keyframe* allAnimationFrames = AnimationFunctions::InterpolateKeyFrames(keyframes, NUMPLAYERATTACKKEYFRAMES);

		//load all frames into the global display list object 
		for (int i = 0; i < NUMPLAYERATTACKKEYFRAMES; i++) {
			d->PlayerAttackKeyFrames[i] = glGenLists(1);
			glNewList(d->PlayerAttackKeyFrames[i], GL_COMPILE);
			glPushMatrix();
			DrawPlayerMesh(allAnimationFrames[i], .3, .3, .3);
			glPopMatrix();
			glEndList();
		}
	}

	static void InitZombieDeathAnimation(AnimationDisplayLists* d) {
		//load the keyframes from the file 
		std::string fileNames[NUMZOMBIEDEATHKEYFRAMES / 2] = { "001",  "008", "016",  "024",  "032" , "040", "048", "056", "064" };
		Keyframe* keyframes = LoadKeyframesFromObj("Animations/ZombieDeath/zombieDeath_000", fileNames, NUMZOMBIEDEATHKEYFRAMES);

		//get inbetween frames 
		Keyframe* allAnimationFrames = AnimationFunctions::InterpolateKeyFrames(keyframes, NUMZOMBIEDEATHKEYFRAMES);

		//load all frames into the global display list object 
		for (int i = 0; i < NUMZOMBIEDEATHKEYFRAMES; i++) {
			d->ZombieDeathKeyFrames[i] = glGenLists(1);
			glNewList(d->ZombieDeathKeyFrames[i], GL_COMPILE);
			glPushMatrix();
			DrawPlayerMesh(allAnimationFrames[i], 99. / 255., 65 / 255., 46 / 255.);
			glPopMatrix();
			glEndList();
		}
	}




	static void InitPlayerDeathAnimation(AnimationDisplayLists* d) {
		//load the keyframes from the file 
		std::string fileNames[NUMPLAYERDEATHKEYFRAMES / 2] = { "001",  "005", "010",  "015",  "020" , "125", "130", "135", "140", "145", "150", "160", "155", "160", "170", "175" };
		Keyframe* keyframes = LoadKeyframesFromObj("Animations/HumanDeath/humanDeath_000", fileNames, NUMPLAYERDEATHKEYFRAMES);

		//get inbetween frames 
		Keyframe* allAnimationFrames = AnimationFunctions::InterpolateKeyFrames(keyframes, NUMPLAYERDEATHKEYFRAMES);

		//load all frames into the global display list object 
		for (int i = 0; i < NUMPLAYERDEATHKEYFRAMES; i++) {
			d->PlayerDeathKeyFrames[i] = glGenLists(1);
			glNewList(d->PlayerDeathKeyFrames[i], GL_COMPILE);
			glPushMatrix();
			DrawPlayerMesh(allAnimationFrames[i], .3, .3, .3);
			glPopMatrix();
			glEndList();
		}
	}

	static void InitZombieWalkAnimation(AnimationDisplayLists* d) {
		std::string fileNames[NUMZOMBIEWALKKEYFRAMES / 2] = { "001",  "020", "040",  "060",  "080" , "100", "115" };
		Keyframe* keyframes = LoadKeyframesFromObj("Animations/ZombieWalk/zombieWalk_000", fileNames, NUMZOMBIEWALKKEYFRAMES);
		Keyframe k = keyframes[0];
		Keyframe k2 = keyframes[1];
		Keyframe k3 = keyframes[2];
		Keyframe k4 = keyframes[3];
		Keyframe k5 = keyframes[4];
		Keyframe k6 = keyframes[5];
		//get inbetween frames 
		Keyframe* allAnimationFrames = AnimationFunctions::InterpolateKeyFrames(keyframes, NUMZOMBIEWALKKEYFRAMES);

		////load all frames into the global display list object 
		for (int i = 0; i < NUMZOMBIEWALKKEYFRAMES; i++) {
			d->ZombieWalkKeyFrames[i] = glGenLists(1);
			glNewList(d->ZombieWalkKeyFrames[i], GL_COMPILE);
			glPushMatrix();
			DrawPlayerMesh(allAnimationFrames[i], 99. / 255., 65 / 255., 46 / 255.);
			glPopMatrix();
			glEndList();
		}
	}

	static void InitPlayerIdleAnimation(AnimationDisplayLists* d) {
		std::string fileNames[NUMPLAYERIDLEKEYFRAMES / 2] = { "001",  "008", "016",  "024",  "032" , "040" };
		Keyframe* keyframes = LoadKeyframesFromObj("Animations/Idle/idle_000", fileNames, NUMPLAYERIDLEKEYFRAMES);
		Keyframe k = keyframes[0];
		Keyframe k2 = keyframes[1];
		Keyframe k3 = keyframes[2];
		Keyframe k4 = keyframes[3];
		Keyframe k5 = keyframes[4];
		Keyframe k6 = keyframes[5];
		//get inbetween frames 
		Keyframe* allAnimationFrames = AnimationFunctions::InterpolateKeyFrames(keyframes, NUMPLAYERIDLEKEYFRAMES);

		//load all frames into the global display list object 
		for (int i = 0; i < NUMPLAYERIDLEKEYFRAMES; i++) {
			d->PlayerIdleKeyFrames[i] = glGenLists(1);
			glNewList(d->PlayerIdleKeyFrames[i], GL_COMPILE);
			glPushMatrix();
			DrawPlayerMesh(allAnimationFrames[i], .3, .3, .3);
			glPopMatrix();
			glEndList();
		}
	}

	static void InitZombieAttackAnimation(AnimationDisplayLists*d) {
		std::string fileNames[NUMZOMBIEATTACKKEYFRAMES / 2] = { "001",  "008", "015",  "022",  "029" , "036", "042" };
		Keyframe* keyframes = LoadKeyframesFromObj("Animations/ZombieAttack/zombieAttack_000", fileNames, NUMZOMBIEATTACKKEYFRAMES);
		Keyframe k = keyframes[0];
		Keyframe k2 = keyframes[1];
		Keyframe k3 = keyframes[2];
		Keyframe k4 = keyframes[3];
		Keyframe k5 = keyframes[4];
		Keyframe k6 = keyframes[5];
		//get inbetween frames 
		Keyframe* allAnimationFrames = AnimationFunctions::InterpolateKeyFrames(keyframes, NUMZOMBIEATTACKKEYFRAMES);

		//load all frames into the global display list object 
		for (int i = 0; i < NUMZOMBIEATTACKKEYFRAMES; i++) {
			d->ZombieAttackKeyFrames[i] = glGenLists(1);
			glNewList(d->ZombieAttackKeyFrames[i], GL_COMPILE);
			glPushMatrix();
			DrawPlayerMesh(allAnimationFrames[i], 99. / 255., 65 / 255., 46 / 255.);
			glPopMatrix();
			glEndList();
		}
	}


	//takes a pointer to an array of key frames and returns an array of size numTotalFrames of interpolated key frames 
	static Keyframe* InterpolateKeyFrames(Keyframe* keyframes, int numTotalFrames) {

		//build an array to hold all the frames and copy the keyframes into it at every other index
		Keyframe* allAnimationFrames = new Keyframe[numTotalFrames];
		int curIndex = 0;
		for (int i = 0; i < numTotalFrames; i++) {
			if (i % 2 == 0) {
				allAnimationFrames[i] = keyframes[curIndex++];
			}
		}

		//add the inbetween frames by linearly iterpolating between the vertex positions and the normals for every other index 
		int prevIndex = 0;
		int nextIndex = 2;
		for (int i = 0; i < numTotalFrames; i++) {
			if (i % 2) {
				if (i == numTotalFrames - 1) nextIndex = 0;
				Keyframe* prevKeyframe = &allAnimationFrames[prevIndex];
				Keyframe* nextKeyframe = &allAnimationFrames[nextIndex];
				int foo = prevKeyframe->vertices->size();
				int goo = nextKeyframe->vertices->size();
				Keyframe* inbetweener = new Keyframe();
				for (int j = 0; j < prevKeyframe->vertices->size(); j++) {
					//data from the previous key frame
					float xPosPrev = prevKeyframe->vertices->at(j).xPos;
					float yPosPrev = prevKeyframe->vertices->at(j).yPos;
					float zPosPrev = prevKeyframe->vertices->at(j).zPos;
					float xNormPrev = prevKeyframe->vertices->at(j).xNorm;
					float yNormPrev = prevKeyframe->vertices->at(j).yNorm;
					float zNormPrev = prevKeyframe->vertices->at(j).zNorm;

					//data for the next key frame
					float xPosNext = nextKeyframe->vertices->at(j).xPos;
					float yPosNext = nextKeyframe->vertices->at(j).yPos;
					float zPosNext = nextKeyframe->vertices->at(j).zPos;
					float xNormNext = nextKeyframe->vertices->at(j).xNorm;
					float yNormNext = nextKeyframe->vertices->at(j).yNorm;
					float zNormNext = nextKeyframe->vertices->at(j).zNorm;

					//the values for the current keyframe will be the midpoints between the two 
					float xPos = (xPosPrev + xPosNext) / 2.0f;
					float yPos = (yPosPrev + yPosNext) / 2.0f;
					float zPos = (zPosPrev + zPosNext) / 2.0f;
					float xNorm = (xNormPrev + xNormNext) / 2.0f;
					float yNorm = (yNormPrev + yNormNext) / 2.0f;
					float zNorm = (zNormPrev + zNormNext) / 2.0f;

					inbetweener->AddVertex(xPos, yPos, zPos, xNorm, yNorm, zNorm);
				}
				allAnimationFrames[i] = *inbetweener;
				prevIndex += 2;
				nextIndex += 2;
			}
		}
		return allAnimationFrames;
	}
};



