#include "engine.hpp"
#include "ahi/core/ahi_device.hpp"
#include <Windows.h>
using namespace lum;
using namespace lum::rhi;

int main() {

    AssetLoader::SetProjectRoot("C:/Users/szymek/Desktop/lumen_assets");
    
    ahi::AudioDevice* dev = ahi::CreateDevice(ahi::AudioBackend::Fmod);

    dev->Initialize();

    auto test = AssetLoader::ResolvePath(RootID::External, "audio/test2.wav");
    if (test.empty()) {
        LUM_LOG_ERROR("Failed to load sound: %s", AssetLoader::GetErrorMessage());
        return 0;
    }

    auto sound = dev->LoadSound(test, ahi::SoundFlag::CreateSample | ahi::SoundFlag::Spatial3D);
    auto group = dev->CreateChannelGroup("test");
    auto effect = dev->CreateEffect(ahi::EffectPreset::ConcertHall);

    dev->AddGroupEffect(group, effect);

    std::vector<ahi::FSoundInstance> instances;
    instances.emplace_back(sound);
    auto& inst = instances.back();
    inst.SetPitch(0.9f);
    inst.SetVolume(0.2f);
    inst.SetLoop(true);
    inst.SetPosition({ 0.f, 0.f, -2.f });

    dev->Play(inst, group);

    while (true) {

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