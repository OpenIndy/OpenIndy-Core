#include "pointcloud.h"

#include "featurewrapper.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief PointCloud::PointCloud
 * \param isNominal
 * \param parent
 */
PointCloud::PointCloud(bool isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPointCloud(this);
    }

}

/*!
 * \brief PointCloud::PointCloud
 * \param copy
 */
PointCloud::PointCloud(const PointCloud &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPointCloud(this);
    }

    this->xyz = copy.xyz;
    this->setBoundingBox(copy.bbox);

}

/*!
 * \brief PointCloud::operator =
 * \param copy
 * \return
 */
PointCloud &PointCloud::operator=(const PointCloud &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPointCloud(this);
    }

     this->xyz = copy.xyz;
    this->setBoundingBox(copy.bbox);

    return *this;

}

/*!
 * \brief PointCloud::~PointCloud
 */
PointCloud::~PointCloud(){

}

/*!
 * \brief PointCloud::getPointCount
 * \return
 */
unsigned long PointCloud::getPointCount() const{
    return this->points.size();
}

/*!
 * \brief PointCloud::addSegment
 * \param segment
 * \return
 */
bool PointCloud::addSegment(const QPointer<FeatureWrapper> &segment){

    //check segment
    if(segment.isNull() || segment->getGeometry().isNull()){
        return false;
    }

    //check if a segment with that name already exists
    foreach(const QPointer<FeatureWrapper> &detectedSegment, this->detectedSegments){

        if(!detectedSegment.isNull() && !detectedSegment->getFeature().isNull()
                && detectedSegment->getFeature()->getFeatureName().compare(segment->getGeometry()->getFeatureName()) == 0){
            return false;
        }

    }

    segment->getGeometry()->setNominalSystem(this->nominalSystem);

    this->detectedSegments.append(segment);

    emit this->pcSegmentAdded(segment);

    return true;

}

/*!
 * \brief PointCloud::clearSegmentation
 */
void PointCloud::clearSegmentation(){
    this->detectedSegments.clear();
}

/*!
 * \brief PointCloud::hasPosition
 * \return
 */
bool PointCloud::hasPosition() const{
    return true;
}

/*!
 * \brief PointCloud::getPointCloudPoints
 * Returns a list of all point cloud points
 * \return
 */
const QList<QPointer<Point_PC> > &PointCloud::getPointCloudPoints() const{
    return this->points;
}

/*!
 * \brief PointCloud::addPointCloudPoint
 * \param point
 */
void PointCloud::addPointCloudPoint(const QPointer<Point_PC> &point){
    this->points.append(point);
}

/*!
 * \brief PointCloud::getBoundingBox
 * Returns the bounding box of the point cloud
 * \return
 */
const BoundingBox_PC &PointCloud::getBoundingBox() const{
    return this->bbox;
}

/*!
 * \brief PointCloud::setBoundingBox
 * \param bbox
 */
void PointCloud::setBoundingBox(const BoundingBox_PC &bbox){
    this->bbox = bbox;
}

/*!
 * \brief PointCloud::recalc
 */
void PointCloud::recalc(){

    Geometry::recalc();

}

/*!
 * \brief PointCloud::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement PointCloud::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement pointCloud = Geometry::toOpenIndyXML(xmlDoc);

    if(pointCloud.isNull()){
        return pointCloud;
    }

    pointCloud.setAttribute("type", getGeometryTypeName(ePointCloudGeometry));

    return pointCloud;

}

/*!
 * \brief PointCloud::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool PointCloud::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

