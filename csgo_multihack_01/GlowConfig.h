#pragma once

#include "pch.h"
#include "CUtlVector.h"

struct ClrRender
{
    BYTE red, green, blue;

    ClrRender(BYTE r = 255, BYTE g = 255, BYTE b = 255)
        : red(r), green(g), blue(b) {}
};

struct GlowStruct // size: 0x38
{
    BYTE buffer0[4]; // padding
    uintptr_t hEntity;
    float red;
    float green;
    float blue;
    float alpha;
    BYTE buffer1[8];
    float bloomAmount;
    BYTE buffer2[4];
    bool renderWhenOccluded;
    bool renderWhenUnOccluded;
    bool fullBloom;
    BYTE buffer3[5];
    int glowStyle;
    BYTE buffer4[4];
};

struct GlowObjectManager {
    CUtlVector<GlowStruct> glowObjects;
    int m_nFirstFreeSlot;
};

namespace GlowColors {
    const ClrRender clrEnemy(255, 0, 0); // red
    const ClrRender clrTeam(0, 255, 0); // blue
    const ClrRender clrAll(255, 255, 255); // none
}
