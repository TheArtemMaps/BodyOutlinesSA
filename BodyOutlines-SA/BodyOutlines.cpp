#include "plugin.h"
#include "CShadows.h"
#include "CTxdStore.h"
#include "CVisibilityPlugins.h"
using namespace plugin;
RwTexture* gpOutline1Tex;
RwTexture* gpOutline2Tex;
RwTexture* gpOutline3Tex;
ThiscallEvent <plugin::AddressList<0x53E981, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<CPed*, 0>, void(CPed*)> onProcessControl;

static RwTexture* RwTextureRead(const char* name, const char* mask) {
    return plugin::CallAndReturn<RwTexture*, 0x07F3AC0>(name, mask);
}
static int32_t GetRandomNumber() {
    return rand() & RAND_MAX;
}

void Init() {
    CTxdStore::PushCurrentTxd();
    int32_t slot2 = CTxdStore::AddTxdSlot("pedoutline");
    CTxdStore::LoadTxd(slot2, GAME_PATH((char*)"MODELS\\PEDOUTLINE.TXD"));
    int32_t slot = CTxdStore::FindTxdSlot("pedoutline");
    CTxdStore::SetCurrentTxd(slot);
    gpOutline1Tex = RwTextureRead("outline_64", nullptr);
    gpOutline2Tex = RwTextureRead("outline2_64", nullptr);
    gpOutline3Tex = RwTextureRead("outline3_64", nullptr);
}

void Shutdown() {
    RwTextureDestroy(gpOutline1Tex);
    RwTextureDestroy(gpOutline2Tex);
    RwTextureDestroy(gpOutline3Tex);
    gpOutline1Tex = nullptr;
    gpOutline2Tex = nullptr;
    gpOutline3Tex = nullptr;
}

void Process(CPed* ped) {
        if (ped && ped->m_nPedState == PEDSTATE_DEAD && CVisibilityPlugins::GetClumpAlpha(ped->m_pRwClump) == 0) {
            CVector pedPos = ped->GetPosition();
            float randAngle = (uint8_t)GetRandomNumber() * (3.14f / 128.0f);

            switch (GetRandomNumber() % 3) {
            case 0:
                CShadows::AddPermanentShadow(SHADOW_DEFAULT, gpOutline1Tex, &pedPos,
                    0.9f * cos(randAngle), 0.9f * sin(randAngle), 0.9f * sin(randAngle), -0.9f * cos(randAngle),
                    255, 255, 255, 255, 4.0f, 40000, 1.0f);
                break;
            case 1:
                CShadows::AddPermanentShadow(SHADOW_DEFAULT, gpOutline2Tex, &pedPos,
                    0.9f * cos(randAngle), 0.9f * sin(randAngle), 0.9f * sin(randAngle), -0.9f * cos(randAngle),
                    255, 255, 255, 255, 4.0f, 40000, 1.0f);
                break;
            case 2:
                CShadows::AddPermanentShadow(SHADOW_DEFAULT, gpOutline3Tex, &pedPos,
                    0.9f * cos(randAngle), 0.9f * sin(randAngle), 0.9f * sin(randAngle), -0.9f * cos(randAngle),
                    255, 255, 255, 255, 4.0f, 40000, 1.0f);
                break;
            default:
                break;
            }
        }
    }




class enablecargearsound {
public:
    enablecargearsound() {
        Events::initGameEvent += []() {
            Init();
        };

        Events::pedRenderEvent += [](CPed* ped) {
            Process(ped);
        };

        Events::shutdownRwEvent += []() {
            Shutdown();
        };

    }
} _enablecargearsound;
