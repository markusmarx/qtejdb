#include "qejdbexception.h"

QEjdbException::QEjdbException(int code, const QString &msg):m_code(code), m_msg(msg)
{
}

void QEjdbException::raise() const
{
    throw *this;
}

QEjdbException *QEjdbException::clone() const {
    return new QEjdbException(*this);
}

int QEjdbException::code() const
{
    return m_code;
}

QString QEjdbException::message() const
{
    return m_msg;
}


QDebug operator<<(QDebug dbg, const QEjdbException &ex)
{
    dbg.nospace() << "QEjdbException[ errorCode = " << ex.code() << ", message = " << ex.message() << ")";
    return dbg.space();
}
