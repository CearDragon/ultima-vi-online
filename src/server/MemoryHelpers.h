#pragma once
#ifndef U6O_MEMORY_HELPERS_H
#define U6O_MEMORY_HELPERS_H

#include <cstdlib> // std::malloc, std::free
#include <cstring> // std::memset
#include <new>     // std::bad_alloc

#include "data_host.h" // mlobj

// ---------------------------------------------------------------------------
// Memory helpers (modernization phase P2.2 / P2.3).
//
// These centralize the legacy `malloc(sizeof(object*) * N)` and
// `malloc(sizeof(creature/npc))` patterns scattered across `function_host.cpp`
// so a future RAII / std::unique_ptr migration only needs to touch one place.
//
// Usage today is still raw-pointer based to preserve the existing ownership
// model (mlobj instances live inside `creature::more` / `player::craft_con`
// and are released by `destroyCreature` / `destroyNpc` / `destroyPlayer`).
//
// Long-term plan (P2.2 follow-up):
//   * Convert `mlobj` to `struct MultiObject { std::vector<object*> parts; };`.
//   * Wrap returned pointers in `std::unique_ptr<MultiObject>` and store
//     `unique_ptr` inside the owning struct.
//   * Audit and replace the matching `free((void*)…)` calls.
// ---------------------------------------------------------------------------

namespace u6o::server
{
    // Allocate an `mlobj` capable of holding `partCount` `object*` slots.
    // Memory is zero-initialised. Released via `releaseMultiObject`.
    inline mlobj* makeMultiObject(std::size_t partCount)
    {
        // mlobj declares `obj[1]` to dodge a compile warning, but is allocated
        // as a flexible array of `partCount` pointers. Allocate exactly that.
        const std::size_t bytes = sizeof(object*) * partCount;
        auto* result = static_cast<mlobj*>(std::malloc(bytes));
        if (!result) throw std::bad_alloc{};
        std::memset(result, 0, bytes);
        return result;
    }

    inline void releaseMultiObject(mlobj* multi) noexcept
    {
        std::free(multi);
    }

    // Allocate a zero-initialised `creature`. Released via `releaseCreature`
    // (which is currently identical to the C `free`).
    inline creature* makeCreature()
    {
        auto* result = static_cast<creature*>(std::malloc(sizeof(creature)));
        if (!result) throw std::bad_alloc{};
        std::memset(result, 0, sizeof(creature));
        return result;
    }

    inline void releaseCreature(creature* crt) noexcept
    {
        std::free(crt);
    }

    // Allocate a zero-initialised `npc`. Released via `releaseNpc`.
    inline npc* makeNpc()
    {
        auto* result = static_cast<npc*>(std::malloc(sizeof(npc)));
        if (!result) throw std::bad_alloc{};
        std::memset(result, 0, sizeof(npc));
        return result;
    }

    inline void releaseNpc(npc* tnpc) noexcept
    {
        std::free(tnpc);
    }

    // ---- P2.2 follow-up: remaining server malloc sites ---------------------

    // Allocate a zero-initialised `player`. Released via `destroyPlayer` (which
    // recursively frees embedded txt buffers and then `std::free`s the slot).
    inline player* makePlayer()
    {
        auto* result = static_cast<player*>(std::malloc(sizeof(player)));
        if (!result) throw std::bad_alloc{};
        std::memset(result, 0, sizeof(player));
        return result;
    }

    // Allocate a zero-initialised `sockets_info` for the per-client send/recv
    // worker threads. Released via `std::free` when the slot is recycled.
    inline sockets_info* makeSocketsInfo()
    {
        auto* result = static_cast<sockets_info*>(std::malloc(sizeof(sockets_info)));
        if (!result) throw std::bad_alloc{};
        std::memset(result, 0, sizeof(sockets_info));
        return result;
    }

    inline void releaseSocketsInfo(sockets_info* si) noexcept
    {
        std::free(si);
    }

    // Custom portraits are a fixed-size RGB565 blob (64x56 pixels = 7168 bytes).
    inline constexpr std::size_t kPortraitDataBytes = 7168;

    // Allocate an uninitialised portrait buffer (caller `memcpy`s pixels in).
    inline unsigned short* makePortraitBuffer()
    {
        auto* result = static_cast<unsigned short*>(std::malloc(kPortraitDataBytes));
        if (!result) throw std::bad_alloc{};
        return result;
    }

    inline void releasePortraitBuffer(unsigned short* buf) noexcept
    {
        std::free(buf);
    }

    // Per-tile cached object-id list inside `player::sobj[bufx][bufy]`. The
    // buffer holds `count` shorts (legacy code asked for `count * 2` bytes).
    inline unsigned short* makeShortBuffer(std::size_t count)
    {
        auto* result = static_cast<unsigned short*>(std::malloc(count * sizeof(unsigned short)));
        if (!result) throw std::bad_alloc{};
        return result;
    }

    inline void releaseShortBuffer(unsigned short* buf) noexcept
    {
        std::free(buf);
    }
} // namespace u6o::server

#endif // U6O_MEMORY_HELPERS_H

