#ifndef OBJECTTRANSFORMATION_H
#define OBJECTTRANSFORMATION_H

#include "function.h"

namespace oi{

/*!
 * \brief The ObjectTransformation class
 * Function that changes a previously solved feature by using other elements
 */
class OI_CORE_EXPORT ObjectTransformation : public Function
{
    Q_OBJECT

public:
    ObjectTransformation(QObject *parent = 0) : Function(parent){}

    virtual ~ObjectTransformation(){}

protected:

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(Station &station, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(station, ctx); }
    bool exec(CoordinateSystem &coordinateSystem, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(coordinateSystem, ctx); }

};

}

#ifndef STR
#define STR(x) #x
#endif
#ifndef STR2
#define STR2(x) STR(x)
#endif

#ifndef CONCAT
#define CONCAT(a, b) a ## b
#endif
#ifndef CONCAT2
#define CONCAT2(a, b) CONCAT(a, b)
#endif

#define OBJECTTRANSFORMATIONFUNCTION_PREFIX de.openIndy.plugin.function.objectTransformation.v
#define ObjectTransformation_iidd STR2(CONCAT2(OBJECTTRANSFORMATIONFUNCTION_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // OBJECTTRANSFORMATION_H
