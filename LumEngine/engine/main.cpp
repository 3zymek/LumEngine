#include "engine.hpp"
#include "ahi/core/ahi_device.hpp"
#include <Windows.h>
using namespace lum;
using namespace lum::rhi;

int main() {

    AssetLoader::SetProjectRoot("C:/Users/szymek/Desktop/lumen_assets");
    
    ahi::AudioDevice* dev = ahi::CreateDevice(ahi::AudioBackend::Fmod);

    dev->Initialize();

    auto test = AssetLoader::ResolvePath(RootID::External, "audio/here_with_me.mp3");
    if (test.empty()) {
        LUM_LOG_ERROR("Failed to load sound: %s", AssetLoader::GetErrorMessage());
        return 0;
    }

    auto sound = dev->LoadSound(test, ahi::SoundFlag::CreateSample | ahi::SoundFlag::Spatial3D);
    auto group = dev->CreateChannelGroup("test");
    ahi::FAudioEffectDescriptor desc;
    desc.mFreqPass.mLow.bEnabled = true;
    desc.mFreqPass.mLow.mCutoff = 120.f;
    desc.mFreqPass.mLow.mResonance = 1.f;
    auto effect = dev->CreateEffect(ahi::EffectPreset::Arena);

    dev->AddGroupEffect(group, effect);

    std::vector<ahi::FSoundInstance> instances(1);
    auto& inst = instances.back();
    inst.mSound = sound;
    inst.mPitch = 1.0f;
    inst.mVolume = 0.3f;
    inst.bLooped = true;

    dev->Play(inst, group);

    while (true) {

        dev->Update(inst);

        Sleep(16);

    }

    return 0;

    Engine e;

    e.Initialize("C:/Users/szymek/Desktop/lumen_assets");

    e.SetScene("scene.lsc");
    
    e.Run();

    e.Finalize();

} 