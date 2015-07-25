#ifndef QEJDBEXCEPTION_H
#define QEJDBEXCEPTION_H

#include <QException>
#include <QDebug>

class Q_DECL_EXPORT QEjdbException : public QException
{

public:

    enum {
        CONNECTIONERROR = 0,
        NOTCONNECTED = 1,
        DBKEYNOTEXIST = 2
    };

    QEjdbException(const int code, const QString &msg);
    ~QEjdbException() throw() {}

    void raise() const;
    QEjdbException *clone() const;



    int code() const;

    QString message() const;


private:
    int m_code;
    QString m_msg;
};


Q_DECL_EXPORT QDebug operator<<(QDebug dbg, const QEjdbException &ex);

#endif // QEJDBEXCEPTION_H
