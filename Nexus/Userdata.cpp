#include "RetroEngine.hpp"

int globalVariablesCount;
int globalVariables[GLOBALVAR_COUNT];
char globalVariableNames[GLOBALVAR_COUNT][0x20];

char gamePath[0x100];


#if RETRO_USE_MOD_LOADER
char modsPath[0x100];

std::vector<ModInfo> modList;

#include <filesystem>
#endif

void InitUserdata()
{
    // userdata files are loaded from this directory
    sprintf(gamePath, "%s", BASE_PATH);
#if RETRO_USE_MOD_LOADER
    sprintf(modsPath, "%s", BASE_PATH);
#endif

    char buffer[0x200];
#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_UWP
    if (!usingCWD)
        sprintf(buffer, "%s/settings.ini", getResourcesPath());
    else
        sprintf(buffer, "%ssettings.ini", gamePath);
#elif RETRO_PLATFORM == RETRO_iOS
    sprintf(buffer, "%s/settings.ini", getDocumentsPath());
#else
    sprintf(buffer, BASE_PATH"settings.ini");
#endif
    FileIO *file = fOpen(buffer, "rb");
    IniParser ini;
    if (!file) {
        ini.SetBool("Dev", "DevMenu", Engine.devMenu = true);
        ini.SetBool("Dev", "EngineDebugMode", engineDebugMode = false);
        ini.SetInteger("Dev", "StartingCategory", Engine.startList = 0xFF);
        ini.SetInteger("Dev", "StartingScene", Engine.startStage = 0xFF);
        ini.SetInteger("Dev", "FastForwardSpeed", Engine.fastForwardSpeed = 8);
        sprintf(Engine.dataFile, "%s", "Data.bin");
        ini.SetString("Dev", "DataFile", Engine.dataFile);

        Engine.startList_Game  = Engine.startList;
        Engine.startStage_Game = Engine.startStage;

        ini.SetBool("Window", "FullScreen", Engine.startFullScreen = DEFAULT_FULLSCREEN);
        ini.SetBool("Window", "Borderless", Engine.borderless = false);
        ini.SetBool("Window", "VSync", Engine.vsync = false);
        ini.SetBool("Window", "EnhancedScaling", Engine.enhancedScaling = true);
        ini.SetInteger("Window", "WindowScale", Engine.windowScale = 2);
        ini.SetInteger("Window", "ScreenWidth", SCREEN_XSIZE = DEFAULT_SCREEN_XSIZE);
        ini.SetInteger("Window", "RefreshRate", Engine.refreshRate = 60);
        ini.SetInteger("Window", "ColourMode", Engine.colourMode = 0);

        ini.SetFloat("Audio", "BGMVolume", bgmVolume / (float)MAX_VOLUME);
        ini.SetFloat("Audio", "SFXVolume", sfxVolume / (float)MAX_VOLUME);

#if RETRO_USING_SDL2
        ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_Scancode)");
        ini.SetInteger("Keyboard 1", "Up", inputDevice[INPUT_UP].keyMappings = SDL_SCANCODE_W);
        ini.SetInteger("Keyboard 1", "Left", inputDevice[INPUT_DOWN].keyMappings = SDL_SCANCODE_S);
        ini.SetInteger("Keyboard 1", "Down", inputDevice[INPUT_LEFT].keyMappings = SDL_SCANCODE_A);
        ini.SetInteger("Keyboard 1", "Right", inputDevice[INPUT_RIGHT].keyMappings = SDL_SCANCODE_D);
        ini.SetInteger("Keyboard 1", "A", inputDevice[INPUT_BUTTONA].keyMappings = SDL_SCANCODE_J);
        ini.SetInteger("Keyboard 1", "B", inputDevice[INPUT_BUTTONB].keyMappings = SDL_SCANCODE_K);
        ini.SetInteger("Keyboard 1", "C", inputDevice[INPUT_BUTTONC].keyMappings = SDL_SCANCODE_L);
        ini.SetInteger("Keyboard 1", "X", inputDevice[INPUT_BUTTONX].keyMappings = SDL_SCANCODE_U);
        ini.SetInteger("Keyboard 1", "Y", inputDevice[INPUT_BUTTONY].keyMappings = SDL_SCANCODE_I);
        ini.SetInteger("Keyboard 1", "Z", inputDevice[INPUT_BUTTONZ].keyMappings = SDL_SCANCODE_O);
        ini.SetInteger("Keyboard 1", "L", inputDevice[INPUT_BUTTONL].keyMappings = SDL_SCANCODE_Q);
        ini.SetInteger("Keyboard 1", "R", inputDevice[INPUT_BUTTONR].keyMappings = SDL_SCANCODE_E);
        ini.SetInteger("Keyboard 1", "Start", inputDevice[INPUT_START].keyMappings = SDL_SCANCODE_RETURN);
        ini.SetInteger("Keyboard 1", "Select", inputDevice[INPUT_SELECT].keyMappings = SDL_SCANCODE_RSHIFT);

        ini.SetComment("Controller 1", "IC1Comment", "Controller Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_GameControllerButton)");
        ini.SetInteger("Controller 1", "Up", inputDevice[INPUT_UP].contMappings = SDL_CONTROLLER_BUTTON_DPAD_UP);
        ini.SetInteger("Controller 1", "Down", inputDevice[INPUT_DOWN].contMappings = SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        ini.SetInteger("Controller 1", "Left", inputDevice[INPUT_LEFT].contMappings = SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        ini.SetInteger("Controller 1", "Right", inputDevice[INPUT_RIGHT].contMappings = SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        ini.SetInteger("Controller 1", "A", inputDevice[INPUT_BUTTONA].contMappings = SDL_CONTROLLER_BUTTON_X);
        ini.SetInteger("Controller 1", "B", inputDevice[INPUT_BUTTONB].contMappings = SDL_CONTROLLER_BUTTON_A);
        ini.SetInteger("Controller 1", "C", inputDevice[INPUT_BUTTONC].contMappings = SDL_CONTROLLER_BUTTON_B);
        ini.SetInteger("Controller 1", "X", inputDevice[INPUT_BUTTONX].contMappings = SDL_CONTROLLER_BUTTON_ZL);
        ini.SetInteger("Controller 1", "Y", inputDevice[INPUT_BUTTONY].contMappings = SDL_CONTROLLER_BUTTON_Y);
        ini.SetInteger("Controller 1", "Z", inputDevice[INPUT_BUTTONZ].contMappings = SDL_CONTROLLER_BUTTON_ZR);
        ini.SetInteger("Controller 1", "L", inputDevice[INPUT_BUTTONL].contMappings = SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        ini.SetInteger("Controller 1", "R", inputDevice[INPUT_BUTTONR].contMappings = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        ini.SetInteger("Controller 1", "Start", inputDevice[INPUT_START].contMappings = SDL_CONTROLLER_BUTTON_START);
        ini.SetInteger("Controller 1", "Select", inputDevice[INPUT_SELECT].contMappings = SDL_CONTROLLER_BUTTON_GUIDE);
#endif

#if RETRO_USING_SDL1
        ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_Scancode)");
        ini.SetInteger("Keyboard 1", "Up", inputDevice[INPUT_UP].keyMappings = SDLK_w);
        ini.SetInteger("Keyboard 1", "Down", inputDevice[INPUT_DOWN].keyMappings = SDLK_s);
        ini.SetInteger("Keyboard 1", "Left", inputDevice[INPUT_LEFT].keyMappings = SDLK_a);
        ini.SetInteger("Keyboard 1", "Right", inputDevice[INPUT_RIGHT].keyMappings = SDLK_d);
        ini.SetInteger("Keyboard 1", "A", inputDevice[INPUT_BUTTONA].keyMappings = SDLK_j);
        ini.SetInteger("Keyboard 1", "B", inputDevice[INPUT_BUTTONB].keyMappings = SDLK_k);
        ini.SetInteger("Keyboard 1", "C", inputDevice[INPUT_BUTTONC].keyMappings = SDLK_l);
        ini.SetInteger("Keyboard 1", "X", inputDevice[INPUT_BUTTONX].keyMappings = SDLK_u);
        ini.SetInteger("Keyboard 1", "Y", inputDevice[INPUT_BUTTONY].keyMappings = SDLK_i);
        ini.SetInteger("Keyboard 1", "Z", inputDevice[INPUT_BUTTONZ].keyMappings = SDLK_o);
        ini.SetInteger("Keyboard 1", "L", inputDevice[INPUT_BUTTONL].keyMappings = SDLK_q);
        ini.SetInteger("Keyboard 1", "R", inputDevice[INPUT_BUTTONR].keyMappings = SDLK_e);
        ini.SetInteger("Keyboard 1", "Start", inputDevice[INPUT_START].keyMappings = SDLK_RETURN);
        ini.SetInteger("Keyboard 1", "Select", inputDevice[INPUT_SELECT].keyMappings = SDLK_RSHIFT);

        ini.SetComment("Controller 1", "IC1Comment", "Controller Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_GameControllerButton)");
        ini.SetInteger("Controller 1", "Up", inputDevice[INPUT_UP].contMappings = 1);
        ini.SetInteger("Controller 1", "Down", inputDevice[INPUT_DOWN].contMappings = 2);
        ini.SetInteger("Controller 1", "Left", inputDevice[INPUT_LEFT].contMappings = 3);
        ini.SetInteger("Controller 1", "Right", inputDevice[INPUT_RIGHT].contMappings = 4);
        ini.SetInteger("Controller 1", "A", inputDevice[INPUT_BUTTONA].contMappings = 7);
        ini.SetInteger("Controller 1", "B", inputDevice[INPUT_BUTTONB].contMappings = 5);
        ini.SetInteger("Controller 1", "C", inputDevice[INPUT_BUTTONC].contMappings = 6);
        ini.SetInteger("Controller 1", "X", inputDevice[INPUT_BUTTONX].contMappings = 10);
        ini.SetInteger("Controller 1", "Y", inputDevice[INPUT_BUTTONY].contMappings = 9);
        ini.SetInteger("Controller 1", "Z", inputDevice[INPUT_BUTTONZ].contMappings = 11);
        ini.SetInteger("Controller 1", "L", inputDevice[INPUT_BUTTONL].contMappings = 12);
        ini.SetInteger("Controller 1", "R", inputDevice[INPUT_BUTTONR].contMappings = 13);
        ini.SetInteger("Controller 1", "Start", inputDevice[INPUT_START].contMappings = 8);
        ini.SetInteger("Controller 1", "Select", inputDevice[INPUT_SELECT].contMappings = 14);
#endif

        ini.Write(BASE_PATH"settings.ini");
    }
    else {
        fClose(file);
        ini = IniParser(BASE_PATH"settings.ini");

        if (!ini.GetBool("Dev", "DevMenu", &Engine.devMenu))
            Engine.devMenu = false;
        if (!ini.GetBool("Dev", "EngineDebugMode", &engineDebugMode))
            engineDebugMode = false;
        if (!ini.GetInteger("Dev", "StartingCategory", &Engine.startList))
            Engine.startList = 0xFF;
        if (!ini.GetInteger("Dev", "StartingScene", &Engine.startStage))
            Engine.startStage = 0xFF;
        if (!ini.GetInteger("Dev", "FastForwardSpeed", &Engine.fastForwardSpeed))
            Engine.fastForwardSpeed = 8;

        Engine.startList_Game  = Engine.startList;
        Engine.startStage_Game = Engine.startStage;

        if (!ini.GetString("Dev", "DataFile", Engine.dataFile))
            StrCopy(Engine.dataFile, "Data.bin");

        if (!ini.GetBool("Window", "FullScreen", &Engine.startFullScreen))
            Engine.startFullScreen = DEFAULT_FULLSCREEN;
        if (!ini.GetBool("Window", "Borderless", &Engine.borderless))
            Engine.borderless = false;
        if (!ini.GetBool("Window", "VSync", &Engine.vsync))
            Engine.vsync = false;
        if (!ini.GetBool("Window", "EnhancedScaling", &Engine.enhancedScaling))
            Engine.enhancedScaling = true;
        if (!ini.GetInteger("Window", "WindowScale", &Engine.windowScale))
            Engine.windowScale = 2;
        if (!ini.GetInteger("Window", "ScreenWidth", &SCREEN_XSIZE))
            SCREEN_XSIZE = DEFAULT_SCREEN_XSIZE;
        if (!ini.GetInteger("Window", "RefreshRate", &Engine.refreshRate))
            Engine.refreshRate = 60;
        int cm = Engine.colourMode;
        if (!ini.GetInteger("Window", "ColourMode", &cm))
            cm = 1;
        Engine.colourMode = cm;

        float bv = 0, sv = 0;
        if (!ini.GetFloat("Audio", "BGMVolume", &bv))
            bv = 1.0f;
        if (!ini.GetFloat("Audio", "SFXVolume", &sv))
            sv = 1.0f;

        bgmVolume = bv * MAX_VOLUME;
        sfxVolume = sv * MAX_VOLUME;

        if (bgmVolume > MAX_VOLUME)
            bgmVolume = MAX_VOLUME;
        if (bgmVolume < 0)
            bgmVolume = 0;

        if (sfxVolume > MAX_VOLUME)
            sfxVolume = MAX_VOLUME;
        if (sfxVolume < 0)
            sfxVolume = 0;

#if RETRO_USING_SDL2
        if (!ini.GetInteger("Keyboard 1", "Up", &inputDevice[INPUT_UP].keyMappings))
            inputDevice[0].keyMappings = SDL_SCANCODE_W;
        if (!ini.GetInteger("Keyboard 1", "Down", &inputDevice[INPUT_DOWN].keyMappings))
            inputDevice[1].keyMappings = SDL_SCANCODE_S;
        if (!ini.GetInteger("Keyboard 1", "Left", &inputDevice[INPUT_LEFT].keyMappings))
            inputDevice[2].keyMappings = SDL_SCANCODE_A;
        if (!ini.GetInteger("Keyboard 1", "Right", &inputDevice[INPUT_RIGHT].keyMappings))
            inputDevice[3].keyMappings = SDL_SCANCODE_D;
        if (!ini.GetInteger("Keyboard 1", "A", &inputDevice[INPUT_BUTTONA].keyMappings))
            inputDevice[4].keyMappings = SDL_SCANCODE_J;
        if (!ini.GetInteger("Keyboard 1", "B", &inputDevice[INPUT_BUTTONB].keyMappings))
            inputDevice[5].keyMappings = SDL_SCANCODE_K;
        if (!ini.GetInteger("Keyboard 1", "C", &inputDevice[INPUT_BUTTONC].keyMappings))
            inputDevice[6].keyMappings = SDL_SCANCODE_L;
        if (!ini.GetInteger("Keyboard 1", "X", &inputDevice[INPUT_BUTTONX].keyMappings))
            inputDevice[7].keyMappings = SDL_SCANCODE_U;
        if (!ini.GetInteger("Keyboard 1", "Y", &inputDevice[INPUT_BUTTONY].keyMappings))
            inputDevice[8].keyMappings = SDL_SCANCODE_I;
        if (!ini.GetInteger("Keyboard 1", "Z", &inputDevice[INPUT_BUTTONZ].keyMappings))
            inputDevice[9].keyMappings = SDL_SCANCODE_O;
        if (!ini.GetInteger("Keyboard 1", "L", &inputDevice[INPUT_BUTTONL].keyMappings))
            inputDevice[10].keyMappings = SDL_SCANCODE_Q;
        if (!ini.GetInteger("Keyboard 1", "R", &inputDevice[INPUT_BUTTONR].keyMappings))
            inputDevice[11].keyMappings = SDL_SCANCODE_E;
        if (!ini.GetInteger("Keyboard 1", "Start", &inputDevice[INPUT_START].keyMappings))
            inputDevice[12].keyMappings = SDL_SCANCODE_RETURN;
        if (!ini.GetInteger("Keyboard 1", "Start", &inputDevice[INPUT_SELECT].keyMappings))
            inputDevice[13].keyMappings = SDL_SCANCODE_RSHIFT;

        if (!ini.GetInteger("Controller 1", "Up", &inputDevice[INPUT_UP].contMappings))
            inputDevice[0].contMappings = SDL_CONTROLLER_BUTTON_DPAD_UP;
        if (!ini.GetInteger("Controller 1", "Down", &inputDevice[INPUT_DOWN].contMappings))
            inputDevice[1].contMappings = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        if (!ini.GetInteger("Controller 1", "Left", &inputDevice[INPUT_LEFT].contMappings))
            inputDevice[2].contMappings = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        if (!ini.GetInteger("Controller 1", "Right", &inputDevice[INPUT_RIGHT].contMappings))
            inputDevice[3].contMappings = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
        if (!ini.GetInteger("Controller 1", "A", &inputDevice[INPUT_BUTTONA].contMappings))
            inputDevice[4].contMappings = SDL_CONTROLLER_BUTTON_X;
        if (!ini.GetInteger("Controller 1", "B", &inputDevice[INPUT_BUTTONB].contMappings))
            inputDevice[5].contMappings = SDL_CONTROLLER_BUTTON_A;
        if (!ini.GetInteger("Controller 1", "C", &inputDevice[INPUT_BUTTONC].contMappings))
            inputDevice[6].contMappings = SDL_CONTROLLER_BUTTON_B;
        if (!ini.GetInteger("Controller 1", "X", &inputDevice[INPUT_BUTTONX].contMappings))
            inputDevice[7].contMappings = SDL_CONTROLLER_BUTTON_ZL;
        if (!ini.GetInteger("Controller 1", "Y", &inputDevice[INPUT_BUTTONY].contMappings))
            inputDevice[8].contMappings = SDL_CONTROLLER_BUTTON_Y;
        if (!ini.GetInteger("Controller 1", "Z", &inputDevice[INPUT_BUTTONZ].contMappings))
            inputDevice[9].contMappings = SDL_CONTROLLER_BUTTON_ZR;
        if (!ini.GetInteger("Controller 1", "L", &inputDevice[INPUT_BUTTONL].contMappings))
            inputDevice[10].contMappings = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
        if (!ini.GetInteger("Controller 1", "R", &inputDevice[INPUT_BUTTONR].contMappings))
            inputDevice[11].contMappings = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
        if (!ini.GetInteger("Controller 1", "Start", &inputDevice[INPUT_START].contMappings))
            inputDevice[12].contMappings = SDL_CONTROLLER_BUTTON_START;
        if (!ini.GetInteger("Controller 1", "Select", &inputDevice[INPUT_SELECT].contMappings))
            inputDevice[13].contMappings = SDL_CONTROLLER_BUTTON_GUIDE;
#endif

#if RETRO_USING_SDL1
        if (!ini.GetInteger("Keyboard 1", "Up", &inputDevice[INPUT_UP].keyMappings))
            inputDevice[0].keyMappings = SDLK_w;
        if (!ini.GetInteger("Keyboard 1", "Down", &inputDevice[INPUT_DOWN].keyMappings))
            inputDevice[1].keyMappings = SDLK_s;
        if (!ini.GetInteger("Keyboard 1", "Left", &inputDevice[INPUT_LEFT].keyMappings))
            inputDevice[2].keyMappings = SDLK_a;
        if (!ini.GetInteger("Keyboard 1", "Right", &inputDevice[INPUT_RIGHT].keyMappings))
            inputDevice[3].keyMappings = SDLK_d;
        if (!ini.GetInteger("Keyboard 1", "A", &inputDevice[INPUT_BUTTONA].keyMappings))
            inputDevice[4].keyMappings = SDLK_j;
        if (!ini.GetInteger("Keyboard 1", "B", &inputDevice[INPUT_BUTTONB].keyMappings))
            inputDevice[5].keyMappings = SDLK_k;
        if (!ini.GetInteger("Keyboard 1", "C", &inputDevice[INPUT_BUTTONC].keyMappings))
            inputDevice[6].keyMappings = SDLK_l;
        if (!ini.GetInteger("Keyboard 1", "X", &inputDevice[INPUT_BUTTONX].keyMappings))
            inputDevice[4].keyMappings = SDLK_j;
        if (!ini.GetInteger("Keyboard 1", "Y", &inputDevice[INPUT_BUTTONY].keyMappings))
            inputDevice[5].keyMappings = SDLK_k;
        if (!ini.GetInteger("Keyboard 1", "Z", &inputDevice[INPUT_BUTTONZ].keyMappings))
            inputDevice[6].keyMappings = SDLK_l;
        if (!ini.GetInteger("Keyboard 1", "L", &inputDevice[INPUT_BUTTONL].keyMappings))
            inputDevice[5].keyMappings = SDLK_q;
        if (!ini.GetInteger("Keyboard 1", "R", &inputDevice[INPUT_BUTTONR].keyMappings))
            inputDevice[6].keyMappings = SDLK_e;
        if (!ini.GetInteger("Keyboard 1", "Start", &inputDevice[INPUT_START].keyMappings))
            inputDevice[7].keyMappings = SDLK_RETURN;
        if (!ini.GetInteger("Keyboard 1", "Select", &inputDevice[INPUT_SELECT].keyMappings))
            inputDevice[7].keyMappings = SDLK_RSHIFT;

        if (!ini.GetInteger("Controller 1", "Up", &inputDevice[INPUT_UP].contMappings))
            inputDevice[0].contMappings = 1;
        if (!ini.GetInteger("Controller 1", "Down", &inputDevice[INPUT_DOWN].contMappings))
            inputDevice[1].contMappings = 2;
        if (!ini.GetInteger("Controller 1", "Left", &inputDevice[INPUT_LEFT].contMappings))
            inputDevice[2].contMappings = 3;
        if (!ini.GetInteger("Controller 1", "Right", &inputDevice[INPUT_RIGHT].contMappings))
            inputDevice[3].contMappings = 4;
        if (!ini.GetInteger("Controller 1", "A", &inputDevice[INPUT_BUTTONA].contMappings))
            inputDevice[4].contMappings = 7;
        if (!ini.GetInteger("Controller 1", "B", &inputDevice[INPUT_BUTTONB].contMappings))
            inputDevice[5].contMappings = 5;
        if (!ini.GetInteger("Controller 1", "C", &inputDevice[INPUT_BUTTONC].contMappings))
            inputDevice[6].contMappings = 6;
        if (!ini.GetInteger("Controller 1", "X", &inputDevice[INPUT_BUTTONX].contMappings))
            inputDevice[4].contMappings = 14;
        if (!ini.GetInteger("Controller 1", "Y", &inputDevice[INPUT_BUTTONY].contMappings))
            inputDevice[5].contMappings = 8;
        if (!ini.GetInteger("Controller 1", "Z", &inputDevice[INPUT_BUTTONZ].contMappings))
            inputDevice[6].contMappings = 15;
        if (!ini.GetInteger("Controller 1", "L", &inputDevice[INPUT_BUTTONL].contMappings))
            inputDevice[5].contMappings = 11;
        if (!ini.GetInteger("Controller 1", "R", &inputDevice[INPUT_BUTTONR].contMappings))
            inputDevice[6].contMappings = 12;
        if (!ini.GetInteger("Controller 1", "Start", &inputDevice[INPUT_START].contMappings))
            inputDevice[7].contMappings = 8;
        if (!ini.GetInteger("Controller 1", "Select", &inputDevice[INPUT_SELECT].contMappings))
            inputDevice[7].contMappings = 14;
#endif
    }
    SetScreenSize(SCREEN_XSIZE, SCREEN_YSIZE);

    // Support for extra controller types SDL doesn't recognise
#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_UWP
    if (!usingCWD)
        sprintf(buffer, "%s/controllerdb.txt", getResourcesPath());
    else
        sprintf(buffer, "%scontrollerdb.txt", gamePath);
#else
    sprintf(buffer, BASE_PATH "controllerdb.txt");
#endif

#if RETRO_USING_SDL2
    file = fOpen(buffer, "rb");
    if (file) {
        fClose(file);

        int nummaps = SDL_GameControllerAddMappingsFromFile(buffer);
        if (nummaps >= 0)
            printLog("loaded %d controller mappings from '%s'\n", buffer, nummaps);
    }
#endif
}

void writeSettings() {
    IniParser ini;

    ini.SetComment("Dev", "DevMenuComment", "Enable this flag to activate dev menu via the ESC key");
    ini.SetBool("Dev", "DevMenu", Engine.devMenu);
    ini.SetComment("Dev", "DebugModeComment", "Enable this flag to activate features used for debugging the engine (may result in slightly slower game speed)");
    ini.SetBool("Dev", "EngineDebugMode", engineDebugMode);
    ini.SetComment("Dev", "SCComment", "Sets the starting category ID");
    ini.SetInteger("Dev", "StartingCategory", Engine.startList);
    ini.SetComment("Dev", "SSComment", "Sets the starting scene ID");
    ini.SetInteger("Dev", "StartingScene", Engine.startStage);
    ini.SetComment("Dev", "FFComment", "Determines how fast the game will be when fastforwarding is active");
    ini.SetInteger("Dev", "FastForwardSpeed", Engine.fastForwardSpeed);

    ini.SetComment("Dev", "DataFileComment", "Determines what Datafile will be loaded");
    ini.SetString("Dev", "DataFile", Engine.dataFile);

    ini.SetComment("Dev", "DataFileComment", "Determines what RSDK file will be loaded");
    ini.SetString("Dev", "DataFile", Engine.dataFile);

    ini.SetComment("Window", "FSComment", "Determines if the window will be fullscreen or not");
    ini.SetBool("Window", "FullScreen", Engine.startFullScreen);
    ini.SetComment("Window", "BLComment", "Determines if the window will be borderless or not");
    ini.SetBool("Window", "Borderless", Engine.borderless);
    ini.SetComment("Window", "VSComment", "Determines if VSync will be active or not");
    ini.SetBool("Window", "VSync", Engine.vsync);
    ini.SetComment("Window", "ESComment", "Determines if Enhanced Scaling will be active or not. Only affects non-multiple resolutions.");
    ini.SetBool("Window", "EnhancedScaling", Engine.enhancedScaling);
    ini.SetComment("Window", "WSComment", "How big the window will be");
    ini.SetInteger("Window", "WindowScale", Engine.windowScale);
    ini.SetComment("Window", "SWComment", "How wide the base screen will be in pixels");
    ini.SetInteger("Window", "ScreenWidth", SCREEN_XSIZE);
    ini.SetComment("Window", "RRComment", "Determines the target FPS");
    ini.SetInteger("Window", "RefreshRate", Engine.refreshRate);
    ini.SetComment("Window", "CMComment", "Determines the output colour mode (0 = 8-bit, 1 = 16-bit, 2 = 32-bit)");
    ini.SetInteger("Window", "ColourMode", Engine.colourMode);

    ini.SetFloat("Audio", "BGMVolume", bgmVolume / (float)MAX_VOLUME);
    ini.SetFloat("Audio", "SFXVolume", sfxVolume / (float)MAX_VOLUME);

#if RETRO_USING_SDL2
    ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_Scancode)");
#endif
#if RETRO_USING_SDL1
    ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDLKeycodeLookup)");
#endif
    ini.SetComment("Keyboard 1", "IK1Comment", "Keyboard Mappings for P1 (Based on: https://wiki.libsdl.org/SDL_Scancode)");
    ini.SetInteger("Keyboard 1", "Up", inputDevice[INPUT_UP].keyMappings);
    ini.SetInteger("Keyboard 1", "Down", inputDevice[INPUT_DOWN].keyMappings);
    ini.SetInteger("Keyboard 1", "Left", inputDevice[INPUT_LEFT].keyMappings);
    ini.SetInteger("Keyboard 1", "Right", inputDevice[INPUT_RIGHT].keyMappings);
    ini.SetInteger("Keyboard 1", "A", inputDevice[INPUT_BUTTONA].keyMappings);
    ini.SetInteger("Keyboard 1", "B", inputDevice[INPUT_BUTTONB].keyMappings);
    ini.SetInteger("Keyboard 1", "C", inputDevice[INPUT_BUTTONC].keyMappings);
    ini.SetInteger("Keyboard 1", "X", inputDevice[INPUT_BUTTONX].keyMappings);
    ini.SetInteger("Keyboard 1", "Y", inputDevice[INPUT_BUTTONY].keyMappings);
    ini.SetInteger("Keyboard 1", "Z", inputDevice[INPUT_BUTTONZ].keyMappings);
    ini.SetInteger("Keyboard 1", "L", inputDevice[INPUT_BUTTONL].keyMappings);
    ini.SetInteger("Keyboard 1", "R", inputDevice[INPUT_BUTTONR].keyMappings);
    ini.SetInteger("Keyboard 1", "Start", inputDevice[INPUT_START].keyMappings);
    ini.SetInteger("Keyboard 1", "Select", inputDevice[INPUT_SELECT].keyMappings);

    ini.SetInteger("Controller 1", "Up", inputDevice[INPUT_UP].contMappings);
    ini.SetInteger("Controller 1", "Down", inputDevice[INPUT_DOWN].contMappings);
    ini.SetInteger("Controller 1", "Left", inputDevice[INPUT_LEFT].contMappings);
    ini.SetInteger("Controller 1", "Right", inputDevice[INPUT_RIGHT].contMappings);
    ini.SetInteger("Controller 1", "A", inputDevice[INPUT_BUTTONA].contMappings);
    ini.SetInteger("Controller 1", "B", inputDevice[INPUT_BUTTONB].contMappings);
    ini.SetInteger("Controller 1", "C", inputDevice[INPUT_BUTTONC].contMappings);
    ini.SetInteger("Controller 1", "X", inputDevice[INPUT_BUTTONX].contMappings);
    ini.SetInteger("Controller 1", "Y", inputDevice[INPUT_BUTTONY].contMappings);
    ini.SetInteger("Controller 1", "Z", inputDevice[INPUT_BUTTONZ].contMappings);
    ini.SetInteger("Controller 1", "L", inputDevice[INPUT_BUTTONL].contMappings);
    ini.SetInteger("Controller 1", "R", inputDevice[INPUT_BUTTONR].contMappings);
    ini.SetInteger("Controller 1", "Start", inputDevice[INPUT_START].contMappings);
    ini.SetInteger("Controller 1", "Select", inputDevice[INPUT_SELECT].contMappings);

    ini.Write(BASE_PATH"settings.ini");
}

#if RETRO_USE_MOD_LOADER
#include <string>
#if RETRO_PLATFORM == RETRO_ANDROID
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif

void initMods()
{
    modList.clear();

    char modBuf[0x100];
    sprintf(modBuf, "%smods/", modsPath);
    fs::path modPath(modBuf);

    if (fs::exists(modPath) && fs::is_directory(modPath)) {
        std::string mod_config = modPath.string() + "/modconfig.ini";
        FileIO *configFile     = fOpen(mod_config.c_str(), "r");
        if (configFile) {
            fClose(configFile);
            IniParser modConfig(mod_config.c_str(), false);

            for (int m = 0; m < modConfig.items.size(); ++m) {
                bool active = false;
                ModInfo info;
                modConfig.GetBool("mods", modConfig.items[m].key, &active);
                if (loadMod(&info, modPath.string(), modConfig.items[m].key, active))
                    modList.push_back(info);
            }
        }

        try {
            auto rdi = fs::directory_iterator(modPath);
            for (auto de : rdi) {
                if (de.is_directory()) {
                    fs::path modDirPath = de.path();

                    ModInfo info;

                    std::string modDir            = modDirPath.string().c_str();
                    const std::string mod_inifile = modDir + "/mod.ini";
                    std::string folder            = modDirPath.filename().string();

                    bool flag = true;
                    for (int m = 0; m < modList.size(); ++m) {
                        if (modList[m].folder == folder) {
                            flag = false;
                            break;
                        }
                    }

                    if (flag) {
                        if (loadMod(&info, modPath.string(), modDirPath.filename().string(), false))
                            modList.push_back(info);
                    }
                }
            }
        } catch (fs::filesystem_error fe) {
            printLog("Mods Folder Scanning Error: ");
            printLog(fe.what());
        }
    }
}

bool loadMod(ModInfo *info, std::string modsPath, std::string folder, bool active)
{
    if (!info)
        return false;

    info->fileMap.clear();
    info->name    = "";
    info->desc    = "";
    info->author  = "";
    info->version = "";
    info->folder  = "";
    info->active  = false;

    const std::string modDir = modsPath + "/" + folder;

    FileIO *f = fOpen((modDir + "/mod.ini").c_str(), "r");
    if (f) {
        fClose(f);
        IniParser modSettings((modDir + "/mod.ini").c_str(), false);

        info->name    = "Unnamed Mod";
        info->desc    = "";
        info->author  = "Unknown Author";
        info->version = "1.0.0";
        info->folder  = folder;

        char infoBuf[0x100];
        // Name
        StrCopy(infoBuf, "");
        modSettings.GetString("", "Name", infoBuf);
        if (!StrComp(infoBuf, ""))
            info->name = infoBuf;
        // Desc
        StrCopy(infoBuf, "");
        modSettings.GetString("", "Description", infoBuf);
        if (!StrComp(infoBuf, ""))
            info->desc = infoBuf;
        // Author
        StrCopy(infoBuf, "");
        modSettings.GetString("", "Author", infoBuf);
        if (!StrComp(infoBuf, ""))
            info->author = infoBuf;
        // Version
        StrCopy(infoBuf, "");
        modSettings.GetString("", "Version", infoBuf);
        if (!StrComp(infoBuf, ""))
            info->version = infoBuf;

        info->active = active;

        // Check for Data/ replacements
        fs::path dataPath(modDir + "/Data");

        if (fs::exists(dataPath) && fs::is_directory(dataPath)) {
            try {
                auto data_rdi = fs::recursive_directory_iterator(dataPath);
                for (auto data_de : data_rdi) {
                    if (data_de.is_regular_file()) {
                        char modBuf[0x100];
                        StrCopy(modBuf, data_de.path().string().c_str());
                        char folderTest[4][0x10] = {
                            "Data/",
                            "Data\\",
                            "data/",
                            "data\\",
                        };
                        int tokenPos = -1;
                        for (int i = 0; i < 4; ++i) {
                            tokenPos = FindStringToken(modBuf, folderTest[i], 1);
                            if (tokenPos >= 0)
                                break;
                        }

                        if (tokenPos >= 0) {
                            char buffer[0x80];
                            for (int i = StrLength(modBuf); i >= tokenPos; --i) {
                                buffer[i - tokenPos] = modBuf[i] == '\\' ? '/' : modBuf[i];
                            }

                            // printLog(modBuf);
                            std::string path(buffer);
                            std::string modPath(modBuf);
                            char pathLower[0x100];
                            memset(pathLower, 0, sizeof(char) * 0x100);
                            for (int c = 0; c < path.size(); ++c) {
                                pathLower[c] = tolower(path.c_str()[c]);
                            }

                            info->fileMap.insert(std::pair<std::string, std::string>(pathLower, modBuf));
                        }
                    }
                }
            } catch (fs::filesystem_error fe) {
                printLog("Data Folder Scanning Error: ");
                printLog(fe.what());
            }
        }

        // Check for Scripts/ replacements
        fs::path scriptPath(modDir + "/Scripts");

        if (fs::exists(scriptPath) && fs::is_directory(scriptPath)) {
            try {
                auto data_rdi = fs::recursive_directory_iterator(scriptPath);
                for (auto data_de : data_rdi) {
                    if (data_de.is_regular_file()) {
                        char modBuf[0x100];
                        StrCopy(modBuf, data_de.path().string().c_str());
                        char folderTest[4][0x10] = {
                            "Scripts/",
                            "Scripts\\",
                            "scripts/",
                            "scripts\\",
                        };
                        int tokenPos = -1;
                        for (int i = 0; i < 4; ++i) {
                            tokenPos = FindStringToken(modBuf, folderTest[i], 1);
                            if (tokenPos >= 0)
                                break;
                        }

                        if (tokenPos >= 0) {
                            char buffer[0x80];
                            for (int i = StrLength(modBuf); i >= tokenPos; --i) {
                                buffer[i - tokenPos] = modBuf[i] == '\\' ? '/' : modBuf[i];
                            }

                            // printLog(modBuf);
                            std::string path(buffer);
                            std::string modPath(modBuf);
                            char pathLower[0x100];
                            memset(pathLower, 0, sizeof(char) * 0x100);
                            for (int c = 0; c < path.size(); ++c) {
                                pathLower[c] = tolower(path.c_str()[c]);
                            }

                            info->fileMap.insert(std::pair<std::string, std::string>(pathLower, modBuf));
                        }
                    }
                }
            } catch (fs::filesystem_error fe) {
                printLog("Script Folder Scanning Error: ");
                printLog(fe.what());
            }
        }
        return true;
    }
    return false;
}
void saveMods()
{
    char modBuf[0x100];
    sprintf(modBuf, "%smods/", modsPath);
    fs::path modPath(modBuf);

    if (fs::exists(modPath) && fs::is_directory(modPath)) {
        std::string mod_config = modPath.string() + "/modconfig.ini";
        IniParser modConfig;

        for (int m = 0; m < modList.size(); ++m) {
            ModInfo *info = &modList[m];

            modConfig.SetBool("mods", info->folder.c_str(), info->active);
        }

        modConfig.Write(mod_config.c_str(), false);
    }
}
#endif
