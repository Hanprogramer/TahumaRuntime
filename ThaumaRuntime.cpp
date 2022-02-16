#include "ThaumaRuntime.h"

bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		success = false;
	}

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		success = false;
	}

	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		success = false;
	}

	return success;
}
void refresh_window_viewport_size(SDL_Window* window, Game* game) {
	int w = 1, h = 1;
	SDL_GetWindowSize(window, &w, &h);
	game->onresize(w, h);
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -10, 10);
}
int main(int argc, char* argv[])
{
	if (argc < 3) {
		std::cout << "Error. Not enough argcount" << std::endl;
		return 1;
	}
	else {
		/// Command line arguments
		string cmd = argv[1];
		if (cmd == "icchange") {
			// Run the icon change command
			// arguments: *default, "icchange", <filename>, <iconfile>
			// TODO: Make this cross platform
#ifdef _WIN32
			HANDLE hResHandler; HINSTANCE inst = GetModuleHandle(NULL);
			LPCSTR filename = argv[2];
			LPCSTR iconfile = argv[3];
			LPWSTR id = MAKEINTRESOURCE(104);
			WORD langId = 0; // use default

			// Get the file size
			DWORD size = 0;
			OFSTRUCT of;
			HANDLE hFile = (HANDLE)OpenFile(iconfile, &of, OF_READ);
			GetFileSize(hFile, &size);
			CloseHandle(hFile);

			//// Load the image first
			HICON image;
			image = (HICON)LoadImageA(inst, iconfile, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
			if (image)
				cout << "Icon successfully loaded" << endl;
			else
				cout << "Error, failed to load the icon file: " << iconfile << endl;


			// Open the file for resource reading and writing
			hResHandler = BeginUpdateResourceA(filename, FALSE);
			if (hResHandler == NULL)
			{
				cout << "Could not open file for writing." << endl;
				return 1;
			}


			BOOL result = UpdateResource(
				hResHandler, // Resource handler
				RT_GROUP_ICON, // Resource type icon
				MAKEINTRESOURCE(104), // Resource ID
				1033, // Language ID 
				image, // Image data
				size // Filesize
			);
			if (result == FALSE)
			{
				cout << "Could not add resource." << endl;
				return 1;
			}
			EndUpdateResource(hResHandler, FALSE);

			//HANDLE hResHandler; HINSTANCE inst = GetModuleHandle(NULL);
			//const char* filename = argv[2];
			//LPCSTR iconfile = argv[3];
			//HGLOBAL hRes; // handle to loaded resource
			//HMODULE hExe; // handle to existing .EXE file
			//HANDLE hUpdateRes; // update resource handle
			//BOOL result;

			//hExe = LoadLibraryA(filename);
			//if (hExe == NULL)
			//{
			//	printf("Error loading executable\n");
			//	return -1;
			//}
			//hRes = FindResource(hExe, MAKEINTRESOURCE("#IDI_ICON"), RT_ICON);
			//if (hRes == NULL)
			//{
			//	printf("Could not locate icon.\n");
			//	return -1;
			//}


			//HGLOBAL hResLoad;   // handle to loaded resource
			//HMODULE hExe;       // handle to existing .EXE file
			//HRSRC hRes;         // handle/ptr. to res. info. in hExe
			//HANDLE hUpdateRes;  // update resource handle
			//LPVOID lpResLock;   // pointer to resource data
			//BOOL result;

			//hExe = LoadLibrary(TEXT("app.exe"));
			//if (hExe == NULL)
			//{
			//	return;
			//}

			//// Locate the icon resource in the .EXE file.
			//hRes = FindResource(hExe, MAKEINTRESOURCE(104), RT_GROUP_ICON);
			//if (hRes == NULL)
			//{
			//	return;
			//}

			//// Load the resource into global memory.
			//hResLoad = LoadResource(hExe, hRes);
			//if (hResLoad == NULL)
			//{
			//	return;
			//}

			//// Lock the resourcex into global memory.
			//lpResLock = LockResource(hResLoad);
			//if (lpResLock == NULL)
			//{
			//	return;
			//}

			//// Open the file to which you want to add the icon resource.
			//hUpdateRes = BeginUpdateResource(TEXT("app_updated.exe"), FALSE);
			//if (hUpdateRes == NULL)
			//{
			//	return;
			//}

			//result = UpdateResource(hUpdateRes,    // update resource handle
			//	RT_GROUP_ICON,                         // change icon resource
			//	MAKEINTRESOURCE(1),         // resource id
			//	MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),  // english
			//	lpResLock,                         // ptr to resource info
			//	SizeofResource(hExe, hRes));       // size of resource info

			//if (result == FALSE)
			//{
			//	return;
			//}

			//// Write changes to app_updated.exe and then close it.
			//if (!EndUpdateResource(hUpdateRes, FALSE))
			//{
			//	return;
			//}

			//// Clean up.
			//if (!FreeLibrary(hExe))
			//{
			//	return;
			//}
#endif

			// Quit the application
			cout << "Icon resource changed succesfully" << endl;
			return 0;
		}
	}

	SDL_Init(SDL_INIT_VIDEO);
	//Use OpenGL 2.0
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_Window* window = SDL_CreateWindow("Made With ThaumaEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_Rect dstrect = { 5, 5, 320, 240 };
	Game* game = NULL;
	bool success = true;
	if (context == NULL)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize OpenGL
		if (!initGL())
		{
			printf("Unable to initialize OpenGL!\n");
			success = false;
		}
		game = new Game(window, context);
		// Load Shader
		game->defaultShaderProgram = CompileDefaultShaders();
		game->load_game(argv[2]);
		game->initialize();
		refresh_window_viewport_size(window, game);
	}

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{

		_clock_LAST = _clock_NOW;
		_clock_NOW = SDL_GetPerformanceCounter();

		_clock_deltaTime = (double)((_clock_NOW - _clock_LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

		game->update(_clock_deltaTime);
		// Handle the event
		while (SDL_PollEvent(&event) != 0)
		{
			//User requests quit
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			//Handle keypress with current mouse position
			else if (event.type == SDL_TEXTINPUT)
			{
				int x = 0, y = 0;
				SDL_GetMouseState(&x, &y);
				// handleKeys( e.text.text[ 0 ], x, y );
			}
			else if (event.type == SDL_WINDOWEVENT && (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)) {
				refresh_window_viewport_size(window, game);
			}
			game->onevent(event);
		}

		//Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		game->draw();

		SDL_GL_SwapWindow(window);
	}
	game->dispose();
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}