#pragma once

#include "behaviortree_cpp/condition_node.h"
#include "oxebots_interfaces/msg/referee.hpp" // Para ter acesso às constantes de comando
#include <map>
#include <string>

class CheckCommand : public BT::ConditionNode
{
public:
  CheckCommand(const std::string& name, const BT::NodeConfig& config);

  static BT::PortsList providedPorts();
  BT::NodeStatus tick() override;

private:
  // Um mapa para converter a string do XML para o valor uint8 do comando
  std::map<std::string, uint8_t> command_map_;
};