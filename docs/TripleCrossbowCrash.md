# Triple Crossbow Crash Investigation

## Issue
The game server (`u6oh.exe`) is crashing with an Access Violation (`0xC0000005`) when players attack using the Triple Crossbow. The exception indicates an attempt to read or write invalid memory during the attack logic.

## Root Cause Location
The Triple Crossbow is defined as a macro:
* `C:\repos\ultima-vi-online\src\common\define_both.h` -> `#define OBJ_TRIPLE_CROSSBOW (50)`

The server handles the specific multi-projectile logic for this weapon in:
* `C:\repos\ultima-vi-online\src\server\loop_host.cpp` (around line 17711)
```cpp
if ((myobj3->type & 1023) == 50)
{
    // triple crossbow
    ...
```

## The Core Issue
Because a single attack with this weapon spawns three projectiles instead of one, the server executes a dedicated loop block. Given the `0xC0000005` fault, the server is crashing while iterating or pushing data for these three missiles.

Considering recent memory-management refactors documented in the project (`docs/plan-serverRefactor.md` mentions migrating from raw `malloc` arrays to safer typed helpers like `makeMultiObject`), the likely culprits in lines 17711–17839 are:
1. **Array Out-Of-Bounds:** Allocating an array for fewer elements and writing to out-of-bounds indices.
2. **Null Dereference:** Failing to properly initialize or check pointer allocation before assigning properties to the new projectile objects.

## Solution Strategy
To fix the server crash, inspect and repair the projectile loop block in `src/server/loop_host.cpp`. (Also check `src/server/loop_host.inc` if it is currently compiled).

**Key things to review:**
* Ensure iteration loops over exactly 3 instances (e.g., `for (int i = 0; i < 3; i++)`).
* Check for missed legacy `malloc()` calls that should be ported to the new typed helpers, or improperly bound memory.
* Add null pointer guards before manipulating the newly spawned bolt items.

### Example Correction
```cpp
if ((myobj3->type & 1023) == OBJ_TRIPLE_CROSSBOW)
{
    // Triple Crossbow multi-shot logic
    for (int boltIndex = 0; boltIndex < 3; boltIndex++) 
    {
        // Ensure allocation is successful before pointer dereference
        object* currentBolt = spawnProjectileLogic(...); 
        if (currentBolt != nullptr)
        {
            currentBolt->type = OBJ_BOLT;
            // Additional property assignments
        }
    }
}
```
