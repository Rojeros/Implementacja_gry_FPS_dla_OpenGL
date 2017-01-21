#include <string>
#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_ttf.h>

/// <summary>	Values that represent coordinates. </summary>
enum coordinates {
	NW,
	N,
	NE,
	E,
	SE,
	S,
	SW,
	W,
	CENTER

};



class Text {
public:
	
	Text(SDL_Renderer *renderer, int width, int height, int fontSize);
	~Text();
	void renderText(const std::string &message,SDL_Color color, coordinates coord,  int line, int column);
	protected:
		SDL_Renderer * renderer;
		int width;
		int height;
		TTF_Font * font;
		int fontSize;
};

class GameUI :public Text {
public:
	GameUI(SDL_Renderer *renderer, int  width, int height, int fontSize) :Text(renderer, width, height, fontSize) {
		 healthTexture = -1;
		 energyTexture = -1;
		ammoTexture = -1;
		allammoTexture = -1;
		scoreTexture = -1;
		weaponNameTexture ="";
		levelTexture ="";
		fpsTexture = 0;
		colorTexture = { 255,255,255 };
		sFontHealth=TTF_RenderText_Blended(font, ("health: "+ std::to_string(healthTexture)).c_str(), colorTexture);
		sFontAmmo = TTF_RenderText_Blended(font, (std::to_string(ammoTexture) +" / "+std::to_string(allammoTexture)).c_str(), colorTexture);;
		sFontScore = TTF_RenderText_Blended(font, ("score: " + std::to_string(scoreTexture)).c_str(), colorTexture);;
		sFontWeaponName = TTF_RenderText_Blended(font, (weaponNameTexture).c_str(), colorTexture);;
		sFontLevel = TTF_RenderText_Blended(font, (levelTexture).c_str(), colorTexture);
		sFontFps = TTF_RenderText_Blended(font, ("FPS: " + std::to_string(fpsTexture)).c_str(), colorTexture);
		changeValues(0,0,0,0,0,"0","0.lvl",0);

		glGenTextures(1, &tex[6]);
		glBindTexture(GL_TEXTURE_2D, tex[6]);
		sFontAim = TTF_RenderText_Blended(font, "+", colorTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFontAim->w, sFontAim->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sFontAim->pixels);
		
	};
	void changeValues(int health,int energy,int ammo,int allammo,int score,std::string weaponName,std::string level,int fps);
	void draw();
	void displayRedSquare();
	protected:
		int healthTexture;
		int energyTexture;
		int ammoTexture;
		int allammoTexture;
		int scoreTexture;
		int fpsTexture;
		std::string weaponNameTexture;
		std::string levelTexture;
		SDL_Surface * sFontHealth;
		SDL_Surface * sFontAmmo;
		SDL_Surface * sFontScore;
		SDL_Surface * sFontWeaponName;
		SDL_Surface * sFontLevel;
		SDL_Surface * sFontFps;
		SDL_Surface * sFontAim;
		SDL_Color colorTexture;

		GLuint tex[7];
		void draw(int index, coordinates coord, int line, int column, float widthT, float heightT);
		void drawBoxes();
};