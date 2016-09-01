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

    //if the feature already exists it is not added
    if(this->featuresIdMap.contains(feature->getFeature()->getId())){
        return false;
    }

    //add the feature to the feature lists and maps
    //this->featuresList.append(feature);
    this->featuresIdMap.insert(feature->getFeature()->getId(), feature);
    //this->featuresNameMap.insert(feature->getFeature()->getFeatureName(), feature);
    this->featuresTypeMap.insert(feature->getFeatureTypeEnum(), feature);

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

                if(!this->featuresList.contains(feature)){
                    this->featuresList.append(feature);
                    this->featuresNameMap.insert(feature->getFeature()->getFeatureName(), feature);

                    if(this->featuresNameMap.values(feature->getFeature()->getFeatureName()).size() == 1){
                        this->featureNames.append(feature->getFeature()->getFeatureName());
                    }

                    this->featuresTypeMap.insert(feature->getFeatureTypeEnum(),feature);

                    if(feature->getFeature()->getGroupName().compare("") != 0){
                            this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), feature);
                    }
                    if(feature->getFeature()->getGroupName().compare("") != 0 &&
                            !this->featureGroups.contains(feature->getFeature()->getGroupName())){
                        this->featureGroups.append(feature->getFeature()->getGroupName());
                    }
                }
                //set mconfig map
                if(!feature->getMasterGeometry().isNull() && feature->getMasterGeometry()->getMeasurementConfig().getIsValid()){
                    QPair<QString, bool> key;
                    key.first = feature->getMasterGeometry()->getMeasurementConfig().getName();
                    key.second = feature->getMasterGeometry()->getMeasurementConfig().getIsSaved();
                    this->geometriesMConfigMap.insert(key, feature->getMasterGeometry());
                }
            }
            break;
    default:// all geometries
        if(feature->getGeometry().isNull()){
            break;
        }
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
                        if(feature->getFeature()->getGroupName().compare("") != 0){
                            if(fwMasterGeom->getFeature()->getGroupName().compare("") == 0){
                                fwMasterGeom->getFeature()->setGroupName(feature->getFeature()->getGroupName());
                                this->featuresGroupMap.insert(fwMasterGeom->getFeature()->getGroupName(),fwMasterGeom);
                                if(this->featureGroups.contains(fwMasterGeom->getFeature()->getGroupName()) != 0){
                                    this->featureGroups.append(fwMasterGeom->getFeature()->getGroupName());
                                }
                            }
                        }

                        if(!fwMasterGeom->getMasterGeometry().isNull() && fwMasterGeom->getMasterGeometry()->getMeasurementConfig().getIsValid()){
                            QPair<QString, bool> key;
                            key.first = fwMasterGeom->getMasterGeometry()->getMeasurementConfig().getName();
                            key.second = fwMasterGeom->getMasterGeometry()->getMeasurementConfig().getIsSaved();
                            this->geometriesMConfigMap.insert(key, fwMasterGeom->getMasterGeometry());
                        }
                        existingMasterGeom = fwMasterGeom;
                        break;
                    }
                }
            }else if(feature->getGeometry()->getIsNominal() && !fwMasterGeom->getMasterGeometry()->getActual().isNull()){

                if(feature->getGeometry()->getFeatureName() == fwMasterGeom->getMasterGeometry()->getActual()->getFeatureName()
                        && feature->getFeatureTypeEnum() == fwMasterGeom->getMasterGeometry()->getActual()->getFeatureWrapper()->getFeatureTypeEnum()){

                    if(!fwMasterGeom->getMasterGeometry()->getNominals().contains(feature->getGeometry())){
                        fwMasterGeom->getMasterGeometry()->addNominal(feature->getGeometry());
                        existingMasterGeom = fwMasterGeom;
                    }
                }
            }else if(feature->getGeometry()->getIsNominal() && fwMasterGeom->getMasterGeometry()->getActual().isNull()){

                if(fwMasterGeom->getMasterGeometry()->getNominals().contains(feature->getGeometry())){
                    existingMasterGeom = fwMasterGeom;
                }else{
                    foreach (QPointer<Geometry> geom, fwMasterGeom->getMasterGeometry()->getNominals()) {
                        if(geom->getFeatureName() == feature->getFeature()->getFeatureName()
                                && geom->getFeatureWrapper()->getFeatureTypeEnum() == feature->getFeatureTypeEnum()){

                            fwMasterGeom->getMasterGeometry()->addNominal(feature->getGeometry());
                            existingMasterGeom = fwMasterGeom;
                        }
                    }
                }
            }
        }

        if(existingMasterGeom.isNull()){
            //erzeuge mastergeom
            QPointer<FeatureWrapper> featWMasterGeom = new FeatureWrapper();

            QPointer<MasterGeometry> newMasterGeometry = new MasterGeometry();
            newMasterGeometry->setFeatureName(feature->getFeature()->getFeatureName());
            newMasterGeometry->setGroupName(feature->getFeature()->getGroupName());

            if(feature->getGeometry()->getIsNominal()){
                newMasterGeometry->addNominal(feature->getGeometry());
            }else{
                newMasterGeometry->setActual(feature->getGeometry());
            }
            featWMasterGeom->setMasterGeometry(newMasterGeometry);

            this->featuresList.append(featWMasterGeom);
            this->geometriesList.append(featWMasterGeom);
            this->featuresIdMap.insert(featWMasterGeom->getFeature()->getId(),featWMasterGeom);
            this->featuresNameMap.insert(featWMasterGeom->getFeature()->getFeatureName(),featWMasterGeom);
            if(this->featuresNameMap.values(feature->getFeature()->getFeatureName()).size() == 1){
                this->featureNames.append(feature->getFeature()->getFeatureName());
            }
            this->featuresTypeMap.insert(featWMasterGeom->getFeatureTypeEnum(),featWMasterGeom);

            if(feature->getFeature()->getGroupName().compare("") != 0){
                this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), featWMasterGeom);
            }
            if(feature->getFeature()->getGroupName().compare("") != 0 &&
                    !this->featureGroups.contains(feature->getFeature()->getGroupName())){
                this->featureGroups.append(feature->getFeature()->getGroupName());
            }

            if(newMasterGeometry->getMeasurementConfig().getIsValid()){
                QPair<QString, bool> key;
                key.first = newMasterGeometry->getMeasurementConfig().getName();
                key.second = newMasterGeometry->getMeasurementConfig().getIsSaved();
                this->geometriesMConfigMap.insert(key, newMasterGeometry);
            }
            if(!feature->getMasterGeometry().isNull() && feature->getMasterGeometry()->getMeasurementConfig().getIsValid()){
                this->usedMConfigs.append(QPair<QString,bool>(feature->getMasterGeometry()->getMeasurementConfig().getName(),
                                                              feature->getMasterGeometry()->getMeasurementConfig().getIsSaved()));
            }

        }

        break;

    /*default: //geometry
        if(!feature->getGeometry().isNull()){
            this->geometriesList.append(feature);
            if(feature->getGeometry()->getMeasurementConfig().getIsValid()){
                QPair<QString, bool> key;
                key.first = feature->getGeometry()->getMeasurementConfig().getName();
                key.second = feature->getGeometry()->getMeasurementConfig().getIsSaved();
                this->geometriesMConfigMap.insert(key, feature->getGeometry());
            }
        }
        break;*/
    }

    //update lists with ids, names, groups and mConfigs
    this->featureIds.append(feature->getFeature()->getId());
    /*if(this->featuresNameMap.values(feature->getFeature()->getFeatureName()).size() == 1){
        this->featureNames.append(feature->getFeature()->getFeatureName());
    }*/
    /*if(feature->getFeature()->getGroupName().compare("") != 0 &&
            !this->featureGroups.contains(feature->getFeature()->getGroupName())){
        this->featureGroups.append(feature->getFeature()->getGroupName());
    }*/
    /*if(!feature->getGeometry().isNull() && feature->getGeometry()->getMeasurementConfig().getIsValid()){
        this->usedMConfigs.append(QPair<QString, bool>(feature->getGeometry()->getMeasurementConfig().getName(),
                                        feature->getGeometry()->getMeasurementConfig().getIsSaved()));
    }*/
    /*if(!feature->getMasterGeometry().isNull() && feature->getMasterGeometry()->getMeasurementConfig().getIsValid()){
        this->usedMConfigs.append(QPair<QString,bool>(feature->getMasterGeometry()->getMeasurementConfig().getName(),
                                                      feature->getMasterGeometry()->getMeasurementConfig().getIsSaved()));
    }*/

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

    //remove the feature from lists and map
    this->featuresList.removeOne(feature);
    this->featuresIdMap.remove(featureId);
    this->featuresNameMap.remove(feature->getFeature()->getFeatureName(), feature);
    this->featuresTypeMap.remove(feature->getFeatureTypeEnum(), feature);
    if(feature->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.remove(feature->getFeature()->getGroupName(), feature);
    }
    switch(feature->getFeatureTypeEnum()){
    case eCoordinateSystemFeature:
        this->coordSystems.removeOne(feature->getCoordinateSystem());
        break;
    case eStationFeature:
        this->stationsList.removeOne(feature->getStation());
        break;
    case eTrafoParamFeature:
        this->trafoParamsList.removeOne(feature->getTrafoParam());
        break;
    case eMasterGeometryFeature:
        if(!feature->getMasterGeometry().isNull()){
            this->geometriesList.removeOne(feature);
            if(!feature->getMasterGeometry()->getActual().isNull()){
                this->featuresList.removeOne(feature->getMasterGeometry()->getActual()->getFeatureWrapper());
            }
            if(feature->getMasterGeometry()->getMeasurementConfig().getIsValid()){
                QPair<QString, bool> key;
                key.first = feature->getMasterGeometry()->getMeasurementConfig().getName();
                key.second = feature->getMasterGeometry()->getMeasurementConfig().getIsSaved();
                this->geometriesMConfigMap.remove(key,feature->getMasterGeometry());
            }
        }
    /*default: //geometry
        if(!feature->getGeometry().isNull()){
            this->geometriesList.removeOne(feature);
            if(feature->getGeometry()->getMeasurementConfig().getIsValid()){
                QPair<QString, bool> key;
                key.first = feature->getGeometry()->getMeasurementConfig().getName();
                key.second = feature->getGeometry()->getMeasurementConfig().getIsSaved();
                this->geometriesMConfigMap.remove(key, feature->getGeometry());
            }
        }
        break;*/
    }

    //update lists with ids, names and groups
    this->featureIds.removeOne(feature->getFeature()->getId());
    if(!this->featuresNameMap.contains(feature->getFeature()->getFeatureName())){
        this->featureNames.removeOne(feature->getFeature()->getFeatureName());
    }
    if(feature->getFeature()->getGroupName().compare("") != 0 &&
            !this->featuresGroupMap.contains(feature->getFeature()->getGroupName())){
        this->featureGroups.removeOne(feature->getFeature()->getGroupName());
    }
    /*if(!feature->getGeometry().isNull() && feature->getGeometry()->getMeasurementConfig().getIsValid()){
        this->usedMConfigs.removeOne(QPair<QString, bool>(feature->getGeometry()->getMeasurementConfig().getName(),
                                           feature->getGeometry()->getMeasurementConfig().getIsSaved()));
    }*/
    if(!feature->getMasterGeometry().isNull() && feature->getMasterGeometry()->getMeasurementConfig().getIsValid()){
        this->usedMConfigs.removeOne(QPair<QString, bool>(feature->getMasterGeometry()->getMeasurementConfig().getName(),
                                                          feature->getMasterGeometry()->getMeasurementConfig().getIsSaved()));
    }

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
    this->featuresList.removeOne(feature);

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
    }
    //clean geometry list if feature is a regular geometry
    if(!feature->getGeometry().isNull()){
        foreach (QPointer<FeatureWrapper> mastergeom, this->geometriesList) {
            if(!mastergeom->getMasterGeometry().isNull() && !mastergeom->getMasterGeometry()->getActual().isNull()
                    && mastergeom->getMasterGeometry()->getActual()->getId() == feature->getFeature()->getId()){

                mastergeom->getMasterGeometry()->removeActual(feature->getGeometry());
            }else{
                foreach (QPointer<Geometry> geom, mastergeom->getMasterGeometry()->getNominals()) {
                    if(!geom.isNull() && feature->getFeature()->getId() == geom->getId()){
                        mastergeom->getMasterGeometry()->removeNominal(feature->getGeometry());
                    }
                }
            }
        }
    }
    /*if(!feature->getGeometry().isNull()){
        this->geometriesList.removeOne(feature);
    }*/

    //clean feature maps
    this->featuresIdMap.remove(featureId);
    this->featuresNameMap.remove(name, feature);
    this->featuresGroupMap.remove(group, feature);
    this->featuresTypeMap.remove(type, feature);
    if(!feature->getMasterGeometry().isNull()){
        this->geometriesMConfigMap.remove(QPair<QString, bool>(feature->getMasterGeometry()->getMeasurementConfig().getName(),
                                          feature->getMasterGeometry()->getMeasurementConfig().getIsSaved()), feature->getMasterGeometry());
    }

    //clean list with ids, names, groups and mConfigs
    this->featureIds.removeOne(featureId);
    if(!this->featuresNameMap.contains(name)){
        this->featureNames.removeOne(name);
    }
    if(!this->featuresGroupMap.contains(group)){
        this->featureGroups.removeOne(group);
    }
    if(!feature->getGeometry().isNull()){
        QPair<QString, bool> mConfig;
        mConfig.first = feature->getMasterGeometry()->getMeasurementConfig().getName();
        mConfig.second = feature->getMasterGeometry()->getMeasurementConfig().getIsSaved();
        if(!this->geometriesMConfigMap.contains(mConfig)){
            this->usedMConfigs.removeOne(mConfig);
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

    //update lists and maps
    if(this->featuresNameMap.contains(oldName)){
        this->featuresNameMap.insert(feature->getFeature()->getFeatureName(), this->featuresNameMap.take(oldName));
    }
    if(!this->featuresNameMap.contains(oldName)){
        this->featureNames.removeOne(oldName);
    }
    if(!this->featureNames.contains(feature->getFeature()->getFeatureName())){
        this->featureNames.append(feature->getFeature()->getFeatureName());
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
            this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), feature);
            if(!this->featureGroups.contains(feature->getFeature()->getGroupName())){
                this->featureGroups.append(feature->getFeature()->getGroupName());
            }
        }

    }else{

        //check if the new group is empty
        if(feature->getFeature()->getGroupName().compare("") != 0){

            this->featuresGroupMap.remove(oldGroup, feature);
            this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), feature);

            if(!this->featuresGroupMap.contains(oldGroup)){
                this->featureGroups.removeOne(oldGroup);
            }
            if(!this->featureGroups.contains(feature->getFeature()->getGroupName())){
                this->featureGroups.append(feature->getFeature()->getGroupName());
            }

        }else{

            this->featuresGroupMap.remove(oldGroup, feature);
            if(!this->featuresGroupMap.contains(oldGroup)){
                this->featureGroups.removeOne(oldGroup);
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
