#include "../../include/infrastructure/ConsoleNotificationService.h"
#include <iostream>

void ConsoleNotificationService::notify(const std::string &message)
{
  std::cout << "[Notification] " << message << "\n";
}
