#ifndef FEATUREATTRIBUTES_H
#define FEATUREATTRIBUTES_H

#include <QString>
#include <QPair>

#include "types.h"

namespace oi{

/*!
 * \brief The FeatureAttributes class
 * Helper class that is used when creating new features to specify their attributes
 */
class OI_CORE_EXPORT FeatureAttributes
{
public:
    FeatureAttributes(){
        this->count = 0;
        this->isActual = false;
        this->isNominal = false;
        this->isCommon = false;
        this->isBundleSystem = false;
        this->isBundleTrafo = false;
    }

    //general attributes
    FeatureTypes typeOfFeature;
    QString name;
    QString group;
    int count;

    //only when typeOfFeature specifies a geometry
    bool isActual;
    bool isNominal;
    bool isCommon;
    QString nominalSystem;
    QString mConfig;

    //function
    QPair<QString, QString> functionPlugin; //T1 = function, T2 = file path

    //only when typeOfFeature specifies a trafo param
    QString startSystem;
    QString destinationSystem;
    bool isBundleTrafo;

    //only when typeOfFeature specifies a coordinate system
    bool isBundleSystem;

};

}

#endif // FEATUREATTRIBUTES_H
