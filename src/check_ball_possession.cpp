#include "oxebots_strategy/check_ball_possession.hpp"
#include <cmath>

namespace oxebots_strategy
{

CheckBallPossession::CheckBallPossession(const std::string& name, const BT::NodeConfig& config, rclcpp::Node::SharedPtr node)
  : BT::ConditionNode(name, config), node_(node)
{
  game_data_sub_ = node_->create_subscription<oxebots_interfaces::msg::GameData>(
    "/game_data", 10, std::bind(&CheckBallPossession::gameDataCallback, this, std::placeholders::_1));
  RCLCPP_INFO(node_->get_logger(), "CheckBallPossession node initialized.");
}

BT::PortsList CheckBallPossession::providedPorts()
{
  return { 
    BT::InputPort<unsigned int>("robot_id"),
    BT::InputPort<double>("threshold", 150.0, "Distance threshold in mm")
  };
}

void CheckBallPossession::gameDataCallback(const oxebots_interfaces::msg::GameData::SharedPtr msg)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  last_game_data_ = msg;
}

std::optional<oxebots_interfaces::msg::RobotGameData> CheckBallPossession::getRobotData(unsigned int robot_id) {
    if (!last_game_data_ || !last_game_data_.value()) return std::nullopt;
    for (const auto& ally : last_game_data_.value()->robots.allies) {
        if (ally.id == robot_id) return ally;
    }
    return std::nullopt;
}

BT::NodeStatus CheckBallPossession::tick()
{
  unsigned int robot_id;
  double threshold;

  if (!getInput<unsigned int>("robot_id", robot_id)) {
    RCLCPP_ERROR(node_->get_logger(), "Missing required input [robot_id] for CheckBallPossession");
    return BT::NodeStatus::FAILURE;
  }
  if (!getInput<double>("threshold", threshold)) {
    RCLCPP_ERROR(node_->get_logger(), "Missing required input [threshold] for CheckBallPossession");
    return BT::NodeStatus::FAILURE;
  }

  std::optional<oxebots_interfaces::msg::GameData::SharedPtr> current_data;
  {
    std::lock_guard<std::mutex> lock(data_mutex_);
    current_data = last_game_data_;
  }

  if (!current_data || !current_data.value()) {
    RCLCPP_WARN_THROTTLE(node_->get_logger(), *node_->get_clock(), 2000, "No game data available for CheckBallPossession.");
    return BT::NodeStatus::FAILURE;
  }

  auto robot_data = getRobotData(robot_id);
  if (!robot_data) {
    RCLCPP_WARN_THROTTLE(node_->get_logger(), *node_->get_clock(), 2000, "No data for robot %d in CheckBallPossession.", robot_id);
    return BT::NodeStatus::FAILURE;
  }

  const auto& ball_pos = current_data.value()->ball;
  double dist_to_ball = std::hypot(robot_data->x - ball_pos.x, robot_data->y - ball_pos.y);

  if (dist_to_ball < threshold) {
    //RCLCPP_INFO_THROTTLE(node_->get_logger(), *node_->get_clock(), 1000, "Robot %d has ball possession (dist: %.1fmm)", robot_id, dist_to_ball);
    return BT::NodeStatus::SUCCESS;
  }

  //RCLCPP_INFO_THROTTLE(node_->get_logger(), *node_->get_clock(), 1000, "Robot %d lost ball possession (dist: %.1fmm)", robot_id, dist_to_ball);
  return BT::NodeStatus::FAILURE;
}

} // namespace oxebots_strategy
