#include "qsignalmock.h"

#include <QtCore/QDebug>

namespace
{
    struct MockedSignal
    {
        MockedSignal(const QByteArray& normalisedSignalSignature, int linkedTofakeSlotId,
                     QSignalMock::QSignalToMockDiverter* signalToMockDiverter);

        QByteArray normalisedSignature;
        int linkedTofakeSlotId;
        QSignalMock::QSignalToMockDiverter* signalToMockDiverter;
    };

    // I can't think of a way of registering the created QSignalToMockDiverters with their containing
    // QSignalMock-derivative while the derivative is being constructed: the only thing I can think of
    // is for them to register with a global list and then, once the QSignalMock-derivative is fully
    // constructed, use this global list to associate them with the QSignalMock and then clear this global
    // list for use with the next QSignalMock-derivative that gets constructed.
    // This is why no more than one QSignalMock-derivative can be in the process of construction at once:
    // they'd trample all over each other's tempMockedSignalsForMockUnderConstruction.
    QList<MockedSignal> tempMockedSignalsForMockUnderConstruction;
    void beginMockedSignalListForMockUnderConstruction();
    void registerMockedSignalForMockUnderConstruction(const char* signalName, const char* signalArgs, QSignalMock::QSignalToMockDiverter* signalToMockDiverter);
    bool isMockedSignalAlreadyRegistered(const QByteArray& normalizedSignalSignature);


    class QSignalReceiverProxy : public QObject
    {
    public:
        QSignalReceiverProxy(const QList<MockedSignal>& mockedSignals, QSignalMock *parentMock, QObject* objectToMonitor);
        virtual ~QSignalReceiverProxy();
        virtual int qt_metacall(QMetaObject::Call call, int slotId,
                                void **arguments);
    private:
        QList<MockedSignal> m_mockedSignals;
        QSignalMock *m_parentMock;
        QObject *m_objectToMonitor;

        void connectAllMockedSignalsToTheirFakeSlot();
    };
}

QSignalMock::QSignalMock(QObject* objectToMonitor)
    : m_objectToMonitor(objectToMonitor), m_signalReceiverProxy(0)
{
    beginMockedSignalListForMockUnderConstruction();
}

QSignalMock::~QSignalMock()
{
    delete m_signalReceiverProxy;
}

void QSignalMock::initMockedSignalReceiverProxy()
{
    m_signalReceiverProxy = new QSignalReceiverProxy(tempMockedSignalsForMockUnderConstruction, this, m_objectToMonitor);
    // initMockedSignalReceiverProxy() is only called when the derivative of QSignalMock is fully
    // constructed: we can now clear tempMockedSignalsForMockUnderConstruction.
    tempMockedSignalsForMockUnderConstruction.clear();
}

namespace
{
    void beginMockedSignalListForMockUnderConstruction()
    {
        Q_ASSERT(tempMockedSignalsForMockUnderConstruction.isEmpty() &&
                 "Either an internal error, or you are building multiple QSignalMocks in separate threads!");
    }

    void registerMockedSignalForMockUnderConstruction(const char* signalName, const char* signalArgs, QSignalMock::QSignalToMockDiverter* signalToMockDiverter)
    {
        const QString signalSignature = QString(signalName) + QString(signalArgs);
        const QByteArray normalizedSignalSignature = QMetaObject::normalizedSignature(signalSignature.toLocal8Bit());

        // No one would normally request the same signal twice, but let's be
        // defensive, anyway.
        if (isMockedSignalAlreadyRegistered(normalizedSignalSignature))
        {
            // We've connected this signal already!
            qWarning() << "Already registered interest for signal " << signalSignature << " - ignoring duplicate request!";
            return;
        }

        const int newFakeSlotId = tempMockedSignalsForMockUnderConstruction.size();
        MockedSignal newMockedSignal(normalizedSignalSignature, newFakeSlotId, signalToMockDiverter);
        tempMockedSignalsForMockUnderConstruction.append(newMockedSignal);
    }

    bool isMockedSignalAlreadyRegistered(const QByteArray& normalizedSignalSignature)
    {
        foreach(const MockedSignal& existingMockedSignal, tempMockedSignalsForMockUnderConstruction)
        {
            if (existingMockedSignal.normalisedSignature == normalizedSignalSignature)
            {
                return true;
            }
        }
        return false;
    }
}


QSignalMock::QSignalToMockDiverter::QSignalToMockDiverter(const char* signalName, const char* signalArgs)
{
    registerMockedSignalForMockUnderConstruction(signalName, signalArgs, this);
};

QSignalReceiverProxy::QSignalReceiverProxy(const QList< MockedSignal >& mockedSignals, QSignalMock* parentMock, QObject* objectToMonitor)
    :   QObject(0),
        m_mockedSignals(mockedSignals),
        m_parentMock(parentMock),
        m_objectToMonitor(objectToMonitor)
{
    connectAllMockedSignalsToTheirFakeSlot();
}

MockedSignal::MockedSignal(const QByteArray& normalisedSignalSignature, int linkedTofakeSlotId, QSignalMock::QSignalToMockDiverter* signalToMockDiverter)
            :   normalisedSignature(normalisedSignalSignature),
                linkedTofakeSlotId(linkedTofakeSlotId),
                signalToMockDiverter(signalToMockDiverter)
{

}

QSignalReceiverProxy::~QSignalReceiverProxy()
{

}

int QSignalReceiverProxy::qt_metacall(QMetaObject::Call call, int slotId, void** arguments)
{
    // This mechanism heavily inspired by http://doc.qt.nokia.com/qq/qq16-dynamicqobject.html:
    // see there for more details!
    // The call to our parents qt_metacall (amongst other things) adjusts the original slotId so
    // that it is relative to the slots defined for this object, excluding those in the parent
    // (QObject) part: essentially, it "undoes" the "+ QObject::metaObject()->methodCount()" we
    // added when we connected our fake slot in connectAllMockedSignalsToTheirFakeSlot()
    slotId = QObject::qt_metacall(call, slotId, arguments);
    if (slotId == -1 || call != QMetaObject::InvokeMetaMethod)
        return slotId;

    foreach(const MockedSignal& mockedSignal, m_mockedSignals)
    {
        if (mockedSignal.linkedTofakeSlotId == slotId)
        {
            mockedSignal.signalToMockDiverter->forwardSignalToMock(m_parentMock, arguments);
            return -1;
        }
    }
    Q_ASSERT(false && "Couldn't find fake slot for id -  this shouldn't happen!");
    return slotId;
}

void QSignalReceiverProxy::connectAllMockedSignalsToTheirFakeSlot()
{
    // This mechanism heavily inspired by http://doc.qt.nokia.com/qq/qq16-dynamicqobject.html:
    // see there for more details! The author's name sounds like that of a James Bond supervillain.
    foreach(const MockedSignal& mockedSignal, m_mockedSignals)
    {
        const int mockedSignalId = m_objectToMonitor->metaObject()->indexOfSignal(mockedSignal.normalisedSignature);
        if (mockedSignalId == -1)
        {
            // Non-existent signal, apparently.  TODO - Maybe we should EXPECT_NE(mockedSignalId, -1) ... ?
            qWarning() << "Could not find signal " << mockedSignal.normalisedSignature << " in QObject: " << m_objectToMonitor;
            continue;
        }

        const bool connectSuccessful = QMetaObject::connect(m_objectToMonitor, mockedSignalId, this,
                                                            // The "+ QObject::metaObject()->methodCount()" ensures
                                                            // that the slot id does not overlap with the
                                                            // slot ids of any slots in the parent (QObject) class.
                                                            mockedSignal.linkedTofakeSlotId + QObject::metaObject()->methodCount());

        if (!connectSuccessful)
        {
            qWarning() << "Could not connect signal " << mockedSignal.normalisedSignature << " to dummy slot: don't know why, though!";
        }
    }
}
