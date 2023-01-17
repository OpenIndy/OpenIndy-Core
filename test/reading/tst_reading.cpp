#include <QString>
#include <QtTest>

#include "reading.h"

using namespace oi;

class ReadingTest : public QObject
{
    Q_OBJECT

public:
    ReadingTest();

private Q_SLOTS:
    void testCartesian6D();

private:

};

ReadingTest::ReadingTest()
{
}


void ReadingTest::testCartesian6D(){


}

QTEST_APPLESS_MAIN(ReadingTest)

#include "tst_reading.moc"
