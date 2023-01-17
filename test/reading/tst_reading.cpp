#include <QString>
#include <QtTest>

#include "chooselalib.h"
#include "reading.h"

#define COMPARE_DOUBLE(actual, expected, threshold) QVERIFY2(std::abs(actual-expected)< threshold, QString("actual: %1, expected: %2").arg(actual).arg(expected).toLatin1().data());

using namespace oi;

class ReadingTest : public QObject
{
    Q_OBJECT

public:
    ReadingTest();

private Q_SLOTS:
    void initTestCase();
    void testCartesian6D();
    void testCartesian();
    void testDistanceReading();
    void testPolarReading();
    void testDirectionReading();
    void testTemperatureReading();
    void testLevelReading();

private:
    QPointer<Reading> writeReadXml(QPointer<Reading> r);
};

void ReadingTest::initTestCase() {
    ChooseLALib::setLinearAlgebra(ChooseLALib::Armadillo);
}

ReadingTest::ReadingTest()
{
}

QPointer<Reading> ReadingTest::writeReadXml(QPointer<Reading> r) {

    QDomDocument root("test");
    QDomElement xml = r->toOpenIndyXML(root);
    root.appendChild(xml);

    QTemporaryFile file;
    // file.setAutoRemove(false);
    file.open();
    qDebug() << file.fileName();

    QTextStream out(&file);
    root.save(out, 4);
    file.close();

    QDomDocument inXml;
    inXml.setContent(&file);

    ReadingUndefined u;
    QPointer<Reading> inReading = new Reading(u);
    inReading->fromOpenIndyXML(inXml.firstChild().toElement());
    return inReading;
}

void ReadingTest::testCartesian6D(){

    ReadingCartesian6D reading;
    reading.isValid = true;
    reading.xyz.setAt(0, 1.);
    reading.xyz.setAt(1, 2.);
    reading.xyz.setAt(2, 3.);
    reading.ijk.setAt(0, 0.1);
    reading.ijk.setAt(1, 0.9);
    reading.ijk.setAt(2, 0.2);

    QPointer<Reading> outReading = new Reading(reading);
    QPointer<Reading> inReading = this->writeReadXml(outReading);

    ReadingCartesian6D in = inReading->getCartesianReading6D();

    QVERIFY(inReading->getTypeOfReading() == eCartesianReading6D);
    QVERIFY(in.isValid);
    COMPARE_DOUBLE(in.xyz.getAt(0), 1., 0.01);
    COMPARE_DOUBLE(in.xyz.getAt(1), 2., 0.01);
    COMPARE_DOUBLE(in.xyz.getAt(2), 3., 0.01);
    COMPARE_DOUBLE(in.ijk.getAt(0), .1, 0.01);
    COMPARE_DOUBLE(in.ijk.getAt(1), .9, 0.01);
    COMPARE_DOUBLE(in.ijk.getAt(2), .2, 0.01);

}

void ReadingTest::testCartesian(){

    ReadingCartesian reading;
    reading.isValid = true;
    reading.xyz.setAt(0, 1.);
    reading.xyz.setAt(1, 2.);
    reading.xyz.setAt(2, 3.);;

    QPointer<Reading> outReading = new Reading(reading);
    QPointer<Reading> inReading = this->writeReadXml(outReading);

    ReadingCartesian in = inReading->getCartesianReading();

    QVERIFY(inReading->getTypeOfReading() == eCartesianReading);
    QVERIFY(in.isValid);
    COMPARE_DOUBLE(in.xyz.getAt(0), 1., 0.01);
    COMPARE_DOUBLE(in.xyz.getAt(1), 2., 0.01);
    COMPARE_DOUBLE(in.xyz.getAt(2), 3., 0.01);

}

void ReadingTest::testDistanceReading(){

    ReadingDistance reading;
    reading.isValid = true;
    reading.distance = 123.4;

    QPointer<Reading> outReading = new Reading(reading);
    QPointer<Reading> inReading = this->writeReadXml(outReading);

    ReadingDistance in = inReading->getDistanceReading();

    QVERIFY(inReading->getTypeOfReading() == eDistanceReading);
    QVERIFY(in.isValid);
    COMPARE_DOUBLE(in.distance, 123.4, 0.01);

}

void ReadingTest::testPolarReading(){

    ReadingPolar reading;
    reading.isValid = true;
    reading.azimuth = 12.2;
    reading.zenith = 23.3;
    reading.distance = 56.8;

    QPointer<Reading> outReading = new Reading(reading);
    QPointer<Reading> inReading = this->writeReadXml(outReading);

    ReadingPolar in = inReading->getPolarReading();

    QVERIFY(inReading->getTypeOfReading() == ePolarReading);
    QVERIFY(in.isValid);
    COMPARE_DOUBLE(in.azimuth, 12.2, 0.01);
    COMPARE_DOUBLE(in.zenith, 23.3, 0.01);
    COMPARE_DOUBLE(in.distance, 56.8, 0.01);

}

void ReadingTest::testDirectionReading(){

    ReadingDirection reading;
    reading.isValid = true;
    reading.azimuth = 12.2;
    reading.zenith = 23.3;

    QPointer<Reading> outReading = new Reading(reading);
    QPointer<Reading> inReading = this->writeReadXml(outReading);

    ReadingDirection in = inReading->getDirectionReading();

    QVERIFY(inReading->getTypeOfReading() == eDirectionReading);
    QVERIFY(in.isValid);
    COMPARE_DOUBLE(in.azimuth, 12.2, 0.01);
    COMPARE_DOUBLE(in.zenith, 23.3, 0.01);

}

void ReadingTest::testTemperatureReading(){

    ReadingTemperature reading;
    reading.isValid = true;
    reading.temperature = 27.1;

    QPointer<Reading> outReading = new Reading(reading);
    QPointer<Reading> inReading = this->writeReadXml(outReading);

    ReadingTemperature in = inReading->getTemperatureReading();

    QVERIFY(inReading->getTypeOfReading() == eTemperatureReading);
    QVERIFY(in.isValid);
    COMPARE_DOUBLE(in.temperature, 27.1, 0.01);

}

void ReadingTest::testLevelReading(){

    ReadingLevel reading;
    reading.isValid = true;
    reading.i = 0.1;
    reading.j = 0.9;
    reading.k = 0.2;

    QPointer<Reading> outReading = new Reading(reading);
    QPointer<Reading> inReading = this->writeReadXml(outReading);

    ReadingLevel in = inReading->getLevelReading();

    QVERIFY(inReading->getTypeOfReading() == eLevelReading);
    QVERIFY(in.isValid);
    COMPARE_DOUBLE(in.i, .1, 0.01);
    COMPARE_DOUBLE(in.j, .9, 0.01);
    COMPARE_DOUBLE(in.k, .2, 0.01);

}

QTEST_APPLESS_MAIN(ReadingTest)

#include "tst_reading.moc"
