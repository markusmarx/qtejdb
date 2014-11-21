#ifndef QEJDBEXCEPTION_H
#define QEJDBEXCEPTION_H

#include <QException>
#include <QDebug>

class QEjdbException : public QException
{

public:

    enum {
        CONNECTIONERROR = 0,
        NOTCONNECTED = 1
    };

    QEjdbException(const int code, const char *msg);
    void raise() const { throw *this; }
    QEjdbException *clone() const { return new QEjdbException(*this); }

    int code() const
    {
        return m_code;
    }

    const char *message() const
    {
        return m_msg;
    }


private:
    int m_code;
    const char *m_msg;
};


QDebug operator<<(QDebug dbg, const QEjdbException &ex);

#endif // QEJDBEXCEPTION_H
