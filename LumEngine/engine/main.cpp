#include "engine.hpp"

using namespace lum;
using namespace lum::rhi;

int main() {
    
    Engine e;

    e.Initialize("C:/Users/szymek/Desktop/lumen_assets");

    e.SetScene("scene.lsc");
    
    e.Run();

    e.Finalize();

} 