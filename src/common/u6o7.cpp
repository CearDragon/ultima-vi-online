//ws2_32.lib winmm.lib
#include "stdafx.h"
#include "u6o7.h"
#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include <direct.h>
#include <wininet.h>
#include <winreg.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#include "dmusic.h"
#include "myfile.h"
#include "myddraw.h"
#include "mytxt.h"
#include "sound.h"
#include "myddraw.h"
#include "frame.h"
#include "windows.h"

#ifdef CLIENT
// Pre-game splash screen helper (CLIENT only). See splash.h / splash.cpp.
#include "../client/splash.h"
#endif

#ifdef CONSOLE
#include <conio.h>
#endif

/* define these in the project, as it also ties in with what to link
//#define CLIENT 
//#define HOST
/*
OPTIONS
i) define CLIENT only, don't link:
  house, function_host, data_host, usecode, spellcode
  this is the "SLIM CLIENT"
ii) define HOST only, this is the "SLIM HOST" ("host" param is assumed, house.cpp must be present)
iii) define HOST & CLIENT:
a) if "host" param present; this is the "TESTING HOST" (house.cpp must be present)
b) if "host" param absent; this is the "TESTING CLIENT" (house.cpp must be present)
*/
/* #define U6O_DEBUG TRUE */ /* moved to define_both.h */

#define SECURE

/*luteijn: Disabled this pragma to help with cleaning code but that opens up an enormous can of worms...*/
#pragma warning(disable:4244) //conversion from 'unsigned long' to 'unsigned short', possible loss of data
/*luteijn: Disabled these pragma's to help with cleaning code. Made me sad.*/
#pragma warning(disable:4018) //signed/unsigned mismatch


//#define TCHAR unsigned char


#include "define_both.h"//host and client definitions
#ifdef HOST
#include "define_host.h"//host definitions
#include "use_code/use_code.h"  // host only
#include "spell_code/spell_code.h"  // host only
#endif
#ifdef CLIENT
#include "define_client.h"//client definitions
#endif


#include "data_both.h"//host and client data
#ifdef HOST
#include "data_host.h"//host data
#endif
#ifdef CLIENT
#include "data_client.h"//client data
#endif

#include "function_both.h"//host and client functions
#ifdef HOST
#include "function_host.h"//host functions
#endif
#ifdef CLIENT
#include "function_client.h"//client functions
#endif


#include "globals.inc" // all the global variables are to be grouped here

static txt *logtext = txtnew();
static txt *ltt1 = txtnew();
static int iii = 0;
//static txt *ltt2 = txtnew();

#ifdef CONSOLE
char cyn = 0;
void KeyEventProc(KEY_EVENT_RECORD ker) {
    if (ker.bKeyDown == false) {
        // Only if key is released
        if (!cyn && (char) (ker.uChar.UnicodeChar) == 'q') {
            _cprintf("Really quit? y/n\n");
            cyn = 1;
            return;
        } else if (cyn == 1 && (char) (ker.uChar.UnicodeChar) == 'y') {
            _cprintf("y\n Shutting down...\n");
            exitrequest = TRUE;
            exitrequest_noconfirm = TRUE;
            cyn = 2;
            return;
        } else if (cyn == 1 && (char) (ker.uChar.UnicodeChar) == 'n') {
            _cprintf("n\n");
            cyn = 0;
            return;
        }
    }
}
#endif

extern "C" WORD __stdcall RtlCaptureStackBackTrace(DWORD, DWORD, PVOID *, PDWORD);

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo) {
    // Build a unique, sortable timestamp suffix so successive crashes don't
    // clobber each other's logs/dumps. Format: YYYYMMDD_HHMMSS (local time).
    // Used for both the human-readable crash log and the minidump. This
    // applies to BOTH binaries (client and host) because the filter is
    // installed unconditionally at the top of _tWinMain.
    SYSTEMTIME st;
    GetLocalTime(&st);
    char stamp[32];
    sprintf(stamp, "%04u%02u%02u_%02u%02u%02u",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

    char txtName[64];
    char dmpName[64];
    sprintf(txtName, "crash_%s.txt", stamp);
    sprintf(dmpName, "crash_%s.dmp", stamp);

    // Open with "w": each crash gets its own timestamped file, so there's no
    // need to append to a shared log anymore.
    FILE *f = fopen(txtName, "w");
    if (f) {
        fprintf(f, "Unhandled Exception Crash!\n");
        fprintf(f, "Exception Code: 0x%08X\n", ExceptionInfo->ExceptionRecord->ExceptionCode);
        fprintf(f, "Exception Address: %p\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);

        void *stack[100];
        WORD frames = CaptureStackBackTrace(0, 100, stack, NULL);
        fprintf(f, "Call Stack (Pointers):\n");
        for (WORD i = 0; i < frames; i++) {
            fprintf(f, "  [%d] %p\n", i, stack[i]);
        }
        fprintf(f, "Generated %s for detailed debugging.\n", dmpName);
        fclose(f);
    }

    HANDLE hFile = CreateFileA(dmpName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId = GetCurrentThreadId();
        mdei.ExceptionPointers = ExceptionInfo;
        mdei.ClientPointers = FALSE;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &mdei, NULL, NULL);
        CloseHandle(hFile);
    }

    // Also try to write standard LOGadd with format if available
    static txt *tcrash = txtnew();
    txtNEWLEN(tcrash, 256);
    sprintf((char *) tcrash->d2, "CRASH! Code: 0x%08X Address: %p", ExceptionInfo->ExceptionRecord->ExceptionCode,
            ExceptionInfo->ExceptionRecord->ExceptionAddress);
    tcrash->l = strlen((const char *) tcrash->d2);
    LOGadd(tcrash);

    return EXCEPTION_EXECUTE_HANDLER;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR lpCmdLine,
                       int nCmdShow) {
    SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

#ifdef CLIENT
    // Pre-game splash screen (CLIENT only — the dedicated host runs without
    // any window). Displays an image from one of the default search paths
    // (see splash.cpp::kDefaultSearchPaths) for kDefaultDurationMs (3s)
    // before any heavy setup runs. All failure modes (missing image,
    // GDI+ init failure, CreateWindow failure) still honor the duration
    // contract so the game-start timing is predictable.
    //
    // The splash is suppressed for the dedicated host build path below
    // (which is selected later via the "host" command line parameter or
    // the CONSOLE define). If a future user wants to skip the splash for
    // automated testing, gate this on a new command-line flag.
    u6o::client::splash::Run(hInstance,
                             u6o::client::splash::DefaultSearchPaths(),
                             u6o::client::splash::kDefaultDurationMs);
#endif

    //temporary use variables
    static long i = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0, i8 = 0, i9 = 0;
    static long x = 0, x2 = 0, x3 = 0, x4 = 0, x5 = 0, x6 = 0, x7 = 0, x8 = 0, x9 = 0;
    static long y = 0, y2 = 0, y3 = 0, y4 = 0, y5 = 0, y6 = 0, y7 = 0, y8 = 0, y9 = 0;
    static long z = 0, z2 = 0, z3 = 0, z4 = 0, z5 = 0, z6 = 0, z7 = 0, z8 = 0, z9 = 0;
    static double d = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 = 0, d7 = 0, d8 = 0, d9 = 0;
    static unsigned char b = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0, b7 = 0, b8 = 0, b9 = 0;
    static float f = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0, f7 = 0, f8 = 0, f9 = 0;
    static file *tfh, *tfh2, *tfh3, *tfh4, *tfh5, *tfh6, *tfh7, *tfh8, *tfh9;
    static txt *t = txtnew(), *t2 = txtnew(), *t3 = txtnew(), *t4 = txtnew(), *t5 = txtnew(), *t6 = txtnew(), *t7 =
            txtnew(), *t8 = txtnew(), *t9 = txtnew();
    static object *myobj, *myobj2, *myobj3, *myobj4, *myobj5, *myobj6, *myobj7, *myobj8, *myobj9;
    static creature *crt, *crt2, *crt3, *crt4, *crt5, *crt6, *crt7, *crt8, *crt9;
    // r666
    static txt *newt1 = txtnew();

    // t111
    //object *moblistnew[20];
    //unsigned int mobcount = 0;
    //for (int mi=0; mi<20; mi++)
    //	moblistnew[mi]=NULL;

#ifdef CONSOLE /* creates a console for the dedicated host */
    HANDLE hStdin;
    DWORD cNumRead = 0;
    INPUT_RECORD irInBuf[1];
    AllocConsole();
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    RemoveMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);
    //ShowWindow(GetConsoleWindow(), SW_SHOWMINIMIZED); /* SW_HIDE , SW_SHOW, SW_SHOWMINIMIZED */
    ShowWindow(GetConsoleWindow(), SW_SHOW); /* SW_HIDE , SW_SHOW, SW_SHOWMINIMIZED */
#endif
    frame_init(); // luteijn: setup the globals originally defined in frame.h
    data_both_init(); // luteijn: setup the globals originally defined in data_both.h
#ifdef HOST
    data_host_init(); // luteijn: setup the globals originally defined in data_both.h
#endif
#ifdef CLIENT
    data_client_init(); // luteijn: setup the globals originally defined in data_client.h
#endif

    function_both_init(); // luteijn: setup the globals originally defined in function_both.h
#ifdef HOST
    function_host_init(); // luteijn: setup the globals originally defined in function_host.h
#endif
#ifdef CLIENT
    function_client_init(); // luteijn: setup the globals originally defined in function_client.h
#endif


    log2file = open2("log.txt", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
    SCRLOG_FILEONLY = TRUE;

    //check if host command line option is defined
    i = -1;
cmdline_length:
    i++;
    if ((unsigned char *) lpCmdLine[i]) goto cmdline_length;
    if (i) {
        //not really a good way to find multiple parameters, but I'm lazy so going to use it for now.
        txtNEWLEN(t, -i);
        memcpy(t->d2, lpCmdLine, i);
        txtlcase(t);
        txtset(t2, "host");
        if (txtsearch(t, t2)) {
            NEThost = 1;
        }
#ifdef CLIENT
        txtset(t2, "-l"); if (txtsearch(t, t2)) {
            leak = 1;
        }
#endif
    }

#ifndef CLIENT
    NEThost = 1; //"host" param is assumed
#endif

#ifndef HOST
    if (NEThost) {
        MessageBox(NULL, "HOST command line parameter invalid in CLIENT ONLY build", "Ultima 6 Online", MB_OK);
        exit(0);
    }
#endif

    //SHARED SETUP
    MSG msg;
    HACCEL hAccelTable;
    MyRegisterClass(hInstance);

    //2007fix hAccelTable=LoadAccelerators(hInstance,(LPCTSTR)IDC_ULTIMATE);
    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_U6O7));


//basic setup ONLY initializes static arrays or loads info from files
#include "setup_both_basic.h"

#ifdef HOST
if (NEThost){
#include "setup_host.inc"
}
#endif
#ifdef CLIENT
#include "setup_client.inc"
#endif
#include "setup_both.inc"

#ifdef CONSOLE
_cprintf (
"This is a dedicated u6o host running on port %d.\nTo quit type q or ctrl-c to force quit.\n", global_TCP_listen_port);
#endif
SCRLOG_FILEONLY=TRUE;
//begin main loop _________________________________________________________________
oldtime=timeGetTime
();
mainloop : delay_overprocess
: //check for messages again
if
(PeekMessage
(&msg
,
NULL
,
NULL
,
0
,
PM_NOREMOVE
)
)
{
		if (!GetMessage(&msg,NULL,0,0)){

#ifdef CLIENT
			if (fonts_added) {
                DeleteObject(fnt1);
                DeleteObject(fnt1naa);
                DeleteObject(fnt2);
                DeleteObject(fnt3);
                DeleteObject(fnt4);
                DeleteObject(fnt5);
                DeleteObject(fnt6);
                DeleteObject(fnt7);
                DeleteObject(systemfont);
                RemoveFontResource(".\\dr\\u6o.ttf");
                SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
                RemoveFontResource(".\\dr\\gargish.ttf");
                SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
                RemoveFontResource(".\\dr\\runes.ttf");
                SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
                RemoveFontResource(".\\dr\\u6o2.ttf");
                SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
            }
#endif

#ifdef CLIENT
			if (clientsettingsvalid){
                cltset.u6omidivolume = u6omidivolume;
                cltset.u6ovolume = u6ovolume;
                cltset.u6ovoicevolume = u6ovoicevolume;
                memcpy(&cltset.spellrecall_partymember, &spellrecall_partymember, 8);
                memcpy(&cltset.spellrecall_i, &spellrecall_i, 8);
                tfh = open2(".\\dr\\settings.bin", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                put(tfh, &cltset, sizeof(client_settings));
                close(tfh);
			
            
            }
			// WINDOW_MAXIMIZED / WINDOW_W / WINDOW_H / WINDOW_X / WINDOW_Y
			// are written from inside the WM_DESTROY handler — see WndProc.
			// They can't be written here because by the time GetMessage
			// returns 0 (WM_QUIT) Windows has already destroyed hWnd, so
			// GetWindowPlacement would fail.
#endif

#ifdef HOST
			if (NEThost) {
                closesocket(u6osocket);
                for (i = 1; i <= socketclientlast; i++) {
                    if (socketclient[i] != INVALID_SOCKET) {
                        shutdown(socketclient[i], SD_RECEIVE | SD_SEND);
                        SleepEx(2048, NULL);
                        closesocket(socketclient[i]);
                    }
                }
            }
#endif

#ifdef CLIENT
			// Fast / clean shutdown (2026-05-28): the previous shutdown path
			// blocked on SleepEx(2048) AFTER asking the socket to shut down,
			// AND never told DirectMusic to stop or release. Net effect: the
			// background MIDI kept playing for several seconds on its own
			// COM apartment thread while the OS kept client.exe locked as
			// "in use" until the sleep returned and ExitProcess ran.
			//
			// Order matters:
			//   1. Stop + release DirectMusic FIRST so the music thread is
			//      gone before the process exits (silences audio
			//      immediately; releases the EXE lock the DirectMusic
			//      synth holds).
			//   2. Best-effort stop on any DirectSound voices, then release
			//      the device so dsound.dll lets go of client.exe too.
			//   3. Then close the socket. The legacy 2048ms SleepEx was a
			//      "give the peer time to ACK the FIN" buffer that's
			//      overkill on the client (one socket; the OS-level
			//      linger handles it). 50ms is plenty.
			if (u6omidisetup &&u6omidi) {
                u6omidi->Stop();
                delete u6omidi;
                u6omidi = NULL;
                u6omidisetup = 0;
            }
			// Stop+release all live DirectSound voices and the dsnd device.
			// Implementation lives in src/client/sound.cpp where the static
			// tempsound[] array is visible.
			soundshutdown();

			shutdown (socketclient[0],SD_RECEIVE|SD_SEND); SleepEx (50,NULL); closesocket (socketclient[0]);
			WSACleanup();
			if (midiout_setup) midiOutClose(midiout_handle);
#endif

			ExitProcess(0);
		}

		if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ){
			TranslateMessage( &msg ); 
			DispatchMessage( &msg );
		}

		goto mainloop; //process next message (if available)
	}
if
(endprogram
==
TRUE
)
goto
mainloop;

#ifdef CLIENT
if (U6O_DISABLEJOYSTICK==FALSE &&!JDISABLED){ //should do only once? and after that skip this
        JDISABLED=1;
		ZeroMemory(&joy,sizeof(JOYINFOEX));
		joy.dwSize=sizeof(JOYINFOEX);
		joy.dwFlags=JOY_RETURNALL;
		joyGetPosEx(JOYSTICKID1,&joy); //joystick information
		//assume ok

		//set POV keys
		i=joy.dwPOV;
		if ((i<=6750)||((i>=29250)&&(i<=35999))){//up
			keyon[256+32]=TRUE;
			if (key_gotrelease[256+32]){
				key[256+32]=TRUE; //user must release
				key_gotrelease[256+32]=FALSE;
			}
		}else{
			keyon[256+32]=FALSE;
			key_gotrelease[256+32]=TRUE;
		}//up
		if ((i>=2250)&&(i<=15750)){//right
			keyon[256+33]=TRUE;
			if (key_gotrelease[256+33]){
				key[256+33]=TRUE; //user must release
				key_gotrelease[256+33]=FALSE;
			}
		}else{
			keyon[256+33]=FALSE;
			key_gotrelease[256+33]=TRUE;
		}//right
		if ((i>=11250)&&(i<=24750)){//down
			keyon[256+34]=TRUE;
			if (key_gotrelease[256+34]){
				key[256+34]=TRUE; //user must release
				key_gotrelease[256+34]=FALSE;
			}
		}else{
			keyon[256+34]=FALSE;
			key_gotrelease[256+34]=TRUE;
		}//down
		if ((i>=20250)&&(i<=33750)){//left
			keyon[256+35]=TRUE;
			if (key_gotrelease[256+35]){
				key[256+35]=TRUE; //user must release
				key_gotrelease[256+35]=FALSE;
			}
		}else{
			keyon[256+35]=FALSE;
			key_gotrelease[256+35]=TRUE;
		}//left


		i2=1;
		for (i=0;i<=31;i++){
			if (joy.dwButtons&i2){
				//button pressed
				keyon[256+i]=TRUE;
				if (key_gotrelease[256+i]){
					key[256+i]=TRUE; //user must release
					key_gotrelease[256+i]=FALSE;
				}

			}else{
				//button released
				keyon[256+i]=FALSE;
				key_gotrelease[256+i]=TRUE;

			}
			i2<<=1;
		}

	} //U6O_DISABLEJOYSTICK
#endif

f=
((

float
)
(timeGetTime()
-
oldtime
)
)
/
1000.0f;

f2=
1.0f
/
16.0f; //client framerate, maybe I should change this to 32 frames and rewrite the effect system.
if
(NEThost) f2 = 1.0f / 64.0f;
//host framerate Kaldosh said that it should be running at 64 fps, so I set tit to 64, but It might cause some unwanted side effects, but shouldn't
if
(f<f2) {
    f2 -= f;
    //f2=seconds we need to wait
    f2 *= 1000.0f;
    //f2=milliseconds we need to wait
    if (f2 > 15.0f) f2 = 15.0f;
    f2 += 1.0f;
    SleepEx(f2, FALSE); //CRITICAL: give processing time to other threads
    goto delay_overprocess; //delay overprocessing: 16 frame limit
}

newtime=timeGetTime
();
et=
((

float
)
(newtime
-
oldtime
)
)
/
1000.0f;

ett
+=
et;
oldtime=newtime;
SleepEx (

0
,
FALSE
); //CRITICAL: give EXTRA processing time to other threads

#ifdef HOST
if (NEThost){
#include "loop_host.cpp"
}
#endif

if
(NEThost) {
    static unsigned long client_frameskip = 0;
    client_frameskip++;
    if (client_frameskip == 4) client_frameskip = 0;
    if (client_frameskip != 0) goto mainloop;
}
#ifdef CLIENT
#include "loop_client.cpp"
#else
if
(exitrequest &&

!
inbritannia_totalplayers
)
 { /* make sure the games are saved */
      endprogram=TRUE; //useless ?
      PostQuitMessage(0);
    }
#endif
#ifdef CONSOLE /* check if the user is requesting quit */
GetNumberOfConsoleInputEvents(hStdin, &cNumRead);
    if (cNumRead) {
    /* read input only if there is something to read */
    if (ReadConsoleInput(hStdin, irInBuf, 1, &cNumRead)) {
        if (irInBuf[0].EventType == KEY_EVENT) {
            KeyEventProc(irInBuf[0].Event.KeyEvent);
        }
    }
    FlushConsoleInputBuffer(hStdin);
}
#endif
goto
mainloop;
return
0; //This line exists purely to keep the compiler happy!
}

WNDCLASSEX wcex;

ATOM MyRegisterClass(HINSTANCE hInstance) {
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = NULL; //CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC) WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    // Icons come from the resource compiler (u6o7.rc.in -> generated
    // u6o7.rc, see CMakeLists.txt icon pipeline). LoadImage with the
    // system-metric size lets Windows pick the right sub-image from the
    // multi-resolution ICO (16/20/24/32/40/48/64/96/128/256 px frames),
    // so the title bar and the taskbar each get a crisp render at the
    // active DPI without us hand-tuning anything.
    wcex.hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_U6O7),
                                   IMAGE_ICON,
                                   GetSystemMetrics(SM_CXICON),
                                   GetSystemMetrics(SM_CYICON),
                                   LR_DEFAULTCOLOR | LR_SHARED);
    wcex.hIconSm = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_SMALL),
                                     IMAGE_ICON,
                                     GetSystemMetrics(SM_CXSMICON),
                                     GetSystemMetrics(SM_CYSMICON),
                                     LR_DEFAULTCOLOR | LR_SHARED);
    wcex.hCursor = NULL; // we use a custom cursor
    wcex.hbrBackground = NULL; // we use a custom refresh
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    return RegisterClassEx(&wcex);
}

#ifdef CLIENT
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    static RECT clrect;
    hInst = hInstance;

    // Option A (single-window-mode cleanup, 2026-05-20):
    //   - Mode 1 (main classic 1024x768, hWnd2) is the only window mode now.
    //   - The desktop<=1024x768 WS_POPUP fullscreen fallback is gone; on a
    //     too-small desktop the window will simply be partially off-screen
    //     and the user can drag/resize it (we now have WS_THICKFRAME).
    //   - hWnd3 (small classic 512x384) is no longer created.
    //   - hWnd4 (N1 enhanced) is created (or rather, NOT created) over in
    //     function_client.cpp's newmodeinit; it's pinned to NULL.
    clrect.top = 0;
    clrect.left = 0;
    clrect.bottom = resyo;
    clrect.right = resxo;
    // WS_OVERLAPPEDWINDOW = caption + sysmenu + thickframe + min/max boxes,
    // giving the player full drag-resize, maximize, and minimize.
    AdjustWindowRect(&clrect, WS_OVERLAPPEDWINDOW, FALSE);
    hWnd2 = CreateWindow(szWindowClass, window_name, WS_OVERLAPPEDWINDOW,
                         0, 0, clrect.right - clrect.left, clrect.bottom - clrect.top, NULL, NULL, hInstance, NULL);

    // Attach the resource-defined menu (Actions/Help) to the client window.
    {
        HMENU appMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_U6O7));
        if (appMenu) {
            SetMenu(hWnd2, appMenu);
            DrawMenuBar(hWnd2);
        }
    }

    hWnd3 = NULL;
    // hWnd4 set NULL in function_client.cpp newmodeinit.

    hWnd = hWnd2;

    // Restore the last session's window layout from settings.txt. We
    // persist five integers in WM_DESTROY (see WndProc):
    //   WINDOW_MAXIMIZED  - 1 if last session ended in maximized state.
    //   WINDOW_X/Y/W/H    - the *restored* (non-maximized) rect.
    // Reading order matters: position+size FIRST so a maximized window
    // has a sensible un-maximize target, THEN ShowWindow with
    // SW_SHOWMAXIMIZED if applicable.
    //
    // getsetting() only populates GETSETTING_RAW when the named key is
    // found in the file, so we reset it before each call to
    // distinguish "missing" from "present and zero" (first-run case).
    //
    // We also pre-check the file's existence: getsetting() opens via
    // the legacy open() helper which pops up a MessageBox per call on
    // missing-file, and we don't want first-run to fire five popups.
    if (GetFileAttributesA("settings.txt") != INVALID_FILE_ATTRIBUTES) {
        long savedX = 0, savedY = 0, savedW = 0, savedH = 0;
        bool haveX = false, haveY = false, haveW = false, haveH = false;
        txtset(GETSETTING_RAW, "");
        getsetting("WINDOW_X");
        if (GETSETTING_RAW->l > 0) {
            savedX = (long) txtnum(GETSETTING_RAW);
            haveX = true;
        }
        txtset(GETSETTING_RAW, "");
        getsetting("WINDOW_Y");
        if (GETSETTING_RAW->l > 0) {
            savedY = (long) txtnum(GETSETTING_RAW);
            haveY = true;
        }
        txtset(GETSETTING_RAW, "");
        getsetting("WINDOW_W");
        if (GETSETTING_RAW->l > 0) {
            savedW = (long) txtnum(GETSETTING_RAW);
            haveW = true;
        }
        txtset(GETSETTING_RAW, "");
        getsetting("WINDOW_H");
        if (GETSETTING_RAW->l > 0) {
            savedH = (long) txtnum(GETSETTING_RAW);
            haveH = true;
        }

        // Need all four to commit the move. Validate against the
        // virtual screen so a window saved on a now-disconnected
        // monitor doesn't open completely off-screen. We require the
        // title bar to overlap the work area by at least 100 px in
        // each dimension — otherwise fall back to the default rect.
        if (haveX && haveY && haveW && haveH && savedW > 200 && savedH > 150) {
            int vsLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
            int vsTop = GetSystemMetrics(SM_YVIRTUALSCREEN);
            int vsRight = vsLeft + GetSystemMetrics(SM_CXVIRTUALSCREEN);
            int vsBottom = vsTop + GetSystemMetrics(SM_CYVIRTUALSCREEN);
            bool onScreen =
                    (savedX + savedW) > (vsLeft + 100) &&
                    savedX < (vsRight - 100) &&
                    (savedY + savedH) > (vsTop + 100) &&
                    savedY < (vsBottom - 100);
            if (onScreen) {
                SetWindowPos(hWnd, NULL,
                             (int) savedX, (int) savedY,
                             (int) savedW, (int) savedH,
                             SWP_NOZORDER | SWP_NOACTIVATE);
            }
        }

        int showCmd = nCmdShow;
        txtset(GETSETTING_RAW, "");
        getsetting("WINDOW_MAXIMIZED");
        if (GETSETTING_RAW->l > 0 && (long) txtnum(GETSETTING_RAW) != 0) {
            showCmd = SW_SHOWMAXIMIZED;
        }
        ShowWindow(hWnd, showCmd);
    } else {
        // First run (or settings.txt deleted): use the launcher-provided
        // default; WM_DESTROY will create the file on this session's exit.
        ShowWindow(hWnd, nCmdShow);
    }

    UpdateWindow(hWnd);


    return TRUE;
}
#endif
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    //int wmId, wmEvent;
    switch (message) {
        case WM_KILLFOCUS:
            break;
        case WM_SETFOCUS:
            break;

#ifdef CLIENT
        // RW-P1.4: widen the resize grab area. WS_OVERLAPPEDWINDOW already
        // gives us a sizing border (WS_THICKFRAME), but on Windows 10/11 the
        // *visible* frame is ~1 px and the actual grabbable border is only
        // SM_CXSIZEFRAME + SM_CXPADDEDBORDER pixels — often just 4 px — so
        // players reported it was hard to catch the edge to resize.
        //
        // We let DefWindowProc do the normal hit-test first. When it reports
        // HTCLIENT (cursor is just inside the client edge) we re-classify the
        // point as the matching border/corner if it falls within an enlarged
        // grip margin. This makes the edges and corners far easier to grab
        // without stealing clicks from anywhere but a thin strip along the
        // window's outer rim. The caption, buttons, and menu are untouched
        // because DefWindowProc only returns HTCLIENT for the play area.
        case WM_NCHITTEST: {
            LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
            if (hit != HTCLIENT) {
                // Caption, sysmenu, min/max boxes, or the real border already
                // resolved — leave them alone. Only the real outer border is a
                // resize edge; caption/buttons are not.
                cursorOverResizeBorder =
                        (hit == HTLEFT || hit == HTRIGHT || hit == HTTOP ||
                         hit == HTBOTTOM || hit == HTTOPLEFT || hit == HTTOPRIGHT ||
                         hit == HTBOTTOMLEFT || hit == HTBOTTOMRIGHT) ? 1 : 0;
                return hit;
            }

            // Grip thickness: the system border plus padding, but never less
            // than a comfortable, DPI-aware floor (~8 px at 96 DPI). Corners
            // use a slightly larger square so diagonal resize is easy to hit.
            // SM_CXPADDEDBORDER (Vista+) may be absent if this TU's SDK target
            // macros predate it; fall back to its documented index value (92).
#ifndef SM_CXPADDEDBORDER
#define SM_CXPADDEDBORDER 92
#endif
            const int sysGrip = GetSystemMetrics(SM_CXSIZEFRAME) +
                                GetSystemMetrics(SM_CXPADDEDBORDER);
            const int grip = (sysGrip > 8) ? sysGrip : 8;
            const int cornerGrip = grip * 2;

            POINT pt = {(LONG)(short) LOWORD(lParam), (LONG)(short) HIWORD(lParam)};
            RECT wr;
            GetWindowRect(hWnd, &wr);

            const bool nearLeft = pt.x < wr.left + grip;
            const bool nearRight = pt.x >= wr.right - grip;
            const bool nearTop = pt.y < wr.top + grip;
            const bool nearBottom = pt.y >= wr.bottom - grip;

            // Corners first (use the larger square so they win over edges).
            const bool inLeftCol = pt.x < wr.left + cornerGrip;
            const bool inRightCol = pt.x >= wr.right - cornerGrip;
            const bool inTopRow = pt.y < wr.top + cornerGrip;
            const bool inBottomRow = pt.y >= wr.bottom - cornerGrip;

            LRESULT zone = HTCLIENT;
            if (nearTop && inLeftCol) zone = HTTOPLEFT;
            else if (nearTop && inRightCol) zone = HTTOPRIGHT;
            else if (nearBottom && inLeftCol) zone = HTBOTTOMLEFT;
            else if (nearBottom && inRightCol) zone = HTBOTTOMRIGHT;
            else if (nearLeft && inTopRow) zone = HTTOPLEFT;
            else if (nearLeft && inBottomRow) zone = HTBOTTOMLEFT;
            else if (nearRight && inTopRow) zone = HTTOPRIGHT;
            else if (nearRight && inBottomRow) zone = HTBOTTOMRIGHT;
            else if (nearLeft) zone = HTLEFT;
            else if (nearRight) zone = HTRIGHT;
            else if (nearTop) zone = HTTOP;
            else if (nearBottom) zone = HTBOTTOM;

            // Remember whether we are over a resize edge so the client main
            // loop yields cursor ownership to Windows (see WM_SETCURSOR and the
            // cursor-assignment block in loop_client.cpp). Without this the
            // per-frame SetCursor() would immediately overwrite the system
            // resize arrows, making the edge feel un-grabbable.
            cursorOverResizeBorder = (zone == HTCLIENT) ? 0 : 1;
            return zone;
        }

        // RW-P1.4: let Windows own the cursor while it sits over a resize edge.
        // The window class has hCursor = NULL and the game re-applies a custom
        // cursor every frame, so without this the system resize arrows never
        // show. When the low word of lParam (the prior WM_NCHITTEST result) is
        // a sizing border, DefWindowProc loads the correct double-arrow cursor;
        // we return TRUE to stop further processing. Anywhere else (HTCLIENT,
        // caption, buttons) falls through to default handling.
        case WM_SETCURSOR: {
            const WORD ht = LOWORD(lParam);
            if (ht == HTLEFT || ht == HTRIGHT || ht == HTTOP || ht == HTBOTTOM ||
                ht == HTTOPLEFT || ht == HTTOPRIGHT || ht == HTBOTTOMLEFT ||
                ht == HTBOTTOMRIGHT) {
                cursorOverResizeBorder = 1;
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            if (ht == HTCLIENT) {
                cursorOverResizeBorder = 0;
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
#endif

        // RW-P1.2: enforce a sensible minimum client size when the window is
        // resizable, so the renderer never has to cope with degenerate dims.
        case WM_GETMINMAXINFO: {
            MINMAXINFO *mmi = (MINMAXINFO *) lParam;
            // Approximate non-client overhead (frame + caption); good enough
            // for a floor — Windows will refuse smaller drag-resizes.
            const long minClientW = 800;
            const long minClientH = 600;
            const long ncxOverhead = GetSystemMetrics(SM_CXSIZEFRAME) * 2;
            const long ncyOverhead = GetSystemMetrics(SM_CYSIZEFRAME) * 2 +
                                     GetSystemMetrics(SM_CYCAPTION);
            mmi->ptMinTrackSize.x = minClientW + ncxOverhead;
            mmi->ptMinTrackSize.y = minClientH + ncyOverhead;
        }
            return 0;

        // RW-P1.3: capture new client dimensions and signal the main loop to
        // run its OnClientResized handling. Surfaces are NOT touched from in
        // here — that work is deferred to the next tick of the main loop.
        //
        // Also drives the `nodisplay` flag from the actual window state so the
        // game pauses while minimized and resumes on restore — without this,
        // minimizing left the client frozen because the previous M-key handler
        // was the only thing that ever cleared `nodisplay`.
        case WM_SIZE:
#ifdef CLIENT
            if (wParam == SIZE_MINIMIZED) {
                nodisplay = TRUE;
            } else {
                nodisplay = FALSE;
            }
#endif
            if (wParam != SIZE_MINIMIZED) {
                long newW = (long) LOWORD(lParam);
                long newH = (long) HIWORD(lParam);
                if (newW != clientW || newH != clientH) {
                    clientW = newW;
                    clientH = newH;
                    dirtyClientSize = true;
                }
            }
            break;

        case WM_KEYDOWN:
        syskeydown:

#ifdef CLIENT
            if (musickeyboard_set != 255) {
                midikeyboard2[wParam & 0xFF] = musickeyboard_set;
                static file *tfh;
                tfh = open2(".\\dr\\midikeyb.bin", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                put(tfh, &midikeyboard2, 256);
                close(tfh);
                static txt *t = txtnew(), *t2 = txtnew();
                txtset(t, "Computer key(");
                txtnumint(t2, wParam & 0xFF);
                txtadd(t, t2);
                txtadd(t, ") assigned to music keyboard note(");
                txtnumint(t2, musickeyboard_set - 24);
                txtadd(t, t2);
                txtadd(t, ")");
                STATUSMESSadd(t);
                musickeyboard_set = 255;
                goto musickeyboard_done;
            }
            if ((wParam & 0xFF) == VK_ESCAPE) {
                if (playinstrument) {
                    if (clientinstrument == 0) STATUSMESSadd("You finish playing the lute.");
                    if (clientinstrument == 1) STATUSMESSadd("You finish playing the harpsichord.");
                    if (clientinstrument == 2) STATUSMESSadd("You finish playing the harp.");
                    if (clientinstrument == 3) STATUSMESSadd("You finish playing the panpipes.");
                    if (clientinstrument == 4) STATUSMESSadd("You finish playing the xylophone.");
                    playinstrument = 0;
                    if (musickeyboard->offset_x < 1024) musickeyboard->offset_x += 2048;
                    goto musickeyboard_done;
                }
            }
#endif

            keyon[wParam & 0xFF] = TRUE;
            if (key_gotrelease[wParam & 0xFF]) {
                key[wParam & 0xFF] = TRUE; //user must release
                key_gotrelease[wParam & 0xFF] = FALSE;
            }

#ifdef CLIENT
            if (playinstrument == 0) GETINPUT_update();
        musickeyboard_done:
#endif

            break;

        case WM_KEYUP:
        syskeyup:
            keyon[wParam & 0xFF] = FALSE;
            key_gotrelease[wParam & 0xFF] = TRUE;
            break;

        //F10 FIX
        case WM_SYSKEYDOWN:
            if ((wParam & 0xFF) == VK_F10) goto syskeydown;
            break;
        case WM_SYSKEYUP:
            if ((wParam & 0xFF) == VK_F10) goto syskeyup;
            break;

        case WM_MOUSEMOVE: {
            // Raw client-area pixel coordinates from Windows.
            long raw_mx = (long) (short) LOWORD(lParam);
            long raw_my = (long) (short) HIWORD(lParam);
            // Translate from window-client space into source-surface space
            // based on the current letterbox blit transform (set by refresh()).
            if (blit_scale > 0.0) {
                mx = (long) ((raw_mx - blit_offx) / blit_scale);
                my = (long) ((raw_my - blit_offy) / blit_scale);
            } else {
                mx = raw_mx;
                my = raw_my;
            }
        }
        break;

        case WM_CHAR:
            keyasc[wParam & 0xFF] = TRUE; //user must release
#ifdef CLIENT
            GETINPUT_update();
#endif
            break;

        case WM_LBUTTONDOWN:
            mbclick |= 1;
            mbheld |= 1;
            mb = mb | 1;
            break;
        case WM_LBUTTONUP:
            if (mbheld & 1) mbheld -= 1;
            mb_release |= 1;
            break;

        case WM_RBUTTONDOWN:
            mbclick |= 2;
            mbheld |= 2;
            mb = mb | 2;
            break;
        case WM_RBUTTONUP:
            if (mbheld & 2) mbheld -= 2;
            mb_release |= 2;
            break;

        case WM_MBUTTONDOWN:
            keyon[0xDA] = TRUE;
            if (key_gotrelease[0xDA]) {
                key[0xDA] = TRUE; //user must release
                key_gotrelease[0xDA] = FALSE;
            }
            break;
        case WM_MBUTTONUP:
            keyon[0xDA] = FALSE;
            key_gotrelease[0xDA] = TRUE;
            break;

        case WM_XBUTTONDOWN:
            if (HIWORD(wParam) & XBUTTON1) {
                keyon[0xDB] = TRUE;
                if (key_gotrelease[0xDB]) {
                    key[0xDB] = TRUE; //user must release
                    key_gotrelease[0xDB] = FALSE;
                }
            } //xbutton1
            if (HIWORD(wParam) & XBUTTON2) {
                keyon[0xDC] = TRUE;
                if (key_gotrelease[0xDC]) {
                    key[0xDC] = TRUE; //user must release
                    key_gotrelease[0xDC] = FALSE;
                }
            } //xbutton2
            return TRUE;
            break;

        case WM_XBUTTONUP:
            if (HIWORD(wParam) & XBUTTON1) {
                keyon[0xDB] = FALSE;
                key_gotrelease[0xDB] = TRUE;
            } //xbutton1
            if (HIWORD(wParam) & XBUTTON2) {
                keyon[0xDC] = FALSE;
                key_gotrelease[0xDC] = TRUE;
            } //xbutton2
            return TRUE;
            break;

        case 0x020A: //WM_MOUSEWHEEL
            static short delta, keyi;
            delta = (wParam >> 16);
            delta /= 120;
            wheel_move += delta;
            break;

        case WM_COMMAND:
#ifdef CLIENT
            switch (LOWORD(wParam)) {
                case IDM_ACTIONS_RESET_UI:
                    // Clear user overrides and snap anchored panels back to defaults.
                    u6o::client::g_qkstf_user_positioned = false;
                    u6o::client::g_qkstf_user_x = 0;
                    u6o::client::g_qkstf_user_y = 0;
                    u6o::client::g_volcontrol_user_positioned = false;
                    u6o::client::g_volcontrol_user_x = 0;
                    u6o::client::g_volcontrol_user_y = 0;
                    cltset.qkstf_offset_x = 32767;
                    cltset.qkstf_offset_y = 32767;
                    cltset.volcontrol_offset_x = 32767;
                    cltset.volcontrol_offset_y = 32767;
                    RepositionAnchoredPanels(backbufferW(), backbufferH());
                    InvalidateRect(hWnd, NULL, FALSE);
                    return 0;

                case IDM_ABOUT:
                    MessageBox(hWnd,
                               "Ultima VI Online\n\nUse Actions -> Reset UI to restore anchored panel positions.",
                               "About",
                               MB_OK | MB_ICONINFORMATION);
                    return 0;

                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    return 0;
            }
#endif
            return DefWindowProc(hWnd, message, wParam, lParam);

        case WM_DESTROY:
#ifdef CLIENT
            // Persist window placement to settings.txt while hWnd is still
            // valid (the post-WM_QUIT shutdown block in _tWinMain runs AFTER
            // Windows has destroyed the handle, so GetWindowPlacement would
            // fail there). We capture the *restored* rect even when the
            // window is currently maximized or minimized, so unmaximizing
            // next session lands in the same spot the user last sized it.
            {
                WINDOWPLACEMENT wp;
                wp.length = sizeof(WINDOWPLACEMENT);
                if (GetWindowPlacement(hWnd, &wp)) {
                    bool maximized =
                            (wp.showCmd == SW_SHOWMAXIMIZED) ||
                            ((wp.showCmd == SW_SHOWMINIMIZED) &&
                             ((wp.flags & WPF_RESTORETOMAXIMIZED) != 0));
                    setsetting_int("WINDOW_MAXIMIZED", maximized ? 1 : 0);
                    setsetting_int("WINDOW_X", (long) wp.rcNormalPosition.left);
                    setsetting_int("WINDOW_Y", (long) wp.rcNormalPosition.top);
                    setsetting_int("WINDOW_W",
                                   (long) (wp.rcNormalPosition.right - wp.rcNormalPosition.left));
                    setsetting_int("WINDOW_H",
                                   (long) (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top));
                }
            }
#endif
            endprogram = TRUE;
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}