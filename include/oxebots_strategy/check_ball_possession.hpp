#pragma once
#include "behaviortree_cpp/condition_node.h"

class CheckBallPossession : public BT::ConditionNode
{
public:
  CheckBallPossession(const std::string& name, const BT::NodeConfig& config)
    : BT::ConditionNode(name, config) {}

  static BT::PortsList providedPorts() {
    return { 
      BT::InputPort<std::string>("desired_possession"),
      BT::InputPort<int>("current_possession")
    };
  }

  BT::NodeStatus tick() override {
    // TODO: Implementar a lógica de verificação
    return BT::NodeStatus::FAILURE;
  }
};