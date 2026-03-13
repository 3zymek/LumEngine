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

    auto test = AssetLoader::LoadAudio(RootID::External, "audio/castle.wav");
    if (!test) {
        LUM_LOG_ERROR("Failed to load sound: %s", AssetLoader::GetErrorMessage());
        return 0;
    }

    auto sound = dev->LoadSound(test.value());

    std::vector<ahi::FSoundInstance> instances;
    instances.emplace_back(sound);
    auto& inst = instances.back();

    //dev->Play(inst);

    dev->PlayOneShot(sound, { .mVolume = 0.2f, .mPitch = 0.9f });

    while (true) {

        /*
        if (GetAsyncKeyState(VK_UP)) {
            float32 vol = inst.GetVolume();
            inst.SetVolume(vol += 0.01f);
            LUM_LOG_INFO("Volume up: %f", vol);
        }
        if (GetAsyncKeyState(VK_DOWN)) {
            float32 vol = inst.GetVolume();
            inst.SetVolume(vol -= 0.01f);
            LUM_LOG_INFO("Volume down: %f", vol);
        }
        if (GetAsyncKeyState(VK_SPACE)) {
            dev->Stop(inst);
        }
       */

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