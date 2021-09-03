#include "image.h"

Image::Image(){

}

Image::Image(QString imgName,QString imgPath, QDate imgDate) : imageName(imgName), imagePath(imgPath), imageDate(imgDate)
{
}

QString Image::getName(){
    return imageName;
}

QString Image::getPath(){
    return imagePath;
}

QDate Image::getDate(){
    return imageDate;
}
