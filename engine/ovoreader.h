#pragma once

#include "node.h"

namespace Eng {
    ENG_API class OvoParser {
        public:
        std::shared_ptr<Node> returnCompleteSceneTree(const std::string& file_path);
    };
}