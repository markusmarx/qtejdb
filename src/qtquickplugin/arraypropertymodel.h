#ifndef ARRAYPROPERTYMODEL_H
#define ARRAYPROPERTYMODEL_H
#include "basemodel.h"
#include <QQmlParserStatus>

class ArrayPropertyModel: public BaseModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString arrayProperty READ arrayProperty WRITE setArrayProperty NOTIFY arrayPropertyChanged)
    Q_PROPERTY(QString propertyCollection READ propertyCollection WRITE setPropertyCollection NOTIFY propertyCollectionChanged)
    Q_PROPERTY(QJSValue parentObject READ parentObject WRITE setParentObject NOTIFY parentObjectChanged)


    QString m_arrayProperty;

    QString m_propertyCollection;

QJSValue m_parentObject;

public:
    ArrayPropertyModel(QObject *parent = 0);

    void classBegin();
    void componentComplete();
    QString arrayProperty() const;

    QString propertyCollection() const;

    QJSValue parentObject() const;

public slots:
    void connected();
    void disconnected();
    void setArrayProperty(QString arrayProperty);
    void setPropertyCollection(QString propertyCollection);

    void setParentObject(QJSValue parentObject);

signals:
    void arrayPropertyChanged(QString arrayProperty);
    void propertyCollectionChanged(QString propertyCollection);
    void parentObjectChanged(QBsonObject parentObject);
protected:
    bool checkProperties();
    void tryInit();
};

#endif // ARRAYPROPERTYMODEL_H
