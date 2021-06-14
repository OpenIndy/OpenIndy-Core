#include "element.h"

using namespace oi;

Element::Element(QObject *parent) : QObject(parent), id(-1), isNullObject(false){

}

Element::Element(const Element &copy, QObject *parent) : QObject(parent){
    this->id = copy.id;
    this->isNullObject = copy.isNullObject;
}

Element &Element::operator=(const Element &copy){
    this->id = copy.id;
    this->isNullObject = copy.isNullObject;
    return *this;
}

Element::~Element(){

}

/*!
 * \brief Element::getId
 * \return
 */
int Element::getId() const{
    return this->id;
}

/*!
 * \brief Element::setId
 * \param id
 */
void Element::setId(const int &id){
    this->id = id;
}

/*!
 * \brief Element::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Element::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    QDomElement element = xmlDoc.createElement("element");
    element.setAttribute("id", this->getId());

    return element;

}

/*!
 * \brief Element::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Element::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }

    if(!xmlElem.hasAttribute("id")){
        return false;
    }

    this->id = xmlElem.attribute("id").toInt();

    return true;

}

/*!
 * null object or invalid object
 * @brief Element::isNull
 * @return
 */
bool Element::isNull() {
    return isNullObject;
}
