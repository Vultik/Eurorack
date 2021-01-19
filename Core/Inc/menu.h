/*
 * menu.h
 *
 *  Created on: Jan 19, 2021
 *      Author: Alexis
 */

#include "ssd1306.h"
#include <stdio.h>


typedef enum{
	MENU1, MENU2, MENU3, MENU3BIS, MENUPITCH, MENUX, MENUY, MENUXBIS
}T_MENU;

void afficherMenu1();
void afficherMenu2();
void afficherMenu3(int test);
void afficherMenuPitch();
void afficherMenuX();
void afficherMenuXbis();
void afficherMenuY();
void affichageMenu(T_MENU menu);
uint8_t init_menu();
void verifValue();
void set_menu_SWITCH_EVENT();
void set_menu_KNOB_EVENT_HIGH();
void set_menu_KNOB_EVENT_LOW();
void set_home_menu();

