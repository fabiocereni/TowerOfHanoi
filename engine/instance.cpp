#include "instance.h"

using namespace Eng;

/**
 * @brief Costruttore di instance
 * @param node
 * @param worldMatrix
 */
Instance::Instance(const std::shared_ptr<Node>& node, const glm::mat4& worldMatrix)
               : node_ptr_{node}, nodeWorldMatrix_{worldMatrix} {}
