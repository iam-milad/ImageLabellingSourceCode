#include "iclass.h"

IClass::IClass(){

}

IClass::IClass(QString theClass)
{
    className = theClass;
}

QString IClass::getName(){
    return className;
}
