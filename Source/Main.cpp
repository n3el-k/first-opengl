#include "core/App.h"
#include "rendering/Renderer.h"

int main()
{
    App app(1280, 720, "OpenGL App");
    
    if(!app.init())
    {
        return -1;
    }
    app.run();
    return 0;
}