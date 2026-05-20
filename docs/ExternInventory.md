# Server Globals Extern Inventory

*Generated for Refactor Plan P3.1*

This file maps the ~120 `extern` declarations found in `src/server/data_host.h` to the 18 specific functional categories listed in the modernization plan. This serves as the roadmap for Phase P3 encapsulation.

## 1. WorldGrid
```cpp
extern unsigned char btu6[1024][1024];
extern unsigned char btu60[256][256];
extern object* od[1024][2048];
extern unsigned char btflags[256];
extern unsigned char chunks[1024][8][8];
extern unsigned char u6wci2[8][8][16][8][3];
extern unsigned short u6wci[128][128];
extern unsigned char u60ci2[32][16][3];
extern unsigned short u60ci[32][32];
extern objentry ol[32768];
extern long oln;
extern object* oul[65536];
extern long ouln;
extern object* f_oul[65536];
extern long f_ouln;
```

## 2. ObjectPool
```cpp
extern object objb[524228];
extern long objb_last;
extern unsigned long objb_free[524228];
extern long objb_free_last;
```

## 3. VlnkPool
```cpp
extern void* vlnkb_lnk[65536];
extern unsigned long vlnkb_off[65536];
extern void* vlnkb_lnks[65536];
extern long vlnkb_last;
extern unsigned long vlnkb_free[65536];
extern long vlnkb_free_last;
```

## 4. ResurrectQueue
```cpp
extern object* resu[65536];
extern object* resu_body[65536];
extern unsigned short resu_body_type[65536];
extern float resu_wait[65536];
extern player* resu_player[65536];
extern unsigned char resu_partymember[65536];
extern unsigned short resu_x[65536];
extern unsigned short resu_y[65536];
extern long nresu;
```

## 5. RespawnQueue
```cpp
extern object* respawn[1200];
extern unsigned short respawn_delay[1200];
extern long respawn_last;
```

## 6. MoongateState
```cpp
extern unsigned char showmoongates;
extern unsigned short moongatex[8], moongatey[8];
extern object* moongate[8][2];
extern unsigned char moonphase;
```

## 7. WindState
```cpp
extern char windx;
extern char windy;
extern unsigned char windnew;
```

## 8. HouseSaves
```cpp
extern housesav_info housesav[256];
```

## 9. EconomyTables
```cpp
extern unsigned char economy_setup[1024][4];
extern long economy_limit[1024][4];
extern long economy_value[1024][4];
extern long economy_change[1024][4];
```

## 10. ScheduleTables
```cpp
extern schedule_i schedule[256][32];
extern schedule_i schedule2[1024][32];
extern long newschedule2;
```

## 11. NpcRegistry
```cpp
extern unsigned long npci[256];
extern unsigned char* npcinf;
extern npcbin_i* npcbin;
extern unsigned short npcobj[256];
extern crtenum_struct crtenum[1073];
extern object* crtenum_pathok_castok[1024];
extern short crtenum_pathok_castok_i;
extern object* crtenum_pathok[1024];
extern short crtenum_pathok_i;
extern object* crtenum_castok[1024];
extern short crtenum_castok_i;
```

## 12. SfxBuffer
```cpp
extern unsigned char sfbuffersend;
extern float sfbufferwait;
extern unsigned long sfi[256][256];
extern sfxtype sf[65536];
extern unsigned long sf_playerid[65536];
extern long sfn;
extern unsigned long sfx_playerid[256];
```

## 13. WizardEyeState
```cpp
extern unsigned char wizardeyesi;
extern unsigned short wizardeyesx[256];
extern unsigned short wizardeyesy[256];
extern unsigned char wizardeyesi2;
extern unsigned char wizardeyesadded;
extern object* wizardeyesobj;
```

## 14. AutoSetupState
```cpp
extern unsigned short autosetup;
extern bool autosetup_next;
extern unsigned short autosetup_counter;
extern bool autospell; // related conceptually
```

## 15. KeybTarget
```cpp
extern char ktar_x[768];
extern char ktar_y[768];
extern float ktar_xydis[768];
```

## 16. MoverNew
```cpp
extern unsigned long MOVERNEW_GLOBALFLAGS;
extern unsigned char MOVERNEW_ERROR;
extern unsigned char mover_blood[1024];
extern unsigned short mover_body[1024];
```

## 17. ReviveLoopExit
```cpp
extern unsigned long revive_infiniteloopexit_i;
extern unsigned long revive_infiniteloopexit_i2;
extern unsigned short revive_infiniteloopexit_i3;
```

## 18. Misc
These will either become their own sub-structs inside `ServerState` or move to relevant managers:

**Account & Login Flow:**
```cpp
extern unsigned char save_buffer[SAVESLOTLAST + 1];
extern txt* save_username[SAVESLOTLAST + 1];
extern txt* save_password[SAVESLOTLAST + 1];
extern txt* save_name[SAVESLOTLAST + 1];
extern unsigned long save_exp[SAVESLOTLAST + 1];
extern unsigned long save_bytes[SAVESLOTLAST + 1];
extern unsigned char save_dump;
extern unsigned char login_dead_callback;
extern unsigned short save_version;
extern unsigned char encryptcode[65536];
extern unsigned char format_usernames;
```

**Player Creation Scratchpad:**
```cpp
extern txt* tname;
extern txt* tusername;
extern txt* tuserpassword;
extern txt* tnewuserpassword;
extern unsigned char tcustomportrait_upload;
extern unsigned short tcustomportrait[3584];
extern unsigned long tnamecolour;
extern unsigned char tmale_female;
extern unsigned short tport;
extern unsigned short ttype;
extern unsigned char tcreatecharacter;
```

**Server Identity & Players:**
```cpp
extern txt* admins[ADMINSMAX];
extern txt* motd;
extern txt* inbritannia;
extern long inbritannia_totalplayers;
extern unsigned long U6ONEWID;
extern player* playerlist[1024];
extern long playerlist_last;
extern unsigned char party_ok[8];
```

**Spells & Reagents:**
```cpp
extern unsigned char cast_spell;
extern unsigned char staff_cast_spell;
extern long CASTSPELL_SPELLTYPE;
extern unsigned long treagent[8];
extern unsigned long tspell[256];
extern bool spellattcrt;
extern unsigned char itemused;
```

**Doors, Levers & Environment:**
```cpp
extern long doorclose_last;
extern float doorclose_wait[1024];
extern object *doorclose_obj[1024], *doorclose_obj2[1024];
extern unsigned short doorclose_oldtype[1024], doorclose_oldtype2[1024];
extern long leverchange_last;
extern float leverchange_wait[1024];
extern object* leverchange_obj[1024];
extern unsigned short leverchange_oldtype[1024];
extern object* portcullis[256][16];
extern object* lever[256][16];
extern object* efield[256][16];
extern object* eswitch[256][16];
```

**Map Specific / Orbit:**
```cpp
extern unsigned short orbx[5][5];
extern unsigned short orby[5][5];
extern object* nuggetsfix;
```

**Object Creation / Tracking Temp:**
```cpp
extern unsigned long objr_x;
extern unsigned long objr_y;
extern unsigned long ol_tag;
extern unsigned long ol_tag_prev;
extern object* newll;
extern unsigned long mycount;
```

## Win32 Stubs (Dead in server)
These are wrapped in `#ifndef CLIENT` but are meaningless for the host logic. Slated for deletion in P3.4.
```cpp
extern unsigned char u6omidisetup;
extern HWND hWnd;
extern HWND hWnd2;
extern HWND hWnd3;
extern HWND hWnd4;
extern RECT desktop_rect;
extern bool smallwindow;
extern unsigned char u6ovolume;
```
