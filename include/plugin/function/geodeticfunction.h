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

#ifdef PLUGIN_INTERFACE_VERSION
#define GeodeticFunction_iidd "de.openIndy.plugin.function.geodeticFunction.v" PLUGIN_INTERFACE_VERSION
#endif

#endif // GEODETICFUNCTION_H
