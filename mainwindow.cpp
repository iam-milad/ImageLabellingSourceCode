#include "mainwindow.h"
#include "linkedlist.h"

#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QTreeWidgetItem>
#include <fstream>
#include <QInputDialog>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new Scene(this);     //Create scene to display image

    view = new QGraphicsView(this);       //visualise the scene as it is invisible by default
    view->setScene(scene);

    imgLinkedList = new LinkedList<Image>();
    clsLinkedlist = new LinkedList<IClass>();

    ui->imgList->setMaximumWidth(320);     //Set the max widget size
    ui->imgList->setMaximumHeight(300);
    ui->classesList->setMaximumWidth(320);
    ui->annotationList->setMaximumWidth(320);

    ui->imageDisplay->addWidget(view);


    // Add tool buttons to the action group. Now the QT will manage the pushed toolbar buttons.
    QActionGroup* actionGroup = new QActionGroup(this);

    for (auto const& iT : ui->mainToolBar->actions())
        actionGroup->addAction(iT);

    // connect slots to the actions.
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSave);
    connect(ui->actionSelect, &QAction::triggered, this, &MainWindow::onSelectTriggered);
    connect(ui->actionLine, &QAction::triggered, this, &MainWindow::onLineTriggered);
    connect(ui->actionRectangle, &QAction::triggered, this, &MainWindow::onRectangleTriggered);
    connect(ui->actionRotate, &QAction::triggered, this, &MainWindow::onRectangleRotateTriggered);

    //Connect lamda functions to the actions
    connect(ui->actionTrapezoid, &QAction::triggered, this, [=](bool aChecked) {
        if (aChecked)
        {
            view->setDragMode(QGraphicsView::RubberBandDrag);
            scene->setMode(Scene::Mode::DrawTrapezoid);
        }
    });

    connect(ui->actionaddPoligon, &QAction::triggered, this, [=](bool aChecked) {
        if (aChecked)
            scene->setMode(Scene::Mode::DrawPoligon);
    });
    ui->mainToolBar->setDisabled(true);
    ui->openButton->setDisabled(true);
    doubleClickedImg = false;
    doubleClickedClass = false;

}

MainWindow::~MainWindow()
{
    delete ui;
    delete imgLinkedList;
    delete clsLinkedlist;
}


void MainWindow::on_browseButton_clicked()
{

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if ( QDialog::Accepted == dialog.exec())
    {
        QStringList filenames = dialog.selectedFiles();
        QStringList::const_iterator it = filenames.begin();
        QStringList::const_iterator eIt = filenames.end();
        while ( it != eIt )
        {
            QString filePath = *it++;

            QFileInfo f(filePath);
            QString fileName = f.fileName(); //get the file name and extension only


            QString fileDate = f.created().toString("ddMMyyyy"); //get the image date on which it was created
            QDate sourceDate = QDate::fromString(fileDate, "ddMMyyyy");
            //int difference = QDate::currentDate().toJulianDay() - sourceDate.toJulianDay(); /*get difference between current date and
                                                                                      // file date for sorting by date*/
            if(fileName != NULL){
                image = Image(fileName,filePath,sourceDate);

            }
            if(ImgNodeAdded() == true){
                addNodeToImgPane(); //Once the image is  added to the linked list, then add it to the image pane list widget
            }else{
                QMessageBox msgBox;
                msgBox.setText("The " + fileName + " image already exist!");
                msgBox.exec();
            }

        }
    }

}

bool MainWindow::ImgNodeAdded(){

    static bool isFirstImg = true;

    if(isFirstImg){
        imgLinkedList->createnode(image);
        isFirstImg = false;
        return true;
    }
    else if(imgLinkedList->nodeItemAlreadyExist(image.getName()) == false && !isFirstImg){
        imgLinkedList->createnode(image);
        return true;
    }
    return false;
}
bool MainWindow::classNodeAdded(){

    static bool isFirstClass = true;

    if(isFirstClass){
        clsLinkedlist->createnode(theClass);
        isFirstClass = false;
        return true;
    }
    else if(clsLinkedlist->nodeItemAlreadyExist(theClass.getName()) == false && !isFirstClass){
        clsLinkedlist->createnode(theClass);
        return true;
    }
    return false;
}

void MainWindow::addNodeToImgPane(){

    ui->imgList->clear();
    int size = imgLinkedList->getSize();
    int count = 1;

    while(count <= size){

        QTreeWidgetItem *nodeToAdd = imgLinkedList->getImageItem(count,ui->imgList);
        ui->imgList->addTopLevelItem(nodeToAdd);
        count++;
    }
}

void MainWindow::addNodeToClassPane(){

    ui->classesList->clear();
    int size = clsLinkedlist->getSize();
    int count = 1;

    while(count <= size){

        QListWidgetItem *nodeToAdd = clsLinkedlist->getClassItem(count,ui->classesList);
        ui->classesList->addItem(nodeToAdd);
        count++;
    }
}

void MainWindow::on_sortImages_activated(const QString &arg1)  //When image pane drop down menu item is clicked,this function will be called
{
    ui->imgList->clear(); //Clear the image pane, useful when sorted linked list items are re-added

    QString option = arg1; //get the selected sorting option text

    if(option == "Name Ascending"){     //check whether option is sort by name or sort by date and execute sorting funtion accordingly
        imgLinkedList->sortByNameAscending();
        addNodeToImgPane();
    }else if(option == "Name Descending"){
        imgLinkedList->sortByNameDescending();
        addNodeToImgPane();
    }else if(option == "Date Ascending"){
        imgLinkedList->sortByDateAscending();
        addNodeToImgPane();
    }else if(option == "Date Descending"){
        imgLinkedList->sortByDateDescending();
        addNodeToImgPane();
    }
}

void MainWindow::on_browseClass_clicked(){
    QString clsFilePath = QFileDialog::getOpenFileName(this, tr("OpenFile"), "C:/", "Image File(*.names)");

    QFile classFile(clsFilePath);
    if (classFile.open(QIODevice::ReadOnly))
    {
       classFilePath = clsFilePath; // if file is opened second time and cancel button is clicked, store the previously opened file path
       QTextStream in(&classFile);
       while (!in.atEnd())
       {
         className = in.readLine();
         if(className != NULL){
             theClass = IClass(className);
             addOrRefuseClass(className,&classFile);
         }
       }

       classFile.close();
    }

    //addNodeToClassPane();

}

void MainWindow::on_imgList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    doubleClickedImg = true;
    if(doubleClickedClass)
        ui->mainToolBar->setDisabled(false);
    ui->openButton->setDisabled(false);
    QString imgPath = imgLinkedList->returnImgPath(item->text(0)); //get the image path to display the selected image
    scene->clear(); //Clear the scene to avoid images being displayed on top of each other

    QPixmap pix(imgPath);
    scene->addPixmap(pix.scaled(1000,800,Qt::KeepAspectRatio));  //add the image to the scene

}

void MainWindow::on_sortClasses_activated(const QString &arg1)
{
    ui->classesList->clear();

    QString option = arg1;

    if(option == "Ascending"){
        clsLinkedlist->sortByNameAscending();
        addNodeToClassPane();
    }else if(option == "Descending"){
        clsLinkedlist->sortByNameDescending();
        addNodeToClassPane();
    }
}

void MainWindow::addOrRefuseClass(QString className, QFile *file){

    QString errorMsg = "The " + className + " class already exist!";

    try{
        theClass = IClass(className);
        if(classNodeAdded() == true){
            QTextStream out(&*file);
            out << theClass.getName() << "\n";
            addNodeToClassPane();

        }else{
            throw errorMsg;
        }
    }catch(QString err){
        QMessageBox msgBox;
        msgBox.setWindowTitle("ERROR");
        msgBox.setStyleSheet("QLabel{min-width: 250px;}");
        msgBox.setText(err);
        msgBox.exec();
    }
}

void MainWindow::on_createClass_clicked()
{
    bool ok;
    QString newClassName = QInputDialog::getText(this, tr("Create New Class"),
                                         tr("Enter New Class Name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !newClassName.isEmpty()){

        QFile file(classFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
            QMessageBox msgBox;
            msgBox.setText("Class file is not opened");
            msgBox.exec();
            return;
        }
        theClass = IClass(newClassName);
        addOrRefuseClass(newClassName,&file);
    }
}

void MainWindow::on_deleteClass_clicked()
{
    QFile f(classFilePath);
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(!line.contains(classItemName)){
                s.append(line + "\n");
            }else if(line.contains(classItemName)){
                s.append("\n");
                clsLinkedlist->deleteNode(classItemName);
                addNodeToClassPane();
            }
        }
        f.resize(0);
        t << s;
        f.close();
    }
}

void MainWindow::on_classesList_itemClicked(QListWidgetItem *item)
{
    doubleClickedClass = true;
    if(doubleClickedImg)
        ui->mainToolBar->setDisabled(false);

    classItemName = item->text();
    scene->setClassName(classItemName);
}


void MainWindow::onSelectTriggered(bool aChecked)
{
    // "Select Mode" is active
    if (aChecked)
    {
        scene->setMode(Scene::Mode::SelectObject);
        view->setDragMode(QGraphicsView::RubberBandDrag);
    }
}

void MainWindow::onLineTriggered(bool aChecked)
{
    // "Draw Line Mode" is active
    if (aChecked)
    {
        view->setDragMode(QGraphicsView::NoDrag);
        scene->setMode(Scene::Mode::DrawLine);
    }
}

void MainWindow::onRectangleTriggered(bool aChecked)
{
    // Add a rectangle mode is active.
    if (aChecked)
    {
        view->setDragMode(QGraphicsView::RubberBandDrag);
        scene->setMode(Scene::Mode::DrawRectangle);
    }
}

void MainWindow::onRectangleRotateTriggered(bool aChecked)
{
    // Rotate rectangle mode is active.
    if (aChecked)
    {
        scene->setMode(Scene::Mode::RotateRectangle);
    }
}

void MainWindow::onSave()
{
    QString fName = QFileDialog::getSaveFileName(this,
        tr("Save"), "",
        tr("Json File (*.json)"));

    scene->save(fName);
}


void MainWindow::on_openButton_clicked(){
    QString json_filter = "JSON (*.json)";

    QString jsonFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "/", json_filter,
                                                        &json_filter);
    if(jsonFilePath.isEmpty())
        return;

    QFile f(jsonFilePath);
    QFileInfo fileInfo(f.fileName());
    QString filename(fileInfo.fileName());

    list.append(qMakePair(filename, jsonFilePath));

    ui->annotationList->addItem(filename);

}

QString MainWindow::getJsonFilePath(QListWidgetItem *anItem){

    QString filePath;

    for(int x = 0; x < list.size(); x++){
        if(list[x].first == anItem->text()){
            filePath = list[x].second;
        }
    }

    return filePath;
}

void MainWindow::sendShapeCoordinates(QList<double> *pList, QString *objName, int type){

    if(!pList->isEmpty()){

        if(type == 2)
            scene->setupShapes(pList,objName,2);
        else if(type == 3)
            scene->setupShapes(pList,objName,3);
        else if(type == 4)
            scene->setupShapes(pList,objName,4);
    }
}

void MainWindow::on_annotationList_itemDoubleClicked(QListWidgetItem *item){

    ui->mainToolBar->setDisabled(false);

    QString jsonFilePath = getJsonFilePath(item);

    QString val;
    QFile file;
    file.setFileName(jsonFilePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject sett2 = d.object();

    QList<double> rectList;
    QList<double> polyList;
    QList<double> trapList;

    QString rObjName = "";
    QString tObjName = "";
    QString pObjName = "";

    QJsonArray objsArray = sett2.value("objects").toArray();

    for(int i = 0; i < objsArray.size(); i++){

        QJsonObject item = objsArray[i].toObject();
        QJsonValue subobj = item["shape"];
        QJsonValue subobj2 = item["object"];

        QJsonArray test = item["coordinates"].toArray();

        for(int j = 0; j < test.size(); j++){
            if(subobj.toString() == "Rectangle"){
                rectList.append(test[j].toDouble());
                rObjName = subobj2.toString();
            }else if(subobj.toString() == "Trapezoid"){
                trapList.append(test[j].toDouble());
                tObjName = subobj2.toString();
            }else if(subobj.toString() == "Polygon"){
                polyList.append(test[j].toDouble());
                pObjName = subobj2.toString();
            }
        }
        sendShapeCoordinates(&rectList, &rObjName, 2);
        sendShapeCoordinates(&trapList, &tObjName, 3);
        sendShapeCoordinates(&polyList, &pObjName, 4);
        rectList.clear();
        polyList.clear();
        trapList.clear();
    }

}
