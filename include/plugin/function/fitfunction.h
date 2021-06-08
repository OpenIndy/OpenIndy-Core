#ifndef FITFUNCTION_H
#define FITFUNCTION_H

#include "function.h"
#include <random>

namespace oi{

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

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

   friend class BestFitCircleUtil;
   friend class BestFitCylinderUtil;

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


class OI_CORE_EXPORT BestFitCircleUtil
{

protected:

    bool bestFitCircleInPlane(FitFunction *function, Circle &circle, QList<IdPoint> points, QList<IdPoint> usablePoints) {
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

        OiVec direction(3);
        if(function->getInputElements().contains(InputElementKey::eDummyPoint) && function->getInputElements()[InputElementKey::eDummyPoint].size() > 0) {
            // computing circle normale by dummy point
            OiVec dummyPoint = function->getInputElements()[InputElementKey::eDummyPoint][0].observation->getXYZ(); // TODO Point
            dummyPoint.removeLast();
            double dot;
            OiVec::dot(dot, dummyPoint - centroid, centroid);
            direction = - dot * dummyPoint;
            direction.normalize();
        } else {
            //check that the normal vector of the plane is defined by the first three points A, B and C (cross product)
            OiVec ab = points.at(1).xyz - points.at(0).xyz;
            ab.removeLast();
            OiVec ac = points.at(2).xyz - points.at(0).xyz;
            ac.removeLast();
            OiVec::cross(direction, ab, ac);
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
            emit function->sendMessage(e.what(), eErrorMessage);
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

            double dr = diff.length() - radius;

            allCircleDistances.add(dr);
            if(points.contains(point)) {
                circleDistances.add(dr);
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
            if(false) {
                function->addDisplayResidual(point.id, v_all.getAt(0), v_all.getAt(1), v_all.getAt(2), v_all.length());
            } else {
                function->addDisplayResidual(point.id, v_all.length() * sgn(allCircleDistances.getAt(i)));
            }


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
        function->statistic.setIsValid(true);
        function->statistic.setStdev(stdev);
        circle.setStatistic(function->statistic);

        return true;

    }
};

class OI_CORE_EXPORT BestFitCylinderUtil
{

private:
    QList<CylinderApproximation> approximations;


protected:

    bool bestFitCylinder(FitFunction *function, Cylinder &cylinder, QList<IdPoint> points, QList<IdPoint> usablePoints) {

        ApproximationTypes approximationType = eFirstTwoPoints; // default
        if(function->getScalarInputParams().stringParameter.size() > 0){
            QString value = function->getScalarInputParams().stringParameter.value("approximation");
            if(value.compare("direction") == 0){
                approximationType = eDirection;
            } else if(value.compare("guess axis") == 0){
                approximationType = eGuessAxis;
            } else if(value.compare("first two dummy points") == 0){
                approximationType = eFirstTwoDummyPoint;
            } else if(value.compare("first two points") == 0){
                approximationType = eFirstTwoPoints;
            }
        }

        //calculate centroid of all observations
        OiVec centroid(4);
        foreach(const IdPoint &point, points){
            centroid = centroid + point.xyz;
        }
        centroid = centroid / (double)points.size();

        // calculate centroid reduced observations
        QList<IdPoint> allReducedInputObservations;
        QList<IdPoint> reducedInputObservations;
        foreach(const IdPoint &obs, usablePoints){
            IdPoint rObs;
            rObs.id = obs.id;
            rObs.xyz = obs.xyz - centroid;
            allReducedInputObservations.append(rObs);
            if(points.contains(obs)) {
                reducedInputObservations.append(rObs);
            }
        }

        // approximate cylinder by 2D circle fit
        if(!this->approximateCylinder(function, cylinder, reducedInputObservations, approximationType)){
            emit function->sendMessage(QString("Error while generating approximations for cylinder parameters of cylinder %1").arg(cylinder.getFeatureName()), eErrorMessage);
            return false;
        }

        // filter the best approximation
        CylinderApproximation bestApproximation;
        double bestStdev  = numeric_limits<double>::max();
        foreach (const CylinderApproximation &approximation, approximations) {
            qDebug() << "approximation" << approximation;
            if(approximation.stdev < bestStdev) {
                bestStdev = approximation.stdev;
                bestApproximation = approximation;
            }
        }


        qDebug() << "bestApproximation" << bestApproximation;

        OiMat Ralpha(4,4);
        OiMat Rbeta(4,4);
        OiMat Rall(4,4);

        Ralpha.setAt(0, 0, 1.0);
        Ralpha.setAt(1, 1, qCos(bestApproximation.approxAlpha));
        Ralpha.setAt(1, 2, -qSin(bestApproximation.approxAlpha));
        Ralpha.setAt(2, 1, qSin(bestApproximation.approxAlpha));
        Ralpha.setAt(2, 2, qCos(bestApproximation.approxAlpha));
        Rbeta.setAt(0, 0, qCos(bestApproximation.approxBeta));
        Rbeta.setAt(0, 2, qSin(bestApproximation.approxBeta));
        Rbeta.setAt(1, 1, 1.0);
        Rbeta.setAt(2, 0, -qSin(bestApproximation.approxBeta));
        Rbeta.setAt(2, 2, qCos(bestApproximation.approxBeta));

        Rall = Rbeta * Ralpha;
        Rall.setAt(3,3,1.0);

        for (int i=0; i<reducedInputObservations.size(); i++) {
            reducedInputObservations[i].xyz = Rall * reducedInputObservations[i].xyz;
            reducedInputObservations[i].xyz.setAt(3, 1.0);
        }
        for (int i=0; i<allReducedInputObservations.size(); i++) {
            allReducedInputObservations[i].xyz = Rall * allReducedInputObservations[i].xyz;
            allReducedInputObservations[i].xyz.setAt(3, 1.0);
        }

        bestApproximation.approxAlpha = 0.0;
        bestApproximation.approxBeta = 0.0;


        if(!this->fitCylinder(function, cylinder, reducedInputObservations, allReducedInputObservations, bestApproximation)){
            emit function->sendMessage(QString("Error while fitting cylinder %1 with solution: %2").arg(cylinder.getFeatureName()).arg(bestApproximation.comment), eErrorMessage);
            cylinder.setIsSolved(false);
            return false;
        }


        emit function->sendMessage(QString("cylinder (%1) best solution: %2").arg(cylinder.getFeatureName()).arg(bestApproximation.comment), eInformationMessage);


        //transform cylinder position (centroid + rotation)
        Position cylinderPosition = cylinder.getPosition();
        OiVec pos = (Rall.inv() * cylinderPosition.getVectorH()) + centroid;
        pos.setAt(3, 1.0);
        cylinderPosition.setVector(pos);

        //transform cylinder axis (centroid + rotation)
        Direction cylinderAxis = cylinder.getDirection();
        OiVec axis = (Rall.inv() * cylinderAxis.getVectorH());
        axis.removeLast();
        axis.normalize();
        cylinderAxis.setVector(axis);


        OiVec direction(3);
        switch(approximationType) {
            case eGuessAxis: {
                break;
            }
            case eDirection: {
                foreach(const InputElement &element, function->getInputElements()[1]){
                    if(!element.geometry.isNull()
                            && element.geometry->getIsSolved()
                            && element.geometry->hasDirection()) {
                        direction = element.geometry->getDirection().getVector();
                        break;
                    }
                }
                break;
            }
            case eFirstTwoDummyPoint: {
                QList<QPointer<Observation> > dummyPoints;
                foreach(const InputElement &element, function->getInputElements()[InputElementKey::eDummyPoint]){
                    if(!element.observation.isNull()
                            && element.observation->getIsSolved()) {
                        dummyPoints.append(element.observation);
                    }
                }
                if(dummyPoints.size() >= 2){
                    OiVec diff = dummyPoints.at(1)->getXYZ() - dummyPoints.at(0)->getXYZ();
                    diff.removeLast();
                    diff.normalize();
                    direction = diff;
                }
                break;
            }
            case eFirstTwoPoints: {
                OiVec diff = points.at(1).xyz - points.at(0).xyz;
                diff.removeLast();
                diff.normalize();
                direction = diff;
                break;
            }
        }

        double angle = 0.0; //angle between r and direction
        if  (!(direction.getAt(0) == 0
            && direction.getAt(1) == 0
            && direction.getAt(2) == 0)) {
            OiVec::dot(angle, axis, direction);
            angle = qAbs(qAcos(angle));
        }

        if(angle > (PI/2.0)){
            Direction cylinderDirection = cylinder.getDirection();
            cylinderDirection.setVector(axis * -1.0);
            cylinder.setCylinder(cylinderPosition, cylinderDirection, cylinder.getRadius());
        }else{
            Direction cylinderDirection = cylinder.getDirection();
            cylinderDirection.setVector(axis);
            cylinder.setCylinder(cylinderPosition, cylinderDirection, cylinder.getRadius());
        }

        return true;

    }

    /*!
     * \brief approximateCylinder
     * \param cylinder
     * \param inputObservations
     * \return
     */
    bool approximateCylinder(FitFunction *function, Cylinder &cylinder, const QList<IdPoint> &inputObservations, ApproximationTypes approximationType){

        //clear current approximations
        this->approximations.clear();

        switch(approximationType) {
            case eGuessAxis: {
                //get the number of observations
                int numPoints = inputObservations.size();

                //calculate centroid of all observations
                OiVec centroid3D(4);
                foreach(const IdPoint &obs, inputObservations){
                    centroid3D = centroid3D + obs.xyz;
                }
                centroid3D.removeLast();
                centroid3D = centroid3D / numPoints;

                //set up covariance matrix of all observations
                OiMat H(3, 3);
                for (int k = 0; k < numPoints; k++) {
                    IdPoint obs = inputObservations.at(k);
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            H.setAt(i,j, H.getAt(i, j) + (obs.xyz.getAt(i) - centroid3D.getAt(i)) * (obs.xyz.getAt(j) - centroid3D.getAt(j)));
                        }
                    }
                }

                //singular value decomposition of H to get the major axis direction of the cylinder observations
                OiMat U(3, 3);
                OiVec d(3);
                OiMat V(3, 3);
                try{
                    H.svd(U, d, V);
                }catch(const exception &e){
                    emit function->sendMessage(QString("SVD error cylinder minimum solution: %1").arg(e.what()), eErrorMessage);
                    return false;
                }

                //one of the eigen-vectors is the approximate cylinder axis
                bool foundOneVaildApproximation = false;
                for(int i = 0; i < 3; i++){
                    OiVec pn; //possible normal vector

                    U.getCol(pn, i); //get eigenvector i

                    if(approximateCylinder(function, pn, inputObservations, QString("eigenvector %1").arg(i))) {
                        foundOneVaildApproximation = true;
                    }
                }
                return foundOneVaildApproximation;
                break;
            }
        case eDirection: {
                //########################
                // approximation direction
                //########################

                Direction approxAxis;
                // find first direction
                foreach(const InputElement &element, function->getInputElements()[1]){
                    if(!element.geometry.isNull()
                            && element.geometry->getIsSolved()
                            && element.geometry->hasDirection()) {
                        approxAxis = element.geometry->getDirection();
                        break;
                    }

                }
                // valid direction found
                OiVec an = approxAxis.getVector();
                if  (!(an.getAt(0) == 0
                    && an.getAt(1) == 0
                    && an.getAt(2) == 0)) {

                    return approximateCylinder(function, an, inputObservations, "approxmation direction");
                } else {
                    return false;
                }

                break;
            }
        case eFirstTwoDummyPoint: {
            QList<QPointer<Observation> > dummyPoints;
            foreach(const InputElement &element, function->getInputElements()[InputElementKey::eDummyPoint]){
                if(!element.observation.isNull()
                        && element.observation->getIsSolved()) {
                    dummyPoints.append(element.observation);
                }
            }
            if(dummyPoints.size() < 2){
                return false;
            }
            OiVec diff = dummyPoints.at(1)->getXYZ() - dummyPoints.at(0)->getXYZ();
            diff.removeLast();

            return approximateCylinder(function, diff, inputObservations, "first two dummy points");

            break;
        }
            case eFirstTwoPoints: {
            default:
                //##############################################################
                //another approximation comes from the first two cylinder points
                //##############################################################

                OiVec diff = inputObservations.at(1).xyz - inputObservations.at(0).xyz;
                diff.removeLast();

                return approximateCylinder(function, diff, inputObservations, "first two cylinder points");

                break;
            }
        }

        return false;

    }

    bool approximateCylinder(FitFunction *function, OiVec pn, const QList<IdPoint> &inputObservations, QString label) {
        //get the number of observations
        int numPoints = inputObservations.size();

        //init helper variables
        OiVec centroid2D(2); //centroid of 2D circle

        double a = 0.0, b = 0.0; //sin + cos of rotation angles
        double a_alpha = 0.0, b_alpha = 0.0, a_beta = 0.0, b_beta = 0.0; //possible rotation angles (check acos + asin)
        double alpha = 0.0, beta = 0.0; //rotation angles
        double _y = 0.0, _z = 0.0;
        double tx = 0.0, ty = 0.0; //transformed 2D coordinates
        double x_m = 0.0, y_m = 0.0, radius = 0.0, sum_vv = 0.0; //result parameters in circle fit
        OiVec v;

        //rotation matrices
        OiMat Ralpha(3, 3);
        OiMat Rbeta(3, 3);
        OiMat Rall(3, 3);

        //a matrices for circle adjustment
        OiVec A1(numPoints);
        OiMat A2(numPoints,3);

        //result vector for circle fit
        OiVec s(3);

        //rotated 2D points
        QList<float> points2D_x;
        QList<float> points2D_y;

        //rotated centroid reduced 2D points
        QList<float> centroidreduced2D_x;
        QList<float> centroidreduced2D_y;

        //calculate rotation angles
        if (pn.getAt(1) == 0 && pn.getAt(2) == 0) {
            a_alpha = PI / 2;
            b_alpha = PI / 2;
        } else if(pn.getAt(1) == 0) {
            a_alpha = 0;
            b_alpha = 0;
        } else {
            a = qSqrt(1.0 / (1.0 + qPow(pn.getAt(2)/pn.getAt(1), 2)));
            b = a * pn.getAt(2) / pn.getAt(1);

            a_alpha = qAsin(a);
            b_alpha = qAcos(b);
        }

        alpha = 0.0;

        //cos and sin are ambiguous, therefor check the right angle here
        if(this->compareAngles(b_alpha, a_alpha)){
            alpha = a_alpha;
        }else if(this->compareAngles(this->getCorrespondingCos(b_alpha), a_alpha)){
            alpha = a_alpha;
        }else if(this->compareAngles(b_alpha, this->getCorrespondingSin(a_alpha))){
            alpha = b_alpha;
        }else{
            alpha = this->getCorrespondingCos(b_alpha);
        }

        //transform eigenvector --> y-component should be 0 now
        _y = pn.getAt(1);
        _z = pn.getAt(2);
        pn.setAt(1, _y * qCos(alpha) - _z * qSin(alpha));
        pn.setAt(2, _y * qSin(alpha) + _z * qCos(alpha));

        if (pn.getAt(0) == 0 && pn.getAt(2) == 0) {
            a_beta = PI / 2;
            b_beta = PI / 2;
        } else if(pn.getAt(0) == 0) {
            a_beta = 0;
            b_beta = 0;
        } else {
            a = qSqrt(1.0 / (1.0 + qPow(pn.getAt(2) / pn.getAt(0),2)));
            b = -1.0 * pn.getAt(2) * a / pn.getAt(0);

            a_beta = qAsin(a);
            b_beta = qAcos(b);
        }

        beta = 0.0;

        //cos and sin are ambiguous, therefor check the right angle here
        if(this->compareAngles(b_beta, a_beta)){
            beta = b_beta;
        }else if(this->compareAngles(this->getCorrespondingCos(b_beta), a_beta)){
            beta = a_beta;
        }else if(this->compareAngles(b_beta, this->getCorrespondingSin(a_beta))){
            beta = b_beta;
        }else{
            beta = this->getCorrespondingCos(b_beta);
        }

        //set up rotation matrices
        Ralpha.setAt(0, 0, 1.0);
        Ralpha.setAt(1, 1, qCos(alpha));
        Ralpha.setAt(1, 2, -qSin(alpha));
        Ralpha.setAt(2, 1, qSin(alpha));
        Ralpha.setAt(2, 2, qCos(alpha));
        Rbeta.setAt(0, 0, qCos(beta));
        Rbeta.setAt(0, 2, qSin(beta));
        Rbeta.setAt(1, 1, 1.0);
        Rbeta.setAt(2, 0, -qSin(beta));
        Rbeta.setAt(2, 2, qCos(beta));

        Rall = Rbeta * Ralpha;

        //###########################################
        //circle fit to determine midpoint and radius
        //###########################################

        //clear transformed point lists
        points2D_x.clear();
        points2D_y.clear();
        centroidreduced2D_x.clear();
        centroidreduced2D_y.clear();

        //reset 2D centroid and recalculate with new rotation
        centroid2D = OiVec(2);

        //rotate observations in XY-plane and calculate 2D centroid
        for(int j = 0; j < numPoints; j++){

            IdPoint obs = inputObservations.at(j);

            tx = Rall.getAt(0,0)*obs.xyz.getAt(0)
                    + Rall.getAt(0,1)*obs.xyz.getAt(1)
                    + Rall.getAt(0,2)*obs.xyz.getAt(2);
            ty = Rall.getAt(1,0)*obs.xyz.getAt(0)
                    + Rall.getAt(1,1)*obs.xyz.getAt(1)
                    + Rall.getAt(1,2)*obs.xyz.getAt(2);

            points2D_x.append( tx );
            points2D_y.append( ty );

            centroid2D.setAt(0, centroid2D.getAt(0) + tx);
            centroid2D.setAt(1, centroid2D.getAt(1) + ty);

        }
        centroid2D.setAt(0, centroid2D.getAt(0) / (float)numPoints);
        centroid2D.setAt(1, centroid2D.getAt(1) / (float)numPoints);

        //calculate centroid reduced 2D coordinates
        for(int j = 0; j < numPoints; j++){
            centroidreduced2D_x.append( points2D_x.at(j) - centroid2D.getAt(0) );
            centroidreduced2D_y.append( points2D_y.at(j) - centroid2D.getAt(1) );
        }

        //fill a matrices
        for(int j = 0; j < numPoints; j++){
            tx = centroidreduced2D_x.at(j);
            ty = centroidreduced2D_y.at(j);

            A1.setAt(j, tx*tx + ty*ty);

            A2.setAt(j,0,tx);
            A2.setAt(j,1,ty);
            A2.setAt(j,2,1.0);
        }

        //solve equation system to get circle parameters
        try{
            if(!OiMat::solve(s, A2.t() * A2, -1.0 * A2.t() * A1)){
                return false;
            }
        }catch(const exception &e){
            emit function->sendMessage(QString("inv error cylinder minimum solution: %1").arg(e.what()), eErrorMessage);
            return false;
        }

        //calculate midpoint + radius
        x_m = (-1.0 * s.getAt(0) / 2.0) + centroid2D.getAt(0);
        y_m = (-1.0 * s.getAt(1) / 2.0) + centroid2D.getAt(1);
        radius = qSqrt(0.25 * (s.getAt(0) * s.getAt(0) + s.getAt(1) * s.getAt(1)) - s.getAt(2));

        //calculate statistic
        v = -1.0 * A1 - A2 * s;
        OiVec::dot(sum_vv, v,v);
        sum_vv = qSqrt(sum_vv / (numPoints-3.0));

        //add approximation
        CylinderApproximation approximation;
        approximation.approxAlpha = alpha;
        approximation.approxBeta = beta;
        approximation.approxXm = -1.0 * x_m;
        approximation.approxYm = -1.0 * y_m;
        approximation.approxRadius = radius;
        approximation.stdev = sum_vv;
        approximation.comment = label;
        this->approximations.append(approximation);

        return true;
    }

    /*!
     * \brief fitCylinder
     * \param cylinder
     * \param inputObservations
     * \param approximation
     * \return
     */
    bool fitCylinder(FitFunction *function, Cylinder &cylinder, const QList<IdPoint> &inputObservations, const QList<IdPoint> &allUsableObservations, const CylinderApproximation &approximation){

        //get the number of observations
        const int numPoints = inputObservations.size();

        //centroids of all points
        OiVec centroid(4);

        //initialize variables
        OiVec v(numPoints*3); //approximation of corrections
        OiVec L0(numPoints*3); //L + v
        OiVec c(numPoints+5); //right side
        OiMat N(numPoints+5, numPoints+5); //normal equation matrix
        OiVec res(numPoints+5); //adjustment result
        double _r = 0.0, _X0 = 0.0, _Y0 = 0.0, _alpha = 0.0, _beta = 0.0;
        double _r_armijo = 0.0, _X0_armijo = 0.0, _Y0_armijo = 0.0, _alpha_armijo = 0.0, _beta_armijo = 0.0;
        //OiVec a(5);
        //OiVec b(5);
        OiVec a(numPoints);
        OiVec b(numPoints);
        double _x = 0.0, _y = 0.0, _z = 0.0;
        double _x_armijo = 0.0, _y_armijo = 0.0, _z_armijo = 0.0;
        double a1 = 0.0, a2 = 0.0;
        double diff = 0.0, _xr = 0.0, _yr = 0.0;
        double sigma = 2.0;
        OiMat Ralpha(3, 3);
        OiMat Rbeta(3, 3);
        OiMat Rall(3, 3);

        //set approximations of unknowns
        _r = approximation.approxRadius;
        _X0 = approximation.approxXm;
        _Y0 = approximation.approxYm;
        _alpha = approximation.approxAlpha;
        _beta = approximation.approxBeta;
        OiVec x(5); //vector of unknown corrections

        //fill L vector
        for(int i = 0; i < numPoints; i++){

            IdPoint obs = inputObservations.at(i);

            L0.setAt(i*3, obs.xyz.getAt(0));
            L0.setAt(i*3+1, obs.xyz.getAt(1));
            L0.setAt(i*3+2, obs.xyz.getAt(2));

            centroid = centroid + obs.xyz;

        }
        centroid.removeLast();
        centroid = centroid / (double)numPoints;

        int numIterations = 0;

        double stopAA = 0.0, stopBB = 0.0, stopXX = 0.0, stopVV = 0.0;
        do{

            //improve unknowns
            _r += x.getAt(0);
            _X0 += x.getAt(1);
            _Y0 += x.getAt(2);
            _alpha += x.getAt(3);
            _beta += x.getAt(4);

            //improve observations
            //L0 = L0 + v;

            //build rotation matrices
            Ralpha.setAt(0, 0, 1.0);
            Ralpha.setAt(1, 1, qCos(_alpha));
            Ralpha.setAt(1, 2, -qSin(_alpha));
            Ralpha.setAt(2, 1, qSin(_alpha));
            Ralpha.setAt(2, 2, qCos(_alpha));
            Rbeta.setAt(0, 0, qCos(_beta));
            Rbeta.setAt(0, 2, qSin(_beta));
            Rbeta.setAt(1, 1, 1.0);
            Rbeta.setAt(2, 0, -qSin(_beta));
            Rbeta.setAt(2, 2, qCos(_beta));

            Rall = Rbeta * Ralpha;

            //fill A and B matrix + w vector + right side
            for(int i = 0; i < numPoints; i++){

                _x = L0.getAt(i*3);
                _y = L0.getAt(i*3+1);
                _z = L0.getAt(i*3+2);

                a1 = _X0 + _x * qCos(_beta) + _y * qSin(_alpha) * qSin(_beta) + _z * qCos(_alpha) * qSin(_beta);
                a2 = _Y0 + _y * qCos(_alpha) - _z * qSin(_alpha);

                //A
                N.setAt(i, numPoints, 1.0);
                N.setAt(i, numPoints+1, -1.0 * a1 / qSqrt(a1*a1 + a2*a2));
                N.setAt(i, numPoints+2, -1.0 * a2 / qSqrt(a1*a1 + a2*a2));
                N.setAt(i, numPoints+3, -1.0 * ((_y * qSin(_beta) * qCos(_alpha) - _z * qSin(_beta) * qSin(_alpha)) * a1 - (_y * qSin(_alpha) + _z * qCos(_alpha)) * a2) / qSqrt(a1*a1 + a2*a2));
                N.setAt(i, numPoints+4, -1.0 * (_y * qSin(_alpha) * qCos(_beta) - _x * qSin(_beta) + _z * qCos(_alpha) * qCos(_beta)) * a1 / qSqrt(a1*a1 + a2*a2));

                //AT
                N.setAt(numPoints, i, 1.0);
                N.setAt(numPoints+1, i, -1.0 * a1 / qSqrt(a1*a1 + a2*a2));
                N.setAt(numPoints+2, i, -1.0 * a2 / qSqrt(a1*a1 + a2*a2));
                N.setAt(numPoints+3, i, -1.0 * ((_y * qSin(_beta) * qCos(_alpha) - _z * qSin(_beta) * qSin(_alpha)) * a1 - (_y * qSin(_alpha) + _z * qCos(_alpha)) * a2) / qSqrt(a1*a1 + a2*a2));
                N.setAt(numPoints+4, i, -1.0 * (_y * qSin(_alpha) * qCos(_beta) - _x * qSin(_beta) + _z * qCos(_alpha) * qCos(_beta)) * a1 / qSqrt(a1*a1 + a2*a2));

                //BBT
                N.setAt(i, i, (-1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2))*(-1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2))
                            + (-1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2))*(-1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2))
                            + (-1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2))*(-1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)));

                _xr = Rall.getAt(0,0) * _x + Rall.getAt(0,1) * _y + Rall.getAt(0,2) * _z + _X0;
                _yr = Rall.getAt(1,0) * _x + Rall.getAt(1,1) * _y + Rall.getAt(1,2) * _z + _Y0;

                //approximate radius of the cylinder minus distance of point i to the z-axis is the contradiction
                diff = _r - qSqrt(_xr*_xr + _yr*_yr);

                c.setAt(i, diff);

            }

            //solve the normal equation system
            try{
                if(!OiMat::solve(res, N, -1.0*c)){
                    emit function->sendMessage(QString("solve error cylinder fit"), eErrorMessage);
                    return false;
                }
            }catch(const exception &e){
                emit function->sendMessage(QString("solve error cylinder fit: %1").arg(e.what()), eErrorMessage);
                return false;
            }

            //get new corrections for unknowns
            for(int i = numPoints; i < numPoints+5; i++){
                x.setAt(i-numPoints, res.getAt(i));
            }

            //calculate improvements
            for(int i = 0; i < numPoints; ++i){

                IdPoint obs = inputObservations.at(i);

                _x = obs.xyz.getAt(0);
                _y = obs.xyz.getAt(1);
                _z = obs.xyz.getAt(2);

                a1 = _X0 + _x * qCos(_beta) + _y * qSin(_alpha) * qSin(_beta) + _z * qCos(_alpha) * qSin(_beta);
                a2 = _Y0 + _y * qCos(_alpha) - _z * qSin(_alpha);

                v.setAt(i*3, (-1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2)) * res.getAt(i));
                v.setAt(i*3+1, (-1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)) * res.getAt(i));
                v.setAt(i*3+2, (-1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)) * res.getAt(i));

            }

            //apply Armijo rule which is useful in case of bad approximations
            /*do{

                sigma = sigma / 2.0;

                _r_armijo = _r + sigma * x.getAt(0);
                _X0_armijo = _X0 + sigma * x.getAt(1);
                _Y0_armijo = _Y0 + sigma * x.getAt(2);
                _alpha_armijo = _alpha + sigma * x.getAt(3);
                _beta_armijo = _beta + sigma * x.getAt(4);

                for(int i = 0; i < 5; i++){
                    _x = L0.getAt(i*3);
                    _y = L0.getAt(i*3+1);
                    _z = L0.getAt(i*3+2);

                    a.setAt(i, _r_armijo - qSqrt( (_X0_armijo + _x*qCos(_beta_armijo) + _y*qSin(_alpha_armijo)*qSin(_beta_armijo) + _z*qCos(_alpha_armijo)*qSin(_beta_armijo))*(_X0_armijo + _x*qCos(_beta_armijo) + _y*qSin(_alpha_armijo)*qSin(_beta_armijo) + _z*qCos(_alpha_armijo)*qSin(_beta_armijo))
                                           + (_Y0_armijo + _y*qCos(_alpha_armijo) - _z*qSin(_alpha_armijo))*(_Y0_armijo + _y*qCos(_alpha_armijo) - _z*qSin(_alpha_armijo)) ));

                    b.setAt(i, _r - qSqrt( (_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))*(_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))
                                                   + (_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha))*(_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha)) ));
                }

                OiVec::dot(stopAA, a, a);
                OiVec::dot(stopBB, b, b);

            }while( stopAA > ( stopBB - 2.0 * 0.001 * sigma * stopBB ) );

            x = sigma * x;*/




            do{

                sigma = sigma / 2.0;

                _r_armijo = _r + sigma * x.getAt(0);
                _X0_armijo = _X0 + sigma * x.getAt(1);
                _Y0_armijo = _Y0 + sigma * x.getAt(2);
                _alpha_armijo = _alpha + sigma * x.getAt(3);
                _beta_armijo = _beta + sigma * x.getAt(4);

                for(int i = 0; i < numPoints; i++){
                    _x = L0.getAt(i*3);
                    _y = L0.getAt(i*3+1);
                    _z = L0.getAt(i*3+2);

                    _x_armijo = L0.getAt(i*3) + sigma * v.getAt(i*3);
                    _y_armijo = L0.getAt(i*3+1) + sigma * v.getAt(i*3+1);
                    _z_armijo = L0.getAt(i*3+2) + sigma * v.getAt(i*3+2);

                    a.setAt(i, _r_armijo - qSqrt( (_X0_armijo + _x_armijo*qCos(_beta_armijo) + _y_armijo*qSin(_alpha_armijo)*qSin(_beta_armijo) + _z_armijo*qCos(_alpha_armijo)*qSin(_beta_armijo))*(_X0_armijo + _x_armijo*qCos(_beta_armijo) + _y_armijo*qSin(_alpha_armijo)*qSin(_beta_armijo) + _z_armijo*qCos(_alpha_armijo)*qSin(_beta_armijo))
                                           + (_Y0_armijo + _y_armijo*qCos(_alpha_armijo) - _z_armijo*qSin(_alpha_armijo))*(_Y0_armijo + _y_armijo*qCos(_alpha_armijo) - _z_armijo*qSin(_alpha_armijo)) ));

                    b.setAt(i, _r - qSqrt( (_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))*(_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))
                                                   + (_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha))*(_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha)) ));
                }

                OiVec::dot(stopAA, a, a);
                OiVec::dot(stopBB, b, b);

            }while( stopAA > stopBB );

            OiVec::dot(stopXX, x, x);
            OiVec::dot(stopVV, v, v);

            x = sigma * x;
            v = sigma * v;





            //nur testweise
            if(sigma < 0.1){
                std::default_random_engine generator;


                /*_r = approximation.approxRadius;
                _X0 = approximation.approxXm;
                _Y0 = approximation.approxYm;
                _alpha = approximation.approxAlpha;
                _beta = approximation.approxBeta;

                OiVec X_temp(5);
                X_temp.setAt(0, _r);
                X_temp.setAt(1, _X0);
                X_temp.setAt(2, _Y0);
                X_temp.setAt(3, _alpha);
                X_temp.setAt(4, _beta);*/

                for(int k = 0; k < 5; k++){
                    std::uniform_real_distribution<double> distribution(-3.0 * qAbs(x.getAt(k)), 3.0 * qAbs(x.getAt(k)));
                    //std::uniform_real_distribution<double> distribution(-0.001 * qAbs(X_temp.getAt(k)), 0.001 * qAbs(X_temp.getAt(k)));
                    //std::uniform_real_distribution<double> distribution(-(double)(1.0/sigma) * qAbs(X_temp.getAt(k)), (double)(1.0/sigma) * qAbs(X_temp.getAt(k)));
                    double dice_roll = distribution(generator);
                    x.setAt(k, x.getAt(k) + dice_roll);
                }
            }



            sigma = 2.0;

            numIterations++;

        //}while( (stopXX > 0.000000001) && (stopVV > 0.000000001) && numIterations < 1000 );
        //}while( (stopXX > 0.00000000001) && (stopVV > 0.00000000001) && numIterations < 1000 );
        //}while( ((stopXX > 0.00000000001) || (stopVV > 0.00000000001)) && numIterations < 1000 );
        //}while( ((stopXX > 0.0000000000001) || (stopVV > 0.00000000001)) && numIterations < 1000 );
        }while( stopXX > 0.0000000000001 && numIterations < 1000 );

        if(numIterations >= 1000){
            emit function->sendMessage(QString("to many iterations: %1").arg(numIterations), eWarningMessage);
            return false;
        }

        //apply corrections to unknowns
        _r += x.getAt(0);
        _X0 += x.getAt(1);
        _Y0 += x.getAt(2);
        _alpha += x.getAt(3);
        _beta += x.getAt(4);

        //calculate rotation matrices
        Ralpha.setAt(0, 0, 1.0);
        Ralpha.setAt(1, 1, qCos(_alpha));
        Ralpha.setAt(1, 2, -qSin(_alpha));
        Ralpha.setAt(2, 1, qSin(_alpha));
        Ralpha.setAt(2, 2, qCos(_alpha));
        Rbeta.setAt(0, 0, qCos(_beta));
        Rbeta.setAt(0, 2, qSin(_beta));
        Rbeta.setAt(1, 1, 1.0);
        Rbeta.setAt(2, 0, -qSin(_beta));
        Rbeta.setAt(2, 2, qCos(_beta));

        Rall = Rbeta * Ralpha;

        //calculate point on cylinder axis
        OiVec xyz(3);
        xyz.setAt(0, _X0);
        xyz.setAt(1, _Y0);
        xyz.setAt(2, 0.0);
        OiMat::solve(xyz, Rall, -1.0 * xyz);

        //calculate cylinder axis
        OiVec axis(3);
        axis.setAt(2, 1.0);
        OiMat::solve(axis, Rall, axis);

        // reset / clear statistic
        function->statistic.reset();

        //calculate sum vv
        double sumVV = 0.0;
        float vrMin = numeric_limits<float>::max();
        float vrMax = numeric_limits<float>::min();
        foreach(const IdPoint &observation, allUsableObservations){
            OiVec v_obs(3);
            _x = observation.xyz.getAt(0);
            _y = observation.xyz.getAt(1);
            _z = observation.xyz.getAt(2);

            float b[3]; //vector between point on cylinder axis and point p which is probably on cylinder
            b[0] = _x - xyz.getAt(0);
            b[1] = _y - xyz.getAt(1);
            b[2] = _z - xyz.getAt(2);

            float n0CrossB[3]; //cross product of cylinder axis (length 1) and b
            n0CrossB[0] = axis.getAt(1) * b[2] - axis.getAt(2) * b[1];
            n0CrossB[1] = axis.getAt(2) * b[0] - axis.getAt(0) * b[2];
            n0CrossB[2] = axis.getAt(0) * b[1] - axis.getAt(1) * b[0];

            float radiusActual = 0.0f; //smallest distance of point p to cylinder axis

            radiusActual = qSqrt( n0CrossB[0]*n0CrossB[0] + n0CrossB[1]*n0CrossB[1] + n0CrossB[2]*n0CrossB[2] );

            float distance = 0.0f;

            distance = radiusActual - _r; //distance error
            if(inputObservations.contains(observation)) { // calculate form error from "used" observations
                vrMin = min(vrMin, distance);
                vrMax = max(vrMax, distance);
            }

            //calculate residual vector
            v_obs.setAt(0, b[0]);
            v_obs.setAt(1, b[1]);
            v_obs.setAt(2, b[2]);
            v_obs.normalize();
            v_obs = distance * v_obs;

            //set up display residuals
            if(false) {
                function->addDisplayResidual(observation.id, v_obs.getAt(0), v_obs.getAt(1), v_obs.getAt(2),
                                   qSqrt(v_obs.getAt(0) * v_obs.getAt(0) + v_obs.getAt(2) * v_obs.getAt(2)));
            } else {
                function->addDisplayResidual(observation.id, distance);
            }

            if(inputObservations.contains(observation)) {
                sumVV += distance * distance;
            }

        }

        function->statistic.setFormError(vrMax - vrMin); // roundness

        //set up result
        Radius cylinderRadius(_r);
        Direction cylinderDirection(axis);
        Position cylinderPosition(xyz);
        cylinder.setCylinder(cylinderPosition, cylinderDirection, cylinderRadius);
        function->statistic.setIsValid(true);
        function->statistic.setStdev(qSqrt(sumVV / ((double)numPoints - 5.0)));
        cylinder.setStatistic(function->statistic);

        return true;

    }

    /*!
     * \brief getCorrespondingCos
     * \param a
     * \return
     */
    double getCorrespondingCos(double a){
        while(a < 0.0){
            a += 2.0*PI;
        }
        while(a > 2.0*PI){
            a -= 2.0*PI;
        }
        return 2.0 * PI - a;
    }

    /*!
     * \brief getCorrespondingSin
     * \param a
     * \return
     */
    double getCorrespondingSin(double a){
        while(a < 0.0){
            a += 2.0*PI;
        }
        while(a > 2.0*PI){
            a -= 2.0*PI;
        }
        if(a <= PI){
            return PI - a;
        }else{
            return 3.0*PI - a;
        }
    }

    /*!
     * \brief compareAngles
     * \param a
     * \param b
     * \return
     */
    bool compareAngles(double a, double b){

        double t = 2.0*PI;

        //a und b between 0 und 2*PI
        while(a < 0.0){
            a += t;
        }
        while(a > t){
            a -= t;
        }
        while(b < 0.0){
            b += t;
        }
        while(b > t){
            b -= t;
        }

        double diff = a - b;
        if(diff < 0.001 && diff > -0.001){
            return true;
        }
        return false;

    }

};

}; // namespace oi
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
