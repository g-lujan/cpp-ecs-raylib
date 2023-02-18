#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include "../external/json.hpp"
#include "components.hpp"
#include "component_registry_types.hpp"



void do_serialize(unsigned long long id, nlohmann::json &output, Component_Registry_Types &registry);
void save(nlohmann::json &output);

#endif