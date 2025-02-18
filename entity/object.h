//
// Created by Cocoa-AP on 1/26/2025.
//

#ifndef DEFAULT_CMAKE_OBJECT_H
#define DEFAULT_CMAKE_OBJECT_H

/////////////////////////////////////////////////////////////////////////////
////
////                      object info notes
////                    ---------------------
////
//// 1 walking/movement state (for NPC and monsters)
//// 2 NPC struct attached (->more)
//// 4 crt attached
//// 8 ->more is a pointer to a root object, refer to root
//// 16temporary object delay value.byte1
//// 32temporary object delay value.byte2
//// 64temporary object delay value.byte3
//// 128 1=stealing, 0=ok to take
//// 256 1=quest item, 0=non-quest item
//// 512	item info USED FOR BURNING DOWN TORCHES and now powder keg fun!
///             enchant uses multiples of 512 starting from 512*5 and going down with each enchant
//// 1024	item info
//// 2048	item info
//// 4096	item info
//// 8192	?
//// 16384	random treasure chest item id, moved it here so It wouldn't mess up with new item info changes easily.
//// 32768 update object


class object {

public:
    unsigned short spriteIndex; // value of sprite sub-index
    unsigned short type; //bits values above 1024 are the sprite sub-index
    unsigned short info; //quantity/position of lever/creature type...

    unsigned short x;
    unsigned short y; //position
    union {
        void *more; //pointer to more info (chest's objects)
        unsigned long more2;
    };
    void *prev; //pointer to previous object (NULL=first)
    void *next; //pointer to next object (NULL=last)
};


#endif //DEFAULT_CMAKE_OBJECT_H
