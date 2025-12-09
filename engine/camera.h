#pragma once
#include "node.h"
#include <glm/glm.hpp>


/**
 * @class Camera
 * @brief Rappresenta una generica camera
 */
namespace Eng {
   class ENG_API Camera : public Node {
   public:
      /// @brief Costruttori, distruttori, operatori di copia e spostamento
      Camera() noexcept = default;
      virtual ~Camera() noexcept = default;
      Camera(const Camera&) = default;
      Camera& operator=(const Camera&) = default;
      Camera(Camera&&) noexcept = default;
      Camera& operator=(Camera&&) noexcept = default;

      /// @brief torna la view matrix
      glm::mat4 getViewMatrix() const noexcept;

      /// @brief varia in base al tipo di camera
      [[nodiscard]] virtual glm::mat4 getProjectionMatrix() const noexcept = 0;

      /// @brief adattare le cam in base al resize della finestra
      virtual void onResize(int width, int height) noexcept = 0;

   };
}
