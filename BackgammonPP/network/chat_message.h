#pragma once

#include <QString>
#include "utility/serializable.h"


class ChatMessage : public Serializable {
public:
    ChatMessage(QString m = "", QString r = "")
        : message{m}
        , receiver{r}
        {};

    QString getMessage() const;
    QString getReceiver() const;
    QString getSender() const;

    void setSender(QString);

    QVariant toVariant() const override;
    void fromVariant(const QVariant &variant) override;

private:
    QString message;
    QString receiver;
    QString sender;
};

