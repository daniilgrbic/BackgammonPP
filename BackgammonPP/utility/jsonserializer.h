#pragma once

#include "serializable.h"

#include <QJsonDocument>
#include <string>

template <class T>
concept IsSerializable = std::is_base_of<Serializable, T>::value;

template <IsSerializable T> class JSONSerializer
{
public:
    JSONSerializer() = default;

    static std::string toJson(const T &serializable)
    {
        return QJsonDocument::fromVariant(serializable.toVariant()).toJson(QJsonDocument::JsonFormat::Compact).toStdString();
    }

    static T fromJson(std::string &json)
    {
        QVariant data = QJsonDocument::fromJson(QByteArray::fromStdString(json)).toVariant();
        T result;
        result.fromVariant(data);
        return result;
    }
};
