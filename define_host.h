#ifndef DEFINE_HOST_H
#define DEFINE_HOST_H
#define DEATHPOSX 2032
#define DEATHPOSY 1012
#define SAVESLOTLAST 6143
//save version 3.0 ENCRYPTED3.0 
//save version 4.0 UNENCRYPTED3.0 
//save ver 5 is same as 4, but with sha2 encrypted pw and 1 extra byte to indicate what encryption is used
#define SAVEVER 5 
#define ADMINSMAX 8
//shrine stat improvements
#define U6O_HONESTY tnpc->intelligence+=3;
#define U6O_COMPASSION tnpc->dexterity+=3;
#define U6O_VALOR tnpc->strength+=3;
#define U6O_JUSTICE tnpc->dexterity+=1; tnpc->intelligence+=1;
#define U6O_SACRIFICE tnpc->strength+=1; tnpc->dexterity+=1;
#define U6O_HONOR tnpc->strength+=1; tnpc->intelligence+=1;
#define U6O_SPIRITUALITY tnpc->strength+=1; tnpc->dexterity+=1; tnpc->intelligence+=1;
#define U6O_HUMILITY

/* spell return values */
#define SPELL_NOTDONE (0)
#define SPELL_SUCCESS (1)
#define SPELL_INVALID (2)
#define SPELL_FAILURE (3)

#define HIRELINGS_MAX (16) //should be at least 11 to get hirelings to all taverns after that they get random positions
#define MOVER_FOUND 65536
#define HOUSEMAX 256
#define HOUSESTORAGESLOTMAX 600
#ifdef CLIENT 
#define FIRST_CLIENT 1
#else 
#define FIRST_CLIENT 0
#endif
#endif /* DEFINE_HOST_H */
