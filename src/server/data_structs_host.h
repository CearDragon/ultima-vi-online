#pragma once
#ifndef DATA_STRUCTS_HOST_H
#define DATA_STRUCTS_HOST_H
#include "data_both.h"
struct housesav_info
{
    unsigned short flags;
    //1 occupied (vacant if NULL)
    //*EXCEPTION*: array index 0: flags set to day of month)
    unsigned char username[32];
    unsigned char charactername[32];
    unsigned short gold;
};
//converse information
struct npcbin_i
{
    //npcbin information/instruction
    long l1;
    long l2;
    long l3;
    long l4;
};
struct objentry
{
    unsigned char status;
    unsigned char unk_h;
    unsigned char unk_d1;
    unsigned char unk_d2;
    unsigned short type;
    unsigned char qty;
    unsigned char tag;
};
struct mlobj
{
    //multi-object index array
    object* obj[1]; //use 1 to avoid compile warning
};
struct crtenum_struct
{
    char x;
    char y;
};
#endif // DATA_STRUCTS_HOST_H
