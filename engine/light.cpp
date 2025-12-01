#include "light.h"

using namespace Eng;

Light::Light(bool smoothShading, int lightNumber, const glm::vec4& ambient,
                     const glm::vec4& diffuse, const glm::vec4& specular,
                     bool ambientEnabled, bool localViewerEnabled)
    : smoothShading_{smoothShading}, lightNumber_{lightNumber}, ambient_{ambient},
      diffuse_{diffuse}, specular_{specular}, ambientEnabled_{ambientEnabled},
      localViewerEnabled_{localViewerEnabled} {}


void Light::render(const glm::mat4& parentMatrix, const glm::mat4& viewMatrix) {}
