#ifndef IMAGE_H
#define IMAGE_H


#include <QString>
#include <QDate>

class Image{
public:
    /*!
     * \brief Image default constructor is required when declaring an object of type Image
     */
    Image();
    /*!
     * \brief Image constructor intialises image name,path and date
     */
    Image(QString, QString, QDate);
    /*!
     * \brief getName method gets the image name
     * \return returns the image name
     */
    QString getName();
    /*!
     * \brief getPath method gets the image path
     * \return returns the image path
     */
    QString getPath();
    /*!
     * \brief getDate method gets the image date
     * \return returns the image date
     */
    QDate getDate();
private:
    /*!
     * \brief imageName variable stores the image name
     */
    QString imageName;
    /*!
     * \brief imagePath variable stores the image path
     */
    QString imagePath;
    /*!
     * \brief imageDate variable stores the image date
     */
    QDate imageDate;
};

#endif // IMAGE_H
