#pragma once

using Component_Registry_Types = std::variant<Component_Registry<Player>,
                                              Component_Registry<Health>,
                                              Component_Registry<Kinematics>,
                                              Component_Registry<Collider>,
                                              Component_Registry<Anim>,
                                              Component_Registry<View>,
                                              Component_Registry<Input>,
                                              Component_Registry<AI>,
                                              Component_Registry<Tile>>;
