#include "data_host.h"
#include "data_both.h"
#include "function_host.h"
#include "function_both.h"
#include "use_code/use_code.h"  // host only

struct NewObjectOptions {
    int *info;
    int *more2;
};

/**
 * ConfigureNewObjectOptions - Configures a NewObjectOptions structure.
 *
 * @param info: Pointer to an integer representing additional information.
 * @param more2: Pointer to an integer representing more data.
 *
 * @return A NewObjectOptions structure with the provided info and more2 values.
 */
NewObjectOptions ConfigureNewObjectOptions(int *info, int *more2);

/**
 * Adds a new object to the specified coordinates.
 *
 * @param x The x-coordinate where the object will be added.
 * @param y The y-coordinate where the object will be added.
 * @param type The type of the new object.
 * @param options Optional parameters for the new object.
 */
void addNewObj(unsigned long x, unsigned long y, unsigned short type, NewObjectOptions *options);
/**
 * \brief Fixes issues related to house objects.
 *
 * This function removes specific objects from the house area to fix known issues.
 */
void houseFixes();
/**
 * \brief Applies autosetup and map fixes.
 *
 * This function toggles specific levers in various locations to apply
 * autosetup and map fixes.
 */
void autoSetupAndMapFixes();
/**
 * \brief Fixes issues related to the Lord British castle drawbridge.
 *
 * This function links the drawbridge to the crank and lock objects.
 */
void lordBritishCastleDrawbridge();
/**
 * \brief Fixes the Minoc Inn doors.
 *
 * This function unlocks the Minoc Inn doors to prevent NPCs from getting stuck.
 */
void minocInnDoorsFix();
/**
 * \brief Fixes various quest items.
 *
 * This function updates the quest items to ensure they are properly handled.
 */
void questItemFix();
/**
 * \brief Fixes the treasure room.
 *
 * This function releases the treasure room lock.
 */
void treasureRoomFix();
/**
 * \brief Fixes issues related to Sutek.
 *
 * This function releases the Sutek lock and adds the required drawbridge objects to the map.
 */
void sutexFix();
/**
 * \brief Fixes the drawbridge in Sutek's area.
 *
 * Fixes the drawbridge in Sutek's area by linking the crank to the drawbridge parts.
 * This function ensures that the crank object is properly linked to the drawbridge parts,
 * and uses the crank to remove the "drawbridge down" items before linking the other parts.
 */
void sutexDrawbridgeFix();
/**
 * \brief Fixes issues related to the codex.
 *
 * This function adds the codex object to the specified coordinates.
 */
void codexFix();
/**
 * \brief Fixes view redirector issues.
 *
 * This function adds new objects to the map to fix view redirector issues.
 */
void viewRedirectorFixes();
/**
 * \brief Fixes nugget issue.
 *
 * This function adds a nugget object to the map.
 */
void nuggetsFix();
/**
 * \brief Fixes issues related to the smith.
 *
 * This function removes a smith related object from the map.
 */
void smithFix();
/**
 * \brief Fixes push me pull you issues.
 *
 * I have no idea what the purpose of this is, it removed an objects from the od array.
 */
void pushMePullYouFix();
/**
 * \brief Fixes potion issues and avatar room issues.
 *
 * This function fixes potion issues and avatar room issues by updating the object data and flags.
 */
void potionFix();
/**
 * \brief Fixes doors, secret doors, and levers.
 *
 * This function iterates over the 2D grid of objects and processes each cell
 * to fix doors, secret doors, and levers.
 */
void doorsAndSecretLevers();
/**
 * \brief Processes fixed objects to ensure buildable parts are not sent.
 *
 * This function iterates over a 2D grid of objects and processes each cell to ensure that
 * buildable parts of fixed objects are not sent to the client.
 */
void fixedObjectSystem();
/**
 * @brief Processes and stores fixed objects in a 2D grid.
 *
 * This function iterates over a 2D grid of objects and processes each cell to store fixed objects.
 * It handles view redirectors and ensures that non-floating objects are recorded in the fixed arrays.
 */
void tobjfixedSystem();
/**
 * @brief Fixes visibility issues by ensuring view redirectors are properly handled.
 *
 * This function iterates over a 2D grid of objects and checks for view redirectors (objects of type 416).
 * If a view redirector is found, it checks the surrounding cells to determine if visibility needs to be enforced.
 * If visibility is required, the coordinates of the view redirector are written to a file.
 */
void visibilityFix();
/**
 * \brief Generates the client `bt.bin` file for reference.
 *
 * This function creates the `bt.bin` file, which is used by the client for reference.
 * It writes the `bt` data to the file and closes it. If the host is a network host,
 * it also adds a random treasure chest with objects to the map.
 */
void generateClientBtFile();
/**
 * \brief Randomly select a location for a treasure chest
 *
 * This function will randomly select a location for a treasure chest and populate
 * the chest with random items.
 */
 void treasureChestFix();

/**
 * \brief Runs all fixes to correct known issues in the host setup.
 *
 * This function runs all fixes to correct known issues in the host setup.
 */
void RunFixes();