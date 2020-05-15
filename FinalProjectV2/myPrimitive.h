void drawCube(float s);
void drawSphere(double radius, int nSlice, int nStack);
void drawSphereHalf(double radius, int nSlice, int nStack);
void drawSpherePercen(double per, double radius, int nSlice, int nStack, float);
void drawCylinder(float rBottom, float rTop, float height, int nSlice, int nStack);
void drawTorus(float radius1, float radius2, int nSide, int nRing);
void drawSpring(float radius1, float radius2, float len, int nSide, int nRing, int nPitch);
void drawRevolution(float cs[][2], int nSide, int nSlice);
void drawFloor(double widthX, double widthZ, int nx, int nz);
