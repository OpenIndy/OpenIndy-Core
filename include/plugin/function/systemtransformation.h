#ifndef SYSTEMTRANSFORMATION_H
#define SYSTEMTRANSFORMATION_H

#include "function.h"

class FeatureUpdater;

namespace oi{

/*!
 * \brief The SystemTransformation class
 * Functions that solves transformation parameters between two coordinate systems (station-station, part-part, alignments)
 */
class OI_CORE_EXPORT SystemTransformation : public Function
{
    friend class ::FeatureUpdater;
    Q_OBJECT

public:
    SystemTransformation(QObject *parent = 0) : Function(parent){

        //only applicable for trafo params
        this->applicableFor.clear();
        this->applicableFor.append(eTrafoParamFeature);

    }

    virtual ~SystemTransformation(){}

    void setInputPoint(const QList<Point> &startSystem, const QList<Point> &destinationSystem){
        this->inputPointsStartSystem = startSystem;
        this->inputPointsDestinationSystem = destinationSystem;
    }

    void setInputElements(const QMap<int, QList<InputElement> > &startSystem, const QMap<int, QList<InputElement> > &destinationSystem){
        this->inputElementsStartSystem = startSystem;
        this->inputElementsDestinationSystem = destinationSystem;
    }

protected:

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(Station &station, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(station, ctx); }
    bool exec(CoordinateSystem &coordinateSystem, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(coordinateSystem, ctx); }

    bool exec(Circle &circle, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(circle, ctx); }
    bool exec(Cone &cone, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(cone, ctx); }
    bool exec(Cylinder &cylinder, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(cylinder, ctx); }
    bool exec(Ellipse &ellipse, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(ellipse, ctx); }
    bool exec(Ellipsoid &ellipsoid, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(ellipsoid, ctx); }
    bool exec(Hyperboloid &hyperboloid, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(hyperboloid, ctx); }
    bool exec(Line &line, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(line, ctx); }
    bool exec(Nurbs &nurbs, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(nurbs); }
    bool exec(Paraboloid &paraboloid, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(paraboloid, ctx); }
    bool exec(Plane &plane, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(plane, ctx); }
    bool exec(Point &point, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(point, ctx); }
    bool exec(PointCloud &pointCloud, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(pointCloud, ctx); }
    bool exec(ScalarEntityAngle &angle, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(angle, ctx); }
    bool exec(ScalarEntityDistance &distance, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(distance, ctx); }
    bool exec(ScalarEntityMeasurementSeries &measurementSeries, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(measurementSeries, ctx); }
    bool exec(ScalarEntityTemperature &temperature, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(temperature, ctx); }
    bool exec(SlottedHole &slottedHole, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(slottedHole, ctx); }
    bool exec(Sphere &sphere, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(sphere, ctx); }
    bool exec(Torus &torus, const FunctionContext &ctx = FunctionContext{}){ return Function::exec(torus, ctx); }

    //special attributes for system transformations (normal transformations)
    QList<Point> inputPointsStartSystem; //input elements solved in start system
    QList<Point> inputPointsDestinationSystem; //input elements solved in destination system

    //special attributes for system transformations (alignments)
    QMap<int, QList<InputElement> > inputElementsStartSystem; //input elements solved in start system
    QMap<int, QList<InputElement> > inputElementsDestinationSystem; //input elements solved in destination system

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

#define SYSTEMTRANSFORMATIONFUNCTION_PREFIX de.openIndy.plugin.function.systemTransformation.v
#define SystemTransformation_iidd STR2(CONCAT2(SYSTEMTRANSFORMATIONFUNCTION_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // SYSTEMTRANSFORMATION_H
