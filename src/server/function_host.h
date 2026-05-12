#ifndef FUNCTION_HOST_H
#define FUNCTION_HOST_H
/* To be able to include this and the link link against the object file
* only prototypes and externs go in here, and MAYBE some inline functions 
* actual global variable definitions should be moved to u6o7.cpp itself. (#include "global_vars.h")
*/
#include <winsock2.h>
#include "define_host.h" /* HOUSEMAX etc */
#include "data_host.h"  /* od[][] etc. */
#include "data_both.h" /*types*/

// --- Socket and Connection Variables ---
extern unsigned long newsocket;
extern unsigned long newsocket_ip;
extern unsigned long tnewsocket;
extern unsigned long tnewsocket_ip;
extern HANDLE hsockets_accept;
extern DWORD idsockets_accept;
extern DWORD WINAPI sockets_accept(LPVOID null_value);

// --- Autopickup System ---
extern unsigned short AUTOPICKUPfirst;
extern unsigned short AUTOPICKUPnextfree;
extern object* AUTOPICKUPobject[65536];
extern double AUTOPICKUPett[65536]; // time at which object was added to list
extern player* AUTOPICKUPplayer[65536]; // player/npc which used the item
extern object* AUTOPICKUPpartymember[65536];
extern unsigned char AUTOPICKUPflags[65536]; // 1=not-for-sale, ...
extern unsigned char AUTOPICKUP_OBJECTVALID[1024]; // array to quickly check if an item could be an autopickup item

void AUTOPICKUPadd(object * partymember, object * obj);
object* AUTOPICKUPcheck(player * tplayer, object * obj);

// --- Object and Mover Variables ---
extern unsigned char OBJcheckflags_flags;
extern unsigned long OBJcheckflags_td;
extern object *OBJtmp, *OBJtmp2, *OBJtmp3;
extern object* OBJaddtocontainer_containermore;
extern object* MOVERNEW_OBJECT;
extern unsigned char OBJmove_allow;
extern object* OBJlist_list[65536];
extern long OBJlist_last;

// --- House System ---
extern unsigned short housex1[HOUSEMAX], housey1[HOUSEMAX], housex2[HOUSEMAX], housey2[HOUSEMAX]; // x,y limits of house
extern unsigned short housepnext[HOUSEMAX];
extern unsigned short housepx[HOUSEMAX][512], housepy[HOUSEMAX][512];
extern unsigned short housecost[HOUSEMAX]; // gold cost per REAL day
extern unsigned short houseinitialcost[HOUSEMAX];
extern unsigned int housestoragenext[HOUSEMAX];
extern unsigned short housestoragex[HOUSEMAX][HOUSESTORAGESLOTMAX], housestoragey[HOUSEMAX][HOUSESTORAGESLOTMAX];
extern unsigned char housestorageadd;
extern unsigned char housestoragerestore; // flags used when saving/restoring house items
extern unsigned short houseentrancex[HOUSEMAX], houseentrancey[HOUSEMAX];
extern unsigned short patchx, patchy; // base offset for adding objects
extern unsigned short housenumber; // current house being created
extern unsigned short basehousenumber;

void house();
unsigned char houseowner(player* p, long housenumber);
unsigned char housecheck(unsigned short x, unsigned short y);
void housesav_update();

// --- Pathfinding (WPF) ---
extern unsigned long wpf_weight[512][512];
extern unsigned char wpf_sourcedest[512][512]; // 0=not set, 1=from source, 2=from dest
extern unsigned long wpf_bestweight[512][512];
extern unsigned short wpf_nx[65536], wpf_ny[65536];
extern unsigned long wpf_nweight[65536];
extern unsigned char wpf_nsource[65536]; // 1 if source else dest
extern long wpf_lastusedn;
extern unsigned short wpf_stackn[65536];
extern long wpf_laststackedn;
extern unsigned long nweight, nextnweight;
extern unsigned char join2sourcepath[65536], join2destpath[65536];
extern unsigned char wpf_nextto;
extern npc* wpf_npc;
extern unsigned char wpf_pathfound, wpf_nodeaddflags;
extern unsigned char WPF_NEXTTO;
extern object* WPF_OBJECT;
extern unsigned char WPF_RETURN;
extern unsigned long WPF_PATHLENGTH;
extern long WPF_OFFSETX, WPF_OFFSETY;

void wpf_pathfind(unsigned char* d, long sourcex, long sourcey, long destx, long desty, long maxdistance,
                  unsigned long type, unsigned long flags);
unsigned char wpf_dangercheck(short x, short y, object* mover);

// --- Collision and Bolt Checks ---
extern unsigned short OBJcheckbolt_x, OBJcheckbolt_y;
extern float Ocb_x, Ocb_y, Ocb_gx, Ocb_gy, Ocb_l;
extern short Ocb_ix, Ocb_iy, Ocb_i, Ocb_il;
extern object* Ocbo;

bool OBJcheckbolt(unsigned short x, unsigned short y, unsigned short x2, unsigned short y2);

// --- WTfind Variables ---
extern object* OBJtl[65536];
extern unsigned long WTf_i, WTf_n, WTf_w, WTf_w2, WTf_itemn;
unsigned long WTfind(object * obj);

// --- Conversation (CON) Variables ---
extern long CONreg[256];
extern unsigned long CONerr;
extern unsigned short CONnpc;
extern unsigned long CONrnd;
extern long CONnumber;
extern unsigned long CONqual;
extern unsigned char CONpartymember;
extern unsigned long CONport;
extern unsigned short CONhousecost;
extern unsigned short CONhouseinitialcost;
extern npc* CONnpc2;

long CON_gv(long v);
void CON_sv(long v, long v2);

// --- Stealing ---
extern txt* stealing_txt;
extern unsigned char stealing_MESSAGE;
void stealing(player * tplayer, object * obj);

// --- Object Save and Respawn ---
extern long objsave_last;
extern unsigned short objsave_x[65536 * 4], objsave_y[65536 * 4];
extern object* objsave_obj[65536 * 4];
extern float objsave_wait[65536 * 4];
extern object* hirl_obj[HIRELINGS_MAX];
extern float hirl_wait[HIRELINGS_MAX];
extern object* objsave_node[65536];
extern long objsave_node_last;

void OBJsave(unsigned short x, unsigned short y);
void crtrespawn(object * obj);

// --- Wind and Stormcloak ---
extern long WINDSPELL_boltn, WINDSPELL_boltx[5], WINDSPELL_bolty[5];
extern long WINDSPELL_n, WINDSPELL_x[128], WINDSPELL_y[128];
void getwindspell(long sx, long sy, long dx, long dy);
unsigned char stormcloakcheck(unsigned short x, unsigned short y);
unsigned char stormcloakcheck2(unsigned short x, unsigned short y, player* p);

// --- Misc Functions and Variables ---
extern long roundfloat_l;
extern HANDLE hrevive_infiniteloopexit;
extern DWORD idrevive_infiniteloopexit;
extern unsigned char partyadd_checkarray[7][7];
extern object* HORSEDISMOUNT_HORSEOBJECT;
extern long houseowner_FAILVALUE;

void function_host_init(void);
void VLNKnew(void* lnks, void* lnk, unsigned long off);
void VLNKremove(void* lnk);
void VLNKsremove(void* lnks);
object* OBJnew();
unsigned char OBJadd(unsigned long x, unsigned long y, object* obj);
void OBJrelease(object * obj);
void free(player * plr);
void free(creature * crt);
void free(npc * tnpc);
void OBJcheckflags(unsigned long x, unsigned long y);
void OBJaddtocontainer(object * container, object * objecttoadd);
void BTset(long x, long y, unsigned short i);
void decrypt(txt * t4);
void addu6monsterdropitems(object * crtobj);
unsigned char movernew(unsigned short type, unsigned short x, unsigned short y, unsigned long flags);
object* OBJaddnew(unsigned long x, unsigned long y, unsigned short type, unsigned short info, unsigned long more2);
void OBJremove(object * obj);
unsigned char OBJmove(object* obj, unsigned long x, unsigned long y);
object* OBJfindlast(unsigned long x, unsigned long y);
object* OBJfindlastall(unsigned long x, unsigned long y);
unsigned long OBJlist(object * obj);
unsigned char OBJmove2(object* obj, unsigned long x, unsigned long y);
unsigned char OBJdir(object* obj, unsigned long x, unsigned long y);
unsigned long SFnew(unsigned short x, unsigned short y);
void inbritanniacheck();
unsigned char ENHANCEget_attack(object * obj);
unsigned char ENHANCEget_defense(object * obj);
unsigned short get_mp_max(unsigned short i);
unsigned char objvisible(player * p, object * myobj);
long roundfloat(float f);
DWORD WINAPI revive_infiniteloopexit(LPVOID null_value);
object* getprimarypartymemberobj(player * sourceplayer);
object* getpartymemberobj(player* sourceplayer, unsigned char n);
long getequiparmourvalue(object * obj);
long getequiph2hwepdamage(object * obj);
void partyadd(player* p, long x, long y);
void horsedismount(object * myobj);
unsigned char usinghorse(player * tplayer);
void additem(npc * tnpc, object * obj);
void additemroot(npc * tnpc, object * obj);
object* npc_to_obj(npc * tnpc, player * tplayer);
unsigned long randomchestlocation(bool tmap);
unsigned long randchestitem();
void ENHANCEnewn(object* obj, unsigned short n, unsigned short n2);
void addhireling(unsigned long x3, schedule_i* sched);
unsigned int new1_getexpdeduction(npc* npc, int option);
int getarenaid(player * player);


#endif FUNCTION_HOST_H