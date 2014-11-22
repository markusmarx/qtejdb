#include "qejdbexception.h"

QEjdbException::QEjdbException(int code, const QString &msg):m_code(code), m_msg(msg)
{
}


QDebug operator<<(QDebug dbg, const QEjdbException &ex)
{
    dbg.nospace() << "QEjdbException[ errorCode = " << ex.code() << ", message = " << ex.message() << ")";
    return dbg.space();
}
