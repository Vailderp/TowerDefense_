#pragma once
#ifndef SFML_DYNAMIC
#define SFML_DYNAMIC
#include <fstream>
#include <functional>
#endif
#ifdef _DEBUG
#pragma comment(lib, "lib/sfml-graphics-d.lib")
#pragma comment(lib, "lib/sfml-audio-d.lib")
#pragma comment(lib, "lib/sfml-network-d.lib")
#pragma comment(lib, "lib/sfml-system-d.lib")
#pragma comment(lib, "lib/sfml-window-d.lib")
#else
#pragma comment(lib, "lib/sfml-graphics.lib")
#pragma comment(lib, "lib/sfml-audio.lib")
#pragma comment(lib, "lib/sfml-network.lib")
#pragma comment(lib, "lib/sfml-system.lib")
#pragma comment(lib, "lib/sfml-window.lib")
#endif
