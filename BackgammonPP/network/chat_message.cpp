#include "chat_message.h"


QVariant ChatMessage::toVariant() const
{
    QVariantMap map;
    map.insert("m", message);
    map.insert("r", receiver);

    return map;
}

void ChatMessage::fromVariant(const QVariant &variant)
{
    QVariantMap map = variant.toMap();

    message = map.value("m").toString();
    receiver = map.value("r").toString();
}

QString ChatMessage::getMessage() const
{
    return message;
}

QString ChatMessage::getReceiver() const
{
    return receiver;
}

QString ChatMessage::getSender() const
{
    return sender;
}

void ChatMessage::setSender(QString s)
{
    sender = s;
}
