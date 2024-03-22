#include "function_client.h"
//#include <windows.h>
#pragma warning(disable: 4018 4244)
void function_client_init(void){
  GETINPUT_tab_pressed=0;
  GETINPUT_txt=NULL;
  GETINPUT_enterpressed=NULL;
  GETINPUT_old=NULL; //used to detect new pointers
  GSs=0;GSx=0;GSy=0;
  midikeyboard_set=-1;
  musickeyboard_set=255;
  clientinstrument=0;
  playinstrument=0;
  midipause=0;
  getsound_MOVERSOUND=0;
  AMBIENTLIGHT_LIGHTVALUE=15;
  AMBIENTLIGHT_SHOWSUN=FALSE;
  STATUSMESSdisplaying=txtnew();//the message currently being displayed
  STATUSMESSwait=0.0f;
  STATUSMESSskipok=0;//the message will be skipped if any messages are pending
  STATUSMESSt=txtnew();//temp txt for building messages (included to aid conversion from older system)
  STATUSMESSpending=txtnew();
  GETSETTING_RAW=txtnew();//the actualt text between the square brackets [...]
  li2_t=txtnew();

}; 

//for compatibility the client supports this function
object *OBJnew_local(){
static object *newobject;
newobject=(object*)malloc(sizeof(object));
ZeroMemory(newobject,sizeof(object));
return newobject;
}

//GetInput functions
//tab_pressed allows program to trap the tab key
//once trapped it also counts as an enterpressed, so serves a dual purpose
//otherwise tab key inserts an undefined amount of spaces
//it MUST be set after a call to getinput_setup
void GETINPUT_setup(txt* input_pointer,void* enterpressed_pointer,unsigned long maxlength){
GETINPUT_txt=input_pointer;
GETINPUT_enterpressed=(unsigned char*) enterpressed_pointer;
GETINPUT_maxlength=maxlength;
GETINPUT_tab_pressed=0;
}
txt* GETINPUT_current(){
return GETINPUT_txt;
}
void GETINPUT_stop(){
if (GETINPUT_enterpressed) {GETINPUT_enterpressed[0]=TRUE; GETINPUT_enterpressed=NULL;}
GETINPUT_txt=NULL;
}

// luteijn: which of these? :)
#include "new_getspr.inc"
//#include "old_getspr.inc"

bool keyhit(unsigned short k){ //returns TRUE if key has been pressed
  if (key[k]==FALSE) return FALSE;
  key[k]=FALSE; return TRUE;
}

bool u6okeyhit(unsigned char i){ //returns TRUE if key has been pressed
  if (U6OK[i][0]){
    if (U6OK[i][1]==0){ //only single key required
      if (key[U6OK[i][0]]==FALSE) return FALSE;
      key[U6OK[i][0]]=FALSE; return TRUE;
    }
    //double key combo?
    if (keyon[U6OK[i][0]]){
      if (key[U6OK[i][1]]==FALSE) return FALSE;
      key[U6OK[i][1]]=FALSE; return TRUE;
    }//first key held
  }//exists
  return FALSE;
}

bool u6okeyon(unsigned char i){ //returns TRUE if key has been pressed
  if (U6OK[i][0]){
    if (U6OK[i][1]==0){ //only single key required
      if (keyon[U6OK[i][0]]) return TRUE;
      return FALSE;
    }
    //double key combo?
    if (keyon[U6OK[i][0]]){
      if (keyon[U6OK[i][1]]) return TRUE;
    }//first key held
  }//exists
  return FALSE;
}

void u6okeyseton(unsigned char i){
  if (U6OK[i][0]){//exists
    if (U6OK[i][1]==0){//only single key required
      keyon[U6OK[i][0]]=1;
    }
    //double key combo?
    keyon[U6OK[i][0]]=1;
    keyon[U6OK[i][1]]=1;
  }//exists
}

void u6okeysetoff(unsigned char i){
  if (U6OK[i][0]){//exists
    if (U6OK[i][1]==0){//only single key required
      keyon[U6OK[i][0]]=0;
    }
    //double key combo?
    keyon[U6OK[i][0]]=0;
    keyon[U6OK[i][1]]=0;
  }//exists
}

void GETINPUT_update(){
  static long i,i2;
  if (GETINPUT_txt!=NULL){ //GETINPUT active, trap ascii keys
    if (GETINPUT_txt!=GETINPUT_old){
      for (i=0;i<=65535;i++) {keyasc[i]=FALSE;} //clear ascii buffer
    }

    //check for PASTE
    if (keyon[VK_SHIFT]){
      if (keyhit(VK_INSERT)){
	goto getinput_paste;
      }
    }
    if (keyon[VK_CONTROL]){
      keyasc[86]=FALSE; keyasc[118]=FALSE;
      if (keyhit(86)){
getinput_paste:
	static unsigned char *getinput_clipboardtxt;
	if(OpenClipboard(NULL))
	{
	  getinput_clipboardtxt=(unsigned char*)GetClipboardData(CF_TEXT);
	  if (getinput_clipboardtxt){
	    i=0;
getinput_paste_next:
	    if (i2=getinput_clipboardtxt[i]){
	      if ((i2>=32)&&(i2<=126)){
		if ((GETINPUT_txt->l<GETINPUT_maxlength)||GETINPUT_maxlength==0){
		  txtNEWLEN(GETINPUT_txt,GETINPUT_txt->l+1);
		  GETINPUT_txt->d[GETINPUT_txt->l-1]=i2;
		}
	      }
	      i++; goto getinput_paste_next;
	    }
	  }
	  CloseClipboard();
	}
	for (i=0;i<=65535;i++){
	  if ((i<0x25)||(i>0x28)){
	    if (i!=VK_CONTROL){
	      if (i!=VK_SHIFT){
		key[i]=FALSE; keyon[i]=FALSE;
	      }
	    }
	  }
	} //clear pressed key buffers
      }
    }



    for (i=32;i<=126;i++){
      if (keyasc[i]){
	keyasc[i]=FALSE;
	if ((GETINPUT_txt->l<GETINPUT_maxlength)||GETINPUT_maxlength==0){
	  txtNEWLEN(GETINPUT_txt,GETINPUT_txt->l+1);
	  GETINPUT_txt->d[GETINPUT_txt->l-1]=i;
	}
	for (i2=0;i2<=65535;i2++) if ((i2<0x25)||(i2>0x28)) {key[i2]=FALSE; keyon[i2]=FALSE; } //clear pressed key buffers
      }}
    if (keyasc[8]){
      keyasc[8]=FALSE;
      if (GETINPUT_txt->l!=0) txtNEWLEN(GETINPUT_txt,GETINPUT_txt->l-1);
      for (i=0;i<=65535;i++) if ((i<0x25)||(i>0x28)) {key[i]=FALSE; keyon[i]=FALSE; } //clear pressed key buffers
    }

    if (keyasc[9]){//tab
      keyasc[9]=FALSE;
      if (GETINPUT_tab_pressed==GETINPUT_TAB_PRESSED_INIT){
	GETINPUT_tab_pressed=GETINPUT_TAB_PRESSED;
	goto getinput_tabreturn;
      }
      if (GETINPUT_tab_pressed==GETINPUT_TAB_PRESSED_STOP){
	for (i=1;i<=8;i++){
	  if ((GETINPUT_txt->l<GETINPUT_maxlength)||GETINPUT_maxlength==0){
	    txtNEWLEN(GETINPUT_txt,GETINPUT_txt->l+1);
	    GETINPUT_txt->d[GETINPUT_txt->l-1]=32;
	  }
	}//i
      }
      for (i=0;i<=65535;i++) if ((i<0x25)||(i>0x28)) {key[i]=FALSE; keyon[i]=FALSE; } //clear pressed key buffers
    }//tab

    if (u6okeyhit(U6OK_TALK_DONE)){
getinput_tabreturn:
      GETINPUT_txt=NULL;
      if (GETINPUT_enterpressed!=NULL){
	GETINPUT_enterpressed[0]=TRUE;
	GETINPUT_enterpressed=NULL;
      }
      for (i=0;i<=65535;i++) if ((i<0x25)||(i>0x28)) {key[i]=FALSE; keyon[i]=FALSE; } //clear pressed key buffers
    }

  }
  GETINPUT_old=GETINPUT_txt;
}




void mididown(unsigned char instrument,unsigned char key){



  if (midiout_setup) midiOutShortMsg(midiout_handle,0x007E0090+key*256+instrument);

  if (midipause==0){
    midipause=1;
    if (U6O_DISABLEMUSIC==FALSE){
      u6omidi->Stop();
    }
  }

  midikeystack[instrument][key]++;
  midikeywait[instrument][key]=ett;
}

void midiup(unsigned char instrument,unsigned char key){
  if (key==255) return;


  if (midikeystack[instrument][key]){
    midikeystack[instrument][key]--;
    if (midikeystack[instrument][key]==0){

      if (instrument==3) if (midiout_setup) midiOutShortMsg(midiout_handle,0x00000080+key*256+instrument);


    }//midikeystack[instrument][key]==0
  }//midikeystack[instrument][key]

}//midiup

void LIGHTnew(unsigned short x,unsigned short y,unsigned long light_data_offset, unsigned short x_axis_size){
  if (timelval==0) return;
  static long asm_copy_vc_bytesx,asm_copy_vc_sourceoffset,asm_copy_vc_destoffset,asm_copy_vc_sourceskip,asm_copy_vc_destskip,asm_copy_vc_rows;
  static long x2,y2,x3,y3,x4,y4;
  static long xoff,yoff;
  xoff=x-x_axis_size/2;
  xoff<<=5;
  yoff=y-x_axis_size/2;
  yoff<<=5;
  x_axis_size<<=5;
  x2=xoff;//start dest x offset
  x3=0;//start source x offset
  x4=x_axis_size; //displayed "pixels" of x axis
  if (xoff<0){x4+=xoff; x2=0; x3=-xoff;}
  if ((xoff+x_axis_size)>1024) x4-=xoff+x_axis_size-1024;
  y2=yoff;//starting dest y offset
  y3=0;//starting source y offset
  y4=x_axis_size; //rows on screen
  if (yoff<0){y4+=yoff; y2=0; y3=-yoff;}
  if ((yoff+x_axis_size)>768) y4-=yoff+x_axis_size-768;
  asm_copy_vc_bytesx=x4;
  asm_copy_vc_sourceskip=x_axis_size-asm_copy_vc_bytesx;
  asm_copy_vc_destskip=1024-asm_copy_vc_bytesx;
  asm_copy_vc_sourceoffset=y3*x_axis_size+light_data_offset+x3;
  asm_copy_vc_destoffset=(y2<<10)+(unsigned long)&ls+x2;
  asm_copy_vc_rows=y4;
  _asm{
    push esi
      push edi
      push ebx
      mov ecx,asm_copy_vc_rows
      mov edx,asm_copy_vc_bytesx
      mov esi,asm_copy_vc_sourceoffset
      mov edi,asm_copy_vc_destoffset
      asm_light1:
      mov ebx,esi
      add ebx,edx
      asm_light0:
      mov eax,[esi]
      add [edi],eax
      add esi,4
      add edi,4
      cmp esi,ebx
      jne asm_light0
      add edi,asm_copy_vc_destskip
      add esi,asm_copy_vc_sourceskip
      dec ecx
      jnz asm_light1
      pop ebx
      pop edi
      pop esi
  }//_asm
}//LIGHTnew



unsigned short objgettype(unsigned short type,unsigned char dir,unsigned char frm){
  switch (type) {
  case OBJ_SKIFF: 
    return type+(dir<<10);
  case OBJ_SHIP: 
    return type+((dir*2+9)<<10);
  case OBJ_GARGOYLE_WINGED: 
    return type+((dir*12)<<10)+(frm<<12)+3*1024;
  case OBJ_GARGOYLE_WINGLESS:
    return type+((dir*3)<<10)+(frm<<10);
  case OBJ_CYCLOPS: 
    return type+(dir<<13)+(frm<<12)+3*1024;
  case OBJ_DRAGON:
    return type+(dir<<11);
  case OBJ_BALLOON:
    return type+3*1024;//hot air balloon
  }
  switch (obji[sprlnk[type]].v4) {
    case 1: //"avatar"
      return type+(dir<<12)+((frm&3)<<10);
    case 4: //"rabbit"
      return type+(dir<<10);
    case 6: //"drake"
      return type+(dir<<11);
    case 2: //"rat" type
    case 8: //"ant" type
      return type+(dir<<11)+(frm<<10);
  }
  // normal case:
  return type+(frm<<10);
}

unsigned char objgetnextframe(unsigned short type,unsigned char frm){
  switch (type) {
    case OBJ_GARGOYLE_WINGED:
      frm++; if (frm>2) frm=0;
      return frm;
    case OBJ_GARGOYLE_WINGLESS:
      frm++; if (frm>2) frm=0;
      return frm;
    case OBJ_CYCLOPS:
      frm++; if (frm>1) frm=0;
  }
  switch (obji[sprlnk[type]].v4) {
    case 1://"avatar" type
      if (frm&128){//special frame check
	frm=0; return frm;
      }
      frm++;
      if (frm>2){
	frm=1+128; return frm;
      }
      return frm;
    case 2://"rat" type
    case 8: //"ant" type
      frm++; if (frm>1) frm=0;
      return frm;
  }
  return frm;//return frm, mover does not support multiple frames
}


void getsound(unsigned short type,long x,long y){
//  static long z; // luteijn: I doubt static is needed.
  long z;
  static unsigned char SNDticktock_done=FALSE;
  static unsigned char SNDfire_done=FALSE;

  if (!vischeck[x][y]) return;
  if (int(ett)==int(ett-et)){
    SNDticktock_done=FALSE;
    SNDfire_done=FALSE;
    return;
  };

  switch (type&1023) {
    case OBJ_FOUNTAIN:
      soundplay2(u6osound[3],u6osound_volume[3]);
      return;
    case OBJ_PROTECTION_FIELD:
      soundplay2(u6osound[33],u6osound_volume[33]);
      return;
    case OBJ_ELECTRIC_FIELD:   
      soundplay2(u6osound[5],u6osound_volume[5]);
      return;
    case OBJ_LOG_SAW:
      soundplay2(u6osound[15],u6osound_volume[15]);
      return;
    case OBJ_CLOCK:
      if (type&1024) { // one part to tick is enough..
	if (SNDticktock_done==FALSE){
	  soundplay2(u6osound[17],u6osound_volume[17]);
	  SNDticktock_done=TRUE;
	}

      }
      return;
      /* luteijn: these fire checks are in a particular order to allow to fall through properly */
    case OBJ_COOKFIRE:
      if ((!(type&(5<<10))) && (!(type&(11<<10)))) {
	return;
	  }
    case OBJ_CAMPFIRE:
    case OBJ_FIREPLACE:
      if (!(type&1024)) { //doused so no sound
	return; 
      }
    case OBJ_BRAZIER:
      if (!(type>>10)) {// this also makes holy braziers sound
        return;
      } 
    case OBJ_FIRE_FIELD:
    case OBJ_FIRE:
    case OBJ_OVEN:
    case OBJ_VOLCANO:
    case OBJ_STOVE:
    case OBJ_LAMPPOST:
      if (((type&1023)==OBJ_LAMPPOST) && (!(type&(3<<10)))) { // only lamp part is noisy
	return;
      }
      if (SNDfire_done==FALSE){
	soundplay2(u6osound[16],u6osound_volume[16]);
	SNDfire_done=TRUE;
      }
      return;
  }


  if (getsound_MOVERSOUND==0) return;

  z=0;//flap
  if ((type&1023)==344) z=1;
  if ((type&1023)==369) z=1;
  if ((type&1023)==411) z=1;
  if (z){
    static unsigned char SNDflap=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDflap==FALSE){
	soundplay2(u6osound[24],u6osound_volume[24]);
      }
      SNDflap=TRUE;
    }
    else{
      SNDflap=FALSE;
    }
  }

  if ((type&1023)==348){
    static unsigned char SNDsheep_bleet=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDsheep_bleet==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[11],u6osound_volume[11]);
      }
      SNDsheep_bleet=TRUE;
    }
    else{
      SNDsheep_bleet=FALSE;
    }
  }

  if ((type&1023)==428){
    static unsigned char SNDcow_bleet=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDcow_bleet==FALSE){
	if (rnd*16.0f<1.0f) soundplay2(u6osound[12],u6osound_volume[12]);
      }
      SNDcow_bleet=TRUE;
    }
    else{
      SNDcow_bleet=FALSE;
    }
  }

  if ((type&1023)==349){
    static unsigned char SNDdog=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdog==FALSE){
	if (rnd*4.0f<1.0f) soundplay2(u6osound[21],u6osound_volume[21]);
      }
      SNDdog=TRUE;
    }
    else{
      SNDdog=FALSE;
    }
  }

  if ((type&1023)==430){
    static unsigned char SNDhorse=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDhorse==FALSE){
	if (rnd*16.0f<1.0f) soundplay2(u6osound[22],u6osound_volume[22]);
      }
      SNDhorse=TRUE;
    }
    else{
      SNDhorse=FALSE;
    }
  }

  if ((type&1023)==358){
    static unsigned char SNDsnake=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDsnake==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[23],u6osound_volume[23]);
      }
      SNDsnake=TRUE;
    }
    else{
      SNDsnake=FALSE;
    }
  }

  if ((type&1023)==356){ //bird
    static unsigned char SNDbird_bleet=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDbird_bleet==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[13],u6osound_volume[13]);

      }
      SNDbird_bleet=TRUE;
    }
    else{
      SNDbird_bleet=FALSE;
    }
  }

  if ((type&1023)==388){//cat.wav
    static unsigned char SNDdone=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdone==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[25],u6osound_volume[25]);
      }
      SNDdone=TRUE;
    }
    else{
      SNDdone=FALSE;
    }
  }

  if ((type&1023)==351){//wolf.wav
    static unsigned char SNDdone=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdone==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[26],u6osound_volume[26]);
      }
      SNDdone=TRUE;
    }
    else{
      SNDdone=FALSE;
    }
  }

  if ((type&1023)==343){//insects.wav
    static unsigned char SNDdone=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdone==FALSE){
	if (rnd*8.0f<1.0f) soundplay2(u6osound[30],u6osound_volume[30]);
      }
      SNDdone=TRUE;
    }
    else{
      SNDdone=FALSE;
    }
  }

  if ((type&1023)==146){//snore.wav
    static unsigned char SNDdone=FALSE;
    if (int(ett)!=int(ett-et)){
      if (SNDdone==FALSE){
	if (rnd*32.0f<1.0f) soundplay2(u6osound[34],u6osound_volume[34]);
      }
      SNDdone=TRUE;
    }
    else{
      SNDdone=FALSE;
    }
  }

}//getsound

void getlight(unsigned short type,long x,long y){
  if (!vischeck[x][y]) return;
  
  int z=0;
  unsigned short frame=type>>10;
  type=type&1023;
  if (frame>0) { // only if the object is ON
    switch (type) {
      case OBJ_CANDLE: 
      case OBJ_BRAZIER:
      case OBJ_CAMPFIRE:
	z=11; 
	break;
      case OBJ_CANDELABRA:
	z=13;
	break;
      case OBJ_FIREPLACE:
	if (frame&1) { // frame 2 is also off 
	  z=7; 
	}
	break;
      case OBJ_CHEST:
	if (frame==3) { //(only if magic locked)
	  z=5;
	}
	break;
      case OBJ_STOVE:
	z=7;
	break;
	break;
      case OBJ_COOKFIRE:
	if (frame==4) {
	  z=9;
	}
	break;
      case OBJ_LAMPPOST:
	if (frame=3) {
	  z=13;
	}
	break;
      case OBJ_MOONGATE:
      case OBJ_RED_GATE:
	z=13;
	break;
    }
  }
  switch (type) { // always on types 
    case OBJ_MAGIC_HELM:
    case OBJ_MAGIC_SHIELD:
    case OBJ_MAGIC_ARMOUR:
    case OBJ_GLASS_SWORD:
    case OBJ_MAGIC_BOW:
    case OBJ_SNAKE_AMULET:
    case OBJ_AMULET_OF_SUBMISSION:
    case OBJ_GEM:
    case OBJ_STAFF:
    case OBJ_LIGHTNING_WAND:
    case OBJ_FIRE_WAND:
    case OBJ_PROTECTION_RING:
    case OBJ_REGENERATION_RING:
    case OBJ_INVISIBILITY_RING:
      z=3;
      break;
    case OBJ_FIRE:
    case OBJ_OVEN:
    case OBJ_HEATSOURCE:
    case OBJ_POISON_FIELD:
    case OBJ_SLEEP_FIELD:
      z=5;
      break;
    case OBJ_PROTECTION_FIELD:
    case OBJ_WISP:
      z=7;
      break;
    case OBJ_FIRE_FIELD:
      z=9;
      break;
    case OBJ_FORCE_FIELD:
      z=11;
      break;
    case OBJ_LIGHTSOURCE:
      z=13;
      break;
  }
  if ((type>=297)&&(type<=300)){ //magically locked doors
    if (frame==13) z=5;
    if (frame==15) z=5;
  }

  switch (z) {
    case 2: LIGHTnew(x,y,(unsigned long)&ls3b,3); break;
    case 3: LIGHTnew(x,y,(unsigned long)&ls3,3); break;
    case 4: LIGHTnew(x,y,(unsigned long)&ls5b,5); break;
    case 5: LIGHTnew(x,y,(unsigned long)&ls5,5); break;
    case 7: LIGHTnew(x,y,(unsigned long)&ls7,7); break;
    case 9: LIGHTnew(x,y,(unsigned long)&ls9,9); break;
    case 11: LIGHTnew(x,y,(unsigned long)&ls11,11); break;
    case 13: LIGHTnew(x,y,(unsigned long)&ls13,13); break;
  }
}//getlight

//portraits 2.0 functions
void loadportrait(unsigned short i,surf *s){
  //s is assumed to be a valid source of the original portrait which will not be deallocated/changed
  portrait_loaded[i]=TRUE;
  portrait[i]=s;
  portrait_doublesize[i]=newsurf(112,128,SURF_SYSMEM16);
  img(portrait_doublesize[i],portrait[i]);

  portrait_halfsize[i]=newsurf(28,32,SURF_SYSMEM16);
  //resample to halfsize image using brightness correction
  cls(portrait_halfsize[i],0);
  static long x,y,z,z2,z3,z4,i2,r,g,b,zz,zz2,zz3,zz4,brightness;
  for (y=0;y<=31;y++){ for (x=0;x<=27;x++){
    z=portrait[i]->o2[y*112+x*2];
    zz=(z&31)+((z>>6)&31)+((z>>11)&31);
    z2=portrait[i]->o2[y*112+x*2+1];
    zz2=(z2&31)+((z2>>6)&31)+((z2>>11)&31);
    z3=portrait[i]->o2[y*112+x*2+56];
    zz3=(z3&31)+((z3>>6)&31)+((z3>>11)&31);
    z4=portrait[i]->o2[y*112+x*2+1+56];
    zz4=(z4&31)+((z4>>6)&31)+((z4>>11)&31);
    brightness=0;
    if (zz>brightness) brightness=zz;
    if (zz2>brightness) brightness=zz2;
    if (zz3>brightness) brightness=zz3;
    if (zz4>brightness) brightness=zz4;
    brightness=brightness*12/16;
    i2=0; if (z) i2++; if (z2) i2++; if (z3) i2++; if (z4) i2++;
    if (i2>=2){
      b=0;
      if (z) b+=(z&31);
      if (z2) b+=(z2&31);
      if (z3) b+=(z3&31);
      if (z4) b+=(z4&31);
      b/=i2;
      g=0;
      if (z) g+=((z>>5)&63);
      if (z2) g+=((z2>>5)&63);
      if (z3) g+=((z3>>5)&63);
      if (z4) g+=((z4>>5)&63);
      g/=i2;
      r=0;
      if (z) r+=((z>>11)&31);
      if (z2) r+=((z2>>11)&31);
      if (z3) r+=((z3>>11)&31);
      if (z4) r+=((z4>>11)&31);
      r/=i2;
      if ((r+b+g)<brightness){
	brightness=brightness-(r+g+b);
	brightness/=3;
	r+=brightness;
	g+=brightness;
	b+=brightness;
      }
      portrait_halfsize[i]->o2[y*28+x]=b+(g<<5)+(r<<11);
    }
  }}

}

surf *getportrait(unsigned short i){
  if (portrait_loaded[i]){
    return portrait[i];
  }
  if (portrait_requested[i]==FALSE){
    portrait_requested[i]=TRUE;
    txtsetchar(portrait_request_txt,42); txtaddshort(portrait_request_txt,i);
    NET_send(NETplayer,NULL,portrait_request_txt);
  }
  return PORTRAIT_UNAVAILABLE;
}

surf *getportrait_doublesize(unsigned short i){
  if (portrait_loaded[i]){
    return portrait_doublesize[i];
  }
  if (portrait_requested[i]==FALSE){
    portrait_requested[i]=TRUE;
    txtsetchar(portrait_request_txt,42); txtaddshort(portrait_request_txt,i);
    NET_send(NETplayer,NULL,portrait_request_txt);
  }
  return PORTRAIT_UNAVAILABLE;
}

surf *getportrait_halfsize(unsigned short i){
  if (portrait_loaded[i]){
    return portrait_halfsize[i];
  }
  if (portrait_requested[i]==FALSE){
    portrait_requested[i]=TRUE;
    txtsetchar(portrait_request_txt,42); txtaddshort(portrait_request_txt,i);
    NET_send(NETplayer,NULL,portrait_request_txt);
  }
  return PORTRAIT_UNAVAILABLE;
}


unsigned char getambientlight(unsigned short x,unsigned short y){

  if ((x>=1280)&&(y>=344)&&(x<=1336)&&(y<=394)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//top floor of keep
  if ((x>=1024)&&(y>=616)&&(x<1072)&&(y<656)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//island floor II
  if ((x>=1280)&&(y>=256)&&(x<1351)&&(y<319)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//castle floor II
  if ((x>=1242)&&(y>=512)&&(x<=1265)&&(y<=538))  {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//underground of keep

  if ((x>=1808)&&(y>=424)&&(x<1902)&&(y<527)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//kat's high level maze
  if ((x>=1280)&&(y>=395)&&(x<1341)&&(y<=432)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//2nd Floor Shop in brit

  if ((x>=1024)&&(y>=0)&&(x<1072)&&(y<48)) {AMBIENTLIGHT_SHOWSUN=TRUE; return 0;}//spirituality shrine

  if ((x>=1780)&&(y>=256)&&(x<=2047)&&(y<=657)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//kat's high level dungeon underground

  if ((x>=1280)&&(y>=319)&&(x<=1291)&&(y<=334)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//"guardians" guild hall (underground)
  if ((x>=1024)&&(y>=512)&&(x<=1258)&&(y<=614)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//newbie dungeon
  if ((x>=1072)&&(y>=616)&&(x<=1141)&&(y<=669)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//island (underground)
  if ((x>=1292)&&(y>=319)&&(x<=1309)&&(y<=333)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//
  if ((x>=1310)&&(y>=319)&&(x<=1319)&&(y<=335)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//
  if ((x>=1320)&&(y>=319)&&(x<=1326)&&(y<=324)) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//
  //if ((x>=1327)&&(y>=319)&&(x<=1358)&&(y<=343))  {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//within toth's house
  
  if ((x>=1024)&&(!((y>=256)&&(x<1280)&&(y<512)))) {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//underground 
  AMBIENTLIGHT_SHOWSUN=TRUE;
  return 0;
}




void txtmakeu6ocompatible(txt* t){
  static long i,ok,x;
txtmakeu6ocompatible_loop:
  for (i=0;i<t->l;i++){
    ok=0;
    x=t->d2[i];
    if ((x>=32)&&(x<=126)) ok=1;
    if (ok==0){
      if (i!=(t->l-1)){//if not the last character, shift the rest of the string
	memcpy(&t->d2[i],&t->d2[i+1],t->l-i-1);
      }
      t->l--; t->d2[t->l]=0;
      goto txtmakeu6ocompatible_loop;
    }//ok==0
  }//i
  for (i=0;i<t->l;i++){
    x=t->d2[i];
    //if (x==64) t->d2[i]=7;//@
    if (x==92) t->d2[i]=8;//"\"
  }//i
}


void STATUSMESSadd(txt *t){
  static long i,i2;
  ps->s->GetDC(&taghdc); SelectObject(taghdc,fnt1);
  if (STATUSMESSpending->l) txtaddchar(STATUSMESSpending,13);
  i2=STATUSMESSpending->l;//starting scan position
  for (i=0;i<t->l;i++){
    txtaddchar(STATUSMESSpending,t->d2[i]);
    tagxy.cx=0; tagxy.cy=0;
    GetTextExtentPoint32(taghdc,&STATUSMESSpending->d[i2],STATUSMESSpending->l-i2,&tagxy);
    if ((tagxy.cx>=1008)&&(i<(t->l-1))){
      txtaddchar(STATUSMESSpending,46); txtaddchar(STATUSMESSpending,46); txtaddchar(STATUSMESSpending,46);
      txtaddchar(STATUSMESSpending,13);
      i2=STATUSMESSpending->l;
    }//tagxy.cx>=1008
  }//i
  ps->s->ReleaseDC(taghdc);
}

void STATUSMESSadd(const char *t){
  static long i,i2,i3;
  i3=strlen(t);
  ps->s->GetDC(&taghdc); SelectObject(taghdc,fnt1);
  if (STATUSMESSpending->l) txtaddchar(STATUSMESSpending,13);
  i2=STATUSMESSpending->l;//starting scan position
  for (i=0;i<i3;i++){
    txtaddchar(STATUSMESSpending,t[i]);
    tagxy.cx=0; tagxy.cy=0;
    GetTextExtentPoint32(taghdc,&STATUSMESSpending->d[i2],STATUSMESSpending->l-i2,&tagxy);
    if ((tagxy.cx>=1008)&&(i<(i3-1))){
      txtaddchar(STATUSMESSpending,46); txtaddchar(STATUSMESSpending,46); txtaddchar(STATUSMESSpending,46);
      txtaddchar(STATUSMESSpending,13);
      i2=STATUSMESSpending->l;
    }//tagxy.cx>=1008
  }//i
  ps->s->ReleaseDC(taghdc);
}

//X returned directly! unsigned long GETSETTING_OPTION;//a number from 0-? indicating the option chosen
//if getsetting returns non-zero it succeeded
long getsetting(const char*d){
  static file *tfh;
  static txt *t=txtnew();
  static txt *t2=txtnew();
  static txt *t3=txtnew();
  static long i,i2,i3,i4,i5;
  txtset(t3,d);
  tfh=open("settings.txt");
  if (tfh->h!=HFILE_ERROR){
getsetting_readnextline:
    i3=seek(tfh);
    if (i3<lof(tfh)){
      txtfilein(t,tfh);
      if (t->l){
	txtset(t2,"{"); i=txtsearch(t,t2); txtright(t,t->l-i);
	txtset(t2,","); i=txtsearch(t,t2); txtleft(t,i-1);
	if (txtsamewithoutcase(t,t3)){
	  seek(tfh,i3);
	  txtfilein(t,tfh);
	  txtset(t2,"["); i=txtsearch(t,t2);
	  txtright(t,t->l-i);
	  txtset(t2,"]"); i=txtsearch(t,t2);
	  txtset(GETSETTING_RAW,t); txtleft(GETSETTING_RAW,i-1); //txtright(t,t->l-i);


	  //get instruction
	  txtset(t2,"{"); i=txtsearch(t,t2); txtright(t,t->l-i);
	  txtset(t2,","); i=txtsearch(t,t2); txtright(t,t->l-i);//skip name of setting

	  i3=0;//if ==1 another instruction awaits
	  txtset(t2,","); i=txtsearch(t,t2); if (i==0) i=10000;
	  txtset(t2,"}"); i2=txtsearch(t,t2); if (i2==0) i2=10000;
	  if (i2<i) i=i2; else i3=1;
	  txtset(t3,t); txtleft(t3,i-1); txtright(t,t->l-i);


	  txtset(t2,"CHOICE"); if (txtsamewithoutcase(t3,t2)){
	    i4=0;
getsetting_choice_nextchoice:
	    txtset(t2,","); i=txtsearch(t,t2); if (i==0) i=10000;
	    txtset(t2,"}"); i2=txtsearch(t,t2); if (i2==0) i2=10000;
	    if (i2<i) i=i2; else i3=1;
	    txtset(t3,t); txtleft(t3,i-1); txtright(t,t->l-i);
	    i4++;
	    if (txtsamewithoutcase(t3,GETSETTING_RAW)){
	      close(tfh); return i4;
	    }
	    if (i3) goto getsetting_choice_nextchoice;
	    close(tfh); return FALSE;
	  }//CHOICE

	  close(tfh);
	  return FALSE;
	}//txtsamewithoutcase
      }//t->l
      goto getsetting_readnextline;
    }
    close(tfh);
  }//tfh->h!=HFILE_ERROR
  return FALSE;
}

void refresh(){
  if (smallwindow){
    if (dxrefresh){
      if (DDRAW_display_pixelformat.dwRGBBitCount!=16){
	static unsigned long pebx,pecx;
	pebx=(unsigned long)ps->o;
	pecx=(unsigned long)ps2->o; //ps2=newsurf(1024/2,768/2,SURF_SYSMEM);
	_asm{
	  mov ebx,pebx
	    mov ecx,pecx
	    mov esi,196608
	    p16to32b:
	    mov ax,[ebx]
	    mov dx,ax
	    and edx,01111100000000000b
	    shl edx,8
	    mov dx,ax
	    and dx,011111100000b
	    shl dx,5
	    mov dl,al
	    and dl,011111b
	    shl dl,3
	    mov [ecx],edx
	    add ebx,4
	    mov di,bx
	    and di,11111111111b
	    jnz p16to32b2
	    add ebx,2048
	    p16to32b2:
	    add ecx,4
	    dec esi
	    jnz p16to32b
	} //asm
	if (NEThost) img(vs,0,0,ps2); else img(vs,512,0,ps2);
      }else{ //16->16 512x384 dx
	static unsigned long pebx,pecx;
	pebx=(unsigned long)ps->o;
	pecx=(unsigned long)ps2->o; //img(ps2,ps);
	_asm{
	  mov ebx,pebx
	    mov ecx,pecx
	    mov si,384
	    mov di,512
	    p16to16c:
	    mov ax,[ebx]
	    mov [ecx],ax
	    add ebx,4
	    add ecx,2
	    dec di
	    jnz p16to16c
	    mov di,512
	    add ebx,2048
	    dec si
	    jnz p16to16c
	} //asm
	if (NEThost) img(vs,0,0,ps2); else img(vs,512,0,ps2);
      }
    }else{ //not dxrefresh
      if (DDRAW_display_pixelformat.dwRGBBitCount!=16){
	img(ps4,ps);   //ps4=newsurf(1024/2,768/2,SURF_SYSMEM16);
	refresh(ps4);
      }else{
	img(ps2,ps);
	refresh(ps2);
      }
    }
  }else{ //full screen
    if (dxrefresh){
      if (DDRAW_display_pixelformat.dwRGBBitCount!=16){ //16->32 1024x768 dx
	static unsigned long pebx,pecx;
	pebx=(unsigned long)ps->o;
	pecx=(unsigned long)ps3->o;
	_asm{
	  mov ebx,pebx
	    mov ecx,pecx
	    mov esi,786432
	    p16to32:
	    mov ax,[ebx]
	    mov dx,ax
	    and edx,01111100000000000b
	    shl edx,8
	    mov dx,ax
	    and dx,011111100000b
	    shl dx,5
	    mov dl,al
	    and dl,011111b
	    shl dl,3
	    mov [ecx],edx
	    add ebx,2
	    add ecx,4
	    dec esi
	    jnz p16to32
	} //asm
	img(vs,0,0,ps3);
      }else{
	img(vs,0,0,ps); //16->16 1024x768 dx
      }
    }else{ //no dxrefresh
      refresh(ps); //16->? 1024x768
    }
  }
}//refresh()

//screen log
void scrlog(const char*d){
  static txt *t=txtnew();
  static unsigned long i;
  txtset(t,d);
  LOGadd(t);

  if (SCRLOG_FILEONLY==FALSE){
    memcpy(ps->o,&ps->o2[16384],1540096); 
    for (i=770048;i<786432;i++){
      ps->o2[i]=(2<<11)+(27<<5)+18;
    }
    txtfnt=systemfont;
    txtout(ps,0,752,t);
    if (intro_ultimavi){
      img(ps,741,0,intro_ultimavi2);
      img0(ps,741,0,intro_ultimavi);
    }
  }

  refresh();
}

/* luteijn:
 * there was some #define magic here to 'replace' loadimage everywhere else with this loadimage2 version 
 * simplified this.
 */
#ifdef loadimage
#undef loadimage
#endif
#define loadimage loadimage

surf *loadimage2(LPCSTR name){
  txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
  return loadimage(name);
}
surf *loadimage2(txt* name){
  txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
  return loadimage(name);
}
surf *loadimage2(LPCSTR name,long flags){
  txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
  return loadimage(name,flags);
}
surf *loadimage2(txt* name,long flags){
  txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
  return loadimage(name,flags);
}
#undef loadimage
#define loadimage loadimage2

