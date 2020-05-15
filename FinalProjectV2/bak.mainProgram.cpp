//
//
//#include <GL/freeglut.h>
//#include <math.h>
//#include "myPrimitive.h"
//
//
//
////  ตัวแปรเกี่ยวกับกล้อง และการเคลื่อนที่ของกล้อง
//float angle = 0.0, deltaAngle = 0.0;
//float eyeX = 0.0, eyeY = 3, eyeZ = 10.0;
//float objX = 0.0, objY = 0.0, objZ = -1.0;
//
//int deltaMove = 0;
//
//
//// กำหนด material
//
//static float mat_ambient[7][4] = {
//	{ 0.0, 0.0, 0.0, 1.0 },			        // Black Plastic
//	{ 0.329412, 0.223529, 0.027451, 1.0 },	//  Brass
//	{ 0.2125,0.1275,0.054,1.0},			  // Bronze
//	{0.25,0.25,0.25,1.0},					  //Chrome
//	{0.191125,0.0735,0.0225,1.0},			  //Copper
//	{0.24725,0.1995,0.0745,1.0},				  //Gold
//	{1.0,1.0,1.0,1.0}
//};
//static float mat_diffuse[7][4] = {
//	{ 0.01, 0.01, 0.01, 1.0 },
//	{ 0.780392, 0.568627, 0.113725, 1.0 },
//	{0.714,0.4284,0.18144,1.0},
//	{0.4,0.4,0.4,1.0},
//	{0.7038,0.27048,0.0828,1.0},
//	{0.75164,0.60648,0.22648,1.0},
//	{1.0,1.0,1.0,1.0}
//};
//
//static float mat_specular[7][4] = {
//	{ 0.5, 0.5, 0.5, 1.0 },
//	{0.992157, 0.941176, 0.807843, 1.0 },
//	{0.393548,0.271906,0.166721,1.0},
//	{0.774597,0.774597,0.774597,1.0},
//	{0.256777,0.137622,0.086014,1.0},
//	{0.628281,0.555802,0.366065,1.0},
//	{1.0,1.0,1.0,1.0}
//};
//
//static float mat_shininess[7][1] = {
//	{ 32.0 },
//	{ 27.8974  },
//	{ 25.6 },
//	{ 76.8 },
//	{ 12.8},
//	{ 51.2 },
//	{1.0}
//};
//
//// ฟังก์ชันกำหนดการเคลื่อนไหวของกล้อง
//void orientMe(float ang) {
//	objX = sin(ang);
//	objZ = -cos(ang);
//}
//void moveMeFlat(int i) {
//	eyeX = eyeX + i * (objX) * 0.1;
//	eyeZ = eyeZ + i * (objZ) * 0.1;
//}
//void Lookup(int i) {
//	objY += 0.01 * i;
//}
//
//
//
//void  display(void)
//{
//
//	float size = 500.0f;
//	float pos[3];
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glMatrixMode(GL_MODELVIEW);
//
//	if (deltaMove)
//		moveMeFlat(deltaMove);
//
//	if (deltaAngle) {
//		angle += deltaAngle;
//		orientMe(angle);
//	}
//	float cam[3] = { eyeX,eyeY,eyeZ };
//
//	glLoadIdentity();
//	gluLookAt(eyeX, eyeY, eyeZ, eyeX + objX, eyeY + objY, eyeZ + objZ, 0.0f, 1.0f, 0.0f);
//
//
//	// -------------  เริ่ม render วัตถุต่างๆ ภายในฉาก
//
//	glPushAttrib(GL_LIGHTING_BIT); // Push คุณสมบัติเกี่ยวกับแสงเก็บไว้ก่อน render วัตถุ
//	glPushMatrix();   // Push "Model view Matrix" เก็บเอาไว้
//
//	drawFloor(size, size, 100, 100);
//
//	glPopMatrix();   // Pop "Model view Matrix" ที่เก็บเอาไว้ออกมา เพื่อใช้สำหรับ object ถัดไป
//	glPopAttrib(); // Pop คุณสมบัติเกี่ยวกับแสงที่เก็บไว้ออกมา เพื่อให้สำหรับ object ถัดไป
//
//
//
//
//	//---------------------------- Sphere ------------------------
//
//	glPushAttrib(GL_LIGHTING_BIT); // เก็บตุณสมบัติเกี่ยวกับการสะท้อนแสงไว้ใน stack
//
//	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[1]);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[1]);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[1]);
//	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[1]);
//
//	glPushMatrix();
//	glTranslatef(0.0, 1.0, 0.0);
//	drawSphere(1.0, 30, 30);
//	glPopMatrix();
//
//
//
//	// -----------------------------------
//
//	glPushMatrix();
//	glTranslatef(-2.0, 1.0, 2.0);
//	drawSphere(1.0, 30, 30);
//	glPopMatrix();
//
//
//	//---------------------------- Cylinder ------------------------
//	glPushMatrix();
//	glTranslatef(3.0, 1.0, 0.0);
//	glRotatef(-90.0, 1, 0, 0);
//	drawCylinder(1.0, 1.0, 2.0, 20, 20);
//	glPopMatrix();
//
//	//---------------------------- Torus ------------------------
//	glPushMatrix();
//	glTranslatef(-3.0, 1.0, 0.0);
//	glRotatef(-90.0, 1, 0, 0);
//	drawTorus(0.3, 1.0, 20, 20);
//	glPopMatrix();
//
//
//	//---------------------------- Wall  ------------------------
//	glPushMatrix();
//	glTranslatef(0.0, 3.0, -2.0);
//	glRotatef(90.0, 1, 0, 0);
//	drawSpring(0.3, 1.0, 20.0, 20, 20, 10);
//	glPopMatrix();
//
//	glPopAttrib(); // ดึงคุณสมบัติเรื่องการสะท้อนแสงกลับมาใช้
//
//
//	// ---------------------------------
//
//	glPushAttrib(GL_LIGHTING_BIT);
//
//	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[3]);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[3]);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[3]);
//	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[3]);
//
//	//---------------------------- Tea Cup ------------------------
//
//	float cs[10][2] = {
//		{0.0, 0.4}, {0.3, 0.5}, {0.4, 1.8}, {0.5, 1.8}, {0.3, 0.4},
//		{0.25, 0.2}, {0.25, 0.15}, {0.5, 0.1}, {0.5, 0.0},{0.0, 0.0}
//	};
//
//	glPushMatrix();
//	glTranslatef(0.0, 0.0, 4.0);
//	glRotatef(-90.0, 1, 0, 0);
//	drawRevolution(cs, 10, 100);
//	glPopMatrix();
//
//	glPopAttrib();
//
//	//--------------
//
//
//
//	glDisable(GL_TEXTURE_2D);
//
//
//	glFlush();
//	glutSwapBuffers();
//}
//
//
//
//void  initEnvironment(void)
//{
//	float  light0_position[] = { 10.0, 10.0, 10.0, 0.0 };
//	float  light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//	float  light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//	float  light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
//
//
//	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
//	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
//
//
//
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//
//
//	glEnable(GL_DEPTH_TEST);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glCullFace(GL_BACK);
//	glEnable(GL_CULL_FACE);
//
//
//	glEnable(GL_NORMALIZE);
//	glShadeModel(GL_SMOOTH);
//
//	glClearColor(0.5, 0.5, 0.8, 0.0);
//
//}
//
//void  reshape(int w, int h)
//{
//	glViewport(0, 0, w, h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(45, (double)w / h, 1, 1000);
//
//}
//
//
//
//void pressKey(int key, int x, int y) {
//
//	switch (key) {
//	case GLUT_KEY_LEFT: deltaAngle = -0.01f; break;
//	case GLUT_KEY_RIGHT: deltaAngle = 0.01f; break;
//	case GLUT_KEY_UP: deltaMove = 1; break;
//	case GLUT_KEY_DOWN: deltaMove = -1; break;
//
//	}
//}
//
//void releaseKey(int key, int x, int y) {
//
//	switch (key) {
//	case GLUT_KEY_LEFT:
//	case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
//	case GLUT_KEY_UP:
//	case GLUT_KEY_DOWN: deltaMove = 0; break;
//
//	}
//}
//
//void  idle(void)
//{
//	glutPostRedisplay();
//}
//
//int  main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
//	glutInitWindowSize(640, 640);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("Sample Texture");
//
//	initEnvironment();
//
//	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	glutIdleFunc(idle);
//	glutSpecialFunc(pressKey);
//	glutSpecialUpFunc(releaseKey);
//
//
//	glutMainLoop();
//	return 0;
//}