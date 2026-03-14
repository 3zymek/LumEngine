#include "engine.hpp"
#include "ahi/core/ahi_device.hpp"
#include "ahi/backend/fmod_device.hpp"
#include <Windows.h>
using namespace lum;
using namespace lum::rhi;

int main() {

    AssetLoader::SetProjectRoot("C:/Users/szymek/Desktop/lumen_assets");
    
    ahi::AudioDevice* dev = new ahi::fmod::FMODDevice();

    dev->Initialize();

    auto test = AssetLoader::LoadAudio(RootID::External, "audio/wirst.wav");
    if (!test) {
        LUM_LOG_ERROR("Failed to load sound: %s", AssetLoader::GetErrorMessage());
        return 0;
    }

    auto sound = dev->LoadSound(test.value(), ahi::SoundFlag::CreateSample | ahi::SoundFlag::Spatial3D);

    auto group = dev->CreateChannelGroup("test");
    ahi::FAudioEffectDescriptor desc;

    desc.mReverb.bEnabled = true;

    auto effect = dev->CreateEffect(desc);

    dev->AddEffectToGroup(group, effect);

    std::vector<ahi::FSoundInstance> instances;
    instances.emplace_back(sound);
    auto& inst = instances.back();
    inst.SetPitch(0.9f);
    inst.SetVolume(0.2f);
    inst.SetLoop(true);

    dev->Play(inst, group);

    while (true) {

        if (GetAsyncKeyState(VK_SPACE)) {
            inst.SetPause(!inst.IsPaused());
        }
       

        dev->Update(instances);

        Sleep(16);

    }

    return 0;

    Engine e;

    e.Initialize("C:/Users/szymek/Desktop/lumen_assets");

    e.SetScene("scene.lsc");
    
    e.Run();

    e.Finalize();

} 