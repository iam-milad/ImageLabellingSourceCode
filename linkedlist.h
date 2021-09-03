#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"

#include <QString>
#include <QDate>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QListWidgetItem>


/*!
 * \brief The LinkedList class is used to implement linkedlist and store the images and classes (template is used to make this class generic)
 */
template <typename T>
class LinkedList{
public:
    /*!
     * \brief LinkedList construtor intialises the head and tail to null pointers
     */
    LinkedList();
    /*!
      *\brief LinkedList destructor deletes all the nodes just before the program is closed
      */
    ~LinkedList();
    /*!
     * \brief createnode method takes generic data types (in this case image and class objects) and create node for it
     * \param T is generic data type
     */
    void createnode(T);
    /*!
     * \brief deleteNode method deletes class node
     * \param className is class name passed to deleteNode method
     */
    void deleteNode(QString className);
    /*!
     * \brief getImageItem method gets the image item from the linkedlist
     * \param index is index of the image where it is stored
     * \param imgItem is the images tree widget, which is passed to QTreeWidgetItem as type
     * \return returns pointer to the tree widget item that going to be added to the image tree widget
     */
    QTreeWidgetItem *getImageItem(int index, QTreeWidget *imgItem);
    /*!
     * \brief getClassItem method gets the class item from the linkedlist
     * \param index is index of the class where it is stored
     * \param clsItem is the class list widget, which is passed to QListWidgetItem as listview
     * \return returns pointer to the list widget item, that's going to be added to class list widget
     */
    QListWidgetItem *getClassItem(int index, QListWidget *clsItem);
    /*!
     * \brief nodeItemAlreadyExist determines whether the node to be added already exit in the linkedlist
     * \param gName is the name of a class or an image
     * \return return true if node item already exist and false if it doesn't
     */
    bool nodeItemAlreadyExist(QString gName);
    /*!
     * \brief returnImgPath method returns the image path when it's clicked on the image pane
     * \param imgName is hold the image name
     * \return returns the image path
     */
    QString returnImgPath(QString imgName);
    /*!
     * \brief sortByNameAscending method sorts images or classes by name in ascending order
     */
    void sortByNameAscending();
    /*!
     * \brief sortByNameDescending method sorts images or classes in descending order
     */
    void sortByNameDescending();
    /*!
     * \brief sortByDateAscending method sorts images by date in ascending order
     */
    void sortByDateAscending();
    /*!
     * \brief sortByDateDescending method sorts images by date in descending order
     */
    void sortByDateDescending();
    /*!
     * \brief getSize method gets the size of the linkedlist
     * \return returns the size of the linkedlist
     */
    int getSize();

private:
    /*!
     * \brief head is the starting node of the linkedlist
     */
    node<T> *head;
    /*!
     * \brief tail is the end node of the linkedlist
     */
    node<T> *tail;
};


template <typename T>
LinkedList<T>::LinkedList(){
        head = nullptr;
        tail = nullptr;
}

template <typename T>
LinkedList<T>::~LinkedList(){

        while (head != nullptr)
        {
            node<T> *tmpNode = head->next;
            delete head;
            head = tmpNode;
        }

}

template <typename T>
bool LinkedList<T>::nodeItemAlreadyExist(QString gName){

    node<T> *temp=new node<T>;
    temp=head;

    while(temp!=nullptr)
    {
        if(gName == temp->data.getName()){
            return true;
        }
        temp=temp->next;
    }
    return false;
}

template <typename T>
void LinkedList<T>::createnode(T value){

        node<T> *temp = new node<T>;
        temp->data=value;
        temp->next=nullptr;

        if(head==nullptr)
        {
            head=temp;
            tail=temp;
            temp=nullptr;
        }
        else
        {
            tail->next=temp;
            tail=temp;
        }
}

template <typename T>
void LinkedList<T>::deleteNode(QString className){

    node<T> *current=new node<T>;
    node<T> *previous=new node<T>;
    current=head;
    for(int i=0;i<getSize();i++)
    {
        if(className == head->data.getName()){
            node<T> *temp=new node<T>;
            temp=head;
            head=head->next;
            delete temp;
            break;
        }else if(className == current->data.getName()){
            previous->next=current->next;
            delete current;
            break;
        }
        previous=current;
        current=current->next;
    }
}

template <typename T>
QString LinkedList<T>::returnImgPath(QString imgName){
    QString imgPath;
    node<T> *temp=new node<T>;
    temp=head;
    while(temp!=nullptr)
    {
        if(imgName == temp->data.getName()){
            imgPath = temp->data.getPath();
            break;
        }
        temp=temp->next;
    }
    return imgPath;
}

template <typename T>
void LinkedList<T>::sortByNameAscending(){

    node<T> *i, *j;

    for(i=head; i != nullptr; i = i->next){

        for(j=i->next; j != nullptr; j = j->next){
            QChar firstChari = i->data.getName()[0].toUpper();
            QChar firstCharj = j->data.getName()[0].toUpper();
            if(firstChari > firstCharj){
                std::swap(i->data,j->data);
            }
        }
    }
}

template <typename T>
void LinkedList<T>::sortByNameDescending(){

    node<T> *i, *j;

    for(i=head; i != nullptr; i = i->next){

        for(j=i->next; j != nullptr; j = j->next){
            QChar firstChari = i->data.getName()[0].toUpper();
            QChar firstCharj = j->data.getName()[0].toUpper();
            if(firstChari < firstCharj){
                std::swap(i->data,j->data);
            }
        }
    }
}

template <typename T>
void LinkedList<T>::sortByDateAscending(){

    node<T> *i, *j;

    for(i=head; i != nullptr; i = i->next){
        for(j=i->next; j != nullptr; j = j->next){
            if(i->data.getDate().daysTo(QDate::currentDate()) > j->data.getDate().daysTo(QDate::currentDate())){
                std::swap(i->data,j->data);
            }
        }
    }
}

template <typename T>
void LinkedList<T>::sortByDateDescending(){

    node<T> *i, *j;

    for(i=head; i != nullptr; i = i->next){
        for(j=i->next; j != nullptr; j = j->next){
            if(i->data.getDate().daysTo(QDate::currentDate()) < j->data.getDate().daysTo(QDate::currentDate())){
                std::swap(i->data,j->data);
            }
        }
    }
}

template <typename T>
int LinkedList<T>::getSize(){

    int count = 0;
    node<T> *temp=new node<T>;
    temp=head;
    while(temp!=nullptr)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

template <typename T>
QTreeWidgetItem *LinkedList<T>::getImageItem(int index, QTreeWidget *imgItem){

    QTreeWidgetItem *img;

    node<T> *temp=new node<T>;
    temp=head;
    img = new QTreeWidgetItem(imgItem);

    for(int i = 0; i < index; i++)
    {
        img->setText(0,temp->data.getName());
        img->setText(1,temp->data.getDate().toString());

        temp = temp->next;
    }

  return img;
}

template <typename T>
QListWidgetItem *LinkedList<T>::getClassItem(int index, QListWidget *clsItem){

    QListWidgetItem *classItem;
    node<T> *temp=new node<T>;
    temp=head;
    classItem = new QListWidgetItem(clsItem);

    for(int i = 0; i < index; i++)
    {
        classItem->setText(temp->data.getName());

        temp = temp->next;
    }

    return classItem;
}




#endif // LINKEDLIST_H
