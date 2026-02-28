#ifndef CONSOLE_NOTIFICATION_SERVICE_H
#define CONSOLE_NOTIFICATION_SERVICE_H

#include "../application/Interface/INotificationService.h"

class ConsoleNotificationService : public INotificationService
{
public:
  void notify(const std::string &message) override;
};

#endif // CONSOLE_NOTIFICATION_SERVICE_H
