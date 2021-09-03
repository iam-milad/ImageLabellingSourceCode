#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include "linkedlist.h"
#include "image.h"
#include "iclass.h"
#include "scene.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDateTime>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class interacts with all other widgets, such as adding and removing items from QWidgetList
 */
class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    /*!
     * \brief constructor intialises object, variables and widgets
     * \param parent parameter is a null pointer by default which takes other widgets as its parent
     */
    MainWindow(QWidget *parent = nullptr);
    /*!
      *\brief destructor deallocates memory taken by dynamically allocated memory
      */
    ~MainWindow();

private:
    /*!
     * \brief method returns whether an image node is added to the linkedlist (adding an image that already exist in the linkedlist will be refused)
     * \return return true if image node is added or false if it's is not added
     */
    bool ImgNodeAdded();
    /*!
     * \brief method returns whether a class node is added to the linkedlist (adding a class that already exist in the linkedlist will be refused)
     * \return returns true if the class node is added or returns false if it's is not added
     */
    bool classNodeAdded();
    /*!
     * \brief addNodeToImgPane method add all the images that are in the linkedlist to the image pane widget
     */
    void addNodeToImgPane();
    /*!
     * \brief addNodeToClassPane method add all the class that are in the linkedlist to the class pane
     */
    void addNodeToClassPane();
    /*!
     * \brief addOrRefuseClass method accepts and adds class to the linkedlist if it's not in the linkedlist already, otherwise exception will be thrown
     * \param className is the class name
     * \param contains classes
     */
    void addOrRefuseClass(QString className, QFile *file);
    /*!
     * \brief sendShapeCoordinates method send the coordinates of the annotated shapes that are in the json file to a method in the Scene class so it can display it
     * \param pList is list of coordinates which can be of rectangle, trapezoid or polygon
     * \param objName is the object or class name e.g. Animal, Car, tree etc
     * \param type is the type of the shape, 2 = rectangle, 3 == trapezoid and 4 = polygon
     */
    void sendShapeCoordinates(QList<double> *pList, QString *objName, int type);
    /*!
     * \brief getJsonFilePath method gets the json file path when and item in the annotaion pane is double click, this enbale the annotated shapes to be displayed automatically
     * \param anItem is the name of the json file
     * \return returns the json file path as string
     */
    QString getJsonFilePath(QListWidgetItem *anItem); 

private slots:
    /*!
     * \brief on_browseButton_clicked methos gets triggered when image pane browse button is clicked
     */
    void on_browseButton_clicked();
    /*!
     * \brief on_sortImages_activated method is triggered when an option from drop down menu is clicked on the image pane
     * \param arg1 is the item from the drop down menu that's clicked
     */
    void on_sortImages_activated(const QString &arg1);
    /*!
     * \brief on_imgList_itemDoubleClicked method get triggered when an image item is double click and then display the image to the scene
     * \param item is the image item with date on the tree widget
     * \param column is the column number
     */
    void on_imgList_itemDoubleClicked(QTreeWidgetItem *item, int column);
    /*!
     * \brief on_browseClass_clicked method is triggered when class browse button is clicked and opens the classes
     */
    void on_browseClass_clicked();
    /*!
     * \brief on_sortClasses_activated method is triggered when an option from drop down menu is clicked on the class pane
     * \param arg1 is the item from the drop down menu that's clicked
     */
    void on_sortClasses_activated(const QString &arg1);
    /*!
     * \brief on_createClass_clicked method is triggered when create class button is clicked and displays a box for the user to enter the new class name
     */
    void on_createClass_clicked();
    /*!
     * \brief on_deleteClass_clicked method is called when delete button the class pane is click
     */
    void on_deleteClass_clicked();
    /*!
     * \brief on_classesList_itemClicked method is triggered when a class item on the class pane is clicked, which then sends gets the item text and assigns it to a memeber variable
     * \param item is the class item clicked
     */
    void on_classesList_itemClicked(QListWidgetItem *item);
    /*!
     * \brief onSelectTriggered method is triggered when select arrow on the toolbar is clicked
     * \param aChecked determins whether select arrow is checked
     */
    void onSelectTriggered(bool aChecked);
    /*!
     * \brief onLineTriggered method is triggered when line on the toolbar is clicked
     * \param aChecked determins whether line is checked
     */
    void onLineTriggered(bool aChecked);
    /*!
     * \brief onRectangleTriggered method is triggered when rectangle on the toolbar is clicked
     * \param aChecked determins whether rectangle is checked
     */
    void onRectangleTriggered(bool aChecked);
    /*!
     * \brief onRectangleRotateTriggered method is triggered when rotate on the toolbar is clicked
     * \param aChecked determins whether rotate is checked
     */
    void onRectangleRotateTriggered(bool aChecked);
    /*!
     * \brief onSave method is triggered when save button is clicked
     */
    void onSave();
    /*!
     * \brief on_openButton_clicked methods is called when open button on the annotation pane is clicked
     */
    void on_openButton_clicked();
    /*!
     * \brief on_annotationList_itemDoubleClicked method gets triggered when a json file item is double clicked
     * \param item is the json file item on the annotaion pane
     */
    void on_annotationList_itemDoubleClicked(QListWidgetItem *item);

private:
    /*!
     * \brief ui is used for adding and removing widget items from GUI widgets on the mainwindow
     */
    Ui::MainWindow *ui;
    /*!
     * \brief imgLinkedList is an object of LinkedList class which used for dealing with images stored in the linkedlist
     */
    LinkedList<Image> *imgLinkedList;
    /*!
     * \brief clsLinkedlist is an object of LinkedList class which used for dealing with classes stored in the linkedlist
     */
    LinkedList<IClass> *clsLinkedlist;
    QString filePath;
    /*!
     * \brief scene is an object of Scene class which is used for adding and removing items from the scene such as images and shapes
     */
    Scene *scene;
    /*!
     * \brief view is an object of QGraphicsView which visualises the scene
     */
    QGraphicsView *view;
    /*!
     * \brief image is an object of Image class used for dealing with images name,path and date
     */
    Image image;
    /*!
     * \brief theClass is an object of IClass used for dealing with class
     */
    IClass theClass;
    /*!
     * \brief className variable is used for storing the className that's used by different methods
     */
    QString className;
    /*!
     * \brief classFilePath is for storing the class file path that's used by different methods
     */
    QString classFilePath;
    /*!
     * \brief classItemName stores the class name thats on the class pane widget
     */
    QString classItemName;
    /*!
     * \brief doubleClickedImg is used to determine whether an image item is double clicked (if an image is double clicked and a class item is clicked then the toolbar will be enabled)
     */
    bool doubleClickedImg;
    /*!
     * \brief doubleClickedClass is used to determine whether a class item is clicked (if an image is double clicked and a class item is clicked then the toolbar will be enabled)
     */
    bool doubleClickedClass;
    /*!
     * \brief list is used for storing the pair of json file name and path
     */
    QList<QPair<QString, QString> > list;
};
#endif // MAINWINDOW_H
