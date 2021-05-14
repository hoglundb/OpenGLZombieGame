#pragma once




const static float xx[] = {
		0.f, 1.f, 0.f, 1.f
};

const static float xy[] = {
		-.5f, .5f, .5f, -.5f
};

const static int xorder[] = {
		1, 2, -3, 4
};

const static float yx[] = {
		0.f, 0.f, -.5f, .5f
};

const static float yy[] = {
		0.f, .6f, 1.f, 1.f
};

const static int yorder[] = {
		1, 2, 3, -2, 4
};

const static float zx[] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
};

const static float zy[] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
};

const static int zorder[] = {
		1, 2, 3, 4, -5, 6
};
// fraction of the length to use as height of the characters:
const static float LENFRAC = 0.10f;


// fraction of length to use as start location of the characters:
const static float BASEFRAC = 1.10f;


static class Geometries {
public:


// the stroke characters 'X' 'Y' 'Z' :
	static void Axes(float length)
	{
		glBegin(GL_LINE_STRIP);
		glVertex3f(length, 0., 0.);
		glVertex3f(0., 0., 0.);
		glVertex3f(0., length, 0.);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(0., 0., 0.);
		glVertex3f(0., 0., length);
		glEnd();

		float fact = LENFRAC * length;
		float base = BASEFRAC * length;

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 4; i++)
		{
			int j = xorder[i];
			if (j < 0)
			{

				glEnd();
				glBegin(GL_LINE_STRIP);
				j = -j;
			}
			j--;
			glVertex3f(base + fact * xx[j], fact*xy[j], 0.0);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 5; i++)
		{
			int j = yorder[i];
			if (j < 0)
			{

				glEnd();
				glBegin(GL_LINE_STRIP);
				j = -j;
			}
			j--;
			glVertex3f(fact*yx[j], base + fact * yy[j], 0.0);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 6; i++)
		{
			int j = zorder[i];
			if (j < 0)
			{

				glEnd();
				glBegin(GL_LINE_STRIP);
				j = -j;
			}
			j--;
			glVertex3f(0.0, fact*zy[j], base + fact * zx[j]);
		}
		glEnd();

	}



	static void DrawPlane(int width, int numStrips, float r1, float g1, float b1, float r2, float g2, float b2) {
		float d = (float)((float)width / (float)numStrips);
		float curX = 0;
		float curZ = 0;
		glPushMatrix();
		glTranslatef(-width / 2.0, 0, -width / 2.0);
		bool Color1 = true;
		int count = 1;
		glShadeModel(GL_FLAT);
		for (int j = 0; j < width; j++) {
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(curX, 0, curZ);
			Lighting::SetMaterial(.1, .6, .1, .5);
			for (int i = 0; i < 2 * width - 1; i++) {
				if (i % 2 == 0) {
					curZ += d;
				}
				else {
					curZ -= d;
					curX += d;
				}
				if (count == 2) {
					Color1 = !Color1; //swap color every other triangle to color the squares 
					count = 0;
				}

				if (Color1) {
					//glColor3f(r1, g1, b1);
				}
				else {
					//glColor3f(r2, g2, b2);
				}

				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(curX, 0, curZ);
				count++;
			}
			glEnd();
			curX = 0;
			count = 1;

		}


		glPopMatrix();

	}
};