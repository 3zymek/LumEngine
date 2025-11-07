#include "ev_bus.hpp"
#include <iostream>
#include <fmod.hpp>
#include <e_define.hpp>
#include "entity.hpp"
#include "c_audio_emitter.hpp"
#include "audio_manager.hpp"
#include "path_service.hpp"
struct test {
    lumEvent;
    test(int a) : t(a) {}
    int t;
};
int main() {

    PathService::SetRoot("assets");

    Entity e;
    
    audio::AudioManager::Global().Init();

    auto audio = e.AddComponent<AudioEmitterComponent>();

    audio->emitterID = audio::AudioManager::Global().CreateEmitter();

    auto emitter = audio::AudioManager::Global().GetEmitter(audio->emitterID);

    audio::AudioManager::Global().LoadSound("test.wav", FMOD_DEFAULT);
    audio::AudioManager::Global().Play("test.wav");

    while (true) { audio::AudioManager::Global().Update(); }
}
