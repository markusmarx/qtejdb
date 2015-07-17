#ifndef QSIGNALMOCK_H
#define QSIGNALMOCK_H

#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/preprocessor/cat.hpp>

class QObject;

/**
 * Helper class to use Google Mock's MOCK_METHODx + EXPECT_CALL etc mechanisms
 * in order to verify that a QObject emits the correct number of signals with
 * the correct parameters.
 *
 * Usage example:
 *
 * Say we have a class called Downloader, which can emit a signal
 *
 *  completed(successful bool, const QByteArray& downloadedData)
 *
 * and say that we want to test that it emits a completed signal once with
 * "successful" equal to "true", and any "downloadedData" (we don't care).
 *
 * We would first declare our downloader signal mock by doing something
 * along the lines of:
 *
 * class DownloaderSignalSpy : public QSignalMock
 * {
 * public:
 *  BEGIN_MOCK_SIGNALS(DownloaderSignalSpy)
 *
 *  MOCK_SIGNAL2(completed, (bool, const QByteArray&));
 * };
 *
 * and then we would fill in the expectations as per usual:
 *
 * Downloader downloader(...);
 * DownloaderSignalSpy downloaderSignalSpy(&downloader);
 *
 * EXPECT_CALL(downloaderSignalSpy, completed(true, _));
 *
 * ... start the downloader off ...
 *
 * So in general, we:
 *
 * - Derive a class (MySignalSpy, say) from QSignalMock that will EXPECT signals from a provided QObject (q, say);
 * - Add BEGIN_MOCK_SIGNALS(MySignalSpy) as the top line;
 * - For each signal emitted by q that we want to spy on:
 *   - Add a MOCK_SIGNALX to MySignalSpy with the signal name and the signals parameter list in brackets.
 *
 * Then, in our test, construct a MySignalSpy taking a pointer to q in its constructor, and set the EXPECT_CALLs
 * appropriately: EXPECT_CALL(methodname, argmatchers) expects q to emit the *signal* named methodname with
 * arguments matching the argmatchers.  Alternatively, EXPECT_SIGNAL may be used instead: this simply forwards
 * its arguments to EXPECT_CALL, but more precisely describes our expectations.
 *
 * Drawbacks:
 *
 * Currently, all EXPECTed signals must be emitted before the QSignalMock is destroyed.  A more usable approach,
 * where a timeout can be provided to catch asynchrous signals but where we don't necessarily have to wait for the
 * full timeout if the mocks expectations become satisfied, will require changes to Google Mock: see
 *
 *  http://code.google.com/p/googlemock/issues/detail?id=155
 *
 * Custom constructors and destructors for MySignalSpy are not allowed.
 *
 * Since during construction of any derivative of QSignalMock some global data is manipulated, constructing two
 * such objects simultaneously will lead to Undefined Behaviour.  However, since mocks are generally created in
 * test bodies, and Google Test runs all tests in the same thread, this should be a non-issue in practice.
 *
 * Signals emitted by q that are not registered via MOCK_SIGNALx are ignored completely, which may or may not be
 * what you want.
 * @author Simon St James
 */

class QSignalMock
{
public:
    virtual ~QSignalMock();

    /**
     * Internal - do not use!
     */
    class QSignalToMockDiverter
    {
    public:
        QSignalToMockDiverter(const char* signalName, const char* signalArgs);
        virtual ~QSignalToMockDiverter() {};
        virtual void forwardSignalToMock(QSignalMock* mockObject, void** signalArgs) = 0;
    };
protected:
    /**
     * Internal - do not use!
     */
    explicit QSignalMock(QObject* objectToMonitor);
    /**
     * Internal - do not use!
     */
    void initMockedSignalReceiverProxy();
private:
    QObject* m_objectToMonitor;
    QObject* m_signalReceiverProxy;
};

// Private macro helpers - do not use!
#define QSIGNALMOCKINTERNAL_DECLARE_AND_DEFINE_P_ARGUMENT_N(SIGNAL_ARGS, N)\
typedef boost::function_traits<void SIGNAL_ARGS>::arg##N##_type arg##N##Type;\
typedef boost::add_pointer<arg##N##Type>::type pointerToArg##N##Type;\
pointerToArg##N##Type pArg##N = reinterpret_cast<pointerToArg##N##Type>(signalArgs[N]);

#define QSIGNALMOCKINTERNAL_FILL_IN_UP_TO_SIGNAL_FORWARDING_BLOCK(SIGNAL_NAME, SIGNAL_ARGS)\
class BOOST_PP_CAT(B, __LINE__) : public QSignalMock::QSignalToMockDiverter\
{\
public:\
    BOOST_PP_CAT(B, __LINE__)() : QSignalToMockDiverter(#SIGNAL_NAME, #SIGNAL_ARGS) {};\
    virtual void forwardSignalToMock(QSignalMock* mockObject, void** signalArgs)\
    {\

    #define INTERNAL_FILL_IN_AFTER_SIGNAL_FORWARDING_BLOCK\
    }\
    } BOOST_PP_CAT(b, __LINE__);


// Public macros.
#define BEGIN_MOCK_SIGNALS(MockedClassName)\
MockedClassName(QObject* objectToMonitor) : QSignalMock(objectToMonitor) { initMockedSignalReceiverProxy();};\
~MockedClassName() {  };\
typedef MockedClassName mocked_type;

#define MOCK_SIGNAL0(SIGNAL_NAME, SIGNAL_ARGS) MOCK_METHOD0(SIGNAL_NAME, void SIGNAL_ARGS);\
    QSIGNALMOCKINTERNAL_FILL_IN_UP_TO_SIGNAL_FORWARDING_BLOCK(SIGNAL_NAME, SIGNAL_ARGS)\
        (void)signalArgs;\
        dynamic_cast<mocked_type*>(mockObject)->SIGNAL_NAME();\
    INTERNAL_FILL_IN_AFTER_SIGNAL_FORWARDING_BLOCK


#define MOCK_SIGNAL1(SIGNAL_NAME, SIGNAL_ARGS) MOCK_METHOD1(SIGNAL_NAME, void SIGNAL_ARGS);\
    QSIGNALMOCKINTERNAL_FILL_IN_UP_TO_SIGNAL_FORWARDING_BLOCK(SIGNAL_NAME, SIGNAL_ARGS)\
        QSIGNALMOCKINTERNAL_DECLARE_AND_DEFINE_P_ARGUMENT_N(SIGNAL_ARGS, 1);\
        dynamic_cast<mocked_type*>(mockObject)->SIGNAL_NAME(*pArg1);\
    INTERNAL_FILL_IN_AFTER_SIGNAL_FORWARDING_BLOCK


#define MOCK_SIGNAL2(SIGNAL_NAME, SIGNAL_ARGS) MOCK_METHOD2(SIGNAL_NAME, void SIGNAL_ARGS);\
    QSIGNALMOCKINTERNAL_FILL_IN_UP_TO_SIGNAL_FORWARDING_BLOCK(SIGNAL_NAME, SIGNAL_ARGS)\
        QSIGNALMOCKINTERNAL_DECLARE_AND_DEFINE_P_ARGUMENT_N(SIGNAL_ARGS, 1);\
        QSIGNALMOCKINTERNAL_DECLARE_AND_DEFINE_P_ARGUMENT_N(SIGNAL_ARGS, 2);\
        dynamic_cast<mocked_type*>(mockObject)->SIGNAL_NAME(*pArg1, *pArg2);\
    INTERNAL_FILL_IN_AFTER_SIGNAL_FORWARDING_BLOCK

#define MOCK_SIGNAL3(SIGNAL_NAME, SIGNAL_ARGS) MOCK_METHOD3(SIGNAL_NAME, void SIGNAL_ARGS);\
    QSIGNALMOCKINTERNAL_FILL_IN_UP_TO_SIGNAL_FORWARDING_BLOCK(SIGNAL_NAME, SIGNAL_ARGS)\
        QSIGNALMOCKINTERNAL_DECLARE_AND_DEFINE_P_ARGUMENT_N(SIGNAL_ARGS, 1);\
        QSIGNALMOCKINTERNAL_DECLARE_AND_DEFINE_P_ARGUMENT_N(SIGNAL_ARGS, 2);\
        QSIGNALMOCKINTERNAL_DECLARE_AND_DEFINE_P_ARGUMENT_N(SIGNAL_ARGS, 3);\
        dynamic_cast<mocked_type*>(mockObject)->SIGNAL_NAME(*pArg1, *pArg2, *pArg3);\
    INTERNAL_FILL_IN_AFTER_SIGNAL_FORWARDING_BLOCK

#define EXPECT_SIGNAL(SIGNAL_NAME, SIGNAL_ARGS) EXPECT_CALL(SIGNAL_NAME, SIGNAL_ARGS)

#endif
