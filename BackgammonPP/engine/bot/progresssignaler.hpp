#ifndef PROGRESSSIGNALER_HPP
#define PROGRESSSIGNALER_HPP
#include <QObject>

class ProgressSignaler : public QObject
{
    Q_OBJECT

public:
    ProgressSignaler();

signals:
    void updateLocalProgress();
    void updateGlobalProgress(int value);
    void updateLocalRange(int lower, int upper);
    void done();
    void sendText(QString string);
};

#endif // PROGRESSSIGNALER_HPP
