#include "entity/Decoration.h"

namespace Rival {

std::string Decoration::getDebugName() const
{
    return "Decoration (" + std::to_string(id) + ")";
}

}  // namespace Rival
