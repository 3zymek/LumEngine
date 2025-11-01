#include <iostream>
#include "ev_manager.hpp"
#include "ev_listener.hpp"

// przykładowe eventy
struct PlayerDamaged : public ev::detail::BaseEvent {
    PlayerDamaged(int id, int dmg) : playerID(id), dmg(dmg) {}
    int playerID{};
    int dmg{};
};

struct FrameUpdate : public ev::detail::BaseEvent {
    FrameUpdate(int f) : frame(f) {}
    int frame{};
};

struct EnemySpawned : public ev::detail::BaseEvent {
    EnemySpawned(std::string n, float x, float y) : name(n), x(x), y(y) {}
    std::string name;
    float x{}, y{};
};

int main()
{
    ev::EventManager em;

    // test static subscribe
    em.Subscribe<PlayerDamaged>([](const PlayerDamaged& e) {
        std::cout << "[GLOBAL] Player " << e.playerID << " took " << e.dmg << "\n";
        });

    // test listener RAII
    {
        ev::EventListener lis(em);

        // słuchacz dynamiczny
        lis.Subscribe<FrameUpdate>([](const FrameUpdate& e) {
            std::cout << "[LISTENER] frame " << e.frame << "\n";
            });

        // emitujemy
        em.Emit<PlayerDamaged>({ 13, 5 });
        em.Emit<FrameUpdate>({ 1 });
        em.Emit<EnemySpawned>({ "Demon", 10,20 });
        em.ProcessAll(); // listener żyje -> powinno wypisać
    } // listener tu umiera -> unsubscribe auto

    // ten FrameUpdate już NIE wywoła listenera
    em.Emit<FrameUpdate>({ 2 });
    em.Emit<PlayerDamaged>({ 13, 99 });
    em.ProcessAll();

    return 0;
}
