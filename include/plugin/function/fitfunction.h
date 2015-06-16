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

#ifdef PLUGIN_INTERFACE_VERSION
#define FitFunction_iidd "de.openIndy.plugin.function.fitFunction.v" PLUGIN_INTERFACE_VERSION
#endif

#endif // FITFUNCTION_H