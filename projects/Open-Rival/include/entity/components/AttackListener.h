#pragma once

namespace Rival {

/**
 * Interface used to listen to attacks.
 */
class AttackListener
{
public:
    /** Called when an entity starts attacking. */
    virtual void onAttackStarted() = 0;

    /** Called when an entity finishes an attack. */
    virtual void onAttackFinished() = 0;
};

}  // namespace Rival
