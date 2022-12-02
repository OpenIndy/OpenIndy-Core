#include <QString>
#include <QtTest>
#include <QPointer>
#include <QList>

#include "sensorconfiguration.h"

#define COMPARE_DOUBLE(actual, expected, threshold) QVERIFY2(std::abs(actual-expected)< threshold, QString("actual: %1, expected: %2").arg(actual).arg(expected).toLatin1().data());

using namespace oi;

class SensorConfigTest : public QObject
{
    Q_OBJECT

public:
    SensorConfigTest();

private Q_SLOTS:
    void testWriteRead();

private:

};

SensorConfigTest::SensorConfigTest()
{
}

void SensorConfigTest::testWriteRead(){

    SensorConfiguration sc;
    QMap<QString, QString> sp;
    sp["a p"] = "1.5";
    sc.setStringParameter(sp);
    QMultiMap<QString, QString> asp;
    asp.insert("a p", "1.5");
    asp.insert("a p", "7/8");
    asp.insert("a p", "0.5");
    sc.setAvailableStringParameter(asp);

    QDomDocument xml("sensorConfig");
    QDomElement root = sc.toOpenIndyXML(xml);
    xml.appendChild(root);

    SensorConfiguration scFromXml;
    scFromXml.fromOpenIndyXML(xml.documentElement());

    QVERIFY(scFromXml.getStringParameter()["a p"] == "1.5");
    QList<QString> values = scFromXml.getAvailableStringParameter().values("a p");
    QVERIFY( values.at(0) == "1.5");
    QVERIFY( values.at(1) == "7/8");
    QVERIFY( values.at(2) == "0.5");
}

QTEST_APPLESS_MAIN(SensorConfigTest)

#include "tst_sensorconfig.moc"
