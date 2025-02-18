//
// Created by Cocoa-AP on 1/26/2025.
//

#ifndef DEFAULT_CMAKE_HOST_SETUP_H
#define DEFAULT_CMAKE_HOST_SETUP_H

#include "../mytxt.h" /* txt* etc */
#include "data_host.h"
#include "../data_both.h" /* schedule* etc. */
#include "../function_host.h" /* host functions */
#include "host_object_init.h"



class host_setup {

    void runHostSetup();

    //// Cocoa - originally from data_host.h

    /**
     * @brief Initializes the host data
     *
     * Initializes the host data, including the object list, the schedule, and the NPC information.
     */
    void initHostData();


    // Cocoa - originally from host.inc

    /**
     * @brief Loads the server ban list
     *
     * Loads the ban list from the file `ban.txt`.
     * If the file does not exist, it creates a new one.
     */
    void loadBanList();

    /**
     * \brief Initializes the Orb of the Moon destinations.
     *
     * This function sets the default and specific coordinates for the Orb of the Moon
     * destinations. It first initializes all coordinates to default values, then sets
     * the outer and inner circle destinations to predefined coordinates.
     */
    void loadOrbOfTheMoonDestinations();


    void loadCreatureEnums();

    void loadMessageOfTheDay();

    void loadStormCloakData();

    void loadBaseTileFlags();

    void loadObjectPassFlags();

    void loadSchedules();

    void load_npc_objects_old();

    void loadNpcObjects();

    void loadSignData();

    void loadWorldOverlayData();

    void spreadCreature(long x, long y, object *obj);

    void createWorldOverlayObjects();

};



#endif //DEFAULT_CMAKE_HOST_SETUP_H
