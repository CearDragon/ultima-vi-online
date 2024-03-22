//#include "stdafx.h" //CRITICAL header information

#include "data_both.h"
#include "define_host.h"
#include "function_host.h"

void house(){
	static object *myobj=OBJnew(),*myobj2=OBJnew(),*myobj3=OBJnew(),*myobj4=OBJnew();

	static long i=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0,i9=0;
	static long x=0,x2=0,x3=0,x4=0,x5=0,x6=0,x7=0,x8=0,x9=0;
	static long y=0,y2=0,y3=0,y4=0,y5=0,y6=0,y7=0,y8=0,y9=0;
	static long z=0,z2=0,z3=0,z4=0,z5=0,z6=0,z7=0,z8=0,z9=0;
	static double d=0,d2=0,d3=0,d4=0,d5=0,d6=0,d7=0,d8=0,d9=0;
	static unsigned char b=0,b2=0,b3=0,b4=0,b5=0,b6=0,b7=0,b8=0,b9=0;
	static float f=0,f2=0,f3=0,f4=0,f5=0,f6=0,f7=0,f8=0,f9=0;


	//init. house arrays
	ZeroMemory(&housex1,sizeof(housex1)); ZeroMemory(&housey1,sizeof(housey1));
	ZeroMemory(&housex2,sizeof(housex2)); ZeroMemory(&housey2,sizeof(housey2));
	ZeroMemory(&housepnext,sizeof(housepnext));
	ZeroMemory(&housepx,sizeof(housepx)); ZeroMemory(&housepy,sizeof(housepy));
	ZeroMemory(&housecost,sizeof(housecost));
	ZeroMemory(&houseinitialcost,sizeof(houseinitialcost));
	ZeroMemory(&housestoragenext,sizeof(housestoragenext));
	ZeroMemory(&housestoragex,sizeof(housestoragex)); ZeroMemory(&housestoragey,sizeof(housestoragey));

/* TODO/FIXME!
 * the inefficient way of calling BTset (not in a loop etc) in the patches, 
 * causes a huge delay when optimizing house.cpp, and even when optimizing for size,
 * still makes more than 1 MB of obj file. I think this contributes most to the size
 * of the host/both versions. Speed is not needed here as it is a one time process.
 * Consider patching basetiles file directly, or at least read in the patches at runtime and
 * parse them, so we don't have an object file full of placing things on the stack,
 * calling a function, rinse, repeat.
 */

#include "map_patches/house1.txt"
#include "map_patches/house2.txt"
#include "map_patches/house3.txt"
#include "map_patches/castlerooms.txt"
#include "map_patches/castlenonhousetiles.txt"
#include "map_patches/castlehousetiles.txt"
#include "map_patches/castlehousepatch.txt"
#include "map_patches/guardianguild.txt"
#include "map_patches/ragnor.txt"
#include "map_patches/katish.txt"
#include "map_patches/mose.txt"
#include "map_patches/bryan.txt"
#include "map_patches/darrell.txt"
#include "map_patches/notir.txt"
//#include "map_patches/toth.txt" //a house floating in the void with respawning staff. A BIG NO NO
#include "map_patches/forestfix.txt"
#include "map_patches/farm.txt"
#include "map_patches/spiritwood.txt"
#include "map_patches/steel.txt"
#include "map_patches/misc.txt"
#include "map_patches/shop.txt"
#include "map_patches/safeSpot.txt"
//#include "map_patches/dungeontest.txt"

}//house
