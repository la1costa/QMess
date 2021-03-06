#pragma once

#include <QObject>
#include <QTcpSocket>

#include "modelsAPI.h"

class EventHandler : public QObject
{
    Q_OBJECT
public:
    explicit EventHandler(QObject *parent = nullptr);
    void handleHistoryRequest();
    void handleMessage(Message data);
    void handleUserStatus(UserStatus data);
signals:
    void notifyReciever(QByteArray data, int recieverId);
    void notifySender(QByteArray data, int senderId);
public slots:

};
