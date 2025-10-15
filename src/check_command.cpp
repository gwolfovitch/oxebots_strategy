#include "oxebots_strategy/check_command.hpp"

CheckCommand::CheckCommand(const std::string& name, const BT::NodeConfig& config)
  : BT::ConditionNode(name, config)
{
  // Preenche o mapa com os comandos que vamos usar
  // Use as constantes da mensagem Referee para evitar erros.
  command_map_["HALT"] = oxebots_interfaces::msg::Referee::COMMAND_HALT;
  command_map_["STOP"] = oxebots_interfaces::msg::Referee::COMMAND_STOP;
  command_map_["NORMAL_START"] = oxebots_interfaces::msg::Referee::COMMAND_NORMAL_START;
  command_map_["FORCE_START"] = oxebots_interfaces::msg::Referee::COMMAND_FORCE_START;
  command_map_["PREPARE_KICKOFF_YELLOW"] = oxebots_interfaces::msg::Referee::COMMAND_PREPARE_KICKOFF_YELLOW;
  command_map_["PREPARE_KICKOFF_BLUE"] = oxebots_interfaces::msg::Referee::COMMAND_PREPARE_KICKOFF_BLUE;
  // Adicione outros comandos conforme necessário
}

BT::PortsList CheckCommand::providedPorts()
{
  return { 
    BT::InputPort<int>("current_command"),
    BT::InputPort<std::string>("command_to_check")
  };
}

BT::NodeStatus CheckCommand::tick()
{
  auto expected_cmd_str = getInput<std::string>("command_to_check");
  auto current_cmd = getInput<int>("current_command");

  // Se qualquer uma das portas de entrada não estiver setada, retorna falha.
  if (!expected_cmd_str || !current_cmd) {
      return BT::NodeStatus::FAILURE;
  }

  // Procura o comando em string no nosso mapa
  auto it = command_map_.find(expected_cmd_str.value());
  if (it == command_map_.end()) {
    // Se o comando do XML não for válido, retorna falha
    return BT::NodeStatus::FAILURE;
  }

  // Compara o comando atual (do blackboard) com o esperado
  if (current_cmd.value() == it->second) {
    return BT::NodeStatus::SUCCESS;
  }
  
  return BT::NodeStatus::FAILURE;
}