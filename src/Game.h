#pragma once
#include <SDL2/SDL.h>
#include "./gl.h"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <iostream>
extern "C" {
#include "../lua542/include/lua.h";
#include "../lua542/include/lauxlib.h";
#include "../lua542/include/lualib.h";
}
#include "../ThaumaEngine.h"
#include <vector>
#include "../lib/json.hpp"
using json = nlohmann::json;
#include <fstream>

// C++ 17 feature
#include <variant>
#include <filesystem>
using Type = std::variant<int, bool, std::string, float, double>;


struct GameSettings {
    std::string Title = "My Game";
    std::string WrokingDirectory = "";
    std::string MainFile = "";
};
class Game{
    public:
        SDL_Window* window;
        SDL_GLContext context;
        Game();
        Game(SDL_Window* win, SDL_GLContext ctx);
        GameSettings settings{};
        GLuint defaultShaderProgram;
        static Game* instance;
        void initialize();
        void update(double deltaTime);
        void draw();
        void dispose();
        void onevent(SDL_Event event);
        void onresize(int w, int h);

        void RegisterLuaApiFunctions(lua_State* L);

        void load_game(string path);

        lua_State* L;
        int LuaDrawFunction = 0;
        int LuaInitFunction = 0;
        int LuaUpdateFunction = 0;
        int LuaDisposeFunction = 0;
        int LuaResizeFunction = 0;

        // Basics things
        int mouse_x = 0, mouse_y = 0;
        int window_width = 1, window_height = 1;
        int tick_speed = 1;

        vector<int> pressedKeys;

        /// <summary>
        /// All the textures inside the game will be stored here
        /// </summary>
        vector<Thauma::Texture*> textures;

        void initLua();
        
        static Game* getInstance() {
            if (instance == nullptr) {
                std::cout << "Error: Game instance is null\n";
                instance = new Game();
            }
            return instance;
        }

        /// <summary>
        /// Set the game title
        /// </summary>
        /// <param name="title">The title of the game window</param>
        void set_game_title(string title);
        void set_window_icon(string path);

        /// <summary>
        /// Draw a texture onto the screen
        /// </summary>
        /// <param name="tex">texture to draw</param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        void draw_texture(int texID, float x, float y);
        void draw_texture(int texID, float x, float y, float w, float h);

        int load_texture(string path);

        void _lua_push(lua_State* L, Type v) {
            // The function gets called here
            switch (v.index()) {
            case 0:// is an integer
                lua_pushinteger(L, std::get<int>(v));
                break;
            case 1: // is an bool
                lua_pushboolean(L, std::get<bool>(v));
                break;
            case 2: // is an string
                lua_pushstring(L, std::get<std::string>(v).c_str());
                break;
            case 3: // is an float
                lua_pushnumber(L, std::get<float>(v));
                break;
            case 4: // is a double
                lua_pushnumber(L, std::get<double>(v));
                break;
            }
        }

        bool is_key_pressed(int keycode) {
            if (std::find(pressedKeys.begin(), pressedKeys.end(), keycode) != pressedKeys.end()) {
                return true;
            }
            else {
                return false;
            }
        }

        /// <summary>
        /// Calls a simple game function with no params
        /// </summary>
        /// <returns>the success state</returns>
        bool _call_lua_gamefunction(lua_State* L, int funcid) {
            if (funcid > 0) {
                // The function gets called here
                lua_rawgeti(L, LUA_REGISTRYINDEX, funcid);
                lua_call(L, 0, 0);
                return true;
            }
            // The function isn't hooked yet, do nothing
            return false;
        }
        bool _call_lua_gamefunction(lua_State* L, int funcid, Type param1) {
            if (funcid != 0) {
                lua_rawgeti(L, LUA_REGISTRYINDEX, funcid);
                _lua_push(L, param1);
                lua_call(L, 1, 0);
                return true;
            }
            // The function isn't hooked yet, do nothing
            return false;
        }
        bool _call_lua_gamefunction(lua_State* L, int funcid, Type param1, Type param2) {
            if (funcid != 0) {
                lua_rawgeti(L, LUA_REGISTRYINDEX, funcid);
                _lua_push(L, param1);
                _lua_push(L, param2);
                lua_call(L, 2, 0);
                return true;
            }
            // The function isn't hooked yet, do nothing
            return false;
        }

        /// <summary>
        /// Must be run after getting the table
        /// Wraps a function inside an object function and get the reference
        /// </summary>
        /// <param name="L">Lua state</param>
        /// <param name="funcname">Function name to hook</param>
        /// <returns>id of the function in registry table, otherwise -1</returns>
        int _hook_lua_gamefunction(lua_State* L, string funcname) {
            int result = -1;
            // The key to look for, now is in position -1
            lua_pushstring(L, funcname.c_str());

            // Get the table (now is on -2) and get the value from string "draw"
            lua_gettable(L, -2);
            if (lua_isfunction(L, -1)) {
                // Pops it out and store it on registry
                result = luaL_ref(L, LUA_REGISTRYINDEX);
            }
            else {
                //luaL_error(L, ("Error, func '" + funcname + "' is not a function, can't be hooked").c_str());
                cout << "No game hook found for: " << funcname << ", skipping..." << endl;
                // Pop the key or the result
                lua_pop(L, 1);
            }
            return result;
        }

        static int wrap_set_game_title(lua_State* L) {
            getInstance()->set_game_title(lua_tostring(L,-1));
            return 0;
        }

        static int wrap_load_texture(lua_State* L) {
            int result = getInstance()->load_texture(lua_tostring(L, -1));
            lua_pushinteger(L, result);
            return 1; // number of results
        }

        static int wrap_draw_texture(lua_State* L) {
            int argcount = lua_gettop(L);
            if (argcount == 3) {
                // do the 2nd draw_texture
                getInstance()->draw_texture(
                    lua_tonumber(L, 1),
                    lua_tonumber(L, 2),
                    lua_tonumber(L, 3)
                );
            }
            else if (argcount == 5) {
                // do the 1st draw_texture
                getInstance()->draw_texture(
                    lua_tonumber(L, 1),
                    lua_tonumber(L, 2),
                    lua_tonumber(L, 3),
                    lua_tonumber(L, 4),
                    lua_tonumber(L, 5)
                   );
            }
            else {
                luaL_error(L, "Error: argument size does not match");
            }

            return 0;
        }

        static int wrap_set_window_icon(lua_State* L) {
            getInstance()->set_window_icon(lua_tostring(L, 1));
            return 0;
        }

        static int wrap_is_key_pressed(lua_State* L) {
            lua_pushboolean(L, getInstance()->is_key_pressed(lua_tointeger(L, 1)));
            return 1;
        }
};