#ifndef FUNCTIONCONTEXT_H
#define FUNCTIONCONTEXT_H
namespace oi {

// TODO remvoe OI_EXEC_CONTEXT
struct FunctionContext { // TODO refactoring to class
    float referenceTemperature = 20.0;
    float actualTemperatur = 20.0;
    QString material = "steel"; // TODO use enum
};

}
#endif // FUNCTIONCONTEXT_H

