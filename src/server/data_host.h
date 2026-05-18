#pragma once
#ifndef DATA_HOST_H
#define DATA_HOST_H
#include "define_host.h" /*SAVESLOTLAST etc */
#include "mytxt.h" /* txt* etc */
#include "data_structs_host.h"
#include "data_shims_host.h"

void data_host_init();

#ifndef CLIENT
extern unsigned char u6omidisetup;
extern HWND hWnd;
extern HWND hWnd2;
extern HWND hWnd3;
extern HWND hWnd4;
extern RECT desktop_rect;
extern bool smallwindow;
extern unsigned char u6ovolume;
#endif // CLIENT
#endif // DATA_HOST_H
