#include "featurecontainer.h"

using namespace oi;

/*!
 * \brief FeatureContainer::FeatureContainer
 */
FeatureContainer::FeatureContainer(){

}

/*!
 * \brief FeatureContainer::~FeatureContainer
 */
FeatureContainer::~FeatureContainer(){

}

/*!
 * \brief FeatureContainer::getFeaturesList
 * \return
 */
const QList<QPointer<FeatureWrapper> > &FeatureContainer::getFeaturesList() const{
    return this->featuresList;
}

/*!
 * \brief FeatureContainer::getCoordinateSystemsList
 * \return
 */
const QList<QPointer<CoordinateSystem> > &FeatureContainer::getCoordinateSystemsList() const{
    return this->coordSystems;
}

/*!
 * \brief FeatureContainer::getStationSystemsList
 * \return
 */
QList<QPointer<CoordinateSystem> > FeatureContainer::getStationSystemsList() const{
    QList<QPointer<CoordinateSystem> > stationSystems;
    foreach(const QPointer<Station> &station, this->stationsList){
        if(!station.isNull() && !station->getCoordinateSystem().isNull()){
            stationSystems.append(station->getCoordinateSystem());
        }
    }
    return stationSystems;
}

/*!
 * \brief FeatureContainer::getBundleSystemList
 * \return
 */
QList<QPointer<CoordinateSystem> > FeatureContainer::getBundleSystemList() const{
    QList<QPointer<CoordinateSystem> > bundleSystems;
    foreach(const QPointer<CoordinateSystem> &system, this->coordSystems){
        if(!system.isNull() && system->getIsBundleSystem()){
            bundleSystems.append(system);
        }
    }
    return bundleSystems;
}

/*!
 * \brief FeatureContainer::getStationsList
 * \return
 */
const QList<QPointer<Station> > &FeatureContainer::getStationsList() const{
    return this->stationsList;
}

/*!
 * \brief FeatureContainer::getTransformationParametersList
 * \return
 */
const QList<QPointer<TrafoParam> > &FeatureContainer::getTransformationParametersList() const{
    return this->trafoParamsList;
}

/*!
 * \brief FeatureContainer::getGeometriesList
 * \return
 */
const QList<QPointer<FeatureWrapper> > &FeatureContainer::getGeometriesList() const{
    return this->geometriesList;
}

/*!
 * \brief FeatureContainer::getFeatureIdList
 * \return
 */
const QList<int> &FeatureContainer::getFeatureIdList() const{
    return this->featureIds;
}

/*!
 * \brief FeatureContainer::getFeatureNameList
 * \return
 */
const QStringList &FeatureContainer::getFeatureNameList() const{
    return this->featureNames;
}

/*!
 * \brief FeatureContainer::getFeatureGroupList
 * \return
 */
const QStringList &FeatureContainer::getFeatureGroupList() const{
    return this->featureGroups;
}

/*!
 * \brief FeatureContainer::getUsedMeasurementConfigs
 * \return
 */
const QList<QPair<QString, bool> > &FeatureContainer::getUsedMeasurementConfigs() const{
    return this->usedMConfigs;
}

/*!
 * \brief FeatureContainer::getFeatureById
 * \param featureId
 * \return
 */
QPointer<FeatureWrapper> FeatureContainer::getFeatureById(const int &featureId) const{
    return this->featuresIdMap.value(featureId, QPointer<FeatureWrapper>(NULL));
}

/*!
 * \brief FeatureContainer::getFeaturesByName
 * \param name
 * \return
 */
QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByName(const QString &name) const{
    return this->featuresNameMap.values(name);
}

/*!
 * \brief FeatureContainer::getFeaturesByGroup
 * \param group
 * \return
 */
QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByGroup(const QString &group) const{
    return this->featuresGroupMap.values(group);
}

/*!
 * \brief FeatureContainer::getFeaturesByType
 * \param type
 * \return
 */
QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByType(const FeatureTypes &type) const{
    return this->featuresTypeMap.values(type);
}

/*!
 * \brief FeatureContainer::getGeometriesByMConfig
 * \param mConfig
 * \return
 */
QList<QPointer<MasterGeometry> > FeatureContainer::getGeometriesByMConfig(const QPair<QString, bool> &mConfig) const{
    return this->geometriesMConfigMap.values(mConfig);
}

/*!
 * \brief FeatureContainer::getFeatureCount
 * \return
 */
int FeatureContainer::getFeatureCount() const{
    return this->featuresList.size();
}

/*!
 * \brief FeatureContainer::getGeometryCount
 * \return
 */
int FeatureContainer::getGeometryCount() const{
    return this->geometriesList.size();
}

/*!
 * \brief FeatureContainer::getFeatureCount
 * \param type
 * \return
 */
int FeatureContainer::getFeatureCount(const FeatureTypes &type) const{
    return this->featuresTypeMap.count(type);
}

/*!
 * \brief FeatureContainer::getFeatureCount
 * \param group
 * \return
 */
int FeatureContainer::getFeatureCount(const QString &group) const{
    return this->featuresGroupMap.count(group);
}

/*!
 * \brief FeatureContainer::getFeatureCount
 * \param mConfig
 * \return
 */
int FeatureContainer::getFeatureCount(const QPair<QString, bool> &mConfig) const{
    return this->geometriesMConfigMap.count(mConfig);
}

/*!
 * \brief FeatureContainer::addFeature
 * \param feature
 * \return
 */
bool FeatureContainer::addFeature(const QPointer<FeatureWrapper> &feature){

    //if the feature is not valid
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    this->addToFeatureIDMap(feature);
    this->featuresTypeMap.insert(feature->getFeatureTypeEnum(), feature);

    //add the feature to the feature lists and maps
    //this->featuresList.append(feature);
    //this->featuresNameMap.insert(feature->getFeature()->getFeatureName(), feature);
    //this->featuresTypeMap.insert(feature->getFeatureTypeEnum(), feature);

    /*if(feature->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), feature);
    }*/
    switch(feature->getFeatureTypeEnum()){
    case eCoordinateSystemFeature:
        this->coordSystems.append(feature->getCoordinateSystem());
        break;
    case eStationFeature:
        this->stationsList.append(feature->getStation());
        if(!feature->getStation()->getCoordinateSystem().isNull()){
            QPointer<FeatureWrapper> stationSystem = new FeatureWrapper();
            stationSystem->setCoordinateSystem(feature->getStation()->getCoordinateSystem());
            this->featuresIdMap.insert(feature->getStation()->getCoordinateSystem()->getId(), stationSystem);
        }
        break;
    case eTrafoParamFeature:
        this->trafoParamsList.append(feature->getTrafoParam());
        break;
    case eMasterGeometryFeature:
            if(!feature->getMasterGeometry().isNull() && !this->geometriesList.contains(feature)){
                this->geometriesList.append(feature);
                this->addToFeatureList(feature);
                this->featuresTypeMap.insert(feature->getFeatureTypeEnum(),feature);
                this->addToFeatureGroupMap(feature);
                //set mconfig map
                this->addToGeomMConfigMap(feature->getMasterGeometry());
            }
            break;
    default:// all geometries
        if(feature->getGeometry().isNull()){
            break;
        }
        this->featuresTypeMap.insert(feature->getFeatureTypeEnum(),feature);

        QPointer<FeatureWrapper> existingMasterGeom;

        foreach (QPointer<FeatureWrapper> fwMasterGeom, this->geometriesList) {
            if(fwMasterGeom->getMasterGeometry().isNull()){
                break;
            }

            if(!feature->getGeometry()->getIsNominal() && !fwMasterGeom->getMasterGeometry()->getActual().isNull()){

                if(feature->getFeatureTypeEnum() == fwMasterGeom->getMasterGeometry()->getActual()->getFeatureWrapper()->getFeatureTypeEnum()
                        && feature->getFeature()->getFeatureName() == fwMasterGeom->getMasterGeometry()->getActual()->getFeatureName()){
                    existingMasterGeom = fwMasterGeom;
                }
            }else if(!feature->getGeometry()->getIsNominal() && fwMasterGeom->getMasterGeometry()->getActual().isNull()){

                foreach (QPointer<Geometry> geom, fwMasterGeom->getMasterGeometry()->getNominals()) {
                    if(geom->getFeatureName() == feature->getFeature()->getFeatureName()
                            && geom->getFeatureWrapper()->getFeatureTypeEnum() == feature->getFeatureTypeEnum()){

                        fwMasterGeom->getMasterGeometry()->setActual(feature->getGeometry());

                        //check group names
                        this->verifyAndAddFeatureGroupMap(feature,fwMasterGeom);
                        this->addToGeomMConfigMap(fwMasterGeom->getMasterGeometry());
                        existingMasterGeom = fwMasterGeom;
                        break;
                    }
                }
            }else if(feature->getGeometry()->getIsNominal() && !fwMasterGeom->getMasterGeometry()->getActual().isNull()){

                if(feature->getGeometry()->getFeatureName() == fwMasterGeom->getMasterGeometry()->getActual()->getFeatureName()
                        && feature->getFeatureTypeEnum() == fwMasterGeom->getMasterGeometry()->getActual()->getFeatureWrapper()->getFeatureTypeEnum()){

                    if(!fwMasterGeom->getMasterGeometry()->getNominals().contains(feature->getGeometry())){
                        fwMasterGeom->getMasterGeometry()->addNominal(feature->getGeometry());

                        //check group names
                        this->verifyAndAddFeatureGroupMap(feature, fwMasterGeom);
                        existingMasterGeom = fwMasterGeom;
                    }
                }
            }else if(feature->getGeometry()->getIsNominal() && fwMasterGeom->getMasterGeometry()->getActual().isNull()){

                if(fwMasterGeom->getMasterGeometry()->getNominals().contains(feature->getGeometry())){
                    existingMasterGeom = fwMasterGeom;
                }else{
                    bool coordSysExists = false;
                    bool sameName = false;
                    bool sameType = false;
                    foreach (QPointer<Geometry> geom, fwMasterGeom->getMasterGeometry()->getNominals()) {
                        if(geom->getFeatureName() == feature->getFeature()->getFeatureName() && geom->getFeatureWrapper()->getFeatureTypeEnum() == feature->getFeatureTypeEnum()){

                            sameName = true;
                            sameType = true;
                            if(geom->getNominalSystem() == feature->getGeometry()->getNominalSystem()){
                                coordSysExists = true;
                            }
                        }
                    }
                    if(!coordSysExists && sameName && sameType){
                        fwMasterGeom->getMasterGeometry()->addNominal(feature->getGeometry());
                        this->verifyAndAddFeatureGroupMap(feature,fwMasterGeom);
                        existingMasterGeom = fwMasterGeom;
                    }
                }
            }
        }

        if(existingMasterGeom.isNull()){
            //erzeuge mastergeom
            this->createNewMasterGeomFromFeature(feature);
        }
        break;
    }
    return true;
}

/*!
 * \brief FeatureContainer::removeFeature
 * \param featureId
 * \return
 */
bool FeatureContainer::removeFeature(const int &featureId){

    //check if the feature exists
    if(!this->featuresIdMap.contains(featureId)){
        return false;
    }

    //get the feature pointer
    QPointer<FeatureWrapper> feature = this->featuresIdMap.value(featureId, QPointer<FeatureWrapper>());

    //check if the feature is valid
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    switch(feature->getFeatureTypeEnum()){
    case eCoordinateSystemFeature:
        this->coordSystems.removeOne(feature->getCoordinateSystem());
        this->removeFromFeatureGroupMap(feature);
        this->removeFromFeatureNameMap(feature);
        break;
    case eStationFeature:
        this->stationsList.removeOne(feature->getStation());
        this->removeFromFeatureGroupMap(feature);
        this->removeFromFeatureNameMap(feature);
        break;
    case eTrafoParamFeature:
        this->trafoParamsList.removeOne(feature->getTrafoParam());
        this->removeFromFeatureGroupMap(feature);
        this->removeFromFeatureNameMap(feature);
        break;
    case eMasterGeometryFeature:
        this->geometriesList.removeOne(feature);
        this->removeFromFeatureList(feature);
        this->removeFromGeomMConfigMap(feature->getMasterGeometry());

        //remove actual feature from lists
        this->featuresTypeMap.remove(feature->getMasterGeometry()->getActual()->getFeatureWrapper()->getFeatureTypeEnum(),
                                     feature->getMasterGeometry()->getActual()->getFeatureWrapper());
        this->removeFromFeatureIDMap(feature->getMasterGeometry()->getActual()->getFeatureWrapper());

        //remove nominal features from lists
        foreach (QPointer<Geometry>geom, feature->getMasterGeometry()->getNominals()) {
            this->featuresTypeMap.remove(geom->getFeatureWrapper()->getFeatureTypeEnum(), geom->getFeatureWrapper());
            this->removeFromFeatureIDMap(geom->getFeatureWrapper());
        }
    default: //geometry
        //delete geometry and also check if mastergeometry still hast attributes. if no, also delete mastergeometry
        foreach (QPointer<FeatureWrapper> fw, this->geometriesList) {
            if(fw->getFeatureTypeEnum() == eMasterGeometryFeature){
                //actual
                if(fw->getMasterGeometry()->getActual() == feature->getGeometry()){
                    fw->getMasterGeometry()->removeActual(feature->getGeometry());
                }
                //nominal
                if(fw->getMasterGeometry()->getNominals().contains(feature->getGeometry())){
                    fw->getMasterGeometry()->removeNominal(feature->getGeometry());
                }
                //check if mastergeometry is still valid
                if(fw->getMasterGeometry()->getActual().isNull() && fw->getMasterGeometry()->getNominals().size() == 0){
                    this->removeFeature(fw->getFeature()->getId());
                }
            }
        }
        break;
    }
    this->featuresTypeMap.remove(feature->getFeatureTypeEnum(), feature);
    this->removeFromFeatureIDMap(feature);

    //delete the feature
    delete feature->getFeature();
    delete feature;

    return true;
}

/*!
 * \brief FeatureContainer::checkAndClean
 * Checks wether there is a feature with the given id and removes it
 * \param featureId
 * \param name
 * \param group
 * \param type
 */
void FeatureContainer::checkAndClean(const int &featureId, const QString &name, const QString &group, const FeatureTypes &type){

    if(!this->featuresIdMap.contains(featureId)){
        return;
    }

    //get and check feature by id
    QPointer<FeatureWrapper> feature = this->featuresIdMap.value(featureId);
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //clean feature list
    //this->featuresList.removeOne(feature);
    this->removeFromFeatureList(feature);
    this->removeFromFeatureNameMap(feature);
    this->removeFromFeatureGroupMap(feature);

    this->removeFromFeatureIDMap(feature);

    this->featuresTypeMap.remove(type, feature);

    //clean coordinate system list
    if(!feature->getCoordinateSystem().isNull()){
        this->coordSystems.removeOne(feature->getCoordinateSystem());
    }

    //clean station list
    if(!feature->getStation().isNull()){
        this->stationsList.removeOne(feature->getStation());
    }

    //clean trafo param list
    if(!feature->getTrafoParam().isNull()){
        this->trafoParamsList.removeOne(feature->getTrafoParam());
    }
    //clean geometry list if feature is a master geometry
    if(!feature->getMasterGeometry().isNull()){
        this->geometriesList.removeOne(feature);
        this->removeFromGeomMConfigMap(feature->getMasterGeometry());
    }
    //clean geometry list if feature is a regular geometry
    if(!feature->getGeometry().isNull()){
        foreach (QPointer<FeatureWrapper> fw, this->geometriesList) {
            if(fw->getFeatureTypeEnum() == eMasterGeometryFeature){
                //actual
                if(fw->getMasterGeometry()->getActual() == feature->getGeometry()){
                    fw->getMasterGeometry()->removeActual(feature->getGeometry());
                }
                //nominal
                if(fw->getMasterGeometry()->getNominals().contains(feature->getGeometry())){
                    fw->getMasterGeometry()->removeNominal(feature->getGeometry());
                }
                //check if mastergeometry is still valid
                if(fw->getMasterGeometry()->getActual().isNull() && fw->getMasterGeometry()->getNominals().size() == 0){
                    this->removeFeature(fw->getFeature()->getId());
                }
            }
        }
    }
}

/*!
 * \brief FeatureContainer::removeAll
 * Remove and delete all features
 */
void FeatureContainer::removeAll(){

    //delete all features
    foreach(const QPointer<FeatureWrapper> &feature, this->featuresList){
        if(!feature.isNull()){
            if(feature->getFeature().isNull()){
                delete feature->getFeature();
            }
            delete feature;
        }
    }

    //clear all lists and maps
    this->featuresList.clear();
    this->coordSystems.clear();
    this->stationsList.clear();
    this->trafoParamsList.clear();
    this->geometriesList.clear();
    this->featuresIdMap.clear();
    this->featuresNameMap.clear();
    this->featuresGroupMap.clear();
    this->featuresTypeMap.clear();
    this->geometriesMConfigMap.clear();
    this->featureIds.clear();
    this->featureNames.clear();
    this->featureGroups.clear();
    this->usedMConfigs.clear();
}

/*!
 * \brief FeatureContainer::featureNameChanged
 * Is called whenever the name of a feature has changed
 * \param featureId
 * \param oldName
 * \return
 */
bool FeatureContainer::featureNameChanged(const int &featureId, const QString &oldName){

    //check if the feature exists
    if(!this->featuresIdMap.contains(featureId)){
        return false;
    }

    //get the feature pointer
    QPointer<FeatureWrapper> feature = this->featuresIdMap.value(featureId, QPointer<FeatureWrapper>());

    //check if the feature is valid
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }
    QPointer<FeatureWrapper> finalWrapper;

    if(feature->getFeatureTypeEnum() == eMasterGeometryFeature){
        finalWrapper = feature;
    }else if(getIsGeometry(feature->getFeatureTypeEnum())){
        finalWrapper = feature->getGeometry()->getMasterGeometry()->getFeatureWrapper();
    }

    //update lists and maps
    if(this->featuresNameMap.contains(oldName)){
        this->featuresNameMap.insert(finalWrapper->getFeature()->getFeatureName(), this->featuresNameMap.take(oldName));
    }
    if(!this->featuresNameMap.contains(oldName)){
        this->featureNames.removeOne(oldName);
    }
    if(!this->featureNames.contains(finalWrapper->getFeature()->getFeatureName())){
        this->featureNames.append(finalWrapper->getFeature()->getFeatureName());
    }
    return true;
}

/*!
 * \brief FeatureContainer::featureGroupChanged
 * Is called whenever the group of a feature has changed
 * \param featureId
 * \param oldGroup
 * \return
 */
bool FeatureContainer::featureGroupChanged(const int &featureId, const QString &oldGroup){

    //check if the feature exists
    if(!this->featuresIdMap.contains(featureId)){
        return false;
    }

    //get the feature pointer
    QPointer<FeatureWrapper> feature = this->featuresIdMap.value(featureId, QPointer<FeatureWrapper>());

    //check if the feature is valid
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //check if the old group was empty
    if(oldGroup.compare("") == 0){

        //check if the new group is empty
        if(feature->getFeature()->getGroupName().compare("") != 0){
            if(!feature->getMasterGeometry().isNull()){
                this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), feature);
                if(!this->featureGroups.contains(feature->getFeature()->getGroupName())){
                    this->featureGroups.append(feature->getFeature()->getGroupName());
                }
                feature->getMasterGeometry()->changeGroupOfGeometries();

            }else if(getIsGeometry(feature->getFeatureTypeEnum())){
                this->featuresGroupMap.insert(feature->getGeometry()->getGroupName(), feature->getMasterGeometry()->getFeatureWrapper());
                if(!this->featuresGroupMap.contains(feature->getFeature()->getGroupName())){
                    this->featureGroups.append(feature->getFeature()->getGroupName());
                    feature->getGeometry()->getMasterGeometry()->setGroupName(feature->getFeature()->getGroupName());
                    feature->getGeometry()->getMasterGeometry()->changeGroupOfGeometries();
                }
            }
        }
    }else{
        //check if the new group is empty
        if(feature->getFeature()->getGroupName().compare("") != 0){

            if(feature->getFeatureTypeEnum() == eMasterGeometryFeature){

                feature->getMasterGeometry()->changeGroupOfGeometries();
                this->featuresGroupMap.remove(oldGroup,feature);
                this->featuresGroupMap.insert(feature->getFeature()->getGroupName(),feature);

            }else if(getIsGeometry(feature->getFeatureTypeEnum())){

                feature->getGeometry()->getMasterGeometry()->setFeatureName(feature->getFeature()->getFeatureName());
                feature->getGeometry()->getMasterGeometry()->changeGroupOfGeometries();
                this->featuresGroupMap.remove(oldGroup,feature);
                this->featuresGroupMap.insert(feature->getFeature()->getGroupName(),feature->getGeometry()->getMasterGeometry()->getFeatureWrapper());
            }
            if(!this->featuresGroupMap.contains(oldGroup)){
                this->featureGroups.removeOne(oldGroup);
            }
            if(!this->featureGroups.contains(feature->getFeature()->getGroupName())){
                this->featureGroups.append(feature->getFeature()->getGroupName());
            }
        }else{
            if(!feature->getMasterGeometry().isNull()){
                feature->getMasterGeometry()->changeGroupOfGeometries();
                this->featuresGroupMap.remove(oldGroup, feature);
                if(!this->featuresGroupMap.contains(oldGroup)){
                    this->featureGroups.removeOne(oldGroup);
                }
            }else if(getIsGeometry(feature->getFeatureTypeEnum())){
                feature->getGeometry()->getMasterGeometry()->setGroupName(feature->getFeature()->getFeatureName());
                feature->getGeometry()->getMasterGeometry()->changeGroupOfGeometries();
                this->featuresGroupMap.remove(oldGroup, feature->getGeometry()->getMasterGeometry()->getFeatureWrapper());
                if(!this->featuresGroupMap.contains(oldGroup)){
                    this->featureGroups.removeOne(oldGroup);
                }
            }
        }
    }
    return true;
}

/*!
 * \brief FeatureContainer::geometryMeasurementConfigChanged
 * \param featureId
 * \param oldMConfig
 * \param oldIsSaved
 * \return
 */
bool FeatureContainer::geometryMeasurementConfigChanged(const int &featureId, const QString &oldMConfig, bool oldIsSaved){

    //check if the feature exists
    if(!this->featuresIdMap.contains(featureId)){
        return false;
    }

    //get the feature pointer
    QPointer<FeatureWrapper> feature = this->featuresIdMap.value(featureId, QPointer<FeatureWrapper>());

    //check if the feature is valid and is a geometry
    if(feature.isNull() || feature->getGeometry().isNull()){
        return false;
    }
    QPointer<MasterGeometry> mGeometry = feature->getMasterGeometry();

    //if both, old and new mConfig, are empty nothing should happen
    if(oldMConfig.compare("") == 0 && !mGeometry->getMeasurementConfig().getIsValid()){
        return true;
    }

    //get map-keys for old and new mConfig
    QPair<QString, bool> oldKey, newKey;
    oldKey.first = oldMConfig;
    oldKey.second = oldIsSaved;
    newKey.first = mGeometry->getMeasurementConfig().getName();
    newKey.second = mGeometry->getMeasurementConfig().getIsSaved();

    //if the old mConfig was empty
    if(oldMConfig.compare("") == 0){
        this->geometriesMConfigMap.insert(newKey, mGeometry);
        if(!this->usedMConfigs.contains(newKey)){
            this->usedMConfigs.append(newKey);
        }
        return true;
    }

    //if the new mConfig is empty
    if(!mGeometry->getMeasurementConfig().getIsValid()){
        this->geometriesMConfigMap.remove(oldKey, mGeometry);
        if(!this->geometriesMConfigMap.contains(oldKey)){
            this->usedMConfigs.removeOne(oldKey);
        }
        return true;
    }

    //if both mConfigs are non-empty
    this->geometriesMConfigMap.remove(oldKey, mGeometry);
    this->geometriesMConfigMap.insert(newKey, mGeometry);
    if(!this->geometriesMConfigMap.contains(oldKey)){
        this->usedMConfigs.removeOne(oldKey);
    }
    if(!this->usedMConfigs.contains(newKey)){
        this->usedMConfigs.append(newKey);
    }

    return true;

}

/*!
 * \brief FeatureContainer::addToFeatureList
 * \param fw
 */
void FeatureContainer::addToFeatureList(QPointer<FeatureWrapper> fw)
{
    if(!fw->getGeometry().isNull()){
        return;
    }
    if(!this->featuresList.contains(fw)){
        this->featuresList.append(fw);
        this->addToFeatureNameMap(fw);
    }
}

/*!
 * \brief FeatureContainer::addToFeatureNampeMap
 * \param fw
 */
void FeatureContainer::addToFeatureNameMap(QPointer<FeatureWrapper> fw)
{
    if(!fw->getGeometry().isNull()){
        return;
    }
    this->featuresNameMap.insert(fw->getFeature()->getFeatureName(),fw);
    if(!this->featureNames.contains(fw->getFeature()->getFeatureName())){
        this->featureNames.append(fw->getFeature()->getFeatureName());
    }
}

/*!
 * \brief FeatureContainer::addToFeatureGroupMap
 * \param fw
 */
void FeatureContainer::addToFeatureGroupMap(QPointer<FeatureWrapper> fw)
{
    if(!fw->getGeometry().isNull()){
        return;
    }
    if(fw->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.insert(fw->getFeature()->getGroupName(),fw);
        if(!this->featureGroups.contains(fw->getFeature()->getGroupName())){
            this->featureGroups.append(fw->getFeature()->getGroupName());
        }
    }
}

/*!
 * \brief FeatureContainer::verifyAndAddFeatureGroupMap
 * \param fw
 * \param masterGeom
 */
void FeatureContainer::verifyAndAddFeatureGroupMap(QPointer<FeatureWrapper> fw, QPointer<FeatureWrapper> masterGeom)
{
    if(!fw->getMasterGeometry().isNull()){
        return;
    }
    if(fw->getFeature()->getGroupName().compare("") != 0){
        if(masterGeom->getFeature()->getGroupName().compare("") == 0){
            masterGeom->getFeature()->setGroupName(fw->getFeature()->getGroupName());
            this->addToFeatureGroupMap(masterGeom);
        }
    }
}

/*!
 * \brief FeatureContainer::addToGeomMConfigMap
 * \param masterGeom
 */
void FeatureContainer::addToGeomMConfigMap(QPointer<MasterGeometry> masterGeom)
{
    if(!masterGeom.isNull() && masterGeom->getMeasurementConfig().getIsValid()){
        QPair<QString, bool> mConfig;
        mConfig.first = masterGeom->getMeasurementConfig().getName();
        mConfig.second = masterGeom->getMeasurementConfig().getIsSaved();
        this->geometriesMConfigMap.insert(mConfig, masterGeom);
    }
    if(!masterGeom.isNull() && masterGeom->getMeasurementConfig().getIsValid()){
        this->usedMConfigs.append(QPair<QString,bool>(masterGeom->getMeasurementConfig().getName(),
                                                      masterGeom->getMeasurementConfig().getIsSaved()));
    }
}

/*!
 * \brief FeatureContainer::addToFeatureIDMap
 * \param fw
 */
void FeatureContainer::addToFeatureIDMap(QPointer<FeatureWrapper> fw)
{
    //if the feature already exists it is not added
    if(this->featuresIdMap.contains(fw->getFeature()->getId())){
        return;
    }
    this->featuresIdMap.insert(fw->getFeature()->getId(),fw);
    if(!this->featureIds.contains(fw->getFeature()->getId())){
        this->featureIds.append(fw->getFeature()->getId());
    }
}

/*!
 * \brief FeatureContainer::removeFromFeatureList
 * \param fw
 */
void FeatureContainer::removeFromFeatureList(QPointer<FeatureWrapper> fw)
{
    if(!fw->getGeometry().isNull()){
        return;
    }
    if(this->featuresList.contains(fw)){
        this->featuresList.removeOne(fw);
        this->removeFromFeatureNameMap(fw);
    }
}

/*!
 * \brief FeatureContainer::removeFromFeatureNameMap
 * \param fw
 */
void FeatureContainer::removeFromFeatureNameMap(QPointer<FeatureWrapper> fw)
{
    if(!fw->getGeometry().isNull()){
        return;
    }
    if(this->featuresNameMap.contains(fw->getFeature()->getFeatureName(),fw)){
        this->featuresNameMap.remove(fw->getFeature()->getFeatureName(),fw);
        if(!this->featuresNameMap.keys().contains(fw->getFeature()->getFeatureName())){
            this->featureNames.removeOne(fw->getFeature()->getFeatureName());
        }
    }
}
/*!
 * \brief FeatureContainer::removeFromFeatureGroupMap
 * \param fw
 */
void FeatureContainer::removeFromFeatureGroupMap(QPointer<FeatureWrapper> fw)
{
    if(!fw->getGeometry().isNull()){
        return;
    }
    if(fw->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.remove(fw->getFeature()->getGroupName(),fw);
        if(!this->featuresGroupMap.keys().contains(fw->getFeature()->getGroupName())){
            this->featureGroups.removeOne(fw->getFeature()->getGroupName());
        }
        this->featuresGroupMap.value("");
    }

}

/*!
 * \brief FeatureContainer::removeFromGeomMConfigMap
 * \param masterGeom
 */
void FeatureContainer::removeFromGeomMConfigMap(QPointer<MasterGeometry> masterGeom)
{
    if(masterGeom->getMeasurementConfig().getIsValid()){
        QPair<QString, bool> mConfig;
        mConfig.first = masterGeom->getMeasurementConfig().getName();
        mConfig.second = masterGeom->getMeasurementConfig().getIsSaved();
        this->geometriesMConfigMap.remove(mConfig, masterGeom);
        if(!this->geometriesMConfigMap.contains(mConfig)){
            this->usedMConfigs.removeOne(mConfig);
        }
    }
}
/*!
 * \brief FeatureContainer::removeFromFeatureIDMap
 * \param fw
 */
void FeatureContainer::removeFromFeatureIDMap(QPointer<FeatureWrapper> fw)
{
    this->featuresIdMap.remove(fw->getFeature()->getId());
    this->featureIds.removeOne(fw->getFeature()->getId());
}

/*!
 * \brief FeatureContainer::createNewMasterGeomfromFeature
 * \param feature
 */
void FeatureContainer::createNewMasterGeomFromFeature(QPointer<FeatureWrapper> feature)
{
    QPointer<FeatureWrapper> featWMasterGeom = new FeatureWrapper();

    QPointer<MasterGeometry> newMasterGeometry = new MasterGeometry();
    newMasterGeometry->setFeatureName(feature->getFeature()->getFeatureName());
    newMasterGeometry->setGroupName(feature->getFeature()->getGroupName());

    if(feature->getGeometry()->getIsNominal()){
        newMasterGeometry->addNominal(feature->getGeometry());
    }else{
        newMasterGeometry->setActual(feature->getGeometry());
        this->addToGeomMConfigMap(newMasterGeometry);
    }
    featWMasterGeom->setMasterGeometry(newMasterGeometry);

    this->addToFeatureList(featWMasterGeom);
    this->geometriesList.append(featWMasterGeom);
    this->addToFeatureIDMap(featWMasterGeom);
    this->addToFeatureNameMap(featWMasterGeom);
    this->featuresTypeMap.insert(featWMasterGeom->getFeatureTypeEnum(),featWMasterGeom);
    this->addToFeatureGroupMap(featWMasterGeom);
}
