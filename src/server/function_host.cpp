#include "function_host.h"
#include "function_both.h" /* LOGadd etc */
#include <math.h> /* fabs etc. */

#pragma warning(disable: 4018 4244)

void function_host_init(void)
{
    /* this groups all the initialisation stuff that was floating around the .h file */

    //sockets_accept sets newsocket to new sockets id and waits till user sets
    //newsocket=INVALID_SOCKET before accepting new sockets
    //shared global variables
    newsocket = INVALID_SOCKET;
    newsocket_ip = 0;
    //temp local variables (transferred to globals after successful connection)
    tnewsocket = INVALID_SOCKET;
    tnewsocket_ip = 0;
    hsockets_accept = NULL;
    idsockets_accept = NULL;

    AUTOPICKUPfirst = 0;
    AUTOPICKUPnextfree = 0;
    housestorageadd = 0;
    housestoragerestore = 0; //flags used when saving/restoring house items in file
    //house creation tool 1.0 variables
    patchx = 0;
    patchy = 0; //base offset for adding objects/changing basetiles
    housenumber = 65535; //house currently being created (1-?, 0 RESERVED, 65535=non-specific)
    basehousenumber = 20;

    //OBJadd_allow=FALSE;
    OBJmove_allow = FALSE;
    OBJlist_last = 0;

    //entry values
    WPF_NEXTTO = 0;
    WPF_OBJECT = NULL;
    objsave_last = -1;
    objsave_node_last = -1;

    hrevive_infiniteloopexit = NULL;
    idrevive_infiniteloopexit = NULL;
    stealing_txt = NULL;
    stealing_MESSAGE = TRUE;
    OBJtmp = NULL;
    OBJtmp2 = NULL;
    OBJtmp3 = NULL;
    OBJaddtocontainer_containermore = NULL;
}


void AUTOPICKUPadd(object* partyMemberObj, object* itemObj)
{
    // adds an item to the autopickup list
    npc* partyMemberNpc = (npc*)partyMemberObj->more;
    AUTOPICKUPobject[AUTOPICKUPnextfree] = itemObj;
    AUTOPICKUPett[AUTOPICKUPnextfree] = ett;
    AUTOPICKUPplayer[AUTOPICKUPnextfree] = partyMemberNpc->player;
    AUTOPICKUPpartymember[AUTOPICKUPnextfree] = partyMemberObj;
    AUTOPICKUPflags[AUTOPICKUPnextfree] = 0;

    if (itemObj->info & 32768)
    {
        // remove mark if present and flag correctly
        itemObj->info ^= 32768;
        AUTOPICKUPflags[AUTOPICKUPnextfree] |= 1;
    }

    AUTOPICKUPnextfree++;
    if (AUTOPICKUPnextfree == AUTOPICKUPfirst)
    {
        AUTOPICKUPfirst++;
    }

    // Self-management: remove expired entries (older than 1024.0f)
    while (AUTOPICKUPfirst != AUTOPICKUPnextfree)
    {
        if (AUTOPICKUPett[AUTOPICKUPfirst] < (ett - 1024.0f))
        {
            AUTOPICKUPfirst++;
        }
        else
        {
            break;
        }
    }
}

object* AUTOPICKUPcheck(player* targetPlayer, object* itemObj)
{
    // Returns an object pointer to the party member to return the item to, or NULL if not found.

    // Self-management: remove expired entries
    while (AUTOPICKUPfirst != AUTOPICKUPnextfree)
    {
        if (AUTOPICKUPett[AUTOPICKUPfirst] < (ett - 1024.0f))
        {
            AUTOPICKUPfirst++;
        }
        else
        {
            break;
        }
    }

    for (short i = AUTOPICKUPfirst; i != AUTOPICKUPnextfree; i++)
    {
        if (AUTOPICKUPobject[i] == itemObj)
        {
            if (AUTOPICKUPplayer[i] == targetPlayer)
            {
                for (short partyIndex = 0; partyIndex <= 7; partyIndex++)
                {
                    if (AUTOPICKUPpartymember[i] == targetPlayer->party[partyIndex])
                    {
                        AUTOPICKUPobject[i] = NULL; // Invalidate entry
                        AUTOPICKUPett[i] = ett - 1024.0f - 1.0f; // Mark for cleanup
                        if (AUTOPICKUPflags[i] & 1)
                        {
                            itemObj->info |= 32768;
                        }
                        return AUTOPICKUPpartymember[i];
                    }
                }
                return NULL;
            }
        }
    }
    return NULL;
}


unsigned long parse_ip_string(txt* ip_text)
{
    // Parses an IP address string (e.g., "127.0.0.1") into an unsigned long.
    // This is a simplified parser based on the original logic.
    txt* temp_text = txtnew();
    txtset(temp_text, ip_text);

    unsigned long ip_value = (unsigned long)txtnum(temp_text);
    for (int i = 0; i < 3; i++)
    {
        // Find next dot
        while (temp_text->l > 0 && temp_text->d2[0] != '.')
        {
            txtright(temp_text, temp_text->l - 1);
        }
        if (temp_text->l > 0)
        {
            txtright(temp_text, temp_text->l - 1); // skip the dot
            ip_value += ((unsigned long)txtnum(temp_text) << (8 * (i + 1)));
        }
    }
    free(temp_text);
    return ip_value;
}

bool is_ip_banned(unsigned long ip_to_check)
{
    file* ban_file = open2("banip.txt", OF_READWRITE | OF_SHARE_COMPAT);
    if (ban_file->h == HFILE_ERROR)
    {
        return false;
    }

    bool is_banned = false;
    txt* line_text = txtnew();
    txt* original_line_text = txtnew();

    while (seek(ban_file) < lof(ban_file))
    {
        txtfilein(line_text, ban_file);
        if (line_text->l > 0)
        {
            txtset(original_line_text, line_text);
            unsigned long banned_ip = parse_ip_string(line_text);
            if (ip_to_check == banned_ip)
            {
                txt* log_msg = txtnew();
                txtset(log_msg, "BannedIP_Blocked:");
                txtadd(log_msg, original_line_text);
                LOGadd(log_msg);
                free(log_msg);
                is_banned = true;
                break;
            }
        }
    }

    free(line_text);
    free(original_line_text);
    close(ban_file);
    return is_banned;
}

DWORD WINAPI sockets_accept(LPVOID null_value)
{
    while (true)
    {
        sockaddr_in accept_addr;
        ZeroMemory(&accept_addr, sizeof(accept_addr));
        int addr_len = sizeof(accept_addr);

        SOCKET accepted_socket = accept(u6osocket, (sockaddr*)&accept_addr, &addr_len);
        if (accepted_socket == INVALID_SOCKET)
        {
            SleepEx(100, FALSE);
            continue;
        }

        unsigned long accepted_ip = 0;
        if (accept_addr.sin_family == AF_INET)
        {
            accepted_ip = accept_addr.sin_addr.S_un.S_addr;
        }

        if (is_ip_banned(accepted_ip))
        {
            shutdown(accepted_socket, SD_RECEIVE | SD_SEND);
            SleepEx(4096, NULL);
            closesocket(accepted_socket);
            continue;
        }

        // Configure socket options
        int flag = 1;
        setsockopt(accepted_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
        int buffer_size = 65536;
        setsockopt(accepted_socket, SOL_SOCKET, SO_RCVBUF, (char*)&buffer_size, sizeof(int));
        setsockopt(accepted_socket, SOL_SOCKET, SO_SNDBUF, (char*)&buffer_size, sizeof(int));
        unsigned long non_blocking = 1;
        ioctlsocket(accepted_socket, FIONBIO, &non_blocking);

        send(accepted_socket, (char*)&U6O_SIGNATURE, 4, 0);

        newsocket_ip = accepted_ip;
        newsocket = accepted_socket;

        // Wait until newsocket is processed (reset to INVALID_SOCKET)
        while (newsocket != INVALID_SOCKET)
        {
            SleepEx(15, FALSE);
        }
    }
    return 0;
}


/*
//volatile links (vlnk)
*/

void VLNKnew(void* links_container, void* linked_object, unsigned long offset)
{
    if (vlnkb_free_last == -1)
    {
        vlnkb_off[++vlnkb_last] = offset;
        vlnkb_lnk[vlnkb_last] = linked_object;
        vlnkb_lnks[vlnkb_last] = links_container;
    }
    else
    {
        long free_index = vlnkb_free[vlnkb_free_last];
        vlnkb_off[free_index] = offset;
        vlnkb_lnk[free_index] = linked_object;
        vlnkb_lnks[free_index] = links_container;
        vlnkb_free_last--;
    }
}

void VLNKremove(void* linked_object)
{
    // Removes any volatile link to the specified object.
    for (long i = 0; i <= vlnkb_last; i++)
    {
        if (vlnkb_lnk[i] == linked_object)
        {
            unsigned long* link_ptr = (unsigned long*)vlnkb_off[i];
            if (link_ptr)
            {
                *link_ptr = NULL;
            }
            vlnkb_lnk[i] = NULL;
            vlnkb_lnks[i] = NULL;
            vlnkb_off[i] = NULL;
            vlnkb_free[++vlnkb_free_last] = i;
        }
    }
}

void VLNKsremove(void* links_container)
{
    // Removes all volatile links associated with the specified container.
    for (long i = 0; i <= vlnkb_last; i++)
    {
        if (vlnkb_lnks[i] == links_container)
        {
            vlnkb_lnk[i] = NULL;
            vlnkb_lnks[i] = NULL;
            vlnkb_off[i] = NULL;
            vlnkb_free[++vlnkb_free_last] = i;
        }
    }
}

object* OBJnew()
{
    if (objb_free_last == -1)
    {
        ZeroMemory(&objb[++objb_last], sizeof(object));
        return &objb[objb_last];
    }
    else
    {
        long free_index = objb_free[objb_free_last--];
        ZeroMemory(&objb[free_index], sizeof(object));
        return &objb[free_index];
    }
}

void OBJrelease(object* obj)
{
    if (!obj) return;

    unsigned long object_index = ((unsigned long)obj - (unsigned long)&objb) / sizeof(object);
    objb_free[++objb_free_last] = object_index;

    // Clear object data
    ZeroMemory(obj, sizeof(object));
}

void free(player* player_ptr)
{
    if (!player_ptr) return;

    for (unsigned int i = 0; i <= 9; i++)
    {
        VLNKremove(player_ptr->ktar[i]);
        VLNKsremove(player_ptr->ktar[i]);
        OBJremove(player_ptr->ktar[i]);
        OBJrelease(player_ptr->ktar[i]);
    }

    VLNKremove(player_ptr->talk_target);
    VLNKsremove(player_ptr->talk_target);
    OBJremove(player_ptr->talk_target);
    OBJrelease(player_ptr->talk_target);

    free(player_ptr->password);
    free(player_ptr->name);
    free(player_ptr->newpassword);
    if (player_ptr->npcname)
    {
        free(player_ptr->npcname);
    }
    free((void*)player_ptr);
}

void free(creature* creature_ptr)
{
    if (!creature_ptr) return;

    object* items_obj = (object*)creature_ptr->items;
    if (items_obj)
    {
        unsigned long item_count = OBJlist(items_obj);
        OBJlist_last = NULL;
        for (unsigned long i = 0; i < item_count; i++)
        {
            OBJremove(OBJlist_list[i]);
            OBJrelease(OBJlist_list[i]);
        }
    }
    free((void*)creature_ptr);
}

void free(npc* npc_ptr)
{
    if (!npc_ptr) return;

    object* current_obj = NULL;
    unsigned long total_item_count = 0;

    for (unsigned int i = 0; i <= 8; i++)
    {
        switch (i)
        {
        case 0: current_obj = npc_ptr->helm;
            break;
        case 1: current_obj = npc_ptr->wep_right;
            break;
        case 2: current_obj = npc_ptr->wep_left;
            break;
        case 3: current_obj = npc_ptr->armour;
            break;
        case 4: current_obj = npc_ptr->boots;
            break;
        case 5: current_obj = npc_ptr->neck;
            break;
        case 6: current_obj = npc_ptr->ring_right;
            break;
        case 7: current_obj = npc_ptr->ring_left;
            break;
        case 8: current_obj = (object*)npc_ptr->items;
            break;
        }
        if (current_obj)
        {
            total_item_count = OBJlist(current_obj);
        }
    }

    OBJlist_last = NULL;
    for (unsigned long i = 0; i < total_item_count; i++)
    {
        object* listItem = OBJlist_list[i];
        unsigned short itemType = listItem->type & 1023;

        if (itemType == OBJ_HORSE_PAPERS)
        {
            // If item being released is horse papers, remove the horse also
            object* paperMore = (object*)listItem->more;
            if (paperMore)
            {
                creature* horseCrt = (creature*)paperMore->more;
                if (horseCrt)
                {
                    object* horseObj = (object*)horseCrt->more;
                    OBJremove(horseObj);
                    OBJrelease(horseObj);
                    free(horseCrt);
                }
                OBJremove(paperMore);
                OBJrelease(paperMore);
            }
        }
        else if (itemType == OBJ_SHIP_DEED)
        {
            // Remove the ship
            object* shipObj = (object*)listItem->more;
            if (shipObj)
            {
                if ((shipObj->type & 1023) == OBJ_SHIP)
                {
                    mlobj* shipParts = NULL;
                    if (shipObj->info & 2)
                    {
                        // If on board the ship
                        npc* pilotNpc = (npc*)shipObj->more;
                        if (pilotNpc && pilotNpc->player)
                        {
                            shipParts = (mlobj*)pilotNpc->player->craft_con;
                        }
                    }
                    else
                    {
                        shipParts = (mlobj*)shipObj->more;
                    }

                    if (shipParts)
                    {
                        for (int j = 0; j <= 4; j++)
                        {
                            OBJremove(shipParts->obj[j]);
                            OBJrelease(shipParts->obj[j]);
                        }
                    }
                }
                // Remove the ship main object / skiff only if no one is aboard and it's not already removed
                if (!(shipObj->info & 2) && shipObj->type != 0 && shipObj->x != 0 && shipObj->y != 0)
                {
                    OBJremove(shipObj);
                    OBJrelease(shipObj);
                }
            }
        }
        OBJremove(listItem);
        OBJrelease(listItem);
    }

    free(npc_ptr->name);
    free((void*)npc_ptr);
}

void OBJcheckflags(unsigned long x, unsigned long y)
{
    // bt[][] unsigned short
    // 512 current basetiles exist -> expandable to 1024 using 10 bits
    // 6 bit flags remain:
    // 1 = land passable
    // 2 = air passable
    // 4 = sea passable
    // 8 = bolt passable
    // 16 = NPC passable (unlocked doors/containing items that can be picked up)
    // 32 = view OK (can be seen past)

    unsigned short baseTileId = bt[y][x] & 1023;
    OBJcheckflags_flags = btflags[baseTileId]; // reset using basetile flags
    OBJcheckflags_flags += 32; // look ok

    if (baseTileId >= 140 && baseTileId < 188) OBJcheckflags_flags &= ~32; // look NOT ok wall
    if (baseTileId >= 240 && baseTileId < 252) OBJcheckflags_flags &= ~32; // look NOT ok cave
    if (baseTileId >= 192 && baseTileId < 208) OBJcheckflags_flags &= ~32; // look NOT ok window

    object* currentObj = od[y][x];
    while (currentObj != NULL)
    {
        OBJcheckflags_td = objpassflags[(currentObj->type >> 10) + sprlnk[currentObj->type & 1023]];

        if ((OBJcheckflags_td & 1) == 0)
        {
            OBJcheckflags_flags &= ~(1 | 2); // remove land and air passable bits
        }

        // override: if special flag set, make land and air passable, remove sea passable
        if (OBJcheckflags_td & 2)
        {
            OBJcheckflags_flags |= (1 | 2);
            OBJcheckflags_flags &= ~4;
        }

        if (OBJcheckflags_td & 4)
        {
            OBJcheckflags_flags &= ~8; // remove bolt passable flag

            unsigned short objType = currentObj->type & 1023;
            // if a bolt can't pass through it may affect our vision too
            if (objType >= 297 && objType <= 300)
            {
                // doors
                OBJcheckflags_flags &= ~32;
            }
            if (objType == 334)
            {
                // secret door
                if ((currentObj->type & 1024) == 0) OBJcheckflags_flags &= ~32;
            }
            if (objType == 213) OBJcheckflags_flags &= ~32; // mousehole
        }

        if (OBJcheckflags_td & 8)
        {
            // NPC passable
            OBJcheckflags_flags |= 16;
        }

        if (currentObj->info & (4 | 2))
        {
            // creature or npc
            OBJcheckflags_flags &= ~4; // not sea passable
            OBJcheckflags_flags &= ~2; // not air passable
        }

        currentObj = (object*)currentObj->next;
    }

    bt[y][x] = baseTileId + OBJcheckflags_flags * 1024;
}

unsigned char OBJadd(unsigned long x, unsigned long y, object* obj)
{
    if (od[y][x] == NULL)
    {
        od[y][x] = obj;
        obj->prev = NULL;
        obj->next = NULL;
        obj->x = x;
        obj->y = y;
    }
    else
    {
        object* currentObj = od[y][x];
        bool inserted = false;

        while (currentObj != NULL)
        {
            if (objfloatflags[(currentObj->type >> 10) + sprlnk[currentObj->type & 1023]])
            {
                object* prevObj = (object*)currentObj->prev;
                if (prevObj != NULL)
                {
                    prevObj->next = obj;
                    obj->prev = prevObj;
                }
                else
                {
                    od[y][x] = obj;
                    obj->prev = NULL;
                }
                obj->next = currentObj;
                currentObj->prev = obj;
                obj->x = x;
                obj->y = y;
                inserted = true;
                break;
            }

            if (currentObj->next == NULL)
            {
                currentObj->next = obj;
                obj->prev = currentObj;
                obj->next = NULL;
                obj->x = x;
                obj->y = y;
                inserted = true;
                break;
            }
            currentObj = (object*)currentObj->next;
        }
    }

    OBJcheckflags(x, y);
    return 0; // success
}


void OBJaddtocontainer(object* container, object* object_to_add)
{
    if (obji[sprlnk[object_to_add->type & 1023]].weight == NULL)
    {
        if (U6O_DEBUG)
        {
            txt* errortext = txtnew();
            MessageBox(NULL, "ERROR CORRECTION: NULL WEIGHT ITEM TO PUT INTO CONTAINER PLACED ABOVE CONTAINER!",
                       "Ultima 6 Online", MB_OK);
            txtnumint(errortext, container->x);
            txtadd(errortext, "<-X CONTAINER LOCATION");
            MessageBox(NULL, errortext->d, "Ultima 6 Online", MB_OK);
            txtnumint(errortext, container->y);
            txtadd(errortext, "<-Y CONTAINER LOCATION");
            MessageBox(NULL, errortext->d, "Ultima 6 Online", MB_OK);
            free(errortext);
        }
        OBJadd(container->x, container->y, object_to_add);
        return;
    }

    object* existing_items = (object*)container->more;
    container->more = object_to_add;
    object_to_add->next = existing_items;
    object_to_add->prev = container;
    if (existing_items)
    {
        existing_items->prev = object_to_add;
    }
}

void BTset(long x, long y, unsigned short tile_id)
{
    x += patchx;
    y += patchy;
    bt[y][x] = tile_id;

    if (NEThost)
    {
        OBJcheckflags(x, y);
        if (housenumber != 65535)
        {
            if (housex1[housenumber] == 0 || housex1[housenumber] > x) housex1[housenumber] = x;
            if (housex2[housenumber] == 0 || housex2[housenumber] < x) housex2[housenumber] = x;
            if (housey1[housenumber] == 0 || housey1[housenumber] > y) housey1[housenumber] = y;
            if (housey2[housenumber] == 0 || housey2[housenumber] < y) housey2[housenumber] = y;

            bool point_exists = false;
            for (unsigned short p = 0; p < housepnext[housenumber]; p++)
            {
                if (x == housepx[housenumber][p] && y == housepy[housenumber][p])
                {
                    point_exists = true;
                    break;
                }
            }

            if (!point_exists)
            {
                unsigned short next_p = housepnext[housenumber];
                housepx[housenumber][next_p] = x;
                housepy[housenumber][next_p] = y;
                housepnext[housenumber]++;
            }
        }
    }
}

/* luteijn: away with thee! 
void encrypt(txt *t4){
static txt *t2=txtnew();
static long x,x2,x3,x4,i3;
//ENCRYPT3.0
x4=rnd*65536; txtset(t2,"????"); t2->df[0]=(float)x4; //code offset
x3=0; //prev UNENCRYPTED value
for(i3=2;i3<t4->l;i3++){ //skip version (first 2 bytes)
x2=t4->d2[i3]; //=original value
x2=x2^0xFF; //bitwise NOT
x2+=x3; //add prev UNENCRYPTED value
if (x2>=256) x2-=256; //"loop" byte value
x2+=encryptcode[x4]; x4=(x4+1)&65535;
if (x2>=256) x2-=256; //"loop" byte value
x3=t4->d2[i3]; //set prev UNENCRYPTED value
t4->d2[i3]=x2; //set ENCRYPTED value
}
txtadd(t4,t2); //add encrypt code starting offset (float)
}
*/

void decrypt(txt* encrypted_text)
{
    // DECRYPT3.0
    txt* temp_float_txt = txtnew();
    // The last 4 bytes of the encrypted text contain the float starting offset for the encryption code
    temp_float_txt->d2[0] = encrypted_text->d2[encrypted_text->l - 4];
    temp_float_txt->d2[1] = encrypted_text->d2[encrypted_text->l - 3];
    temp_float_txt->d2[2] = encrypted_text->d2[encrypted_text->l - 2];
    temp_float_txt->d2[3] = encrypted_text->d2[encrypted_text->l - 1];

    float code_offset_f = temp_float_txt->df[0];
    long code_index = (long)code_offset_f;
    long prev_unencrypted_value = 0;

    // skip version (first 2 bytes) and the last 4 bytes (encryption offset)
    for (long i = 2; i < (encrypted_text->l - 4); i++)
    {
        long encrypted_value = encrypted_text->d2[i];

        encrypted_value -= encryptcode[code_index];
        code_index = (code_index + 1) & 65535;
        if (encrypted_value < 0) encrypted_value += 256;

        encrypted_value -= prev_unencrypted_value;
        if (encrypted_value < 0) encrypted_value += 256;

        long unencrypted_value = encrypted_value ^ 0xFF; // bitwise NOT
        prev_unencrypted_value = unencrypted_value;
        encrypted_text->d2[i] = (unsigned char)unencrypted_value;
    }
    free(temp_float_txt);
}

static void AddMonsterDropItem(object* container, unsigned short itemType, int chance, int monsterType,
                               bool isChestItem)
{
    if ((int)(rnd * (float)chance) != 0) return;

    int extraCount = 0;
    if (itemType == 36 || itemType == 37 || itemType == 38)
    {
        // spear, axe, dagger
        extraCount = (int)(rnd * 5.0f) + 1;
    }

    for (int i = 0; i <= extraCount; i++)
    {
        object* invObj = OBJnew();
        invObj->type = itemType;

        if (itemType == 88)
        {
            // gold
            int roll = (int)(rnd * 3.0f);
            if (!isChestItem)
            {
                invObj->more2 = (unsigned long)(rnd * 26.0f) + 10;
                if (roll == 2) invObj->more2 = (unsigned long)(rnd * 26.0f) + 35;
            }
            else
            {
                invObj->more2 = (unsigned long)(rnd * 46.0f) + 10;
                if (roll == 2) invObj->more2 = (unsigned long)(rnd * 46.0f) + 55;
            }
        }
        else if (itemType == 58)
        {
            // spell (random potion)
            invObj->type = 275 + (int)(rnd * 8.0f) * 1024;
        }
        else if (itemType == 90)
        {
            // torch
            invObj->more2 = (unsigned long)(rnd * 3.0f) + 1;
        }
        else if (itemType == 77)
        {
            // gem
            invObj->more2 = 1;
        }
        else if (itemType == OBJ_TMAP)
        {
            unsigned long location = randomchestlocation(true);
            unsigned long ty = location >> 10;
            unsigned long tx = location & 1023;
            int rx1, rx2;
            do
            {
                rx1 = (int)(rnd * 7.0f);
                rx2 = (int)(rnd * 7.0f);
            }
            while (((bt[ty + (rx2 - 3) * 2][tx + (rx1 - 3) * 2] & 1023) >= 8) &&
                ((bt[ty + (rx2 - 3) * 2][tx + (rx1 - 3) * 2] & 1023) < 16));
            int rx3 = (int)(rnd * 7.0f);
            int rx5 = (int)(rnd * 7.0f);
            invObj->more2 = tx + 1024 * ty + 1024 * 1024 * rx1 + 1024 * 1024 * 8 * rx2 +
                1024 * 1024 * 8 * 8 * rx3 + 1024 * 1024 * 8 * 8 * 8 * rx5;
        }
        else if (itemType == 129)
        {
            // meat
            if (isChestItem || monsterType == 362)
            {
                // Chest or Winged Gargoyle
                int roll = (int)(rnd * 3.0f);
                if (roll != 2)
                {
                    if (roll == 1) invObj->type = 128; // bread
                    invObj->more2 = (unsigned long)(rnd * 5.0f) + 1;
                }
                else
                {
                    invObj->type = 133; // ham
                    invObj->more2 = 0;
                }
            }
            else
            {
                invObj->more2 = (unsigned long)(rnd * 3.0f) + 1;
            }
        }

        invObj->info |= 112; // identified flags
        OBJaddtocontainer(container, invObj);
        ENHANCEnewn(invObj, 8, 4);

        if (itemType == 42)
        {
            // crossbow
            object* bolts = OBJnew();
            bolts->type = 56; // bolts
            bolts->more2 = (unsigned long)(rnd * 25.0f) + 1;
            bolts->info |= 112;
            OBJaddtocontainer(container, bolts);
        }
        else if (itemType == 41)
        {
            // bow
            object* arrows = OBJnew();
            arrows->type = 55; // arrows
            arrows->more2 = (unsigned long)(rnd * 25.0f) + 1;
            arrows->info |= 112;
            OBJaddtocontainer(container, arrows);
        }
        else if (itemType == 98)
        {
            // chest
            invObj->type = 98 + 1024;
            // Recursively add items to the chest
            AddMonsterDropItem(invObj, 88, 2, monsterType, true); // gold
            AddMonsterDropItem(invObj, 90, 2, monsterType, true); // torch
            AddMonsterDropItem(invObj, 129, 2, monsterType, true); // meat
            AddMonsterDropItem(invObj, 77, 16, monsterType, true); // gem
        }
    }
}

void addu6monsterdropitems(object* monsterObj)
{
    if (!monsterObj || !monsterObj->more) return;

    creature* monster = (creature*)monsterObj->more;
    unsigned short monsterType = monsterObj->type & 1023;
    object* inventory = (object*)monster->items;
    if (!inventory) return;

    unsigned short weapons[32], armors[32], items[32];
    int weaponCount = 0, armorCount = 0, itemCount = 0;

    // Fill the potential drop lists based on monster type
    switch (monsterType)
    {
    case 384: // BEGGAR/PEASANT
        weapons[weaponCount++] = 38;
        weapons[weaponCount++] = 34;
        armors[armorCount++] = 185;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 428: // COW
        items[itemCount++] = 129;
        break;
    case 424: // CYCLOPS
        weapons[weaponCount++] = 44;
        weapons[weaponCount++] = 33;
        weapons[weaponCount++] = 34;
        armors[armorCount++] = 20;
        armors[armorCount++] = 3;
        armors[armorCount++] = 13;
        items[itemCount++] = 98;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 350: // DEER
        items[itemCount++] = 129;
        break;
    case 411: // DRAGON
        items[itemCount++] = 98;
        items[itemCount++] = 88;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 369: // DRAKE
        items[itemCount++] = 88;
        items[itemCount++] = 88;
        break;
    case 387: // WOMAN/DRESS-WEARER
        weapons[weaponCount++] = 38;
        armors[armorCount++] = 185;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 385: // FARMER
        weapons[weaponCount++] = 105;
        weapons[weaponCount++] = 101;
        armors[armorCount++] = 17;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 376: // FIGHTER
        weapons[weaponCount++] = 43;
        weapons[weaponCount++] = 42;
        armors[armorCount++] = 21;
        armors[armorCount++] = 10;
        armors[armorCount++] = 3;
        armors[armorCount++] = 22;
        items[itemCount++] = 88;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 362: // WINGED GARGOYLE
        weapons[weaponCount++] = 46;
        weapons[weaponCount++] = 41;
        weapons[weaponCount++] = 47;
        weapons[weaponCount++] = 49;
        armors[armorCount++] = 18;
        armors[armorCount++] = 1;
        items[itemCount++] = 58;
        items[itemCount++] = 129;
        items[itemCount++] = 57;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 363: // GARGOYLE (WINGLESS)
        weapons[weaponCount++] = 34;
        weapons[weaponCount++] = 49;
        weapons[weaponCount++] = 37;
        armors[armorCount++] = 1;
        armors[armorCount++] = 9;
        armors[armorCount++] = 18;
        armors[armorCount++] = 20;
        break;
    case 382: // GUARD
        weapons[weaponCount++] = 42;
        weapons[weaponCount++] = 47;
        armors[armorCount++] = 22;
        armors[armorCount++] = 10;
        armors[armorCount++] = 3;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 370: // HEADLESS
        weapons[weaponCount++] = 34;
        weapons[weaponCount++] = 36;
        armors[armorCount++] = 17;
        armors[armorCount++] = 9;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 383: // JESTER
        weapons[weaponCount++] = 38;
        weapons[weaponCount++] = 33;
        armors[armorCount++] = 17;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 386: // LUTE-PLAYER/MUSICIAN
        weapons[weaponCount++] = 41;
        weapons[weaponCount++] = 33;
        weapons[weaponCount++] = 43;
        armors[armorCount++] = 19;
        armors[armorCount++] = 1;
        armors[armorCount++] = 9;
        items[itemCount++] = 88;
        items[itemCount++] = 158;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 378: // MAGE
        weapons[weaponCount++] = 38;
        armors[armorCount++] = 17;
        armors[armorCount++] = 1;
        items[itemCount++] = 58;
        items[itemCount++] = 98;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 380: // MERCHANT
        weapons[weaponCount++] = 38;
        weapons[weaponCount++] = 43;
        armors[armorCount++] = 17;
        armors[armorCount++] = 1;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 372: // MONGBAT
        weapons[weaponCount++] = 38;
        weapons[weaponCount++] = 43;
        weapons[weaponCount++] = 36;
        weapons[weaponCount++] = 37;
        armors[armorCount++] = 9;
        armors[armorCount++] = 17;
        armors[armorCount++] = 18;
        items[itemCount++] = 98;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 347: // REAPER
        items[itemCount++] = 58;
        items[itemCount++] = 88;
        break;
    case 348: // SHEEP
        items[itemCount++] = 129;
        break;
    case 368: // SKELETON
        weapons[weaponCount++] = 43;
        weapons[weaponCount++] = 36;
        weapons[weaponCount++] = 37;
        weapons[weaponCount++] = 41;
        weapons[weaponCount++] = 38;
        armors[armorCount++] = 9;
        armors[armorCount++] = 1;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 361: // SPIDER
        items[itemCount++] = 129;
        break;
    case 377: // SWASHBUCKLER
        weapons[weaponCount++] = 43;
        weapons[weaponCount++] = 41;
        weapons[weaponCount++] = 33;
        armors[armorCount++] = 19;
        armors[armorCount++] = 1;
        armors[armorCount++] = 9;
        items[itemCount++] = 88;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 371: // TROLL
        weapons[weaponCount++] = 37;
        weapons[weaponCount++] = 36;
        weapons[weaponCount++] = 43;
        weapons[weaponCount++] = 41;
        weapons[weaponCount++] = 34;
        armors[armorCount++] = 9;
        armors[armorCount++] = 18;
        armors[armorCount++] = 1;
        armors[armorCount++] = 19;
        items[itemCount++] = 98;
        items[itemCount++] = 88;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    case 379: // VILLAGER
        weapons[weaponCount++] = 43;
        weapons[weaponCount++] = 33;
        armors[armorCount++] = 17;
        items[itemCount++] = 88;
        items[itemCount++] = OBJ_TMAP;
        break;
    default:
        break;
    }

    // Process lists and add items with appropriate chances
    for (int i = 0; i < weaponCount; i++)
    {
        int chance = (i == 0) ? 4 : (i < 3 ? 8 : 16);
        AddMonsterDropItem(inventory, weapons[i], chance, monsterType, false);
    }
    for (int i = 0; i < armorCount; i++)
    {
        int chance = (i == 0) ? 4 : (i < 3 ? 8 : 16);
        AddMonsterDropItem(inventory, armors[i], chance, monsterType, false);
    }
    for (int i = 0; i < itemCount; i++)
    {
        unsigned short type = items[i];
        int chance = (i == 0) ? 4 : (i < 3 ? 8 : 16);
        if (type == 58) chance = 16; // Potions are rarer
        if (type == OBJ_TMAP) chance = 128; // Maps are very rare
        AddMonsterDropItem(inventory, type, chance, monsterType, false);
    }
}


/*
MOVERNEW: FLAGS
flags can be set globally, in which case they need to be reset when finished
or locally by being passed as flags
local flags and OR-ed with global flags
*/
//unsigned long MOVERNEW_GLOBALFLAGS=0;
//unsigned char MOVERNEW_ERROR;//valid when movernew(...) returns 0
//1=FAILED (object is not a mover)
//2=IGNORED (object is not the primary part of a mover)
static unsigned char SetMoverAllegiance(object* obj, unsigned short x, unsigned short y, unsigned long flags)
{
    unsigned short typeID = obj->type & 1023;
    unsigned char allegiance = 1; // default aggressive

    if (typeID == 382) // guard
    {
        if (x < 1024) allegiance = 4; // not underground
    }
    else if (typeID == 373 || typeID == 430 || typeID == 428 || typeID == 356 || typeID == 348 || typeID == 350)
    {
        // wisp, horse, cow, bird, sheep, deer
        allegiance = 0; // passive
    }
    else if (typeID == 388) // cat
    {
        if (x >= 400 && y >= 576 && x <= 415 && y <= 591) allegiance = 0; // Dr Cat's cats
    }
    else if (typeID == 352) // ghost
    {
        if (x == 139 && y == 196) allegiance = 0; // abby ghosts
    }

    if (flags & MOVERNEW_FLAG_PASSIVE) allegiance = 0;
    if (flags & MOVERNEW_FLAG_FRIENDLY) allegiance = 4;
    if (flags & MOVERNEW_FLAG_AGRESSIVE) allegiance = 1;

    return allegiance;
}

static void InitializeAggressiveMoverInventory(object* obj, object* invbag)
{
    addu6monsterdropitems(obj);

    // XP based gold
    if ((long)(rnd * 8.0f) == 0)
    {
        long goldAmount = obji[sprlnk[obj->type & 1023]].v5;
        goldAmount = (long)(rnd * (float)(goldAmount + 1));
        if (goldAmount > 0)
        {
            object* existingGold = (object*)invbag->more;
            while (existingGold)
            {
                if (existingGold->type == 88)
                {
                    existingGold->more2 += goldAmount;
                    return;
                }
                existingGold = (object*)existingGold->next;
            }
            object* invobj = OBJnew();
            invobj->type = 88;
            invobj->more2 = goldAmount;
            invobj->info |= 112;
            OBJaddtocontainer(invbag, invobj);
        }
    }
}

static void Initialize2PartCreature(object* obj, creature* crt, unsigned short x, unsigned short y)
{
    object* backPart = OBJnew();
    backPart->more = obj;
    backPart->type = obj->type + 8 * 1024;
    backPart->info |= 8;
    OBJadd(x, y + 1, backPart);
    crt->more = backPart;

    // Sutek's creatures' hp increase
    bool onSutekIsland = (x >= 747 && x <= 829 && y >= 905 && y <= 983);
    bool inCage = (x >= 792 && x <= 796 && y >= 937 && y <= 938);
    if (onSutekIsland && !inCage) crt->hp *= 4;
}

static void Initialize4PartCreature(object* obj, creature* crt, unsigned short x, unsigned short y)
{
    mlobj* mmyobj = (mlobj*)malloc(sizeof(object*) * 3);
    crt->more = mmyobj;
    obj->type = (obj->type & 1023) + 3 * 1024;

    for (int i = 0; i < 3; i++)
    {
        object* part = OBJnew();
        part->more = obj;
        mmyobj->obj[i] = part;
        part->info |= 8;
        int offsetX = 0, offsetY = 0;
        if (i == 0)
        {
            offsetX = -1;
            offsetY = -1;
            part->type = obj->type - 3 * 1024;
        }
        else if (i == 1)
        {
            offsetY = -1;
            part->type = obj->type - 2 * 1024;
        }
        else if (i == 2)
        {
            offsetX = -1;
            part->type = obj->type - 1 * 1024;
        }
        OBJadd(x + offsetX, y + offsetY, part);
    }
}

static void InitializeHydra(object* obj, creature* crt, unsigned short x, unsigned short y)
{
    mlobj* mmyobj = (mlobj*)malloc(sizeof(object*) * 8);
    crt->more = mmyobj;
    int types[8] = {28, 0, 4, 8, 12, 16, 20, 24};
    int offX[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int offY[8] = {-1, -1, -1, 0, 1, 1, 1, 0};

    for (int i = 0; i < 8; i++)
    {
        object* part = OBJnew();
        part->more = obj;
        mmyobj->obj[i] = part;
        part->type = 425 + types[i] * 1024;
        part->info |= 8;
        OBJadd(x + offX[i], y + offY[i], part);
    }
}

static void InitializeSerpent(object* obj, creature* crt, unsigned short x, unsigned short y)
{
    mlobj* mmyobj = (mlobj*)malloc(sizeof(object*) * 7);
    crt->more = mmyobj;
    obj->type = (obj->type & 1023) + 4 * 1024;
    int types[7] = {8, 7, 6, 9, 8, 7, -3};
    int offX[7] = {0, -1, -1, 0, 0, -1, -1};
    int offY[7] = {-1, -1, 0, 0, -1, -1, 0};

    for (int i = 0; i < 7; i++)
    {
        object* part = OBJnew();
        part->more = obj;
        mmyobj->obj[i] = part;
        part->type = obj->type + types[i] * 1024;
        part->info |= 8;
        OBJadd(x + offX[i], y + offY[i], part);
    }
}

static void InitializeDragon(object* obj, creature* crt, unsigned short x, unsigned short y)
{
    mlobj* mmyobj = (mlobj*)malloc(sizeof(object*) * 4);
    crt->more = mmyobj;
    obj->type = 411;
    int types[4] = {8, 16, 24, 32};
    int offX[4] = {0, 0, -1, 1};
    int offY[4] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; i++)
    {
        object* part = OBJnew();
        part->more = obj;
        mmyobj->obj[i] = part;
        part->type = obj->type + types[i] * 1024;
        part->info |= 8;
        OBJadd(x + offX[i], y + offY[i], part);
    }
}

static void InitializeTanglevine(object* obj, creature* crt)
{
    mlobj* mmyobj = (mlobj*)malloc(sizeof(object*) * 4);
    crt->more = mmyobj;
    obj->type = 365;

    for (int i = 0; i < 4; i++)
    {
        object* tendril = OBJnew();
        mmyobj->obj[i] = tendril;

        int offsetX = 0, offsetY = 0;
        if (i == 0)
        {
            offsetY = -1;
            tendril->type = 366 + 1024;
        }
        else if (i == 1)
        {
            offsetY = 1;
            tendril->type = 366 + 1024;
        }
        else if (i == 2)
        {
            offsetX = -1;
            tendril->type = 366;
        }
        else if (i == 3)
        {
            offsetX = 1;
            tendril->type = 366;
        }

        OBJadd(obj->x + offsetX, obj->y + offsetY, tendril);

        creature* tendrilCrt = (creature*)malloc(sizeof(creature));
        ZeroMemory(tendrilCrt, sizeof(creature));
        tendril->more = tendrilCrt;
        tendril->info |= 4;

        tendrilCrt->crt_struct = TRUE;
        tendrilCrt->al = crt->al;
        tendrilCrt->hp = 32767;
        tendrilCrt->mp = 0;
        tendrilCrt->more = obj;
        tendrilCrt->respawn_hp = tendrilCrt->hp;
        tendrilCrt->respawn_x = tendril->x;
        tendrilCrt->respawn_y = tendril->y;

        object* invbag = OBJnew();
        invbag->type = 188;
        tendrilCrt->items = invbag;
    }
}

unsigned char movernew(unsigned short type, unsigned short x, unsigned short y, unsigned long flags)
{
    flags |= MOVERNEW_GLOBALFLAGS;

    MOVERNEW_ERROR = 0;
    unsigned short baseType = type & 1023;

    if (!tclass_mover[type])
    {
        MOVERNEW_ERROR = 1;
        if (obji[sprlnk[baseType]].v4)
        {
            MOVERNEW_ERROR = 2;
        }
    }
    if (baseType == 423 || baseType == 412 || baseType == 414 || baseType == 415 || baseType == 366)
    {
        MOVERNEW_ERROR = 2;
    }

    if (MOVERNEW_ERROR) return 0;

    MOVERNEW_OBJECT = OBJnew();
    MOVERNEW_OBJECT->type = type;
    OBJadd(x, y, MOVERNEW_OBJECT);

    MOVERNEW_OBJECT->more = malloc(sizeof(creature));
    ZeroMemory(MOVERNEW_OBJECT->more, sizeof(creature));

    creature* crt = (creature*)MOVERNEW_OBJECT->more;
    crt->crt_struct = TRUE;
    MOVERNEW_OBJECT->info |= 4; //<-crt

    crt->hp = (obji[sprlnk[baseType]].v8 >> 8) * 4;
    crt->mp = rnd * 9;

    crt->al = SetMoverAllegiance(MOVERNEW_OBJECT, x, y, flags);

    object* invbag = OBJnew();
    invbag->type = 188;
    crt->items = invbag; // inventory bag (not shown)

    if (crt->al == 1)
    {
        InitializeAggressiveMoverInventory(MOVERNEW_OBJECT, invbag);
    }

    if (baseType == 428 || baseType == 348 || baseType == 350)
    {
        // cow/sheep/deer
        if ((int)(rnd * 2.0f) != 0)
        {
            object* invobj = OBJnew();
            invobj->type = 129; // meat
            invobj->more2 = 1;
            invobj->info |= 112;
            OBJaddtocontainer(invbag, invobj);
        }
    }

    int v4 = obji[sprlnk[baseType]].v4;
    if (v4 == 8) Initialize2PartCreature(MOVERNEW_OBJECT, crt, x, y);
    else if (v4 == 5) Initialize4PartCreature(MOVERNEW_OBJECT, crt, x, y);
    else if (v4 == 11) InitializeHydra(MOVERNEW_OBJECT, crt, x, y);
    else if (v4 == 10) InitializeSerpent(MOVERNEW_OBJECT, crt, x, y);
    else if (v4 == 9) InitializeDragon(MOVERNEW_OBJECT, crt, x, y);
    else if (v4 == 13) InitializeTanglevine(MOVERNEW_OBJECT, crt);

    if (baseType == 375)
    {
        // slime
        MOVERNEW_OBJECT->info |= (3 << 9); // slime divide value
    }

    // respawn info
    crt->respawn_hp = crt->hp;
    crt->respawn_x = MOVERNEW_OBJECT->x;
    crt->respawn_y = MOVERNEW_OBJECT->y;

    return 1;
}

object* OBJaddnew(unsigned long x, unsigned long y, unsigned short type, unsigned short info, unsigned long more2)
{
    // call movernew()
    if (movernew(type, x, y, NULL)) return MOVERNEW_OBJECT; // mover was created successfully!
    if (MOVERNEW_ERROR == 2) return NULL; // IGNORED (object is not the primary part of a mover)

    // check for multiple quantity items with NULL quantity and set to 1
    if (obji[sprlnk[type & 1023] + (type >> 10)].flags & 4096)
    {
        if (more2 == 0) more2 = 1;
    }

    object* obj = OBJnew();
    obj->type = type;
    obj->info = info;
    obj->more2 = more2;

    unsigned long realX = x + patchx;
    unsigned long realY = y + patchy;

    OBJadd(realX, realY, obj);

    if (housenumber != 65535)
    {
        if ((housex1[housenumber] == 0) || (housex1[housenumber] > realX)) housex1[housenumber] = (unsigned short)realX;
        if ((housex2[housenumber] == 0) || (housex2[housenumber] < realX)) housex2[housenumber] = (unsigned short)realX;
        if ((housey1[housenumber] == 0) || (housey1[housenumber] > realY)) housey1[housenumber] = (unsigned short)realY;
        if ((housey2[housenumber] == 0) || (housey2[housenumber] < realY)) housey2[housenumber] = (unsigned short)realY;

        for (unsigned short p = 0; p < housepnext[housenumber]; p++)
        {
            if ((realX == housepx[housenumber][p]) && (realY == housepy[housenumber][p])) return obj;
        }

        if (housepnext[housenumber] < 512)
        {
            unsigned short p = housepnext[housenumber];
            housepx[housenumber][p] = (unsigned short)realX;
            housepy[housenumber][p] = (unsigned short)realY;
            housepnext[housenumber]++;
        }
    }

    return obj;
}


void OBJremove(object* obj)
{
    //OBJremove detects container objects
    OBJtmp = (object*)obj->prev;
    OBJtmp2 = (object*)obj->next;
    if (OBJtmp == NULL)
    {
        //if no object preceeds obj
        if (OBJtmp2 == NULL)
        {
            //if no object is after obj
            od[obj->y][obj->x] = NULL;
            //type 13743 (horse rear)
            //info 49
            //x 6400
            //y 47361
            //more 1
            //next 0
            //prev 0
            //called from objmove2 in winmain
            //is this a result of not removing rear end of horse previously??!!??!!??
        }
        else
        {
            //an object is after obj
            od[obj->y][obj->x] = OBJtmp2;
            OBJtmp2->prev = NULL;
        }
    }
    else
    {
        //an object is before obj
        if (OBJtmp2 == NULL)
        {
            //if no object is after obj
            //if the previous object is NOT a container carrying obj
            if (OBJtmp->next == obj) { OBJtmp->next = NULL; }
            else { OBJtmp->more = NULL; }
        }
        else
        {
            //an object is after obj
            //if the previous object is NOT a container carrying obj
            if (OBJtmp->next == obj) { OBJtmp->next = OBJtmp2; }
            else { OBJtmp->more = OBJtmp2; }
            OBJtmp2->prev = OBJtmp; //fix the prev-link of the next object
        }
    }
    OBJcheckflags(obj->x, obj->y); //check flags after obj was removed
    //clear any link information and position information
    obj->next = NULL;
    obj->prev = NULL;
    obj->x = 0;
    obj->y = 0;
}

unsigned char OBJmove(object* obj, unsigned long x, unsigned long y)
{
    if ((x < 0) || (x > 2047) || (y < 0) || (y > 1023)) return 1;

    if (OBJmove_allow) goto moveallow;


    if ((bt[y][x] & 1024) || (keyon[VK_SPACE] && U6O_DEBUG))
    {
        //if ((bt[y][x]&1024)||(1)){
    moveallow:
        OBJremove(obj);
        OBJadd(x, y, obj);
        return 0;
    } //walking living passable
    return 1;
}

object* OBJfindlast(unsigned long x, unsigned long y)
{
    //finds the last object (*not including floating objects)
    //used for droping items
    if ((x < 0) || (y < 0)) return NULL;
    if ((x >= 2048) || (y >= 1024)) return NULL;


    OBJtmp = od[y][x];
    if (OBJtmp != NULL)
    {
    OBJfindlast_next:
        if (OBJtmp->next != NULL)
        {
            OBJtmp2 = (object*)OBJtmp->next;

            if ((objfloatflags[(OBJtmp2->type >> 10) + sprlnk[OBJtmp2->type & 1023]] & 1) == 0)
            {
                OBJtmp = (object*)OBJtmp->next;
                goto OBJfindlast_next;
            }
        }
        if ((objfloatflags[(OBJtmp->type >> 10) + sprlnk[OBJtmp->type & 1023]] & 1) != 0) OBJtmp = NULL;
    }
    return OBJtmp;
}

object* OBJfindlastall(unsigned long x, unsigned long y)
{
    //finds the last object (including floating objects)
    //used for dropping items
    if ((x < 0) || (y < 0)) return NULL;
    if ((x >= 2048) || (y >= 1024)) return NULL;
    OBJtmp = od[y][x];
    if (OBJtmp != NULL)
    {
    OBJfindlast_next:
        if (OBJtmp->next != NULL)
        {
            OBJtmp = (object*)OBJtmp->next;
            goto OBJfindlast_next;
        }
    }
    return OBJtmp;
}

//OBJlist creates list of all internal&next objects, returns number of objects (0=none)
//NOTE: OBJlist creates a global list, user sets to NULL
unsigned long OBJlist(object* obj)
{
    static object* l[65536]; //link
    static long l_last, l_done;
    static object* myobj;
    if (obj == NULL) return OBJlist_last;
    l_last = -1;
    l_done = -1;
    myobj = obj;
OBJlist_nextobject:
    OBJlist_list[OBJlist_last] = myobj; //add object to list
    OBJlist_last++;
    if (myobj->more)
    {
        //container
        if (obji[sprlnk[myobj->type & 1023]].flags & 1024)
        {
            l_last++;
            l[l_last] = (object*)myobj->more; //add unchecked container
        }
    }
    if (myobj->next)
    {
        if ((myobj->next != myobj->more) || ((obji[sprlnk[myobj->type & 1023]].flags & 1024) == 0))
        {
            //avoid reverse linked inventory containers
            myobj = (object*)myobj->next;
            goto OBJlist_nextobject; //get next object
        }
    }
    if (l_done < l_last)
    {
        l_done++;
        myobj = l[l_done];
        goto OBJlist_nextobject; //check next container
    }
    return OBJlist_last;
}


//OBJmove2 !2003: moves a creature/NPC and adjusts picture sub-index
//x,y are it's new position

static void UpdateSlimeTile(long x, long y)
{
    if (x < 0 || x > 2047 || y < 0 || y > 1023) return;
    object* slime = OBJfindlast(x, y);
    if (!slime || (slime->type & 1023) != 375) return;

    unsigned long connections = 0;
    object* neighbor;
    if ((neighbor = OBJfindlast(x, y - 1)) && (neighbor->type & 1023) == 375) connections |= 1;
    if ((neighbor = OBJfindlast(x + 1, y)) && (neighbor->type & 1023) == 375) connections |= 2;
    if ((neighbor = OBJfindlast(x, y + 1)) && (neighbor->type & 1023) == 375) connections |= 4;
    if ((neighbor = OBJfindlast(x - 1, y)) && (neighbor->type & 1023) == 375) connections |= 8;

    slime->type = 375 + connections * 1024;
}

static void UpdateSlimeAndNeighbors(long x, long y)
{
    UpdateSlimeTile(x, y);
    UpdateSlimeTile(x, y - 1);
    UpdateSlimeTile(x + 1, y);
    UpdateSlimeTile(x, y + 1);
    UpdateSlimeTile(x - 1, y);
}

static void UpdateSlimeNeighbors(long x, long y)
{
    UpdateSlimeTile(x, y - 1);
    UpdateSlimeTile(x + 1, y);
    UpdateSlimeTile(x, y + 1);
    UpdateSlimeTile(x - 1, y);
}

static void RemoveMultiObject(object* mainObj, int count)
{
    mlobj* multiObj;
    if (mainObj->info & 2)
    {
        // piloted by NPC/Player?
        npc* pilot = (npc*)mainObj->more;
        if (!pilot || !pilot->player) return;
        multiObj = (mlobj*)pilot->player->craft_con;
    }
    else
    {
        multiObj = (mlobj*)mainObj->more;
    }
    if (!multiObj) return;
    for (int i = 0; i < count; i++)
    {
        if (multiObj->obj[i])
        {
            OBJremove(multiObj->obj[i]);
        }
    }
}

static void UpdateHydraParts(object* obj, unsigned long x, unsigned long y)
{
    npc* tnpc3 = (npc*)obj->more;
    mlobj* mmyobj = (mlobj*)tnpc3->more;
    OBJadd(x - 1, y - 1, mmyobj->obj[0]);
    OBJadd(x, y - 1, mmyobj->obj[1]);
    OBJadd(x + 1, y - 1, mmyobj->obj[2]);
    OBJadd(x + 1, y, mmyobj->obj[3]);
    OBJadd(x + 1, y + 1, mmyobj->obj[4]);
    OBJadd(x, y + 1, mmyobj->obj[5]);
    OBJadd(x - 1, y + 1, mmyobj->obj[6]);
    OBJadd(x - 1, y, mmyobj->obj[7]);
}

static void UpdateSerpentParts(object* obj, unsigned long x, unsigned long y, long oldX, long oldY, long dirX,
                               long dirY)
{
    unsigned short baseType = obj->type & 1023;
    unsigned long directionIndex = 0;
    if (dirX == -1) directionIndex = 3;
    else if (dirX == 1) directionIndex = 1;
    else if (dirY == -1) directionIndex = 0;
    else if (dirY == 1) directionIndex = 2;

    obj->type = baseType + (directionIndex << 11);

    npc* tnpc3 = (npc*)obj->more;
    mlobj* mmyobj = (mlobj*)tnpc3->more;

    for (int i = 6; i >= 1; i--)
    {
        OBJremove(mmyobj->obj[i]);
        OBJadd(mmyobj->obj[i - 1]->x, mmyobj->obj[i - 1]->y, mmyobj->obj[i]);
    }
    OBJremove(mmyobj->obj[0]);
    OBJadd(oldX, oldY, mmyobj->obj[0]);

    // Tail direction
    if (mmyobj->obj[5]->x < mmyobj->obj[6]->x) mmyobj->obj[6]->type = baseType + 7 * 1024;
    else if (mmyobj->obj[5]->x > mmyobj->obj[6]->x) mmyobj->obj[6]->type = baseType + 3 * 1024;
    else if (mmyobj->obj[5]->y < mmyobj->obj[6]->y) mmyobj->obj[6]->type = baseType + 1 * 1024;
    else if (mmyobj->obj[5]->y > mmyobj->obj[6]->y) mmyobj->obj[6]->type = baseType + 5 * 1024;

    for (int i = 5; i >= 0; i--)
    {
        unsigned long nextDir = 0;
        if (i != 0)
        {
            if (mmyobj->obj[i - 1]->x > mmyobj->obj[i]->x) nextDir = 1;
            else if (mmyobj->obj[i - 1]->x < mmyobj->obj[i]->x) nextDir = 3;
            else if (mmyobj->obj[i - 1]->y < mmyobj->obj[i]->y) nextDir = 0;
            else if (mmyobj->obj[i - 1]->y > mmyobj->obj[i]->y) nextDir = 2;
        }
        else
        {
            if (x > mmyobj->obj[i]->x) nextDir = 1;
            else if (x < mmyobj->obj[i]->x) nextDir = 3;
            else if (y < mmyobj->obj[i]->y) nextDir = 0;
            else if (y > mmyobj->obj[i]->y) nextDir = 2;
        }

        unsigned long prevDir = 0;
        if (mmyobj->obj[i + 1]->x > mmyobj->obj[i]->x) prevDir = 1;
        else if (mmyobj->obj[i + 1]->x < mmyobj->obj[i]->x) prevDir = 3;
        else if (mmyobj->obj[i + 1]->y < mmyobj->obj[i]->y) prevDir = 0;
        else if (mmyobj->obj[i + 1]->y > mmyobj->obj[i]->y) prevDir = 2;

        if (nextDir == 0)
        {
            if (prevDir == 1) mmyobj->obj[i]->type = baseType + 10 * 1024;
            if (prevDir == 2) mmyobj->obj[i]->type = baseType + 8 * 1024;
            if (prevDir == 3) mmyobj->obj[i]->type = baseType + 13 * 1024;
        }
        else if (nextDir == 1)
        {
            if (prevDir == 0) mmyobj->obj[i]->type = baseType + 10 * 1024;
            if (prevDir == 2) mmyobj->obj[i]->type = baseType + 11 * 1024;
            if (prevDir == 3) mmyobj->obj[i]->type = baseType + 9 * 1024;
        }
        else if (nextDir == 2)
        {
            if (prevDir == 0) mmyobj->obj[i]->type = baseType + 8 * 1024;
            if (prevDir == 1) mmyobj->obj[i]->type = baseType + 11 * 1024;
            if (prevDir == 3) mmyobj->obj[i]->type = baseType + 12 * 1024;
        }
        else if (nextDir == 3)
        {
            if (prevDir == 0) mmyobj->obj[i]->type = baseType + 13 * 1024;
            if (prevDir == 1) mmyobj->obj[i]->type = baseType + 9 * 1024;
            if (prevDir == 2) mmyobj->obj[i]->type = baseType + 12 * 1024;
        }
    }
}

static void UpdateDragonParts(object* obj, unsigned long x, unsigned long y, long dirX, long dirY)
{
    unsigned short baseType = obj->type & 1023;
    unsigned long directionIndex = 0;
    if (dirX == -1) directionIndex = 3;
    else if (dirX == 1) directionIndex = 1;
    else if (dirY == -1) directionIndex = 0;
    else if (dirY == 1) directionIndex = 2;

    obj->type = baseType + (directionIndex << 11);

    npc* tnpc3 = (npc*)obj->more;
    mlobj* mmyobj = (mlobj*)tnpc3->more;

    if (directionIndex == 0)
    {
        mmyobj->obj[0]->type = obj->type + 8 * 1024 + (int)(rnd * 2.0f) * 1024;
        OBJadd(x, y - 1, mmyobj->obj[0]);
        mmyobj->obj[1]->type = obj->type + 16 * 1024 + (int)(rnd * 2.0f) * 1024;
        OBJadd(x, y + 1, mmyobj->obj[1]);
        mmyobj->obj[2]->type = obj->type + 24 * 1024;
        OBJadd(x - 1, y, mmyobj->obj[2]);
        mmyobj->obj[3]->type = obj->type + 32 * 1024;
        OBJadd(x + 1, y, mmyobj->obj[3]);
    }
    else if (directionIndex == 1)
    {
        mmyobj->obj[0]->type = obj->type + 8 * 1024 + (int)(rnd * 2.0f) * 1024;
        OBJadd(x + 1, y, mmyobj->obj[0]);
        mmyobj->obj[1]->type = obj->type + 16 * 1024 + (int)(rnd * 2.0f) * 1024;
        OBJadd(x - 1, y, mmyobj->obj[1]);
        mmyobj->obj[2]->type = obj->type + 24 * 1024;
        OBJadd(x, y - 1, mmyobj->obj[2]);
        mmyobj->obj[3]->type = obj->type + 32 * 1024;
        OBJadd(x, y + 1, mmyobj->obj[3]);
    }
    else if (directionIndex == 2)
    {
        mmyobj->obj[0]->type = obj->type + 8 * 1024 + (int)(rnd * 2.0f) * 1024;
        OBJadd(x, y + 1, mmyobj->obj[0]);
        mmyobj->obj[1]->type = obj->type + 16 * 1024 + (int)(rnd * 2.0f) * 1024;
        OBJadd(x, y - 1, mmyobj->obj[1]);
        mmyobj->obj[2]->type = obj->type + 24 * 1024;
        OBJadd(x + 1, y, mmyobj->obj[2]);
        mmyobj->obj[3]->type = obj->type + 32 * 1024;
        OBJadd(x - 1, y, mmyobj->obj[3]);
    }
    else if (directionIndex == 3)
    {
        mmyobj->obj[0]->type = obj->type + 8 * 1024 + (int)(rnd * 2.0f) * 1024;
        OBJadd(x - 1, y, mmyobj->obj[0]);
        mmyobj->obj[1]->type = obj->type + 16 * 1024 + (int)(rnd * 2.0f) * 1024;
        OBJadd(x + 1, y, mmyobj->obj[1]);
        mmyobj->obj[2]->type = obj->type + 24 * 1024;
        OBJadd(x, y + 1, mmyobj->obj[2]);
        mmyobj->obj[3]->type = obj->type + 32 * 1024;
        OBJadd(x, y - 1, mmyobj->obj[3]);
    }
}

static void Update4PartCreatureParts(object* obj, unsigned long x, unsigned long y, long dirX, long dirY)
{
    unsigned short baseType = obj->type & 1023;
    unsigned long animFrame = (obj->type >> 12) % 3;
    animFrame++;
    if (animFrame == 3) animFrame = 0;

    if (baseType == 424)
    {
        // cyclops
        animFrame = (obj->type >> 12) & 1;
        animFrame++;
        if (animFrame == 2) animFrame = 0;
    }

    unsigned long directionIndex = 0;
    if (dirX == -1) directionIndex = 3;
    else if (dirX == 1) directionIndex = 1;
    else if (dirY == -1) directionIndex = 0;
    else if (dirY == 1) directionIndex = 2;

    if (baseType == 424)
    {
        obj->type = baseType + (animFrame << 12) + (directionIndex << 13) + 1024 * 3;
    }
    else
    {
        obj->type = baseType + (animFrame << 12) + (directionIndex * 12288) + 1024 * 3;
    }

    npc* tnpc3 = (npc*)obj->more;
    if (tnpc3)
    {
        mlobj* mmyobj = (mlobj*)tnpc3->more;
        mmyobj->obj[0]->type = obj->type - 3 * 1024;
        OBJadd(x - 1, y - 1, mmyobj->obj[0]);
        mmyobj->obj[1]->type = obj->type - 2 * 1024;
        OBJadd(x, y - 1, mmyobj->obj[1]);
        mmyobj->obj[2]->type = obj->type - 1 * 1024;
        OBJadd(x - 1, y, mmyobj->obj[2]);
    }
}

static int GetDirectionIndex(long dirX, long dirY)
{
    if (dirY == -1) return 0; // North
    if (dirX == 1) return 1; // East
    if (dirY == 1) return 2; // South
    if (dirX == -1) return 3; // West
    return 0;
}

static void UpdateBalloonParts(object* obj, unsigned long x, unsigned long y)
{
    mlobj* mmyobj;
    if (obj->info & 2)
    {
        npc* tnpc = (npc*)obj->more;
        player* tplayer = tnpc->player;
        mmyobj = (mlobj*)tplayer->craft_con;
    }
    else
    {
        mmyobj = (mlobj*)obj->more;
    }
    OBJadd(x - 1, y - 1, mmyobj->obj[0]);
    OBJadd(x, y - 1, mmyobj->obj[1]);
    OBJadd(x - 1, y, mmyobj->obj[2]);
}

static void UpdateTanglevineBase(object* obj, unsigned long x, unsigned long y)
{
    creature* tcrt = (creature*)obj->more;
    mlobj* mmyobj = (mlobj*)tcrt->more;

    for (int i = 0; i < 4; i++)
    {
        object* tendril = mmyobj->obj[i];
        int offsetX = 0, offsetY = 0;
        if (i == 0) offsetY = -1;
        else if (i == 1) offsetY = 1;
        else if (i == 2) offsetX = -1;
        else if (i == 3) offsetX = 1;

        if (i < 2) tendril->type = 366 + 1 * 1024;
        else tendril->type = 366;

        OBJremove(tendril);
        OBJadd(x + offsetX, y + offsetY, tendril);

        creature* tendrilCrt = (creature*)tendril->more;
        object* child = (object*)tendrilCrt->more;
        while (child != obj)
        {
            OBJremove(child);
            child = (object*)child->more;
        }
        tendrilCrt->more = obj;
    }
}

static void UpdateTanglevineTendril(object* obj, unsigned long x, unsigned long y, long dirX, long dirY)
{
    unsigned long anim = obj->type >> 10;
    object* newPart = OBJnew();
    newPart->type = 366 + (anim << 10);
    creature* tcrt = (creature*)obj->more;
    object* prevPart = (object*)tcrt->more;
    newPart->more = tcrt->more;
    OBJadd(x - dirX, y - dirY, newPart);

    tcrt->more = newPart;

    long prevX = prevPart->x;
    long prevY = prevPart->y;

    if (prevX < newPart->x)
    {
        newPart->type = 366 + (0 << 10);
        if (dirY == 1) newPart->type = 366 + (4 << 10);
        if (dirY == -1) newPart->type = 366 + (5 << 10);
    }
    else if (prevX > newPart->x)
    {
        newPart->type = 366 + (0 << 10);
        if (dirY == 1) newPart->type = 366 + (3 << 10);
        if (dirY == -1) newPart->type = 366 + (2 << 10);
    }
    else if (prevY < newPart->y)
    {
        newPart->type = 366 + (1 << 10);
        if (dirX == 1) newPart->type = 366 + (2 << 10);
        if (dirX == -1) newPart->type = 366 + (5 << 10);
    }
    else if (prevY > newPart->y)
    {
        newPart->type = 366 + (1 << 10);
        if (dirX == 1) newPart->type = 366 + (3 << 10);
        if (dirX == -1) newPart->type = 366 + (4 << 10);
    }

    if (dirX) obj->type = 366;
    else obj->type = 366 + 1024;
}

static void UpdatePersonPose(object* obj, long dirX, long dirY)
{
    unsigned short typeID = obj->type & 1023;
    unsigned long animFrame = (obj->type >> 10) & 3;

    if (typeID == 363) animFrame = (obj->type >> 10) % 3; // gargoyle

    if (obj->info & 32768)
    {
        if ((animFrame == 1) && (obj->info & 1))
        {
            animFrame = 0;
            obj->info -= 1;
        }
        else
        {
            animFrame++;
            if (animFrame == 4)
            {
                obj->info |= 1;
                animFrame = 1;
            }
            if (animFrame == 3)
            {
                obj->info |= 1;
                animFrame = 1;
            }
        }
        obj->info -= 32768;
    }

    int directionIndex = GetDirectionIndex(dirX, dirY);

    if (typeID == 363)
    {
        obj->type = typeID + (animFrame << 10) + (directionIndex << 10) * 3;
    }
    else
    {
        obj->type = typeID + (animFrame << 10) + (directionIndex << 12);
    }
}

static void UpdateBoatParts(object* obj, unsigned long x, unsigned long y, int directionIndex)
{
    mlobj* mmyobj;
    if (obj->info & 2)
    {
        npc* tnpc = (npc*)obj->more;
        player* tplayer = tnpc->player;
        mmyobj = (mlobj*)tplayer->craft_con;
    }
    else
    {
        mmyobj = (mlobj*)obj->more;
    }

    if (directionIndex == 0) // North
    {
        mmyobj->obj[0]->type = obj->type - 1 * 1024;
        OBJadd(x - 1, y, mmyobj->obj[0]);
        mmyobj->obj[1]->type = obj->type - 8 * 1024;
        OBJadd(x, y - 1, mmyobj->obj[1]);
        mmyobj->obj[2]->type = obj->type - 9 * 1024;
        OBJadd(x - 1, y - 1, mmyobj->obj[2]);
        mmyobj->obj[3]->type = obj->type + 8 * 1024;
        OBJadd(x, y + 1, mmyobj->obj[3]);
        mmyobj->obj[4]->type = obj->type + 7 * 1024;
        OBJadd(x - 1, y + 1, mmyobj->obj[4]);
    }
    else if (directionIndex == 2) // South
    {
        mmyobj->obj[0]->type = obj->type - 1 * 1024;
        OBJadd(x - 1, y, mmyobj->obj[0]);
        mmyobj->obj[1]->type = obj->type - 8 * 1024;
        OBJadd(x, y + 1, mmyobj->obj[1]);
        mmyobj->obj[2]->type = obj->type - 9 * 1024;
        OBJadd(x - 1, y + 1, mmyobj->obj[2]);
        mmyobj->obj[3]->type = obj->type + 8 * 1024;
        OBJadd(x, y - 1, mmyobj->obj[3]);
        mmyobj->obj[4]->type = obj->type + 7 * 1024;
        OBJadd(x - 1, y - 1, mmyobj->obj[4]);
    }
    else if (directionIndex == 1) // East
    {
        mmyobj->obj[0]->type = obj->type - 1 * 1024;
        OBJadd(x, y - 1, mmyobj->obj[0]);
        mmyobj->obj[1]->type = obj->type - 8 * 1024;
        OBJadd(x + 1, y, mmyobj->obj[1]);
        mmyobj->obj[2]->type = obj->type - 9 * 1024;
        OBJadd(x + 1, y - 1, mmyobj->obj[2]);
        mmyobj->obj[3]->type = obj->type + 8 * 1024;
        OBJadd(x - 1, y, mmyobj->obj[3]);
        mmyobj->obj[4]->type = obj->type + 7 * 1024;
        OBJadd(x - 1, y - 1, mmyobj->obj[4]);
    }
    else if (directionIndex == 3) // West
    {
        mmyobj->obj[0]->type = obj->type - 1 * 1024;
        OBJadd(x, y - 1, mmyobj->obj[0]);
        mmyobj->obj[1]->type = obj->type - 8 * 1024;
        OBJadd(x - 1, y, mmyobj->obj[1]);
        mmyobj->obj[2]->type = obj->type - 9 * 1024;
        OBJadd(x - 1, y - 1, mmyobj->obj[2]);
        mmyobj->obj[3]->type = obj->type + 8 * 1024;
        OBJadd(x + 1, y, mmyobj->obj[3]);
        mmyobj->obj[4]->type = obj->type + 7 * 1024;
        OBJadd(x + 1, y - 1, mmyobj->obj[4]);
    }
}

static void UpdateSlime(object* obj, unsigned long x, unsigned long y)
{
    unsigned long connections = 0;
    object* neighbor;

    if (neighbor = OBJfindlast(x, y - 1))
    {
        if ((neighbor->type & 1023) == 375)
        {
            connections |= 1;
            UpdateSlimeTile(neighbor->x, neighbor->y);
        }
    }
    if (neighbor = OBJfindlast(x + 1, y))
    {
        if ((neighbor->type & 1023) == 375)
        {
            connections |= 2;
            UpdateSlimeTile(neighbor->x, neighbor->y);
        }
    }
    if (neighbor = OBJfindlast(x, y + 1))
    {
        if ((neighbor->type & 1023) == 375)
        {
            connections |= 4;
            UpdateSlimeTile(neighbor->x, neighbor->y);
        }
    }
    if (neighbor = OBJfindlast(x - 1, y))
    {
        if ((neighbor->type & 1023) == 375)
        {
            connections |= 8;
            UpdateSlimeTile(neighbor->x, neighbor->y);
        }
    }
    obj->type = 375 + connections * 1024;
}

static void Update2PartCreatureParts(object* obj, unsigned long x, unsigned long y, long dirX, long dirY)
{
    unsigned short baseType = obj->type & 1023;
    unsigned long animFrame = (obj->type >> 10) & 1;
    animFrame++;
    if (animFrame == 2) animFrame = 0;

    unsigned long directionIndex = 0;
    if (dirX == -1) directionIndex = 3;
    else if (dirX == 1) directionIndex = 1;
    else if (dirY == -1) directionIndex = 0;
    else if (dirY == 1) directionIndex = 2;

    obj->type = baseType + (animFrame << 10) + (directionIndex << 11);

    creature* crt = (creature*)obj->more;
    object* backPart = (object*)crt->more;
    backPart->type = obj->type + 8 * 1024;
    OBJremove(backPart);
    OBJadd(x - (long)dirX, y - (long)dirY, backPart);
}

static void RemoveConstructs(object* obj, unsigned short type)
{
    if (type == 412) RemoveMultiObject(obj, 5); // boat
    else if (obji[sprlnk[type]].v4 == 5) RemoveMultiObject(obj, 3); // 4-part
    else if (obji[sprlnk[type]].v4 == 9) RemoveMultiObject(obj, 4); // dragon
    else if (obji[sprlnk[type]].v4 == 11) RemoveMultiObject(obj, 8); // hydra
}

static bool CanObjectMoveTo(object* obj, unsigned long x, unsigned long y)
{
    if (x > 2047 || y > 1023) return false;
    if (OBJmove_allow) return true;

    unsigned short type = obj->type & 1023;
    int passMode = 0;
    if (type == 345 || type == 346) passMode = 2; // squid, sea serpent
    if (type == 343 || type == 344 || type == 356 || type == 362 || type == 369 || type == 372 || type == 373 || type ==
        411)
        passMode = 3; // air

    if (type == 366)
    {
        // tangle vine tendril
        if (od[y][x]) return false;
    }

    if (type == 352)
    {
        // ghost
        object* destObj = OBJfindlast(x, y);
        if (destObj && (destObj->info & (2 + 4))) return false;
        return true;
    }

    if (type == 354)
    {
        // mouse
        if ((bt[y][x] & 1024) == 0)
        {
            // blocked
            object* destObj = OBJfindlastall(x, y);
            if (destObj && (destObj->type & 1023) == 213) return true; // mousehole
            if (destObj && destObj->prev)
            {
                object* prevObj = (object*)destObj->prev;
                if ((prevObj->type & 1023) == 310 && (prevObj->type & 1024)) return true; // portcullis
            }
        }
    }

    if (type == 413)
    {
        // silver serpent
        if (((obj->type >> 10) == 0) && (y > obj->y)) return false;
        if (((obj->type >> 10) == 2) && (x < obj->x)) return false;
        if (((obj->type >> 10) == 4) && (y < obj->y)) return false;
        if (((obj->type >> 10) == 6) && (x > obj->x)) return false;
        if (((obj->type >> 10) == 14) && (y > obj->y)) return false;
        if (((obj->type >> 10) == 15) && (x < obj->x)) return false;
    }

    if (passMode == 2)
    {
        // sea
        object* destObj = OBJfindlast(x, y);
        if (destObj && (destObj->info & (2 + 4))) return false;
        if (bt[y][x] & (4 * 1024)) return true;
        return false;
    }

    if (passMode == 3)
    {
        // air
        object* destObj = OBJfindlast(x, y);
        if (destObj && (destObj->info & (2 + 4))) return false;
        if (bt[y][x] & (2 * 1024)) return true;
        return false;
    }

    if (type == 423)
    {
        // balloon
        object* destObj = OBJfindlast(x, y);
        if (destObj && (destObj->info & (2 + 4))) return false;
        if (bt[y][x] & (4 * 1024)) return true; // over sea
        if (bt[y][x] & 1024)
        {
            object* destObjAll = OBJfindlastall(x, y);
            if (destObjAll)
            {
                unsigned short destType = destObjAll->type & 1023;
                if (destType >= 297 && destType <= 302) return false; // doorway or door
            }
            return true;
        }
        if (od[y][x] && (od[y][x]->type & 1023) == 423)
        {
            if ((bt[y][x] & 1023) <= 124) return true;
        }
        if (od[y][x] == NULL)
        {
            if ((bt[y][x] & 1023) <= 124) return true;
        }
        return false;
    }

    // ships
    if (type == 412)
    {
        // ship
        object* destObj = OBJfindlast(x, y);
        if (destObj && (destObj->info & 4)) return false;
        unsigned short tile = bt[y][x] & 1023;
        if ((tile >= 8 && tile < 16) && (bt[y][x] & (4 * 1024))) return true;
        return false;
    }
    if (type == 414 || type == 415)
    {
        // skiff, raft
        object* destObj = OBJfindlast(x, y);
        if (destObj && (destObj->info & 4)) return false;
        if (bt[y][x] & (4 * 1024)) return true;
        return false;
    }

    if (obji[sprlnk[type]].v4 == 10)
    {
        // serpent body parts
        object* destObj = OBJfindlast(x, y);
        if (destObj && obji[sprlnk[destObj->type & 1023]].v4 == 10)
        {
            if ((destObj->info & 8) && destObj->more == obj) return true;
        }
    }

    // check for animals and fences
    if (type == 428 || type == 430 || type == 348)
    {
        if (od[y][x] && (od[y][x]->type & 1023) == 281) return false;
    }

    if (obji[sprlnk[type]].v4 == 8)
    {
        // 2-part
        object* destObj = OBJfindlast(x, y);
        if (destObj && obji[sprlnk[destObj->type & 1023]].v4 == 8)
        {
            if ((destObj->info & 8) && destObj->more == obj) return true;
        }
    }

    if ((bt[y][x] & 1024) || (keyon[VK_SPACE] && U6O_DEBUG)) return true;

    return false;
}

unsigned char OBJmove2(object* obj, unsigned long x, unsigned long y)
{
    if (x > 2047 || y > 1023) return 1;

    if (!CanObjectMoveTo(obj, x, y)) return 1;

    unsigned long oldX = obj->x;
    unsigned long oldY = obj->y;
    long dirX = 0, dirY = 0;
    unsigned short typeID = obj->type & 1023;

    if (oldX == 0 && oldY == 0)
    {
        dirY = 1;
        dirX = 0; // face forward
    }
    else
    {
        dirX = (x > oldX) ? 1 : (x < oldX ? -1 : 0);
        dirY = (y > oldY) ? 1 : (y < oldY ? -1 : 0);

        OBJremove(obj); // remove from old location

        if (typeID == 423) RemoveMultiObject(obj, 3);
        else if (typeID == 375) UpdateSlimeNeighbors(oldX, oldY);
        else RemoveConstructs(obj, typeID);
    }

    int moveType = 0;
    if (obji[sprlnk[typeID]].v4 == 1) moveType = 1; // person
    if (typeID == 412) moveType = 254; // boat
    if (obji[sprlnk[typeID]].v4 == 2) moveType = 2; // demon, mongbat, gargoyle
    if (obji[sprlnk[typeID]].v4 == 6) moveType = 2; // drake
    if (obji[sprlnk[typeID]].v4 == 8) moveType = 3; // 2-part
    if (obji[sprlnk[typeID]].v4 == 5) moveType = 4; // 4-part
    if (obji[sprlnk[typeID]].v4 == 9) moveType = 9; // dragon
    if (obji[sprlnk[typeID]].v4 == 10) moveType = 10; // serpent
    if (obji[sprlnk[typeID]].v4 == 11) moveType = 11; // hydra
    if (obji[sprlnk[typeID]].v4 == 4) moveType = 5; // mouse/rabbit
    if (typeID == 414) moveType = 255; // skiff

    if (typeID == 423)
    {
        // balloon
        UpdateBalloonParts(obj, x, y);
    }

    if (typeID == 365)
    {
        // tangle vine
        UpdateTanglevineBase(obj, x, y);
    }

    if (typeID == 366)
    {
        // tanglevine tendril
        UpdateTanglevineTendril(obj, x, y, dirX, dirY);
    }

    if (moveType == 11)
    {
        // hydra
        UpdateHydraParts(obj, x, y);
    }

    if (moveType == 10)
    {
        // serpent
        UpdateSerpentParts(obj, x, y, oldX, oldY, dirX, dirY);
    }

    if (moveType == 9)
    {
        // dragon
        UpdateDragonParts(obj, x, y, dirX, dirY);
    }

    if (moveType == 4)
    {
        // 4-part cyclops/winged gargoyle
        Update4PartCreatureParts(obj, x, y, dirX, dirY);
    }

    if (moveType == 3)
    {
        // 2-part creature
        Update2PartCreatureParts(obj, x, y, dirX, dirY);
    }

    if (moveType == 1)
    {
        UpdatePersonPose(obj, dirX, dirY);
    }

    if (moveType == 2)
    {
        // demon, drake, etc
        unsigned long animFrame = (obj->type >> 10) & 1;
        animFrame++;
        if (animFrame == 2) animFrame = 0;
        int directionIndex = GetDirectionIndex(dirX, dirY);
        obj->type = typeID + (animFrame << 10) + (directionIndex << 11);
    }

    if (moveType == 255 || moveType == 5)
    {
        // skiff or mouse/rabbit
        int directionIndex = GetDirectionIndex(dirX, dirY);
        obj->type = typeID + (directionIndex << 10);
    }

    if (moveType == 254)
    {
        // boat
        int directionIndex = GetDirectionIndex(dirX, dirY);
        obj->type = typeID + ((directionIndex * 2 + 9) << 10);
        UpdateBoatParts(obj, x, y, directionIndex);
    }

    OBJadd(x, y, obj);

    if (typeID == 375)
    {
        // slime
        UpdateSlime(obj, x, y);
    }

    return 0;
}


//OBJdir changes direction of NPC/creature to face x,y
unsigned char OBJdir(object* obj, unsigned long x, unsigned long y)
{
    static long dx, dy;
    static unsigned short movertype;
    static creature* crt;
    static object* obj2;

    movertype = obj->type & 1023;

    if (obji[sprlnk[movertype]].v4 == 8)
    {
        //two part movers (only frame change)
        if (obj->info & (2 + 4))
        {
            //crt or npc
            crt = (creature*)obj->more;
            obj2 = (object*)crt->more;
            if (obj->type & 1024)
            {
                obj->type -= 1024;
                if (obj2->type & 1024) obj2->type -= 1024;
            }
            else
            {
                obj->type |= 1024;
                obj2->type |= 1024;
            }
        } //crt or npc
        return NULL;
    }

    if (movertype == 366) return NULL; //tanglevine tendril
    if (movertype == 365) return NULL; //tanglevine core
    if (movertype == 413)
    {
        //serpent
        //open & close mouth
        if (obj->type == (413 + 0 * 1024))
        {
            obj->type = 413 + 14 * 1024;
            return NULL;
        }
        if (obj->type == (413 + 2 * 1024))
        {
            obj->type = 413 + 15 * 1024;
            return NULL;
        }
        if (obj->type == (413 + 14 * 1024))
        {
            obj->type = 413 + 0 * 1024;
            return NULL;
        }
        if (obj->type == (413 + 15 * 1024))
        {
            obj->type = 413 + 2 * 1024;
            return NULL;
        }
        return NULL;
    }
    if (movertype == 347) return NULL; //reaper
    if (movertype == 343) return NULL; //insects
    if (movertype == 344) return NULL; //bat
    if (movertype == 345) return NULL; //squid
    if (movertype == 352) return NULL; //ghost
    if (movertype == 353) return NULL; //gremlin
    if (movertype == 355) return NULL; //gazer
    if (movertype == 356) return NULL; //bird
    if (movertype == 357) return NULL; //corpser
    if (movertype == 360) return NULL; //rotworms
    if (movertype == 364) return NULL; //acid slug
    if (movertype == 373) return NULL; //wisp
    if (movertype == 374) return NULL; //hydra
    if (movertype == 425) return NULL; //hydra (tenticles)
    if (movertype == 375) return NULL; //slime
    if (movertype == 391) return NULL; //hoeing farmer
    if (movertype == 415) return NULL; //raft

    //current method cannot be used if near map boundaries!
    if ((obj->x <= 1) || (obj->y <= 1) || (obj->x >= 2046) || (obj->y >= 1022)) return NULL;


    dx = 0;
    dy = 0;
    if (x > obj->x) dx = 1;
    if (x < obj->x) dx = -1;
    if (y > obj->y) dy = 1;
    if (y < obj->y) dy = -1;

    if (abs((long)(x - obj->x)) > abs((long)(y - obj->y))) dy = 0;
    else dx = 0;


    OBJmove_allow = TRUE;
    OBJmove2(obj, obj->x - dx, obj->y - dy);
    //OBJmove2(obj,obj->x-dx,obj->y-dy);
    //OBJmove2(obj,obj->x+dx*2,obj->y+dy*2);
    OBJmove2(obj, obj->x + dx, obj->y + dy);
    OBJmove_allow = FALSE;

    return NULL;

    /*
    if ((xx>=376)&&(xx<=387)) xx2=1; //fighter...woman
    if ((xx>=409)&&(xx<=410)) xx2=1; //LB+avatar
    if ((xx>=367)&&(xx<=372)) xx2=2; //demon...mongbat
  
    if (xx2==2){
    xx3=(obj->type>>10)&1;
    //fix direction
    if (dx==-1) xx4=3;
    if (dx==1) xx4=1;
    if (dy==-1) xx4=0;
    if (dy==1) xx4=2;
    obj->type=xx+(xx3<<10)+(xx4<<11);
    } //xx2=2
    */

    return NULL;
}


static unsigned long GetTilePathfindWeight(int x, int y, unsigned long type, unsigned long flags, npc* pathfindingNpc)
{
    int worldX = (int)WPF_OFFSETX + x;
    int worldY = (int)WPF_OFFSETY + y;

    if (worldX < 0 || worldY < 0 || worldX >= 2048 || worldY >= 1024) return 0;

    unsigned short tile = bt[worldY][worldX];
    unsigned short tileType = tile & 1023;
    unsigned long weight = 0;

    if (type == 0) // NPC/Creature
    {
        unsigned long penalty = 0;
        if ((tile & 1024) == 0) // Not land passable
        {
            if ((tile & 16384) == 0) return 0; // Not NPC-swap passable

            if (flags & 2) penalty = 65536; // Last resort
            else if (flags & 1)
            {
                object* obj = od[worldY][worldX];
                while (obj)
                {
                    if (obj->info & 2)
                    {
                        npc* tnpc = (npc*)obj->more;
                        if (pathfindingNpc->player == tnpc->player)
                        {
                            penalty = 16;
                            break;
                        }
                    }
                    obj = (object*)obj->next;
                }
            }
            if (penalty == 0) return 0; // Still blocked if no penalty set but not passable
        }

        weight = 4;
        if (flags & 4) weight = 12; // Avoid non-path tiles for scheduled NPCs

        if ((tileType >= 108 && tileType <= 111) || (tileType >= 208 && tileType <= 220)) weight = 3; // Path
        else if (tileType >= 3 && tileType <= 6) // Swamp
        {
            weight = 44;
            if (pathfindingNpc && pathfindingNpc->boots && pathfindingNpc->boots->type == 28) weight = 4;
        }
        else if (tileType == 221) weight = 36; // Light lava
        else if (tileType == 222) weight = 44; // Medium lava
        else if (tileType == 223) weight = 52; // Heavy lava

        object* obj = od[worldY][worldX];
        while (obj)
        {
            unsigned short objType = obj->type & 1023;
            if (objType == 172) weight = 36; // Spikes
            else if (objType == 173 && (obj->info & (1 << 9))) weight = 44; // Visible trap
            else if (objType == 317) weight = 36; // Fire field
            else if (objType == 318 && (pathfindingNpc && (pathfindingNpc->flags & 1) == 0)) weight = 44;
                // Poison field (if not poisoned)
            else if (objType == 320) weight = 36; // Sleep field
            else if (objType == 366) weight = 28; // Tendril
            obj = (object*)obj->next;
        }
        weight += penalty;
    }
    else if (type == 1) // Land passable check
    {
        if (tile & 1024) weight = 1;
    }
    else if (type == 2) // Sea passable check
    {
        if (tile & 4096) weight = 1;
    }
    else if (type == 3) // Air passable check
    {
        if (tile & 2048) weight = 1;
    }
    else if (type == 4) // Ghost passable check
    {
        object* obj = OBJfindlast(worldX, worldY);
        weight = 1;
        if (obj && (obj->info & 6)) weight = 0;
    }
    else if (type == 5) // Tanglevine tendril passable check
    {
        weight = 1;
        if (!(tile & 1024)) weight = 0;
        if (od[worldY][worldX]) weight = 0;
    }

    return weight;
}

void wpf_pathfind(unsigned char* d, long sourcex, long sourcey, long destx, long desty, long maxdistance,
                  unsigned long type, unsigned long flags)
{
    static long x, x2, x3, x4, y, y2, y3, y4, z, z2, z3, z4, zlooplast; //temp use variables
    static object *obj, obj2, obj3, obj4;
    static npc *tnpc, *tnpc2;
    static mlobj* mmyobj;

    wpf_nextto = WPF_NEXTTO;
    WPF_NEXTTO = 0;

    if ((flags & 8) == 0)
    {
        //return if dest is the same as source
        if ((destx == sourcex) && (desty == sourcey))
        {
            WPF_PATHLENGTH = 0;
            WPF_RETURN = WPF_PATHFOUND;
            return;
        }
        if (wpf_nextto)
        {
            if ((abs(destx - sourcex) <= 1) && (abs(desty - sourcey) <= 1))
            {
                WPF_PATHLENGTH = 0;
                WPF_RETURN = WPF_PATHFOUND;
                return;
            }
        } //wpf_nextto
        //return if dest is out of range of maxdistance
        x = 0;
        if (wpf_nextto) x = 1;
        if ((abs(destx - sourcex) + x) > maxdistance)
        {
            WPF_RETURN = WPF_NOPATHFOUND;
            return;
        }
        if ((abs(desty - sourcey) + x) > maxdistance)
        {
            WPF_RETURN = WPF_NOPATHFOUND;
            return;
        }
    } //(flags&8)==0

    WPF_OFFSETX = sourcex - maxdistance - 1;
    WPF_OFFSETY = sourcey - maxdistance - 1;
    z = maxdistance * 2 + 2; //array limit

    ZeroMemory(&wpf_sourcedest, sizeof(wpf_sourcedest));

    //set boundries
    for (x = 0; x <= z; x++)
    {
        wpf_weight[x][0] = 0;
        wpf_weight[x][z] = 0;
        wpf_weight[0][x] = 0;
        wpf_weight[z][x] = 0;
    }


    if (type == 0)
    {
        //type 0 setup
        wpf_npc = (npc*)WPF_OBJECT->more;
    }

    //create weightings
    for (y = 1; y < z; y++)
    {
        for (x = 1; x < z; x++)
        {
            wpf_weight[x][y] = GetTilePathfindWeight(x, y, type, flags, wpf_npc);
        }
    } //x,y
    //wpfdebug1:

    wpf_pathfound = 0;
    wpf_lastusedn = -1;
    wpf_laststackedn = 0;
    ZeroMemory(&wpf_nweight, sizeof(wpf_nweight));

    //allow squares belonging to parts of the mover calling the pathfind to be passable
    if (obji[sprlnk[WPF_OBJECT->type & 1023]].v4 == 8)
    {
        //2-part mover
        obj = WPF_OBJECT;
        if (obj->info & 8) obj = (object*)obj->more; //get "head" of mover
        wpf_npc = (npc*)obj->more;
        obj = (object*)wpf_npc->more;

        //NEW GPF THIS LINE! X2!
        wpf_weight[obj->x - WPF_OFFSETX][obj->y - WPF_OFFSETY] = 1;
    } //8
    if (obji[sprlnk[WPF_OBJECT->type & 1023]].v4 == 10)
    {
        //silver serpent
        obj = WPF_OBJECT;
        if (obj->info & 8) obj = (object*)obj->more; //get "head" of mover
        wpf_npc = (npc*)obj->more;
        mmyobj = (mlobj*)wpf_npc->more;
        for (z = 1; z <= 6; z++)
        {
            wpf_weight[mmyobj->obj[z]->x - WPF_OFFSETX][mmyobj->obj[z]->y - WPF_OFFSETY] = 1;
        } //z
        wpf_weight[mmyobj->obj[0]->x - WPF_OFFSETX][mmyobj->obj[0]->y - WPF_OFFSETY] = 0;
    } //10
    //wpfdebug2:

    if ((flags & 8) == 0)
    {
        //add dest node(s)
        x = destx - WPF_OFFSETX;
        y = desty - WPF_OFFSETY;
        if (wpf_nextto)
        {
            //extra dest nodes can only be added if the square they are on is walkable!

            wpf_weight[x][y] = 0;
            y--;
            if (z = wpf_weight[x][y])
            {
                wpf_lastusedn++;
                wpf_nx[wpf_lastusedn] = x;
                wpf_ny[wpf_lastusedn] = y;
                wpf_nsource[wpf_lastusedn] = 2;
                wpf_nweight[wpf_lastusedn] = z;
                wpf_sourcedest[x][y] = 2;
                wpf_bestweight[x][y] = z;
            }
            y++;
            x++;
            if (z = wpf_weight[x][y])
            {
                wpf_lastusedn++;
                wpf_nx[wpf_lastusedn] = x;
                wpf_ny[wpf_lastusedn] = y;
                wpf_nsource[wpf_lastusedn] = 2;
                wpf_nweight[wpf_lastusedn] = z;
                wpf_sourcedest[x][y] = 2;
                wpf_bestweight[x][y] = z;
            }
            x--;
            y++;
            if (z = wpf_weight[x][y])
            {
                wpf_lastusedn++;
                wpf_nx[wpf_lastusedn] = x;
                wpf_ny[wpf_lastusedn] = y;
                wpf_nsource[wpf_lastusedn] = 2;
                wpf_nweight[wpf_lastusedn] = z;
                wpf_sourcedest[x][y] = 2;
                wpf_bestweight[x][y] = z;
            }
            y--;
            x--;
            if (z = wpf_weight[x][y])
            {
                wpf_lastusedn++;
                wpf_nx[wpf_lastusedn] = x;
                wpf_ny[wpf_lastusedn] = y;
                wpf_nsource[wpf_lastusedn] = 2;
                wpf_nweight[wpf_lastusedn] = z;
                wpf_sourcedest[x][y] = 2;
                wpf_bestweight[x][y] = z;
            }
            x++;

            //if (wpf_lastusedn==-1){
            x++;
            y--;
            if (z = wpf_weight[x][y])
            {
                wpf_lastusedn++;
                wpf_nx[wpf_lastusedn] = x;
                wpf_ny[wpf_lastusedn] = y;
                wpf_nsource[wpf_lastusedn] = 2;
                wpf_nweight[wpf_lastusedn] = z;
                wpf_sourcedest[x][y] = 2;
                wpf_bestweight[x][y] = z;
            }
            y += 2;
            if (z = wpf_weight[x][y])
            {
                wpf_lastusedn++;
                wpf_nx[wpf_lastusedn] = x;
                wpf_ny[wpf_lastusedn] = y;
                wpf_nsource[wpf_lastusedn] = 2;
                wpf_nweight[wpf_lastusedn] = z;
                wpf_sourcedest[x][y] = 2;
                wpf_bestweight[x][y] = z;
            }
            x -= 2;
            if (z = wpf_weight[x][y])
            {
                wpf_lastusedn++;
                wpf_nx[wpf_lastusedn] = x;
                wpf_ny[wpf_lastusedn] = y;
                wpf_nsource[wpf_lastusedn] = 2;
                wpf_nweight[wpf_lastusedn] = z;
                wpf_sourcedest[x][y] = 2;
                wpf_bestweight[x][y] = z;
            }
            y -= 2;
            if (z = wpf_weight[x][y])
            {
                wpf_lastusedn++;
                wpf_nx[wpf_lastusedn] = x;
                wpf_ny[wpf_lastusedn] = y;
                wpf_nsource[wpf_lastusedn] = 2;
                wpf_nweight[wpf_lastusedn] = z;
                wpf_sourcedest[x][y] = 2;
                wpf_bestweight[x][y] = z;
            }
            //}


            /*
            //wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;
            x++; y--;
            if(wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;}
            y+=2;
            if(wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;}
            x-=2;
            if(wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;}
            y-=2;
            if(wpf_weight[x][y]){ wpf_lastusedn++; wpf_nx[wpf_lastusedn]=x; wpf_ny[wpf_lastusedn]=y; wpf_nsource[wpf_lastusedn]=2; wpf_nweight[wpf_lastusedn]=1; wpf_sourcedest[x][y]=2; wpf_bestweight[x][y]=1;}
            */

            if (wpf_lastusedn == -1)
            {
                WPF_RETURN = WPF_NOPATHFOUND;
                return;
            }
            //could crash be because this line should be ==0???
        }
        else
        {
            //singular dest node
            wpf_lastusedn++;
            wpf_nx[wpf_lastusedn] = x;
            wpf_ny[wpf_lastusedn] = y;
            wpf_nsource[wpf_lastusedn] = 2;
            wpf_nweight[wpf_lastusedn] = 1;
            wpf_sourcedest[x][y] = 2;
            wpf_bestweight[x][y] = 1;
        }
    } //(flags&8)==0

    //add source node
    wpf_lastusedn++;
    wpf_nx[wpf_lastusedn] = maxdistance + 1;
    wpf_ny[wpf_lastusedn] = maxdistance + 1;
    wpf_nsource[wpf_lastusedn] = 1;
    wpf_nweight[wpf_lastusedn] = 1;
    wpf_sourcedest[maxdistance + 1][maxdistance + 1] = 1;
    wpf_bestweight[maxdistance + 1][maxdistance + 1] = 1;

    static unsigned long ild;
    ild = 0;

    static unsigned long wpf_goalweight, wpf_goalx, wpf_goaly;
    wpf_goalweight = 0x7FFFFFFF;

    nweight = 1;
wpf_pathfindloop:

    if (nweight >= wpf_goalweight)
    {
        x = wpf_goalx;
        y = wpf_goaly;
        goto wpf_pathfound;
    }


    ild++;
    if (ild >= 65536)
        MessageBox(NULL, "ild exceeded!", "Ultima 6 Online", MB_OK);

    if (wpf_lastusedn == -1)
    {
        if (wpf_goalweight == 0x7FFFFFFF)
        {
            WPF_RETURN = WPF_NOPATHFOUND;
            return;
        }
    }

    wpf_nodeaddflags = 0;
    nextnweight = 0x7FFFFFFF;
    z = 0;
    zlooplast = wpf_lastusedn;
wpf_zloop:
    if (z2 = wpf_nweight[z])
    {
        if (z2 == nweight)
        {
            if (wpf_nsource[z] == 1)
            {
                //source node
                z3 = 1;
                z4 = 2;
            }
            else
            {
                //dest node
                z3 = 2;
                z4 = 1;
            } //node type


            //scan adjacent squares
            x = wpf_nx[z];
            y = wpf_ny[z];


            //if (wpf_sourcedest[x][y]&4) goto wpf_pathfound;//check for path joined bit


            x++;
            if (x2 = wpf_weight[x][y])
            {
                //weight exists? if so square is passable
                if (y2 = wpf_sourcedest[x][y])
                {
                    if (y2 & z4)
                    {
                        if ((z2 + x2) < wpf_goalweight)
                        {
                            wpf_goalweight = z2 + x2;
                            wpf_goalx = x;
                            wpf_goaly = y;
                        }
                    }
                    //if same type ignore
                }
                else
                {
                    //wpf_sourcedest==NULL
                    //add new node
                    //	wpf_naxp1:
                    wpf_nodeaddflags |= z3;
                    if (wpf_laststackedn)
                    {
                        y2 = wpf_stackn[wpf_laststackedn];
                        wpf_nx[y2] = x;
                        wpf_ny[y2] = y;
                        wpf_nsource[y2] = z3;
                        wpf_nweight[y2] = z2 + x2;
                        wpf_laststackedn--;
                    }
                    else
                    {
                        wpf_lastusedn++;
                        wpf_nx[wpf_lastusedn] = x;
                        wpf_ny[wpf_lastusedn] = y;
                        wpf_nsource[wpf_lastusedn] = z3;
                        wpf_nweight[wpf_lastusedn] = z2 + x2;
                    }
                    wpf_sourcedest[x][y] = z3;
                    wpf_bestweight[x][y] = z2 + x2; //update array
                    if ((z2 + x2) < nextnweight) nextnweight = z2 + x2;
                } //wpf_sourcedest
            } //wpf_weight

            x -= 2;
            if (x2 = wpf_weight[x][y])
            {
                //weight exists? if so square is passable
                if (y2 = wpf_sourcedest[x][y])
                {
                    if (y2 & z4)
                    {
                        if ((z2 + x2) < wpf_goalweight)
                        {
                            wpf_goalweight = z2 + x2;
                            wpf_goalx = x;
                            wpf_goaly = y;
                        }
                    }
                    //if same type ignore
                }
                else
                {
                    //wpf_sourcedest==NULL
                    //add new node
                    //	wpf_naxm1:
                    wpf_nodeaddflags |= z3;
                    if (wpf_laststackedn)
                    {
                        y2 = wpf_stackn[wpf_laststackedn];
                        wpf_nx[y2] = x;
                        wpf_ny[y2] = y;
                        wpf_nsource[y2] = z3;
                        wpf_nweight[y2] = z2 + x2;
                        wpf_laststackedn--;
                    }
                    else
                    {
                        wpf_lastusedn++;
                        wpf_nx[wpf_lastusedn] = x;
                        wpf_ny[wpf_lastusedn] = y;
                        wpf_nsource[wpf_lastusedn] = z3;
                        wpf_nweight[wpf_lastusedn] = z2 + x2;
                    }
                    wpf_sourcedest[x][y] = z3;
                    wpf_bestweight[x][y] = z2 + x2; //update array
                    if ((z2 + x2) < nextnweight) nextnweight = z2 + x2;
                } //wpf_sourcedest
            } //wpf_weight

            x++;
            y--;
            if (x2 = wpf_weight[x][y])
            {
                //weight exists? if so square is passable
                if (y2 = wpf_sourcedest[x][y])
                {
                    if (y2 & z4)
                    {
                        if ((z2 + x2) < wpf_goalweight)
                        {
                            wpf_goalweight = z2 + x2;
                            wpf_goalx = x;
                            wpf_goaly = y;
                        }
                    }
                    //if same type ignore
                }
                else
                {
                    //wpf_sourcedest==NULL
                    //add new node
                    //	wpf_naym1:
                    wpf_nodeaddflags |= z3;
                    if (wpf_laststackedn)
                    {
                        y2 = wpf_stackn[wpf_laststackedn];
                        wpf_nx[y2] = x;
                        wpf_ny[y2] = y;
                        wpf_nsource[y2] = z3;
                        wpf_nweight[y2] = z2 + x2;
                        wpf_laststackedn--;
                    }
                    else
                    {
                        wpf_lastusedn++;
                        wpf_nx[wpf_lastusedn] = x;
                        wpf_ny[wpf_lastusedn] = y;
                        wpf_nsource[wpf_lastusedn] = z3;
                        wpf_nweight[wpf_lastusedn] = z2 + x2;
                    }
                    wpf_sourcedest[x][y] = z3;
                    wpf_bestweight[x][y] = z2 + x2; //update array
                    if ((z2 + x2) < nextnweight) nextnweight = z2 + x2;
                } //wpf_sourcedest
            } //wpf_weight

            y += 2;
            if (x2 = wpf_weight[x][y])
            {
                //weight exists? if so square is passable
                if (y2 = wpf_sourcedest[x][y])
                {
                    if (y2 & z4)
                    {
                        if ((z2 + x2) < wpf_goalweight)
                        {
                            wpf_goalweight = z2 + x2;
                            wpf_goalx = x;
                            wpf_goaly = y;
                        }
                    }
                    //if same type ignore
                }
                else
                {
                    //wpf_sourcedest==NULL
                    //add new node
                    //	wpf_nayp1:
                    wpf_nodeaddflags |= z3;
                    if (wpf_laststackedn)
                    {
                        y2 = wpf_stackn[wpf_laststackedn];
                        wpf_nx[y2] = x;
                        wpf_ny[y2] = y;
                        wpf_nsource[y2] = z3;
                        wpf_nweight[y2] = z2 + x2;
                        wpf_laststackedn--;
                    }
                    else
                    {
                        wpf_lastusedn++;
                        wpf_nx[wpf_lastusedn] = x;
                        wpf_ny[wpf_lastusedn] = y;
                        wpf_nsource[wpf_lastusedn] = z3;
                        wpf_nweight[wpf_lastusedn] = z2 + x2;
                    }
                    wpf_sourcedest[x][y] = z3;
                    wpf_bestweight[x][y] = z2 + x2; //update array
                    if ((z2 + x2) < nextnweight) nextnweight = z2 + x2;
                } //wpf_sourcedest
            } //wpf_weight

            //remove current node (add to stack if necessary)
            if (z == wpf_lastusedn)
            {
                wpf_lastusedn--;
            }
            else
            {
                wpf_laststackedn++;
                wpf_stackn[wpf_laststackedn] = z;
                wpf_nweight[z] = 0;
            }
        }
        else
        {
            //z2==nweight
            //z2!=nweight
            if (z2 < nextnweight) nextnweight = z2;
            wpf_nodeaddflags |= wpf_nsource[z];
        } //z2==nweight
    } //z2=wpf_nweight[z]
    z++;
    if (z <= zlooplast) goto wpf_zloop;

    if (flags & 8) wpf_nodeaddflags |= 2; //override dest. node available flag
    if (((wpf_nodeaddflags & 1) == 0) || ((wpf_nodeaddflags & 2) == 0))
    {
        if (wpf_goalweight == 0x7FFFFFFF)
        {
            WPF_RETURN = WPF_NOPATHFOUND;
            return;
        }
        nextnweight = wpf_goalweight;
    }

    nweight = nextnweight;
    goto wpf_pathfindloop;

wpf_pathfound:

    /*
    //make a file dump of the array
    static file *tfh;
    static txt *t=txtnew();
    static txt *t2=txtnew();
    tfh=open2("wpf.txt",OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
    z2=maxdistance*2+2;
    for (y2=0;y2<=z2;y2++){
    txtset(t,"");
    for (x2=0;x2<=z2;x2++){
    txtnumint(t2,wpf_sourcedest[x2][y2]);
    txtadd(t,t2);
    t2->d2[0]=((wpf_bestweight[x2][y2]>>2)&15)+65;
    txtadd(t,t2);
    }
    txtfileout(t,tfh);
    }
    close(tfh);
    */

    WPF_RETURN = WPF_PATHFOUND;
    WPF_PATHLENGTH = 0;
    //x and y are where pathfind met
    x4 = x;
    y4 = y; //backup

    ild = 0;


    //allow for this square to be the source or dest!!!
    x3 = sourcex - WPF_OFFSETX;
    y3 = sourcey - WPF_OFFSETY;
    x2 = -1;
join2sourcepath_loop:
    if ((x != x3) || (y != y3))
    {
        //not the source
        ild++;
        if (ild >= 65536)
            MessageBox(NULL, "ild2 exceeded!", "Ultima 6 Online", MB_OK);


        x2++;
        //check surrounding squares for lowest source bestweight
        y2 = 0x7FFFFFFF;
        if (wpf_sourcedest[x][y + 1] & 1)
        {
            if (wpf_bestweight[x][y + 1] < y2)
            {
                join2sourcepath[x2] = PFup;
                y2 = wpf_bestweight[x][y + 1];
            }
        }
        if (wpf_sourcedest[x - 1][y] & 1)
        {
            if (wpf_bestweight[x - 1][y] < y2)
            {
                join2sourcepath[x2] = PFright;
                y2 = wpf_bestweight[x - 1][y];
            }
        }
        if (wpf_sourcedest[x][y - 1] & 1)
        {
            if (wpf_bestweight[x][y - 1] < y2)
            {
                join2sourcepath[x2] = PFdown;
                y2 = wpf_bestweight[x][y - 1];
            }
        }
        if (wpf_sourcedest[x + 1][y] & 1)
        {
            if (wpf_bestweight[x + 1][y] < y2)
            {
                join2sourcepath[x2] = PFleft;
                y2 = wpf_bestweight[x + 1][y];
            }
        }
        if (join2sourcepath[x2] == PFleft) x++;
        if (join2sourcepath[x2] == PFright) x--;
        if (join2sourcepath[x2] == PFup) y++;
        if (join2sourcepath[x2] == PFdown) y--;
        goto join2sourcepath_loop;
    } //not the source
    //copy join2sourcepath to *d
    for (x3 = x2; x3 >= 0; x3--)
    {
        d[WPF_PATHLENGTH] = join2sourcepath[x3];
        WPF_PATHLENGTH++;
    }

    x = x4;
    y = y4; //restore path connection co-ordinates
    ild = 0;
    x3 = destx - WPF_OFFSETX;
    y3 = desty - WPF_OFFSETY;
    x2 = -1;
join2destpath_loop:

    //if ((wpf_bestweight[x][y]!=1)||(wpf_sourcedest[x][y]&1)){//not the dest

    if ((abs(x - x3) > wpf_nextto) || (abs(y - y3) > wpf_nextto))
    {
        //not the dest

        ild++;
        if (ild >= 65536)
            MessageBox(NULL, "ild3 exceeded!", "Ultima 6 Online", MB_OK);


        x2++;
        //check surrounding squares for lowest dest bestweight
        y2 = 0x7FFFFFFF;

        if (wpf_sourcedest[x][y - 1] & 2)
        {
            if (wpf_bestweight[x][y - 1] < y2)
            {
                join2destpath[x2] = PFup;
                y2 = wpf_bestweight[x][y - 1];
            }
        }
        if (wpf_sourcedest[x + 1][y] & 2)
        {
            if (wpf_bestweight[x + 1][y] < y2)
            {
                join2destpath[x2] = PFright;
                y2 = wpf_bestweight[x + 1][y];
            }
        }

        if (wpf_sourcedest[x][y + 1] & 2)
        {
            if (wpf_bestweight[x][y + 1] < y2)
            {
                join2destpath[x2] = PFdown;
                y2 = wpf_bestweight[x][y + 1];
            }
        }
        if (wpf_sourcedest[x - 1][y] & 2)
        {
            if (wpf_bestweight[x - 1][y] < y2)
            {
                join2destpath[x2] = PFleft;
                y2 = wpf_bestweight[x - 1][y];
            }
        }
        if (join2destpath[x2] == PFleft) x--;
        if (join2destpath[x2] == PFright) x++;
        if (join2destpath[x2] == PFup) y--;
        if (join2destpath[x2] == PFdown) y++;
        goto join2destpath_loop;
    } //not the dest
    //copy join2destpath to *d
    for (x3 = 0; x3 <= x2; x3++)
    {
        d[WPF_PATHLENGTH] = join2destpath[x3];
        WPF_PATHLENGTH++;
    }

    return;
}

//returns a comparitive value re. danger of a square for a mover to walk on/over
unsigned char wpf_dangercheck(short x, short y, object* mover)
{
    static long z, z2, z3, z4; //temp use variables
    static object *obj, obj2;
    static npc* tnpc;
    z = 0;
    if (x >= 0)
    {
        if (y >= 0)
        {
            if (x < 2048)
            {
                if (y < 1024)
                {
                    //map boundries check
                    tnpc = (npc*)mover->more;
                    z4 = bt[y][x] & 1023;
                    if ((z4 >= 3) && (z4 <= 6))
                    {
                        z = 44; //swamp(no boots)
                        if (tnpc->boots) { if (tnpc->boots->type == 28) { z = 0; } } //swamp boots
                    }
                    if (z4 == 221) z = 36; //light lava
                    if (z4 == 222) z = 44; //medium lava
                    if (z4 == 223) z = 52; //heavy lava
                    if (obj = od[y][x])
                    {
                    wpfdc:
                        z4 = obj->type & 1023;
                        if (z4 == 172) z = 36; //spikes
                        if (z4 == 173)
                        {
                            //trap
                            if (obj->info & (1 << 9)) z = 44;
                        } //trap
                        if (z4 == 317) z = 36; //fire field
                        if (z4 == 318)
                        {
                            //poison field
                            if ((tnpc->flags & 1) == 0) z2 = 44; //only if not poisoned
                        } //318
                        if (z4 == 320) z = 36; //sleep field
                        if (z4 == 366) z = 28; //tendril
                        if (obj = (object*)obj->next) goto wpfdc;
                    } //obj
                }
            }
        }
    }
    return z;
} //wpf_dangercheck

bool OBJcheckbolt(unsigned short x, unsigned short y, unsigned short x2, unsigned short y2)
{
    OBJcheckbolt_x = x2;
    OBJcheckbolt_y = y2;
    Ocb_x = x;
    Ocb_y = y;
    Ocb_ix = abs(x2 - x);
    Ocb_iy = abs(y2 - y);
    Ocb_l = sqrt((float)(Ocb_ix * Ocb_ix + Ocb_iy * Ocb_iy)); //*lookup table
    Ocb_gx = ((float)x2 - x) / Ocb_l;
    Ocb_gy = ((float)y2 - y) / Ocb_l;
    Ocb_il = Ocb_l;
    for (Ocb_i = 0; Ocb_i < Ocb_il; Ocb_i++)
    {
        Ocb_x += Ocb_gx;
        Ocb_y += Ocb_gy;
        Ocb_ix = Ocb_x + 0.5f;
        Ocb_iy = Ocb_y + 0.5f;
        if ((bt[Ocb_iy][Ocb_ix] & 8192) == 0)
        {
            if ((Ocb_ix == x2) && (Ocb_iy == y2)) return FALSE;
            OBJcheckbolt_x = Ocb_ix;
            OBJcheckbolt_y = Ocb_iy;
            return TRUE;
        }
        if ((Ocb_ix == x2) && (Ocb_iy == y2)) return FALSE; //*remove later
        //Ocbo=OBJnew();
        //Ocbo->type=2;
        //OBJadd(Ocb_ix,Ocb_iy,Ocbo);
    } //i loop
    return FALSE;
}

unsigned long SFnew(unsigned short x, unsigned short y)
{
    if (sfi[y >> 3][x >> 3])
    {
        sfn++;
        sf[sfn].next = sfi[y >> 3][x >> 3];
        sfi[y >> 3][x >> 3] = sfn;
    }
    else
    {
        sfn++;
        sfi[y >> 3][x >> 3] = sfn;
        sf[sfn].next = 0;
    }
    return sfn;
}

unsigned long WTfind(object* obj)
{
    if (obj == NULL) return 0;
    WTf_i = 0;
    WTf_n = 0;
    WTf_w = 0;
    WTf_itemn = 0;
WTf_next:
    WTf_itemn++;
    WTf_w2 = obji[sprlnk[obj->type & 1023]].weight;
    if (obji[sprlnk[obj->type & 1023] + (obj->type >> 10)].flags & 4096)
    {
        WTf_w2 = obji[sprlnk[obj->type & 1023]].weight * obj->more2; //multiple objects
    }
    if (WTf_w2 == 0) return 0;
    WTf_w += WTf_w2;
    //1024 container (general)
    if (obji[sprlnk[obj->type & 1023]].flags & 1024)
    {
        if (obj->more)
        {
            OBJtl[WTf_n] = (object*)obj->more; //add container to todo list
            WTf_n++;
        }
    }
    if ((obj->next != NULL) && (WTf_itemn != 1))
    {
        obj = (object*)obj->next;
        goto WTf_next;
    }
    if (WTf_i < WTf_n)
    {
        obj = OBJtl[WTf_i];
        WTf_i++;
        goto WTf_next;
    }
    return WTf_w;
}

unsigned char houseowner(player* p, long housenumber)
{
    static long y;
    static npc* tnpc;
    houseowner_FAILVALUE = 1;
    if (housenumber <= 0) return 0; //range check
    houseowner_FAILVALUE = 2;
    if (housenumber >= 256) return 0;
    houseowner_FAILVALUE = 3;
    if ((housesav[housenumber].flags & 1) == 0) return 0; //unoccupied
    //*

    houseowner_FAILVALUE = 4;
    if (p->party[0] == NULL) return 0; //can't confirm character name
    houseowner_FAILVALUE = 5;


    tnpc = (npc*)p->party[0]->more;
    for (y = 0; y <= 31; y++)
    {
        if (y < p->name->l)
        {
            if (p->name->d2[y] != housesav[housenumber].username[y]) return 0;
        }
        else
        {
            if (housesav[housenumber].username[y]) return 0;
        }
        if (y < tnpc->name->l)
        {
            if (tnpc->name->d2[y] != housesav[housenumber].charactername[y]) return 0;
        }
        else
        {
            if (housesav[housenumber].charactername[y]) return 0;
        }
    } //y
    return 1;
} //houseowner

unsigned char housecheck(unsigned short x, unsigned short y)
{
    static short x6, y6;
    for (x6 = 1; x6 <= 255; x6++)
    {
        //scan all houses
        //check if x and y are inside the house.
        if (x >= housex1[x6])
        {
            if (x <= housex2[x6])
            {
                if (y >= housey1[x6])
                {
                    if (y <= housey2[x6])
                    {
                        for (y6 = 0; y6 < housepnext[x6]; y6++)
                        {
                            if (x == housepx[x6][y6])
                            {
                                if (y == housepy[x6][y6])
                                {
                                    return (x6); //return house number.
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return (0);
}

//save housesav buffer to disk (with current date)
void housesav_update()
{
    static txt *t = txtnew(), *t2 = txtnew();
    static file* tfh;
    txtset(t2, "??");
    t2->ds[0] = 12345; //Not Crypted
    txtNEWLEN(t, -17408);
    memcpy(&t->d2[0], &housesav, 17408);
    /*	luteijn: no encryption!
    *
    *      encrypt(t);
    */
    txtadd(t2, t);
    tfh = open2(".\\save\\house.sav", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
    put(tfh, &t2->d2[0], t2->l);
    close(tfh);
}

long CON_gv(long v)
{
    if (v == (65536 + 0)) return tplayer->NPCflags[CONnpc] & 255; //FLG_JMP
    if (v == (65536 + 1)) return CONerr; //ERROR
    if (v == (65536 + 2)) return tnpc->schedule_last;
    if (v == (65536 + 3)) return (tplayer->NPCflags[CONnpc] & 256) >> 8; //FLG_NAME
    if (v == (65536 + 4)) return CONrnd; //RANDOM
    if (v == (65536 + 5)) return CONnumber; //NUMBER
    if (v == (65536 + 7)) return CONpartymember; //PARTYMEMBER
    if (v == (65536 + 8)) return tplayer->male_female; //MALEFEMALE
    if (v == (65536 + 9)) return (tplayer->NPCflags[CONnpc] & 512) >> 9; //FLG_INTRO
    if (v == (65536 + 10)) return (tplayer->NPCflags[CONnpc] & 1024) >> 10; //FLG_INTRO2
    if (v == (65536 + 11)) return (tplayer->NPCflags[CONnpc] & 0xFF0000) >> 16; //FLG_VALUE
    if (v == (65536 + 12)) return (tplayer->NPCflags[CONnpc] & 0xFF000000) >> 24; //FLG_VALUE2
    if (v == (65536 + 13)) return CONport; //PORT
    if (v == (65536 + 14)) return (tplayer->NPCflags[CONnpc] & 2048) >> 11; //FLG_KARMA
    if (v == (65536 + 15)) return tplayer->karma; //KARMA

    if (v == (65536 + 16))
    {
        if (CONpartymember)
        {
            tnpc3 = NULL;
            if (tplayer->party[CONpartymember - 1])
            {
                tnpc3 = (npc*)tplayer->party[CONpartymember - 1]->more;
            }
            else
            {
                if (tplayer->party[0]) tnpc3 = (npc*)tplayer->party[0]->more;
            }
            if (tnpc3) return tnpc3->port;
        } //CONpartymember
        return 187; //blank!
    }

    if (v == (65536 + 17))
    {
        //PARTYMEMBER_LEVEL
        if (CONpartymember)
        {
            if (tplayer->party[CONpartymember - 1])
            {
                tnpc3 = (npc*)tplayer->party[CONpartymember - 1]->more;
                return tnpc3->lev;
            }
        }
        return 0;
    }

    if (v == (65536 + 18))
    {
        //RESURRECT_COST (returns value or 50 if it isn't sure...)
        static long x6, x5, y6, i3;
        for (x6 = 0; x6 <= 7; x6++)
        {
            if (tplayer->party[x6])
            {
                tnpc3 = (npc*)tplayer->party[x6]->more;
                x5 = OBJlist((object*)tnpc3->items->more); //x5=last object number
            }
        } //!=NULL,x6
        OBJlist_last = NULL;
        for (y6 = 0; y6 < x5; y6++)
        {
            if ((OBJlist_list[y6]->type & 1023) == 339)
            {
                //dead body
                //scan resurrect list
                for (i3 = 0; i3 <= nresu; i3++)
                {
                    if (resu[i3])
                    {
                        if (resu_body[i3] == OBJlist_list[y6])
                        {
                            if (resu_player[i3])
                            {
                                //has a player pointer
                                //resurrection possible
                                tnpc3 = (npc*)resu[i3]->more;
                                if (resu_partymember[i3] == 0)
                                {
                                    if (resu_player[i3]->GNPCflags[25])
                                    {
                                        return 0;
                                    } //25s
                                } //resu_partymember==0
                                return tnpc3->lev * 50;
                            }
                        }
                    }
                }
            }
        }
        return 50;
    }

    if (v == (65536 + 19)) return CONhousecost; //HOUSECOST

    if (v == (65536 + 20))
    {
        //HOUSEBALANCE (returns an error if no house is owned)
        if (houseowner(tplayer, tplayer->GNPCflags[28]) == 0)
        {
            CONerr = 1;
            return 0;
        }
        return housesav[tplayer->GNPCflags[28]].gold;
    }

    if (v == (65536 + 21)) return CONhouseinitialcost; //HOUSEINITIALCOST

#pragma region
    if (v == 65536 + 22) // BULKPRICE
        return tplayer->bulkprice;
#pragma endregion Bulk sell patch, Xenkan 2010-12-04

    if ((v & 0xFFFFFF00) == 262144) return tplayer->GNPCflags[v & 0xFF];
    if ((v & 0xFFFFFF00) == 131072) return CONreg[v & 0xFF];

    return v;
}

void CON_sv(long v, long v2)
{
    v2 = CON_gv(v2);
    if (v == (65536 + 0))
    {
        tplayer->NPCflags[CONnpc] &= 0xFFFFFF00;
        tplayer->NPCflags[CONnpc] |= v2;
    } //FLG_JMP
    if (v == (65536 + 1)) CONerr = v2; //ERROR
    if (v == (65536 + 3))
    {
        tplayer->NPCflags[CONnpc] &= (0xFFFFFFFF - 256);
        tplayer->NPCflags[CONnpc] |= (v2 * 256);
    } //FLG_NAME
    if (v == (65536 + 4))
    {
        v2 = rnd * (v2 + 1);
        CONrnd = v2;
    } //RANDOM
    if (v == (65536 + 5)) CONnumber = v2; //NUMBER
    if (v == (65536 + 6)) CONqual = v2; //CON_QUAL WRITEONLY!
    if (v == (65536 + 7))
    {
        CONpartymember = v2; //PARTYMEMBER
        //validate PARTYMEMBER (if invalid set to 0)
        if ((CONpartymember) && (CONpartymember <= 8))
        {
            if (!tplayer->party[CONpartymember - 1]) CONpartymember = 0;
        } //CONpartymember
    } //PARTYMEMBER
    if (v == (65536 + 9))
    {
        tplayer->NPCflags[CONnpc] &= (0xFFFFFFFF - 512);
        tplayer->NPCflags[CONnpc] |= (v2 * 512);
    } //FLG_INTRO
    if (v == (65536 + 10))
    {
        tplayer->NPCflags[CONnpc] &= (0xFFFFFFFF - 1024);
        tplayer->NPCflags[CONnpc] |= (v2 * 1024);
    } //FLG_INTRO2
    if (v == (65536 + 11))
    {
        tplayer->NPCflags[CONnpc] &= 0xFF00FFFF;
        tplayer->NPCflags[CONnpc] |= (v2 << 16);
    } //FLG_VALUE
    if (v == (65536 + 12))
    {
        tplayer->NPCflags[CONnpc] &= 0x00FFFFFF;
        tplayer->NPCflags[CONnpc] |= (v2 << 24);
    } //FLG_VALUE2
    if (v == (65536 + 13)) CONport = v2; //PORT
    if (v == (65536 + 14))
    {
        tplayer->NPCflags[CONnpc] &= (0xFFFFFFFF - 2048);
        tplayer->NPCflags[CONnpc] |= (v2 * 2048);
    } //FLG_KARMA
    if (v == (65536 + 15))
    {
        if ((tplayer->NPCflags[CONnpc] & 2048) == 0)
        {
            tplayer->NPCflags[CONnpc] |= 2048;
            if (v2 < tplayer->karma)
            {
                if (stealing_txt == NULL) stealing_txt = txtnew();
                txtset(stealing_txt, "?");
                stealing_txt->d2[0] = 8;
                txtadd(stealing_txt, "You feel less virtuous than before!");
                NET_send(NETplayer, tplayer->net, stealing_txt);
            } //v2<
            tplayer->karma = v2; //KARMA
        } //FLG_KARMA==0
    }

    if (v == (65536 + 19))
    {
        //HOUSECOST
        if ((v2 >= 1) && (v2 <= 255))
        {
            if ((housesav[v2].flags & 1) == 0)
            {
                CONhousecost = housecost[v2];
                CONhouseinitialcost = houseinitialcost[v2];
                return;
            }
        }
        CONhousecost = 0; //house not available
        CONhouseinitialcost = 0;
    }

    if (v == (65536 + 20))
    {
        //HOUSEBALANCE (returns an error if no house is owned)
        if (houseowner(tplayer, tplayer->GNPCflags[28]) == 0)
        {
            CONerr = 1;
            return;
        }
        housesav[tplayer->GNPCflags[28]].gold = v2;
        housesav_update();
    }

    if ((v & 0xFFFFFF00) == 131072) CONreg[v & 0xFF] = v2;
    if ((v & 0xFFFFFF00) == 262144) tplayer->GNPCflags[v & 0xFF] = v2;
}

void stealing(player* tplayer, object* obj)
{
    static long x8, x9, s, x5, y6;
    static object* obj2;
    if (obj == NULL) return;
    s = 0;
    x5 = OBJlist(obj); //x5=last object number
    OBJlist_last = NULL;
    for (y6 = 0; y6 < x5; y6++)
    {
        obj2 = OBJlist_list[y6];
        if (obj2->info & 128)
        {
            //stealing!
            x8 = 1;
            for (x9 = 0; x9 <= 1023; x9++) { if (tplayer->sitem[x9] == obj2) x8 = 0; }
            if (x8)
            {
                tplayer->sitem[tplayer->sitem_next++ & 1023] = obj2;
                if (obj2->type == 88)
                {
                    tplayer->karma -= ((float)obj2->more2 / 16.0f);
                    goto karma_spec;
                } //gold
                if (obj2->type == 55)
                {
                    tplayer->karma -= ((float)obj2->more2 / 16.0f);
                    goto karma_spec;
                } //arrows
                if (obj2->type == 56)
                {
                    tplayer->karma -= ((float)obj2->more2 / 16.0f);
                    goto karma_spec;
                } //bolts
                if (obji[sprlnk[obj2->type & 1023] + (obj2->type >> 10)].flags & 4096)
                {
                    tplayer->karma -= ((float)obj2->more2 / 4.0f);
                    goto karma_spec;
                } //multiple
                tplayer->karma -= 2;
            karma_spec:
                s = 1;
            } //x8
        } //&128
    } //y6
    if ((s) && (stealing_MESSAGE == TRUE))
    {
        if (stealing_txt == NULL) stealing_txt = txtnew();
        txtset(stealing_txt, "?");
        stealing_txt->d2[0] = 8;
        txtadd(stealing_txt, "You feel less virtuous than before!");
        NET_send(NETplayer, tplayer->net, stealing_txt);
    } //s
}

unsigned char stormcloakcheck(unsigned short x, unsigned short y)
{
    static short i, x2, y2;
    for (i = 0; i <= stormcloak_last; i++)
    {
        x2 = abs(x - stormcloak_x[i]);
        y2 = abs(y - stormcloak_y[i]);
        if (x2 <= 7)
        {
            if (y2 <= 7)
            {
                if (stormcloak_mask[y2][x2]) return (1);
            }
        } //x2,y2<=7
    } //i
    return (0);
}

unsigned char stormcloakcheck2(unsigned short x, unsigned short y, player* p)
{
    static short i, x2, y2;
    for (i = 0; i <= stormcloak_last; i++)
    {
        if (stormcloak_player[i] == p)
        {
            //field created by current player
            x2 = abs(x - stormcloak_x[i]);
            y2 = abs(y - stormcloak_y[i]);
            if (x2 <= 7)
            {
                if (y2 <= 7)
                {
                    if (stormcloak_mask[y2][x2]) return (1);
                }
            } //x2,y2<=7
        } //p
    } //i
    return (0);
}

void inbritanniacheck()
{
    if (!inbritannia) inbritannia = txtnew();
    static txt *t = txtnew(), *t2 = txtnew();
    static long x, x2, x3;
    static npc* tnpc;
    static file* tfh;

    txtset(t, "");
    inbritannia_totalplayers = 0;
    x2 = 0;
    for (x = 0; x <= playerlist_last; x++)
    {
        //create and send UPDATE message
        if (playerlist[x])
        {
            //current player
            if (playerlist[x]->net)
            {
                //net connection available


                if (playerlist[x]->party[0])
                {
                    tnpc = (npc*)playerlist[x]->party[0]->more;
                    if (x2) txtadd(t, ", ");
                    txtadd(t, tnpc->name);
                    txtadd(t, " (");
                    txtnumint(t2, tnpc->lev);
                    txtadd(t, t2);
                    txtadd(t, ")");
                    x2 = 1;
                    inbritannia_totalplayers++;
                }
                else
                {
                    for (x3 = 0; x3 <= nresu; x3++)
                    {
                        if (resu[x3])
                        {
                            if (resu_player[x3] == playerlist[x])
                            {
                                if (resu_partymember[x3] == 0)
                                {
                                    tnpc = (npc*)resu[x3]->more;
                                    if (x2) txtadd(t, ", ");
                                    txtadd(t, tnpc->name);
                                    txtadd(t, " (");
                                    txtnumint(t2, tnpc->lev);
                                    txtadd(t, t2);
                                    txtadd(t, ", dead)");
                                    x2 = 1;
                                    inbritannia_totalplayers++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for (x = 0; x < t->l; x++)
    {
        if (t->d2[x] == 34) t->d2[x] = 39;
    }

    txtnumint(inbritannia, inbritannia_totalplayers);
    txtadd(inbritannia, " in Britannia: ");
    txtadd(inbritannia, t);

    txtset(t2, "c:\\public_html\\who.jss");
    tfh = open2(t2, OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
    if (tfh->h != HFILE_ERROR)
    {
        txtset(t2, "document.write(\x022");
        txtadd(t2, inbritannia);
        txtadd(t2, "\x022);");
        txtfileout(t2, tfh);
        close(tfh);
    } //!=HFILE_ERROR
} //inbritanniacheck

unsigned char ENHANCEget_attack(object* obj)
{
    static long x2;
    //EXCEPTIONS!
    x2 = obj->type & 1023;
    if (obji[sprlnk[x2]].flags & (1 + 2 + 4 + 8 + 16 + 32 + 64))
    {
        //can be worn
        if (obji[sprlnk[x2]].flags & 4096) return 0; //multiple
        if (x2 == 78) return 0; //staff
        if (x2 == 79) return 0; //lightning
        if (x2 == 80) return 0; //fire
        if (x2 == 48) return 0; //glass sword
        if (x2 == 256) return 0; //protection ring
        if (x2 == 141) return 0; //decorative sword
        if (obji[sprlnk[x2]].v1)
        {
            //weapon
            return obj->more2 & 15;
        }
    }
    return 0;
}

unsigned char ENHANCEget_defense(object* obj)
{
    static long x2;
    //EXCEPTIONS!
    x2 = obj->type & 1023;
    if (obji[sprlnk[x2]].flags & (1 + 2 + 4 + 8 + 16 + 32 + 64))
    {
        //can be worn
        if (obji[sprlnk[x2]].flags & 4096) return 0; //multiple
        if (x2 == 78) return 0; //staff
        if (x2 == 79) return 0; //lightning
        if (x2 == 80) return 0; //fire
        if (x2 == 48) return 0; //glass sword
        if (x2 == 256) return 0; //protection ring
        if (x2 == 141) return 0; //decorative sword
        if (obji[sprlnk[x2]].v2)
        {
            //armour
            return (obj->more2 & 0xF0) >> 4;
        }
    }
    return 0;
}


void OBJsave(unsigned short x, unsigned short y)
{
    static object *obj, *obj2, *obj3, *obj4;
    static long x2, x3, x4, x5, x8, y2, y8;
    //check if index already exists
    for (x2 = 0; x2 <= objsave_last; x2++)
    {
        if (objsave_obj[x2])
        {
            //valid
            if ((objsave_x[x2] == x) && (objsave_y[x2] == y)) return; //index already saved
        } //valid
    } //x2
    //add index
    for (x2 = 0; x2 <= objsave_last; x2++)
    {
        if (objsave_obj[x2] == NULL) goto objsave_freeindex;
    }
    if (objsave_last == (65536 * 4)) return;
    objsave_last++;
    x2 = objsave_last;
objsave_freeindex:
    objsave_x[x2] = x;
    objsave_y[x2] = y;
    objsave_obj[x2] = NULL;

    objsave_wait[x2] = 8192.0f + rnd * 8192.0f; //default values ~2 to ~4h
    //objsave_wait[x2]=10.0f; //debug
    if ((237 == x) && (156 == y)) objsave_wait[x2] = 256.0f + rnd * 256.0f; //default values

    //find something worth backing up!
    //ignore skiffs, dead bodies, and some other wierd stuff (how?)

    objsave_node_last = -1;
    obj4 = NULL;
    if (obj = od[y][x])
    {
    objsave_nextnode:

        //MessageBox(NULL,"node","Ultima 6 Online",MB_OK);

        obj3 = NULL;
    objsave_next:
        //add it?
        if ((obj->info & 112) == 0)
        {
            //permanent object
            if ((obj->info & 16384) && !(obji[sprlnk[obj->type & 1023]].flags & 1024))
            {
                //treasurechest object and not chest
                x4 = randchestitem();
                x5 = x4 >> 10;
                x4 = x4 & 1023;
                obj->type = x4;
                obj->more2 = x5;
                ENHANCEnewn(obj, 4, 4);
            }
            if ((obji[sprlnk[obj->type & 1023]].weight) || (obji[sprlnk[obj->type & 1023]].flags & 1024) || ((obj->type
                & 1023) == OBJ_WEB) || ((obj->type & 1023) >= OBJ_FIRE_FIELD && (obj->type & 1023) <= OBJ_SLEEP_FIELD))
            {
                //weight or container, spider web or field
                //floating object with weight SHOULD BE ADDED, dont ignore floating
                //EXCEPTIONS!
                if ((obj->type & 1023) == 414) goto skipsaveobj; //skiff
                if (obj->info & 256) goto skipsaveobj; //quest item
                if (obj == nuggetsfix) goto skipsaveobj; //newbie nuggets cannot respawn
                //add save obj

                //*** CONTAINER ERROR! ***
                obj2 = OBJnew();
                obj2->type = obj->type;
                obj2->info = obj->info;
                obj2->prev = obj3;
                if (obj3) obj3->next = obj2; //link
                if (obj2->more2 = obj->more2)
                {
                    //!=NULL
                    if (obji[sprlnk[obj->type & 1023]].flags & 1024)
                    {
                        //container
                        objsave_node_last++;
                        objsave_node[objsave_node_last] = obj2;
                    }
                }
                ENHANCEnewn(obj2, 8, 4);

                if (obj4)
                {
                    obj4->more = (object*)obj2;
                    obj2->prev = obj4;
                }

                if ((obj3 == NULL) && (obj4 == NULL)) objsave_obj[x2] = obj2;
                obj3 = obj2; //set prev

                obj->info |= 112; //set temp obj
                obj4 = NULL;
                //NEWCODE
                if ((obj2->info & 16384) && ((obj2->type & 1023) == OBJ_CHEST))
                {
                    //random new location for treasure chest
                    objsave_wait[x2] = 1.0f; //respawn new chest immediately
                    x8 = randomchestlocation(false);
                    y8 = x8 >> 10;
                    x8 = x8 & 1023;
                    x3 = 2 + rnd * 2;
                    objsave_x[x2] = x8;
                    objsave_y[x2] = y8; //set new random location
                    //objsave_x[x2]=349; objsave_y[x2]=349; //debug location
                    obj2->type = OBJ_CHEST + 1024 * x3;
                }
                //NEWCODEEND
            } //weight or container
        } //permanent object
    skipsaveobj:
        if (obj = (object*)obj->next) goto objsave_next;
        if (objsave_node_last != -1)
        {
            obj4 = objsave_node[objsave_node_last];
            obj = (object*)obj4->more;
            objsave_node_last--;
            goto objsave_nextnode;
        }
    } //obj (first)
} //OBJsave

//calculate mp_max of hired partymember based on INT
unsigned short get_mp_max(unsigned short i)
{
    switch (i)
    {
    case 0:
        return 0;
    case 1:
    case 2:
        return 1;
    case 3:
    case 4:
        return 2;
    case 5:
    case 6:
        return 3;
    case 7:
    case 8:
        return 4;
    case 9:
    case 10:
        return 5;
    case 11:
    case 12:
        return 6;
    case 13:
    case 14:
        return 7;
    case 15:
    case 16:
        return 8;
    case 17:
        return 9;
    case 18:
        return 10;
    case 19:
        return 11;
    case 20:
        return 12;
    case 21:
        return 13;
    case 22:
        return 14;
    case 23:
        return 15;
    case 24:
        return 16;
    case 25:
        return 18;
    case 26:
        return 20;
    case 27:
        return 22;
    case 28:
        return 24;
    case 29:
        return 26;
    case 30:
        return 28;
    case 31:
        return 30;
    default:
        return i;
    }
}

unsigned char objvisible(player* p, object* myobj)
{
    static long i2;
    static object* myobj2;
    i2 = myobj->type & 1023; //get object identity
    if (myobj->info & 256)
    {
        //quest
        if (i2 == 73)
        {
            //moonstone
            if (p->GNPCflags[4] & (1 << (myobj->type >> 10))) return 0;
        }
        if (myobj->type == (275 + 2 * 1024))
        {
            //yellow potion
            if (p->GNPCflags[32] & myobj->more2) return 0;
        } //yellow potion
        if ((i2 >= 242) && (i2 <= 249))
        {
            //rune
            if (p->GNPCflags[5] & (1 << (i2 - 242))) return 0;
        }
        if ((i2 >= 400) && (i2 <= 407))
        {
            //map(0-7)
            if (p->GNPCflags[7] & (1 << (i2 - 400))) return 0;
        }
        if (i2 == 62)
        {
            //vortex cube
            if ((myobj->x == 923) && (myobj->y == 850))
            {
                if ((p->GNPCflags[18] & 1) == 0) return 0;
            }
            if ((myobj->x == 1939) && (myobj->y == 57))
            {
                if (p->GNPCflags[6] & (1 << 1)) return 0;
            }
        }
        if (i2 == 389)
        {
            //silver tablet
            if (p->GNPCflags[6] & (1 << 2)) return 0;
        }
        if (i2 == 390)
        {
            //silver tabletII
            if (p->GNPCflags[6] & (1 << 3)) return 0;
        }
        if (i2 == 270)
        {
            //balloon plans
            if (p->GNPCflags[6] & (1 << 4)) return 0;
        }
        if (i2 == 395)
        {
            //broken gargoyle lens
            if (p->GNPCflags[6] & (1 << 5)) return 0;
        }
        if (i2 == 394)
        {
            //brit lens
            if ((p->GNPCflags[18] & 2) == 0) return 0;
        }
        if (i2 == 396)
        {
            //garg lens
            if ((p->GNPCflags[18] & 4) == 0) return 0;
        }
    } //quest
    if (myobj == nuggetsfix)
    {
        //nuggets
        if (p->online < 1536) return 0;
        if (p->GNPCflags[6] & (1 << 6)) return 0;
    }
    if (i2 == 59)
    {
        //codex
        if (p->GNPCflags[18] & 8) return 0;
    }
    if (i2 == 51)
    {
        //force field
        myobj2 = (object*)myobj->prev;
        if (p->GNPCflags[3] & (1 << (myobj2->type >> 10))) return 0;
    } //force field
    if (i2 == 173)
    {
        //trap
        if ((myobj->info & (1 << 9)) == 0) return 0;
    } //trap
    return 1;
} //objvisible

long roundfloat(float f)
{
    __asm {
        fld f
        fistp roundfloat_l
        }
    return roundfloat_l;
}

void getwindspell(long sx, long sy, long dx, long dy)
{
    static long windspell_i[128];
    static long x, x2, x3, x4;
    static long y, y2, y3, y4;
    static long z, z2, z3, z4;
    static float fx, fx2, fx3, fx4, fy, fy2, fy3, fy4, f, f2, f3, f4;
    WINDSPELL_boltn = -1;
    WINDSPELL_n = -1;
    z = abs(dx - sx);
    z2 = abs(dy - sy);
    if (z > z2) z3 = z;
    else z3 = z2;
    f = z3;
    if (z3 == 0) return;
    if (z3 == 1)
    {
        //3 point fan
        if (z && (z2 == 0))
        {
            //3 vertical
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy - 1;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy + 1;
        }
        if (z2 && (z == 0))
        {
            //3 horizontal
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx - 1;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx + 1;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
        }
        if (z2 && z)
        {
            //3 diagonally
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = sx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = sy;
        }
    }
    else
    {
        //5 point fan
        x3 = abs(dx - sx);
        y3 = abs(dy - sy);
        x = 0;
        y = 0;
        if (x3 >= y3) x = 1;
        if (y3 >= x3) y = 1;
        if (y3 >= (x3 / 2))
        {
            if (x3 >= (y3 / 2))
            {
                x = 1;
                y = 1;
            }
        }
        if (x && (y == 0))
        {
            //5 vertical
            z4 = 1;
            if (dx > sx) z4 = -1;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx + z4;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy - 2;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy - 1;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy + 1;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx + z4;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy + 2;
        }
        if (y && (x == 0))
        {
            //5 horizontal
            z4 = 1;
            if (dy > sy) z4 = -1;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx - 2;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy + z4;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx - 1;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx + 1;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx + 2;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy + z4;
        }
        if (x && y)
        {
            //5 diagonally
            //...
            x4 = -1;
            if (dx > sx) x4 = 1;
            y4 = -1;
            if (dy > sy) y4 = 1;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx + x4;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy - y4;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx - x4;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy + y4;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx + x4;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy - y4 * 2;
            WINDSPELL_boltn++;
            WINDSPELL_boltx[WINDSPELL_boltn] = dx - x4 * 2;
            WINDSPELL_bolty[WINDSPELL_boltn] = dy + y4;
        }
    } //3 or 5 point fan

    //remove out-of-map wind lines
    z3 = 0;
    z2 = WINDSPELL_boltn;
    for (z = 0; z <= z2; z++)
    {
        x = WINDSPELL_boltx[z];
        y = WINDSPELL_bolty[z];
        if ((x >= 0) && (x <= 2047) && (y >= 0) && (y <= 1023))
        {
            WINDSPELL_boltx[z3] = x;
            WINDSPELL_bolty[z3] = y;
            z3++;
        }
        else
        {
            WINDSPELL_boltn--;
        }
    } //z
    if (WINDSPELL_boltn == -1) return;

    //get xy locations
    f *= 8.0f;
    for (z = 0; z <= WINDSPELL_boltn; z++)
    {
        fx = sx;
        fy = sy;
        fx2 = (float(WINDSPELL_boltx[z] - sx)) / f;
        fy2 = (float(WINDSPELL_bolty[z] - sy)) / f;
        x2 = sx;
        y2 = sy;
    windspelllineloop:
        fx += fx2;
        fy += fy2;
        x = roundfloat(fx);
        y = roundfloat(fy);
        if ((x != x2) || (y != y2))
        {
            fx3 = fabs(fx - x);
            fy3 = fabs(fy - y);
            if ((fx3 <= 0.4375) || (fy3 <= 0.4375))
            {
                //non-corner check
                WINDSPELL_n++;
                WINDSPELL_x[WINDSPELL_n] = x;
                WINDSPELL_y[WINDSPELL_n] = y;
                windspell_i[WINDSPELL_n] = 0;
                if ((x == WINDSPELL_boltx[z]) && (y == WINDSPELL_bolty[z])) goto windspelllinedone;
                if ((bt[y][x] & 8192) == 0)
                {
                    //not bolt passable
                    WINDSPELL_boltx[z] = x;
                    WINDSPELL_bolty[z] = y;
                    goto windspelllinedone;
                }
                x2 = x;
                y2 = y;
            } //non-corner check
        }
        goto windspelllineloop;
    windspelllinedone:;
    } //z
    //enumerate each xy location
    z2 = 0;
windspell_z2loop:
    if (windspell_i[z2] == 0)
    {
        x = WINDSPELL_x[z2];
        y = WINDSPELL_y[z2];
        windspell_i[z2] = 1;
        z3 = 2;
        for (z = z2 + 1; z <= WINDSPELL_n; z++)
        {
            if ((WINDSPELL_x[z] == x) && (WINDSPELL_y[z] == y))
            {
                windspell_i[z] = z3;
                z3++;
            } //x&&y
        } //z
    } //z2
    z2++;
    if (z2 <= WINDSPELL_n) goto windspell_z2loop;
    //eliminate all xy locations which aren't the 1st or 3rd
    for (z = 0; z <= WINDSPELL_n; z++)
    {
        if ((windspell_i[z] != 1) && (windspell_i[z] != 3)) WINDSPELL_x[z] = 0;
    } //z
} //getwindspell

DWORD WINAPI revive_infiniteloopexit(LPVOID null_value)
{
revive_infiniteloopexit_loop:
    SleepEx(1000, FALSE);
    if (revive_infiniteloopexit_i == revive_infiniteloopexit_i2) revive_infiniteloopexit_i3++;
    else revive_infiniteloopexit_i3 = 0;
    if (revive_infiniteloopexit_i3 >= 128)
    {
        u6orevive = 254;
        seek(u6orevive_fh, 0);
        put(u6orevive_fh, &u6orevive, 1);
        //put(u6orevive_fh,&DEBUGlastline,4);
        put(u6orevive_fh, &u6opl, 4); //ALWAYS 0 (left for compatibiliy with previous versions of revive)
        exit(0);
    }
    revive_infiniteloopexit_i2 = revive_infiniteloopexit_i;
    goto revive_infiniteloopexit_loop;
    return 0;
}

//returns player->party[0] or resu[...] if dead
object* getprimarypartymemberobj(player* sourceplayer)
{
    if (sourceplayer->party[0]) return sourceplayer->party[0];
    static unsigned long i;
    for (i = 0; i <= nresu; i++)
    {
        if (resu[i])
        {
            if (resu_player[i] == sourceplayer)
            {
                if (resu_partymember[i] == 0)
                {
                    return resu[i];
                }
            }
        }
    }
    return NULL;
}

//see getprimarypartymemberobj
object* getpartymemberobj(player* sourceplayer, unsigned char n)
{
    if (sourceplayer->party[n]) return sourceplayer->party[n];
    static unsigned long i;
    for (i = 0; i <= nresu; i++)
    {
        if (resu[i])
        {
            if (resu_player[i] == sourceplayer)
            {
                if (resu_partymember[i] == n)
                {
                    return resu[i];
                }
            }
        }
    }
    return NULL;
}

//used for crts with monster-drop inventories to determine the extra armour they could be using
long getequiparmourvalue(object* obj)
{
    static long armourmask, armourvalue;
    if (!obj) return 0;
    armourvalue = 0;
    armourmask = 0; //used stop usaage of 2 or more similar armour types (eg. 2 helms)
getequiparmourvalue_next:
    if (obji[sprlnk[obj->type & 1023]].flags & (1 + 2 + 4 + 8 + 16 + 32 + 64))
    {
        //equipable
        if ((obji[sprlnk[obj->type & 1023]].flags & (1 + 2 + 4 + 8 + 16 + 32 + 64)) & armourmask)
            goto
                getequiparmourvalue_alreadyusing;
        if (obji[sprlnk[obj->type & 1023]].v2)
        {
            armourvalue += (obji[sprlnk[obj->type & 1023]].v2 + ENHANCEget_defense(obj));
            armourmask |= (obji[sprlnk[obj->type & 1023]].flags & (1 + 2 + 4 + 8 + 16 + 32 + 64));
        } //v2
    } //equippable flags
getequiparmourvalue_alreadyusing:
    if (obj = (object*)obj->next) goto getequiparmourvalue_next;
    return armourvalue;
}


long getequiph2hwepdamage(object* obj)
{
    static long weapondamage;
    if (!obj) return 0;
    weapondamage = 0;
getequiph2hwepdamage_next:

    /*
    if (obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64)){//equipable
    if ((obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64))&armourmask) goto getequiparmourvalue_alreadyusing;
    if (obji[sprlnk[obj->type&1023]].v2){
    armourvalue+=(obji[sprlnk[obj->type&1023]].v2+ENHANCEget_defense(obj));
    armourmask|=(obji[sprlnk[obj->type&1023]].flags&(1+2+4+8+16+32+64));
    }//v2
  
    }//equippable flags
    getequiparmourvalue_alreadyusing:
    */

    if (obji[sprlnk[obj->type & 1023]].flags & (1 + 2 + 4 + 8 + 16 + 32 + 64))
    {
        //equipable?
        if (obji[sprlnk[obj->type & 1023]].v1)
        {
            //can do damage
            if ((obji[sprlnk[obj->type & 1023]].v1 + ENHANCEget_attack(obj)) > weapondamage)
            {
                //greater than currently known weapon?

                //eliminate certain types
                if ((obj->type & 1023) == 33) goto getequiph2hwepdamage_skip; //sling
                if ((obj->type & 1023) == 41) goto getequiph2hwepdamage_skip; //bow
                if ((obj->type & 1023) == 42) goto getequiph2hwepdamage_skip; //crossbow
                if ((obj->type & 1023) == 49) goto getequiph2hwepdamage_skip; //boomerang
                if ((obj->type & 1023) == 50) goto getequiph2hwepdamage_skip; //triple crossbow!
                if ((obj->type & 1023) == 49) goto getequiph2hwepdamage_skip; //boomerang
                if ((obj->type & 1023) == 54) goto getequiph2hwepdamage_skip; //magic bow
                weapondamage = (obji[sprlnk[obj->type & 1023]].v1 + ENHANCEget_attack(obj));
            }
        }
    }
getequiph2hwepdamage_skip:
    if (obj = (object*)obj->next) goto getequiph2hwepdamage_next;
    return weapondamage;
}

//the purpose of this function is to:
//1. spread out party after using a boat, balloon, ladder, etc.
//2. fix certain piling up bugs
//3. remove any prerecorded paths
//all alive partymembers are assumed to be in a removed but existant state
void partyadd(player* p, long x, long y)
{
    static long x2, y2, x3, y3, z, z2, i;
    static object* obj;
    static npc* tnpc;
    ZeroMemory(&partyadd_checkarray, sizeof(partyadd_checkarray));
    //prepare array of checkable places
    for (y2 = -2; y2 <= 2; y2++)
    {
        for (x2 = -2; x2 <= 2; x2++)
        {
            x3 = x + x2;
            y3 = y + y2;
            if ((x3 >= 0) && (x3 <= 2047) && (y3 >= 0) && (y3 <= 1023))
            {
                if (bt[y3][x3] & 1024)
                {
                    //walkable location
                    partyadd_checkarray[x2 + 3][y2 + 3] = 1;
                }
            } //boundaries check
        }
    }
    partyadd_checkarray[1][1] = 0;
    partyadd_checkarray[5][1] = 0;
    partyadd_checkarray[5][5] = 0;
    partyadd_checkarray[1][5] = 0;
    z2 = 0;
    for (i = 0; i <= 7; i++)
    {
        if (obj = p->party[i])
        {
            tnpc = (npc*)obj->more;
            if (obj->x || obj->y)
            {
                //remove if not removed!
                OBJremove(obj);
                if (tnpc->horse) OBJremove((object*)tnpc->more);
            }

            if (tnpc->horse)
            {
                if (partyadd_checkarray[3][2] == 1)
                {
                    //up
                    partyadd_checkarray[3][2] = 2;
                    x2 = x;
                    y2 = y - 1;
                    goto partyadd_foundlocation2;
                }
                if (partyadd_checkarray[4][3] == 1)
                {
                    //right
                    partyadd_checkarray[4][3] = 2;
                    x2 = x + 1;
                    y2 = y;
                    goto partyadd_foundlocation2;
                }
                if (partyadd_checkarray[3][4] == 1)
                {
                    //down
                    partyadd_checkarray[3][4] = 2;
                    x2 = x;
                    y2 = y + 1;
                    goto partyadd_foundlocation2;
                }
                if (partyadd_checkarray[2][3] == 1)
                {
                    //left
                    partyadd_checkarray[2][3] = 2;
                    x2 = x - 1;
                    y2 = y;
                    goto partyadd_foundlocation2;
                }
                if (partyadd_checkarray[3][2] == 2)
                {
                    //up
                    partyadd_checkarray[3][2] = 2;
                    x2 = x;
                    y2 = y - 1;
                    goto partyadd_foundlocation2;
                }
                if (partyadd_checkarray[4][3] == 2)
                {
                    //right
                    partyadd_checkarray[4][3] = 2;
                    x2 = x + 1;
                    y2 = y;
                    goto partyadd_foundlocation2;
                }
                if (partyadd_checkarray[3][4] == 2)
                {
                    //down
                    partyadd_checkarray[3][4] = 2;
                    x2 = x;
                    y2 = y + 1;
                    goto partyadd_foundlocation2;
                }
                if (partyadd_checkarray[2][3] == 2)
                {
                    //left
                    partyadd_checkarray[2][3] = 2;
                    x2 = x - 1;
                    y2 = y;
                    goto partyadd_foundlocation2;
                }
                //No good location was found! So place horse facing right from left square
                OBJadd(x - 1, y, obj);
                partyadd_checkarray[2][3] = 2;
                x2 = x;
                y2 = y;
                goto partyadd_foundlocation3;
            } //horse

            if (z2)
            {
                z = 128; //try up to 128 times then give up
            partyadd_nextlocation:
                if (z >= 92)
                {
                    x3 = rnd * 3;
                    y3 = rnd * 3;
                    x3 += 2;
                    y3 += 2;
                }
                else
                {
                    x3 = rnd * 5;
                    y3 = rnd * 5;
                    x3++;
                    y3++;
                }
                if (partyadd_checkarray[x3][y3] == 1)
                {
                    //possible
                    if ((partyadd_checkarray[x3 + 1][y3] == 2) || (partyadd_checkarray[x3 - 1][y3] == 2) || (
                        partyadd_checkarray[x3][y3 + 1] == 2) || (partyadd_checkarray[x3][y3 - 1] == 2))
                    {
                        //next to another partymember
                        x2 = x + x3 - 3;
                        y2 = y + y3 - 3;
                        goto partyadd_foundlocation;
                    } //next to another partymember
                } //possible
                z--;
                if (z) goto partyadd_nextlocation;
            }
            x2 = x;
            y2 = y;
            x3 = 3;
            y3 = 3;
        partyadd_foundlocation:
            partyadd_checkarray[x3][y3] = 2;
            //add at x2,y2
        partyadd_foundlocation2:
            OBJadd(x, y, obj);
        partyadd_foundlocation3:
            if ((x != x2) || (y != y2))
            {
                OBJmove_allow = TRUE;
                OBJmove2(obj, x2, y2);
                OBJmove_allow = FALSE;
            } //((x!=x2)||(y!=y2))
            z2++;
            tnpc->pathn = 0;
        } //->party[i]
    } //i
} //partyadd()


void horsedismount(object* myobj)
{
    //forces npc to dismount horse
    //variable names are the same as those used for a "normal" dismount
    static npc* tnpc2;
    static object *myobj5, *myobj6;
    HORSEDISMOUNT_HORSEOBJECT = NULL;
    tnpc2 = (npc*)myobj->more;
    if (tnpc2->horse == NULL) return;
    //restore the horses head
    myobj5 = tnpc2->horse;
    tnpc2->horse = NULL;
    OBJadd(myobj->x, myobj->y, myobj5);
    if ((myobj->type & 1023) == 412)
    {
        myobj5->type = myobj->type - 431 + 430 + 19;
    }
    else
    {
        myobj5->type = myobj->type - 431 + 430;
    }
    HORSEDISMOUNT_HORSEOBJECT = myobj5;
    //restore the horses tail
    myobj6 = (object*)tnpc2->more;
    myobj6->more = myobj5;
    myobj6->type = myobj6->type - 431 + 430;
    //restore the partymember
    tnpc2->more = NULL;
    myobj->type = tnpc2->originalobjtype;
    tnpc2->originalobjtype = NULL;
}

unsigned char usinghorse(player* tplayer)
{
    static long i;
    static npc* tnpc;
    for (i = 0; i <= 7; i++)
    {
        if (tplayer->party[i])
        {
            tnpc = (npc*)tplayer->party[i]->more;
            if (tnpc->horse) return 1;
        } //tplayer->party[i]
    } //i
    return 0;
}

void additem(npc* tnpc, object* obj)
{
    //adds an item to an npc's currently open bag (or root inventory if no bag is open)
    //note: item must be removed first!
    static object *baseitem, *baseitemmore;
    if (obj->next || obj->prev) exit(501); //ERROR! Item to add has not been removed correctly!
    baseitem = tnpc->baseitem;
    baseitemmore = (object*)baseitem->more;
    if (baseitemmore)
    {
        obj->next = baseitemmore;
        baseitemmore->prev = obj;
    }
    else
    {
        obj->next = NULL;
    }
    obj->prev = baseitem;
    baseitem->more = obj;
}

void additemroot(npc* tnpc, object* obj)
{
    //adds an item to an npc's ROOT inventory regardless of which bag is open
    //note: item must be removed first!
    static object *baseitem, *baseitemmore;
    if (obj->next || obj->prev) exit(502); //ERROR! Item to add has not been removed correctly!
    baseitem = tnpc->items;
    baseitemmore = (object*)baseitem->more;
    if (baseitemmore)
    {
        obj->next = baseitemmore;
        baseitemmore->prev = obj;
    }
    else
    {
        obj->next = NULL;
    }
    obj->prev = baseitem;
    baseitem->more = obj;
}


object* npc_to_obj(npc* tnpc, player* tplayer)
{
    // find the object related to the npc, e.g. to use the x,y coordinates.
    int i;
    if (tplayer)
    {
        //check all npcs in party 
        for (i = 0; i <= 7; i++)
        {
            if (tplayer->party[i])
            {
                if (tnpc == (npc*)tplayer->party[i]->more)
                {
                    return tplayer->party[i];
                }
            }
        }
    }
    // TODO: NULL player passed, have to check all objects to find one that refers to the NPC
    return NULL;
}

unsigned long randomchestlocation(bool tmap)
{
    //randomizes overland random treasurechest location and treasuremap chest locations
    static short x3, x2, y2;
    static long x8, y8;
    x3 = 1;
    while (x3)
    {
        x3 = 0;
        x2 = rnd * 15;
        y2 = rnd * 15;
        if (((x2 <= 1 || x2 >= 13) && y2 == 0) || ((x2 == 0 || x2 == 14) && y2 == 1) || ((x2 == 11 || x2 == 12) && y2 ==
            4))
        {
            //no valid spawn locations (or very few of them)
            x3 = 1;
        }
        else if (((x2 == 10 || x2 == 11) && y2 == 5) || ((x2 >= 7 && x2 <= 11) && y2 == 6) || ((x2 >= 7 && x2 <= 12) &&
            y2 == 7))
        {
            //splitting up to make it less messy
            x3 = 1;
        }
        else if (((x2 == 0 || x2 == 6 || x2 == 7 || (x2 >= 9 && x2 <= 12)) && y2 == 8) || ((x2 <= 1 || x2 == 6 || x2 ==
            9 || x2 >= 11) && y2 == 9))
        {
            x3 = 1;
        }
        else if (((x2 == 0 || (x2 >= 7 && x2 <= 9) || x2 >= 11) && y2 == 10) || ((x2 <= 2 || (x2 >= 7 && x2 <= 12)) &&
            y2 == 11))
        {
            x3 = 1;
        }
        else if (((x2 == 0 || x2 == 3 || (x2 >= 7 && x2 <= 9) || x2 == 11 || x2 == 12) && y2 == 12) || ((x2 == 0 || x2
            == 3 || x2 == 9 || x2 == 10 || x2 == 12) && y2 == 13))
        {
            x3 = 1;
        }
        else if ((x2 == 0 || x2 == 3 || x2 == 4) && y2 == 14)
        {
            x3 = 1;
        }
    }
    x8 = 32 + x2 * 64 + (unsigned short)(rnd * 64);
    y8 = 26 + y2 * 64 + (unsigned short)(rnd * 64);
    //two different while loop checks for treasure map and the overland random treasure chest
    while ((!tmap && (bt[y8][x8] & 1024) == 0) || (tmap && (bt[y8][x8] & 1024) == 0) || !(((bt[y8][x8] & 1023) == 1) ||
        (((bt[y8][x8] & 1023) >= 52) && ((bt[y8][x8] & 1023) < 64)) || (((bt[y8][x8] & 1023) >= 73) && ((bt[y8][x8] &
            1023) < 96)) || (((bt[y8][x8] & 1023) >= 108) && ((bt[y8][x8] & 1023) < 112))) || ((x8 == 707) && (y8 ==
        835)))
    {
        x8 = 32 + x2 * 64 + (unsigned short)(rnd * 64);
        y8 = 26 + y2 * 64 + (unsigned short)(rnd * 64);
    }

    /*x8=408+(unsigned short)(rnd*57)-28;
    y8=535+(unsigned short)(rnd*45)-22;
    while ((bt[y8][x8]&1024)==0) {
    x8=408+(unsigned short)(rnd*57)-28;
    y8=535+(unsigned short)(rnd*45)-22;
    }*/
    //x8=349;
    //y8=349;
    x8 = x8 + y8 * 1024;
    return (x8);
}

/*static unsigned long list[500]; // for debugging
static unsigned long list2[16]; // for debugging
unsigned char init=0;*/
unsigned long randchestitem()
{
    //random item types and quantity for treasure chest contents
    static short x5;
    static long x4;
    //x4=rnd*4;
    x4 = rnd * 9; //make wep & armour more common
    x5 = 0;
    switch (x4)
    {
    //random new item type
    case 0: //any armour
    case 4:
    case 6:
    case 8:
        x4 = rnd * 25;
        if (x4 == 0) { x4 = 27; }
        break;
    case 1: //any weapon (excluding wands, staves, glass swords and rollingpins, knives etc)
    case 5:
    case 7:
        x4 = 33 + rnd * 18;
        if (x4 == 48) { x4 = 54; }
        break;
    case 2: //magic stuff (rings,wands etc)
        x4 = 78 + rnd * 8;
        if (x4 == 82) { x4 = 48; }
        if (x4 > 82) { x4 += 173; }
        break;
    case 3: //gold
        x5 = rnd * 3;
        x4 = 88 + x5;
        x5 = rnd * 1000 / (x5 * 10 + 1) + 1;
        if (x4 == 90) { x4 = 77; }
        break;
    }
    //for testing the loot spread
    /*if (!init) {
        ZeroMemory(list,sizeof(list));
        ZeroMemory(list2,sizeof(list2));
        init=1;
    }
    if(x4<500){
      list[x4]++;
    }*/
    x4 = x4 + 1024 * x5;
    return (x4);
}

void ENHANCEnewn(object* obj, unsigned short n, unsigned short n2)
{
    static long x, x2, x3;
    static unsigned long temp;
    //EXCEPTIONS!
    x2 = obj->type & 1023;
    if (obji[sprlnk[x2]].flags & (1 + 2 + 4 + 8 + 16 + 32 + 64))
    {
        //can be worn
        if (obji[sprlnk[x2]].flags & 4096) return; //multiple
        if (x2 == 78) return; //staff
        if (x2 == 79) return; //lightning
        if (x2 == 80) return; //fire
        if (x2 == 48) return; //glass sword
        if (x2 == 256) return; //protection ring
        if (x2 == 141) return; //decorative sword
        if (x = obji[sprlnk[x2]].v2)
        {
            //armour
            obj->more2 &= 0xFFFFFF00;
            x = 0;
            temp = 0;
            x2 = rnd * n;
            if (x2 == 0)
            {
                x++;
                x3 = 1;
                while ((x2 == 0) && (x != 15))
                {
                    temp = n2;
                    x3++;
                    while ((temp < 32769) && (x3 != 15))
                    {
                        temp *= n2;
                        x3++;
                    }
                    x2 = rnd * temp;
                    while ((x2 < temp / n2) && (x != 15))
                    {
                        x++;
                        temp /= n2;
                    }
                }
            } //x2
            obj->more2 |= (x << 4);
            //list2[x]++; //for debugging
            return;
        } //armour
        if (x = obji[sprlnk[x2]].v1)
        {
            //weapon
            obj->more2 &= 0xFFFFFF00;
            x = 0;
            x2 = rnd * n;
            if (x2 == 0)
            {
                x++;
                x3 = 1;
                while ((x2 == 0) && (x != 15))
                {
                    temp = n2;
                    x3++;
                    while ((temp < 32769) && (x3 != 15))
                    {
                        temp *= n2;
                        x3++;
                    }
                    x2 = rnd * temp;
                    while ((x2 < temp / n2) && (x != 15))
                    {
                        x++;
                        temp /= n2;
                    }
                }
            } //x2
            //list2[x]++; //for debugging
            obj->more2 |= x;
        } //weapon
    } //can be worn
} //ENHANCEnew

/* adds new hireling npc to specified position, if there is a hireling already in that position do nothing*/
void addhireling(unsigned long x3, schedule_i* sched)
{
    static unsigned long x, x2, x4, x5, x6;
    static object *myobj, *myobj2;
    static file* tfh;
    static txt* t = txtnew();
    ///NEW npc
    if ((x3 > HIRELINGS_MAX) || (hirl_obj[x3])) { return; }
    myobj = OBJnew();
    x6 = x3;
    x2 = rnd * 2; //x2=0 male 1=female
    myobj->type = OBJ_AVATAR; //type
    OBJmove_allow = TRUE;
    if (x3 >= 11) { x3 = rnd * 11; } //
    if (x3 <= 1) { OBJmove2(myobj, 383, 406); } //add to map, <-needs NPC pointer //britain
    else if (x3 == 2) { OBJmove2(myobj, 404, 589); } //paws
    else if (x3 == 3) { OBJmove2(myobj, 423, 733); } //trinsic
    else if (x3 == 4) { OBJmove2(myobj, 157, 847); } //jhelom
    else if (x3 == 5) { OBJmove2(myobj, 592, 930); } //serpent's hold
    else if (x3 == 6 || x3 == 7) { OBJmove2(myobj, 926, 527); } //moonglow
    else if (x3 == 8) { OBJmove2(myobj, 567, 615); } //buccaneer's den
    else if (x3 == 9) { OBJmove2(myobj, 739, 691); } //new magincia
    else if (x3 == 10) { OBJmove2(myobj, 233, 152); } //yew
    OBJmove_allow = FALSE;
    tnpc = (npc*)malloc(sizeof(npc));
    ZeroMemory(tnpc, sizeof(npc));

    myobj->more = tnpc;
    myobj->info |= 2; //<-


    tnpc->port = 0;
    //choose port based on x2
    x3 = rnd * 6;
    if (x2)
    {
        if (x3 == 0) tnpc->port = 194;
        if (x3 == 1) tnpc->port = 195;
        if (x3 == 2) tnpc->port = 197;
        if (x3 == 3) tnpc->port = 198;
        if (x3 == 4) tnpc->port = 202;
        if (x3 == 5) tnpc->port = 203;
    }
    else
    {
        if (x3 == 0) tnpc->port = 196;
        if (x3 == 1) tnpc->port = 199;
        if (x3 == 2) tnpc->port = 200;
        if (x3 == 3) tnpc->port = 201;
        if (x3 == 4) tnpc->port = 204;
        if (x3 == 5) tnpc->port = 205;
    }


    tnpc->converse = 201;
    tnpc->name = txtnew();

    //select name for character
    //enumerate names
    x3 = 0;
    if (x2) tfh = open(".\\host\\female.txt");
    else tfh = open(".\\host\\male.txt");
enunam_next:
    txtfilein(t, tfh);
    if (t->l)
    {
        x3++;
        goto enunam_next;
    }
    close(tfh);
    x3 = rnd * x3;
    if (x2) tfh = open(".\\host\\female.txt");
    else tfh = open(".\\host\\male.txt");
    for (x = 0; x <= x3; x++)
    {
        txtfilein(tnpc->name, tfh);
    }
    close(tfh);

    if (!sched)
    {
        newschedule2++;
        for (x = 0; x <= 31; x++) schedule2[newschedule2][x].hour = 255;
        schedule2[newschedule2][0].hour = 1;
        schedule2[newschedule2][0].x = myobj->x;
        schedule2[newschedule2][0].y = myobj->y;
        schedule2[newschedule2][0].type = 0x8F;
        tnpc->schedule = &schedule2[newschedule2][0];
    }
    else
    {
        sched->x = myobj->x;
        sched->y = myobj->y;
        tnpc->schedule = sched;
    }
    tnpc->schedule_i = -1; //unknown
    tnpc->order = 2; //schedule

    //create an INVISIBLE container for player's items
    myobj2 = OBJnew();
    myobj2->type = OBJ_BAG; //bag (INVISIBLE)
    tnpc->items = myobj2;
    tnpc->baseitem = myobj2;

    tnpc->upflags |= 1; //full update

    //8 base on all stats + 16 (cheap NPC)
    //16 base on all stats + 16 (high NPC)

    x4 = rnd * 2;
    if (x4)
    {
        tnpc->s = 16;
        tnpc->d = 16;
        tnpc->i = 16;
    }
    else
    {
        tnpc->s = 8;
        tnpc->d = 8;
        tnpc->i = 8;
    }
    for (x = 0; x <= 15; x++)
    {
        x5 = rnd * 3;
        if (x5 == 0) tnpc->s++;
        if (x5 == 1) tnpc->d++;
        if (x5 == 2) tnpc->i++;
    }
    tnpc->hp = 30;
    tnpc->hp_max = 30;
    tnpc->lev = 1;
    tnpc->exp = 256 + rnd * 256 + rnd * (x4 * 512);

    tnpc->mp_max = get_mp_max(tnpc->i);
    tnpc->mp = tnpc->mp_max;

    tnpc->wt = 0;
    tnpc->wt_max = tnpc->s * 3 * 100;
    tnpc->wt2 = 0;
    tnpc->wt2_max = tnpc->s * 50;

    static unsigned short bodytype[16];

    if (x2)
    {
        //female
        bodytype[0] = OBJ_FIGHTER;
        bodytype[1] = OBJ_SWASHBUCKLER;
        bodytype[2] = OBJ_MAGE;
        bodytype[3] = OBJ_FARMER;
        bodytype[4] = OBJ_MUSICIAN;
        bodytype[5] = OBJ_WOMAN;
        if ((tnpc->s < tnpc->s) && (tnpc->s < tnpc->d)) bodytype[0] = 0;
        if (x4)
        {
            bodytype[2] = 0;
            bodytype[3] = 0;
        }
        if ((tnpc->d < tnpc->s) && (tnpc->d < tnpc->i)) bodytype[4] = 0;
    fbodytype_retry:
        x5 = rnd * 6;
        if (!bodytype[x5]) goto fbodytype_retry;
        myobj->type = bodytype[x5];
    }
    else
    {
        //male
        bodytype[0] = OBJ_FIGHTER;
        bodytype[1] = OBJ_SWASHBUCKLER;
        bodytype[2] = OBJ_MAGE;
        bodytype[3] = OBJ_PEASANT;
        bodytype[4] = OBJ_FARMER;
        bodytype[5] = OBJ_MUSICIAN;
        bodytype[6] = OBJ_AVATAR;
        if ((tnpc->s < tnpc->s) && (tnpc->s < tnpc->d)) bodytype[0] = 0;
        if ((tnpc->i < tnpc->s) && (tnpc->i < tnpc->d)) bodytype[2] = 0;
        if (x4)
        {
            bodytype[3] = 0;
            bodytype[4] = 0;
        }
        else { bodytype[6] = 0; }
        if ((tnpc->d < tnpc->s) && (tnpc->d < tnpc->i)) bodytype[5] = 0;
    mbodytype_retry:
        x5 = rnd * 7;
        if (!bodytype[x5]) goto mbodytype_retry;
        myobj->type = bodytype[x5];
        /*
        Honesty (Truth - Mage) i3
        Compassion (Love - Bard)  d3
        Valor (Courage - Fighter) s3
        Justice (Truth and Love - Druid) s1 i1
        Sacrifice (Love and Courage - Tinker) s1 d1
        Honor (Truth and Courage - Paladin) d1 i1
        Spirituality (Truth and Love and Courage - Ranger) s1 d1 i1
        Humility (None - Shepherd) N/A
        #define U6O_HONESTY tnpc->i+=3; mage, woman(with dress)
        #define U6O_COMPASSION tnpc->d+=3; bard(aka iolo)
        #define U6O_VALOR tnpc->s+=3; fighter,fighter
        #define U6O_JUSTICE tnpc->s+=1; tnpc->i+=1;
        #define U6O_SACRIFICE tnpc->s+=1; tnpc->d+=1; aka shamino
        #define U6O_HONOR tnpc->d+=1; tnpc->i+=1;
        #define U6O_SPIRITUALITY tnpc->s+=1; tnpc->d+=1; tnpc->i+=1;
        #define U6O_HUMILITY farmer
        */
    }
    hirl_obj[x6] = myobj;
    hirl_wait[x6] = 8192.0f + rnd * 8192.0f; //default values ~2 to ~4h
    //hirl_wait[x6]=150.0f+rnd*150.0f; //debug
}

void crtrespawn(object* obj)
{
    static int x2;
    static creature* crt;
    crt = (creature*)obj->more;
    OBJmove_allow = TRUE;
    for (x2 = 0; x2 <= 15; x2++)
    {
        //2004 fix for silver serpents
        OBJmove2(obj, DEATHPOSX + x2, DEATHPOSY);
    }
    OBJmove_allow = FALSE;
    //obj will now be destroyed
    VLNKremove(obj);
    VLNKsremove(obj);

    if (crt->respawn_hp)
    {
        //respawn info
        for (x2 = 0; x2 <= (respawn_last + 1); x2++)
        {
            if (respawn[x2] == NULL)
            {
                respawn[x2] = obj;
                respawn_delay[x2] = 1200; //~34 min
                respawn_delay[x2] += 1200; //+~17 min
                if (x2 > respawn_last)
                {
                    respawn_last++;
                }
                goto respawn_added;
            }
        }
    respawn_added:;
    }
    else
    {
        /* free summoned creatures and split slimes*/
        free(crt);
        OBJremove(obj);
        OBJrelease(obj);
    }
}


// c111 host functions
unsigned int new1_getexpdeduction(npc* npc, int option)
{
    unsigned short level = npc->lev;
    unsigned int deductexp;
    int currentlevelexp;
    float deductpercent;

    if (level <= 3)
    {
        if (option == 1)
            deductpercent = (float)1 / 16;
        else
            deductpercent = (float)1 / 8;

        deductexp = npc->exp * deductpercent;
    }
    else if (level > 3)
    {
        if (level >= 8)
        {
            if (option == 1)
                deductpercent = (float)1 / 2;
            else
                deductpercent = (float)1;
        }
        else
        {
            if (option == 1)
                deductpercent = (float)1 / 4;
            else
                deductpercent = (float)1 / 2;
        }

        currentlevelexp = npc->exp - new1_getexprequired(level - 1);
        deductexp = 2000 * deductpercent;

        if (currentlevelexp <= deductexp)
            deductexp = currentlevelexp;
    }

    return deductexp;
}

// t111
int getarenaid(player* player)
{
    int arenaid = -1;

    if (player)
    {
        int px = player->x;
        int py = player->y;

        for (int i = 0; i < arenacount; i++)
        {
            if (((px >= arenalocx[i]) && (px < arenalocx[i] + arenasizex[i])) && ((py >= arenalocy[i]) && (py <
                arenalocy[i] + arenasizey[i])))
            {
                arenaid = i;
                break;
            }
        }
    }

    return arenaid;
}