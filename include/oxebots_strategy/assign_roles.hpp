#pragma once
#include "behaviortree_cpp/action_node.h"

class AssignRoles : public BT::SyncActionNode
{
public:
  AssignRoles(const std::string& name, const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return {
      BT::OutputPort<int>("attacker_id"),
      BT::OutputPort<int>("defender_id")
    };
  }

  BT::NodeStatus tick() override {
    // TODO: Implementar a lógica de atribuição de papéis
    // Ex: setOutput("attacker_id", 1); setOutput("defender_id", 2);
    return BT::NodeStatus::SUCCESS;
  }
};