#ifndef CONSTRUCTFUNCTION_H
#define CONSTRUCTFUNCTION_H

#include "function.h"

namespace oi{

/*!
 * \brief The ConstructFunction class
 * Function that solves geometries by constructing them from other elements
 */
class OI_CORE_EXPORT ConstructFunction : public Function
{
    Q_OBJECT

public:
    ConstructFunction(QObject *parent = 0) : Function(parent){}

    virtual ~ConstructFunction(){}

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

#define CONSTRUCTFUNCTION_PREFIX de.openIndy.plugin.function.constructFunction.v
#define ConstructFunction_iidd STR(CONSTRUCTFUNCTION_PREFIX ## PLUGIN_INTERFACE_VERSION)

#endif // CONSTRUCTFUNCTION_H
