// กำหนด material

static float mat_ambient[8][4] = {
	{ 0.0, 0.0, 0.0, 1.0 },			        // Black Plastic
	{ 0.329412, 0.223529, 0.027451, 1.0 },	//  Brass
	{ 0.2125,0.1275,0.054,1.0},			  // Bronze
	{0.25,0.25,0.25,1.0},					  //Chrome
	{0.191125,0.0735,0.0225,1.0},			  //Copper
	{0.24725,0.1995,0.0745,1.0},				  //Gold
	{1.0,1.0,1.0,1.0},
	{ 0.1322, 0.0, 0.7, 0.7 },
};
static float mat_diffuse[8][4] = {
	{ 0.01, 0.01, 0.01, 1.0 },
	{ 0.780392, 0.568627, 0.113725, 1.0 },
	{0.714,0.4284,0.18144,1.0},
	{0.4,0.4,0.4,1.0},
	{0.7038,0.27048,0.0828,1.0},
	{0.75164,0.60648,0.22648,1.0},
	{1.0,1.0,1.0,1.0},
	{ 1.0, 0.0, 0.1, 0.6 }// Alpha
};

static float mat_specular[7][4] = {
	{ 0.5, 0.5, 0.5, 1.0 },
	{0.992157, 0.941176, 0.807843, 1.0 },
	{0.393548,0.271906,0.166721,1.0},
	{0.774597,0.774597,0.774597,1.0},
	{0.256777,0.137622,0.086014,1.0},
	{0.628281,0.555802,0.366065,1.0},
	{1.0,1.0,1.0,1.0}
};

static float mat_shininess[7][1] = {
	{ 32.0 },
	{ 27.8974  },
	{ 25.6 },
	{ 76.8 },
	{ 12.8},
	{ 51.2 },
	{1.0}
};
