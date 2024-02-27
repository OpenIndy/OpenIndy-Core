#include "oijob.h"
#include "bundleadjustment.h"
using namespace oi;

/*!
 * \brief OiJob::OiJob
 * \param parent
 */
OiJob::OiJob(QObject *parent) : QObject(parent), nextId(1), activeGroup("All Groups"){

}

/*!
 * \brief OiJob::~OiJob
 */
OiJob::~OiJob(){

    //remove all features
    this->featureContainer.removeAll();

    //delete device pointer
    if(!this->jobDevice.isNull()){
        delete this->jobDevice.data();
    }

}

/*!
 * \brief OiJob::getJobName
 * \return
 */
const QString &OiJob::getJobName() const{
    return this->jobName;
}

/*!
 * \brief OiJob::setJobName
 * \param jobName
 */
void OiJob::setJobName(const QString &jobName){
    this->jobName = jobName;
}

/*!
 * \brief OiJob::getJobDevice
 * \return
 */
const QPointer<QFileDevice> &OiJob::getJobDevice() const{
    return this->jobDevice;
}

/*!
 * \brief OiJob::setJobDevice
 * \param jobDevice
 */
void OiJob::setJobDevice(const QPointer<QFileDevice> &jobDevice){
    this->jobDevice = jobDevice;
}

/*!
 * \return the last calculated digest of the job content
 */
const QString &OiJob::getDigest() const {
    return this->digest;
}

/*!
 * \brief set the digest of the job content
 */
void OiJob::setDigest(const QString &digest) {
    this->digest = digest;
}

/*!
 * \return true if the project file version compatible with this program version
 */
const CompatibilyCheckResult OiJob::checkCompatibilty() const {
    QStringList jobParts = loadedVersion.split(".");
    if(jobParts.length()<2) {
        return eCheckResult_job_wo_valid_version;
    }

    int jobMajor = jobParts.at(0).toInt();
    int jobMinor = jobParts.at(1).toInt();

    QStringList oiParts =  QString(OPENINDY_VERSION).split(".");
    if(oiParts.length()<2) {
        return eCheckResult_oi_wo_valid_version;
    }

    int oiMajor = oiParts.at(0).toInt();
    int oiMinor = oiParts.at(1).toInt();

    if(jobMajor <= 22 && jobMinor <= 1) {
        return eCheckResult_job_lt_oi_22_1;
    } else if(oiMajor == jobMajor) {
        if(oiMinor == jobMinor) {
            return eCheckResult_match;
        } else if (oiMinor > jobMinor) {
            return eCheckResult_oi_gt_job;
        } else {
            return eCheckResult_oi_lt_job;
        }
    } else if(oiMajor > jobMajor) {
        return eCheckResult_oi_gt_job;
    } else {
        return eCheckResult_oi_lt_job;
    }

}

void OiJob::setLoadedProjectVersion(const QString &loadedVersion) {
    this->loadedVersion = loadedVersion;
}

/*!
 * \brief OiJob::generateUniqueId
 * \return
 */
int OiJob::generateUniqueId(){
    this->nextId++;
    return (this->nextId - 1);
}

/*!
 * \brief OiJob::validateFeatureName
 * \param name
 * \param type
 * \param isNominal
 * \param nominalSystem
 * \return
 */
bool OiJob::validateFeatureName(const QString &name, const FeatureTypes &type, const bool &isNominal, const QPointer<CoordinateSystem> &nominalSystem){

    //do not accept empty names
    if(name.compare("") == 0){
        return false;
    }

    //get a list of features with name name
    QList<QPointer<FeatureWrapper> > features = this->featureContainer.getFeaturesByName(name);

    //accept name if no other feature with that name exists
    if(features.size() == 0){
        return true;
    }

    if(getIsGeometry(type) && isNominal == true){ //nominal geometry

        //check if nominal system is valid
        if(nominalSystem.isNull()){
            return false;
        }

        //all equal name features have to be geometries, but no nominal with the same type and nominal system
        foreach(const QPointer<FeatureWrapper> &feature, features){

            //check if feature is valid
            if(feature.isNull()){
                continue;
            }

            //check if feature is a geometry
            if(feature->getGeometry().isNull()){
                return false;
            }

            //check if feature is a nominal with same type and nominal system
            if(feature->getFeatureTypeEnum() == type && feature->getGeometry()->getIsNominal()
                    && feature->getGeometry()->getNominalSystem() == nominalSystem){
                return false;
            }

        }

        return true;

    }else if(getIsGeometry(type)){ //actual geometry

        //all equal name features have to be geometries, but no actual with the same type
        foreach(const QPointer<FeatureWrapper> &feature, features){

            //check if feature is valid
            if(feature.isNull()){
                continue;
            }

            //check if feature is a geometry
            if(feature->getGeometry().isNull()){
                return false;
            }

            //check if feature is an actual with same type
            if(feature->getFeatureTypeEnum() == type && !feature->getGeometry()->getIsNominal()){
                return false;
            }

        }

        return true;

    }else{ //non-geometry feature
        return false;
    }

}

/*!
 * \brief OiJob::getFeatureCount
 * \return
 */
int OiJob::getFeatureCount() const{
    return this->featureContainer.getFeatureCount();
}

/*!
 * \brief OiJob::getGeometryCount
 * \return
 */
int OiJob::getGeometryCount() const{
    return this->featureContainer.getGeometryCount();
}

/*!
 * \brief OiJob::getFeatureCount
 * \param type
 * \return
 */
int OiJob::getFeatureCount(const FeatureTypes &type) const{
    return this->featureContainer.getFeatureCount(type);
}

/*!
 * \brief OiJob::getFeatureCount
 * \param group
 * \return
 */
int OiJob::getFeatureCount(const QString &group) const{
    return this->featureContainer.getFeatureCount(group);
}

/*!
 * \brief OiJob::getFeatureIdList
 * \return
 */
const QList<int> &OiJob::getFeatureIdList() const{
    return this->featureContainer.getFeatureIdList();
}

/*!
 * \brief OiJob::getFeatureNameList
 * \return
 */
const QStringList &OiJob::getFeatureNameList() const{
    return this->featureContainer.getFeatureNameList();
}

/*!
 * \brief OiJob::getFeatureGroupList
 * \return
 */
const QStringList &OiJob::getFeatureGroupList() const{
    return this->featureContainer.getFeatureGroupList();
}

/*!
 * \brief OiJob::getUsedMeasurementConfigs
 * \return
 */
const QList<MeasurementConfigKey> &OiJob::getUsedMeasurementConfigs() const{
    return this->featureContainer.getUsedMeasurementConfigs();
}

/*!
 * \brief OiJob::getFeaturesList
 * \return
 */
const QList<QPointer<FeatureWrapper> > &OiJob::getFeaturesList() const{
    return this->featureContainer.getFeaturesList();
}

/*!
 * \brief OiJob::getCoordinateSystemsList
 * Returns a list of all nominal systems
 * \return
 */
const QList<QPointer<CoordinateSystem> > &OiJob::getCoordinateSystemsList() const{
    return this->featureContainer.getCoordinateSystemsList();
}

/*!
 * \brief OiJob::getStationSystems
 * \return
 */
QList<QPointer<CoordinateSystem> > OiJob::getStationSystemsList() const{
    return this->featureContainer.getStationSystemsList();
}

/*!
 * \brief OiJob::getBundleSystemList
 * \return
 */
/*
QList<QPointer<CoordinateSystem> > OiJob::getBundleSystemList() const{ // TODO OI-982 remove
    return this->featureContainer.getBundleSystemList();
}
*/

/*!
 * \brief OiJob::getStationsList
 * \return
 */
const QList<QPointer<Station> > &OiJob::getStationsList() const{
    return this->featureContainer.getStationsList();
}

/*!
 * \brief OiJob::getTransformationParametersList
 * \return
 */
const QList<QPointer<TrafoParam> > &OiJob::getTransformationParametersList() const{
    return this->featureContainer.getTransformationParametersList();
}

/*!
 * \brief OiJob::getGeometriesList
 * \return
 */
const QList<QPointer<FeatureWrapper> > &OiJob::getGeometriesList() const{
    return this->featureContainer.getGeometriesList();
}

/*!
 * \brief OiJob::getFeatureById
 * \param featureId
 * \return
 */
QPointer<FeatureWrapper> OiJob::getFeatureById(const int &featureId) const{
    return this->featureContainer.getFeatureById(featureId);
}

/*!
 * \brief OiJob::getFeaturesByName
 * \param name
 * \param startWith use name as "start with" pattern
 * \return
 */
QList<QPointer<FeatureWrapper> > OiJob::getFeaturesByName(const QString &name, const bool startWith) const{
    return this->featureContainer.getFeaturesByName(name, startWith);
}

/*!
 * \brief OiJob::getFeaturesByGroup
 * \param group
 * \return
 */
QList<QPointer<FeatureWrapper> > OiJob::getFeaturesByGroup(const QString &group) const{
    return this->featureContainer.getFeaturesByGroup(group);
}

/*!
 * \brief OiJob::getFeaturesByType
 * \param type
 * \return
 */
QList<QPointer<FeatureWrapper> > OiJob::getFeaturesByType(const FeatureTypes &type) const{
    return this->featureContainer.getFeaturesByType(type);
}

/*!
 * \brief OiJob::getGeometriesByMConfig
 * \param mConfig
 * \return
 */
QList<QPointer<Geometry> > OiJob::getGeometriesByMConfig(const MeasurementConfigKey &key) const{
    return this->featureContainer.getGeometriesByMConfig(key);
}

/*!
 * \brief OiJob::getActiveFeature
 * \return
 */
const QPointer<FeatureWrapper> &OiJob::getActiveFeature() const{
    return this->activeFeature;
}

/*!
 * \brief OiJob::getActiveStation
 * \return
 */
const QPointer<Station> &OiJob::getActiveStation() const{
    return this->activeStation;
}

const QPointer<CoordinateSystem> &OiJob::getActiveBundleSystem() const{
    return this->activeBundleSystem;
}

const QPointer<oi::BundleAdjustment> &OiJob::getActiveBundleAdjustment() const{
    if(this->activeBundleSystem.isNull()){
        return QPointer<oi::BundleAdjustment>(NULL);
    }
    return this->activeBundleSystem->getBundlePlugin();
}

const QJsonObject OiJob::getActiveBundleTemplate() const{
    if(this->activeBundleSystem.isNull()){
        return QJsonObject();
    }
    return this->activeBundleSystem->getBundleTemplate();
}

/*!
 * \brief OiJob::getActiveCoordinateSystem
 * \return
 */
const QPointer<CoordinateSystem> &OiJob::getActiveCoordinateSystem() const{
    return this->activeCoordinateSystem;
}

/*!
 * \brief OiJob::getActiveGroup
 * \return
 */
const QString &OiJob::getActiveGroup() const{
    return this->activeGroup;
}

/*!
 * \brief OiJob::setActiveGroup
 * \param group
 * \return
 */
bool OiJob::setActiveGroup(const QString &group){
    if(this->featureContainer.getFeatureGroupList().contains(group) && this->activeGroup.compare(group) != 0){
        this->activeGroup = group;
        emit this->activeGroupChanged();
        return true;
    }else if(group.compare("All Groups") == 0){
        this->activeGroup = group;
        emit this->activeGroupChanged();
        return true;
    }
    return false;
}

/*!
 * \brief OiJob::addFeature
 * \param feature
 * \return
 */
bool OiJob::addFeature(const QPointer<FeatureWrapper> &feature){

    //check if feature is valid
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //check attributes and set up relations
    if(!this->checkAndSetUpNewFeature(feature)){
        return false;
    }

    //connect the feature's signals to slots in OiJob
    this->connectFeature(feature);

    //if a group is set for the new feature emit the group changed signal
    if(feature->getFeature()->getGroupName().compare("") != 0
            && !this->featureContainer.getFeatureGroupList().contains(feature->getFeature()->getGroupName())){
        emit this->availableGroupsChanged();
    }

    //features added signals
    emit this->featureSetChanged();
    if(feature->getFeatureTypeEnum() == eCoordinateSystemFeature){
        emit this->coordSystemSetChanged();
    }else if(feature->getFeatureTypeEnum() == eStationFeature){
        emit this->stationSetChanged();
    }else if(feature->getFeatureTypeEnum() == eTrafoParamFeature){
        emit this->trafoParamSetChanged();
    }else{
        emit this->geometrySetChanged();
    }

    return true;

}

/*!
 * \brief OiJob::addFeatures
 * \param fAttr
 * \return
 */
QList<QPointer<FeatureWrapper> > OiJob::addFeatures(const FeatureAttributes &fAttr){

    QList<QPointer<FeatureWrapper> > result;

    //at least one feature should be created
    if(fAttr.count <= 0){
        return result;
    }

    //if type is a geometry then at least one of isNominal and isActual has to be true
    if(getIsGeometry(fAttr.typeOfFeature) && !fAttr.isNominal && !fAttr.isActual){
        return result;
    }

    //if isNominal is true there has to be a valid nominalSystem
    QPointer<CoordinateSystem> nominalSystem;
    if(fAttr.isNominal){
        QList<QPointer<FeatureWrapper> > features = this->featureContainer.getFeaturesByName(fAttr.nominalSystem);
        if(features.size() != 1){
            emit this->sendMessage("No valid nominal system specified", eErrorMessage);
            return result;
        }
        if(features.at(0).isNull() || features.at(0)->getCoordinateSystem().isNull() || features.at(0)->getCoordinateSystem()->getIsStationSystem()){
            emit this->sendMessage("No valid nominal system specified", eErrorMessage);
            return result;
        }
        nominalSystem = features.at(0)->getCoordinateSystem();
    }

    //if type is a TrafoParam then start and destination system have to be in the job
    QPointer<CoordinateSystem> startSystem, destSystem;
    if(fAttr.typeOfFeature == eTrafoParamFeature){
        QList<QPointer<FeatureWrapper> > startSystems = this->featureContainer.getFeaturesByName(fAttr.startSystem);
        QList<QPointer<FeatureWrapper> > destinationSystems = this->featureContainer.getFeaturesByName(fAttr.destinationSystem);
        if(startSystems.size() != 1 || destinationSystems.size() != 1
                || startSystems.at(0).isNull() || destinationSystems.at(0).isNull()){
            emit this->sendMessage("No valid start and/or destination system specified", eErrorMessage);
            return result;
        }
        if(!startSystems.at(0)->getCoordinateSystem().isNull()){
            startSystem = startSystems.at(0)->getCoordinateSystem();
        }else if(!startSystems.at(0)->getStation().isNull() && !startSystems.at(0)->getStation()->getCoordinateSystem().isNull()){
            startSystem = startSystems.at(0)->getStation()->getCoordinateSystem();
        }
        if(!destinationSystems.at(0)->getCoordinateSystem().isNull()){
            destSystem = destinationSystems.at(0)->getCoordinateSystem();
        }else if(!destinationSystems.at(0)->getStation().isNull() && !destinationSystems.at(0)->getStation()->getCoordinateSystem().isNull()){
            destSystem = destinationSystems.at(0)->getStation()->getCoordinateSystem();
        }

        if(startSystem.isNull() || destSystem.isNull()){
            emit this->sendMessage("No valid start and/or destination system specified", eErrorMessage);
            return result;
        }

    }

    //create and validate feature names
    QStringList featureNames = this->createFeatureNames(fAttr.name, fAttr.count);
    if(getIsGeometry(fAttr.typeOfFeature)){
        if(fAttr.isNominal){
            foreach(const QString &name, featureNames){
                if(!validateFeatureName(name, fAttr.typeOfFeature, true, nominalSystem)){
                    emit this->sendMessage("No valid feature name specified", eErrorMessage);
                    return result;
                }
            }
        }
        if(fAttr.isActual){
            foreach(const QString &name, featureNames){
                if(!validateFeatureName(name, fAttr.typeOfFeature)){
                    emit this->sendMessage("No valid feature name specified", eErrorMessage);
                    return result;
                }
            }
        }
    }else{
        foreach(const QString &name, featureNames){
            if(!validateFeatureName(name, fAttr.typeOfFeature)){
                emit this->sendMessage("No valid feature name specified", eErrorMessage);
                return result;
            }
        }
    }

    //check if the group is a new group
    bool isNewGroup = false;
    if(fAttr.group.compare("") != 0 && !this->featureContainer.getFeatureGroupList().contains(fAttr.group)){
        isNewGroup = true;
    }

    //create the features and add them to OpenIndy
    foreach(const QString &name, featureNames){

        //create nominal
        if(getIsGeometry(fAttr.typeOfFeature) && fAttr.isNominal){

            //create and check feature
            QPointer<FeatureWrapper> feature = this->createFeatureWrapper(fAttr.typeOfFeature, true);
            if(feature.isNull() || feature->getGeometry().isNull()){
                continue;
            }

            //pass the job to the feature
            feature->getFeature()->setJob(this);

            //set feature attributes
            feature->getFeature()->name = name;
            feature->getFeature()->group = fAttr.group;
            feature->getGeometry()->isNominal = true;

            //set nominal system
            feature->getGeometry()->nominalSystem = nominalSystem;
            nominalSystem->nominalsList.append(feature);
            nominalSystem->nominalsMap.insert(feature->getGeometry()->getId(), feature);

            //set solved state
            if(nominalSystem->getIsActiveCoordinateSystem()){
                feature->getGeometry()->setIsSolved(true);
            }

            //search corresponding actual
            QList<QPointer<FeatureWrapper> > equalNameFeatures = this->featureContainer.getFeaturesByName(name);
            foreach(const QPointer<FeatureWrapper> &equal, equalNameFeatures){
                if(!equal.isNull() && equal->getFeatureTypeEnum() == fAttr.typeOfFeature && !equal->getGeometry()->getIsNominal()){
                    feature->getGeometry()->actual = equal->getGeometry();
                    equal->getGeometry()->nominals.append(feature->getGeometry());
                }
            }

            //add and connect feature
            this->featureContainer.addFeature(feature);
            this->connectFeature(feature);

            //add feature to result list
            result.append(feature);

        }

        //create actual
        if(getIsGeometry(fAttr.typeOfFeature) && fAttr.isActual){

            //create and check feature
            QPointer<FeatureWrapper> feature = this->createFeatureWrapper(fAttr.typeOfFeature, true);
            if(feature.isNull() || feature->getGeometry().isNull()){
                continue;
            }

            //pass the job to the feature
            feature->getFeature()->setJob(this);

            //set feature attributes
            feature->getFeature()->name = name;
            feature->getFeature()->group = fAttr.group;
            feature->getGeometry()->isNominal = false;
            feature->getGeometry()->isCommon = fAttr.isCommon;

            //search corresponding nominal
            QList<QPointer<FeatureWrapper> > equalNameFeatures = this->featureContainer.getFeaturesByName(name);
            foreach(const QPointer<FeatureWrapper> &equal, equalNameFeatures){
                if(!equal.isNull() && equal->getFeatureTypeEnum() == fAttr.typeOfFeature && equal->getGeometry()->getIsNominal()){
                    equal->getGeometry()->actual = feature->getGeometry();
                    feature->getGeometry()->nominals.append(equal->getGeometry());
                }
            }

            //add and connect feature
            this->featureContainer.addFeature(feature);
            this->connectFeature(feature);

            //add feature to result list
            result.append(feature);

        }

        //create non-geometry feature
        if(!getIsGeometry(fAttr.typeOfFeature)){

            //create and check feature
            QPointer<FeatureWrapper> feature = this->createFeatureWrapper(fAttr.typeOfFeature, true);
            if(feature.isNull() || feature->getFeature().isNull()){
                continue;
            }

            //pass the job to the feature
            feature->getFeature()->setJob(this);

            //set feature attributes
            feature->getFeature()->setFeatureName(name);
            feature->getFeature()->group = fAttr.group;

            //if type is trafo param set start and destination system
            if(fAttr.typeOfFeature == eTrafoParamFeature){
                feature->getTrafoParam()->from = startSystem;
                feature->getTrafoParam()->to = destSystem;
                startSystem->trafoParams.append(feature->getTrafoParam());
                destSystem->trafoParams.append(feature->getTrafoParam());

                //check if one system is station and one system is coordinate system, then set datum to true
                if((startSystem->getIsStationSystem() || startSystem->getIsBundleSystem()) && (!destSystem->getIsBundleSystem()) && !destSystem->getIsStationSystem()){
                    feature->getTrafoParam()->setIsDatumTrafo(true);
                }else if((destSystem->getIsStationSystem() || destSystem->getIsBundleSystem()) && (!startSystem->getIsBundleSystem() && !startSystem->getIsStationSystem())){
                    feature->getTrafoParam()->setIsDatumTrafo(true);
                }else{
                    feature->getTrafoParam()->setIsDatumTrafo(false);
                }

                //set used-state to "use" as default
                feature->getTrafoParam()->setIsUsed(true);

            }

            //if type of feature is a coordinate system
            if(fAttr.typeOfFeature == eCoordinateSystemFeature){
                feature->getCoordinateSystem()->isBundleSystem = fAttr.isBundleSystem;
                if(feature->getCoordinateSystem()->isBundleSystem) {
                    this->activeBundleSystem = feature->getCoordinateSystem();
                }
            }

            //add and connect feature
            this->featureContainer.addFeature(feature);
            this->connectFeature(feature);

            //add feature to result list
            result.append(feature);

        }

    }

    //emit signals
    if(isNewGroup){
        emit this->availableGroupsChanged();
    }
    emit this->featureSetChanged();
    if(fAttr.typeOfFeature == eCoordinateSystemFeature){
        emit this->coordSystemSetChanged();
    }else if(fAttr.typeOfFeature == eStationFeature){
        emit this->stationSetChanged();
    }else if(fAttr.typeOfFeature == eTrafoParamFeature){
        emit this->trafoParamSetChanged();
    }else{
        emit this->geometrySetChanged();
    }

    return result;

}

/*!
 * \brief OiJob::addFeatures
 * Add one or more features of various types which are not related to each other
 * \param features
 * \return
 */
bool OiJob::addFeatures(const QList<QPointer<FeatureWrapper> > &features, bool overwrite){

    QList<FeatureTypes> addedFeatureTypes;

    foreach(const QPointer<FeatureWrapper> &feature, features){

        //check if feature is valid
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //check attributes and set up relations
        if(!this->checkAndSetUpNewFeature(feature, overwrite)){
            continue;
        }

        //connect the feature's signals to slots in OiJob
        this->connectFeature(feature);

        //add feature type to list of added feature types
        if(!addedFeatureTypes.contains(feature->getFeatureTypeEnum())){
            addedFeatureTypes.append(feature->getFeatureTypeEnum());
        }

    }

    //features added signals
    emit this->featureSetChanged();
    if(addedFeatureTypes.contains(eCoordinateSystemFeature)){
        emit this->coordSystemSetChanged();
        addedFeatureTypes.removeOne(eCoordinateSystemFeature);
    }else if(addedFeatureTypes.contains(eStationFeature)){
        emit this->stationSetChanged();
        addedFeatureTypes.removeOne(eStationFeature);
    }else if(addedFeatureTypes.contains(eTrafoParamFeature)){
        emit this->trafoParamSetChanged();
        addedFeatureTypes.removeOne(eTrafoParamFeature);
    }else if(addedFeatureTypes.size() > 0){
        emit this->geometrySetChanged();
    }

    return true;

}

/*!
 * \brief OiJob::removeFeature
 * \param featureId
 * \return
 */
bool OiJob::removeFeature(const int &featureId){

    //get the feature by id
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //check wether the feature could be deleted
    if(!this->canRemoveFeature(feature)){
        emit this->sendMessage(QString("Cannot remove feature %1").arg(feature->getFeature()->getFeatureName()), eWarningMessage);
        return false;
    }

    //possibly reset active group
    if(feature->getFeature()->getGroupName().compare("") != 0 && feature->getFeature()->getGroupName().compare(this->activeGroup) == 0
            && this->featureContainer.getFeaturesByGroup(feature->getFeature()->getGroupName()).size() == 1){
        this->activeGroup = "";
        emit this->activeGroupChanged();
    }

    this->disconnectFeature(feature);

    bool success = this->featureContainer.removeFeature(featureId);

    emit this->featureSetChanged();

    emit this->recalcFeatureSet();

    return success;

}

/*!
 * \brief OiJob::removeFeature
 * \param feature
 * \return
 */
bool OiJob::removeFeature(const QPointer<FeatureWrapper> &feature){

    //check feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //check wether the feature could be deleted
    if(!this->canRemoveFeature(feature)){
        emit this->sendMessage(QString("Cannot remove feature %1").arg(feature->getFeature()->getFeatureName()), eWarningMessage);
        return false;
    }

    //possibly reset active group
    if(feature->getFeature()->getGroupName().compare("") != 0 && feature->getFeature()->getGroupName().compare(this->activeGroup) == 0
            && this->featureContainer.getFeaturesByGroup(feature->getFeature()->getGroupName()).size() == 1){
        this->activeGroup = "";
        emit this->activeGroupChanged();
    }

    this->disconnectFeature(feature);

    bool success = this->featureContainer.removeFeature(feature->getFeature()->getId());

    emit this->featureSetChanged();

    emit this->recalcFeatureSet();

    return success;

}

/*!
 * \brief OiJob::removeFeatures
 * \param featureIds
 * \return
 */
bool OiJob::removeFeatures(const QSet<int> &featureIds){

    bool success = true;

    //check all features wether they could be removed
    foreach(const int &id, featureIds){

        //get the feature by id
        QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(id);
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //check wether the feature could be removed
        if(!this->canRemoveFeature(feature)){
            continue;
        }

    }

    //remove features
    this->blockSignals(true);
    foreach(const int &id, featureIds){
        if(!this->removeFeature(id)){
            success = false;
        }
    }
    this->blockSignals(false);

    emit this->activeGroupChanged();
    emit this->featureSetChanged();

    emit this->recalcFeatureSet();

    return success;

}

/*!
 * \brief OiJob::removeFeatures
 * \param features
 * \return
 */
bool OiJob::removeFeatures(const QList<QPointer<FeatureWrapper> > &features){

    bool success = true;

    //check all features wether they could be removed
    foreach(const QPointer<FeatureWrapper> &feature, features){

        //check feature
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //check wether the feature could be removed
        if(!this->canRemoveFeature(feature)){
            continue;
        }

    }

    //remove features
    this->blockSignals(true);
    foreach(const QPointer<FeatureWrapper> &feature, features){
        if(!this->removeFeature(feature)){
            success = false;
        }
    }
    this->blockSignals(false);

    emit this->activeGroupChanged();
    emit this->featureSetChanged();

    emit this->recalcFeatureSet();

    return success;

}

/*!
 * \brief OiJob::removeAll
 */
void OiJob::removeAll(){
    this->featureContainer.removeAll();
}

/*!
 * \brief OiJob::addFunction
 * \param function
 */
void OiJob::addFunction(const QPointer<Function> &function){

    //check active feature
    if(this->activeFeature.isNull() || this->activeFeature->getFeature().isNull()){
        emit this->sendMessage("No active feature", eErrorMessage);
        return;
    }

    //check function
    if(function.isNull()){
        return;
    }

    //connect the function
    QObject::connect(function.data(), &Function::sendMessage, this, &OiJob::sendMessage, Qt::AutoConnection);

    //add the function to the active feature
    this->activeFeature->getFeature()->addFunction(function);

    //set up dependencies
    QMap<int, QList<InputElement> > inputElements = function->getInputElements();
    foreach(int key, inputElements.keys()){
        foreach(const InputElement &element, inputElements[key]){
            this->setUpDependencies(element, this->activeFeature->getFeature());
        }
    }

}

/*!
 * \brief OiJob::removeFunction
 * \param functionIndex
 */
void OiJob::removeFunction(const int &functionIndex){

    //check active feature
    if(this->activeFeature.isNull() || this->activeFeature->getFeature().isNull()){
        emit this->sendMessage("No active feature", eErrorMessage);
        return;
    }

    //get and check the function at the specified index
    if(functionIndex < 0 || functionIndex >= this->activeFeature->getFeature()->getFunctions().size()){
        return;
    }
    QPointer<Function> function = this->activeFeature->getFeature()->getFunctions().at(functionIndex);
    if(function.isNull()){
        return;
    }

    //get a list of remaining function
    QList<QPointer<Function> > remainingFunctions;
    foreach(const QPointer<Function> &f, this->activeFeature->getFeature()->getFunctions()){
        if(f.isNull() || f == function){
            continue;
        }
        remainingFunctions.append(f);
    }

    //get a list of input elements and reset dependencies
    QMap<int, QList<InputElement> > inputElements = function->getInputElements();
    foreach(int key, inputElements.keys()){
        foreach(const InputElement &element, inputElements[key]){

            //check if element is contained by any other function of the feature
            bool isStillIncluded = false;
            foreach(const QPointer<Function> &f, remainingFunctions){
                if(f->hasInputElement(element.id)){
                    isStillIncluded = true;
                    break;
                }
            }

            //reset dependencies
            if(!isStillIncluded){
                this->resetDependencies(element, this->activeFeature->getFeature());
            }

        }
    }

    //remove and delete the function
    this->activeFeature->getFeature()->removeFunction(functionIndex);
    delete function.data();

}

/*!
 * \brief OiJob::addInputObservation
 * Adds an existing observation the the specified function of the target feature
 * \param target
 * \param functionPosition
 * \param neededElementsIndex
 * \param observation
 */
void OiJob::addInputObservation(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const QPointer<Observation> &observation){

    //check target feature and observation
    if(target.isNull() || target->getFeature().isNull() || observation.isNull()){
        return;
    }

    //check function position
    if(functionPosition < 0 || functionPosition >= target->getFeature()->getFunctions().size()
            || target->getFeature()->getFunctions().at(functionPosition).isNull()){
        return;
    }

    //create and add input element
    InputElement element(observation->getId());
    element.typeOfElement = eObservationElement;
    element.observation = observation;
    target->getFeature()->getFunctions().at(functionPosition)->addInputElement(element, neededElementsIndex);

    //create dependencies
    if(!target->getGeometry().isNull()){
        observation->addTargetGeometry(target->getGeometry());
    }

}

/*!
 * \brief OiJob::addInputReading
 * Adds an existing reading to the specified function of the target feature
 * \param target
 * \param functionPosition
 * \param neededElementsIndex
 * \param reading
 */
void OiJob::addInputReading(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const QPointer<Reading> &reading){

    //check target feature and reading
    if(target.isNull() || target->getFeature().isNull() || reading.isNull()){
        return;
    }

    //check function position
    if(functionPosition < 0 || functionPosition >= target->getFeature()->getFunctions().size()
            || target->getFeature()->getFunctions().at(functionPosition).isNull()){
        return;
    }

    //create and add input element
    InputElement element(reading->getId());
    element.typeOfElement = getElementTypeEnum(reading->getTypeOfReading());

    switch(reading->getTypeOfReading()){
    case eDistanceReading:
        element.distanceReading = reading;
        break;
    case eCartesianReading:
        element.cartesianReading = reading;
        break;
    case ePolarReading:
        element.polarReading = reading;
        break;
    case eDirectionReading:
        element.directionReading = reading;
        break;
    case eTemperatureReading:
        element.temperatureReading = reading;
        break;
    case eLevelReading:
        element.levelReading = reading;
        break;
    }

    target->getFeature()->getFunctions().at(functionPosition)->addInputElement(element, neededElementsIndex);

}

/*!
 * \brief OiJob::addInputFeature
 * Adds an existing feature to the specified function of the target feature
 * \param target
 * \param functionPosition
 * \param neededElementsIndex
 * \param feature
 */
void OiJob::addInputFeature(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const QPointer<FeatureWrapper> &feature){

    //check target and input features
    if(target.isNull() || target->getFeature().isNull() || feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //check function position
    if(functionPosition < 0 || functionPosition >= target->getFeature()->getFunctions().size()){
        return;
    }

    QPointer<Function> function = target->getFeature()->getFunctions().at(functionPosition);
    if(function.isNull()) {
        return;
    }

    //check circular reference
    if(this->checkCircleWarning(target->getFeature(), feature->getFeature())){
        emit this->sendMessage("Cannot add input element: circular reference error", eErrorMessage, eMessageBoxMessage);
        return;
    }


    //create and add input element
    InputElement element(feature->getFeature()->getId());
    element.typeOfElement = eUndefinedElement;
    if(neededElementsIndex == InputElementKey::eDummyPoint) {
        for(const NeededElement ne : function->getNeededElements()) {
            if(ne.key == InputElementKey::eDummyPoint) {
                element.typeOfElement = ne.typeOfElement;
                break;
            }
        }
    } else {
        element.typeOfElement = function->getNeededElements().at(neededElementsIndex).typeOfElement;
    }

    switch(element.typeOfElement){
    case eCircleElement:{

        //check circle
        if(feature->getGeometry().isNull() || feature->getCircle().isNull()){
            return;
        }

        //set up input element
        element.circle = feature->getCircle();
        element.geometry = feature->getGeometry();

        break;

    }case eConeElement:{

        //check cone
        if(feature->getGeometry().isNull() || feature->getCone().isNull()){
            return;
        }

        //set up input element
        element.cone = feature->getCone();
        element.geometry = feature->getGeometry();

        break;

    }case eCylinderElement:{

        //check cylinder
        if(feature->getGeometry().isNull() || feature->getCylinder().isNull()){
            return;
        }

        //set up input element
        element.cylinder = feature->getCylinder();
        element.geometry = feature->getGeometry();

        break;

    }case eEllipseElement:{

        //check ellipse
        if(feature->getGeometry().isNull() || feature->getEllipse().isNull()){
            return;
        }

        //set up input element
        element.ellipse = feature->getEllipse();
        element.geometry = feature->getGeometry();

        break;

    }case eEllipsoidElement:{

        //check ellipsoid
        if(feature->getGeometry().isNull() || feature->getEllipsoid().isNull()){
            return;
        }

        //set up input element
        element.ellipsoid = feature->getEllipsoid();
        element.geometry = feature->getGeometry();

        break;

    }case eHyperboloidElement:{

        //check hyperboloid
        if(feature->getGeometry().isNull() || feature->getHyperboloid().isNull()){
            return;
        }

        //set up input element
        element.hyperboloid = feature->getHyperboloid();
        element.geometry = feature->getGeometry();

        break;

    }case eLineElement:{

        //check line
        if(feature->getGeometry().isNull() || feature->getLine().isNull()){
            return;
        }

        //set up input element
        element.line = feature->getLine();
        element.geometry = feature->getGeometry();

        break;

    }case eNurbsElement:{

        //check nurbs
        if(feature->getGeometry().isNull() || feature->getNurbs().isNull()){
            return;
        }

        //set up input element
        element.nurbs = feature->getNurbs();
        element.geometry = feature->getGeometry();

        break;

    }case eParaboloidElement:{

        //check paraboloid
        if(feature->getGeometry().isNull() || feature->getParaboloid().isNull()){
            return;
        }

        //set up input element
        element.paraboloid = feature->getParaboloid();
        element.geometry = feature->getGeometry();

        break;

    }case ePlaneElement:{

        //check plane
        if(feature->getGeometry().isNull() || feature->getPlane().isNull()){
            return;
        }

        //set up input element
        element.plane = feature->getPlane();
        element.geometry = feature->getGeometry();

        break;

    }case ePointElement:{

        //check point
        if(feature->getGeometry().isNull() || feature->getPoint().isNull()){
            return;
        }

        //set up input element
        element.point = feature->getPoint();
        element.geometry = feature->getGeometry();

        break;

    }case ePointCloudElement:{

        //check point cloud
        if(feature->getGeometry().isNull() || feature->getPointCloud().isNull()){
            return;
        }

        //set up input element
        element.pointCloud = feature->getPointCloud();
        element.geometry = feature->getGeometry();

        break;

    }case eScalarEntityAngleElement:{

        //check angle
        if(feature->getGeometry().isNull() || feature->getScalarEntityAngle().isNull()){
            return;
        }

        //set up input element
        element.scalarEntityAngle = feature->getScalarEntityAngle();
        element.geometry = feature->getGeometry();

        break;

    }case eScalarEntityDistanceElement:{

        //check distance
        if(feature->getGeometry().isNull() || feature->getScalarEntityDistance().isNull()){
            return;
        }

        //set up input element
        element.scalarEntityDistance = feature->getScalarEntityDistance();
        element.geometry = feature->getGeometry();

        break;

    }case eScalarEntityMeasurementSeriesElement:{

        //check series
        if(feature->getGeometry().isNull() || feature->getScalarEntityMeasurementSeries().isNull()){
            return;
        }

        //set up input element
        element.scalarEntityMeasurementSeries = feature->getScalarEntityMeasurementSeries();
        element.geometry = feature->getGeometry();

        break;

    }case eScalarEntityTemperatureElement:{

        //check temperature
        if(feature->getGeometry().isNull() || feature->getScalarEntityTemperature().isNull()){
            return;
        }

        //set up input element
        element.scalarEntityTemperature = feature->getScalarEntityTemperature();
        element.geometry = feature->getGeometry();

        break;

    }case eSlottedHoleElement:{

        //check slotted hole
        if(feature->getGeometry().isNull() || feature->getSlottedHole().isNull()){
            return;
        }

        //set up input element
        element.slottedHole = feature->getSlottedHole();
        element.geometry = feature->getGeometry();

        break;

    }case eSphereElement:{

        //check sphere
        if(feature->getGeometry().isNull() || feature->getSphere().isNull()){
            return;
        }

        //set up input element
        element.sphere = feature->getSphere();
        element.geometry = feature->getGeometry();

        break;

    }case eTorusElement:{

        //check torus
        if(feature->getGeometry().isNull() || feature->getTorus().isNull()){
            return;
        }

        //set up input element
        element.torus = feature->getTorus();
        element.geometry = feature->getGeometry();

        break;

    }case eDirectionElement:{

        //check direction
        if(feature->getGeometry().isNull() || !feature->getGeometry()->hasDirection()){
            return;
        }

        //set up input element
        element.geometry = feature->getGeometry();

        break;

    }case ePositionElement:{

        //check position
        if(feature->getGeometry().isNull() || !feature->getGeometry()->hasPosition()){
            return;
        }

        //set up input element
        element.geometry = feature->getGeometry();

        break;

    }case eRadiusElement:{

        //check radius
        if(feature->getGeometry().isNull() || !feature->getGeometry()->hasRadius()){
            return;
        }

        //set up input element
        element.geometry = feature->getGeometry();

        break;

    }case eCoordinateSystemElement:{

        //check coordinate system
        if(feature->getCoordinateSystem().isNull()){
            return;
        }

        //set up input element
        element.coordSystem = feature->getCoordinateSystem();

        break;

    }case eStationElement:{

        //check station
        if(feature->getStation().isNull()){
            return;
        }

        //set up input element
        element.station = feature->getStation();

        break;

    }case eTrafoParamElement:{

        //check trafo param
        if(feature->getTrafoParam().isNull()){
            return;
        }

        //set up input element
        element.trafoParam = feature->getTrafoParam();

        break;

    }default:{
        return;
    }
    }

    //add the input element to the function
    function->addInputElement(element, neededElementsIndex);

    //create dependencies
    target->getFeature()->addPreviouslyNeeded(feature);

}

/*!
 * \brief OiJob::removeInputElement
 * Removes an input element from the specified function
 * \param target
 * \param functionPosition
 * \param neededElementsIndex
 * \param elementId
 */
void OiJob::removeInputElement(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const int &elementId){

    //check target
    if(target.isNull() || target->getFeature().isNull()){
        return;
    }

    //check function position
    if(functionPosition < 0 || functionPosition >= target->getFeature()->getFunctions().size()
            || target->getFeature()->getFunctions().at(functionPosition).isNull()){
        return;
    }
    Function *function = target->getFeature()->getFunctions().at(functionPosition);

    //check and get input elements
    if(!function->hasInputElement(elementId)){
        return;
    }
    InputElement inputElement = function->getInputElement(elementId);

    //remove the input element with the specified id
    function->removeInputElement(elementId, neededElementsIndex);

    //check wether any function of the feature still contains the element
    bool isStillIncluded = false;
    foreach(const QPointer<Function> &function, target->getFeature()->getFunctions()){
        if(function.isNull()){
            continue;
        }
        if(function->hasInputElement(elementId)){
            isStillIncluded = true;
            break;
        }
    }

    //reset dependencies if necessary
    if(!isStillIncluded){
        this->resetDependencies(inputElement, target->getFeature());
    }

}

/*!
 * \brief OiJob::addMeasurementResults
 * Creates and adds observations for the given readings
 * \param geomId
 * \param readings
 */
void OiJob::addMeasurementResults(const int &geomId, const QList<QPointer<Reading> > &readings){

    //check active station
    QPointer<Station> activeStation = this->activeStation;
    if(activeStation.isNull() || activeStation->getCoordinateSystem().isNull()){
        foreach(const QPointer<Reading> &reading, readings){
            if(!reading.isNull()){
                delete reading.data();
            }
        }
        return;
    }

    //get and check feature with the id geomId
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(geomId);
    if(feature.isNull() || feature->getGeometry().isNull()){
        foreach(const QPointer<Reading> &reading, readings){
            if(!reading.isNull()){
                delete reading.data();
            }
        }
        return;
    }

    //run through all readings
    feature->getGeometry()->blockSignals(true);
    foreach(const QPointer<Reading> &reading, readings){

        //check reading
        if(reading.isNull()){
            continue;
        }

        //create and set up observation
        QPointer<Observation> observation = new Observation();
        reading->setObservation(observation);
        activeStation->getCoordinateSystem()->addObservation(observation);
        observation->addTargetGeometry(feature->getGeometry());

        //add the observation to the first function of the feature (if it is a fit function)
        if(feature->getGeometry()->getFunctions().size() >= 1
                && !feature->getGeometry()->getFunctions().at(0).isNull()
                && feature->getGeometry()->getFunctions().at(0)->getNeededElements().size() > 0
                && feature->getGeometry()->getFunctions().at(0)->getNeededElements().at(0).typeOfElement == eObservationElement){
            this->addInputObservation(feature, 0, observation->getIsDummyPoint() ? InputElementKey::eDummyPoint : InputElementKey::eDefault, observation);
        }

    }
    feature->getGeometry()->blockSignals(false);

    //emit the observations changed signal only once at the end
    feature->getGeometry()->geomObservationsChanged(feature->getGeometry()->getId());

    //recalculate the feature
    emit this->recalcFeature(feature->getFeature());

}

/*!
 * \brief OiJob::removeObservations
 * \param featureId
 */
void OiJob::removeObservations(const int &featureId){

    //get and check feature with the id featureId
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull() || feature->getGeometry().isNull()){
        return;
    }

    //run through all observations of the feature
    QList<QPointer<Observation> > observations = feature->getGeometry()->getObservations();
    foreach(const QPointer<Observation> &obs, observations){
        delete obs.data();
    }

    //recalculate the feature
    emit this->recalcFeature(feature->getFeature());

    emit this->geometryObservationsChanged(feature->getFeature()->getId());

}

void OiJob::enableOrDisableObservations(const int &featureId, bool enable) {
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull()) {
        emit this->sendMessage(QString("Cannot enable or disable observations from %1").arg(feature.isNull() ? QString("feature id: %1").arg(featureId) : feature->getFeature()->getFeatureName()), eWarningMessage);
        return;
    }

    QPointer<Station> station = feature->getStation();
    if(!station.isNull()) { // is station
        enableOrDisableStationObservations(station, enable);
    } else {
        enableOrDisableGeometryObservations(featureId, enable, QPointer<Station>());
    }
}

void OiJob::enableOrDisableStationObservations(QPointer<Station> station, bool enable) {
    foreach(int featureId, this->featureContainer.getFeatureIdList()) {
        enableOrDisableGeometryObservations(featureId, enable, station);
    }
}

void OiJob::enableOrDisableGeometryObservations(const int &featureId, bool enable, QPointer<Station> station) {
    int functionIndex = 0;
    int neededElementIndex = 0;
    //get and check feature with the id featureId
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull() || feature->getGeometry().isNull()){
        emit this->sendMessage(QString("Cannot enable or disable observations from %1").arg(feature.isNull() ? QString("feature id: %1").arg(featureId) : feature->getFeature()->getFeatureName()), eWarningMessage);
        return;
    }

    //run through all observations of the feature
    QList<QPointer<Observation> > observations = feature->getGeometry()->getObservations();
    foreach(const QPointer<Observation> &obs, observations){
        if(!station.isNull() && station->getId() != obs->getStation()->getId()) {
            continue; // skip
        }
        int elementId = obs->getId();

        //check if the current use state already equals the new use state
        if(feature->getFeature()->getFunctions()[functionIndex]->getShouldBeUsed(neededElementIndex, elementId) == enable){
            continue;
        }

        //set should use
        feature->getFeature()->getFunctions()[functionIndex]->setShouldBeUsed(neededElementIndex, elementId, enable);
    }

    //recalculate the feature
    emit this->recalcFeature(feature->getFeature());
}

void OiJob::enableObservations(const int &featureId)
{
    enableOrDisableObservations(featureId, true);
}
void OiJob::disableObservations(const int &featureId)
{
    enableOrDisableObservations(featureId, false);
}
/*!
 * \brief OiJob::removeObservations
 * \param featureId
 * \param selectedIds
 */
void OiJob::removeObservations(const int &featureId, const QList<int> selectedIds)
{
    //get and check feature with the id featureId
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull() || feature->getGeometry().isNull()){
        return;
    }

    //run through all observations of the feature
    QList<QPointer<Observation> > observations = feature->getGeometry()->getObservations();
    foreach(const QPointer<Observation> &obs, observations){
        if (selectedIds.contains(obs->getId())) {
            delete obs.data();
        }
    }

    //recalculate the feature
    emit this->recalcFeature(feature->getFeature());

    emit this->geometryObservationsChanged(feature->getFeature()->getId());
}

/*!
 * \brief OiJob::removeAllObservations
 */
void OiJob::removeAllObservations(){

    //get a list of all geometries
    QList<QPointer<FeatureWrapper> > geometries = this->featureContainer.getGeometriesList();

    //remove all observations
    foreach(const QPointer<FeatureWrapper> &geom, geometries){

        //check geometry
        if(geom.isNull() || geom->getGeometry().isNull()){
            continue;
        }

        //remove all observations of the current geometry
        this->removeObservations(geom->getGeometry()->getId());

    }

}

/*!
 * \brief OiJob::setShouldBeUsed
 * \param target
 * \param functionIndex
 * \param neededElementIndex
 * \param elementId
 * \param use
 * \param recalc
 */
void OiJob::setShouldBeUsed(const QPointer<FeatureWrapper> &target, const int &functionIndex, const int &neededElementIndex, const int &elementId, const bool &use, const bool &recalc){

    //check target feature
    if(target.isNull() || target->getFeature().isNull()){
        return;
    }

    //check function position
    if(functionIndex < 0 || functionIndex >= target->getFeature()->getFunctions().size()
            || target->getFeature()->getFunctions().at(functionIndex).isNull()){
        return;
    }

    //check if the current use state already equals the new use state
    if(target->getFeature()->getFunctions()[functionIndex]->getShouldBeUsed(neededElementIndex, elementId) == use){
        return;
    }

    //set should use
    target->getFeature()->getFunctions()[functionIndex]->setShouldBeUsed(neededElementIndex, elementId, use);

    //force recalculation if requested
    if(recalc){
        emit this->recalcFeature(target->getFeature());
    }

}

/*!
 * \brief OiJob::setActiveFeature
 * \param featureId
 */
void OiJob::setActiveFeature(const int &featureId){

    //get and check the feature
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    if(feature->getFeature()->getIsActiveFeature()){ //if the feature was activated

        //check if feature already is the active feature
        if(!this->activeFeature.isNull() && this->activeFeature == feature){
            return;
        }

        //get pointer to the old active feature
        QPointer<FeatureWrapper> oldActiveFeature = this->activeFeature;

        //save feature as active feature
        this->activeFeature = feature;

        //set currently active feature to deactive
        if(!oldActiveFeature.isNull() && !oldActiveFeature->getFeature().isNull()){
            oldActiveFeature->getFeature()->setActiveFeatureState(false);
        }

        emit this->activeFeatureChanged();

    }else{ //if the feature was deactivated

        //if the feature was deactivated without specifying a new active feature
        if(!this->activeFeature.isNull() && this->activeFeature == feature){
            this->activeFeature = QPointer<FeatureWrapper>(NULL);
            emit this->activeFeatureChanged();
            return;
        }

        //if there is a new active feature
        if(!this->activeFeature.isNull() && !this->activeFeature->getFeature().isNull()){
            return;
        }

    }

}

/*!
 * \brief OiJob::setActiveStation
 * \param featureId
 */
void OiJob::setActiveStation(const int &featureId){

    //get and check the feature
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull() || feature->getStation().isNull()){
        return;
    }

    if(feature->getStation()->getIsActiveStation()){ //if the station was activated

        //check if station already is the active station
        if(!this->activeStation.isNull() && this->activeStation == feature->getStation()){
            return;
        }

        //set currently active station to deactive
        if(!this->activeStation.isNull()){
            this->activeStation->blockSignals(true);
            this->activeStation->setActiveStationState(false);
            this->activeStation->blockSignals(false);
        }

        //set sensor of the new station and reset sensor of old one
        if(!this->activeStation.isNull()){

            //take sensor from the active station
            QPointer<Sensor> sensor(NULL);
            if(this->activeStation->getIsSensorSet()){
                sensor = this->activeStation->takeSensor();
            }

            //save station as active station
            this->activeStation = feature->getStation();

            //set sensor of activated station
            if(!sensor.isNull()){
                this->activeStation->setSensor(sensor);
            }

        }else{

            //save station as active station
            this->activeStation = feature->getStation();

        }

        emit this->activeStationChanged();

    }else{ //if the station was deactivated

        //if the station was deactivated without specifying a new active station
        if(!this->activeStation.isNull() && this->activeStation == feature->getStation()){

            //reset sensor
            this->activeStation->resetSensor();

            //reset active station
            this->activeStation = QPointer<Station>(NULL);
            emit this->activeStationChanged();
            return;

        }

    }

}

/*!
 * \brief OiJob::setActiveCoordinateSystem
 * \param featureId
 */
void OiJob::setActiveCoordinateSystem(const int &featureId){

    //get and check the feature
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull() || feature->getCoordinateSystem().isNull()){
        return;
    }

    if(feature->getCoordinateSystem()->getIsActiveCoordinateSystem()){ //if the system was activated

        //check if system already is the active system
        if(!this->activeCoordinateSystem.isNull() && this->activeCoordinateSystem == feature->getCoordinateSystem()){
            return;
        }

        QPointer<CoordinateSystem> oldSystem = this->activeCoordinateSystem;

        //save system as active system
        this->activeCoordinateSystem = feature->getCoordinateSystem();

        //set currently active system to deactive
        if(!oldSystem.isNull()){
            oldSystem->setActiveCoordinateSystemState(false);
        }

        emit this->activeCoordinateSystemChanged();

    }else{ //if the system was deactivated

        //if the system was deactivated without specifying a new active system
        if(!this->activeCoordinateSystem.isNull() && this->activeCoordinateSystem == feature->getCoordinateSystem()){
            this->activeCoordinateSystem = QPointer<CoordinateSystem>(NULL);
            emit this->activeCoordinateSystemChanged();
            return;
        }

    }

}

/*!
 * \brief OiJob::setFeatureName
 * \param featureId
 * \param oldName
 */
void OiJob::setFeatureName(const int &featureId, const QString &oldName){

    //get and check the feature
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //save the ids of renamed features
    QList<int> renamedFeatures;

    //update geometry relations
    if(getIsGeometry(feature->getFeatureTypeEnum())){

        //check geometry
        if(feature->getGeometry().isNull()){
            return;
        }

        //clear old relations
        if(!feature->getGeometry().isNull()){
            if(feature->getGeometry()->getIsNominal() && !feature->getGeometry()->getActual().isNull()){

                //remove this nominal from its actual
                feature->getGeometry()->getActual()->removeNominal(feature->getGeometry());
                feature->getGeometry()->actual = QPointer<Geometry>(NULL);

            }else if(!feature->getGeometry()->getIsNominal()){

                //rename all nominals of this actual
                foreach(const QPointer<Geometry> &geometry, feature->getGeometry()->getNominals()){

                    //check geometry
                    if(geometry.isNull() || geometry->getFeatureWrapper().isNull()){
                        continue;
                    }

                    //validate the new feature name and update the name or remove the nominal from its actual
                    if(this->validateFeatureName(feature->getFeature()->getFeatureName(), geometry->getFeatureWrapper()->getFeatureTypeEnum(),
                                                 true, geometry->getNominalSystem())){
                        geometry->name = feature->getFeature()->getFeatureName();
                        renamedFeatures.append(geometry->getId());
                    }else{
                        feature->getGeometry()->removeNominal(geometry);
                        geometry->actual = QPointer<Geometry>(NULL);
                    }

                }

            }
        }

        //set up new relations
        QList<QPointer<FeatureWrapper> > equalNameFeatures = this->featureContainer.getFeaturesByName(feature->getFeature()->getFeatureName());
        foreach(const QPointer<FeatureWrapper> &other, equalNameFeatures){
            if(!other.isNull() && !other->getFeature().isNull() && feature->getFeatureTypeEnum() == other->getFeatureTypeEnum()){
                if(!feature->getGeometry()->getIsNominal()){ //if feature is an actual
                    feature->getGeometry()->addNominal(other->getGeometry());
                }else if(!other->getGeometry()->getIsNominal()){ //if feature is a nominal and other is an actual
                    other->getGeometry()->addNominal(feature->getGeometry());
                }
            }
        }

    }

    //update feature container
    this->featureContainer.featureNameChanged(featureId, oldName);
    foreach(const int &id, renamedFeatures){
        this->featureContainer.featureNameChanged(id, oldName);
    }

    emit this->featureAttributesChanged();
    emit this->featureNameChanged(featureId, oldName);

}

/*!
 * \brief OiJob::setFeatureGroup
 * \param featureId
 * \param oldGroup
 */
void OiJob::setFeatureGroup(const int &featureId, const QString &oldGroup){

    //get and check the feature
    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(featureId);
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //check if the group is a new group
    bool isNewGroup = false;
    if(!this->featureContainer.getFeatureGroupList().contains(feature->getFeature()->getGroupName())){
        isNewGroup = true;
    }

    //update feature container
    this->featureContainer.featureGroupChanged(featureId, oldGroup);

    //check if there is another feature with the group oldGroup
    bool wasLastGroup = false;
    if(!this->featureContainer.getFeatureGroupList().contains(oldGroup)){
        wasLastGroup = true;
    }

    emit this->featureAttributesChanged();
    emit this->featureGroupChanged(featureId, oldGroup);
    if(isNewGroup || wasLastGroup){
        emit this->availableGroupsChanged();
    }

}

/*!
 * \brief OiJob::setFeatureComment
 * \param featureId
 */
void OiJob::setFeatureComment(const int &featureId){
    emit this->featureAttributesChanged();
    emit this->featureCommentChanged(featureId);
}

/*!
 * \brief OiJob::setFeatureIsUpdated
 * \param featureId
 */
void OiJob::setFeatureIsUpdated(const int &featureId){
    emit this->featureAttributesChanged();
    emit this->featureIsUpdatedChanged(featureId);
}

/*!
 * \brief OiJob::setFeatureIsSolved
 * \param featureId
 */
void OiJob::setFeatureIsSolved(const int &featureId){
    emit this->featureAttributesChanged();
    emit this->featureIsSolvedChanged(featureId);
}

/*!
 * \brief OiJob::setFeatureFunctions
 * \param featureId
 */
void OiJob::setFeatureFunctions(const int &featureId){
    emit this->featureAttributesChanged();
    emit this->featureFunctionsChanged(featureId);
}

/*!
 * \brief OiJob::setFeatureUsedFor
 * \param featureId
 */
void OiJob::setFeatureUsedFor(const int &featureId){
    emit this->featureAttributesChanged();
    emit this->featureUsedForChanged(featureId);
}

/*!
 * \brief OiJob::setFeaturePreviouslyNeeded
 * \param featureId
 */
void OiJob::setFeaturePreviouslyNeeded(const int &featureId){
    emit this->featureAttributesChanged();
    emit this->featurePreviouslyNeededChanged(featureId);
}

/*!
 * \brief OiJob::setGeometryIsCommon
 * \param featureId
 */
void OiJob::setGeometryIsCommon(const int &featureId){
    emit this->geometryIsCommonChanged(featureId);
}

/*!
 * \brief OiJob::setGeometryNominals
 * \param featureId
 */
void OiJob::setGeometryNominals(const int &featureId){
    emit this->geometryNominalsChanged(featureId);
}

/*!
 * \brief OiJob::setGeometryActual
 * \param featureId
 */
void OiJob::setGeometryActual(const int &featureId){
    emit this->geometryActualChanged(featureId);
}

/*!
 * \brief OiJob::setGeometryObservations
 * \param featureId
 */
void OiJob::setGeometryObservations(const int &featureId){
    emit this->geometryObservationsChanged(featureId);
}

/*!
 * \brief OiJob::setGeometryNominalSystem
 * \param featureId
 */
void OiJob::setGeometryNominalSystem(const int &featureId){
    emit this->geometryNominalSystemChanged(featureId);
}

/*!
 * \brief OiJob::setGeometryStatistic
 * \param featureId
 */
void OiJob::setGeometryStatistic(const int &featureId){
    emit this->geometryStatisticChanged(featureId);
}

/*!
 * \brief OiJob::setGeometrySimulationData
 * \param featureId
 */
void OiJob::setGeometrySimulationData(const int &featureId){
    emit this->geometrySimulationDataChanged(featureId);
}

/*!
 * \brief OiJob::setGeometryMeasurementConfig
 * \param featureId
 * \param oldMConfig
 * \param oldIsSaved
 */
void OiJob::setGeometryMeasurementConfig(const int &featureId, const QString &oldMConfig, const MeasurementConfigKey oldKey){

    //update feature container
    this->featureContainer.geometryMeasurementConfigChanged(featureId, oldMConfig, oldKey);

    emit this->geometryMeasurementConfigChanged(featureId, oldMConfig, oldKey);

}

/*!
 * \brief OiJob::setSystemObservations
 * \param featureId
 * \param obsId
 */
void OiJob::setSystemObservations(const int &featureId, const int &obsId){
    emit this->systemObservationsChanged(featureId, obsId);
}

/*!
 * \brief OiJob::setSystemTrafoParams
 * \param featureId
 */
void OiJob::setSystemTrafoParams(const int &featureId){
    emit this->systemTrafoParamsChanged(featureId);
}

/*!
 * \brief OiJob::setSystemsNominals
 * \param featureId
 */
void OiJob::setSystemsNominals(const int &featureId){
    emit this->systemsNominalsChanged(featureId);
}

/*!
 * \brief OiJob::setStationSensor
 * \param featureId
 */
void OiJob::setStationSensor(const int &featureId){
    emit this->stationSensorChanged(featureId);
}

/*!
 * \brief OiJob::setTrafoParamParameters
 * \param featureId
 */
void OiJob::setTrafoParamParameters(const int &featureId){
    emit this->trafoParamParametersChanged(featureId);
}

/*!
 * \brief OiJob::setTrafoParamSystems
 * \param featureId
 */
void OiJob::setTrafoParamSystems(const int &featureId){
    emit this->trafoParamSystemsChanged(featureId);
}

/*!
 * \brief OiJob::setTrafoParamIsUsed
 * \param featureId
 */
void OiJob::setTrafoParamIsUsed(const int &featureId){
    emit this->trafoParamIsUsedChanged(featureId);
}

/*!
 * \brief OiJob::setTrafoParamIsDatum
 * \param featureId
 */
void OiJob::setTrafoParamIsDatum(const int &featureId)
{
    emit this->trafoParamIsDatumChanged(featureId);
}

/*!
 * \brief OiJob::setTrafoParamValidTime
 * \param featureId
 */
/*void OiJob::setTrafoParamValidTime(const int &featureId){
    emit this->trafoParamValidTimeChanged(featureId);
}*/

/*!
 * \brief OiJob::setTrafoParamIsMovement
 * \param featureId
 */
/*void OiJob::setTrafoParamIsMovement(const int &featureId){
    emit this->trafoParamIsMovementChanged(featureId);
}*/

/*!
 * \brief OiJob::elementAboutToBeDeleted
 * \param elementId
 * \param name
 * \param group
 * \param type
 */
void OiJob::elementAboutToBeDeleted(const int &elementId, const QString &name, const QString &group, const FeatureTypes &type){

    QPointer<FeatureWrapper> feature = this->featureContainer.getFeatureById(elementId);
    this->disconnectFeature(feature);
    this->featureContainer.checkAndClean(elementId, name, group, type);
}

/*!
 * \brief OiJob::connectFeature
 * \param feature
 */
void OiJob::connectFeature(const QPointer<FeatureWrapper> &feature){

    //check feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //general element connects
    QObject::connect(feature->getFeature().data(), &Element::elementAboutToBeDeleted,
                     this, &OiJob::elementAboutToBeDeleted, Qt::AutoConnection);

    //general feature connects
    QObject::connect(feature->getFeature().data(), &Feature::featureNameChanged,
                     this, &OiJob::setFeatureName, Qt::AutoConnection);
    QObject::connect(feature->getFeature().data(), &Feature::featureGroupChanged,
                     this, &OiJob::setFeatureGroup, Qt::AutoConnection);
    QObject::connect(feature->getFeature().data(), &Feature::featureCommentChanged,
                     this, &OiJob::setFeatureComment, Qt::AutoConnection);
    //QObject::connect(feature->getFeature().data(), &Feature::featureIsUpdatedChanged,
    //                 this, &OiJob::setFeatureIsUpdated, Qt::AutoConnection);
    QObject::connect(feature->getFeature().data(), &Feature::featureIsSolvedChanged,
                     this, &OiJob::setFeatureIsSolved, Qt::QueuedConnection);
    QObject::connect(feature->getFeature().data(), &Feature::featureFunctionListChanged,
                     this, &OiJob::setFeatureFunctions, Qt::AutoConnection);
    QObject::connect(feature->getFeature().data(), &Feature::featureUsedForChanged,
                     this, &OiJob::setFeatureUsedFor, Qt::AutoConnection);
    QObject::connect(feature->getFeature().data(), &Feature::featurePreviouslyNeededChanged,
                     this, &OiJob::setFeaturePreviouslyNeeded, Qt::AutoConnection);
    QObject::connect(feature->getFeature().data(), &Feature::featureIsActiveChanged,
                     this, &OiJob::setActiveFeature, Qt::AutoConnection);

    //general geometry connects
    if(getIsGeometry(feature->getFeatureTypeEnum())){
        QObject::connect(feature->getGeometry().data(), &Geometry::geomIsCommonChanged,
                         this, &OiJob::setGeometryIsCommon, Qt::AutoConnection);
        QObject::connect(feature->getGeometry().data(), &Geometry::geomNominalsChanged,
                         this, &OiJob::setGeometryNominals, Qt::AutoConnection);
        QObject::connect(feature->getGeometry().data(), &Geometry::geomActualChanged,
                         this, &OiJob::setGeometryActual, Qt::AutoConnection);
        QObject::connect(feature->getGeometry().data(), &Geometry::geomObservationsChanged,
                         this, &OiJob::setGeometryObservations, Qt::AutoConnection);
        QObject::connect(feature->getGeometry().data(), &Geometry::geomNominalSystemChanged,
                         this, &OiJob::setGeometryNominalSystem, Qt::AutoConnection);
        QObject::connect(feature->getGeometry().data(), &Geometry::geomStatisticChanged,
                         this, &OiJob::setGeometryStatistic, Qt::AutoConnection);
        QObject::connect(feature->getGeometry().data(), &Geometry::geomSimulationDataChanged,
                         this, &OiJob::setGeometrySimulationData, Qt::AutoConnection);
        QObject::connect(feature->getGeometry().data(), &Geometry::geomMeasurementConfigChanged,
                         this, &OiJob::setGeometryMeasurementConfig, Qt::AutoConnection);
    }

    //trafo param connects
    if(feature->getFeatureTypeEnum() == eTrafoParamFeature){
        QObject::connect(feature->getTrafoParam().data(), &TrafoParam::transformationParameterChanged,
                         this, &OiJob::setTrafoParamParameters, Qt::AutoConnection);
        QObject::connect(feature->getTrafoParam().data(), &TrafoParam::coordinateSystemsChanged,
                         this, &OiJob::setTrafoParamSystems, Qt::AutoConnection);
        QObject::connect(feature->getTrafoParam().data(), &TrafoParam::isUsedChanged,
                         this, &OiJob::setTrafoParamIsUsed, Qt::AutoConnection);
        /*QObject::connect(feature->getTrafoParam().data(), &TrafoParam::validTimeChanged,
                         this, &OiJob::setTrafoParamValidTime, Qt::AutoConnection);*/
        /*QObject::connect(feature->getTrafoParam().data(), &TrafoParam::isMovementChanged,
                         this, &OiJob::setTrafoParamIsMovement, Qt::AutoConnection);*/
        QObject::connect(feature->getTrafoParam().data(), &TrafoParam::isDatumTrafoChanged,
                         this, &OiJob::setTrafoParamIsDatum, Qt::AutoConnection);
    }

    //station connects
    if(feature->getFeatureTypeEnum() == eStationFeature){
        QObject::connect(feature->getStation().data(), &Station::activeStationChanged,
                         this, &OiJob::setActiveStation, Qt::AutoConnection);
        QObject::connect(feature->getStation().data(), &Station::sensorChanged,
                         this, &OiJob::setStationSensor, Qt::AutoConnection);
        if(!feature->getStation()->getCoordinateSystem().isNull()){
            QObject::connect(feature->getStation()->getCoordinateSystem().data(), &CoordinateSystem::observationsChanged,
                             this, &OiJob::setSystemObservations, Qt::AutoConnection);
            QObject::connect(feature->getStation()->getCoordinateSystem().data(), &CoordinateSystem::transformationParametersChanged,
                             this, &OiJob::setSystemTrafoParams, Qt::AutoConnection);
            QObject::connect(feature->getStation()->getCoordinateSystem().data(), &CoordinateSystem::activeCoordinateSystemChanged,
                             this, &OiJob::setActiveCoordinateSystem, Qt::AutoConnection);
        }
    }

    //coordinate system connects
    if(feature->getFeatureTypeEnum() == eCoordinateSystemFeature){
        QObject::connect(feature->getCoordinateSystem().data(), &CoordinateSystem::nominalsChanged,
                         this, &OiJob::setSystemsNominals, Qt::AutoConnection);
        QObject::connect(feature->getCoordinateSystem().data(), &CoordinateSystem::transformationParametersChanged,
                         this, &OiJob::setSystemTrafoParams, Qt::AutoConnection);
        QObject::connect(feature->getCoordinateSystem().data(), &CoordinateSystem::activeCoordinateSystemChanged,
                         this, &OiJob::setActiveCoordinateSystem, Qt::AutoConnection);
    }

}

/*!
 * \brief OiJob::disconnectFeature
 * \param feature
 */
void OiJob::disconnectFeature(const QPointer<FeatureWrapper> &feature){

    //check feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //general element connects
    QObject::disconnect(feature->getFeature().data(), &Element::elementAboutToBeDeleted,
                     this, &OiJob::elementAboutToBeDeleted);

    //general feature connects
    QObject::disconnect(feature->getFeature().data(), &Feature::featureNameChanged,
                     this, &OiJob::setFeatureName);
    QObject::disconnect(feature->getFeature().data(), &Feature::featureGroupChanged,
                     this, &OiJob::setFeatureGroup);
    QObject::disconnect(feature->getFeature().data(), &Feature::featureCommentChanged,
                     this, &OiJob::setFeatureComment);
    //QObject::disconnect(feature->getFeature().data(), &Feature::featureIsUpdatedChanged,
    //                 this, &OiJob::setFeatureIsUpdated);
    QObject::disconnect(feature->getFeature().data(), &Feature::featureIsSolvedChanged,
                     this, &OiJob::setFeatureIsSolved);
    QObject::disconnect(feature->getFeature().data(), &Feature::featureFunctionListChanged,
                     this, &OiJob::setFeatureFunctions);
    QObject::disconnect(feature->getFeature().data(), &Feature::featureUsedForChanged,
                     this, &OiJob::setFeatureUsedFor);
    QObject::disconnect(feature->getFeature().data(), &Feature::featurePreviouslyNeededChanged,
                     this, &OiJob::setFeaturePreviouslyNeeded);
    QObject::disconnect(feature->getFeature().data(), &Feature::featureIsActiveChanged,
                     this, &OiJob::setActiveFeature);

    //general geometry connects
    if(getIsGeometry(feature->getFeatureTypeEnum())){
        QObject::connect(feature->getGeometry().data(), &Geometry::geomIsCommonChanged,
                         this, &OiJob::setGeometryIsCommon);
        QObject::disconnect(feature->getGeometry().data(), &Geometry::geomNominalsChanged,
                         this, &OiJob::setGeometryNominals);
        QObject::disconnect(feature->getGeometry().data(), &Geometry::geomActualChanged,
                         this, &OiJob::setGeometryActual);
        QObject::disconnect(feature->getGeometry().data(), &Geometry::geomObservationsChanged,
                         this, &OiJob::setGeometryObservations);
        QObject::disconnect(feature->getGeometry().data(), &Geometry::geomNominalSystemChanged,
                         this, &OiJob::setGeometryNominalSystem);
        QObject::disconnect(feature->getGeometry().data(), &Geometry::geomStatisticChanged,
                         this, &OiJob::setGeometryStatistic);
        QObject::disconnect(feature->getGeometry().data(), &Geometry::geomSimulationDataChanged,
                         this, &OiJob::setGeometrySimulationData);
        QObject::disconnect(feature->getGeometry().data(), &Geometry::geomMeasurementConfigChanged,
                         this, &OiJob::setGeometryMeasurementConfig);
    }

    //trafo param connects
    if(feature->getFeatureTypeEnum() == eTrafoParamFeature){
        QObject::disconnect(feature->getTrafoParam().data(), &TrafoParam::transformationParameterChanged,
                         this, &OiJob::setTrafoParamParameters);
        QObject::disconnect(feature->getTrafoParam().data(), &TrafoParam::coordinateSystemsChanged,
                         this, &OiJob::setTrafoParamSystems);
        QObject::disconnect(feature->getTrafoParam().data(), &TrafoParam::isUsedChanged,
                         this, &OiJob::setTrafoParamIsUsed);
        /*QObject::disconnect(feature->getTrafoParam().data(), &TrafoParam::validTimeChanged,
                         this, &OiJob::setTrafoParamValidTime);*/
        /*QObject::disconnect(feature->getTrafoParam().data(), &TrafoParam::isMovementChanged,
                         this, &OiJob::setTrafoParamIsMovement);*/
        QObject::disconnect(feature->getTrafoParam().data(), &TrafoParam::isDatumTrafoChanged,
                         this, &OiJob::setTrafoParamIsDatum);
    }

    //station connects
    if(feature->getFeatureTypeEnum() == eStationFeature){
        QObject::disconnect(feature->getStation().data(), &Station::activeStationChanged,
                         this, &OiJob::setActiveStation);
        QObject::disconnect(feature->getStation().data(), &Station::sensorChanged,
                         this, &OiJob::setStationSensor);
        if(!feature->getStation()->getCoordinateSystem().isNull()){
            QObject::disconnect(feature->getStation()->getCoordinateSystem().data(), &CoordinateSystem::observationsChanged,
                             this, &OiJob::setSystemObservations);
            QObject::disconnect(feature->getStation()->getCoordinateSystem().data(), &CoordinateSystem::transformationParametersChanged,
                             this, &OiJob::setSystemTrafoParams);
            QObject::disconnect(feature->getStation()->getCoordinateSystem().data(), &CoordinateSystem::activeCoordinateSystemChanged,
                             this, &OiJob::setActiveCoordinateSystem);
        }
    }

    //coordinate system connects
    if(feature->getFeatureTypeEnum() == eCoordinateSystemFeature){
        QObject::disconnect(feature->getCoordinateSystem().data(), &CoordinateSystem::nominalsChanged,
                         this, &OiJob::setSystemsNominals);
        QObject::disconnect(feature->getCoordinateSystem().data(), &CoordinateSystem::transformationParametersChanged,
                         this, &OiJob::setSystemTrafoParams);
        QObject::disconnect(feature->getCoordinateSystem().data(), &CoordinateSystem::activeCoordinateSystemChanged,
                         this, &OiJob::setActiveCoordinateSystem);
    }

}

/*!
 * \brief OiJob::createFeatureNames
 * \param name
 * \return
 */
QStringList OiJob::createFeatureNames(const QString &name, const int &count) const{

    QStringList result;

    //if only one feature shall be created take the hole name
    if(count == 1){
        result.append(name);
        return result;
    }

    //split base name from postfix
    QString baseName = name;
    QString postFix;

    int index = baseName.lastIndexOf(QRegExp("[0-9]*$"), baseName.length()-1);
    while(index > -1){
        postFix.prepend(QStringRef(&baseName, index, baseName.length() - index).toString());
        baseName.resize(index);
        index = baseName.lastIndexOf(QRegExp("[0-9]*$"), baseName.length()-1);
    }

    //add leading zero
    int numLeadingZeros = 0;
    while(postFix.size() > 1 && postFix.at(0) == '0'){
        numLeadingZeros++;
        postFix.remove(0, 1);
    }

    numLeadingZeros = numLeadingZeros + postFix.length();

    int postFixInt = postFix.toInt();
    if(postFixInt == 0){
        postFixInt++;
    }
    QString leadZeros = "";

    for(int i = 0; i < count; i++){
        leadZeros.clear();

        postFix = QString::number(postFixInt+i);

        int diff = numLeadingZeros - postFix.length();

        if(diff > 0){
            for(int j = 0; j < diff; j++){
                leadZeros.append("0");
            }
        }

        if(leadZeros.compare("") == 0 && postFix.toInt() < 10){
            leadZeros.append("0");
        }

        result.append(QString("%1%2%3").arg(baseName).arg(leadZeros).arg(postFix));
    }

    return result;

}

/*!
 * \brief OiJob::createFeatureWrapper
 * Create a feature wrapper containing a feature of the given type
 * \param type
 * \param isNominal
 * \return
 */
QPointer<FeatureWrapper> OiJob::createFeatureWrapper(const FeatureTypes &type, bool isNominal) const{

    QPointer<FeatureWrapper> feature = new FeatureWrapper();

    switch(type){
    case eStationFeature:{
        Station *station = new Station();
        feature->setStation(station);
        break;
    }case eTrafoParamFeature:{
        TrafoParam *trafoParam = new TrafoParam();
        feature->setTrafoParam(trafoParam);
        break;
    }case eCoordinateSystemFeature:{
        CoordinateSystem *coordSystem = new CoordinateSystem();
        feature->setCoordinateSystem(coordSystem);
        break;
    }case eCircleFeature:{
        Circle *circle = new Circle(isNominal);
        feature->setCircle(circle);
        break;
    }case eConeFeature:{
        Cone *cone = new Cone(isNominal);
        feature->setCone(cone);
        break;
    }case eCylinderFeature:{
        Cylinder *cylinder = new Cylinder(isNominal);
        feature->setCylinder(cylinder);
        break;
    }case eEllipseFeature:{
        Ellipse *ellipse = new Ellipse(isNominal);
        feature->setEllipse(ellipse);
        break;
    }case eEllipsoidFeature:{
        Ellipsoid *ellipsoid = new Ellipsoid(isNominal);
        feature->setEllipsoid(ellipsoid);
        break;
    }case eHyperboloidFeature:{
        Hyperboloid *hyperboloid = new Hyperboloid(isNominal);
        feature->setHyperboloid(hyperboloid);
        break;
    }case eLineFeature:{
        Line *line = new Line(isNominal);
        feature->setLine(line);
        break;
    }case eNurbsFeature:{
        Nurbs *nurbs = new Nurbs(isNominal);
        feature->setNurbs(nurbs);
        break;
    }case eParaboloidFeature:{
        Paraboloid *paraboloid = new Paraboloid(isNominal);
        feature->setParaboloid(paraboloid);
        break;
    }case ePlaneFeature:{
        Plane *plane = new Plane(isNominal);
        feature->setPlane(plane);
        break;
    }case ePointFeature:{
        Point *point = new Point(isNominal);
        feature->setPoint(point);
        break;
    }case ePointCloudFeature:{
        PointCloud *pointCloud = new PointCloud(isNominal);
        feature->setPointCloud(pointCloud);
        break;
    }case eScalarEntityAngleFeature:{
        ScalarEntityAngle *angle = new ScalarEntityAngle(isNominal);
        feature->setScalarEntityAngle(angle);
        break;
    }case eScalarEntityDistanceFeature:{
        ScalarEntityDistance *distance = new ScalarEntityDistance(isNominal);
        feature->setScalarEntityDistance(distance);
        break;
    }case eScalarEntityMeasurementSeriesFeature:{
        ScalarEntityMeasurementSeries *measurementSeries = new ScalarEntityMeasurementSeries(isNominal);
        feature->setScalarEntityMeasurementSeries(measurementSeries);
        break;
    }case eScalarEntityTemperatureFeature:{
        ScalarEntityTemperature *temperature = new ScalarEntityTemperature(isNominal);
        feature->setScalarEntityTemperature(temperature);
        break;
    }case eSlottedHoleFeature:{
        SlottedHole *slottedHole = new SlottedHole(isNominal);
        feature->setSlottedHole(slottedHole);
        break;
    }case eSphereFeature:{
        Sphere *sphere = new Sphere(isNominal);
        feature->setSphere(sphere);
        break;
    }case eTorusFeature:{
        Torus *torus = new Torus(isNominal);
        feature->setTorus(torus);
        break;
    }default:{
        delete feature.data();
    }
    }

    return feature;

}

/*!
 * \brief OiJob::checkAndSetUpNewFeature
 * Check the attributes of the given feature and set up relations
 * \param feature
 * \return
 */
bool OiJob::checkAndSetUpNewFeature(const QPointer<FeatureWrapper> &feature, bool overwrite){

    //pass the job to the feature
    feature->getFeature()->setJob(this);

    //check feature name
    bool isNominal = (!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal());
    QPointer<CoordinateSystem> nominalSystem = isNominal?feature->getGeometry()->getNominalSystem():QPointer<CoordinateSystem>(NULL);
    if(!this->validateFeatureName(feature->getFeature()->getFeatureName(), feature->getFeatureTypeEnum(),
                                 isNominal, nominalSystem)){

        //check if feature should be overwritten
        if(overwrite && feature->getFeatureTypeEnum() == ePointFeature){

            //get existing feature and set new values
            //get a list of features with name name
            QList<QPointer<FeatureWrapper> > features = this->featureContainer.getFeaturesByName(feature->getFeature()->getFeatureName());

            foreach (QPointer<FeatureWrapper> fw, features) {
                if(fw->getGeometry()->getIsNominal() && fw->getGeometry()->getNominalSystem() == feature->getGeometry()->getNominalSystem()){
                    //overwrite existing position with new position
                    fw->getPoint()->setPoint(feature->getPoint()->getPosition());
                    emit this->sendMessage("changed values of existing features", eWarningMessage);
                    return false;
                }
            }
        }else{
            emit this->sendMessage("feature name already exists", eWarningMessage);
            return false;
        }
    }

    //new feature shall not be active
    feature->getFeature()->setActiveFeatureState(false);

    //check feature relations    
    if(feature->getFeatureTypeEnum() == eCoordinateSystemFeature){
        if(!feature->getCoordinateSystem().isNull()
                && feature->getCoordinateSystem()->getIsBundleSystem()){
            this->activeBundleSystem = feature->getCoordinateSystem();
        }

        //check if the system is a nominal system
        if(feature->getCoordinateSystem()->getIsStationSystem()){
            return false;
        }

        //new coordinate system shall not be active
        feature->getCoordinateSystem()->setActiveCoordinateSystemState(false);

        //a nominal system cannot have observations
        if(feature->getCoordinateSystem()->getObservations().size() > 0){
            return false;
        }

        //a new system cannot have trafo params
        if(feature->getCoordinateSystem()->getTransformationParameters().size() > 0){
            return false;
        }

        //a new system cannot have nominals
        if(feature->getCoordinateSystem()->getNominals().size() > 0){
            return true;
        }

    }else if(feature->getFeatureTypeEnum() == eStationFeature){

        //new station shall not be active
        feature->getStation()->setActiveStationState(false);

    }else if(feature->getFeatureTypeEnum() == eTrafoParamFeature){

        //start and destination system have to exist in this job
        if(feature->getTrafoParam()->getStartSystem().isNull()
                || feature->getTrafoParam()->getDestinationSystem().isNull()){
            return false;
        }
        QPointer<FeatureWrapper> jobFrom = this->getFeatureById(feature->getTrafoParam()->getStartSystem()->getId());
        QPointer<FeatureWrapper> jobTo = this->getFeatureById(feature->getTrafoParam()->getDestinationSystem()->getId());
        if(jobFrom.isNull() || jobTo.isNull() || jobFrom->getCoordinateSystem().isNull()
                || jobTo->getCoordinateSystem().isNull() ||
                jobFrom->getCoordinateSystem() != feature->getTrafoParam()->getStartSystem() ||
                jobTo->getCoordinateSystem() != feature->getTrafoParam()->getDestinationSystem()){
            return false;
        }

    }else if(getIsGeometry(feature->getFeatureTypeEnum())){

        if(feature->getGeometry()->getIsNominal()){ // check nominal

            //a nominal geometry cannot have nominals
            if(feature->getGeometry()->getNominals().size() > 0){
                return false;
            }

            //a nominal cannot have observations
            if(feature->getGeometry()->getObservations().size() > 0){
                return false;
            }

            //check if actual is in the same job / add actual to nominal
            if(feature->getGeometry()->getActual().isNull()){
                QList<QPointer<FeatureWrapper> > equalNameFeatures = this->getFeaturesByName(feature->getFeature()->getFeatureName());
                foreach(const QPointer<FeatureWrapper> &equal, equalNameFeatures){
                    if(!equal.isNull() && equal->getFeatureTypeEnum() == feature->getFeatureTypeEnum() && !equal->getGeometry()->getIsNominal()){
                        feature->getGeometry()->actual = equal->getGeometry();
                        equal->getGeometry()->nominals.append(feature->getGeometry());
                    }
                }
            }else{
                QPointer<FeatureWrapper> jobActual = this->getFeatureById(feature->getGeometry()->getActual()->getId());
                if(jobActual.isNull() || jobActual->getGeometry().isNull() || jobActual->getGeometry() != feature->getGeometry()->getActual()){
                    return false;
                }
            }

            //check if nominal system is in the same job
            if(feature->getGeometry()->getNominalSystem().isNull()
                    || !this->getCoordinateSystemsList().contains(feature->getGeometry()->getNominalSystem())){
                return false;
            }

        }else{ //check actual

            //check and add nominals
            if(feature->getGeometry()->getNominals().size() > 0){
                return false;
            }
            QList<QPointer<FeatureWrapper> > equalNameFeatures = this->getFeaturesByName(feature->getFeature()->getFeatureName());
            foreach(const QPointer<FeatureWrapper> equal, equalNameFeatures){
                if(!equal.isNull() && equal->getFeatureTypeEnum() == feature->getFeatureTypeEnum() && equal->getGeometry()->getIsNominal()){
                    feature->getGeometry()->nominals.append(equal->getGeometry());
                    equal->getGeometry()->actual = feature->getGeometry();
                }
            }

            //a new actual shall not have observations
            if(feature->getGeometry()->getObservations().size() > 0){
                return false;
            }

            //an actual cannot have a nominal system
            if(!feature->getGeometry()->getNominalSystem().isNull()){
                return false;
            }

        }

    }

    //check if the group is a new group
    bool isNewGroup = false;
    if(feature->getFeature()->getGroupName().compare("") != 0
            && !this->featureContainer.getFeatureGroupList().contains(feature->getFeature()->getGroupName())){
        isNewGroup = true;
    }

    //add the feature to the internal lists and maps
    this->featureContainer.addFeature(feature);

    //add nominal to nominal list of coordinate system
    if(isNominal && !nominalSystem.isNull()){
        nominalSystem->addNominal(feature);
    }

    //add nominal to nominal list of actual
    if(isNominal && !feature->getGeometry()->getActual().isNull()){
        feature->getGeometry()->getActual()->addNominal(feature->getGeometry());
    }

    //add actual to nominals
    if(!isNominal && getIsGeometry(feature->getFeatureTypeEnum())){
        foreach(const QPointer<Geometry> &nominal, feature->getGeometry()->getNominals()){
            if(!nominal.isNull()){
                nominal->setActual(feature->getGeometry());
            }
        }
    }

    //if a group is set for the new feature emit the group changed signal
    if(isNewGroup){
       emit this->availableGroupsChanged();
    }

    return true;

}

/*!
 * \brief OiJob::canRemoveFeature
 * Check wether a feature could be deleted or not
 * \param feature
 * \return
 */
bool OiJob::canRemoveFeature(const QPointer<FeatureWrapper> &feature) const{

    //do not remove active station or active coordinate system
    if(!feature->getStation().isNull() && feature->getStation()->getIsActiveStation()){
        emit this->sendMessage("Cannot delete the active station", eWarningMessage);
        return false;
    }
    if( (!feature->getCoordinateSystem().isNull() && feature->getCoordinateSystem()->getIsActiveCoordinateSystem())
            || (!feature->getStation().isNull() && !feature->getStation()->getCoordinateSystem().isNull()
                && feature->getStation()->getCoordinateSystem()->getIsActiveCoordinateSystem()) ){
        emit this->sendMessage("Cannot delete the active coordinate system", eWarningMessage);
        return false;
    }

    //do not delete a station system (without deleting the station)
    if(!feature->getCoordinateSystem().isNull() && feature->getCoordinateSystem()->getIsStationSystem()){
        emit this->sendMessage("Cannot delete a station system without the corresponding station", eWarningMessage);
        return false;
    }

    //do not delete coordinate systems with observations
    if(!feature->getStation().isNull() && !feature->getStation()->getCoordinateSystem().isNull()
            && feature->getStation()->getCoordinateSystem()->getObservations().size() > 0){
        emit this->sendMessage("Cannot delete a station which contains one or more observations", eWarningMessage);
        return false;
    }

    //do not delete coordinate systems with nominals
    if(!feature->getCoordinateSystem().isNull() && feature->getCoordinateSystem()->getNominals().size() > 0){
        emit this->sendMessage("Cannot delete a coordinate system which contains one or more nominals", eWarningMessage);
        return false;
    }

    return true;

}

/*!
 * \brief OiJob::checkCircleWarning
 * \param activeFeature
 * \param usedForActiveFeature
 * \return
 */
bool OiJob::checkCircleWarning(const QPointer<Feature> &activeFeature, const QPointer<Feature> &usedForActiveFeature){

    //check features
    if(activeFeature.isNull() || usedForActiveFeature.isNull() || activeFeature->getId() == usedForActiveFeature->getId()){
        return true;
    }

    //check if active feature is in the list of previously needed features of usedForActiveFeature
    foreach(const QPointer<FeatureWrapper> &feature, usedForActiveFeature->getPreviouslyNeeded()){

        //check feature
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        if(feature->getFeature()->getId() == activeFeature->getId()){
            return true;
        }else if(feature->getFeature()->getPreviouslyNeeded().size() > 0
                 && this->checkCircleWarning(activeFeature, feature->getFeature())){
            return true;
        }

    }

    return false;

}

/*!
 * \brief OiJob::setUpDependencies
 * \param element
 * \param feature
 */
void OiJob::setUpDependencies(const InputElement &element, const QPointer<Feature> &feature){

    if(getIsFeature(element.typeOfElement)){

        switch(element.typeOfElement){
        case eCircleElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eConeElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eCylinderElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eEllipseElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eEllipsoidElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eHyperboloidElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eLineElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eNurbsElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eParaboloidElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case ePlaneElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case ePointElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case ePointCloudElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eScalarEntityAngleElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eScalarEntityDistanceElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eScalarEntityMeasurementSeriesElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eScalarEntityTemperatureElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eSlottedHoleElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eSphereElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eTorusElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eCoordinateSystemElement:{
            if(!element.coordSystem.isNull() && !element.coordSystem->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.coordSystem->getFeatureWrapper());
            }
            break;
        }case eStationElement:{
            if(!element.station.isNull() && !element.station->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.station->getFeatureWrapper());
            }
            break;
        }case eTrafoParamElement:{
            if(!element.trafoParam.isNull() && !element.trafoParam->getFeatureWrapper().isNull()){
                feature->addPreviouslyNeeded(element.trafoParam->getFeatureWrapper());
            }
            break;
        }
        }

    }else if(getIsObservation(element.typeOfElement)){

        if(!element.observation.isNull() && !feature->getFeatureWrapper().isNull() && !feature->getFeatureWrapper()->getGeometry().isNull()){
            feature->getFeatureWrapper()->getGeometry()->addObservation(element.observation);
        }

    }else if(getIsReading(element.typeOfElement)){

        return;

    }else if(element.typeOfElement != eUndefinedElement){

        if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
            feature->addPreviouslyNeeded(element.geometry->getFeatureWrapper());
        }

    }

}

/*!
 * \brief OiJob::resetDependencies
 * \param element
 * \param feature
 */
void OiJob::resetDependencies(const InputElement &element, const QPointer<Feature> &feature){

    if(getIsFeature(element.typeOfElement)){

        switch(element.typeOfElement){
        case eCircleElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eConeElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eCylinderElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eEllipseElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eEllipsoidElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eHyperboloidElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eLineElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eNurbsElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eParaboloidElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case ePlaneElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case ePointElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case ePointCloudElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eScalarEntityAngleElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eScalarEntityDistanceElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eScalarEntityMeasurementSeriesElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eScalarEntityTemperatureElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eSlottedHoleElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eSphereElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eTorusElement:{
            if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
            }
            break;
        }case eCoordinateSystemElement:{
            if(!element.coordSystem.isNull() && !element.coordSystem->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.coordSystem->getFeatureWrapper());
            }
            break;
        }case eStationElement:{
            if(!element.station.isNull() && !element.station->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.station->getFeatureWrapper());
            }
            break;
        }case eTrafoParamElement:{
            if(!element.trafoParam.isNull() && !element.trafoParam->getFeatureWrapper().isNull()){
                feature->removePreviouslyNeeded(element.trafoParam->getFeatureWrapper());
            }
            break;
        }
        }

    }else if(getIsObservation(element.typeOfElement)){

        if(!element.observation.isNull() && !feature->getFeatureWrapper().isNull() && !feature->getFeatureWrapper()->getGeometry().isNull()){
            element.observation->removeTargetGeometry(feature->getFeatureWrapper()->getGeometry());
        }

    }else if(getIsReading(element.typeOfElement)){

        return;

    }else if(element.typeOfElement != eUndefinedElement){

        if(!element.geometry.isNull() && !element.geometry->getFeatureWrapper().isNull()){
            feature->removePreviouslyNeeded(element.geometry->getFeatureWrapper());
        }

    }

}

/*!
 * \brief OiJob::addFeaturesFromXml
 * Add features when a project has been loaded (only accessible from ProjectExchanger)
 * \param features
 */
void OiJob::addFeaturesFromXml(const QList<QPointer<FeatureWrapper> > &features){

    foreach(const QPointer<FeatureWrapper> &feature, features){

        //check if feature is valid
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //check if the feature is a station system
        if(!feature->getCoordinateSystem().isNull() && feature->getCoordinateSystem()->getIsStationSystem()){
            continue;
        }

        //pass the job instance to the feature
        feature->getFeature()->job = this;
        for(QPointer<Function> function : feature->getFeature()->getFunctions()) {
            QObject::connect(function.data(), &Function::sendMessage, this, &OiJob::sendMessage, Qt::AutoConnection);
        }

        if(!feature->getStation().isNull()){

            //pass job to the station system
            if(!feature->getStation()->getCoordinateSystem().isNull()){
                feature->getStation()->getCoordinateSystem()->job = this;
            }

            //pass the job to the station point
            if(!feature->getStation()->getPosition().isNull()){
                feature->getStation()->getPosition()->job = this;
            }

        }else if(!feature->getCoordinateSystem().isNull()){
            if(feature->getCoordinateSystem()->getIsBundleSystem()){
                if(!feature->getCoordinateSystem()->getBundlePlugin().isNull()){
                    feature->getCoordinateSystem()->getBundlePlugin()->setCurrentJob(this);
                }
            }
        }

        //connect the feature's signals to slots in OiJob
        this->connectFeature(feature);

        //add the feature
        this->featureContainer.addFeature(feature);

        //save active feature
        if(feature->getFeature()->getIsActiveFeature()){
            this->activeFeature = feature;
        }

        //save active coordinate system
        if(!feature->getCoordinateSystem().isNull() && feature->getCoordinateSystem()->getIsActiveCoordinateSystem()){
            this->activeCoordinateSystem = feature->getCoordinateSystem();
        }else if(!feature->getStation().isNull() && !feature->getStation()->getCoordinateSystem().isNull()
                 && feature->getStation()->getCoordinateSystem()->getIsActiveCoordinateSystem()){
            this->activeCoordinateSystem = feature->getStation()->getCoordinateSystem();
        }

        //save active station
        if(!feature->getStation().isNull() && feature->getStation()->getIsActiveStation()){
            this->activeStation = feature->getStation();
        }

    }

}

void OiJob::createTemplateFromJob() {

    this->removeAllObservations(); // from geometries

    // set first station to active station
    QList<QPointer<Station> > stations = this->getStationsList();
    std:sort(stations.begin(), stations.end(),[] (const QPointer<Station>& lhs, const QPointer<Station>& rhs) {
        return lhs->getId() < rhs->getId();
    });

    // remove used sensors
    QList<Sensor> usedSensors;
    int i=0;
    for(QPointer<Station> station : stations) {
        if(++i=1) {
            blockSignals(true); // supress copying of sensor
            stations.first()->setActiveStationState(true);
            blockSignals(false);
            stations.first()->setActiveFeatureState(true);
        } else {
            stations.first()->setActiveStationState(false);
            stations.first()->setActiveFeatureState(false);
        }
        station->resetSensor();
        // remove used sensors
        station->setUsedSensors(usedSensors);
        // remove observations from coordinate system
        QPointer<CoordinateSystem> coordianteSystem = station->getCoordinateSystem();
        for(QPointer<Observation> obs : coordianteSystem->getObservations()) {
            coordianteSystem->removeObservation(obs);
        }
    }

    // remove observations from coordinate system
    QList<QPointer<CoordinateSystem> > coordianteSystems = this->getCoordinateSystemsList();
    for(QPointer<CoordinateSystem> coordianteSystem : coordianteSystems) {
        for(QPointer<Observation> obs : coordianteSystem->getObservations()) {
            coordianteSystem->removeObservation(obs);
        }

        QPointer<BundleAdjustment> bundle = coordianteSystem->getBundlePlugin();
        if(!bundle.isNull()) {
            bundle->clearResults();
        }
    }

    // remove transformations parameter
    QList<QPointer<TrafoParam> > trafoParams = this->getTransformationParametersList();
    OiVec rotation(3);
    OiVec translation(3);
    OiVec scale(3);
    scale.setAt(0,1.);
    scale.setAt(1,1.);
    scale.setAt(2,1.);
    for(QPointer<TrafoParam> trafoParam : trafoParams) {
        trafoParam->setTransformationParameters(rotation, translation, scale);
        trafoParam->setIsSolved(false);
    }
}

