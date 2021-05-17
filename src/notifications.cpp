#include "notifications.h"

Notifications::Notifications() {
    notifications_queue = xQueueCreate(10, sizeof(Notification_t));
}

void Notifications::enqueueNotification(char * message) {
    size_t len = strlen(message) + 1;
    char * copy = (char *) pvPortMalloc(len);

    if (copy) {
        memcpy(copy, message, len);
    }

    Notification_t notification = {
        .message = message
    };

    xQueueSend(notifications_queue, &notification, portMAX_DELAY);
}

bool Notifications::popNotification(Notification_t * notification) {
    return xQueueReceive(notifications_queue, &notification, 0) == pdTRUE;
}