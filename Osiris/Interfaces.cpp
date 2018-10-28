#include <stdexcept>
#include <type_traits>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "xorstr.hpp"

#include "Interfaces.h"

Interfaces::Interfaces()
{
    engineClient = reinterpret_cast<EngineClient*>(find(xorstr_("engine.dll"), xorstr_("VEngineClient")));
    clientEntityList = reinterpret_cast<ClientEntityList*>(find(xorstr_("client_panorama.dll"), xorstr_("VClientEntityList")));
    client = reinterpret_cast<Client*>(find(xorstr_("client_panorama.dll"), xorstr_("VClient")));
    surface = reinterpret_cast<Surface*>(find(xorstr_("vguimatsurface.dll"), xorstr_("VGUI_Surface")));
    cvar = reinterpret_cast<Cvar*>(find(xorstr_("vstdlib.dll"), xorstr_("VEngineCvar")));
}

void* Interfaces::find(const std::string& module, const std::string& name)
{
    void* foundInterface = nullptr;
    std::add_pointer_t<void* (const char* name, int* returnCode)> createInterface = reinterpret_cast<decltype(createInterface)>(GetProcAddress(GetModuleHandleA(module.c_str()), xorstr_("CreateInterface")));

    for (int i = 1; i < 100; i++) {
        std::string interfaceName{ name + std::string{ '0' } + std::to_string(i) };
        foundInterface = (void*)createInterface(interfaceName.c_str(), nullptr);
        if (foundInterface)
            break;

        interfaceName = name + xorstr_("00") + std::to_string(i);
        foundInterface = (void*)createInterface(interfaceName.c_str(), nullptr);
        if (foundInterface)
            break;
    }
    return foundInterface;
}
