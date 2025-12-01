#include "texture.h"
#include <GL/freeglut.h>

using namespace Eng;

/*

// --- AGGIUNGI QUESTO BLOCCO MANUALMENTE ---
// Serve perché il gl.h di Windows è troppo vecchio e non conosce queste costanti
#ifndef GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif
// ------------------------------------------
*/

Texture::Texture(const unsigned char* bitmap, const int width, const int height) noexcept
   : width_{width}, height_{height} {
   // Create and bind a texture:
   glGenTextures(1, &texId_);
   glBindTexture(GL_TEXTURE_2D, texId_);

   // Change texture settings:
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   // in teoria questo è meglio (filtro trilineare)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


   /* per filtro anisotropico
   float maxAnisotropy;
   glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
   */



   // Load texture content from a byte array:
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB,
   GL_UNSIGNED_BYTE, bitmap);

   // devo usarle da qualche parte
   // gluBuild2DMipmaps();
}


void Texture::bind() const {
   glBindTexture(GL_TEXTURE_2D, texId_);
   glEnable(GL_TEXTURE_2D);
}



Texture::~Texture() noexcept {
   // Release unused resources:
   glDeleteTextures(1, &texId_);
}


void Texture::render(const glm::mat4& parentMatrix, const glm::mat4& viewMatrix) {


}


