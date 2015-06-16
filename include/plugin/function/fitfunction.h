#ifndef FITFUNCTION_H
#define FITFUNCTION_H

#include "function.h"

namespace oi{

/*!
 * \brief The FitFunction class
 * Function that solves geometries by fitting them using observations
 */
class OI_CORE_EXPORT FitFunction : public Function
{
    Q_OBJECT

public:
    FitFunction(QObject *parent = 0) : Function(parent){

        //needed elements are only observations
        NeededElement element;
        element.typeOfElement = eObservationElement;
        element.description = QString("Select as many observations as you want to be used to fit the target geometry.");
        element.infinite = true;
        this->neededElements.clear();
        this->neededElements.append(element);

    }

    virtual ~FitFunction(){}

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

#define FITFUNCTION_PREFIX de.openIndy.plugin.function.fitFunction.v
#define FitFunction_iidd STR2(CONCAT2(FITFUNCTION_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // FITFUNCTION_H
