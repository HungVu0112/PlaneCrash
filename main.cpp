#include "Loadimage_Func.h"
#include "Player.h"
#include "Bullets.h"

int Delay[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, - 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3 };
int index = 0;
int flag = 0;
int DelayTime = 100;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
/////////////////////////////////////
bool initSDL();
bool loadBackGround();
void close();
////////////////////////////////////////////////
Game menu, menutext1, menutext2, menutext3;
Uint8 a = 255;
int menutext1_X = 100, menutext1_Y = 100;
int menutext2_X = 150, menutext2_Y = 400;
bool checkChangeMenu = false;
void changeBackGround1();
//////////////////////////////////////////////////////////////////////////////////////
Game control_, controltext, livestext, control, heart, dots, changePoint, Roundtext;
Uint8 b = 255;
int controltext_X = 190, controltext_Y = 200;
int livestext_X = 750, livestext_Y = 200;
int changepoint_X = 1100, changepoint_Y = 630;
bool checkClicked = false;
void changeBackGround2();
////////////////////////////////////////////////////
Player player_, Round1BackGround, Lives;
int BackGround_Speed = 0, BackGround_width = 6131;
void Bullet_fired();

int main(int argc, char* args[]) {
	if (!initSDL()) {
		cout << "Could not initialize !! " << endl;
	}
	else if (!loadBackGround()) {
		cout << "Could not load menu background ! " << endl;
	}
	else {
		bool quit = false;
		SDL_Event e;
		while (!quit) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) quit = true;
				if(flag == 0) menutext2.handleEvent(e, menutext2_X, menutext2_Y, menutext2, checkChangeMenu);
				if (flag == 1) changePoint.handleEvent(e, changepoint_X, changepoint_Y, changePoint, checkClicked);
				player_.handleMove(e, renderer);

			}
			
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			SDL_RenderClear(renderer);

			// Control Guide
			if (checkChangeMenu) {
				if (flag == 0) changeBackGround1();

				if (checkClicked) {
					if (flag == 1) changeBackGround2();

					player_.move(player_);
					Round1BackGround.render_(BackGround_Speed, 0, NULL, renderer);
					
					while (DelayTime > 0) {
						Roundtext.render(350, 250, NULL, renderer);
						SDL_RenderPresent(renderer);
						DelayTime--;
					}
					
					Lives.render_(50, 20, NULL, renderer);
					player_.renderPlayer(NULL, renderer);
					Bullet_fired();

					SDL_RenderPresent(renderer);

					if (-BackGround_Speed >= BackGround_width) BackGround_Speed = -BackGround_width;
					else BackGround_Speed -= 2;


				}
				else {
					control_.render(0, 0, NULL, renderer);
					controltext.render(controltext_X, controltext_Y, NULL, renderer);
					control.render(250, 350, NULL, renderer);
					livestext.render(livestext_X, livestext_Y, NULL, renderer);
					heart.render(800, 360, NULL, renderer);
					dots.render(910, 600, NULL, renderer);
					changePoint.render(changepoint_X, changepoint_Y, NULL, renderer);
					SDL_RenderPresent(renderer);

					controltext_X -= Delay[index]; controltext_Y += Delay[index];
					livestext_X -= Delay[index]; livestext_Y += Delay[index];
					if (index == (MAX - 1)) index = 0;
					else index++;
				}
			}
			else {
				// Menu Background
				menu.render(0, 0, NULL, renderer);
				menutext1.render(menutext1_X, menutext1_Y, NULL, renderer);
				menutext2.render(menutext2_X, menutext2_Y, NULL, renderer);

				SDL_RenderPresent(renderer);

				menutext1_X -= Delay[index];
				menutext1_Y += Delay[index];
				if (index == (MAX - 1)) index = 0;
				else index++;
			}
		}
	}
	close();
	return 0;
}

bool initSDL() {
	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << "Could not create window ! " << SDL_GetError();
		return false;
	}
	else {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL) {
			cout << "Could not create renderer ! " << SDL_GetError();
			return false;
		}
		else {
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

			int IMGflags = IMG_INIT_PNG;
			if (!(IMG_Init(IMGflags) & IMGflags)) {
				cout << "SDL_Image could not initialize ! " << IMG_GetError();
				return false;
			}

			if (TTF_Init() == -1) {
				cout << "SDL_ttf could not initialize ! " << TTF_GetError();
				return false;
			}
		}
	}
	return true;
}

bool loadBackGround() {
	
	// Menu background
	if (!menu.loadFromFile("Images/menutex.png", renderer)) {
		cout << "Could not load menu texture ! " << endl;
		return false;
	}
	else {
		menu.setBlendMode(SDL_BLENDMODE_BLEND);
	}

	SDL_Color menutext1_color = { 0, 0, 0 };
	if (!menutext1.loadFromText("DODGE GAME", "Images/lazy2.ttf", menutext1_color, 150, renderer)) {
		cout << "Could not load text ! " << endl;
		return false;
	}

	SDL_Color menutext2_color = { 0, 0, 0 };
	if (!menutext2.loadFromText("PLAY", "Images/lazy2.ttf", menutext2_color, 70, renderer)) {
		cout << "Could not load text ! " << endl;
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Control Guide background
	if (!control_.loadFromFile("Images/control.png", renderer)) {
		cout << "Could not load control background ! " << endl;
		return false;
	}
	else {
		control_.setBlendMode(SDL_BLENDMODE_BLEND);
	}

	if (!control.loadFromFile("Images/arrowcontrol.png", renderer)) {
		cout << "Could not load arrowcontrol texture ! " << endl;
		return false;
	}

	if (!heart.loadFromFile("Images/heart.png", renderer)) {
		cout << "Could not load heart texture ! " << endl;
		return false;
	}

	if (!dots.loadFromFile("Images/dots.png", renderer)) {
		cout << "Could not load dots texture ! " << endl;
		return false;
	}

	if (!changePoint.loadFromFile("Images/plane.png", renderer)) {
		cout << "Could not load change texture ! " << endl;
		return false;
	}

	SDL_Color controltext_color = { 0, 0, 0 };
	if (!controltext.loadFromText("CONTROL", "Images/lazy2.ttf", controltext_color, 100, renderer)) {
		cout << "Could not load control text ! " << endl;
		return false;
	}

	SDL_Color livestex_color = { 0, 0, 0 };
	if (!livestext.loadFromText("LIVES", "Images/lazy2.ttf", livestex_color, 100, renderer)) {
		cout << "Could not load lives text ! " << endl;
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Player Texture 
	if (!player_.loadFromFile_("Images/plane.png", renderer)) {
		cout << "Could not load player texture ! " << endl;
		return false;
	}

	if (!Round1BackGround.loadFromFile_("Images/bgPlay1.png", renderer)) {
		cout << "Could not load back ground ! " << endl;
		return false;
	}

	if (!Lives.loadFromFile_("Images/heartreal.png", renderer)) {
		cout << "Could not load heart_2 texture ! " << endl;
		return false;
	}

	SDL_Color Roundtext_Color = { 160, 214, 230 };
	if (!Roundtext.loadFromText("Round 1", "Images/lazy2.ttf", Roundtext_Color, 150, renderer)) {
		cout << "Could not load ROUND text ! " << endl;
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////



	return true;
}

void close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void changeBackGround1() {
	while (a > 0) {
		a -= 5;

		control_.render(0, 0, NULL, renderer);
		menu.setAlphaMod(a);
		menu.render(0, 0, NULL, renderer);
		SDL_RenderPresent(renderer);
	}
	flag++;
}

void changeBackGround2() {
	while (b > 0) {
		b -= 5;

		Round1BackGround.render_(0, 0, NULL, renderer);
		control_.setAlphaMod(b);
		control_.render(0, 0, NULL, renderer);
		SDL_RenderPresent(renderer);
	}
	flag++;
}

void Bullet_fired() {
	for (int i = 0; i < player_.getBulletlist().size(); i++) {
		vector<Bullets*> _bullet_list_ = player_.getBulletlist();
		Bullets* bullet_ = _bullet_list_.at(i);
		if (bullet_ != NULL) {
			if (bullet_->get_is_move()) {
				bullet_->render(NULL, renderer);
				bullet_->move_B(screen_width, screen_height);
			}
			else {
				if (bullet_ != NULL) {
					_bullet_list_.erase(_bullet_list_.begin() + i);
					player_.setbulletlist(_bullet_list_);

					delete bullet_;
					bullet_ = NULL;
				}
			}
		}
	}
}

