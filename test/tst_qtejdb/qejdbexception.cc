#include "gtest/gtest.h"
#include "qtejdb/qejdbexception.h"
#include <QDebug>

TEST(QEjdbExceptionTest, TestRaise)
{
    QEjdbException ex(QEjdbException::CONNECTIONERROR, "test");
    EXPECT_THROW(ex.raise(), QEjdbException);
}
TEST(QEjdbExceptionTest, TestCode)
{
    QEjdbException ex(QEjdbException::CONNECTIONERROR, "test");
    EXPECT_EQ(QEjdbException::CONNECTIONERROR, ex.code());
}
TEST(QEjdbExceptionTest, TestMessage)
{
    QEjdbException ex(QEjdbException::CONNECTIONERROR, "test");
    EXPECT_EQ("test", ex.message());
}
TEST(QEjdbExceptionTest, TestClone)
{
    QEjdbException ex(QEjdbException::CONNECTIONERROR, "test");
    QEjdbException *ex2 = ex.clone();
    EXPECT_EQ(QEjdbException::CONNECTIONERROR, ex2->code());
    EXPECT_EQ("test", ex2->message());
}
TEST(QEjdbExceptionTest, TestQDebug)
{
    QEjdbException ex(QEjdbException::CONNECTIONERROR, "test");
    qDebug() << ex;
}
