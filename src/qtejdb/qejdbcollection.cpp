#include "qejdbcollection.h"
#include "qejdbcollection_p.h"
#include "qejdbdatabase_p.h"

/*!
 * \class QEjdbCollection
 * \ingroup qtejdb-database
 * \inmodule qtejdb
 * \since 1.0
 * \brief QEjdbCollection provide a collection with methods to call database
 * funcitons.
 */

/*!
 * \fn QEjdbCollection::QEjdbCollection(const QEjdbCollection &other)
 * Copy constructor.
 */
QEjdbCollection::QEjdbCollection(const QEjdbCollection &other):d(other.d)
{
}

/*!
 * \fn QEjdbCollection::exist()
 *
 * Returns \c true if collection exist otherwise \c false.
 */
bool QEjdbCollection::exist()
{
    return d->exist();
}

/*!
 * \fn QEjdbCollection::create()
 *
 * Creates the collection and return the result.
 */
bool QEjdbCollection::create()
{
    return d->create();
}
/*!
 * \fn QEjdbCollection::drop()
 *
 * Removes a collection from database and return the result.
 */
bool QEjdbCollection::drop()
{
    return d->drop();
}

/*!
 * \fn QEjdbCollection::load(QBsonOid oid)
 *
 * Loads a bson by \a oid from the collection and return the result. If no bson
 * found a empty object is returned.
 */
QBsonObject QEjdbCollection::load(QBsonOid oid)
{
    return d->load(oid);
}

/*!
 * \fn QEjdbCollection::save(QBsonObject &object)
 *
 * Saves the bson \a object in the collection and return the result. If the
 * bson is saved the \a object have an oid.
 */
bool QEjdbCollection::save(QBsonObject &object)
{
    return d->save(object);
}

/*!
 * \fn QEjdbCollection::remove(QBsonOid oid)
 *
 * Remove the bson by \a oid and return the result.
 */
bool QEjdbCollection::remove(QBsonOid oid)
{
    return d->remove(oid);
}

/*!
 * \brief QEjdbCollection::query(QBsonObject query, QBsonObject hints)
 *
 * Queries a collection an return the result. For \a query and \a hints
 * documentation see \l http://ejdb.org/doc/ql/ql.html.
 */
QEjdbResult QEjdbCollection::query(QBsonObject query, QBsonObject hints)
{
    return d->query(query, hints);
}

//
// QEjdbCollectionPrivate
//

bool QEjdbCollectionPrivate::exist()
{
    if (isDbValid()) {
        return m_db.data()->containsCollection(m_collection);
    }
    return false;
}


bool QEjdbCollectionPrivate::create()
{
    return isDbValid() && m_db.data()->createCollection(m_collection);
}


bool QEjdbCollectionPrivate::drop()
{
    return isDbValid() && m_db.data()->removeCollection(m_collection);
}

QBsonObject QEjdbCollectionPrivate::load(QBsonOid oid)
{
    return isDbValid() ? m_db.data()->m_worker->load(m_collection, oid) : QBsonObject();
}

bool QEjdbCollectionPrivate::save(QBsonObject &object)
{
    return isDbValid() && m_db.data()->m_worker->save(m_collection, object);
}

bool QEjdbCollectionPrivate::remove(QBsonOid oid)
{
    return isDbValid() && m_db.data()->m_worker->remove(m_collection, oid);
}

QEjdbResult QEjdbCollectionPrivate::query(QBsonObject query, QBsonObject hints)
{
    return isDbValid() ? m_db.data()->m_worker->query(m_collection, query, hints) : QEjdbResult();
}

QEjdbCollectionPrivate::QEjdbCollectionPrivate(QString collection, QSharedPointer<QEjdbDatabasePrivate> db)
    :m_collection(collection), m_db(db)
{

}

bool QEjdbCollectionPrivate::isDbValid() const
{
    try {
        return !m_db.isNull() && m_db.data()->isOpen();
    } catch (QEjdbException &e) {
        return false;
    }
}


