#include <Fuego.h>

class SandboxApp : public Fuego::Application
{
   public:
    SandboxApp() = default;
};

Fuego::Application* Fuego::CreateApplication() { return new SandboxApp(); }
