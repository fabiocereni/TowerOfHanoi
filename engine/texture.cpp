#include "texture.h"
#include <GL/freeglut.h>

using namespace Eng;

Texture::Texture(const unsigned char* bitmap, const int width, const int height, const int numberOfChannels) noexcept
    : width_{ width }, height_{ height }, numberOfChannels_{ numberOfChannels }
{

    /// @brief generazione texture
    glGenTextures(1, &texId_);
    glBindTexture(GL_TEXTURE_2D, texId_);

    /// @brief parametrizzazione texture

    /// @details attiva il repeat orizzontalmente
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    /// @details attiva il repeat verticalmente
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /// @details attiva il filtraggio trilineare per la magnificazione
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    /// @details attiva il filtro trilineare per la minimizzazione
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    /// @brief costruzione MIPMAP
    /// @details se 3 canali attiva RGB
    if (numberOfChannels_ == 3) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width_, height_, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap);

    /// @details altrimenti attiva RGBA
    } else {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width_, height_, GL_BGRA_EXT, GL_UNSIGNED_BYTE, bitmap);
    }

}

/// @brief distruttore
/// @details elimina la texture associata
Texture::~Texture() noexcept {
    glDeleteTextures(1, &texId_);
}

/**
 * @brief Si occupa di eseguire il binding della texture
 */
void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texId_);
    glEnable(GL_TEXTURE_2D);
}


/// @brief La texture non renderizza niente da sola
void Texture::render(const glm::mat4&) {}