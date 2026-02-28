#ifndef INOTIFICATION_SERVICE_H
#define INOTIFICATION_SERVICE_H

#include <string>

class INotificationService
{
public:
  virtual void notify(const std::string &message) = 0;
  
  virtual ~INotificationService() = default;
};

#endif