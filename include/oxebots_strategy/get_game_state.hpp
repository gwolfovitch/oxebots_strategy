#pragma once

#include "behaviortree_cpp/action_node.h"
#include "rclcpp/rclcpp.hpp"
#include "oxebots_interfaces/msg/game_data.hpp"
#include "oxebots_interfaces/msg/referee.hpp"
#include <mutex>
#include <optional>

class GetGameState : public BT::SyncActionNode
{
public:
  GetGameState(const std::string& name, const BT::NodeConfig& config, rclcpp::Node::SharedPtr node);

  static BT::PortsList providedPorts();
  BT::NodeStatus tick() override;

private:
  void gcCallback(const oxebots_interfaces::msg::Referee::SharedPtr msg);
  void gameDataCallback(const oxebots_interfaces::msg::GameData::SharedPtr msg);

  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<oxebots_interfaces::msg::Referee>::SharedPtr gc_sub_;
  rclcpp::Subscription<oxebots_interfaces::msg::GameData>::SharedPtr game_data_sub_;

  std::mutex data_mutex_;
  std::optional<oxebots_interfaces::msg::Referee::SharedPtr> last_gc_data_;
  std::optional<oxebots_interfaces::msg::GameData::SharedPtr> last_game_data_;
};
