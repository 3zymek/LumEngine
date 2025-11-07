#include "ev_bus.hpp"
#include <iostream>
#include <fmod.hpp>
#include <e_define.hpp>
#include "entity.hpp"
#include "c_audio_emitter.hpp"
#include "audio_manager.hpp"
struct test {
    Event;
    test(int a) : t(a) {}
    int t;
};
int main() {
    Entity e;
    auto audio = e.AddComponent<AudioEmitterComponent>();
    audio->get().id = audio::AudioManager::Global().CreateEmitter();
}
