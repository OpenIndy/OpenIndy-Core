#ifndef FITFUNCTION_H
#define FITFUNCTION_H

#include "function.h"

namespace oi{

struct IdPoint {
    int id;
    OiVec xyz;
};
inline bool operator==(const IdPoint& p1, const IdPoint& p2){
    return p1.id == p2.id
            && p1.xyz == p2.xyz;
}

class CylinderApproximation{
public:
    double approxRadius;
    double approxXm;
    double approxYm;
    double approxAlpha;
    double approxBeta;
    double stdev;
    QString comment;

    operator QString() const {
        return QString("comment=%1, stdev=%2, approxXm=%3, approxYm=%4, approxAlpha=%5, approxBeta=%6, approxRadius=%7")
                    .arg(comment).arg(stdev).arg(approxXm).arg(approxYm).arg(approxAlpha).arg(approxBeta).arg(approxRadius);
    }
};

enum ApproximationTypes{
    eFirstTwoPoints = 0,
    eDirection,
    eGuessAxis,
    eFirstTwoDummyPoint
};

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

    bool bestFitCircleInPlane(Circle &circle, QList<IdPoint> points, QList<IdPoint> usablePoints) {
        //calculate centroid
        OiVec centroid(4);
        foreach(const IdPoint &point, points){
            centroid = centroid + point.xyz;
        }
        centroid = centroid / (double)points.size();
        centroid.removeLast();

        //principle component analysis
        OiMat a(points.size(), 3);
        for(int i = 0; i < points.size(); i++){
            a.setAt(i, 0, points.at(i).xyz.getAt(0) - centroid.getAt(0));
            a.setAt(i, 1, points.at(i).xyz.getAt(1) - centroid.getAt(1));
            a.setAt(i, 2, points.at(i).xyz.getAt(2) - centroid.getAt(2));
        }
        OiMat ata = a.t() * a;
        OiMat u(3,3);
        OiVec d(3);
        OiMat v(3,3);
        ata.svd(u, d, v);

        //get smallest eigenvector which is n vector
        int eigenIndex = -1;
        double eVal = 0.0;
        for(int i = 0; i < d.getSize(); i++){
            if(d.getAt(i) < eVal || i == 0){
                eVal = d.getAt(i);
                eigenIndex = i;
            }
        }
        OiVec n(3);
        u.getCol(n, eigenIndex);
        n.normalize();

        //check that the normal vector of the plane is defined by the first three points A, B and C (cross product)
        OiVec ab = points.at(1).xyz - points.at(0).xyz;
        ab.removeLast();
        OiVec ac = points.at(2).xyz - points.at(0).xyz;
        ac.removeLast();
        OiVec direction(3);
        OiVec::cross(direction, ab, ac);
        direction.normalize();

        if(this->inputElements.contains(InputElementKey::eDummyPoint) && this->inputElements[InputElementKey::eDummyPoint].size() > 0) {
            // computing circle normale by dummy point
            OiVec dummyPoint = inputElements[InputElementKey::eDummyPoint][0].observation->getXYZ(); // TODO Point
            dummyPoint.removeLast();
            double dot;
            OiVec::dot(dot, dummyPoint - centroid, centroid);
            direction = - dot * dummyPoint;
            direction.normalize();
        }
        double angle = 0.0; //angle between n and direction
        OiVec::dot(angle, n, direction);
        angle = qAbs(qAcos(angle));
        if(angle > (PI/2.0)){
            n = n * -1.0;
        }

        //calculate smallest distance of the plane from the origin
        double dOrigin = n.getAt(0) * centroid.getAt(0) + n.getAt(1) * centroid.getAt(1) + n.getAt(2) * centroid.getAt(2);

        //calculate the distances of each observation from the plane
        /*OiVec planeDistances;
        for(int i = 0; i < inputObservations.size(); i++){

            double distance = qAbs(n.getAt(0) * inputObservations.at(i)->getXYZ().getAt(0)
                                   + n.getAt(1) * inputObservations.at(i)->getXYZ().getAt(1)
                                   + n.getAt(2) * inputObservations.at(i)->getXYZ().getAt(2) - dOrigin);

            planeDistances.add( distance );

        }*/

        //get transformation matrix
        OiMat t = u.inv();

        //transform centroid into 2D space
        OiVec centroid2D = t * centroid;
        centroid2D.removeLast();

        //calculate centroid reduced coordinates in 2D space
        QList<OiVec> centroidReducedCoordinates;
        QList<OiVec> allCentroidReducedCoordinates;

        foreach(const IdPoint point, usablePoints){
            OiVec xyz(4);
            xyz = point.xyz;
            xyz.removeLast(); //remove the homogeneous item
            xyz = t * xyz;
            xyz.removeLast();
            OiVec reduced = xyz - centroid2D;
            allCentroidReducedCoordinates.append(reduced);
            if(points.contains(point)) {
                centroidReducedCoordinates.append(reduced);
            }
        }

        //calculate best fit circle in 2D space
        OiVec A1(points.size());
        OiMat A2(points.size(), 3);
        for(int i = 0; i < points.size(); i++){
            A1.setAt(i, centroidReducedCoordinates.at(i).getAt(0)*centroidReducedCoordinates.at(i).getAt(0)
                     + centroidReducedCoordinates.at(i).getAt(1)*centroidReducedCoordinates.at(i).getAt(1));

            A2.setAt(i,0,centroidReducedCoordinates.at(i).getAt(0));
            A2.setAt(i,1,centroidReducedCoordinates.at(i).getAt(1));
            A2.setAt(i,2,1.0);
        }
        OiVec s(3);
        try{
            if(!OiMat::solve(s, A2.t() * A2, -1.0 * A2.t() * A1)){
                return false;
            }
        }catch(const exception &e){
            emit this->sendMessage(e.what(), eErrorMessage);
            return false;
        }

        //calculate center and radius in 2D space
        OiVec xm(3);
        xm.setAt(0, (-1.0 * s.getAt(0) / 2.0) + centroid2D.getAt(0));
        xm.setAt(1, (-1.0 * s.getAt(1) / 2.0) + centroid2D.getAt(1));
        xm.setAt(2, (t * centroid).getAt(2));
        double radius = qSqrt(0.25 * (s.getAt(0) * s.getAt(0) + s.getAt(1) * s.getAt(1)) - s.getAt(2));

        //calculate distance of each observation from the 2D circle
        OiVec circleDistances;
        OiVec allCircleDistances;
        int i=0;
        foreach(const IdPoint &point, usablePoints){
            OiVec reduced = allCentroidReducedCoordinates[i++];
            OiVec diff(2);
            diff.setAt(0, reduced.getAt(0) + centroid2D.getAt(0) - xm.getAt(0));
            diff.setAt(1, reduced.getAt(1) + centroid2D.getAt(1) - xm.getAt(1));

            double distance = 0.0;
            OiVec::dot(distance, diff, diff);
            distance = qSqrt(distance);
            distance = qAbs(distance - radius);

            allCircleDistances.add(distance);
            if(points.contains(point)) {
                circleDistances.add(distance);
            }
        }

        //transform center into 3D space
        OiMat t_inv = t.inv();
        xm = t_inv * xm;

        //calculate 3D residuals for each observation
        for(int i = 0; i < usablePoints.size(); i++){
            const IdPoint point = usablePoints[i];
            OiVec reduced = allCentroidReducedCoordinates[i];

            OiVec v_circle(3);
            OiVec v_plane(3);
            OiVec v_all(3);

            //calculate residual vector of 2D circle fit
            v_circle.setAt(0, reduced.getAt(0) + centroid2D.getAt(0) - xm.getAt(0));
            v_circle.setAt(1, reduced.getAt(1) + centroid2D.getAt(1) - xm.getAt(1));
            v_circle.setAt(2, 0.0);
            v_circle.normalize();
            v_circle = allCircleDistances.getAt(i) * v_circle;
            v_circle = t_inv * v_circle;

            //calculate residual vector of plane fit
            double distance = n.getAt(0) * point.xyz.getAt(0) + n.getAt(1) * point.xyz.getAt(1)
                    + n.getAt(2) * point.xyz.getAt(2) - dOrigin;
            v_plane = distance * n;

            //calculate the at all residual vector
            v_all = v_circle + v_plane;

            //set up display residual
            addDisplayResidual(point.id, v_all.getAt(0), v_all.getAt(1), v_all.getAt(2),
                               qSqrt(v_all.getAt(0) * v_all.getAt(0)
                                    + v_all.getAt(1) * v_all.getAt(1)
                                    + v_all.getAt(2) * v_all.getAt(2)));

        }

        //set result
        Position circlePosition;
        circlePosition.setVector(xm);
        Radius circleRadius;
        circleRadius.setRadius(radius);
        Direction circleNormal;
        circleNormal.setVector(n);
        circle.setCircle(circlePosition, circleNormal, circleRadius);

        //set statistic
        double stdev = 0.0;
        for(int i = 0; i < centroidReducedCoordinates.size(); i++){
            double v_i = circleDistances.getAt(i);
            stdev += v_i*v_i;
        }
        stdev = qSqrt(stdev / (centroidReducedCoordinates.size() - 3.0));
        this->statistic.setIsValid(true);
        this->statistic.setStdev(stdev);
        circle.setStatistic(this->statistic);

        return true;

    }
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
