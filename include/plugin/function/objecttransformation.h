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

    bool exec(Station &station){ return Function::exec(station); }
    bool exec(CoordinateSystem &coordinateSystem){ return Function::exec(coordinateSystem); }

};

}

#ifndef STR
#define STR(x) #x
#endif

#define OBJECTTRANSFORMATIONFUNCTION_PREFIX de.openIndy.plugin.function.objectTransformation.v
#define ObjectTransformation_iidd STR(OBJECTTRANSFORMATIONFUNCTION_PREFIX ## PLUGIN_INTERFACE_VERSION)

#endif // OBJECTTRANSFORMATION_H
