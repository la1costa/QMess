#include "runnabledirector.h"
#include "models.h"
#include "Handlers/eventhandler.h"

#include <QDataStream>

RunnableDirector::RunnableDirector(QObject *parent) : QObject(parent), socket(nullptr)
{

}

void RunnableDirector::setSocket(QAbstractSocket *socket)
{
    this->socket = socket;
}

void RunnableDirector::run()
{
    if(socket != nullptr)
    {
        int m_CurrentMessage = eMessage;
        QByteArray data = socket->readAll();

        qDebug() << "TcpDataProvider::getData size " << data.size();
        QDataStream ds(&data, QIODevice::ReadWrite);
        ds.setVersion(QDataStream::Qt_5_11);
        EventHandler handler;
        while(!ds.atEnd())
        {
            ds >> m_CurrentMessage;

            switch(m_CurrentMessage)
            {
                case eMessage:
                {
                    Message incommingMess;
                    ds >> incommingMess;
                    connect(&handler,SIGNAL(finish(QTcpSocket *, QByteArray)),
                                this, SLOT(forward(QTcpSocket *, QByteArray)), Qt::ConnectionType::QueuedConnection);
                    handler.handleMessage(incommingMess);
                }
                break;

                case eUserStatus:
                {
                    UserStatus userStat;
                    ds >> userStat;

                    handler.handleUserStatus(userStat);
                }
                break;

                case eCurrentUserResponse:
                {
                    UserStatus userStat;
                    ds >> userStat;

                }
                break;
                case  eMessageHistoryRequest:
                default:
                break;
            }
        }
    }
}

void RunnableDirector::notifyRecieverProvider(QByteArray data, size_t recieverId)
{
    emit notifyReciever(data, recieverId);
}

void RunnableDirector::notifySenderProvider(QByteArray data, size_t senderId)
{
    emit notifySender(data, senderId);
}

void RunnableDirector::forward(QTcpSocket *socket, QByteArray data)
{
    emit finished(socket, data);
}
