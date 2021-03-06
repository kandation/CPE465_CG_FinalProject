#include <GL/freeglut.h>
#include <math.h>
#include "myPrimitive.h"  

static float   camera_yaw = 0.0f;
static float   camera_pitch = -20.0f;
static float   camera_distance = 8.0f;


static int     drag_mouse_r = 0;
static int     drag_mouse_l = 0;
static int     drag_mouse_m = 0;
static int     last_mouse_x, last_mouse_y;


static int     win_width, win_height;


// material ประเภทต่างๆ ในกรณีนี้มีทั้งหมด 6 ประเภท เลือกใช้โดยกำหนดเบอร์ index ของ array
//------------------------------------------------
static float mat_ambient[6][4] = {
	{ 0.0, 0.0, 0.0, 1.0 },			        // Black Plastic
	{ 0.329412, 0.223529, 0.027451, 1.0 },	//  Brass		
	{ 0.2125,0.1275,0.054,1.0},			  // Bronze
	{0.25,0.25,0.25,1.0},					  //Chrome
	{0.191125,0.0735,0.0225,1.0},			  //Copper
	{0.24725,0.1995,0.0745,1.0}				  //Gold
};
static float mat_diffuse[6][4] = {
	{ 0.01, 0.01, 0.01, 1.0 },    		        // Black Plastic
	{ 0.780392, 0.568627, 0.113725, 1.0 },	//  Brass
	{0.714,0.4284,0.18144,1.0},		  // Bronze
	{0.4,0.4,0.4,1.0},				  //Chrome
	{0.7038,0.27048,0.0828,1.0},		  //Copper
	{0.75164,0.60648,0.22648,1.0}		  //Gold
};

static float mat_specular[6][4] = {
	{ 0.5, 0.5, 0.5, 1.0 },		        // Black Plastic
	{0.992157, 0.941176, 0.807843, 1.0 },	//  Brass
	{0.393548,0.271906,0.166721,1.0},		  // Bronze
	{0.774597,0.774597,0.774597,1.0},				  //Chrome
	{0.256777,0.137622,0.086014,1.0},		  //Copper
	{0.628281,0.555802,0.366065,1.0}		  //Gold
};

static float mat_shininess[6][1] = {
	{ 32.0 },		        // Black Plastic
	{ 27.8974  },	//  Brass
	{ 25.6 },		  // Bronze
	{ 76.8 },				  //Chrome
	{ 12.8},		  //Copper
	{ 51.2 }		  //Gold
};
//------------------------------------------------


void drawObject(void) {



	//---------------------------- Sphere ------------------------
	//  ในกรณีนี้กำหนดให้ใช้ material เบอร์ 1 กับพื้นผิวชอง sphere
	// ก่อน Render วัตถุให้กำหนด material ของวัตถุก่อน 

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[1]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[1]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[1]);

	// ว้ตถุทุกชิ้นในโปรแกรมนี้จะมีจุดศูนย์กลางอยู่ที่จุด (0,0,0) ดังนั้นต้องย้ายวัตถุไปในตำแหน่งที่ต้องการก่อน 
	// และเนื่องจากเราต้องการย้ายของใครของมัน ดังนั้นก็เลยต้องหุ้มด้วย push & pop ดังนี้ 

	glPushMatrix();
	glTranslatef(3.0, 1.0, 0.0); // ในกรณีนี้ย้ายไปที่ (3,1,0)
	drawSphere(1.0, 30, 30);
	glPopMatrix();

	//---------------------------- Cylinder ------------------------
	//  ในกรณีนี้กำหนดให้ใช้ material เบอร์ 2 กับพื้นผิวชอง Cyliner
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[2]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[2]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[2]);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[2]);

	glPushMatrix();
	glTranslatef(1.0, 1.0, 0.0);
	glRotatef(-90.0, 1, 0, 0);
	drawCylinder(1.0, 1.0, 2.0, 20, 20);
	glPopMatrix();

	//---------------------------- Torus ------------------------
	//  ในกรณีนี้กำหนดให้ใช้ material เบอร์ 3 กับพื้นผิวชอง Torus
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[3]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[3]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[3]);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[3]);

	glPushMatrix();
	glTranslatef(-1.0, 1.0, -2.0);
	glRotatef(-90.0, 1, 0, 0);
	drawTorus(0.3, 1.0, 20, 20);
	glPopMatrix();

	//---------------------------- Spring ------------------------
	//  ในกรณีนี้กำหนดให้ใช้ material เบอร์ 4 กับพื้นผิวชอง spring
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[4]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[4]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[4]);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[4]);

	glPushMatrix();
	glTranslatef(-4.0, 1.0, 0.0);
	glRotatef(-90.0, 1, 0, 0);
	drawSpring(0.3, 1.0, 20.0, 20, 20, 10);
	glPopMatrix();

	//---------------------------- Tea Cup ------------------------
	//  ในกรณีนี้กำหนดให้ใช้ material เบอร์ 1 กับพื้นผิวชอง Tea cup
	float cs[10][2] = {
		{0.0, 0.4}, {0.3, 0.5}, {0.4, 1.8}, {0.5, 1.8}, {0.3, 0.4},
		{0.25, 0.2}, {0.25, 0.15}, {0.5, 0.1}, {0.5, 0.0},{0.0, 0.0}
	};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient[5]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse[5]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular[5]);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess[5]);
	glPushMatrix();
	glTranslatef(0.0, 1.0, 4.0);
	glRotatef(-90.0, 1, 0, 0);
	drawRevolution(cs, 10, 100);
	glPopMatrix();

}

void  display(void)
{
	// 3 แถวต่อจากนี้ไม่ต้องเปลี่ยนเลย เพราะเป็นการกำหนดค่าเริ่มต้นของ Model View Matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// สำหรับโปรแกรมนี้ เนื่องจากต้องการใช้เมาส์ในการหมุนกล้อง เลยกำหนดการย้ายการหมุนกล้องดังนี้ 
	// ซึ่งตรงนี้จะไปเชื่อมกับฟังก์ชัน motion() + mouse()

	glTranslatef(0.0, 0.0, -camera_distance);
	glRotatef(-camera_pitch, 1.0, 0.0, 0.0);
	glRotatef(-camera_yaw, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);


	// render ระนาบก่อน
	glPushMatrix();
	drawFloor(100, 100, 10, 10);
	glPopMatrix();

	// หลังจากนั้น render วัตถุอื่นๆ 
	// จริงๆ จะเขียนไว้ที่เดียวกันก็ได้ หรือจะแยกแบบนี้ก็ได้ 

	drawObject();

	// ฟังก์ชันนี้ไว้ทำ animation เพื่อให้ animation ไหลลื่น ต้องใส่ไว้เสมอ
	glutSwapBuffers();
}

// ฟังก์ชันนี้จะเรียกใช้ครั้งแรก และในกรณีที่ window มีการย่อหรือย้ายขนาดจาก user

void  reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); // กำหนดใช้ Projection Matrix 
	glLoadIdentity();

	// ในกรณีนี้จะเรียกใช้ฟังก์ชันนี้เพื่อกำหนดขนาดของ Frustum 
	// near = 1, far = 500  
	// ลองเปลี่ยน 45 องศา เป็นตัวเลขอื่นดู 
	// ซึ่งข้อมูลตรงนี้จะถูกไปใช้ในการกำหนด Projection Matrix 

	gluPerspective(45, (double)w / h, 1, 500);
	win_width = w;
	win_height = h;

	// แต่ถ้าใครอยากใช้เหมือนที่เรียนในห้องให้ลองใช้ฟังก์ชันนี้ดู
	// glFrustum(
		//GLdouble left,
		//GLdouble right,
		//GLdouble bottom,
		//GLdouble top,
		//GLdouble zNear,
		//GLdouble zFar
	//);
}

void  mouse(int button, int state, int mx, int my)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		drag_mouse_l = 1;
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
		drag_mouse_l = 0;
	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
		drag_mouse_r = 1;
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP))
		drag_mouse_r = 0;

	if ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN))
		drag_mouse_m = 1;
	else if ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_UP))
		drag_mouse_m = 0;

	glutPostRedisplay();

	last_mouse_x = mx;
	last_mouse_y = my;
}

void  motion(int mx, int my)
{
	if (drag_mouse_r)
	{
		camera_yaw -= (mx - last_mouse_x) * 1.0;
		if (camera_yaw < 0.0)
			camera_yaw += 360.0;
		else if (camera_yaw > 360.0)
			camera_yaw -= 360.0;

		camera_pitch -= (my - last_mouse_y) * 1.0;
		if (camera_pitch < -90.0)
			camera_pitch = -90.0;
		else if (camera_pitch > 90.0)
			camera_pitch = 90.0;
	}
	if (drag_mouse_l)
	{
		camera_distance += (my - last_mouse_y) * 0.2;
		if (camera_distance < 2.0)
			camera_distance = 2.0;
	}

	last_mouse_x = mx;
	last_mouse_y = my;

	glutPostRedisplay();
}

void  keyboard(unsigned char key, int mx, int my)
{

}

void  idle(void)
{
	glutPostRedisplay();
}

void  initEnvironment(void)
{

	// กำหนดค่าต่างๆ ของแสง
	// ตำแหน่่ง และประเภทของแสง 
	float  light0_position[] = { 10.0, 10.0, 10.0, 0.0 };
	float  light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float  light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float  light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };



	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	// กำหนดสีของพื้นหลังจอภาพ
	glClearColor(0.5, 0.5, 0.8, 0.0);
}


int  main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D Primitive shapes");

	initEnvironment();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


