#include "util.h"

// calcula la media de las normales de 2 poligonos que comparten una arista
GLfloat * calcularProductoVectorialMedio(GLfloat * centro, GLfloat * vertcompartido, GLfloat * vertlateral1, GLfloat * vertlateral2)
{
	GLfloat * normal1;
	GLfloat * normal2;
	
	normal1 = calcularProductoVectorial(centro,vertcompartido,vertlateral1);
	normal2 = calcularProductoVectorial(centro,vertcompartido,vertlateral2);

	GLfloat * normalmedia = new GLfloat[3];

	normalmedia[0] = (normal1[0]+normal2[0])/2;
	normalmedia[1] = (normal1[1]+normal2[1])/2;
	normalmedia[2] = (normal1[2]+normal2[2])/2;
	normalizarVector(normalmedia);
	delete normal1;
	delete normal2;
	return (normalmedia);
}

void normalizarVector(GLfloat *v)
{
	GLfloat size= sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	v[0]/=size;
	v[1]/=size;
	v[2]/=size;
}

GLfloat * calcularProductoVectorial(GLfloat * vert1, GLfloat * vert2, GLfloat * vert3)
{
	GLfloat* v = new GLfloat [3];
//	(vert2-vert1)*(vert2-vert3)
	v[0] = (vert2[1]-vert1[1])*(vert2[2]-vert3[2]);
	v[1] = (vert2[2]-vert1[0])*(vert2[0]-vert3[2]);
	v[2] = (vert2[0]-vert1[1])*(vert2[1]-vert3[0]);
	return v;
}


GLfloat dotProduct(GLfloat * v1, GLfloat * v2)
{
	return v1[0]*v2[1]+v1[0]*v2[2]+
			v1[1]*v2[0]+v1[1]*v2[2]+
			v1[2]*v2[0]+v1[2]*v2[1];
}


GLuint loadTextureFromFile(const char filename [], bool mipmap)
{
/*
	static bool initialized = true;
	if(!initialized)
	{
		//configuracion de matrices para arreglar las texturas
		glMatrixMode(GL_TEXTURE);
		glRotatef(180,0.0f,0.0f,1.0f);
		glScalef(-1.0f,1.0f,1.0f);
		glMatrixMode(GL_MODELVIEW);
		initialized=true;	
	}
*/		
	glPushAttrib (GL_ALL_ATTRIB_BITS);

		
	GLuint texture;
	SDL_Surface* imgFile;
	
    if ( !( imgFile = IMG_Load ( filename ) ) )
    {
		printf("No se ha podido cargar la imagen:%s\n",filename);
        return 0;
    }
	GLuint colorMode;
	if(imgFile->format->BytesPerPixel == 4) // it has alpha value
		colorMode = GL_RGBA;
	else
		colorMode = GL_RGB;

    	
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	if(mipmap){
		// Load mipmap texture
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgFile->w, imgFile->h, colorMode, GL_UNSIGNED_BYTE, imgFile->pixels);
	} else {
		// Load normal texture
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D (GL_TEXTURE_2D, 0, colorMode, imgFile->w, imgFile->h, 0, colorMode, GL_UNSIGNED_BYTE, imgFile->pixels);
	}
	glPopAttrib ();
	delete imgFile;
	return texture;
}


void DrawRect(GLuint textureID, SDL_Rect * rect)
{
		glBindTexture(GL_TEXTURE_2D,textureID);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);		
			glTexCoord2f(0.0f,1.0f);
			glVertex3f(rect->x,rect->y+rect->h,0.03f);
		
			glTexCoord2f(1.0f,1.0f);
			glVertex3f(rect->x+rect->w,rect->y+rect->h,0.03f);
		
			glTexCoord2f(1.0f,0.0f);
			glVertex3f(rect->x+rect->w,rect->y,0.03f);
		
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(rect->x,rect->y,0.03f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
}
