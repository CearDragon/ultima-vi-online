#pragma once
#ifndef DEFINE_HOST_H
#define DEFINE_HOST_H

// ---------------------------------------------------------------------------
// Server-wide compile-time constants and enums.
//
// Modernization phase P1 (see docs/plan-serverRefactor.md): legacy `#define`
// numeric constants are replaced with `inline constexpr` values and `enum
// class`. The original macro names are kept as deprecated aliases so the
// (very large) legacy translation units continue to compile while we migrate
// call sites incrementally.
// ---------------------------------------------------------------------------

namespace u6o::server
{
    // World coordinates the server teleports a dead player to.
    inline constexpr int kDeathPosX = 2032;
    inline constexpr int kDeathPosY = 1012;

    // Save-slot table is indexed [0..kSaveSlotLast] inclusive, so the array
    // length is kSaveSlotLast + 1.
    inline constexpr int kSaveSlotLast = 6143;

    // Save-file format version.
    //   3.0  ENCRYPTED3.0
    //   4.0  UNENCRYPTED3.0
    //   5    same as 4 plus SHA-2 encrypted password and 1 extra byte
    //        indicating which encryption is used.
    inline constexpr int kSaveVersion = 5;

    inline constexpr int kAdminsMax = 8;

    // Hireling cap: should be at least 11 so all taverns get a hireling
    // before random placement kicks in.
    inline constexpr int kHirelingsMax = 16;

    // Sentinel returned by movernew when a mover is located.
    inline constexpr unsigned long kMoverFound = 65536;

    inline constexpr int kHouseMax = 256;
    inline constexpr int kHouseStorageSlotMax = 600;

#ifdef CLIENT
    inline constexpr int kFirstClient = 1;
#else
    inline constexpr int kFirstClient = 0;
#endif

    // Spell-cast result code returned by the spell engine.
    enum class SpellResult : int
    {
        NotDone = 0,
        Success = 1,
        Invalid = 2,
        Failure = 3,
    };
} // namespace u6o::server

// ---------------------------------------------------------------------------
// Legacy macro shims. DO NOT USE in new code. Will be removed once every
// translation unit references the constants above directly (plan P1.2 / P7).
// ---------------------------------------------------------------------------
#define DEATHPOSX            (::u6o::server::kDeathPosX)
#define DEATHPOSY            (::u6o::server::kDeathPosY)
#define SAVESLOTLAST         (::u6o::server::kSaveSlotLast)
#define SAVEVER              (::u6o::server::kSaveVersion)
#define ADMINSMAX            (::u6o::server::kAdminsMax)
#define HIRELINGS_MAX        (::u6o::server::kHirelingsMax)
#define MOVER_FOUND          (::u6o::server::kMoverFound)
#define HOUSEMAX             (::u6o::server::kHouseMax)
#define HOUSESTORAGESLOTMAX  (::u6o::server::kHouseStorageSlotMax)
#define FIRST_CLIENT         (::u6o::server::kFirstClient)

#define SPELL_NOTDONE        (static_cast<int>(::u6o::server::SpellResult::NotDone))
#define SPELL_SUCCESS        (static_cast<int>(::u6o::server::SpellResult::Success))
#define SPELL_INVALID        (static_cast<int>(::u6o::server::SpellResult::Invalid))
#define SPELL_FAILURE        (static_cast<int>(::u6o::server::SpellResult::Failure))

// ---------------------------------------------------------------------------
// Shrine stat-improvement macros — REMOVED (P1.4 complete).
// Use the inline functions in `ShrineEffects.h` instead:
//   applyHonestyShrine, applyCompassionShrine, applyValorShrine,
//   applyJusticeShrine, applySacrificeShrine, applyHonorShrine,
//   applySpiritualityShrine, applyHumilityShrine
// ---------------------------------------------------------------------------

#endif // DEFINE_HOST_H
