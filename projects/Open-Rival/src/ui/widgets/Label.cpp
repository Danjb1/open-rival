#include "ui/widgets/Label.h"

namespace Rival {

Label::Label(const TextSpan& span, const TextProperties& properties)
{
    setText(span, properties);
}

void Label::setText(const TextSpan& span, const TextProperties& properties)
{
    setVisual(TextVisual { span, properties });
}

}  // namespace Rival
