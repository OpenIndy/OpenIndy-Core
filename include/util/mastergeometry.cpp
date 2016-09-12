#include "mastergeometry.h"

#include "featurewrapper.h"
#include "oijob.h"
#include "coordinatesystem.h"
#include "observation.h"
#include "station.h"
#include "function.h"
#include "geometry.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief oi::MasterGeometry::MasterGeometry
 * \param parent
 */
oi::MasterGeometry::MasterGeometry(QObject *parent)
{

}

MasterGeometry::MasterGeometry(const MasterGeometry &copy, QObject *parent) : Feature(copy, parent)
{
    //copy attributes
    this->myActual = copy.myActual;
    this->myNominals = copy.myNominals;
}

MasterGeometry &MasterGeometry::operator =(const MasterGeometry &copy)
{
    return *this;
}

MasterGeometry::~MasterGeometry()
{

}

/*!
 * \brief oi::MasterGeometry::getNominals
 * \return
 */
const QList<QPointer<Geometry> > &MasterGeometry::getNominals() const
{
    return this->myNominals;
}

/*!
 * \brief oi::MasterGeometry::addNominal
 * \param nominal
 * \return
 */
bool MasterGeometry::addNominal(const QPointer<Geometry> &nominal)
{
    if(!nominal.isNull() && nominal->getIsNominal()){

        //check if the nominal is in the same job
        if(!this->job.isNull()){
            QPointer<FeatureWrapper> jobNominal = this->job->getFeatureById(nominal->getId());
            if(jobNominal.isNull() || jobNominal->getGeometry().isNull() || jobNominal->getGeometry() != nominal){
                return false;
            }
        }

        //actual already set?
        if(this->myActual.isNull()){
            //check for other nominals and their featuretypes
            if(this->myNominals.size()>0){
                if(checkActualNominaltype(nominal,this->myNominals.first())){
                    //check if this geometry already contains the nominal
                    if(!this->myNominals.contains(nominal)){
                        this->myNominals.append(nominal);
                        nominal->setMasterGeom(this);

                        emit this->geomNominalsChanged(this->id);

                        return true;
                    }
                }
            }else{//first nominal
                this->myNominals.append(nominal);
                nominal->setMasterGeom(this);

                emit this->geomNominalsChanged(this->id);

                return true;
            }
        }else{
            //check feature type of actual and nominal
            if(checkActualNominaltype(this->myActual,nominal)){
                if(!this->myNominals.contains(nominal)){
                    this->myNominals.append(nominal);
                    nominal->setActual(this->myActual);
                    nominal->setMasterGeom(this);

                    emit this->geomNominalsChanged(this->id);

                    return true;
                }
            }
        }
    }
    return false;
}

/*!
 * \brief MasterGeometry::removeNominal
 * \param nominal
 * \return
 */
bool MasterGeometry::removeNominal(const QPointer<Geometry> &nominal)
{
    //check if the nominal is valid
    if(nominal.isNull()){
        return false;
    }

    //remove nominal
    nominal->removeMasterGeom();
    nominal->setActual(QPointer<Geometry> (NULL));
    return this->myNominals.removeOne(nominal);
}

/*!
 * \brief MasterGeometry::getActual
 * \return
 */
const QPointer<Geometry> &MasterGeometry::getActual() const
{
    return this->myActual;
}

/*!
 * \brief MasterGeometry::setActual
 * \param actual
 * \return
 */
bool MasterGeometry::setActual(const QPointer<Geometry> &actual)
{
    //check if this is a nominal feature
    if(!actual->getIsNominal()){
        return false;
    }

    //check actual
    if(myActual.isNull() || (!this->myActual.isNull() && this->myActual->getId() == actual->getId())
            || actual->getIsNominal()){
        return false;
    }

    //check if actual is in the same job
    if(!this->job.isNull()){
        QPointer<FeatureWrapper> jobActual = this->job->getFeatureById(actual->getId());
        if(jobActual.isNull() || jobActual->getGeometry().isNull() || jobActual->getGeometry() != actual){
            return false;
        }
    }

    //check feature types
    if(this->myNominals.size()>0){
        if(checkActualNominaltype(actual, this->myNominals.first())){
            this->myActual = actual;
            //add actual to all nominals
            for(int i=0; i<this->myNominals.size();i++){
                this->myNominals.at(i)->setActual(this->myActual);
                actual->addNominal(this->myNominals.at(i));
            }
            actual->setMasterGeom(this);

            emit this->geomActualChanged(this->id);

            return true;
        }
    }else{
        this->myActual = actual;
        actual->setMasterGeom(this);

        emit this->geomActualChanged(this->id);

        return true;
    }
    return false;
}

/*!
 * \brief MasterGeometry::removeActual
 * \param actual
 * \return
 */
bool MasterGeometry::removeActual(const QPointer<Geometry> &actual)
{
    //check if actual is valid
    if(actual.isNull()){
        return false;
    }
    actual->removeMasterGeom();
    this->myActual.clear();

    return true;
}

/*!
 * \brief MasterGeometry::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement MasterGeometry::toOpenIndyXML(QDomDocument &xmlDoc) const
{
    QDomElement geometry = Feature::toOpenIndyXML(xmlDoc);

    if(geometry.isNull()){
        return geometry;
    }

    geometry.setTagName("geometry");

    //add corresponding acutal geometry
    if(!this->myActual.isNull()){
        QDomElement actual = xmlDoc.createElement("actual");
        actual.setAttribute("ref", this->myActual->getId());
        geometry.appendChild(actual);
    }

    //add nominals
    if(this->myNominals.size()>0){
        QDomElement nominals = xmlDoc.createElement("nominalGeometries");
        foreach (const QPointer<Geometry> &geom, this->myNominals){
            if(!geom.isNull()){
                QDomElement nominal = xmlDoc.createElement("geometry");
                nominal.setAttribute("ref", geom->getId());
                nominals.appendChild(nominal);
            }
        }
        geometry.appendChild(nominals);
    }

    //add measurement config
    QDomElement mConfig = xmlDoc.createElement("measurementConfig");
    mConfig.setAttribute("name",this->activeMeasurementConfig.getName());
    geometry.appendChild(mConfig);

    return geometry;
}

/*!
 * \brief MasterGeometry::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool MasterGeometry::fromOpenIndyXML(QDomElement &xmlElem)
{
    bool result = Feature::fromOpenIndyXML(xmlElem);

    return result;
}

/*!
 * \brief MasterGeometry::changeNameofGeometries
 */
void MasterGeometry::changeNameOfGeometries()
{
    if(!this->myActual.isNull()){
        this->myActual->setFeatureName(this->getFeatureName());
    }
    foreach (QPointer<Geometry> geom, this->myNominals) {
        geom->setFeatureName(this->getFeatureName());
    }
}

/*!
 * \brief MasterGeometry::changeGroupOfGeometries
 */
void MasterGeometry::changeGroupOfGeometries()
{
    if(!this->myActual.isNull()){
        this->myActual->setGroupName(this->getGroupName());
    }
    foreach (QPointer<Geometry> geom, this->myNominals) {
        geom->setGroupName(this->getGroupName());
    }
}

/*!
 * \brief MasterGeometry::recalc
 */
void MasterGeometry::recalc()
{
}

/*!
 * \brief MasterGeometry::getMeasurementConfig
 * \return
 */
const MeasurementConfig &MasterGeometry::getMeasurementConfig() const
{
    return this->activeMeasurementConfig;
}

/*!
 * \brief MasterGeometry::setMeasurementConfig
 * \param myConfig
 */
void MasterGeometry::setMeasurementConfig(const MeasurementConfig &myConfig)
{
    QString oldName = this->activeMeasurementConfig.getName();
    bool oldIsSaved = this->activeMeasurementConfig.getIsSaved();
    this->activeMeasurementConfig = myConfig;
    emit this->geomMeasurementConfigChanged(this->id, oldName, oldIsSaved);
}

/*!
 * \brief MasterGeometry::checkActualNominaltype compares the actual and nominal feature type
 * \param actual
 * \param nominal
 * \return
 */
bool MasterGeometry::checkActualNominaltype(QPointer<Geometry> actual, QPointer<Geometry> nominal)
{
    if(!actual.isNull() && !nominal.isNull() && actual->getFeatureWrapper()->getFeatureTypeEnum() == nominal->getFeatureWrapper()->getFeatureTypeEnum()){
        return true;
    }
    return false;
}
