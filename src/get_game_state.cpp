#include "oxebots_strategy/get_game_state.hpp"

GetGameState::GetGameState(const std::string& name, const BT::NodeConfig& config, rclcpp::Node::SharedPtr node)
  : BT::SyncActionNode(name, config), node_(node)
{
  gc_sub_ = node_->create_subscription<oxebots_interfaces::msg::Referee>(
    "/gc_data", 10, std::bind(&GetGameState::gcCallback, this, std::placeholders::_1));

  game_data_sub_ = node_->create_subscription<oxebots_interfaces::msg::GameData>(
    "/game_data", 10, std::bind(&GetGameState::gameDataCallback, this, std::placeholders::_1));

  RCLCPP_INFO(node_->get_logger(), "Nó 'GetGameState' inicializado e inscrito nos tópicos.");
}

BT::PortsList GetGameState::providedPorts()
{
  return { 
    BT::OutputPort<int>("command"), 
    BT::OutputPort<double>("ball_x"),
    BT::OutputPort<double>("ball_y")
  };
}

void GetGameState::gcCallback(const oxebots_interfaces::msg::Referee::SharedPtr msg)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  last_gc_data_ = msg;
}

void GetGameState::gameDataCallback(const oxebots_interfaces::msg::GameData::SharedPtr msg)
{
  std::lock_guard<std::mutex> lock(data_mutex_);
  last_game_data_ = msg;
}

BT::NodeStatus GetGameState::tick()
{
  std::lock_guard<std::mutex> lock(data_mutex_);

  if (last_gc_data_.has_value()) {
    setOutput("command", last_gc_data_.value()->command);
  } else {
    RCLCPP_WARN_THROTTLE(node_->get_logger(), *node_->get_clock(), 2000, "Nenhum dado recebido do Game Controller ainda.");
  }

  if (last_game_data_.has_value()) {
    setOutput("ball_x", last_game_data_.value()->ball.x);
    setOutput("ball_y", last_game_data_.value()->ball.y);
  } else {
    RCLCPP_WARN_THROTTLE(node_->get_logger(), *node_->get_clock(), 2000, "Nenhum dado recebido de GameData ainda.");
  }

  return BT::NodeStatus::SUCCESS;
}
