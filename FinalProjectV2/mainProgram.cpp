#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "myPrimitive.h"
#include "myColor.h"
#include "myRandom.h"

#define M_PI 3.14159265358979323846

float mat_tempColor[4];

float eyeYFirst = 4.0;

//  ตัวแปรเกี่ยวกับกล้อง และการเคลื่อนที่ของกล้อง
float angle = 0.0, deltaAngle = 0.0;
float eyeX = 0.0, eyeY = eyeYFirst, eyeZ = 10.0;
float objX = 0.0, objY = 0.0, objZ = -1.0;

int deltaMove = 0;

int currentTime, lastTime;
float deltaTime;
float elapsedTime = 0.0f;
int fps = 0;

float speed = 0.2f;

MyRandom myrand;
int grass_near;
float* grass_size;
float** grass_world2_pos;

float* worlds_size;
float** worlds_pos;
int* world_things;

float*** worlds_grass_pos;

bool isRandom = false;

void showFps() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentTime - lastTime) / 1000.0;
	elapsedTime += deltaTime;
	fps++;

	if (elapsedTime >= 1.0f) {
		printf("x = (%f, %f, %f)\n", eyeX, eyeY, eyeZ);
		fps = 0;
		elapsedTime = 0.0f;
	}
	lastTime = currentTime;
}

// ฟังก์ชันกำหนดการเคลื่อนไหวของกล้อง
void orientMe(float ang) {
	objX = sin(ang);
	objZ = -cos(ang);
}
void moveMeFlat(int i) {
	eyeX = eyeX + i * (objX)*speed;
	eyeZ = eyeZ + i * (objZ)*speed;
}
void Lookup(int i) {
	objY += speed * i;
}

void setMyMatTempColor(float r, float g, float b, float a = 1.0f) {
	mat_tempColor[0] = r;
	mat_tempColor[1] = g;
	mat_tempColor[2] = b;
	mat_tempColor[3] = a;
}
void setMyMatTempColor255(float r, float g, float b, float a = 255.0f) {
	mat_tempColor[0] = r / 255.0;
	mat_tempColor[1] = g / 255.0;
	mat_tempColor[2] = b / 255.0;
	mat_tempColor[3] = a / 255.0;
}

float* getMyTempColor() {
	return mat_tempColor;
}

float myFindAngle(float x1, float y1, float x2, float y2) {
	float delta_x = x2 - x1;
	float delta_y = y2 - y1;

	float inRads = atan2(delta_y, delta_x);

	if (inRads < 0)
		inRads = fabsf(inRads);
	else
		inRads = 2 * M_PI - inRads;

	return ((inRads * (180 / M_PI)) + 270);
}

float myVectorDistanct(float x1, float y1, float z1, float x2, float y2, float z2) {
	float x = x2 - x1;
	float y = y2 - y1;
	float z = z2 - z1;
	return sqrt((x * x) + (y * y) + (z * z));
}

void useMyTransplarent() {
	glPushAttrib(GL_LIGHTING_BIT); // เปิดใช้ stack แสง

	glEnable(GL_BLEND);  // เปิดใช้การเกลี่ยสี (สำหรับทำโปร่งแสง)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ฟังก์ชั่นในการหา Detreal การเกลี่ยสี

	glDepthMask(GL_FALSE);  // ให้วัตถุทะลุขึ้นมาได้
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}

void closeMyTransplarent() {
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	glCullFace(GL_BACK);
	glPopAttrib();
}

void myEnableBackCull() {
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}

void myEnableFontCull() {
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
}

void myDisableCull() {
	glDisable(GL_CULL_FACE);
}

void myColor2CV(float* c1, float* c2) {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c1);
	glMaterialfv(GL_FRONT, GL_AMBIENT, c2);
}

void myColorPastel(float r255, float g255, float b255, float a255 = 255.0, float per = 0.75) {
	setMyMatTempColor255(r255, g255, b255, a255);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
	setMyMatTempColor255(r255 - (per * r255), g255 - (per * g255), b255 - (per * b255), a255);
	glMaterialfv(GL_FRONT, GL_AMBIENT, getMyTempColor());
}
void myColorPastelBoth(float r255, float g255, float b255, float a255 = 255.0, float per = 0.75) {
	setMyMatTempColor255(r255, g255, b255, a255);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getMyTempColor());
	setMyMatTempColor255(r255 - (per * r255), g255 - (per * g255), b255 - (per * b255), a255);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getMyTempColor());
}

void myColorPastelBack(float r255, float g255, float b255, float a255 = 255.0, float per = 0.75) {
	setMyMatTempColor255(r255, g255, b255, a255);
	glMaterialfv(GL_BACK, GL_DIFFUSE, getMyTempColor());
	setMyMatTempColor255(r255 - (per * r255), g255 - (per * g255), b255 - (per * b255), a255);
	glMaterialfv(GL_BACK, GL_AMBIENT, getMyTempColor());
}

void myFixSeeTransparent() {
	glDepthMask(GL_TRUE);
}

void drawSphereWorld(int wid, float world_margin, float world_ground_percen) {
	/* ---------------
	 * คำแนะนำการสร้าง Transparent Glass
	 * ให้สร้างทีหลังสุดเพื่อที่ Z-depth จะได้ทำงานได้ถูก
	 */
	 // Pre Calculate
	float world_dif = worlds_size[wid] - ((1 - world_ground_percen) * worlds_size[wid]);

	// Draw World Ground Circle
	glPushAttrib(GL_LIGHTING_BIT); {
		//setMyMatTempColor255(154, 204, 237, 2);
		setMyMatTempColor(0.8, 0.8, 0.3, 0.0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getMyTempColor());
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getMyTempColor());
		setMyMatTempColor255(183, 217, 133, 255);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getMyTempColor());

		//glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 20);

		//printf("%WID= d", wid);

		glPushMatrix(); {
			glTranslatef(worlds_pos[wid][0], 0.0, worlds_pos[wid][1]);
			glRotatef(90.0, 1, 0, 0);
			glScaled(worlds_size[wid] - world_margin - 0.1, worlds_size[wid] - world_margin - 0.1, worlds_size[wid] - world_margin - 0.1);
			drawSpherePercen(0.5, 1.0, 20, 100, 0);
			//drawSphereHalf(1.0,20,30);
		}glPopMatrix();
	}glPopAttrib();

	// Final Draw Assmosphere - inner
	setMyMatTempColor(0.0, 0.0, 0.0, 0.2);
	useMyTransplarent(); {
		glPushMatrix(); {
			myFixSeeTransparent();
			myEnableFontCull();

			myColorPastel(255, 199, 229, 0.75);
			//myColorPastel(255, 0, 0, 160);
			glTranslatef(worlds_pos[wid][0], 0.0, worlds_pos[wid][1]);
			glScaled(worlds_size[wid] - world_margin, worlds_size[wid] - world_margin, worlds_size[wid] - world_margin);
			drawSphere(1.0, 20, 20);

			myDisableCull();
		}glPopMatrix();
	}closeMyTransplarent();

	// Final Draw Assmosphere

	useMyTransplarent(); {
		glPushMatrix(); {
			myColorPastel(230.0, 230.0, 230.0, 150.0, 0.25);
			//glTranslatef(0.0, world_dif, 0.0);
			//glRotatef(90.0, 1, 0, 0);
			glTranslatef(worlds_pos[wid][0], 0, worlds_pos[wid][1]);
			glScaled(worlds_size[wid], worlds_size[wid], worlds_size[wid]);
			drawSphere(1.0, 20, 20);
		}glPopMatrix();
	}closeMyTransplarent();
}

void myGenerateWorld(int world_num, float galaxy_radius) {
	if (!worlds_size) {
		printf("Generate World Data\n");
		worlds_size = new float[world_num];
		world_things = new int[world_num];
		worlds_pos = new float* [world_num];
		float delta = 360.0 / (float)world_num;
		float theta = 0;
		for (int i = 0; i < world_num; i++) {
			worlds_pos[i] = new float[2];
			worlds_pos[i][0] = galaxy_radius * cos(theta * 0.0174532925f);
			worlds_pos[i][1] = galaxy_radius * sin(theta * 0.0174532925f);

			worlds_size[i] = myrand.Rand(30.0f, 50.0f);
			theta += delta;
		}
	}
}

void myDrawFountain() {
	/// Draw  fountain
	glPushAttrib(GL_LIGHTING_BIT); {
		setMyMatTempColor(1.0, 1.0, 1.0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getMyTempColor());
		setMyMatTempColor255(130, 204, 181);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getMyTempColor());
		glPushMatrix(); {
			glScaled(30, 5, 30);
			glRotated(-90, 1, 0, 0);
			float cs[][2] = {
				{0.0,0.1},{0.1,0.1},{0.1,0.3} ,{0.11,0.3},{0.11,0.0},{0,0}
			};
			drawRevolution(cs, 6, 100);
		}glPopMatrix();
	}glPopAttrib();

	// Draw Water in fountain
	useMyTransplarent(); {
		glPushMatrix(); {
			myColorPastel(154, 204, 237, 255, 0.65);
			glScaled(3, 4, 3);
			glTranslated(0, 0.15, 0);
			glRotated(-90, 1, 0, 0);
			drawCylinder(1.0, 1.0, 0.3, 10, 10);
		}glPopMatrix();
	}closeMyTransplarent();

	// Draw Upper fountain
	glPushAttrib(GL_LIGHTING_BIT); {
		setMyMatTempColor(1.0, 1.0, 1.0, 1.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, getMyTempColor());

		glPushMatrix(); {
			glTranslated(0, 1, 0);
			glScaled(2, 6, 2);
			glRotated(-90, 1, 0, 0);
			drawCylinder(1.0, 1.0, 0.3, 10, 10);
		}glPopMatrix();
	}glPopAttrib();

	// Draw Upper fountain water
	useMyTransplarent(); {
		glPushMatrix(); {
			glDepthMask(GL_TRUE);
			//myEnableBackCull();
			myColorPastel(154, 204, 237, 130, 0.65);
			glTranslated(0, 2, 0);
			glScaled(2.1, 0.5, 2.1);
			glRotated(-90, 1, 0, 0);
			drawCylinder(1.0, 1.0, 1.0, 100, 10);
			glDisable(GL_CULL_FACE);
		}glPopMatrix();
	}closeMyTransplarent();

	// Draw Water on top fountain
	useMyTransplarent(); {
		glDepthMask(GL_TRUE);
		myColorPastelBoth(154, 204, 237, 130, 0.65);
		glPushMatrix(); {
			/*
			setMyMatTempColor255(154, 204, 237,130);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getMyTempColor());
			setMyMatTempColor255(154, 204, 237, 130);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getMyTempColor());*/

			glScaled(0.7, 4, 0.7);
			glTranslated(0, 1, 0);
			glRotated(-90, 1, 0, 0);
			drawCylinder(0.3, 1.0, 1.0, 5, 10);
		}glPopMatrix();

		// Draw Toppest Water
		glPushMatrix(); {
			glTranslatef(0.0, 1.0, 0.0);
			glRotatef(-90.0, 1, 0, 0);
			glScaled(6, 6, 6);
			drawSpherePercen(0.2, 1.0, 20, 100, 0);
		}glPopMatrix();

		// Draw Upper fountain water
		glPushMatrix(); {
			glTranslated(0, 2, 0);
			glScaled(2.1, 0.5, 2.1);
			glRotated(-90, 1, 0, 0);
			drawCylinder(1.0, 1.0, 1.0, 100, 10);
		}glPopMatrix();
	}closeMyTransplarent();
}

void create_grass(int grass_num, float min_pos, float max_pos) {
	/*
	 * สั่งวาดต้นหญ้า บนพิกัดที่กำหนด
	 *
	*/
	grass_num = (!grass_num) ? 10 : grass_num;
	grass_near = 10;

	// สุ่มแผงต้นหญ้า (เหมือนการแปะ Texture ต้นหญ้า
	if (!grass_size) {
		printf("Random Grass Size\n");
		grass_size = new float[grass_near];
		for (int i = 0; i < grass_near; i++) {
			grass_size[i] = myrand.Rand(3.0f, 5.0f);
		}
	}

	// ถ้ากลุ่มหญ้าไม่ได้ถูกสร้าง ก็สร้าง พร้อมให้พิกัดมัน
	if (!grass_world2_pos) {
		printf("Random Grass Pos\n");
		grass_world2_pos = new float* [grass_num];
		for (int i = 0; i < grass_num; i++) {
			grass_world2_pos[i] = new float[2];
			grass_world2_pos[i][0] = myrand.Rand(min_pos, max_pos);
			grass_world2_pos[i][1] = myrand.Rand(min_pos, max_pos);
		}
	}

	glPushAttrib(GL_LIGHTING_BIT); {
		setMyMatTempColor255(130, 204, 181);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
		glMateriali(GL_FRONT, GL_SHININESS, 20);
		for (int j = 0; j < grass_num; j++) {
			glPushMatrix(); {
				glTranslatef(grass_world2_pos[j][0], 0.0, grass_world2_pos[j][1]);
				for (int i = 0; i < grass_near; i++) {
					glPushMatrix(); {
						glTranslated(i / 5.0, 0, 0);
						glScalef(0.5, grass_size[i], 0.1);
						glRotatef(-90, 1, 0, 0);
						drawCylinder(1.0, 0.0, 1.0, 3, 3);
					}glPopMatrix();
				}
			}glPopMatrix();
		}
	}glPopAttrib();
}

void myDrawChair(
	float m_chair_lag_h = 1.5,
	float m_chair_lag_d = 0.2,
	float m_chair_bak_ratio = 1.5,
	float m_chair_tab_h = 0.3,
	float m_chair_tab_l = 4.0,
	float m_chair_tab_w = 1.5) {
	glPushMatrix(); {
		glTranslatef(0.0f, m_chair_lag_h, 0.0f);
		glPushMatrix(); {
			glTranslatef(0.0, m_chair_tab_h / 2.0, 0);
			glScalef(m_chair_tab_l, m_chair_tab_h, m_chair_tab_w);
			drawCube(1.0);
		}glPopMatrix();
		// Draw Leg
		glPushMatrix(); {
			glTranslatef((-m_chair_tab_l / 2.0) + (m_chair_lag_d / 2.0), -m_chair_lag_h / 2.0, (m_chair_tab_w / 2.0) - (m_chair_lag_d / 2.0));
			glScalef(m_chair_lag_d, m_chair_lag_h, m_chair_lag_d);
			drawCube(1.0);
		}glPopMatrix();
		glPushMatrix(); {
			glTranslatef((m_chair_tab_l / 2.0) - (m_chair_lag_d / 2.0), -m_chair_lag_h / 2.0, (m_chair_tab_w / 2.0) - (m_chair_lag_d / 2.0));
			glScalef(m_chair_lag_d, m_chair_lag_h, m_chair_lag_d);
			drawCube(1.0);
		}glPopMatrix();
		glPushMatrix(); {
			glTranslatef((m_chair_tab_l / 2.0) - (m_chair_lag_d / 2.0), -m_chair_lag_h / 2.0, -(m_chair_tab_w / 2.0) + (m_chair_lag_d / 2.0));
			glScalef(m_chair_lag_d, m_chair_lag_h, m_chair_lag_d);
			drawCube(1.0);
		}glPopMatrix();
		glPushMatrix(); {
			glTranslatef((-m_chair_tab_l / 2.0) + (m_chair_lag_d / 2.0), -m_chair_lag_h / 2.0, -(m_chair_tab_w / 2.0) + (m_chair_lag_d / 2.0));
			glScalef(m_chair_lag_d, m_chair_lag_h, m_chair_lag_d);
			drawCube(1.0);
		}glPopMatrix();
		// Draw Backrest pole
		glPushMatrix(); {
			glTranslatef((m_chair_tab_l / 2.0) - (m_chair_lag_d / 2.0), (m_chair_lag_h * m_chair_bak_ratio / 2.0) + (m_chair_tab_h / 2.0), -(m_chair_tab_w / 2.0) + (m_chair_lag_d / 2.0));
			glScalef(m_chair_lag_d, m_chair_bak_ratio * m_chair_lag_h, m_chair_lag_d);
			drawCube(1.0);
		}glPopMatrix();
		glPushMatrix(); {
			glTranslatef((-m_chair_tab_l / 2.0) + (m_chair_lag_d / 2.0), (m_chair_lag_h * m_chair_bak_ratio / 2.0) + (m_chair_tab_h / 2.0), -(m_chair_tab_w / 2.0) + (m_chair_lag_d / 2.0));
			glScalef(m_chair_lag_d, m_chair_bak_ratio * m_chair_lag_h, m_chair_lag_d);
			drawCube(1.0);
		}glPopMatrix();
		//Draw Backrest
		glPushMatrix(); {
			glTranslatef(0.0, (m_chair_lag_h * m_chair_bak_ratio / 2.0) + (m_chair_tab_h), -(m_chair_tab_w / 2.0) + (m_chair_lag_d / 2.0));
			glRotatef(90.0, 1.0, 0, 0);
			glScalef(m_chair_tab_l, m_chair_lag_d, m_chair_tab_w);
			drawCube(1.0);
		}glPopMatrix();
	}glPopMatrix();
}

void myDrawFence(
	float m_fence_long = 20.0,
	float m_fence_d = 2.0f,
	float m_fence_w = 0.5f,
	float m_fence_h = 2.0f) {
	float m_fence_dif = fabsf(m_fence_w - m_fence_d);
	float ln = m_fence_long / (m_fence_dif);

	glPushAttrib(GL_LIGHTING_BIT);
	for (float i = 0.0; i < ln; i += 1.0) {
		glPushMatrix(); {
			setMyMatTempColor255(i * (255 / 50), 0, 0);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
			glTranslatef(0, m_fence_h / 2.0, i * -m_fence_dif);
			glScalef(0.2, m_fence_h, m_fence_w);
			drawCube(1.0);
		}glPopMatrix();
	}
	glPushMatrix(); {
		glTranslatef(0, m_fence_h / 2.0, -0.5 * ln * m_fence_dif);
		glScalef(0.2, 0.2, ln * m_fence_dif - m_fence_dif);
		drawCube(1.0);
	}glPopMatrix();
	glPopAttrib();
}

void myDrawFenceRot(float degree, float distance) {
	glPushMatrix(); {
		// เดิมที ไม่มี glTranslatef แต่ใส่มาเพราะ บัครั้ว หาองศาไม่ได้เลยต้อง Force code ไป
		glTranslatef(0, 0, distance - (1.2 * distance));
		glRotated(degree, 0.0, 1.0, 0.0);
		myDrawFence(distance);
	}glPopMatrix();
}

void myDrawBrigdeAngle(float degree, float m_bridge_long, float posX, float posY, float posZ) {
	float m_bridge_width = 5.0;

	glPushMatrix(); {
		glTranslatef(posX, posY, posZ);
		glRotatef(degree, 0.0, 1.0, 0.0);
		glPushMatrix(); {
			glTranslatef(m_bridge_width / 2.0, 0, 0);
			myDrawFenceRot(0.0, m_bridge_long);
			glTranslatef(-m_bridge_width, 0, 0);
			myDrawFenceRot(0.0, m_bridge_long);
		}glPopMatrix();

		useMyTransplarent(); {
			setMyMatTempColor255(255, 0, 0, 160);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
			myFixSeeTransparent();
			glPushMatrix(); {
				glTranslatef(0, 0, -m_bridge_long / 2.0);
				glScalef(m_bridge_width, 0.3, m_bridge_long);
				drawCube(1);
			}glPopMatrix();
		}closeMyTransplarent();
	}glPopMatrix();
}

void myDrawBrigdePoint(float x1, float z1, float x2, float z2) {
	float m_bridge_width = 5.0;
	float degree = myFindAngle(x1, z1, x2, z2);
	float distance = myVectorDistanct(x1, 0, z1, x2, 0, z2);

	//printf("DEG=%f ,%f\n", degree, distance);

	glPushMatrix(); {
		glTranslatef(x1, 0, z1);
		glRotatef(degree, 0.0, 1.0, 0.0);
		glPushMatrix(); {
			glTranslatef(m_bridge_width / 2.0, 0, 0);
			myDrawFenceRot(0, distance * 0.75);
			glTranslatef(-m_bridge_width, 0, 0);
			myDrawFenceRot(0, distance * 0.75);
		}glPopMatrix();

		useMyTransplarent(); {
			setMyMatTempColor255(255, 0, 0, 160);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
			myFixSeeTransparent();
			glPushMatrix(); {
				glTranslatef(0, 0, -distance / 2.0);
				glScalef(m_bridge_width, 0.3, distance);
				drawCube(1);
			}glPopMatrix();
		}closeMyTransplarent();
	}glPopMatrix();
}

void myDrawHome() {
	glPushAttrib(GL_LIGHTING_BIT); {
		float cl = (1.414213562 * 10.0) / 4.0;
		float sl = (1.414213562 * 15.0) / 4.0;
		setMyMatTempColor255(255, 0, 0, 255);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
		setMyMatTempColor255(233, 150, 122, 255);
		glMaterialfv(GL_FRONT, GL_AMBIENT, getMyTempColor());
		glPushMatrix(); {
			glTranslatef(0, sl, 0);
			glRotatef(90, 0, 1, 0);
			//หลังคาซ้าย
			glPushMatrix(); {
				glTranslatef(0.0, 0.0, cl);
				glRotatef(45, 1.0, 0.0, 0.0);
				glScalef(15.0, 0.2, 10.0);
				drawCube(1);
			}glPopMatrix();
			//หลังคาขวา
			glPushMatrix(); {
				glTranslatef(0.0, 0.0, -cl);
				glRotatef(-45, 1.0, 0.0, 0.0);
				glScalef(15.0, 0.2, 10.0);
				drawCube(1);
			}glPopMatrix();
		}glPopMatrix();

		// แผงปิดสามเหลี่ยม หน้า
		setMyMatTempColor255(233, 150, 122, 255);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
		glPushMatrix(); {
			glTranslatef(0, 5, -10.0 / 2.0);
			glRotatef(-90, 1, 0, 0);
			glScalef(8.0, 0.1, 8.0);
			drawCylinder(1, 0, 1, 7, 7);
		}glPopMatrix();
		// แผงปิดสามเหลี่ยม หลัง
		glPushMatrix(); {
			glTranslatef(0, 5, 10.0 / 2.0);
			glRotatef(-90, 1, 0, 0);
			glScalef(8.0, 0.1, 8.0);
			drawCylinder(1, 0, 1, 7, 7);
		}glPopMatrix();
		//วาดตัวบ้าน
		glPushMatrix(); {
			glTranslatef(0, 2.5, 0);
			glScalef(10, 5, 10);
			glRotatef(-90, 1, 0, 0);
			drawCube(1);
		}glPopMatrix();

		//วาดประตู

		setMyMatTempColor255(88, 24, 69, 255);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
		glPushMatrix(); {
			glTranslatef(0, 2, 4.8);
			glScalef(3, 4, 3);
			glRotatef(-90, 1, 0, 0);
			drawCube(1);
		}glPopMatrix();
	}glPopAttrib();
}

void myDrawFlower(float flower_h = 3.0) {
	glPushAttrib(GL_LIGHTING_BIT); {
		setMyMatTempColor255(254, 221, 16, 255);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, getMyTempColor());
		glPushMatrix(); {
			glTranslatef(0, flower_h, 0);

			glPushMatrix(); {
				glTranslatef(0, 1, 0);
				glRotatef(-90, 1, 0, 0);
				drawCylinder(0.0, 1.0, 1.0, 5, 5);
			}glPopMatrix();

			//เกสร

			glPushMatrix(); {
				glTranslatef(0.2, 1.8, 0.5);
				glScalef(0.1, 0.4, 0.1);
				glRotatef(-90, 1, 0, 0);
				drawCylinder(0.0, 1.0, 1.0, 5, 5);
			}glPopMatrix();
			glPushMatrix(); {
				glTranslatef(-0.2, 1.8, 1.2);
				glScalef(0.1, 0.4, 0.1);
				glRotatef(-90, 1, 0, 0);
				drawCylinder(0.0, 1.0, 1.0, 5, 5);
			}glPopMatrix();
		}glPopMatrix();
	}glPopAttrib();

	glPushAttrib(GL_LIGHTING_BIT); {
		// ก้าน
		setMyMatTempColor255(0, 128, 128, 255);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
		glPushMatrix(); {
			glTranslatef(0, flower_h / 2.0, 0);
			glScalef(0.1, flower_h, 0.1);
			glRotatef(-90, 1, 0, 0);
			drawCylinder(1.0, 1.0, 1.0, 5, 5);
		}glPopMatrix();
	}glPopAttrib();
}

void myCapsuldWorld(int world_ind, float r) {
	for (int i = 0; i < world_ind; i++) {
		drawSphereWorld(i, 1.0, 0.25);
	}

	//float m_degree;
	float m_x1 = 0, m_y1 = 0, m_x2 = 0, m_y2 = 0;
	for (int i = 0; i < world_ind; i++) {
		if (i == world_ind - 1) {
			// ไม่ทำแล้ว ต้องกลับไปอ่านใหม่ =w=
			/*m_degree = myFindAngle(worlds_pos[i][0], worlds_pos[i][1], worlds_pos[0][0], worlds_pos[0][1]);
			m_x1 = worlds_size[i] * cos(m_degree);
			m_y1 = worlds_size[i] * sin(m_degree);
			m_x2 = worlds_size[i] * cos(-m_degree);
			m_y2 = worlds_size[i] * sin(-m_degree);*/
			myDrawBrigdePoint(worlds_pos[i][0] + m_x1, worlds_pos[i][1] + m_y1, worlds_pos[0][0] + m_x2, worlds_pos[0][1] + m_y2);
		}
		else {
			/*m_degree = myFindAngle(worlds_pos[i][0], worlds_pos[i][1], worlds_pos[i + 1][0], worlds_pos[i + 1][1]);
			m_x1 = worlds_size[i] * cos(m_degree);
			m_y1 = worlds_size[i] * sin(m_degree);*/
			myDrawBrigdePoint(worlds_pos[i][0] + m_x1, worlds_pos[i][1] + m_y1, worlds_pos[i + 1][0], worlds_pos[i + 1][1]);
		}
	}

	// จาก Center to Worlds
	/*for (int i = 0; i < world_ind; i++) {
		myDrawBrigdePoint(0, 0, worlds_pos[i][0], worlds_pos[i][1]);
	}*/
}

void myGlessGenerator(int w_ind, int num_per_world, int grass_near) {
	// สุ่มแผงต้นหญ้า (เหมือนการแปะ Texture ต้นหญ้า
	if (!grass_size) {
		printf("Random Grass Size\n");
		grass_size = new float[grass_near];
		for (int i = 0; i < grass_near; i++) {
			grass_size[i] = myrand.Rand(3.0f, 5.0f);
		}
	}

	if (!worlds_grass_pos) {
		printf("Random Grass World POS\n");
		worlds_grass_pos = new float** [w_ind];
		for (int i = 0; i < w_ind; i++) {
			worlds_grass_pos[i] = new float* [num_per_world];
			for (int j = 0; j < num_per_world; j++) {
				worlds_grass_pos[i][j] = new float[2];
				worlds_grass_pos[i][j][0] = myrand.Rand(worlds_pos[i][0] - worlds_size[i], worlds_pos[i][0] + worlds_size[i]);
				worlds_grass_pos[i][j][1] = myrand.Rand(worlds_pos[i][1] - worlds_size[i], worlds_pos[i][1] + worlds_size[i]);

				printf("%f\n", worlds_grass_pos[i][j][0]);
			}
		}
	}
}

void myDrawGrassWorld(int wid, int w_ind) {
	glPushAttrib(GL_LIGHTING_BIT); {
		setMyMatTempColor255(130, 204, 181);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, getMyTempColor());
		glMateriali(GL_FRONT, GL_SHININESS, 20);
		for (int i = 0; i < w_ind; i++) {
			glPushMatrix(); {
				glTranslatef(worlds_grass_pos[wid][i][0], 0.0, worlds_grass_pos[wid][i][1]);
				for (int j = 0; j < grass_near; j++) {
					glPushMatrix(); {
						glTranslated(i / 5.0, 0, 0);
						glScalef(0.5, grass_size[i], 0.1);
						glRotatef(-90, 1, 0, 0);
						drawCylinder(1.0, 0.0, 1.0, 3, 3);
					}glPopMatrix();
				}
			}glPopMatrix();
		}
	}glPopAttrib();
}

void myGrassCreate(int w_ind) {
	for (int wid = 0; wid < w_ind; wid++) {
		//glPopMatrix();
		myDrawGrassWorld(wid, w_ind);
		//glPushMatrix();
	}
}

void myRandomThing(int w_ind, int n_thing) {
	if (!isRandom) {
		for (int i = 0; i < w_ind; i++) {
			world_things[i] = myrand.Rand(0, n_thing);
		}
		isRandom = true;
	}
}
void  display(void)
{
	float size = 500.0f;
	float pos[3];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	if (deltaMove)
		moveMeFlat(deltaMove);

	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}
	float cam[3] = { eyeX,eyeY,eyeZ };

	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, eyeX + objX, eyeY + objY, eyeZ + objZ, 0.0f, 1.0f, 0.0f);
	showFps();

	// เปิดให้มีการ Render 2-side face เพื่อใช้ทำ World ans asmosphere
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	// -------------  เริ่ม render วัตถุต่างๆ ภายในฉาก

	// Draw Checked-Ground
	glPushAttrib(GL_LIGHTING_BIT); {
		glPushMatrix(); {
			//drawFloor(size, size, 100, 100);
		}glPopMatrix();
	}glPopAttrib();

	int world_ind = 3;
	myGenerateWorld(world_ind, 150);
	myCapsuldWorld(world_ind, 150);
	myGlessGenerator(world_ind, 10, 10);
	myGrassCreate(world_ind);

	myRandomThing(world_ind, 4);

	glPushMatrix(); {
		glTranslatef(worlds_pos[0][0], 0, worlds_pos[0][1]);
		create_grass(20, -15.0f, 15.0f);
	}glPopMatrix();

	// Random Object into world
	for (int i = 0; i < world_ind; i++) {
		glPushMatrix(); {
			glTranslatef(worlds_pos[i][0], 0.0, worlds_pos[i][1]);
			switch (world_things[i]) {
			case 0:myDrawFountain(); break;
			case 1:myDrawChair(); break;
			case 2:myDrawFlower(); break; // ทำเป็นกลุ่มเยอะๆไม่ทัน
			case 3:myDrawHome(); break;
			}
		}glPopMatrix();
	}

	//myDrawChair();

	//glTranslated(0,-0.15,0);

	//glTranslated(0, 0.15, 0);
	//drawCylinder(1.0,1.0,0.3,10,10);

	//glTranslatef(0.0, -1.5,0.0);
	//glRotatef(-90.0, 1, 0, 0);

	   // Pop "Model view Matrix" ที่เก็บเอาไว้ออกมา เพื่อใช้สำหรับ object ถัดไป
	glPopAttrib(); // Pop คุณสมบัติเกี่ยวกับแสงที่เก็บไว้ออกมา เพื่อให้สำหรับ object ถัดไป

	//---------------------------- Sphere ------------------------

	//glPushAttrib(GL_LIGHTING_BIT); // เก็บตุณสมบัติเกี่ยวกับการสะท้อนแสงไว้ใน stack
	////glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[7]);
	////glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[7]);
	////glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[1]);
	////glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[1]);

	////glEnable(GL_COLOR_MATERIAL);
	////glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//setMatTempColor(1.0,0.0,0.3,0.5);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_tempColor);
	//
	////glColor4f(0.1, 0.0, 0.0, 0.5);
	//glColor4f(0.0, 0.0, 0.8,0.5);

	//glPushMatrix();
	//glTranslatef(0.0, 0.0, 0.0);
	//drawSphere(5.0, 30, 30);
	//glPopMatrix();
	//glPopAttrib();
	//glDisable(GL_COLOR_MATERIAL);

	//glPushAttrib(GL_LIGHTING_BIT);

	// -----------------------------------
	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[1]);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[1]);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[1]);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[1]);

	//glPushMatrix();

	//glTranslatef(-2.0, 1.0, 2.0);
	//drawSphere(1.0, 30, 30);
	//glPopMatrix();

	////---------------------------- Cylinder ------------------------
	//glPushMatrix();
	//glTranslatef(3.0, 1.0, 0.0);
	//glRotatef(-90.0, 1, 0, 0);
	//drawCylinder(2.0, 1.0, 5.0, 20, 20);
	//glPopMatrix();

	////---------------------------- Torus ------------------------
	//glPushMatrix();
	//glTranslatef(-3.0, 1.0, 0.0);
	//glRotatef(-90.0, 1, 0, 0);
	//drawTorus(0.3, 1.0, 20, 20);
	//glPopMatrix();

	//glPopAttrib(); // ดึงคุณสมบัติเรื่องการสะท้อนแสงกลับมาใช้

	//// ---------------------------------

	//glPushAttrib(GL_LIGHTING_BIT);

	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[3]);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[3]);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[3]);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[3]);

	////---------------------------- Tea Cup ------------------------

	//float cs[10][2] = {
	//	{0.0, 0.4}, {0.3, 0.5}, {0.4, 1.8}, {0.5, 1.8}, {0.3, 0.4},
	//	{0.25, 0.2}, {0.25, 0.15}, {0.5, 0.1}, {0.5, 0.0},{0.0, 0.0}
	//};

	//glPushMatrix();
	//glTranslatef(0.0, 0.0, 4.0);
	//glRotatef(-90.0, 1, 0, 0);
	//drawRevolution(cs, 10, 100);
	//glPopMatrix();

	//glPopAttrib();

	////--------------

	glDisable(GL_TEXTURE_2D);

	glFlush();
	glutSwapBuffers();
}

void  initEnvironment(void)
{
	float  light0_position[] = { 100.0, 100.0, 100.0, 0.0 };
	float  light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float  light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float  light0_ambient[] = { 0.8, 0.8, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	glClearColor(13.0 / 255.0, 191.0 / 255.0, 221.0 / 255.0, 0.0);
	//glClearColor(1.0, 1.0, 1.0, 0.0);
}

void  reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)w / h, 1, 1000);
}

void pressKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: deltaAngle = -0.01f; break;
	case GLUT_KEY_RIGHT: deltaAngle = 0.01f; break;
	case GLUT_KEY_UP: deltaMove = 1; break;
	case GLUT_KEY_DOWN: deltaMove = -1; break;
	case GLUT_KEY_SHIFT_L: eyeY += 0.1; break;
	case GLUT_KEY_ALT_L: eyeY -= 0.1; break;
	case GLUT_KEY_CTRL_L: eyeY = eyeYFirst; break;
	case GLUT_KEY_F1:isRandom = false; break;
	}
}

void releaseKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
	}
}

void  idle(void)
{
	glutPostRedisplay();
}

int  main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);

	glutInitWindowSize(640, 640);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Sample Texture");

	float xxy = myFindAngle(0, 0, 0, 0);
	printf("DEG=%f\n", xxy);
	xxy = myFindAngle(0, 0, 0, 1);
	printf("DEG=%f\n", xxy);
	xxy = myFindAngle(0, 0, 1, 0);
	printf("DEG=%f\n", xxy);
	xxy = myFindAngle(0, 0, -1, 0);
	printf("DEG=%f\n", xxy);
	xxy = myFindAngle(0, 0, 0, -1);
	printf("DEG=%f\n", xxy);

	initEnvironment();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Exit
	glutKeyboardFunc(keyboard);

	glutIdleFunc(idle);

	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	glutMainLoop();
	return 0;
}