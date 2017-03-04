#include "Map.h"
#include <windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <SDL.h>

vector2D Map::lastPosition;
float  Map::lastHeiht;

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
	glDeleteTextures(1, (GLuint*)&terrain->terrainTexture[0]);
	glDeleteTextures(1, (GLuint*)&terrain->terrainTexture[1]);
	for (int i = 0; i < terrain->width; i++) {

		delete[] terrain->vertices[i];

	}
	delete[] terrain->vertices;
	glDeleteLists(terrain->terrainList, 1);
	delete box[0];
	delete box[1];
	delete box[2];
	delete box[3];
	delete box[4];
	delete box[5];

	delete terrain;
}




void Map::loadHeightMap(char *fileName, float heightFactor)
{

	SDL_Surface* bitmap = SDL_LoadBMP(fileName);
	if (bitmap == NULL) {

		std::string text = fileName;
		text = "can't open a file: " + text;
		
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text.c_str(), NULL);
		exit(3);
	}

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



void Map::initTerrain(char *fileName, char*floorTexture, char*horizonTexture,float heightReducingFactor)
{
	int i, j;
	lastPosition=vector2D(0,0);
	lastHeiht = 0;

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
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to


	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(floorTexture, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(floorTexture);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN) {
		
			std::string text = floorTexture;
			text = "can't open a file: " + text;

			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text.c_str(), NULL);
			exit(3);
		
	}
	//std::cout << "FIF " << fif << " " << filename << std::endl;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, floorTexture);
	//if the image failed to load, return failure
	if (!dib) {
		std::string text = floorTexture;
		text = "can't open a file: " + text;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text.c_str(), NULL);
		exit(3);
	}
	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);

	FREE_IMAGE_COLOR_TYPE color = FreeImage_GetColorType(dib);
	if (color != FIC_RGB && color != FIC_RGBALPHA)
	{
		FreeImage_Unload(dib);
		std::string text = floorTexture;
		text = "color error in file: " + text;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text.c_str(), NULL);
		exit(3);
	}
	//if this somehow one of these failed (they shouldn't), return failure
	//	std::cout << (bits == 0) << std::endl;
	if ((bits == 0) || (width == 0) || (height == 0)) {
		std::string text = floorTexture;
		text = "size error in file: " + text;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text.c_str(), NULL);
		exit(3);
		return ;
	}
	//std::cout << width << " " << height << std::endl;
	//if this texture ID is in use, unload the current texture

	glGenTextures(1, (GLuint*)&terrain->terrainTexture[0]);
	glBindTexture(GL_TEXTURE_2D, (GLuint)terrain->terrainTexture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, (color == FIC_RGB) ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDisable(GL_TEXTURE_2D);




	glEnable(GL_TEXTURE_2D);
	//image format
	fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	dib=0;
	//pointer to the image data
	bits=0;
	//image width and height
	width=0;
	height=0;
	//OpenGL's image ID to map to


	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(horizonTexture, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(horizonTexture);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN) {
		std::string text = horizonTexture;
		text = "can't open a file: " + text;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text.c_str(), NULL);
		exit(3);
	}
	//std::cout << "FIF " << fif << " " << filename << std::endl;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, horizonTexture);
	//if the image failed to load, return failure
	if (!dib) {
		std::string text = horizonTexture;
		text = "can't open a file: " + text;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text.c_str(), NULL);
		exit(3);
	}
	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);

	color = FreeImage_GetColorType(dib);
	if (color != FIC_RGB && color != FIC_RGBALPHA)
	{
		std::string text = horizonTexture;
		text = "color error in file: " + text;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text.c_str(), NULL);
		exit(3);
	}
	//if this somehow one of these failed (they shouldn't), return failure
	//	std::cout << (bits == 0) << std::endl;
	if ((bits == 0) || (width == 0) || (height == 0)) {
		std::string text = horizonTexture;
		text = "size error in file: " + text;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", text.c_str(), NULL);
		exit(3);
	}
	//std::cout << width << " " << height << std::endl;
	//if this texture ID is in use, unload the current texture

	glGenTextures(1, (GLuint*)&terrain->terrainTexture[1]);
	glBindTexture(GL_TEXTURE_2D, (GLuint)terrain->terrainTexture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, (color == FIC_RGB) ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDisable(GL_TEXTURE_2D);


	box[0] = new CollisionPlane(vector3d(0,0,1),	vector3d(5,100,-5),					vector3d(terrain->width-5,100,-5),						vector3d(terrain->width-5,-100,-5),							vector3d(5,-100,-5));
	box[1] = new CollisionPlane(vector3d(1,0,0),	vector3d(terrain->width-5,100,-5),	vector3d(terrain->width-5,100, -terrain->height+5),		vector3d(terrain->width-5,-100, -terrain->height+5),		vector3d(terrain->width-5,-100,-5));
	box[2] = new CollisionPlane(vector3d(0,0,-1),	vector3d(5,100,-terrain->height+5),	vector3d(terrain->width-5,100, -terrain->height+5),		vector3d(terrain->width-5,-100, -terrain->height+5),		vector3d(5,-100, -terrain->height+5));
	box[3] = new CollisionPlane(vector3d(-1,0,0),	vector3d(5,100,-5),					vector3d(5,100, -terrain->height+5),					vector3d(5,-100, -terrain->height+5),						vector3d(5,-100,-5));
	box[4] = new CollisionPlane(vector3d(0,1,0),	vector3d(5,100,-5),					vector3d(5,100, -terrain->height+5),					vector3d(terrain->width-5,100, -terrain->height+5),			vector3d(terrain->width-5,100,-5));
	box[5] = new CollisionPlane(vector3d(0,-1,0),	vector3d(5,-100,-5),					vector3d(terrain->width-5,-100,-5),						vector3d(terrain->width-5,-100, -terrain->height+5),	vector3d(5,-100, -terrain->height+5));

}



void Map::freeTerrain()
{
	glDeleteTextures(1, (GLuint*)&terrain->terrainTexture[0]);
	glDeleteTextures(1, (GLuint*)&terrain->terrainTexture[1]);
	for (int i = 0; i < terrain->width; i++) {

		delete[] terrain->vertices[i];

	}
	delete[] terrain->vertices;
	glDeleteLists(terrain->terrainList, 1);
	delete box[0];
	delete box[1];
	delete box[2];
	delete box[3];
	delete box[4];
	delete box[5];

	delete terrain;
}



void Map::renderTerrain()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, terrain->terrainTexture[0]);
	glCallList(terrain->terrainList);

	glBindTexture(GL_TEXTURE_2D, terrain->terrainTexture[1]);
	glBegin(GL_QUADS);

	//glColor3f(0, 0, 0.6);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f((terrain->width * 3), -20, (terrain->height*(3)));
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f((terrain->width * 3), -20, (terrain->height*(-3)));
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f((terrain->width * (-3)), -20, ((terrain->height*(-3))));
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f((terrain->width * (-3)), -20, (terrain->height*(3)));

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

}

CollisionPlane ** Map::getBox()
{
	return box;
}



cellIndex Map::getActiveTriangle( float x, float z)
{
	int i, j;
	cellIndex result;
	result.index_x = -1;
	result.index_z = -1;
	result.index_triangle = -1;
	vector2D vertices[3];

	

	for (i = 1; i < terrain->width; i++)
	{
		
		for (j = 1; j < terrain->height; j++)
		{
			if (terrain->vertices[i - 1][j - 1].x == (int)x &&  terrain->vertices[i - 1][j - 1].z == (int)z) {
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
					return result;
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
					return result;
				}
			}
		}
	}


	
	return result;
}



float Map::getTerrainHeight( float x, float z)
{
	if (lastPosition.x == x && lastPosition.z == z) {
		return lastHeiht;

	}
	vector3d vertices[3];
	vector3d planeNormal;
	float a, b, c, d;
	float height;
	cellIndex activeTriangle = getActiveTriangle( x, z);
	//przypisywanie odpowiednich wierzcho³ków w zale¿noœci od pozycji trójk¹ta w kwadracie
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
	else if (activeTriangle.index_triangle == -1)
	{
		return 0;
	}
	planeNormal = getNormal(vertices[0], vertices[1], vertices[2]);

	a = planeNormal.x;
	b = planeNormal.y;
	c = planeNormal.z;
	d = -(a*vertices[0].x + b*vertices[0].y + c*vertices[0].z);

	height = -(a*x + c*z + d) / b;

	lastPosition = vector2D(x, z);
	lastHeiht = height;

	return height;
}
