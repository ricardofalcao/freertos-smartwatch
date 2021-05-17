#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

typedef struct {

    char * message;

} Notification_t;

class Notifications {
    private:
        QueueHandle_t notifications_queue;

    public:
        Notifications();

        void enqueueNotification(char * message);

        bool popNotification(Notification_t * notification);
};

extern Notifications notifications;