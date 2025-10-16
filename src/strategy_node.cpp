#include "rclcpp/rclcpp.hpp"
#include "behaviortree_cpp/bt_factory.h"
#include "oxebots_strategy/go_to_point_node.h"
#include "ament_index_cpp/get_package_share_directory.hpp"
#include "oxebots_strategy/get_game_state.hpp"
#include "oxebots_strategy/check_command.hpp"
#include "oxebots_strategy/assign_roles.hpp"
#include "oxebots_strategy/check_ball_possession.hpp"
#include "oxebots_strategy/calculate_point_behind_ball.hpp"


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("strategy_node");

  // 1. Encontra o caminho de compartilhamento do seu pacote.
  std::string package_share_directory = ament_index_cpp::get_package_share_directory("oxebots_strategy");

  // 2. Define o caminho padrão para o arquivo XML.
  std::string default_tree_path = package_share_directory + "/attack_strategy.xml";

  // 3. Declara o parâmetro, permitindo que ele seja sobrescrito pelo launch file.
  node->declare_parameter<std::string>("bt_xml_path", default_tree_path);
  std::string tree_path = node->get_parameter("bt_xml_path").as_string();

  RCLCPP_INFO(node->get_logger(), "Carregando árvore de comportamento de: %s", tree_path.c_str());

  BT::BehaviorTreeFactory factory;
  //factory.registerBuiltins();
  factory.registerNodeType<oxebots_strategy::GoToPointNode>("GoToPoint", node);
  factory.registerNodeType<GetGameState>("GetGameState", node);

  // Registra os nós que não precisam do handle do ROS
  factory.registerNodeType<CheckCommand>("CheckCommand");
  factory.registerNodeType<AssignRoles>("AssignRoles");
  factory.registerNodeType<oxebots_strategy::CalculatePointBehindBall>("CalculatePointBehindBall");
  factory.registerNodeType<oxebots_strategy::CheckBallPossession>("CheckBallPossession", node);

  // 4. Cria a árvore a partir do caminho obtido pelo parâmetro.
  try
  {
    auto tree = factory.createTreeFromFile(tree_path);

    // Loop principal de execução da árvore
    rclcpp::Rate rate(10);
    while (rclcpp::ok())
    {
      tree.tickOnce();
      rclcpp::spin_some(node);
      rate.sleep();
    }
  }
  catch (const BT::RuntimeError& e)
  {
    // Captura erros se o arquivo não for encontrado ou tiver um formato inválido
    RCLCPP_ERROR(node->get_logger(), "ERRO DE EXECUÇÃO DA ÁRVORE: %s", e.what());
  }


  rclcpp::shutdown();
  return 0;
}