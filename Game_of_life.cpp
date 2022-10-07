#include <allegro.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <allegro.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

const int b_screen = 500;
const int h_screen = 500;
const int d = 500;
int zoom=1;
BITMAP* buffer;
int step = 0;

struct cell{
	int i;
	int j;
};

const int neighborhood_size=9;
cell vicinato[neighborhood_size];

//STATES -------

int read_matrix[d][d];   // Read  Matrix
int write_matrix[d][d];  // Write Matrix

//ALLEGRO COLOR -----

int white;
int red;
int black;

void create_neighborhood(int i, int j){

	vicinato[0].i = i;
	vicinato[0].j = j;

	vicinato[1].i = i-1;
	vicinato[1].j = j;

	vicinato[2].i = i;
	vicinato[2].j = j-1;

	vicinato[3].i = i;
	vicinato[3].j = j+1;

	vicinato[4].i = i+1;
	vicinato[4].j = j;

	vicinato[5].i = i-1;
	vicinato[5].j = j-1;

	vicinato[6].i = i+1;
	vicinato[6].j = j-1;

	vicinato[7].i = i+1;
	vicinato[7].j = j+1;

	vicinato[8].i = i-1;
	vicinato[8].j = j+1;
}

int getToroidal(int i, int size){

	if(i < 0){
		return i+size;
	}else{
		if(i > size-1){
			return i-size;
		}
	}
	return i;
}

void transiction_function(){
	for (int y = 0; y < d; ++y) {
		for (int x = 0; x < d; ++x) {
				create_neighborhood(y,x);
				int sum = 0;
				for (int n = 1; n < neighborhood_size; ++n) {
					int indexi = getToroidal(vicinato[n].i,d);
					int indixj = getToroidal(vicinato[n].j,d);
					sum += read_matrix[indexi][indixj];
				}
				
				if((sum == 3) || (sum==2 && read_matrix[y][x] == 1) )
					write_matrix[y][x] =1;
				else
					write_matrix[y][x] =0;
			}
	}
}

void swap(){
	for (int y = 0; y < d; ++y) {
		for (int x = 0; x < d; ++x) {
			read_matrix[y][x] = write_matrix[y][x];
		}
	}
}

void global_transiction_function(){
	transiction_function();
	swap();
	step++;
}

void initGameOfLife()
{
//  Glider Game of life
//
//	for (int y = 0; y < d; ++y) {
//		for (int x = 0; x < d; ++x) {
//			read_matrix[y][x]=0;
//			write_matrix[y][x]=0;
//		}
//	}
//	read_matrix[1][3] = 1;
//	read_matrix[2][1] = 1;
//	read_matrix[2][3] = 1;
//	read_matrix[3][2] = 1;
//	read_matrix[3][3] = 1;
//
//	write_matrix[1][3] = 1;
//	write_matrix[2][1] = 1;
//	write_matrix[2][3] = 1;
//	write_matrix[3][2] = 1;
//	write_matrix[3][3] = 1;
	
	for (int y = 0; y < d; ++y) {
		for (int x = 0; x < d; ++x) {
			int state = rand()%2;
			read_matrix[y][x]=state;
			write_matrix[y][x]=state;
		}
	}

}

// INIT ALLEGRO
void initAllegro(){
	allegro_init();
	install_keyboard();
	install_mouse();
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, b_screen, h_screen, 0 ,0 );
	buffer = create_bitmap(b_screen,h_screen);
	red = makecol(255,0,0);
	white = makecol(255,255,255);
	black = makecol(0,0,0);
}

//DRAW
void drawwithAllegro(){
	for (int y = 0; y < d; ++y) {
		for (int x = 0; x < d; ++x) {
			switch(read_matrix[y][x]){
			case 0:
				rectfill(buffer,y*zoom,x*zoom,y*zoom+zoom,x*zoom+zoom,white);
				break;

			case 1:
				rectfill(buffer,y*zoom,x*zoom,y*zoom+zoom,x*zoom+zoom,red);
				break;
			}
		}
	}

	textprintf_ex(buffer, font, 0 ,0, white, black, "step %d ", step);
	blit(buffer,screen, 0, 0,0,0,b_screen, h_screen);
}


int main() {
	initAllegro();
	initGameOfLife();
	drawwithAllegro();
	bool pressed_p_button=false;
	int microsec = 100000;

	while(!key[KEY_ESC]){
		
		if(key[KEY_P])
			pressed_p_button=true;

		if(key[KEY_R])
			pressed_p_button=false;

		if(!pressed_p_button)
			global_transiction_function();

		drawwithAllegro();
	}

	return 0;
}
END_OF_MAIN()
