#include "qejdbdatabase.h"
#include <QDebug>
#include <QDir>

QEJDBDatabase::QEJDBDatabase(): m_db(0)
{
}

QEJDBDatabase::~QEJDBDatabase()
{
    close();
}

bool QEJDBDatabase::open(QString path, QString databaseName, int mode)
{
    m_dbPath = QDir(path);
    m_dbName = databaseName;
    if (isOpen()) close();
    m_db = ejdbnew();

    bool res = ejdbopen(m_db, m_dbPath.absoluteFilePath(databaseName).toLatin1(), mode);

    if (!res) return false;

    // load collections
    loadCollections();

    return true;
}

bool QEJDBDatabase::close()
{
    bool res;

    if (!isOpen()) return false;

    if (!ejdbclose(m_db)) res = false;
    ejdbdel(m_db);

    return res;
}

bool QEJDBDatabase::isOpen()
{
    return m_db && ejdbisopen(m_db);
}

bool QEJDBDatabase::remove()
{

    close();
    // set filter for databasefiles and remove
    QStringList filter;
    filter << m_dbName + "*";
    m_dbPath.setNameFilters(filter);
    QFileInfoList files = m_dbPath.entryInfoList(QDir::Files);

    foreach(QFileInfo f, files) {
        if (f.isFile()) {
            QFile(f.absoluteFilePath()).remove();
        }
    }
    return true;
}

QEJDBCollection QEJDBDatabase::collection(QString collectionName)
{
    if (m_collections.contains(collectionName)) {
        QSharedPointer<QEJDBCollection> ptr = m_collections.value(collectionName);
        return *ptr;
    }


    EJCOLL *col = ejdbgetcoll(m_db, collectionName.toLatin1());

    if (!col) {
        throw 1;
    }

    return storeCollection(col, collectionName);

}

QEJDBCollection QEJDBDatabase::createCollection(QString collectionName)
{
    EJCOLL *col = ejdbcreatecoll(m_db, collectionName.toLatin1(), NULL);
    return storeCollection(col, collectionName);
}

QEJDBCollection QEJDBDatabase::storeCollection(EJCOLL *col, QString collectionName)
{
    QSharedPointer<QEJDBCollection> ptr;
    if (m_collections.contains(collectionName)) {
        //todo exception handling

    } else{
        ptr = QSharedPointer<QEJDBCollection>(new QEJDBCollection(m_db, col, collectionName));
        m_collections.insert(collectionName, ptr);
    }
    return *ptr;
}

void QEJDBDatabase::loadCollections()
{
   //todo
}
