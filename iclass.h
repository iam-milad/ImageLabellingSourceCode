#ifndef ICLASS_H
#define ICLASS_H

#include <QString>
/*!
 * \brief The IClass class is used for classes (note: using string value only wasn't an effeciant way since a getName method was needed for sorting to be identical with Image class)
 */
class IClass
{
public:
    /*!
     * \brief IClass default contructor is required when object is created without passing any values
     */
    IClass();
    /*!
     * \brief IClass constructor initialises the class name
     * \param theClass variable holds the class namme
     */
    IClass(QString theClass);
    /*!
     * \brief getName method gets the class name
     * \return returns the class name
     */
    QString getName();

private:
    /*!
     * \brief className variable stores the class name
     */
    QString className;
};

#endif // ICLASS_H
