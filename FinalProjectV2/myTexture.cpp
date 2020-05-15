#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846

void drawTexPlate(float s)
{
	float p[4][3] = 
	{ { -s/2.0, 0.0,s/2.0}, { s/2.0,0.0, s/2.0}, { s/2.0,0.0, -s/2.0}, { -s/2.0,0.0, -s/2.0}};

	glEnable(GL_TEXTURE_2D);

	/*
	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0); 
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(p[0]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(p[1]);
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(p[2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(p[3]);
	glEnd();
	*/

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(p[0]);
	glTexCoord2f(4.0f, 0.0f); glVertex3fv(p[1]);
	glTexCoord2f(4.0f, 4.0f); glVertex3fv(p[2]);
	glTexCoord2f(0.0f, 4.0f); glVertex3fv(p[3]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//-----------------------------------------------------------------------
void drawTexCube(float s)
{
	float p[8][3] =
	{ {0.5*s,0.5*s,0.5*s}, {-0.5*s,0.5*s,0.5*s}, {-0.5*s,-0.5*s,0.5*s}, 
	  {0.5*s,-0.5*s,0.5*s},{0.5*s,0.5*s,-0.5*s}, {-0.5*s,0.5*s,-0.5*s},
	  {-0.5*s,-0.5*s,0-0.5*s}, {0.5*s,-0.5*s,-0.5*s}
	};

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f); 
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(p[0]);
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(p[1]);
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(p[2]);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(p[3]);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(1.0f,0.0f,0.0f); 
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(p[0]);
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(p[3]);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(p[7]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(p[4]);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0f,1.0f,0.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(p[0]);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(p[4]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(p[5]);
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(p[1]);
	glEnd();

	glBegin(GL_QUADS);
	 	glNormal3f(-1.0f,0.0f,0.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(p[6]);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(p[2]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(p[1]);
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(p[5]);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0f,-1.0f,0.0f); 
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(p[2]);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(p[6]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(p[7]);
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(p[3]);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,-1.0f); 
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(p[4]);
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(p[7]);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(p[6]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(p[5]);
        glEnd();
	glDisable(GL_TEXTURE_2D);
}


void drawTexSphere(float radius, int nSlice, int nStack)
{
	int i, ii, j;
	double phi; 
	double theta; 
	float p[31][31][3]; 
	float *p1,*p2,*p3,*p4;
	float th1, th2, ph1, ph2;

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;

	for(i = 0;i <= nSlice;i++)
	{	
		phi = 2.0 * M_PI * (double)i / (double)nSlice;
		for(j=0;j<=nStack;j++)
		{	
			theta = M_PI * (double)j / (double)nStack;
			p[i][j][0] = -(float)(radius * sin(theta) * cos(phi));
			p[i][j][1] = -(float)(radius * sin(theta) * sin(phi));
			p[i][j][2] = (float)(radius * cos(theta));            
		}
	}
	glEnable(GL_TEXTURE_2D);
	for(i = 0;i < nSlice;i++)
	{
		ii = i + 1;
		th1 = (float)i / (float)nSlice;
		th2 = (float)ii / (float)nSlice;
		if(i == nSlice-1) th2 = 1.0;
		for(j = 0;j < nStack; j++)
		{
			ph1 = 1.0f - (float)j / (float)nStack;
			ph2 = 1.0f - (float)(j+1) / (float)nStack;
			p1 = p[i][j];	 p2 = p[i][j+1];
			p3 = p[ii][j+1]; p4 = p[ii][j];
			glBegin(GL_QUADS);
				glNormal3fv(p1);glTexCoord2f(th1, ph1);glVertex3fv(p1);
				glNormal3fv(p2);glTexCoord2f(th1, ph2);glVertex3fv(p2);
				glNormal3fv(p3);glTexCoord2f(th2, ph2);glVertex3fv(p3);
				glNormal3fv(p4);glTexCoord2f(th2, ph1);glVertex3fv(p4);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
}

//----------------------------------------------------------------------
void drawTexCylinder(float rBottom, float rTop, float height, int nSlice, int nStack)
{
	float p[31][31][3];
	float n[30][3];
	double theta0, theta, z, r;
	int i, ii, j;

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;
	
	theta0 = 2*M_PI/(double)nSlice;
	for(j=0; j<=nStack; j++)
	{
		z = height * (1.0 - (double)j/(double)nStack);
		r = rBottom + (rTop - rBottom) * z / height;
		for(i=0;i<nSlice;i++)
		{
			theta = theta0*(double)i;
			p[i][j][0] = (float)(r * cos(theta)); 
			p[i][j][1] = (float)(r * sin(theta)); 
			p[i][j][2] = (float)z-0.5f*height;    
		}
	}
	glBegin(GL_TRIANGLES);
		glNormal3f(0.0f,0.0f,1.0f);
		for(i = 0; i < nSlice; i++) 
		{ 
			ii = i + 1;
			if(ii == nSlice) ii = 0;
			glVertex3f(0.0f, 0.0f, height/2.0f);
			glVertex3fv(p[i][0]);
			glVertex3fv(p[ii][0]);
		}
	glEnd();

	glBegin(GL_TRIANGLES);
		glNormal3f(0.0f,0.0f,-1.0f); 
		for(i = nSlice-1; i >= 0; i--) 
		{
			ii = i - 1;
			if(i == 0) ii = nSlice - 1;
			glVertex3f(0.0f, 0.0f, -height/2.0f);
			glVertex3fv(p[i][nStack]);
			glVertex3fv(p[ii][nStack]);
		}
	glEnd();

	double nz, nxy, rr;
	rr = rBottom - rTop;
	nz = rr / sqrt(rr*rr + height * height);
	nxy = sqrt(1.0 - nz*nz);
	for(i=0; i<nSlice;i++)
	{
		n[i][0] = (float)(nxy * cos(theta0 * (double)i));
		n[i][1] = (float)(nxy * sin(theta0 * (double)i));
		n[i][2] = (float)nz;
	}

	glEnable(GL_TEXTURE_2D);
	float s1, t1, s2, t2;
	for(j=0; j<nStack; j++)
		for(i=0;i<nSlice;i++)
		{
			ii = i+1;
			if(ii == nSlice) ii = 0;
			s1 = (float)i / (float)nSlice; s2 = (float)(i+1) / (float)nSlice;
			t1 = 1.0f -(float)j / (float)nStack; t2 = 1.0f -(float)(j+1) / (float)nStack;
			glBegin(GL_QUADS);
			glNormal3fv(n[i]);
			glTexCoord2f(s1, t1); glVertex3fv(p[i][j]);
			glTexCoord2f(s1, t2); glVertex3fv(p[i][j+1]);
			glNormal3fv(n[ii]);
			glTexCoord2f(s2, t2); glVertex3fv(p[ii][j+1]); 
			glTexCoord2f(s2, t1); glVertex3fv(p[ii][j]);
			glEnd();
		}
	glDisable(GL_TEXTURE_2D);
}

//-----------------------------------------------------------------------------------------

void drawTexTorus(float radius1, float radius2, int nSide, int nRing)
{	
	int i,ii,j,jj;
	double phi,ph1,ph2; 
	double theta,th1,th2;
	float rr[31], zz[31];
	float p[961][3];

	if(radius1 > radius2) { printf("radius1 < radius2\n "); return;}
	if(nRing > 30) nRing = 30;
	if(nSide > 30) nSide = 30;

	for(j = 0; j < nSide; j++)
	{
		theta = M_PI-2.0*M_PI*(double)j/(double)nSide;
		rr[j] = (float)(radius2 + radius1 * (float)cos(theta)); 
		zz[j] = (float)(radius1 * (float)sin(theta));
	}

	for(i = 0; i < nRing; i++)
	{
		phi = 2.0*M_PI*(double)i/(double)nRing;
		for(j = 0; j < nSide; j++)
		{	
			p[j*nRing+i][0] = rr[j] * (float)cos(phi); 
			p[j*nRing+i][1] = rr[j] * (float)sin(phi); 
			p[j*nRing+i][2] = zz[j];   
		}
	}

	double s1, t1, s2, t2;
	glEnable(GL_TEXTURE_2D);
	for(i = 0; i < nRing; i++)
	{
		ii = i+1;
		if(ii == nRing) ii = 0;
		s1 = (double)i/(double)nRing; s2 = (double)(i+1)/(double)nRing;
		ph1 = 2.0 * M_PI * s1;
		ph2 = 2.0 * M_PI * s2;
		for(j = 0;j < nSide;j++)
		{
			jj = j+1;
			if(jj == nSide) jj = 0;
			t1 = 1.0 - (double)j / (double)nSide;
			t2 = 1.0 - (double)(j+1) / (double)nSide;
			th1 = M_PI-2.0 * M_PI * (double)j / (double)nSide;
			th2 = M_PI-2.0*M_PI*(double)jj/(double)nSide;
			glBegin(GL_QUADS);
			glNormal3d(cos(th1)*cos(ph1),cos(th1)*sin(ph1),sin(th1));
			glTexCoord2d(s1,t1); glVertex3fv(p[j*nRing+i]); 
			glNormal3d(cos(th2)*cos(ph1),cos(th2)*sin(ph1),sin(th2));
			glTexCoord2d(s1,t2); glVertex3fv(p[jj*nRing+i]);
			glNormal3d(cos(th2)*cos(ph2),cos(th2)*sin(ph2),sin(th2));
			glTexCoord2d(s2,t2); glVertex3fv(p[jj*nRing+ii]); 
			glNormal3d(cos(th1)*cos(ph2),cos(th1)*sin(ph2),sin(th1));
			glTexCoord2d(s2,t1); glVertex3fv(p[j*nRing+ii]);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
}
