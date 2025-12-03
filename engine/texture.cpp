#include "texture.h"
#include <GL/freeglut.h>
#include <GL/glu.h> // NECESSARIO per gluBuild2DMipmaps

using namespace Eng;

Texture::Texture(const unsigned char* bitmap, const int width, const int height) noexcept
   : width_{width}, height_{height}
{

   // generazione texture
   glGenTextures(1, &texId_);
   glBindTexture(GL_TEXTURE_2D, texId_);

   // parametrizzazione texture
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // filtro trilineare
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

   // costruzione MIPMAP
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width_, height_,GL_RGB,GL_UNSIGNED_BYTE, bitmap
   );
}

void Texture::bind() const {
   glBindTexture(GL_TEXTURE_2D, texId_);
   glEnable(GL_TEXTURE_2D); // fixed pipeline
}

Texture::~Texture() noexcept {
   glDeleteTextures(1, &texId_);
}

// La texture non renderizza niente da sola
void Texture::render(const glm::mat4&) {}
