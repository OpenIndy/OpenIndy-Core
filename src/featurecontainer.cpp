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
const QList<Key> &FeatureContainer::getUsedMeasurementConfigs() const{
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
 * \param startWith use name as "start with" pattern
 * \return
 */
QList<QPointer<FeatureWrapper> > FeatureContainer::getFeaturesByName(const QString &name, const bool startWith) const{
    if(startWith) {
        QList<QPointer<FeatureWrapper> > result;
        foreach(const QPointer<FeatureWrapper> &feature, this->featuresList){
            if(!feature.isNull() && !feature->getFeature().isNull() && feature->getFeature()->getFeatureName().startsWith(name, Qt::CaseInsensitive)){
                result.append(feature);
            }
        }
        return result;
    } else {
        return this->featuresNameMap.values(name);
    }
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
QList<QPointer<Geometry> > FeatureContainer::getGeometriesByMConfig(const Key &key) const{
    return this->geometriesMConfigMap.values(key);
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
    this->featuresList.append(feature);
    this->featuresIdMap.insert(feature->getFeature()->getId(), feature);
    this->featuresNameMap.insert(feature->getFeature()->getFeatureName(), feature);
    this->featuresTypeMap.insert(feature->getFeatureTypeEnum(), feature);
    if(feature->getFeature()->getGroupName().compare("") != 0){
        this->featuresGroupMap.insert(feature->getFeature()->getGroupName(), feature);
    }
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
    default: //geometry
        if(!feature->getGeometry().isNull()){
            this->geometriesList.append(feature);
            if(feature->getGeometry()->getMeasurementConfig().isValid()){;
                this->geometriesMConfigMap.insert(feature->getGeometry()->getMeasurementConfig().getKey(),
                                                  feature->getGeometry());
            }
        }
        break;
    }

    //update lists with ids, names, groups and mConfigs
    this->featureIds.append(feature->getFeature()->getId());
    if(this->featuresNameMap.values(feature->getFeature()->getFeatureName()).size() == 1){
        this->featureNames.append(feature->getFeature()->getFeatureName());
    }
    if(feature->getFeature()->getGroupName().compare("") != 0 &&
            !this->featureGroups.contains(feature->getFeature()->getGroupName())){
        this->featureGroups.append(feature->getFeature()->getGroupName());
    }
    if(!feature->getGeometry().isNull() && feature->getGeometry()->getMeasurementConfig().isValid()){
        this->usedMConfigs.append(feature->getGeometry()->getMeasurementConfig().getKey());
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
    default: //geometry
        if(!feature->getGeometry().isNull()){
            this->geometriesList.removeOne(feature);
            if(feature->getGeometry()->getMeasurementConfig().isValid()){
                this->geometriesMConfigMap.remove(feature->getGeometry()->getMeasurementConfig().getKey(),
                                                  feature->getGeometry());
            }
        }
        break;
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
    if(!feature->getGeometry().isNull() && feature->getGeometry()->getMeasurementConfig().isValid()){
        this->usedMConfigs.removeOne(feature->getGeometry()->getMeasurementConfig().getKey());
    }

    //delete the feature
    delete feature->getFeature().data();
    delete feature.data();

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

    //clean geometry list
    if(!feature->getGeometry().isNull()){
        this->geometriesList.removeOne(feature);
    }

    //clean feature maps
    this->featuresIdMap.remove(featureId);
    this->featuresNameMap.remove(name, feature);
    this->featuresGroupMap.remove(group, feature);
    this->featuresTypeMap.remove(type, feature);
    if(!feature->getGeometry().isNull()){
        this->geometriesMConfigMap.remove(feature->getGeometry()->getMeasurementConfig().getKey(), feature->getGeometry());
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
        if(!this->geometriesMConfigMap.contains(feature->getGeometry()->getMeasurementConfig().getKey())){
            this->usedMConfigs.removeOne(feature->getGeometry()->getMeasurementConfig().getKey());
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
                delete feature->getFeature().data();
            }
            delete feature.data();
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
    QPointer<Geometry> geometry = feature->getGeometry();

    //if both, old and new mConfig, are empty nothing should happen
    if(oldMConfig.compare("") == 0 && !geometry->getMeasurementConfig().isValid()){
        return true;
    }

    //get map-keys for old and new mConfig
    Key oldKey = feature->getGeometry()->getMeasurementConfig().getKey(); // TODO OI-948
    Key newKey = feature->getGeometry()->getMeasurementConfig().getKey();

    //if the old mConfig was empty
    if(oldMConfig.compare("") == 0){
        this->geometriesMConfigMap.insert(newKey, geometry);
        if(!this->usedMConfigs.contains(newKey)){
            this->usedMConfigs.append(newKey);
        }
        return true;
    }

    //if the new mConfig is empty
    if(!geometry->getMeasurementConfig().isValid()){
        this->geometriesMConfigMap.remove(oldKey, geometry);
        if(!this->geometriesMConfigMap.contains(oldKey)){
            this->usedMConfigs.removeOne(oldKey);
        }
        return true;
    }

    //if both mConfigs are non-empty
    this->geometriesMConfigMap.remove(oldKey, geometry);
    this->geometriesMConfigMap.insert(newKey, geometry);
    if(!this->geometriesMConfigMap.contains(oldKey)){
        this->usedMConfigs.removeOne(oldKey);
    }
    if(!this->usedMConfigs.contains(newKey)){
        this->usedMConfigs.append(newKey);
    }

    return true;

}
