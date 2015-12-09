#ifndef EXCHANGEDEFINEDFORMAT_H
#define EXCHANGEDEFINEDFORMAT_H

#include "exchangeinterface.h"

namespace oi{

/*!
 * \brief The OiExchangeDefinedFormat class
 * Interface that shall be used for files with a special standardized format (like *.obj or *.pts etc.)
 */
class OI_CORE_EXPORT ExchangeDefinedFormat : public ExchangeInterface
{
    Q_OBJECT

public:
    explicit ExchangeDefinedFormat(QObject *parent = 0);

    virtual ~ExchangeDefinedFormat();

    //#########################################
    //methods to get or set further information
    //#########################################

    const QStringList &getSupportedFileExtensions() const;

    const QList<ElementTypes> &getUsedElements() const;
    void setUsedElements(const QList<ElementTypes> &usedElementTypes);

protected:

    //###########################
    //input and output parameters
    //###########################

    QList<ElementTypes> usedElementTypes; //types of elements that shall be im- or exported

    QStringList supportedFileExtensions;

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

#define EXCHANGE_DEFINEDFORMAT_PREFIX de.openIndy.plugin.exchange.exchangeDefinedFormat.v
#define ExchangeDefinedFormat_iidd STR2(CONCAT2(EXCHANGE_DEFINEDFORMAT_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // EXCHANGEDEFINEDFORMAT_H
