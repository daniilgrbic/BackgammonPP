#include "chat_message.h"


auto ChatMessage::toVariant() const -> QVariant
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

auto ChatMessage::getMessage() const -> QString
{
    return message;
}

auto ChatMessage::getReceiver() const -> QString
{
    return receiver;
}

auto ChatMessage::getSender() const -> QString
{
    return sender;
}

void ChatMessage::setSender(QString s)
{
    sender = s;
}
