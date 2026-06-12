# Admin & Chat Commands Reference

**Status:** Documented June 2026.
**Scope:** every chat command the host and client recognize, plus the
privilege model that gates the admin ones.
**Audience:** server operators and anyone touching the command dispatch in
`src/server/loop_host.cpp`.

Commands are typed into the chat input. Three categories exist:

- **Host commands** — sent to the server; admin ones are privilege-gated.
- **Debug / builder commands** — backtick (`` ` ``) prefixed; only active when
  the host runs in *easy mode*.
- **Client-local commands** — handled entirely in the client, never sent to
  the host.

## Privilege model

Two mechanisms work together, both host-side:

1. **`admins.txt`** — a newline-separated list of player names loaded at
   startup (`setup_host.inc:236–245` into the `admins[]` array).
   A name in this file is granted **full privileges** on login
   (`loop_host.cpp:5895–5903`) and is the **only** way to use `/QUITHOST`.
2. **Per-capability privilege bits** — each player carries a set of 2-bit
   permission fields packed into `GNPCflags`.
   For a capability with index `idx` (0–12), the player's level is:

   ```c
   level = (GNPCflags[idx / 16 + 40] >> ((idx % 16) * 2)) & 3;   // 0..3
   ```

   A command runs only if the caller's `level` for that capability is high
   enough.
   Most commands also enforce a **hierarchy check**: you cannot act on a
   target whose level for that capability is **higher than your own**
   (rejected with *"This player's privilege level is higher than yours!"*).

Insufficient rights are rejected with *"Incorrect privileges!"*
(`privilege_denied`).
Every successful admin action is appended to `.\save\adminlog.txt`.

### Privilege levels vs. capability indices

- **Levels** are tiers `0` (none), `1`, `2`, `3` granted with the
  `/GRANT*` commands below.
- **Capability indices** (0–12) identify *which* action is being gated; the
  2-bit value stored at each index is compared against the caller's level.

## Privilege-management commands

Set or clear a player's privilege tier.
Each requires the caller to already hold a sufficient level
(`privileges_levelrequiredtochange`).
Dispatch: `loop_host.cpp:7268–7307`.

| Command | Grants level | Caller level required |
|---|---|---|
| `/GRANT <name>`   | 1 | 2 |
| `/UNGRANT <name>` | removes 1 | 1 |
| `/GRANT2 <name>`  | 2 | 3 |
| `/UNGRANT2 <name>`| removes 2 | 2 |
| `/GRANT3 <name>`  | 3 | 3 |
| `/UNGRANT3 <name>`| removes 3 | 3 |

## Admin moderation commands

Privilege-gated by capability index (`loop_host.cpp:7313–8205`).
Each entry below lists the index checked.

| Command | Index | Effect |
|---|---|---|
| `/BAN <name>` | 0 | Ban by character name (writes `ban.txt`) |
| `/UNBAN <name>` | 1 | Remove a name ban |
| `/KICK <name>` | 2 | Disconnect a player |
| `/DELETEPORTRAIT <name>` | 3 | Delete a custom portrait and kick the player |
| `/BLOCKPORTRAITUPLOAD <name>` | 4 | Prevent a player uploading portraits |
| `/UNBLOCKPORTRAITUPLOAD <name>` | 5 | Re-allow portrait uploads |
| `/BANVOICECHAT <name>` | 6 | Block a player from voice chat |
| `/UNBANVOICECHAT <name>` | 7 | Restore voice-chat access |
| `/SETMOTD <text>` | 8 | Set the message of the day (`motd.txt`) |
| `/SYSBAN <name>` | 9 | System ban (client time-limited to 8s in-world) |
| `/UNSYSBAN <name>` | 9 | Release a system ban |
| `/BANIP <name>` | 10 | Ban a player's IP (writes `banip.txt`) |
| `/UNBANIP <name>` | 11 | Remove an IP ban |
| `/GETIP <name>` | 12 | Report a player's IP (no hierarchy check) |
| `/QUITHOST` | — | Shut down the server. **`admins.txt` names only** (`loop_host.cpp:8208–8236`) |

## General player commands (not gated)

Available to everyone (`loop_host.cpp:7237–7259, 8240–8613`).

| Command | Effect |
|---|---|
| `/WHO` | List players currently in Britannia |
| `/WHERE` | Report your own tile coordinates as a system message |
| `/ME <text>` | Emote / action message |
| `/RETURNBODY` | Recover your corpse when dead |
| `/RETURNBODYFOLLOWERS` | Recover your corpse together with followers |
| `/RESYNC` | Request a world resync from the host |

## Debug / builder commands (easy-mode host only)

Backtick-prefixed and gated behind `if (easymodehostn1)`, **not** the
privilege system (`loop_host.cpp:8267–8515`).
They only function when the host is launched in easy mode.

| Command | Effect |
|---|---|
| `` `ADD `` / `` `ADD1 `` / `` `ADD2 `` / `` `ADD3 `` / `` `ADD9 `` | Spawn mobs / objects |
| `` `SET <n> `` | Set the arena mob-spawn amount (must be standing in an arena) |
| `` `KARMA `` | Report your current karma |
| `` `LOC `` | Report your location coordinates |

## Client-local commands

Handled in the client and never transmitted to the host
(`src/client/loop_client.cpp:4748–4821`).

| Command | Effect |
|---|---|
| `/CLOUDS` | Enable cloud rendering |
| `/NOCLOUDS` | Disable cloud rendering |
| `/IGNORE <name>` | Hide a player's chat |
| `/UNIGNORE <name>` | Stop ignoring a player |
| `/MUTE <name>` | Silence a player's voice chat |
| `/UNMUTE <name>` | Restore a player's voice chat |

## Data files

| File | Purpose |
|---|---|
| `admins.txt` | Names granted full privileges (host working dir) |
| `ban.txt` | Banned character names |
| `banip.txt` | Banned IP addresses |
| `motd.txt` | Message of the day |
| `.\save\adminlog.txt` | Audit log of admin actions |

## Code reference index

| Concern | Location |
|---|---|
| Command dispatch (`/` block, pass 1) | `src/server/loop_host.cpp:7221`+ |
| Privilege-change commands (`/GRANT*`) | `loop_host.cpp:7268–7307` |
| Capability index assignment | `loop_host.cpp:7313–7380` |
| Command execution + gating (pass 2) | `loop_host.cpp:7489–8264` |
| `/WHO`, `/RESYNC`, `/RETURNBODY*` | `loop_host.cpp:8520–8613` |
| Debug backtick block (`easymodehostn1`) | `loop_host.cpp:8267–8515` |
| Full-privilege grant from `admins.txt` | `loop_host.cpp:5895–5903` |
| `admins.txt` load | `src/server/setup_host.inc:236–245` |
| Client-local commands | `src/client/loop_client.cpp:4748–4821` |
| `admins[]` / `ADMINSMAX` | `src/server/data_host.h:71` |

## Notes & caveats

- There is no in-game way to edit `admins.txt`; full-rights admins are
  configured by editing that file on the host and restarting.
- `/GETIP` intentionally skips the hierarchy check (reading an IP is allowed
  even against higher-level players).
- `/SYSBAN` relies on a client-side acknowledgement; if the client has already
  quit, the message may not be received.
- The two-pass structure (assign capability index, then execute) means the
  command tokens appear twice in `loop_host.cpp`; keep both passes in sync when
  adding a command.

