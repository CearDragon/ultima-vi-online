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
#define U6O_HONESTY tnpc->i+=3;
#define U6O_COMPASSION tnpc->d+=3;
#define U6O_VALOR tnpc->s+=3;
#define U6O_JUSTICE tnpc->d+=1; tnpc->i+=1;
#define U6O_SACRIFICE tnpc->s+=1; tnpc->d+=1;
#define U6O_HONOR tnpc->s+=1; tnpc->i+=1;
#define U6O_SPIRITUALITY tnpc->s+=1; tnpc->d+=1; tnpc->i+=1;
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

// Guardian Guild communal storage house (2026-06).
//
// The guild's storage shelves used to be registered to the guild *building*
// house (basehousenumber+26) and to the shared scratch house (basehousenumber+0).
// Because those houses can be owned by a player, the per-player logout path
// (loop_host.cpp `housestorageadd`) saved the shelf contents into that player's
// .sav and *removed* them from the world ("decay"), restoring them only when
// that owner logged back in.
//
// For the guild we want owner-INDEPENDENT, persistent storage. We register the
// shelves to this dedicated house number instead. It is:
//   * ABSOLUTE (not basehousenumber + offset). `basehousenumber` is NOT a
//     constant — map patches reassign it as they load (bryan=48, steel=52,
//     spiritwood=99, ...). guardianguild.txt runs while it is 20, but by the
//     time guardianguild_save/load run (after house()) the last patch has left
//     it at some other value. Deriving the house number from basehousenumber at
//     save/load time therefore pointed at the WRONG (empty) house and nothing
//     persisted. A fixed absolute number is immune to that.
//   * UNIQUE. House 120 (what 20+100 produced) is ALSO used by spiritwood
//     (99+21), so it is shared — keying on it would sweep spiritwood's storage
//     into the guild file. 200 is unreachable by any patch's
//     basehousenumber+offset (max basehousenumber is 99; offsets are small) and
//     is well clear of every house number currently in use (top ~124).
//   * NON-OWNABLE. No purchase path sets housecost for it, so no player ever has
//     GNPCflags[28] == 200 -> the logout save/remove path never touches the
//     shelves (they "do not decay"); but the tiles are still genuine
//     housestorage slots, so the storage-shelf gameplay rules (stolen-item
//     block, 8-item stack limit, drop handling that scans i3 = 1..255) keep
//     working communally.
//
// Cross-restart persistence is handled separately by guardianobjs.sav via
// guardianguild_save()/guardianguild_load() in function_host.cpp (saved on host
// shutdown, loaded at host startup). Used identically (as an absolute house id)
// in both that code and assets/map_patches/guardianguild.txt.
#define GUARDIANGUILD_STORAGE_HOUSE 200
#ifdef CLIENT
#define FIRST_CLIENT 1
#else
#define FIRST_CLIENT 0
#endif
#endif /* DEFINE_HOST_H */