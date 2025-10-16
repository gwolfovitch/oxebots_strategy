#include "oxebots_strategy/calculate_point_behind_ball.hpp"
#include <cmath>

namespace oxebots_strategy
{

CalculatePointBehindBall::CalculatePointBehindBall(const std::string& name, const BT::NodeConfig& config)
  : BT::SyncActionNode(name, config)
{}

BT::PortsList CalculatePointBehindBall::providedPorts()
{
  return {
    BT::InputPort<double>("ball_x"),
    BT::InputPort<double>("ball_y"),
    BT::InputPort<double>("target_x"),
    BT::InputPort<double>("target_y"),
    BT::InputPort<double>("distance", 200.0, "Distance behind the ball in mm"),
    BT::OutputPort<double>("behind_x"),
    BT::OutputPort<double>("behind_y")
  };
}

BT::NodeStatus CalculatePointBehindBall::tick()
{
  double ball_x, ball_y, target_x, target_y, distance;

  if (!getInput<double>("ball_x", ball_x) || !getInput<double>("ball_y", ball_y) ||
      !getInput<double>("target_x", target_x) || !getInput<double>("target_y", target_y) ||
      !getInput<double>("distance", distance)) {
    return BT::NodeStatus::FAILURE;
  }

  double vec_x = target_x - ball_x;
  double vec_y = target_y - ball_y;

  double vec_mag = std::hypot(vec_x, vec_y);

  if (vec_mag < 1e-6) { // Ball is already at the target, no valid direction
    return BT::NodeStatus::FAILURE;
  }

  // Normalize the vector
  double norm_vec_x = vec_x / vec_mag;
  double norm_vec_y = vec_y / vec_mag;

  // Calculate the point behind the ball
  double behind_x = ball_x - norm_vec_x * distance;
  double behind_y = ball_y - norm_vec_y * distance;

  setOutput("behind_x", behind_x);
  setOutput("behind_y", behind_y);

  return BT::NodeStatus::SUCCESS;
}

} // namespace oxebots_strategy
