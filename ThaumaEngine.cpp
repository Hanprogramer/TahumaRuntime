#include "ThaumaEngine.h"
Thauma::Texture::Texture()
{
}

Thauma::Texture::~Texture()
{
    glDeleteTextures(1, &textureID);
}

int Thauma::Texture::load(string path)
{
    // Error checking
    GLenum error = GL_NO_ERROR;
    bool success = true;
    // Load the image as surface
	texture = IMG_Load(path.c_str());

    // Detect texture format
    if (texture->format->BitsPerPixel >= 4)
        mode = GL_RGBA;
    else
        mode = GL_RGB;

    // Generate teture id
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Actually create the texure
    glTexImage2D(GL_TEXTURE_2D, 0, mode, texture->w, texture->h, 0,
        mode, GL_UNSIGNED_BYTE, texture->pixels);

    // Apply some params

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //Check for error
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        success = false;
    }

    if (success) {
        is_loaded = true;
        src_rect.w = texture->w;
        src_rect.h = texture->h;
        SDL_FreeSurface(texture);
        return ERROR_OK;
    }
    else
    	return ERROR_FAILED;
}
