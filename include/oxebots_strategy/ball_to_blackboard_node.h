// NÓ FEITO ATRAVÉS DO GPT

#pragma once

#include "behaviortree_cpp/action_node.h"
#include "rclcpp/rclcpp.hpp"
#include "oxebots_interfaces/msg/game_data.hpp"

namespace oxebots_strategy
{

class BallToBlackboardNode : public BT::SyncActionNode
{
public:
  BallToBlackboardNode(const std::string& name, const BT::NodeConfig& config, rclcpp::Node::SharedPtr node);

  static BT::PortsList providedPorts();

  BT::NodeStatus tick() override;

private:
  void gameDataCallback(const oxebots_interfaces::msg::GameData::SharedPtr msg);

  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<oxebots_interfaces::msg::GameData>::SharedPtr game_data_sub_;

  oxebots_interfaces::msg::GameData::SharedPtr last_game_data_;
};

}  // namespace oxebots_strategy
