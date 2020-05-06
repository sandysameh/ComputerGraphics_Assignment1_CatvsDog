
#include <math.h>
#include <stdio.h>
#include <Windows.h>    
#include <string.h>
#include<MMSystem.h>
#pragma comment(lib, "winmm.lib") 
using namespace std;
#include <stdlib.h>
#include <glut.h>

double moontrans = 0.001;
double starTrans = 0.1;

int yUpL = 0;
int power = 0;
int pointx = 0;
int pointy = 0;
int powerB = 0;


int p0[2];
int p1[2];
int p2[2];
int p3[2];


float t = 0;
//General points Deciding 
double curveX = 0;
double curveY = 0;


//For LionWeapon Translation
double curveXlion = 0;
double curveYlion = 0;

//for Bird weapon Translation
double curveXBird = 0;
double curveYBird = 0;

int lifelion = 220; //4 lives enha bn3mel -20 !!
int lifeBird = 860; // -20 4 lives

int scaledef = 1;
bool def = true; //going like the moon 
double defTrans = -200;

bool turn = true; //true then lion if false then Bird



bool birdlost = false;
bool lionLost = false;

char* text = "I'M ANGRY @(!%WG";
char* textBird = "BIRD LOST";
char* textLion = "LION LOST";

bool doneshootlion = true;
bool doneshootBird = true;
void drawText(const char *text, int length, int x, int y) {
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glLoadIdentity();
	glRasterPos2i(x, y);
	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);


}


void key(unsigned char k, int x, int y)//keyboard function, takes 3 parameters
									// k is the key pressed from the keyboard
									// x and y are mouse postion when the key was pressed.
{
	if (k == 'p'&&power < 120 && turn == true) //if the letter d is pressed, then the object will be translated in the x axis by 10 (moving to the right).
		power += 5;
	if (k == 'p'&&powerB < 120 && turn == false) //if the letter d is pressed, then the object will be translated in the x axis by 10 (moving to the right).
		powerB += 5;

	glutPostRedisplay();//redisplay to update the screen with the changes
}

void keyUp(unsigned char k, int x, int y)//keyboard up function is called whenever the keyboard key is released
{
	if (k == 'p'&&turn == true) {
		doneshootlion = false;
		t = 0;
		if (power <= 30) {//newpart (doesn't hit ay haga)
			p0[0] = 0;
			p0[1] = 0;
			p1[0] = 10;
			p1[1] = 200;
			p2[0] = 200;
			p2[1] = 180;
			p3[0] = 200;
			p3[1] = 0;
		}

		if (power > 30 && power <= 60) { //supposedly hits the obstacle
			p0[0] = 0;
			p0[1] = 111;
			p1[0] = 2;
			p1[1] = 355;
			p2[0] = 283;
			p2[1] = 373;
			p3[0] = 279;
			p3[1] = 29;

		}

		if (power > 60 && power <= 90) {//passes the obstacle 
			p0[0] = 38;
			p0[1] = 90;
			p1[0] = 69;
			p1[1] = 375;
			p2[0] = 392;
			p2[1] = 429;
			p3[0] = 466;
			p3[1] = 1;
		}
		if (power > 90) {//will always hit the beak without aslan hitting defender
			p0[0] = 2;
			p0[1] = 13;
			p1[0] = 104;
			p1[1] = 313;
			p2[0] = 472;
			p2[1] = 150;
			p3[0] = 650;
			p3[1] = 10;
		}
		/*curveXlion = 0;
		curveYlion = 0;
		curveYBird = 0;
		curveXBird = 0;*/

		power = 0;
		turn = false;

	}
	else {
		if (k == 'p'&&turn == false) {
			doneshootBird = false;
			t = 0;
			if (powerB <= 30) {//newpart (doesn't hit ay haga)
				p0[0] = 0;
				p0[1] = 0;
				p1[0] = 10;
				p1[1] = 200;
				p2[0] = 200;
				p2[1] = 180;
				p3[0] = 200;
				p3[1] = 0;
			}

			if (powerB > 30 && powerB <= 60) { //supposedly hits the obstacle
				p0[0] = 0;
				p0[1] = 111;
				p1[0] = 2;
				p1[1] = 355;
				p2[0] = 283;
				p2[1] = 373;
				p3[0] = 279;
				p3[1] = 29;

			}

			if (powerB > 60 && powerB <= 90) {//passes the obstacle 
				p0[0] = 38;
				p0[1] = 90;
				p1[0] = 69;
				p1[1] = 375;
				p2[0] = 392;
				p2[1] = 429;
				p3[0] = 466;
				p3[1] = 1;
			}
			if (powerB > 90) {//will always hit the beak without aslan hitting defender
				p0[0] = 2;
				p0[1] = 13;
				p1[0] = 104;
				p1[1] = 313;
				p2[0] = 472;
				p2[1] = 150;
				p3[0] = 650;
				p3[1] = 10;
			}


			/*curveYBird = 0;
			curveXBird = 0;
			curveXlion = 0;
			curveYlion = 0;*/
			powerB = 0;
			turn = true;

		}
	}


	glutPostRedisplay();//redisplay to update the screen with the changed
}





int* bezier(float t, int* p0, int* p1, int* p2, int* p3)
{
	int res[2];
	res[0] = pow((1 - t), 3) * p0[0] + 3 * t * pow((1 - t), 2) * p1[0] + 3 * pow(t, 2) * (1 - t) * p2[0] + pow(t, 3) * p3[0];
	res[1] = pow((1 - t), 3) * p0[1] + 3 * t * pow((1 - t), 2) * p1[1] + 3 * pow(t, 2) * (1 - t) * p2[1] + pow(t, 3) * p3[1];
	return res;
}



void drawStarDefend() {

	glBegin(GL_QUADS);
	glColor3f(0.196078, 0.6, 0.8);
	glVertex3f(10, 400, 0);
	glVertex3f(10, 390, 0);
	glVertex3f(60, 390, 0);
	glVertex3f(60, 400, 0);

	glColor3f(1.00, 0.43, 0.78);
	glVertex3f(10, 390, 0);
	glVertex3f(10, 380, 0);
	glVertex3f(60, 380, 0);
	glVertex3f(60, 390, 0);

	glColor3f(0, 1.0, 0.498039);
	glVertex3f(10, 380, 0);
	glVertex3f(10, 370, 0);
	glVertex3f(60, 370, 0);
	glVertex3f(60, 380, 0);


	glEnd();



	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0);
	glVertex3f(40, 400, 0);
	glVertex3f(80, 400, 0);
	glVertex3f(60, 360, 0);

	glVertex3f(40, 370, 0);
	glVertex3f(80, 370, 0);
	glVertex3f(60, 410, 0);



	glEnd();




}






void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric *quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}
void drawLion() {
	//glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f);//ORANGE
	drawCircle(100, 100, 45);//Hair of the lion

	glBegin(GL_QUADS);//Face of the lion
	glColor3f(1.0f, 1.0f, 0.0f);//YELLOW
	glVertex3f(77, 70, 0);
	glVertex3f(77, 125, 0);
	glVertex3f(124, 125, 0);
	glVertex3f(124, 70, 0);
	glEnd();

	glBegin(GL_TRIANGLES);//Ears of the line
	glColor3f(0.1f, 0.1f, 0.0f);//Outer EAR (BRONZE)
	glVertex3f(110, 125, 0);
	glVertex3f(126, 125, 0);
	glVertex3f(118, 140, 0);

	glColor3f(0.1f, 0.1f, 0.0f);//Inner Ear
	glVertex3f(75, 125, 0);
	glVertex3f(90, 125, 0);
	glVertex3f(83, 140, 0);
	glEnd();

	glPushMatrix();
	glPointSize(4);
	glBegin(GL_POINTS);//EYES
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(112, 112, 0);
	glVertex3f(88, 112, 0);
	glEnd();


	glPopMatrix();

	glPushMatrix();//MOUTH
	glLineWidth(4);
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);//BLACK
	glVertex3f(88, 80, 0);
	glVertex3f(112, 80, 0);
	glEnd();
	glPopMatrix();

	//glPopMatrix();
}


void lionPower() {

	/*glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 1);
	glVertex3f(20, 0, 0);
	glVertex3f(40, 0, 0);



	glVertex3f(40, 80, 0);
	glVertex3f(20, 80, 0);
	glEnd();*/



	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex3f(20, 0, 0);
	glVertex3f(40, 0, 0);
	glVertex3f(40, power, 0);
	glVertex3f(20, power, 0);
	glEnd();

}

void lionWeapon() {
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(145, 100, 0);
	glVertex3f(145, 150, 0);
	glVertex3f(170, 150, 0);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	drawCircle(150, 145, 3);


}

void lionLife() {
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	glVertex3f(140, 20, 0);
	glVertex3f(140, 40, 0);
	glVertex3f(lifelion, 40, 0);
	glVertex3f(lifelion, 20, 0);
	glEnd();

}

void Birdlife() {
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	glVertex3f(780, 20, 0);
	glVertex3f(780, 40, 0);
	glVertex3f(lifeBird, 40, 0);
	glVertex3f(lifeBird, 20, 0);
	glEnd();

}

void birdWeapon() {

	glColor3f(0.0, 0.5, 0.0);
	drawCircle(790, 90, 10);
	glBegin(GL_QUADS);
	glColor3f(0.0, 0.5, 0.0);
	glVertex3f(785, 95, 0);
	glVertex3f(785, 105, 0);
	glVertex3f(795, 105, 0);
	glVertex3f(795, 95, 0);
	glEnd();




}



void BirdPower() {

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex3f(930, 0, 0);
	glVertex3f(910, 0, 0);
	glVertex3f(910, powerB, 0);
	glVertex3f(930, powerB, 0);
	glEnd();

}

void drawBird() {

	glBegin(GL_QUADS);//BODY OF THE BIRD
	glColor3f(0.0f, 0.5f, 0.5f);//BLUE--GREEN
	glVertex3f(815, 145, 0);
	glVertex3f(895, 145, 0);
	glVertex3f(895, 70, 0);
	glVertex3f(815, 70, 0);
	glEnd();

	glBegin(GL_TRIANGLES);//BEAK OF THE BIRD
	glColor3f(1.0f, 0.5f, 0.0f);//Orange
	glVertex3f(815, 140, 0);
	glVertex3f(800, 135, 0);
	glVertex3f(815, 120, 0);
	glEnd();

	glPushMatrix();
	glPointSize(8);
	glBegin(GL_POINTS);//EYES OF THE BIRD
	glColor3f(0.0f, 0.0f, 0.0f);//Black
	glVertex3f(825, 130, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();//DOT IN EYE OF THE BIRD
	glPointSize(3);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);//WHITE
	glVertex3f(825, 130, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glLineWidth(4);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(835, 70, 0);//LEG 1 (Left)
	glVertex3f(835, 55, 0);

	glVertex3f(835, 55, 0); //LEG 1 (finger left)
	glVertex3f(825, 55, 0);

	glVertex3f(835, 55, 0);//Leg 1 (finger down)
	glVertex3f(835, 50, 0);

	glVertex3f(875, 70, 0);//LEG 2 (Right)
	glVertex3f(875, 55, 0);

	glVertex3f(875, 55, 0); //LEG 2 (finger left)
	glVertex3f(865, 55, 0);

	glVertex3f(875, 55, 0);//LEG 2 (Finger Down)
	glVertex3f(875, 50, 0);

	glEnd();



}

void drawTree() {

	glBegin(GL_QUADS);
	glColor3f(0.36, 0.25, 0.20);//Brown(Dark) STEM
	glVertex3f(455, 0, 0);
	glVertex3f(495, 0, 0);
	glVertex3f(495, 130, 0);
	glVertex3f(455, 130, 0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.5, 0.0);//DARK GREEN (BIG BUSH)
	glVertex3f(415, 120, 0);
	glVertex3f(535, 120, 0);
	glVertex3f(475, 160, 0);


	glColor3f(0.0, 1.0, 0.0); //GREEN (Medium bush)
	glVertex3f(430, 150, 0);
	glVertex3f(520, 150, 0);
	glVertex3f(475, 190, 0);

	glColor3f(0.498039, 1.0, 0); //Light Green (Small Bush)
	glVertex3f(440, 180, 0);
	glVertex3f(510, 180, 0);
	glVertex3f(475, 210, 0);

	glEnd();
}

void drawMoon() {
	glPushMatrix();
	//glTranslated(moontrans, 0, 0);
	glColor3f(1.0, 1.0, 0.0);//Big MOON
	drawCircle(100, 550, 20);
	glPushMatrix();//DRAWING ANOTHER CIRCLE TO DO AN ECLIPSE
	glColor3f(0.0f, 0.0f, 0.0f);
	drawCircle(120, 550, 20);
	glPopMatrix();


	glPushMatrix();
	glBegin(GL_POINTS);
	glPointSize(1);
	glColor3f(0, 0, 0);
	glVertex3f(96, 560, 0);
	glVertex3f(91, 560, 0);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glLineWidth(2);
	glBegin(GL_LINES);

	glColor3f(0, 0, 0);

	glVertex3f(89, 550, 0);
	glVertex3f(92, 545, 0);

	glVertex3f(92, 545, 0);
	glVertex3f(95, 550, 0);

	//glVertex3f(34,525)


	glEnd();
	glPopMatrix();

	glPopMatrix();


}

void drawGems() {
	//WE WILL DRAW 4 SQUARES then ha3mel function We will name it rotated Gems Translation ;)


	glBegin(GL_QUADS);
	glColor3f(0.90, 0.91, 0.98);//1st star to the left
	glVertex3f(90, 480, 0);
	glVertex3f(80, 470, 0);
	glVertex3f(90, 460, 0);
	glVertex3f(100, 470, 0);

	glVertex3f(150, 480, 0); //2nd star to the left
	glVertex3f(140, 470, 0);
	glVertex3f(150, 460, 0);
	glVertex3f(160, 470, 0);

	glVertex3f(860, 480, 0); //1st star to the right
	glVertex3f(850, 470, 0);
	glVertex3f(860, 460, 0);
	glVertex3f(870, 470, 0);

	glVertex3f(800, 480, 0); //2nd star to the right
	glVertex3f(810, 470, 0);
	glVertex3f(800, 460, 0);
	glVertex3f(790, 470, 0);
	glEnd();



	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);
	glVertex3f(90, 480, 0);
	glVertex3f(90, 460, 0);
	glVertex3f(80, 470, 0);
	glVertex3f(100, 470, 0);


	glVertex3f(150, 480, 0); //2nd star to the left
	glVertex3f(140, 470, 0);
	glVertex3f(150, 460, 0);
	glVertex3f(160, 470, 0);

	glVertex3f(860, 480, 0); //1st star to the right
	glVertex3f(850, 470, 0);
	glVertex3f(860, 460, 0);
	glVertex3f(870, 470, 0);

	glVertex3f(800, 480, 0); //2nd star to the right
	glVertex3f(810, 470, 0);
	glVertex3f(800, 460, 0);
	glVertex3f(790, 470, 0);

	glEnd();


}





void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	if (birdlost == true)
	{
		drawText(textBird, 9, 490, 300);
	}
	else {
		if (lionLost == true) {
			drawText(textLion, 9, 460, 300);
		}
		else {


			drawLion();
			drawBird();
			drawTree();




			glPushMatrix(); //Moving Moon
			glTranslated(moontrans, 0, 0);
			drawMoon();
			glPopMatrix();


			glPushMatrix();
			glTranslated(0, starTrans, 0);
			drawGems();
			glPopMatrix();

			//newpart





			glPushMatrix();
			glTranslated(curveXlion, curveYlion, 0);
			lionWeapon();
			glPopMatrix();



			glPushMatrix();
			glTranslated(curveXBird, curveYBird, 0);
			birdWeapon();
			glPopMatrix();
			//End of new Headache part

			lionPower();
			BirdPower();



			glPushMatrix();
			glTranslated(defTrans, 0, 0);
			//glScaled(scaledef, 1, 1);
			drawStarDefend();

			glPopMatrix();



			//LION RETURNING BACK


			if (curveXlion >= 180 && curveXlion <= 260 && curveYlion >= 0 && curveYlion <= 145) {
				curveYlion = 0;
				curveXlion = 0;

			}




			if (curveXlion >= 279 && curveXlion <= 400 && curveYlion >= 0 && curveYlion <= 200) {//Tree law el tree bas lazem ableha el defeneder
				curveXlion = 0;
				curveYlion = 0;
				//doneshoot = true;

			}
			else {



				if (curveXlion >= 400 && curveXlion <= 466 && curveYlion >= 1 && curveYlion <= 20) {
					curveXlion = 0;
					curveYlion = 0;
					//doneshoot = true;
				}


				if (curveXlion >= 640 && curveYlion <= 150 && curveYlion >= 10 && turn == false) {//if hits the opponent
					curveXlion = 0;
					curveXlion = 0;


					for (float i = 0; i < 200; i += 1) {
						drawText(text, 15, 830, 150);
					}
					sndPlaySound(TEXT("bird.wav"), SND_ASYNC);

					if (lifeBird >= 780 && doneshootlion == false) {
						lifeBird = lifeBird - 20;
					}
					else
					{
						if (lifeBird == 780) {
							birdlost = true;
						}
					}
					doneshootlion = true;
				}
			}


			//BIRD TURN BACK

			if (curveX >= 180 && curveX <= 260 && curveY >= 0 && curveY <= 145) {
				curveYBird = 0;
				curveXBird = 0;

			}





			if (curveX >= 279 && curveX <= 400 && curveY >= 0 && curveY <= 200) {//Tree law el tree bas lazem ableha el defeneder
				curveXBird = 0;
				curveYBird = 0;
				//doneshoot = true;

			}
			else {



				if (curveX >= 400 && curveX <= 466 && curveY >= 1 && curveY <= 20) {
					curveXBird = 0;
					curveYBird = 0;
					//doneshoot = true;
				}


				if (curveX >= 640 && curveY <= 150 && curveY >= 10 && turn == true) {//if hits the opponent
					curveXBird = 0;
					curveXBird = 0;
					//for (float i = 0; i < 200; i += 1) {
					drawText(text, 15, 90, 160);


					if (lifelion >= 140 && doneshootBird == false) {
						sndPlaySound(TEXT("lion.wav"), SND_ASYNC);
						lifelion = lifelion - 20;
					}
					else
					{
						if (lifelion == 140) {
							lionLost = true;
						}
					}
					doneshootBird = true;
				}
			}


			lionLife();
			Birdlife();





			/*

			glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(150.0f, 200.0f, 0.0f);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(100.0f, 100.0f, 0.0f);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(200.0f, 100.0f, 0.0f);
			glEnd();*/


		}
	}
	glFlush();
}


void Anim()
{
	if (moontrans >= 900) {
		moontrans = 0.1;
	}
	moontrans += 0.01; //for rotation of moon 

	starTrans -= 0.001;
	if (460 + starTrans < 430) {
		starTrans = 0.1;

	}

	if (def == true) {
		defTrans += 0.1;
	}

	if (0 + defTrans > 900) {
		/*scaledef = -1;*/
		defTrans = -400;
	}
	/*def = false;*/
/*}
if (100 + defTrans < 200 && def == false) {
	defTrans = 100;
	scaledef = 1;
	def = true;
}
if (def == false) {
	defTrans -= 0.1;

}*/

//NOT WORKING






	if (t < 1)
	{


		int*p = bezier(t, p0, p1, p2, p3);
		curveX = p[0];
		curveY = p[1];
		t += 0.001;

		if (turn == false) { //dor el lion
			curveXlion = curveX;
			curveYlion = curveY;

		}
		if (turn == true) {//dor el bird
			curveXBird = -curveX;
			curveYBird = curveY;
		}

		//when Hits Defender Be5!(LION WEAPON)
		if (curveXlion + 160 >= 10 + defTrans && curveXlion + 170 <= 100 + defTrans && curveY + 120 >= 360) {
			t = 2;
			curveXlion = 0;
			curveYlion = 0;

		}

		//when Hits DEFENDER be5 (BIRD WEAPON)
		if (curveXBird + 795 >= 10 + defTrans && curveXBird + 800 <= 100 + defTrans && curveY + 100 >= 360) {
			t = 2;
			curveXBird = 0;
			curveYBird = 0;

		}






	}
	//if(t==0){
	//
	///*	for(float i=0;i>1000000000000000;i+=0.0000000001*pow(10,-465)){
	//	}
	//	for (float i = 0; i > 1000000000000000; i += 0.0000000001*pow(10, -465)) {
	//	}*/
	//	curveXlion = 0;
	//	curveYlion = 0;
	//}










	glutPostRedisplay();
}





void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(200, 200);

	glutCreateWindow("Lions VS Perry The Platops");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);

	glutKeyboardFunc(key);			//call the keyboard function
	glutKeyboardUpFunc(keyUp);		//call the keyboard up function


	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, 1000, 0.0, 600);
	//glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glutMainLoop();
}
