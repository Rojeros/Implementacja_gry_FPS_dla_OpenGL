#include "Text.h"

Text::Text(SDL_Renderer * renderer, int width, int height, int fontSize) :renderer(renderer), width(width), height(height), fontSize(fontSize)
{
	font = TTF_OpenFont("data/arial.ttf", fontSize);

}

Text::~Text()
{
	TTF_CloseFont(font);

}

void Text::renderText(const std::string & message, SDL_Color color, coordinates coord, int line, int column)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, width, 0, height); // m_Width and m_Height is the resolution of window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);



	float tmp;
	float x = 0, y = 0;
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	tmp = color.r;
	color.r = color.b;
	color.b = tmp;

	SDL_Surface * sFont = TTF_RenderText_Blended(font, message.c_str(), color);
	glColor4f(1, 1, 1, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sFont->pixels);


	if (coord == N || coord == NW || coord == NE)	//top
		y = height - (sFont->h*1.5) + (line*sFont->h*1.2);
	if (coord == S || coord == SE || coord == SW)	//down
		y = (0) + (sFont->h *1.5) + (line*sFont->h*1.2);

	if (coord == E || coord == NE || coord == SE)	//right
		x = width - (sFont->w*1.5) + (column*sFont->w*1.2);
	if (coord == W || coord == NW || coord == SW)	//left
		x = 0 + (sFont->w*0.5) + (column*sFont->w*1.2);

	if (coord == N || coord == S || coord == CENTER)	//center,x
		x = width / 2 - (sFont->w / 2) + (column*sFont->w*1.2);
	if (coord == W || coord == E || coord == CENTER)	//center, y
		y = (height / 2) - (sFont->h / 2) + (line*sFont->h*1.2);




	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 1); glVertex2f(x, y);
		glTexCoord2f(1, 1); glVertex2f(x + sFont->w, y);
		glTexCoord2f(1, 0); glVertex2f(x + sFont->w, y + sFont->h);
		glTexCoord2f(0, 0); glVertex2f(x, y + sFont->h);
	}
	glEnd();




	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);

	glDeleteTextures(1, &texture);
	SDL_FreeSurface(sFont);
}

void GameUI::changeValues(int health, int energy, int ammo, int allammo, int score, std::string weaponName, std::string level, int fps)
{
	if (healthTexture != health) {
		SDL_FreeSurface(sFontHealth);
		glGenTextures(1, &tex[0]);
		glBindTexture(GL_TEXTURE_2D, tex[0]);
		healthTexture = health;
		sFontHealth = TTF_RenderText_Blended(font, ("health: " + std::to_string(healthTexture)).c_str(), colorTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFontHealth->w, sFontHealth->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sFontHealth->pixels);
	}
	if (energyTexture != energy) {
		energyTexture = energy;
	}
	if (ammoTexture != ammo || allammoTexture != allammo) {
		SDL_FreeSurface(sFontAmmo);
		glGenTextures(1, &tex[1]);
		glBindTexture(GL_TEXTURE_2D, tex[1]);
		ammoTexture = ammo;
		allammoTexture = allammo;
		sFontAmmo = TTF_RenderText_Blended(font, (std::to_string(ammoTexture) + " / " + std::to_string(allammoTexture)).c_str(), colorTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFontAmmo->w, sFontAmmo->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sFontAmmo->pixels);
	}

	if (scoreTexture != score) {
		SDL_FreeSurface(sFontScore);
		glGenTextures(1, &tex[2]);
		glBindTexture(GL_TEXTURE_2D, tex[2]);
		scoreTexture = score;
		sFontScore = TTF_RenderText_Blended(font, ("score: " + std::to_string(scoreTexture)).c_str(), colorTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFontScore->w, sFontScore->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sFontScore->pixels);
	}
	if (weaponNameTexture != weaponName) {
		SDL_FreeSurface(sFontWeaponName);
		glGenTextures(1, &tex[3]);
		glBindTexture(GL_TEXTURE_2D, tex[3]);
		weaponNameTexture = weaponName;
		sFontWeaponName = TTF_RenderText_Blended(font, (weaponNameTexture).c_str(), colorTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFontWeaponName->w, sFontWeaponName->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sFontWeaponName->pixels);
	}
	if (levelTexture != level) {
		SDL_FreeSurface(sFontLevel);
		glGenTextures(1, &tex[4]);
		glBindTexture(GL_TEXTURE_2D, tex[4]);
		levelTexture = level;
		sFontLevel = TTF_RenderText_Blended(font, (levelTexture).c_str(), colorTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFontLevel->w, sFontLevel->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sFontLevel->pixels);
	}
	if (fpsTexture != fps) {
		SDL_FreeSurface(sFontFps);
		glGenTextures(1, &tex[5]);
		glBindTexture(GL_TEXTURE_2D, tex[5]);
		fpsTexture = fps;
		sFontFps = TTF_RenderText_Blended(font, ("FPS: " + std::to_string(fpsTexture)).c_str(), colorTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFontFps->w, sFontFps->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sFontFps->pixels);
	}
}

void GameUI::draw()
{
	//health
	draw(0, SW, 0, 0, sFontHealth->w, sFontHealth->h);
	//ammo
	draw(1, NE, -1, 0, sFontAmmo->w, sFontAmmo->h);
	//score
	draw(2, NW, 0, 0, sFontScore->w, sFontScore->h);
	//weapon
	draw(3, NE, 0, 0, sFontWeaponName->w, sFontWeaponName->h);
	//level
	draw(4, N, 0, 0, sFontLevel->w, sFontLevel->h);
	//fps
	draw(5, NW, -1, 0, sFontFps->w, sFontFps->h);
	//Aim
	draw(6, CENTER, 0, 0, sFontAim->w, sFontAim->h);
	drawBoxes();
	

}

void GameUI::displayRedSquare()
{
	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluOrtho2D(0, width, 0, height); // m_Width and m_Height is the resolution of window
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);

	glColor4f(1, 1, 1, 1);
	glColor4f(0.5f, 0.0f, 0.0f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(width, 0);
	glVertex2f(width, height);
	glVertex2f(0, height);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);




}

GameUI::~GameUI()
{
	SDL_FreeSurface (sFontHealth);
	SDL_FreeSurface (sFontAmmo);
	SDL_FreeSurface (sFontScore);
	SDL_FreeSurface (sFontWeaponName);
	SDL_FreeSurface (sFontLevel);
	SDL_FreeSurface( sFontFps);
	SDL_FreeSurface( sFontAim);
}

void GameUI::draw(int index, coordinates coord, int line, int column, float widthT, float heightT)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, width, 0, height); // m_Width and m_Height is the resolution of window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);



	float tmp;
	float x = 0;
	float	y = 0;

	glBindTexture(GL_TEXTURE_2D, tex[index]);
	
	glColor4f(1, 1, 1, 1);

	float margin = height *0.03;


	if (coord == N || coord == NW || coord == NE)	//top
		y = height - (heightT+margin) + (line*heightT*1.2);
	if (coord == S || coord == SE || coord == SW)	//down
		y = (0) + ( margin) + (line*heightT*1.2);

	if (coord == E || coord == NE || coord == SE)	//right
		x = width - (widthT+ margin) + (column*widthT*1.2);
	if (coord == W || coord == NW || coord == SW)	//left
		x = 0 + (margin) + (column*widthT*1.2);

	if (coord == N || coord == S || coord == CENTER)	//center,x
		x = width / 2 - (widthT / 2) + (column*widthT*1.2);
	if (coord == W || coord == E || coord == CENTER)	//center, y
		y = (height / 2) - (heightT / 2) + (line*heightT*1.2);

	
	glBegin(GL_QUADS);
	{
		

		glTexCoord2f(0, 1); glVertex2f(x, y);
		glTexCoord2f(1, 1); glVertex2f(x + widthT, y);
		glTexCoord2f(1, 0); glVertex2f(x + widthT, y + heightT);
		glTexCoord2f(0, 0); glVertex2f(x, y + heightT);
	}
	glEnd();
	
	
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);



}

void GameUI::drawBoxes()
{
	float xl = 0;
	float xr = 0;
	float	y = 0;
	float margin = height *0.04;

	float  minusLife = (float)( 100 - healthTexture)/100;
	float boxLifeWidth = width*0.3;
	boxLifeWidth = boxLifeWidth - boxLifeWidth*minusLife;
	float  green = 1 - minusLife;
	float  redLife = minusLife;


	float  minusEnergy = (float)(100 - energyTexture) / 100;
	float boxEnergyWidth = width*0.3;
	boxEnergyWidth = boxEnergyWidth - boxEnergyWidth*minusEnergy;
	float  blue = 1 - minusEnergy;
	float  redEnergy = minusEnergy;

	
		y = (0) + (margin)+(fontSize*1.2);

		xr = width - (boxEnergyWidth + margin);
		xl = 0 + (margin);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, width, 0, height); // m_Width and m_Height is the resolution of window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(redLife, green, 0.0);
	glVertex2f(xl, y);
	glColor3f(redLife, green, 0.0);
	glVertex2f(xl + boxLifeWidth, y);
	glColor3f(redLife, green, 0.0);
	glVertex2f(xl + boxLifeWidth, y + fontSize);
	glColor3f(redLife, green, 0.0);
	glVertex2f(xl , y + fontSize);
	glEnd();

	glBegin(GL_QUADS);
	
	glColor3f(redEnergy, 0, blue);
	glVertex2f(xr, y);
	glColor3f(redEnergy, 0.0, blue);
	glVertex2f(xr + boxEnergyWidth, y);
	glColor3f(redEnergy, 0.0, blue);
	glVertex2f(xr + boxEnergyWidth, y + fontSize);
	glColor3f(redEnergy, 0, blue);
	glVertex2f(xr, y + fontSize);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);

}
