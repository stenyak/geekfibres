#include "initgl.h"

int InitGL (SDL_Surface * surface, int windowWidth, int windowHeight, int windowDepth)
{
	int videoFlags;
	const SDL_VideoInfo *videoInfo;

	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		fprintf (stderr, "Video initialization failed: %s\n",SDL_GetError ());
		exit(-1);
	}
	atexit (SDL_Quit);

	videoInfo = SDL_GetVideoInfo ();
	if (!videoInfo)
	{
		fprintf (stderr, "Video query failed: %s\n", SDL_GetError ());
		exit(-1);
	}

	/* initialize flags to pass to SDL_SetVideoMode */
	videoFlags  = SDL_OPENGL |SDL_GL_DOUBLEBUFFER |SDL_HWPALETTE | SDL_RESIZABLE;

	/* check if we can allocate memory in hardware for the window */
	if (videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	/* initialize video mode */
	surface = SDL_SetVideoMode (windowWidth, windowHeight, windowDepth, videoFlags);

	if (!surface)
	{
		fprintf (stderr, "Video mode set failed: %s\n", SDL_GetError ());
		exit(-1);
	}
	else
	{
		fprintf (stderr, "Video mode set succeded\n");
	}

	const char windowTitle[] = "PATATAS ASESINAAAAAAS";
	SDL_WM_SetCaption (windowTitle, windowTitle);

	// Hacer que la aplicacion no suelte el raton
	//    SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(SDL_DISABLE);

	if (SDL_EnableKeyRepeat (100, SDL_DEFAULT_REPEAT_INTERVAL))
	{
		fprintf (stderr, "Setting keyboard repeat failed: %s\n",SDL_GetError ());
		exit(-1);
	}

	// -- opengl initialization 

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 

	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel (GL_SMOOTH);
	glEnable (GL_DEPTH_TEST);

	
	//glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);


/*	GLfloat luz_ambient[] = {0.3f,0.3f,0.3f,1.0f};
	GLfloat luz_diffuse[] = {0.85f,0.85f,0.85f,1.0f};
	GLfloat luz_especular[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat pos_luz[] = {1.0f,0.4f,0.6f,0.0f};


	glLightfv(GL_LIGHT0,GL_AMBIENT, luz_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, luz_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR, luz_especular);
	glLightfv(GL_LIGHT0,GL_POSITION, pos_luz);
*/

	glMatrixMode(GL_MODELVIEW);

	return videoFlags;
}

void SetPerspectiveMode(int windowWidth, int windowHeight)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glEnable(GL_DEPTH_TEST);
	gluPerspective (120.0, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.1, 1000.0);
	glMatrixMode (GL_MODELVIEW);
}

void SetOrthoMode(int windowWidth, int windowHeight)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glDisable(GL_DEPTH_TEST);
	glOrtho (0, windowWidth,windowHeight,0, -1.1f, 1.1f);
	glMatrixMode (GL_MODELVIEW);
}





