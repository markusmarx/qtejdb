#include "qtejdb/qejdbworker.h"
#include "gtest/gtest.h"

TEST(QEjdbWorkerTest, TestCreateFromFileUrl)
{
    QEjdbWorker *worker = QEjdbWorker::createFromUrl(QUrl("file:test.db"), 0);
    QEjdbFileWorker *fileWorker = dynamic_cast<QEjdbFileWorker*>(worker);
    EXPECT_TRUE(fileWorker != 0);
}
