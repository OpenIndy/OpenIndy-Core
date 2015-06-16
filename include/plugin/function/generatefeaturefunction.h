#ifndef GENERATEFEATUREFUNCTION_H
#define GENERATEFEATUREFUNCTION_H

#include "function.h"

namespace oi{

/*!
 * \brief The GenerateFeatureFunction class
 * Function that uses its input elements to generate one or more new feature
 */
class OI_CORE_EXPORT GenerateFeatureFunction : public Function
{
    Q_OBJECT

public:
    GenerateFeatureFunction(QObject *parent = 0) : Function(parent){}

    virtual ~GenerateFeatureFunction(){}

signals:
    void addFeature(const QPointer<FeatureWrapper> &feature);
    void addFeatures(const QList<QPointer<FeatureWrapper> > &features);

};

}

#ifdef PLUGIN_INTERFACE_VERSION
#define GenerateFeatureFunction_iidd "de.openIndy.plugin.function.generateFeatureFunction.v" PLUGIN_INTERFACE_VERSION
#endif

#endif // GENERATEFEATUREFUNCTION_H