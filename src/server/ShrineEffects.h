#pragma once
#ifndef U6O_SHRINE_EFFECTS_H
#define U6O_SHRINE_EFFECTS_H

#include "data_both.h" // npc

// ---------------------------------------------------------------------------
// Shrine effects (modernization phase P1.4).
//
// Replaces the legacy U6O_HONESTY / U6O_COMPASSION / ... function-like macros
// from define_host.h. Each function bumps the appropriate stat(s) of the
// supplied npc by the same amount the original macros did:
//
//   Honesty      i += 3
//   Compassion   d += 3
//   Valor        s += 3
//   Justice      d += 1, i += 1
//   Sacrifice    s += 1, d += 1
//   Honor        s += 1, i += 1
//   Spirituality s += 1, d += 1, i += 1
//   Humility     no-op (shepherd, by design)
//
// Use these in new code instead of the macros.
// ---------------------------------------------------------------------------

namespace u6o::server
{
    inline constexpr unsigned short kShrinePrimaryBonus = 3;
    inline constexpr unsigned short kShrineSecondaryBonus = 1;

    inline void applyHonestyShrine(npc& target) noexcept
    {
        target.i += kShrinePrimaryBonus;
    }

    inline void applyCompassionShrine(npc& target) noexcept
    {
        target.d += kShrinePrimaryBonus;
    }

    inline void applyValorShrine(npc& target) noexcept
    {
        target.s += kShrinePrimaryBonus;
    }

    inline void applyJusticeShrine(npc& target) noexcept
    {
        target.d += kShrineSecondaryBonus;
        target.i += kShrineSecondaryBonus;
    }

    inline void applySacrificeShrine(npc& target) noexcept
    {
        target.s += kShrineSecondaryBonus;
        target.d += kShrineSecondaryBonus;
    }

    inline void applyHonorShrine(npc& target) noexcept
    {
        target.s += kShrineSecondaryBonus;
        target.i += kShrineSecondaryBonus;
    }

    inline void applySpiritualityShrine(npc& target) noexcept
    {
        target.s += kShrineSecondaryBonus;
        target.d += kShrineSecondaryBonus;
        target.i += kShrineSecondaryBonus;
    }

    inline void applyHumilityShrine(npc& /*target*/) noexcept
    {
        // Shepherd virtue: no stat change. Kept as a function so callers can
        // dispatch uniformly over all eight virtues.
    }
} // namespace u6o::server

#endif // U6O_SHRINE_EFFECTS_H

