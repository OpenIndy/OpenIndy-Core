#ifndef GEODETICFUNCTION_H
#define GEODETICFUNCTION_H

#include "function.h"

namespace oi{

/*!
 * \brief The GeodeticFunction class
 * for future use only
 */
class OI_CORE_EXPORT GeodeticFunction : public Function
{
    Q_OBJECT

public:
    GeodeticFunction(QObject *parent = 0) : Function(parent){}

    virtual ~GeodeticFunction(){}

protected:

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(Station &station){ return Function::exec(station); }
    bool exec(CoordinateSystem &coordinateSystem){ return Function::exec(coordinateSystem); }
    bool exec(TrafoParam &trafoParam){ return Function::exec(trafoParam); }

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

#define GEODETICFUNCTION_PREFIX de.openIndy.plugin.function.geodeticFunction.v
#define GeodeticFunction_iidd STR2(CONCAT2(GEODETICFUNCTION_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // GEODETICFUNCTION_H
