#pragma once

#include "behaviortree_cpp/condition_node.h"
#include "rclcpp/rclcpp.hpp"
#include "oxebots_interfaces/msg/game_data.hpp"
#include <mutex>
#include <optional>

namespace oxebots_strategy
{

class CheckBallPossession : public BT::ConditionNode
{
public:
  CheckBallPossession(const std::string& name, const BT::NodeConfig& config, rclcpp::Node::SharedPtr node);

  static BT::PortsList providedPorts();
  BT::NodeStatus tick() override;

private:
  void gameDataCallback(const oxebots_interfaces::msg::GameData::SharedPtr msg);
  std::optional<oxebots_interfaces::msg::RobotGameData> getRobotData(unsigned int robot_id);

  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<oxebots_interfaces::msg::GameData>::SharedPtr game_data_sub_;
  
  std::mutex data_mutex_;
  std::optional<oxebots_interfaces::msg::GameData::SharedPtr> last_game_data_;
};

} // namespace oxebots_strategy
