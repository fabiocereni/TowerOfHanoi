#pragma once
#include "node.h"
#include <glm/glm.hpp>

namespace Eng {
   class ENG_API Camera : public Node {
   public:
      Camera() noexcept = default;
      virtual ~Camera() noexcept = default;

      // camera non copiabile (dovremmo copiare tutta la gerarchia)
      Camera(const Camera&) = delete;
      Camera& operator=(const Camera&) = delete;

      // camera spostabile (sposta solo "l'ownership" del puntatore)
      Camera(Camera&&) noexcept = default;
      Camera& operator=(Camera&&) noexcept = default;

      // la view non dipende dalla proiezione
      glm::mat4 getViewMatrix() const noexcept;

      // deve essere implementata in base al tipo di camera
      [[nodiscard]] virtual glm::mat4 getProjectionMatrix() const noexcept = 0;


   };

}
