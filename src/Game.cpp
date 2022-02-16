#include "Game.h"
GLuint VertexArrayID;
GLuint vertexBuffer;
using namespace std;
Game *Game::instance = NULL;
namespace fs = std::filesystem;
bool CheckLua(lua_State* L, int r) {
    if (r != LUA_OK) {
        // There's an error with the lua, output it
        std::string errormsg = lua_tostring(L, -1);
        std::cout << errormsg << std::endl;
        return false;
    }
    return true;
}


void Game::draw_texture(int texID, float x, float y) {
    Thauma::Texture* tex = textures.at(texID);
    glPushMatrix();
    glTranslatef(x,y, 0);
    glBindTexture(GL_TEXTURE_2D, tex->textureID);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0); glVertex2i(0, 0);
    glTexCoord2f(1, 0); glVertex2i(tex->src_rect.w, 0);
    glTexCoord2f(1, 1); glVertex2i(tex->src_rect.w, tex->src_rect.h);
    glTexCoord2f(0, 1); glVertex2i(0, tex->src_rect.h);
    glEnd();

    glPopMatrix();
}
void Game::draw_texture(int texID, float x, float y, float w, float h)
{
    Thauma::Texture* tex = textures.at(texID);
    glPushMatrix();
    //glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, tex->textureID);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0); glVertex2i(x, y);
    glTexCoord2f(1, 0); glVertex2i(x+w, y);
    glTexCoord2f(1, 1); glVertex2i(x+w, y+h);
    glTexCoord2f(0, 1); glVertex2i(x, y+h);
    glEnd();

    glPopMatrix();
}
int Game::load_texture(string path)
{
    Thauma::Texture* tex = new Thauma::Texture();
    if (tex->load(path) == Thauma::ERROR_OK) {
        // Successfuly loaded the texture
        textures.push_back(tex);
        return textures.size() - 1;
    }
    return -1;
}
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

Game::Game()
{
    instance = this;
}

Game::Game(SDL_Window* win, SDL_GLContext ctx) {
    this->window = win;
    this->context = ctx;
    instance = this;
}
void Game::load_game(string path)
{
    cout << "Loading project at " << path << endl;
    // Load the project.json file
    fs::path dir = path;
    fs::path file = "project.json";
    fs::path filepath = path / file;
    cout << filepath << endl;
    if (std::filesystem::exists(filepath)) {
        try {
            ifstream ifs(filepath);
            json j;
            ifs >> j;
            settings.MainFile = j["main"];
            settings.WrokingDirectory = path;

            // Sets the current working directory
            std::filesystem::current_path(path);
        }
        catch (exception e) {
            cout << "Error reading project.json: " << e.what() << endl;
            throw 505;
        }
    }
    else {
        cout << "Thauma: Error, cant read project.json (or doesn't exists)" << endl;
        throw 505;
    }
}
void Game::initLua(){
    std::cout << "Initializing lua scripting\n";
    L = luaL_newstate();
    luaL_openlibs(L);
    this->RegisterLuaApiFunctions(L);
    if (CheckLua(L, luaL_dofile(L, settings.MainFile.c_str()))) {
        //// Do something
        lua_getglobal(L, "Game"); // get the variable Gamae
        if (lua_istable(L, -1)) {
            LuaInitFunction = _hook_lua_gamefunction(L, "init");
            LuaDrawFunction = _hook_lua_gamefunction(L, "draw");
            LuaUpdateFunction = _hook_lua_gamefunction(L, "update");
            LuaDisposeFunction = _hook_lua_gamefunction(L, "dispose");
            LuaResizeFunction = _hook_lua_gamefunction(L, "resize");
        }
        else {
            luaL_error(L, "Thauma: Error, 'Game' isn't an object or does not exist");
        }
    }
}
Thauma::Texture* tex;
void Game::initialize(){
    textures.reserve(99);
    pressedKeys.reserve(20);
    initLua();
    _call_lua_gamefunction(L, LuaInitFunction);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

}

void Game::update(double deltaTime){
    _call_lua_gamefunction(L, LuaUpdateFunction, 0.05f * deltaTime);
}
void Game::draw(){
    _call_lua_gamefunction(L, LuaDrawFunction);
}

void Game::dispose(){
    cout << "Closing lua" << endl;
    _call_lua_gamefunction(L, LuaDisposeFunction);
    lua_close(L);
}

void Game::onevent(SDL_Event event){
    switch (event.type) {
    case SDL_KEYDOWN:
        pressedKeys.push_back(event.key.keysym.scancode);
        break;
    case SDL_KEYUP:
        pressedKeys.erase(std::remove(pressedKeys.begin(), pressedKeys.end(), event.key.keysym.scancode), pressedKeys.end());
        break;
    }
}

void Game::onresize(int w, int h)
{
    window_width = w;
    window_height = h;
    lua_pushnumber(L, w);
    lua_setglobal(L, "window_width");
    lua_pushnumber(L, h);
    lua_setglobal(L, "window_height");
    _call_lua_gamefunction(L, LuaResizeFunction, w, h);
}


void Game::RegisterLuaApiFunctions(lua_State* L) {
    lua_pushnumber(L, SDL_SCANCODE_W);
    lua_setglobal(L, "keycode_W");
    lua_pushnumber(L, SDL_SCANCODE_S);
    lua_setglobal(L, "keycode_S");
    lua_pushnumber(L, SDL_SCANCODE_A);
    lua_setglobal(L, "keycode_A");
    lua_pushnumber(L, SDL_SCANCODE_D);
    lua_setglobal(L, "keycode_D");

    lua_register(L, "set_game_title", wrap_set_game_title);
    lua_register(L, "set_window_icon", wrap_set_window_icon);
    lua_register(L, "load_texture", wrap_load_texture);
    lua_register(L, "draw_texture", wrap_draw_texture);
    lua_register(L, "is_key_pressed", wrap_is_key_pressed);
}

void Game::set_game_title(std::string title)
{
    SDL_SetWindowTitle(this->window, title.c_str());
}

void Game::set_window_icon(string path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    SDL_SetWindowIcon(window, surface);
    SDL_FreeSurface(surface);
}
