/*
 * menu.c
 *
 *  Created on: Jan 19, 2021
 *      Author: Alexis
 */

#include "fonts.h"
#include "menu.h"

static char *note_name[96] = {
"C0","C0#","D0","D0#","E0","F0","F0#","G0","G0#","A0","A0#","B0",
"C1","C1#","D1","D1#","E1","F1","F1#","G1","G1#","A1","A1#","B1",
"C2","C2#","D2","D2#","E2","F2","F2#","G2","G2#","A2","A2#","B2",
"C3","C3#","D3","D3#","E3","F3","F3#","G3","G3#","A3","A3#","B3",
"C4","C4#","D4","D4#","E4","F4","F4#","G4","G4#","A4","A4#","B4",
"C5","C5#","D5","D5#","E5","F5","F5#","G5","G5#","A5","A5#","B5",
"C6","C6#","D6","D6#","E6","F6","F6#","G6","G6#","A6","A6#","B6",
"C7","C7#","D7","D7#","E7","F7","F7#","G7","G7#","A7","A7#","B7"
};

T_MENU testMenu = MENU1;
int verifAffichage = 0;
float pas = 0.1; //utilisé pour modifier la valeur de x et y

//Variables externes
extern float x;
extern float y;
extern uint8_t note_index;

uint8_t init_menu(){
	uint8_t init = SSD1306_Init();
	if(init != 1){
		printf("ERROR INIT");
	}
	return init;
}

//Patch crade pour résoudre le problème de la range de x et y
void verifValue(){
	if(x < 0){x = 0;}
	if(y < 0){y = 0;}
}

void set_home_menu(){
	affichageMenu(testMenu);
}

void set_menu_SWITCH_EVENT(){
	if(testMenu == MENU1)
				testMenu = MENUPITCH;
			else if(testMenu == MENU2)
				testMenu = MENUX;
			else if(testMenu == MENU3)
				testMenu = MENUY;
			else if(testMenu == MENU3BIS)
				testMenu = MENUXBIS;
			else if(testMenu == MENUPITCH)
				testMenu = MENU1;
			else if(testMenu == MENUX)
				testMenu = MENU2;
			else if(testMenu == MENUY)
				testMenu = MENU3;
			else if(testMenu == MENUXBIS)
				testMenu = MENU3BIS;

			verifValue();
			affichageMenu(testMenu);
}

void set_menu_KNOB_EVENT_HIGH(){
	if(testMenu == MENU1 || testMenu == MENU2)
	{
		testMenu ++;
		verifValue();
		affichageMenu(testMenu);
	}
	else if(testMenu == MENU3BIS)
	{
		testMenu = MENU3;
		verifValue();
		affichageMenu(testMenu);
	}
	else if(testMenu == MENUX || testMenu == MENUY || testMenu == MENUXBIS || testMenu == MENUPITCH)
	{
		if(testMenu == MENUX || testMenu == MENUXBIS)
		{
			if(x < 1)
			{
				x += pas;
				verifValue();
				affichageMenu(testMenu);
			}
		}
		else if(testMenu == MENUY)
		{
			if(y < 1)
			{
				y += pas;
				verifValue();
				affichageMenu(testMenu);
			}
		}
		else if(testMenu == MENUPITCH)
		{
			if(note_index < 95)
			{
				note_index ++ ;
				verifValue();
				affichageMenu(testMenu);
			}
		}
	}
}

void set_menu_KNOB_EVENT_LOW(){
	if(testMenu == MENU2 || testMenu == MENU3BIS)
	{
		testMenu = MENU1;
		verifValue();
		affichageMenu(testMenu);
	}
	else if(testMenu == MENU3)
	{
		testMenu = MENU3BIS;
		verifValue();
		affichageMenu(testMenu);
	}
	else if(testMenu == MENUX || testMenu == MENUY || testMenu == MENUXBIS || MENUPITCH)
	{
		if(testMenu == MENUX || testMenu == MENUXBIS)
		{
			if(x > 0)
			{
				x -= pas;
				verifValue();
				affichageMenu(testMenu);
			}
		}
		else if(testMenu == MENUY)
		{
			if(y > 0)
			{
				y -= pas;
				verifValue();
				affichageMenu(testMenu);
			}
		}
		else if(testMenu == MENUPITCH)
		{
			if(note_index > 0)
			{
				note_index -- ;
				verifValue();
				affichageMenu(testMenu);
			}
		}
	}
}

void afficherMenu1(){
	char varX[10];
	sprintf(varX, "%f", x);
	char varPitch[10];
	sprintf(varPitch, "%s", note_name[note_index]);

	SSD1306_Clear();
	SSD1306_GotoXY(5,5);
	SSD1306_Puts("PITCH", &Font_11x18, 0);
	SSD1306_GotoXY(80,5);
	SSD1306_Puts(varPitch, &Font_11x18, 1);

	SSD1306_GotoXY(5,35);
	SSD1306_Puts("X", &Font_11x18, 1);
	SSD1306_GotoXY(80,35);
	SSD1306_Puts(varX, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

void afficherMenu2()
{
	char varX[10];
	sprintf(varX, "%f", x);
	char varPitch[10];
	sprintf(varPitch, "%s", note_name[note_index]);

	SSD1306_Clear();

	SSD1306_GotoXY(5,5);
	SSD1306_Puts("PITCH", &Font_11x18, 1);
	SSD1306_GotoXY(80,5);
	SSD1306_Puts(varPitch, &Font_11x18, 1);

	SSD1306_GotoXY(5,35);
	SSD1306_Puts("X", &Font_11x18, 0);
	SSD1306_GotoXY(80,35);
	SSD1306_Puts(varX, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}
void afficherMenu3(int test)
{
	char varX[10];
	sprintf(varX, "%f", x);
	char varY[10];
	sprintf(varY, "%f", y);
	SSD1306_Clear();
	if(test == 0)
	{
		SSD1306_GotoXY(5,5);
		SSD1306_Puts("X", &Font_11x18, 1);
		SSD1306_GotoXY(80,5);
		SSD1306_Puts(varX, &Font_11x18, 1);

		SSD1306_GotoXY(5,35);
		SSD1306_Puts("Y", &Font_11x18, 0);
		SSD1306_GotoXY(80,35);
		SSD1306_Puts(varY, &Font_11x18, 1);
	}
	else
	{
		SSD1306_GotoXY(5,5);
		SSD1306_Puts("X", &Font_11x18, 0);
		SSD1306_GotoXY(80,5);
		SSD1306_Puts(varX, &Font_11x18, 1);

		SSD1306_GotoXY(5,35);
		SSD1306_Puts("Y", &Font_11x18, 1);
		SSD1306_GotoXY(80,35);
		SSD1306_Puts(varY, &Font_11x18, 1);
	}
	SSD1306_UpdateScreen();
}
void afficherMenuPitch()
{
	char varX[10];
	sprintf(varX, "%f", x);
	char varPitch[10];
	sprintf(varPitch, "%s", note_name[note_index]);
	SSD1306_Clear();

	SSD1306_GotoXY(5,5);
	SSD1306_Puts("PITCH", &Font_11x18, 1);
	SSD1306_GotoXY(80,5);
	SSD1306_Puts(varPitch, &Font_11x18, 0);

	SSD1306_GotoXY(5,35);
	SSD1306_Puts("X", &Font_11x18, 1);
	SSD1306_GotoXY(80,35);
	SSD1306_Puts(varX, &Font_11x18, 1);

	SSD1306_UpdateScreen();
}
void afficherMenuX()
{
	char varX[10];
	sprintf(varX, "%f", x);
	char varPitch[10];
	sprintf(varPitch, "%s", note_name[note_index]);
	SSD1306_Clear();

	SSD1306_GotoXY(5,5);
	SSD1306_Puts("PITCH", &Font_11x18, 1);
	SSD1306_GotoXY(80,5);
	SSD1306_Puts(varPitch, &Font_11x18, 1);

	SSD1306_GotoXY(5,35);
	SSD1306_Puts("X", &Font_11x18, 1);
	SSD1306_GotoXY(80,35);
	SSD1306_Puts(varX, &Font_11x18, 0);

	SSD1306_UpdateScreen();
}
void afficherMenuXbis()
{
	char varX[10];
	sprintf(varX, "%f", x);
	char varY[10];
	sprintf(varY, "%f", y);
	SSD1306_Clear();

	SSD1306_GotoXY(5,5);
	SSD1306_Puts("X", &Font_11x18, 1);
	SSD1306_GotoXY(80,5);
	SSD1306_Puts(varX, &Font_11x18, 0);

	SSD1306_GotoXY(5,35);
	SSD1306_Puts("Y", &Font_11x18, 1);
	SSD1306_GotoXY(80,35);
	SSD1306_Puts(varY, &Font_11x18, 1);

	SSD1306_UpdateScreen();
}
void afficherMenuY()
{
	char varX[10];
	sprintf(varX, "%f", x);
	char varY[10];
	sprintf(varY, "%f", y);
	SSD1306_Clear();

	SSD1306_GotoXY(5,5);
	SSD1306_Puts("X", &Font_11x18, 1);
	SSD1306_GotoXY(80,5);
	SSD1306_Puts(varX, &Font_11x18, 1);

	SSD1306_GotoXY(5,35);
	SSD1306_Puts("Y", &Font_11x18, 1);
	SSD1306_GotoXY(80,35);
	SSD1306_Puts(varY, &Font_11x18, 0);

	SSD1306_UpdateScreen();
}

void affichageMenu(T_MENU menu)
{
	switch(menu)
	{
	case MENU1 :
		afficherMenu1();
		break;
	case MENU2 :
		afficherMenu2();
		break;
	case MENU3 :
		afficherMenu3(0);
		break;
	case MENU3BIS :
		afficherMenu3(1);
		break;
	case MENUX :
		afficherMenuX();
		break;
	case MENUY :
		afficherMenuY();
		break;
	case MENUXBIS :
		afficherMenuXbis();
		break;
	case MENUPITCH :
		afficherMenuPitch();
		break;
	default :
		SSD1306_Clear();
		//SSD1306_AfficherMot("ERROR");
		SSD1306_UpdateScreen();
	}
}
