#pragma once
#include "object.h"


/**
 * @class Texture
 * @brief Rappresenta una generica texture
 */
namespace Eng {
    class ENG_API Texture final : public Object {

    public:
        /// @brief Costruttori, distruttore, operatori di copia e spostamento

        /**
         * @brief Costruttore della texture
         */
        Texture(const unsigned char* bitmap, int width, int height, int numberOfChannels) noexcept;

        ~Texture() noexcept override;
        Texture(const Texture& other) = default;
        Texture(Texture&& other) noexcept = default;
        Texture& operator=(const Texture& other) = default;
        Texture& operator=(Texture&& other) noexcept = default;

        void render(const glm::mat4& modelViewMatrix) override;

        void bind() const;

    protected:
        /// @brief id univoco della texture
        unsigned int texId_;
        /// @brief Larghezza della texture
        int width_;
        /// @brief Altezza della texture
        int height_;
        /// @brief Numero di canali da utilizzare
        int numberOfChannels_;
    };
}