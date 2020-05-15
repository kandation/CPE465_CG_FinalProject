#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846

//-------------------------------------------------------------------
void drawFloor(double widthX, double widthZ, int nx, int nz)
{
	int i, j;
	//Floor
	double wX = widthX / (double)nx;
	double wZ = widthZ / (double)nz;

	static float diffuse[][4] = {
	  { 0.9f, 0.6f, 0.3f, 1.0f}, { 0.3f, 0.5f, 0.8, 1.0f} };
	static float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	static float specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);

	glNormal3d(0.0, 1.0, 0.0);
	glPushMatrix();
	glBegin(GL_QUADS);
	for (j = 0; j < nz; j++) {
		double z1 = -widthZ / 2.0 + wZ * j; double z2 = z1 + wZ;
		for (i = 0; i < nx; i++) {
			double x1 = -widthX / 2.0 + wX * i; double x2 = x1 + wX;

			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[(i + j) & 1]);
			glVertex3d(x1, 0.0, z1);
			glVertex3d(x1, 0.0, z2);
			glVertex3d(x2, 0.0, z2);
			glVertex3d(x2, 0.0, z1);
		}
	}
	glEnd();
	glPopMatrix();
}

//-----------------------------------------------------------------------

void drawCube(float s)
{
	float p[8][3] =
	{ {0.5 * s,0.5 * s,0.5 * s}, {-0.5 * s,0.5 * s,0.5 * s}, {-0.5 * s,-0.5 * s,0.5 * s},
	  {0.5 * s,-0.5 * s,0.5 * s},{0.5 * s,0.5 * s,-0.5 * s}, {-0.5 * s,0.5 * s,-0.5 * s},
	  {-0.5 * s,-0.5 * s,0 - 0.5 * s}, {0.5 * s,-0.5 * s,-0.5 * s}
	};

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3fv(p[0]); glVertex3fv(p[1]);
	glVertex3fv(p[2]); glVertex3fv(p[3]);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3fv(p[0]); glVertex3fv(p[3]);
	glVertex3fv(p[7]); glVertex3fv(p[4]);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3fv(p[0]); glVertex3fv(p[4]);
	glVertex3fv(p[5]); glVertex3fv(p[1]);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3fv(p[1]); glVertex3fv(p[5]);
	glVertex3fv(p[6]); glVertex3fv(p[2]);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3fv(p[2]); glVertex3fv(p[6]);
	glVertex3fv(p[7]); glVertex3fv(p[3]);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3fv(p[4]); glVertex3fv(p[7]);
	glVertex3fv(p[6]); glVertex3fv(p[5]);
	glEnd();
}

//-----------------------------------------------------------------------
void drawSphere(double radius, int nSlice, int nStack)
{
	int i, j;
	double phi;
	double theta;
	float p[31][31][3];
	float* p1, * p2, * p3, * p4;

	if (nSlice > 30) nSlice = 30;
	if (nStack > 30) nStack = 30;

	for (i = 0; i <= nSlice; i++)
	{
		phi = 2.0 * M_PI * (double)i / (double)nSlice;
		for (j = 0; j <= nStack; j++)
		{
			theta = M_PI * (double)j / (double)nStack;
			p[i][j][0] = (float)(radius * sin(theta) * cos(phi));
			p[i][j][1] = (float)(radius * sin(theta) * sin(phi));
			p[i][j][2] = (float)(radius * cos(theta));
		}
	}
	//Top(j=0)
	for (i = 0; i < nSlice; i++)
	{
		p1 = p[i][0];	  p2 = p[i][1];
		p3 = p[i + 1][1];
		glBegin(GL_TRIANGLES);
		glNormal3fv(p1); glVertex3fv(p1);
		glNormal3fv(p2); glVertex3fv(p2);
		glNormal3fv(p3); glVertex3fv(p3);
		glEnd();
	}
	//Bottom
	j = nStack - 1;
	for (i = 0; i < nSlice; i++)
	{
		p1 = p[i][j];	  p2 = p[i][j + 1];
		p3 = p[i + 1][j];
		glBegin(GL_TRIANGLES);
		glNormal3fv(p1); glVertex3fv(p1);
		glNormal3fv(p2); glVertex3fv(p2);
		glNormal3fv(p3); glVertex3fv(p3);
		glEnd();
	}

	for (i = 0; i < nSlice; i++) {
		for (j = 1; j < nStack - 1; j++)
		{
			p1 = p[i][j];	  p2 = p[i][j + 1];
			p3 = p[i + 1][j + 1]; p4 = p[i + 1][j];
			glBegin(GL_QUADS);
			glNormal3fv(p1); glVertex3fv(p1);
			glNormal3fv(p2); glVertex3fv(p2);
			glNormal3fv(p3); glVertex3fv(p3);
			glNormal3fv(p4); glVertex3fv(p4);
			glEnd();
		}
	}
}
//-----------------------------------------------------------------------
void drawSphereHalf(double radius, int nSlice, int nStack)
{
	int i, j;
	double phi;
	double theta;
	float p[31][31][3];
	float* p1, * p2, * p3, * p4;

	if (nSlice > 30) nSlice = 30;
	if (nStack > 30) nStack = 30;

	for (i = 0; i <= nSlice; i++)
	{
		phi = 2.0 * M_PI * (double)i / (double)nSlice;
		for (j = 0; j <= nStack; j++)
		{
			theta = M_PI * (double)j / (double)nStack;
			p[i][j][0] = (float)(radius * sin(theta) * cos(phi));
			p[i][j][1] = (float)(radius * sin(theta) * sin(phi));
			p[i][j][2] = (float)(radius * cos(theta));
		}
	}
	//Top(j=0)
	for (i = 0; i < nSlice; i++)
	{
		p1 = p[i][0];	  p2 = p[i][1];
		p3 = p[i + 1][1];
		glBegin(GL_TRIANGLES);
		glNormal3fv(p1); glVertex3fv(p1);
		glNormal3fv(p2); glVertex3fv(p2);
		glNormal3fv(p3); glVertex3fv(p3);
		glEnd();
	}
	//Bottom
	j = nStack - 1;
	j = ((nStack) / 2.0) - 1;
	for (i = 0; i < nSlice; i++)
	{
		p1 = p[i][j];	  p2 = p[i][j + 1];
		p3 = p[i + 1][j];
		glBegin(GL_TRIANGLES);
		glNormal3fv(p1); glVertex3fv(p1);
		glNormal3fv(p3); glVertex3fv(p3);
		glNormal3f(0, 0, 0);
		glVertex3f(0, 0, 0);
		glEnd();
	}

	for (i = 0; i < nSlice; i++) {
		for (j = 1; j < nStack / 2.0; j++)
		{
			p1 = p[i][j];
			p2 = p[i][j + 1];
			p3 = p[i + 1][j + 1];
			p4 = p[i + 1][j];
			glBegin(GL_QUADS);
			glNormal3fv(p1); glVertex3fv(p1);
			glNormal3fv(p2); glVertex3fv(p2);
			glNormal3fv(p3); glVertex3fv(p3);
			glNormal3fv(p4); glVertex3fv(p4);
			glEnd();
		}
	}
}
//-----------------------------------------------------------------------
void drawSpherePercen(double per, double radius, int nSlice, int nStack, float offset = 0.0)
{
	int i, j;
	double phi;
	double theta;
	float p[31][31][3];
	float* p1, * p2, * p3, * p4;

	if (nSlice > 30) nSlice = 30;
	if (nStack > 30) nStack = 30;
	if (per > 1.0) per = 1.0;
	if (per < 0.0) per = 0.0;

	for (i = 0; i <= nSlice; i++)
	{
		phi = 2.0 * M_PI * (double)i / (double)nSlice;
		for (j = 0; j <= nStack; j++)
		{
			theta = M_PI * (double)j / (double)nStack;
			p[i][j][0] = (float)(radius * sin(theta) * cos(phi));
			p[i][j][1] = (float)(radius * sin(theta) * sin(phi));
			p[i][j][2] = (float)(radius * cos(theta));
		}
	}
	//Top(j=0)
	for (i = 0; i < nSlice; i++)
	{
		p1 = p[i][0];	  p2 = p[i][1];
		p3 = p[i + 1][1];
		glBegin(GL_TRIANGLES);
		glNormal3fv(p1); glVertex3fv(p1);
		glNormal3fv(p2); glVertex3fv(p2);
		glNormal3fv(p3); glVertex3fv(p3);
		glEnd();
	}
	//Bottom
	j = nStack - 1;
	j = ((nStack)*per) + offset;

	for (i = 0; i < nSlice; i++)
	{
		p1 = p[i][j];	  p2 = p[i][j + 1];
		p3 = p[i + 1][j];
		glBegin(GL_TRIANGLES);
		glNormal3fv(p1); glVertex3fv(p1);
		glNormal3fv(p3); glVertex3fv(p3);
		glNormal3f(0, 0, p3[2]);
		glVertex3f(0, 0, p3[2]);
		glEnd();
	}

	for (i = 0; i < nSlice; i++) {
		for (j = 1; j < nStack * per; j++)
		{
			p1 = p[i][j];
			p2 = p[i][j + 1];
			p3 = p[i + 1][j + 1];
			p4 = p[i + 1][j];
			glBegin(GL_QUADS);
			glNormal3fv(p1); glVertex3fv(p1);
			glNormal3fv(p2); glVertex3fv(p2);
			glNormal3fv(p3); glVertex3fv(p3);
			glNormal3fv(p4); glVertex3fv(p4);
			glEnd();
		}
	}
}

//----------------------------------------------------------------------
void drawCylinder(float rBottom, float rTop, float height, int nSlice, int nStack)
{
	//	float p[961][3];
	float p[31][31][3];
	float n[30][3];
	double theta, z, r;
	int i, ii, j;

	if (nSlice > 31) nSlice = 30;
	if (nStack > 31) nStack = 30;

	double theta0 = 2 * M_PI / (double)nSlice;
	for (j = 0; j <= nStack; j++)
	{
		z = height * (1.0 - (double)j / (double)nStack);
		r = rBottom + (rTop - rBottom) * z / height;
		for (i = 0; i < nSlice; i++)
		{
			theta = theta0 * (double)i;
			p[i][j][0] = (float)(r * cos(theta));
			p[i][j][1] = (float)(r * sin(theta));
			p[i][j][2] = (float)z - 0.5f * height;
		}
	}
	//(Top)
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for (i = 0; i < nSlice; i++)
	{
		ii = i + 1;
		if (ii == nSlice) ii = 0;
		glVertex3f(0.0f, 0.0f, height / 2.0f);
		glVertex3fv(p[i][0]);
		glVertex3fv(p[ii][0]);
	}
	glEnd();
	//(Bottom)
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0f, 0.0f, -1.0f);
	for (i = nSlice - 1; i >= 0; i--)
	{
		ii = i - 1;
		if (i == 0) ii = nSlice - 1;
		glVertex3f(0.0f, 0.0f, -height / 2.0f);
		glVertex3fv(p[i][nStack]);
		glVertex3fv(p[ii][nStack]);
	}
	glEnd();

	double nz, nxy, rr;
	rr = rBottom - rTop;
	nz = rr / sqrt(rr * rr + height * height);
	nxy = sqrt(1.0 - nz * nz);
	for (i = 0; i < nSlice; i++)
	{
		n[i][0] = (float)(nxy * cos(theta0 * (double)i));
		n[i][1] = (float)(nxy * sin(theta0 * (double)i));
		n[i][2] = (float)nz;
	}

	for (j = 0; j < nStack; j++)
		for (i = 0; i < nSlice; i++)
		{
			ii = i + 1;
			if (ii == nSlice) ii = 0;

			glBegin(GL_QUADS);
			glNormal3fv(n[i]);
			glVertex3fv(p[i][j]);
			glVertex3fv(p[i][j + 1]);
			glNormal3fv(n[ii]);
			glVertex3fv(p[ii][j + 1]);
			glVertex3fv(p[ii][j]);
			glEnd();
		}
}

//-----------------------------------------------------------------------------------------

void drawTorus(float radius1, float radius2, int nSide, int nRing)
{
	int i, ii, j, jj;
	double phi, ph1, ph2;
	double theta, th1, th2;
	float rr[31], zz[31];
	float p[961][3];

	if (radius1 > radius2) { printf("radius1 < radius2\n "); return; }
	if (nRing > 30) nRing = 30;
	if (nSide > 30) nSide = 30;

	for (j = 0; j < nSide; j++)
	{
		theta = M_PI - 2.0 * M_PI * (double)j / (double)nSide;
		rr[j] = (float)(radius2 + radius1 * (float)cos(theta));
		zz[j] = (float)(radius1 * (float)sin(theta));
	}
	for (i = 0; i < nRing; i++)
	{
		phi = 2.0 * M_PI * (double)i / (double)nRing;
		for (j = 0; j < nSide; j++)
		{
			p[j * nRing + i][0] = rr[j] * (float)cos(phi);
			p[j * nRing + i][1] = rr[j] * (float)sin(phi);
			p[j * nRing + i][2] = zz[j];
		}
	}

	double s1, s2;

	for (i = 0; i < nRing; i++)
	{
		ii = i + 1;
		if (ii == nRing) ii = 0;
		s1 = (double)i / (double)nRing; s2 = (double)(ii) / (double)nRing;
		ph1 = 2.0 * M_PI * s1;
		ph2 = 2.0 * M_PI * s2;
		for (j = 0; j < nSide; j++)
		{
			jj = j + 1;
			if (jj == nSide) jj = 0;
			th1 = M_PI - 2.0 * M_PI * (double)j / (double)nSide;
			th2 = M_PI - 2.0 * M_PI * (double)jj / (double)nSide;
			glBegin(GL_QUADS);
			glNormal3d(cos(th1) * cos(ph1), cos(th1) * sin(ph1), sin(th1));
			glVertex3fv(p[j * nRing + i]);
			glNormal3d(cos(th2) * cos(ph1), cos(th2) * sin(ph1), sin(th2));
			glVertex3fv(p[jj * nRing + i]);
			glNormal3d(cos(th2) * cos(ph2), cos(th2) * sin(ph2), sin(th2));
			glVertex3fv(p[jj * nRing + ii]);
			glNormal3d(cos(th1) * cos(ph2), cos(th1) * sin(ph2), sin(th1));
			glVertex3fv(p[j * nRing + ii]);
			glEnd();
		}
	}
}

//----------------------------------------------------------------
void drawSpring(float radius1, float radius2, float len, int nSide, int nRing, int nPitch)
{
	int i, ii, j, jj, k, k1, k2;
	double phi, ph1, ph2;
	double theta, th1, th2;
	float p[21][21][51][3];
	float p1[3], p2[3], p3[3], p4[3];
	float rr[21], zz[21];
	float pitch, dp, hh;

	if (radius2 < radius1) {
		printf("drawSpring radius2 > radius1\n");
	}
	if (nRing > 20) nRing = 20;
	if (nSide > 20) nSide = 20;
	if (nPitch > 50) nPitch = 50;
	pitch = len / (float)nPitch;
	if (pitch < 2 * radius1) pitch = radius1 * 2.0;
	dp = (float)pitch / (float)nRing;

	for (j = 0; j < nSide; j++)
	{
		theta = M_PI - 2.0 * M_PI * (double)j / (double)nSide;
		rr[j] = radius2 + radius1 * (float)cos(theta);
		zz[j] = radius1 * (float)sin(theta);
	}
	hh = 0;
	for (k = 0; k < nPitch; k++)
		for (i = 0; i < nRing; i++)
		{
			phi = 2.0 * M_PI * (double)i / (double)nRing;
			for (j = 0; j < nSide; j++)
			{
				p[i][j][k][0] = rr[j] * (float)cos(phi);
				p[i][j][k][1] = rr[j] * (float)sin(phi);
				p[i][j][k][2] = zz[j] + hh;
			}
			hh += dp;
		}
	k = nPitch - 1; i = nRing;
	for (j = 0; j < nSide; j++) {
		phi = 0.0;
		p[i][j][k][0] = rr[j] * (float)cos(phi);
		p[i][j][k][1] = rr[j] * (float)sin(phi);
		p[i][j][k][2] = zz[j] + hh;
	}
	for (k = 0; k < nPitch; k++)
		for (i = 0; i < nRing; i++) {
			ii = i + 1;
			k1 = k; k2 = k;
			if (ii == nRing) {
				if (k < nPitch - 1) { ii = 0; k2 = k + 1; }
			}
			ph1 = 2.0 * M_PI * (double)i / (double)nRing;
			ph2 = 2.0 * M_PI * (double)ii / (double)nRing;
			for (j = 0; j < nSide; j++)
			{
				jj = j + 1;
				if (jj == nSide) jj = 0;
				th1 = M_PI - 2.0 * M_PI * (double)j / (double)nSide;
				th2 = M_PI - 2.0 * M_PI * (double)jj / (double)nSide;

				p1[0] = p[i][j][k1][0]; p1[1] = p[i][j][k1][1]; p1[2] = p[i][j][k1][2];
				p2[0] = p[i][jj][k1][0]; p2[1] = p[i][jj][k1][1]; p2[2] = p[i][jj][k1][2];
				p3[0] = p[ii][jj][k2][0]; p3[1] = p[ii][jj][k2][1]; p3[2] = p[ii][jj][k2][2];
				p4[0] = p[ii][j][k2][0]; p4[1] = p[ii][j][k2][1]; p4[2] = p[ii][j][k2][2];

				glBegin(GL_QUADS);
				glNormal3d(cos(th1) * cos(ph1), cos(th1) * sin(ph1), sin(th1)); glVertex3fv(p1);
				glNormal3d(cos(th2) * cos(ph1), cos(th2) * sin(ph1), sin(th2)); glVertex3fv(p2);
				glNormal3d(cos(th2) * cos(ph2), cos(th2) * sin(ph2), sin(th2)); glVertex3fv(p3);
				glNormal3d(cos(th1) * cos(ph2), cos(th1) * sin(ph2), sin(th1)); glVertex3fv(p4);
				glEnd();
			}
		}

	glBegin(GL_POLYGON);
	glNormal3d(0.0, -1.0, 0.0);
	for (j = nSide - 1; j >= 0; j--) glVertex3fv(p[0][j][0]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3d(0.0, 1.0, 0.0);
	for (j = 0; j < nSide; j++) glVertex3fv(p[nRing][j][nPitch - 1]);
	glEnd();
}
//---------------------------------------------------------------------------
void drawRevolution(float cs[][2], int nSide, int nSlice)
{
	int i, ii, j, jj, jm, jp;
	double phi;
	float p[31][31][3];
	float n0[30][2];
	float n1[30][2];
	float nn[31][31][3];
	double len, dx, dz;

	if (nSide > 30) { printf("nSide < 30\n"); }//exit(1); }
	if (nSlice > 30) nSlice = 30;

	int nSide2;
	if (cs[0][0] == 0.0 && cs[nSide - 1][0] == 0.0)
		nSide2 = nSide - 1;
	else nSide2 = nSide;

	for (j = 0; j < nSide; j++)
	{
		jp = j + 1;
		if (jp == nSide) jp = 0;
		dx = (double)(cs[jp][0] - cs[j][0]);
		dz = (double)(cs[jp][1] - cs[j][1]);
		len = sqrt(dx * dx + dz * dz);
		n0[j][0] = -dz / len;
		n0[j][1] = dx / len;
	}

	if (cs[0][0] == 0.0 && cs[nSide - 1][0] == 0.0)
	{
		n1[0][0] = 0.0; n1[0][1] = 1.0;
		n1[nSide - 1][0] = 0.0; n1[nSide - 1][1] = -1.0;
		for (j = 1; j < nSide - 1; j++)
		{
			n1[j][0] = (n0[j - 1][0] + n0[j][0]) / 2.0;
			n1[j][1] = (n0[j - 1][1] + n0[j][1]) / 2.0;
		}
	}
	else
	{
		for (j = 0; j < nSide; j++)
		{
			jm = j - 1;
			if (j == 0) jm = nSide - 1;
			n1[j][0] = (n0[jm][0] + n0[j][0]) / 2.0;
			n1[j][1] = (n0[jm][1] + n0[j][1]) / 2.0;
		}
	}

	for (i = 0; i < nSlice; i++)
	{
		phi = 2.0 * M_PI * (double)i / (double)nSlice;
		for (j = 0; j < nSide; j++)
		{
			p[i][j][0] = cs[j][0] * (float)cos(phi);
			p[i][j][1] = cs[j][0] * (float)sin(phi);
			p[i][j][2] = cs[j][1];
			nn[i][j][0] = n1[j][0] * (float)cos(phi);
			nn[i][j][1] = n1[j][0] * (float)sin(phi);
			nn[i][j][2] = n1[j][1];
		}
	}

	for (i = 0; i < nSlice; i++)
	{
		ii = i + 1;
		if (ii == nSlice) ii = 0;
		for (j = 0; j < nSide2; j++)
		{
			jj = j + 1;

			if (jj == nSide) jj = 0;
			glBegin(GL_QUADS);
			glNormal3fv(nn[i][j]);  glVertex3fv(p[i][j]);
			glNormal3fv(nn[i][jj]); glVertex3fv(p[i][jj]);
			glNormal3fv(nn[ii][jj]); glVertex3fv(p[ii][jj]);
			glNormal3fv(nn[ii][j]); glVertex3fv(p[ii][j]);
			glEnd();
		}
	}
}