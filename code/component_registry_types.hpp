#ifndef _COMPONENT_REGISTRY_TYPES_H_
#define _COMPONENT_REGISTRY_TYPES_H_

#include <variant>
#include "components.hpp"

using Component_Registry_Types = std::variant<Component_Registry<Player>,
                                              Component_Registry<Health>,
                                              Component_Registry<Kinematics>,
                                              Component_Registry<Collider>,
                                              Component_Registry<Anim>,
                                              Component_Registry<View>,
                                              Component_Registry<Input>,
                                              Component_Registry<AI>,
                                              Component_Registry<Tile>>;

#endif