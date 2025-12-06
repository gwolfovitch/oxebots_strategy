// NÓ FEITO ATRAVÉS DO GPT (bem como suas adequações)

#include "oxebots_strategy/ball_to_blackboard_node.h"

#include <functional>
#include <utility>

namespace oxebots_strategy
{

BallToBlackboardNode::BallToBlackboardNode(const std::string& name, const BT::NodeConfig& config, rclcpp::Node::SharedPtr node)
  : BT::SyncActionNode(name, config), node_(std::move(node))
{
  game_data_sub_ = node_->create_subscription<oxebots_interfaces::msg::GameData>(
    "/game_data", 10, std::bind(&BallToBlackboardNode::gameDataCallback, this, std::placeholders::_1));

  RCLCPP_INFO(node_->get_logger(), "BallToBlackboardNode configurado.");
}

BT::PortsList BallToBlackboardNode::providedPorts()
{
  return {
    BT::OutputPort<double>("ball_x"),
    BT::OutputPort<double>("ball_y")
  };
}

void BallToBlackboardNode::gameDataCallback(const oxebots_interfaces::msg::GameData::SharedPtr msg)
{
  last_game_data_ = msg;
}

BT::NodeStatus BallToBlackboardNode::tick()
{
  if (!last_game_data_) {
    RCLCPP_WARN_THROTTLE(node_->get_logger(), *node_->get_clock(), 1000, "Aguardando dados da bola...");
    return BT::NodeStatus::FAILURE;
  }

  const auto& ball = last_game_data_->ball;
  setOutput("ball_x", static_cast<double>(ball.x));
  setOutput("ball_y", static_cast<double>(ball.y));

  return BT::NodeStatus::SUCCESS;
}

}  // namespace oxebots_strategy
