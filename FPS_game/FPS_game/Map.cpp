#include "Map.h"
#include <windows.h>
#include <stdio.h>

#include <gl\gl.h>
#include <gl\glu.h>

#include <SDL.h>
#include <GL\SDL_image.h>

// this function taken from SDL documentation
// http://www.libsdl.org/cgi/docwiki.cgi/Introduction_to_SDL_Video#getpixel
Uint32 getPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;

	Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		return *p;
	case 2:
		return *(Uint16*)p;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32*)p;
	default:
		return 0;
	}
}
// functions for reading pixels
float getColourAt(SDL_Surface* surface,int x, int y)
{
	Uint32 pixel;
	Uint8 red, green, blue;
	float rgb;

	pixel = getPixel(surface, x, y);

	SDL_LockSurface(surface);

	SDL_GetRGB(pixel, surface->format, &red, &green, &blue);

	SDL_UnlockSurface(surface);

	rgb = (float)(red+blue+green)/ (float)3;

	return rgb;
}



Map::Map()
{
	terrain=new terrainT(0, 0, 0);
}


Map::~Map()
{
}




void Map::loadHeightMap(char *fileName, float heightFactor)
{
	SDL_Surface* bitmap = SDL_LoadBMP(fileName);

	//	FILE *file;
	unsigned char *terrainData;
	int terrainSize;
	int i, j, counter;
	
	//errno_t errorCode = fopen_s(&file,fileName, "rb");
	//if (!file)
	//	return;

	//fseek(file, 18, SEEK_SET);
	//fread(&terrain->width, 4, 1, file);
	terrain->width=bitmap->w;

	//fseek(file, 22, SEEK_SET);
	//fread(&terrain->height, 4, 1, file);
	terrain->height = bitmap->h;
	
	terrainSize = terrain->width * terrain->height * 3;

	terrainData = new unsigned char[terrainSize];
	//fseek(file, 54, SEEK_SET);
	//fread(terrainData, terrainSize, 1, file);
	//fclose(file);
	terrain->vertices = new vector3d*[terrain->width];
	for (i = 0; i < terrain->width; i++)
	{
		terrain->vertices[i] = new vector3d[terrain->height];
	}
	Uint32 *pixels = (Uint32 *)bitmap->pixels; //Get the requested pixel 
	
	counter = 0;
	for (i = 0; i < terrain->width; i++)
	{
		for (j = 0; j <terrain->height; j++)
		{
			terrain->vertices[i][j].x = (float)(i);
			terrain->vertices[i][j].z = -(float)(j);

			terrain->vertices[i][j].y = getColourAt(bitmap,i,j) * heightFactor;
				counter+=3;
		}
	}

	delete terrainData;
	SDL_FreeSurface(bitmap);

}



void Map::initTerrain(char *fileName, float heightReducingFactor)
{
	int i, j;
	
	loadHeightMap(fileName, heightReducingFactor);

	terrain->terrainList = glGenLists(1);

	glNewList(terrain->terrainList, GL_COMPILE);

	
	glBegin(GL_TRIANGLES);
	for (i = 1; i < terrain->width; i++)
	{
		for (j = 1; j < terrain->height; j++)
		{
			vector3d v1(terrain->vertices[i - 1][j - 1].x, terrain->vertices[i - 1][j - 1].y, terrain->vertices[i - 1][j - 1].z);
			vector3d v2(terrain->vertices[i][j - 1].x, terrain->vertices[i][j - 1].y, terrain->vertices[i][j - 1].z);
			vector3d v3(terrain->vertices[i][j].x, terrain->vertices[i][j].y, terrain->vertices[i][j].z);

			vector3d vn = vector3d::normalVector(v1, v2, v3);
			glNormal3f(vn.x, vn.y, vn.z);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v1.x, v1.y, v1.z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(v2.x, v2.y, v2.z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(v3.x, v3.y, v3.z);

			v1 = vector3d(terrain->vertices[i - 1][j - 1].x, terrain->vertices[i - 1][j - 1].y, terrain->vertices[i - 1][j - 1].z);
			v2= vector3d(terrain->vertices[i][j].x, terrain->vertices[i][j].y, terrain->vertices[i][j].z);
			v3= vector3d(terrain->vertices[i - 1][j].x, terrain->vertices[i - 1][j].y, terrain->vertices[i - 1][j].z);
			vn = vector3d::normalVector(v1, v2, v3);
			glNormal3f(vn.x, vn.y, vn.z);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(v1.x, v1.y, v1.z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(v2.x, v2.y, v2.z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(v3.x, v3.y, v3.z);
		}
	}
	glEnd();

	glEndList();

	glEnable(GL_TEXTURE_2D);
	//TODO: change to freeimage
	SDL_Surface * tex= SDL_LoadBMP("data/grass2.bmp");
	tex = SDL_ConvertSurfaceFormat(tex, SDL_PIXELFORMAT_RGB24, 0);
	glGenTextures(1, (GLuint*)&terrain->terrainTexture);
	glBindTexture(GL_TEXTURE_2D, (GLuint)terrain->terrainTexture);

	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB, tex->w, tex->h, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDisable(GL_TEXTURE_2D);
}



void Map::freeTerrain()
{
	int i;

	for (i = 0; i < terrain->width; i++)
	{
		delete(terrain->vertices[i]);
	}
	delete(terrain->vertices);

	glDeleteLists(terrain->terrainList, 1);
}



void Map::renderTerrain()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, terrain->terrainTexture);
	glCallList(terrain->terrainList);
	glDisable(GL_TEXTURE_2D);
}



cellIndex Map::getActiveTriangle( float x, float z)
{
	int i, j;
	cellIndex result;
	vector2D vertices[3];

	for (i = 1; i < terrain->width; i++)
	{
		for (j = 1; j < terrain->height; j++)
		{
			vertices[0].x = terrain->vertices[i - 1][j - 1].x;
			vertices[0].z = terrain->vertices[i - 1][j - 1].z;
			vertices[1].x = terrain->vertices[i][j - 1].x;
			vertices[1].z = terrain->vertices[i][j - 1].z;
			vertices[2].x = terrain->vertices[i][j].x;
			vertices[2].z = terrain->vertices[i][j].z;

			if (Collision::isCollision2points(x, z, vertices))
			{
				result.index_x = i;
				result.index_z = j;
				result.index_triangle = 0;
				goto finish;
			}


			vertices[0].x = terrain->vertices[i - 1][j - 1].x;
			vertices[0].z = terrain->vertices[i - 1][j - 1].z;
			vertices[1].x = terrain->vertices[i][j].x;
			vertices[1].z = terrain->vertices[i][j].z;
			vertices[2].x = terrain->vertices[i - 1][j].x;
			vertices[2].z = terrain->vertices[i - 1][j].z;

			if (Collision::isCollision2points(x, z, vertices))
			{
				result.index_x = i;
				result.index_z = j;
				result.index_triangle = 1;
				goto finish;
			}
				
		}
	}

finish:
	
	return result;
}



float Map::getTerrainHeight( float x, float z)
{
	vector3d vertices[3];
	vector3d planeNormal;
	float a, b, c, d;
	float height;
	cellIndex activeTriangle = getActiveTriangle( x, z);

	if (activeTriangle.index_triangle == 0)
	{
		vertices[0].x = terrain->vertices[activeTriangle.index_x - 1][activeTriangle.index_z - 1].x;
		vertices[0].y = terrain->vertices[activeTriangle.index_x - 1][activeTriangle.index_z - 1].y;
		vertices[0].z = terrain->vertices[activeTriangle.index_x - 1][activeTriangle.index_z - 1].z;
		vertices[1].x = terrain->vertices[activeTriangle.index_x][activeTriangle.index_z - 1].x;
		vertices[1].y = terrain->vertices[activeTriangle.index_x][activeTriangle.index_z - 1].y;
		vertices[1].z = terrain->vertices[activeTriangle.index_x][activeTriangle.index_z - 1].z;
		vertices[2].x = terrain->vertices[activeTriangle.index_x][activeTriangle.index_z].x;
		vertices[2].y = terrain->vertices[activeTriangle.index_x][activeTriangle.index_z].y;
		vertices[2].z = terrain->vertices[activeTriangle.index_x][activeTriangle.index_z].z;
	}
	else if (activeTriangle.index_triangle == 1)
	{
		vertices[0].x = terrain->vertices[activeTriangle.index_x - 1][activeTriangle.index_z - 1].x;
		vertices[0].y = terrain->vertices[activeTriangle.index_x - 1][activeTriangle.index_z - 1].y;
		vertices[0].z = terrain->vertices[activeTriangle.index_x - 1][activeTriangle.index_z - 1].z;
		vertices[1].x = terrain->vertices[activeTriangle.index_x][activeTriangle.index_z].x;
		vertices[1].y = terrain->vertices[activeTriangle.index_x][activeTriangle.index_z].y;
		vertices[1].z = terrain->vertices[activeTriangle.index_x][activeTriangle.index_z].z;
		vertices[2].x = terrain->vertices[activeTriangle.index_x - 1][activeTriangle.index_z].x;
		vertices[2].y = terrain->vertices[activeTriangle.index_x - 1][activeTriangle.index_z].y;
		vertices[2].z = terrain->vertices[activeTriangle.index_x - 1][activeTriangle.index_z].z;
	}

	planeNormal = getNormal(vertices[0], vertices[1], vertices[2]);

	a = planeNormal.x;
	b = planeNormal.y;
	c = planeNormal.z;
	d = -(a*vertices[0].x + b*vertices[0].y + c*vertices[0].z);

	height = -(a*x + c*z + d) / b;
	return height;
}
