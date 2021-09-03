#include "scene.h"
#include <QActionGroup>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <math.h>

#define DATA_SHAPETYPE 0

#define SHAPE_LINE		1
#define SHAPE_RECT		2
#define SHAPE_TRAPEZOID	3
#define SHAPE_POLYGON	4

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
    , m_Mode(Mode::NoMode)
    , m_origPoint()
    , m_itemToDraw(nullptr)
    , m_CurrentPolygon(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::setMode(Mode aMode)
{
    m_Mode = aMode;
    m_CurrentPolygon = nullptr; // for the add polygon function.
}

void Scene::save(const QString& aFileName)
{
    QJsonDocument doc;
    QJsonArray a;
    QJsonObject shapes;

    for (auto const &iT : items())
    {
        QJsonArray res;
        QJsonObject o;

        switch (iT->data(DATA_SHAPETYPE).toInt()) // Get the type of the selected object.
        {
        case SHAPE_RECT:
        {
            QGraphicsRectItem *ri = static_cast<QGraphicsRectItem*>(iT);
            o.insert("object", ri->toolTip());
            o.insert("shape","Rectangle");


            //Scene relative coordinates.
            res.append(ri->mapToScene(ri->rect().topLeft()).x());
            res.append(ri->mapToScene(ri->rect().topLeft()).y());
            res.append(ri->rect().width());
            res.append(ri->rect().height());

        }
        break;
        case SHAPE_POLYGON:
        {
            QGraphicsPolygonItem *ri = static_cast<QGraphicsPolygonItem*>(iT);

            o.insert("object", ri->toolTip());
            o.insert("shape","Polygon");
            for (auto const& iT : ri->polygon())
            {
                QPointF sc = ri->mapToScene(iT);
                res.append(sc.x());
                res.append(sc.y());
            }
        }
        break;
        case SHAPE_TRAPEZOID:
        {
            QGraphicsPolygonItem *ri = static_cast<QGraphicsPolygonItem*>(iT);
            o.insert("object", ri->toolTip());
            o.insert("shape","Trapezoid");
            for (auto const& iT : ri->polygon())
            {
                QPointF sc = ri->mapToScene(iT);
                res.append(sc.x());
                res.append(sc.y());

            }
        }
        break;
        default:
            break;
        }
        if(!res.isEmpty()){
            o.insert("coordinates", res);
            a.push_back(o);
        }
    }

    shapes.insert("TotalShapes",items().size()-1);
    shapes.insert("objects", a);

    doc.setObject(shapes);

    QFile jsonFile(aFileName); //Save to shape.json the selected object.
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
    jsonFile.close();

}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *aEvent)
{
    setSelectable(false);

    switch (m_Mode)
    {
    case Mode::DrawLine:
        m_origPoint = aEvent->scenePos(); // Draw line from the mouse press position.
        break;
    case Mode::DrawTrapezoid:
        //addTrapezoid(aEvent); // Add fix size trapezoid to the scene
        break;
    case Mode::DrawRectangle:
        //setSelectable(true);
        //addRectangle(aEvent); // Add fix size rectangle to the scene
        break;
    case Mode::SelectObject:
        setSelectable(true); // Select mode selected. Now can select and move objects in the scene
        break;
    case Mode::Edit:
        setSelectable(true); // Edit objects
        break;
    case Mode::RotateRectangle:
        setSelectable(true); // Rotate Rectangle
        m_origPoint = aEvent->scenePos();
        break;
    case Mode::DrawPoligon:
        addPolygonPoint(aEvent); // Add point to the polygon.
        break;
    default:
        break;
    }

    QGraphicsScene::mousePressEvent(aEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *aEvent)
{
    switch (m_Mode)
    {
        case Mode::DrawLine:
            drawSceneLine(aEvent);
            break;
        case Mode::SelectObject:
            break;
        case Mode::Edit:
            if (selectedItems().size() == 1) // only one selected item allowed for edit.
            {
                QGraphicsItem *gi = static_cast<QGraphicsItem*>(selectedItems()[0]);
                selectedItems()[0]->setFlag(QGraphicsItem::ItemIsMovable, false);

                switch (gi->data(DATA_SHAPETYPE).toInt()) // get the type of the selected graphics item.
                {
                case SHAPE_RECT:
                    editRectangle(aEvent);
                    break;
                case SHAPE_POLYGON:
                    editPolygon(aEvent, false);
                    break;
                case SHAPE_TRAPEZOID:
                    editTrapezoid(aEvent);
                    break;
                default:
                    break;
                }
            }
            break;
        case Mode::RotateRectangle:
            if (selectedItems().size() == 1) // only one selected item allowed for rotate.
            {
                QGraphicsItem *gi = static_cast<QGraphicsItem*>(selectedItems()[0]);
                selectedItems()[0]->setFlag(QGraphicsItem::ItemIsMovable, false);
                if (gi->data(DATA_SHAPETYPE).toInt() == SHAPE_RECT)
                {
                    rotateRectangle(aEvent);
                }
                else if (gi->data(DATA_SHAPETYPE).toInt() == SHAPE_TRAPEZOID)
                {
                    rotateTrapezoid(aEvent);
                }
            }
        default:
            break;

    }

    QGraphicsScene::mouseMoveEvent(aEvent);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *aEvent)
{
    m_itemToDraw = nullptr;

    switch (m_Mode)
    {
    case Mode::DrawRectangle:
        addRectangle(aEvent);
        break;
    case Mode::DrawTrapezoid:
        addTrapezoid(aEvent);
        break;
    default:
        break;
    }

    QGraphicsScene::mouseReleaseEvent(aEvent);
}

void Scene::keyPressEvent(QKeyEvent *aEvent)
{
    switch (aEvent->key() )
    {
    case Qt::Key_Delete:
        for (auto& iT : selectedItems())
        {
            removeItem(iT); // remove selected objects from the scene.
        }
        break;
    case Qt::Key_Control:
        if (selectedItems().size()==1) // Set edit mode if left control pressed
        {
            setMode(Mode::Edit);
        }
        break;

    }

    QGraphicsScene::keyPressEvent(aEvent);
}

void Scene::keyReleaseEvent(QKeyEvent *aEvent)
{
    switch (aEvent->key())
    {
    case Qt::Key_Control:
        setMode(Mode::SelectObject); //Set back to selection mode after Control button released.
        break;
    default:
        break;
    }

    QGraphicsScene::keyReleaseEvent(aEvent);
}

void Scene::drawSceneLine(QGraphicsSceneMouseEvent *aEvent)
{
    // Draw a line
    if (!m_itemToDraw)
    {
        m_itemToDraw = new QGraphicsLineItem;
        addItem(m_itemToDraw);
        m_itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
        m_itemToDraw->setPos(m_origPoint);
    }
    m_itemToDraw->setLine(0, 0,
        aEvent->scenePos().x() - m_origPoint.x(),
        aEvent->scenePos().y() - m_origPoint.y());
}

void Scene::drawTrapezoid(QRectF *trapP){

    QRectF r = *trapP;

    QPolygonF f;
    f.append(QPointF(r.left() + r.width() / 6, r.top()));
    f.append(QPointF(r.right() - r.width() / 6, r.top()));
    f.append(QPointF(r.right(), r.bottom()));
    f.append(QPointF(r.left(), r.bottom()));

    QGraphicsPolygonItem *a = addPolygon(f, QPen(Qt::black, 3, Qt::SolidLine)); // mark as trapezoid
    a->setData(DATA_SHAPETYPE, SHAPE_TRAPEZOID);
    a->setToolTip(className);
}

void Scene::drawStoredTrapezoid(QPolygonF *polygonP){

    QGraphicsPolygonItem *a = addPolygon(*polygonP, QPen(Qt::black, 3, Qt::SolidLine)); // mark as trapezoid
    a->setData(DATA_SHAPETYPE, SHAPE_TRAPEZOID);
    a->setToolTip(className);
}

void Scene::addTrapezoid(QGraphicsSceneMouseEvent* aEvent)
{
    // Add  trapezoid
    QRectF r = selectionArea().boundingRect();
    drawTrapezoid(&r);

}

void Scene::setupShapes(QList<double> *pList, QString *m_Object,int type){

    className = *m_Object;
    QRectF a;
    QPolygonF r;
    QPolygonF p;

    if(type == SHAPE_RECT){
        a.setX(pList->at(0));
        a.setY(pList->at(1));
        a.setWidth(pList->at(2));
        a.setHeight(pList->at(3));
        drawRectangle(&a);

    }else if(type == SHAPE_TRAPEZOID){
        r.append(QPointF(pList->at(0),pList->at(1)));
        r.append(QPointF(pList->at(2),pList->at(3)));
        r.append(QPointF(pList->at(4),pList->at(5)));
        r.append(QPointF(pList->at(6),pList->at(7)));
        drawStoredTrapezoid(&r);

    }else if(type == SHAPE_POLYGON){
        for(int i = 0; i < pList->size()/2; i++){
            //qDebug() << "i = " << i << i+i << i+i+1;
            p.append(QPointF(pList->at(i+i), pList->at(i+i+1)));
        }
        drawPolygon(&p);
    }

}

void Scene::drawRectangle(QRectF *rectP){

    QGraphicsRectItem *a = addRect(*rectP, QPen(Qt::black, 3, Qt::SolidLine)); // mark as rectangle
    a->setData(DATA_SHAPETYPE, SHAPE_RECT);
    a->setToolTip(className);
}

void Scene::addRectangle(QGraphicsSceneMouseEvent* aEvent)
{
    // Add rectangle
    QRectF r = selectionArea().boundingRect();
    drawRectangle(&r);

}

void Scene::setClassName(const QString &aClassName){
    className = aClassName;
}

void Scene::drawPolygon(QPolygonF *polyP){

    m_CurrentPolygon = addPolygon(*polyP, QPen(Qt::black, 3, Qt::SolidLine));
    m_CurrentPolygon->setData(DATA_SHAPETYPE, SHAPE_POLYGON); // mark as polygon
    m_CurrentPolygon->setToolTip(className);
}

void Scene::addPolygonPoint(QGraphicsSceneMouseEvent *aEvent)
{
    //remove the selected polygon if exists, and add a new poligon with this new point.
    QPolygonF f;

    if (m_CurrentPolygon)
    {
        f = m_CurrentPolygon->polygon();
        removeItem(m_CurrentPolygon);
    }


    f.append(aEvent->scenePos());
    drawPolygon(&f);

}

void Scene::setSelectable(bool aSelectable)
{

    for(int i = 0; i < items().size()-1; i++){
        items()[i]->setFlag(QGraphicsItem::ItemIsSelectable, aSelectable);
        items()[i]->setFlag(QGraphicsItem::ItemIsMovable, aSelectable);
    }
}

void Scene::editRectangle(QGraphicsSceneMouseEvent *aEvent)
{
    //resize rectangle with the selected
    QGraphicsRectItem *ri = static_cast<QGraphicsRectItem*>(selectedItems()[0]);

    QPointF mouse = ri->mapFromScene(aEvent->scenePos());

    QRectF rect = ri->rect();
    QRectF r2 = rect;

    // find the selected corner.
    r2.setBottomLeft(mouse);

    float min = sqrt(pow(rect.bottomLeft().x() - mouse.x(), 2) + pow(rect.bottomLeft().y() - mouse.y(), 2));
    float tmp = sqrt(pow(rect.bottomRight().x() - mouse.x(), 2) + pow(rect.bottomRight().y() - mouse.y(), 2));
    if (tmp < min)
    {
        tmp = min;
        r2 = rect;
        r2.setBottomRight(mouse);
    }

    tmp = sqrt(pow(rect.topLeft().x() - mouse.x(), 2) + pow(rect.topLeft().y() - mouse.y(), 2));
    if (tmp < min)
    {
        tmp = min;
        r2 = rect;
        r2.setTopLeft(mouse);
    }

    tmp = sqrt(pow(rect.topRight().x() - mouse.x(), 2) + pow(rect.topRight().y() - mouse.y(), 2));
    if (tmp < min)
    {
        tmp = min;
        r2 = rect;
        r2.setTopRight(mouse);
    }

    if (r2.width() > 3 && r2.height() > 3) // Line is not allowed.
    {
        ri->setRect(r2);
        ri->update();
    }
}


void Scene::editPolygon(QGraphicsSceneMouseEvent *aEvent, bool aShallConvex)
{
    // move the selected point of the polygon.
    QGraphicsPolygonItem *pi = static_cast<QGraphicsPolygonItem*>(selectedItems()[0]);

    QPolygonF p = pi->polygon();

    QPointF mouse = pi->mapFromScene(aEvent->scenePos());

    float min = std::numeric_limits<float>::max();
    int idx = 0;

    // find the selected point
    for (int i = 0; i < p.size(); i++)
    {
        float dist = sqrt(pow(p[i].x() - mouse.x(), 2) + pow(p[i].y() - mouse.y(), 2));
        if (dist < min)
        {
            min = dist;
            idx = i;
        }
    }

    p[idx] = mouse;

    if (aShallConvex && !polygonIsConvex(p))
        return;

    pi->setPolygon(p);
}

void Scene::editTrapezoid(QGraphicsSceneMouseEvent *aEvent)
{
    //Trapeziod shall convex, so can be a triangle. Trapezoid is a polygon.
    editPolygon(aEvent, true);
}

void Scene::rotateRectangle(QGraphicsSceneMouseEvent *aEvent)
{
    // rotate rectangle with the mouse cursor.
    QRectF rect;
    foreach(QGraphicsItem *item, selectedItems()) // only one item can be selected.
    {
        rect |= item->mapToScene(item->boundingRect()).boundingRect();
    }
    QPointF center = rect.center(); // calcuate the rotation angle.

    float delta_x = aEvent->scenePos().x() - center.x();
    float delta_y = aEvent->scenePos().y() - center.y();
    float theta_radians = atan2(delta_y, delta_x);

    QTransform t;
    t.translate(center.x(), center.y());

    foreach(QGraphicsItem *item, selectedItems())
    {
        t.rotate(theta_radians * 180 / M_PI - item->rotation()); // rotate rectange with the calculated andge.
        t.translate(-center.x(), -center.y());

        item->setPos(t.map(item->pos())); // map to the scene.
        //item->setRotation(item->rotation() + 10);
        item->setRotation((theta_radians * 180 / M_PI));
    }
}

void Scene::rotateTrapezoid(QGraphicsSceneMouseEvent* aEvent)
{
    //rotate trapezoid is similar to rotating rectangle.
    rotateRectangle(aEvent);
}

//// Return true if the polygon is convex.
bool Scene::polygonIsConvex(QPolygonF const& p)
{
     /*For each set of three adjacent points A, B, C,
     find the cross product AB � BC. If the sign of
     all the cross products is the same, the angles
     are all positive or negative (depending on the
     order in which we visit them) so the polygon
     is convex.*/
    bool got_negative = false;
    bool got_positive = false;
    int num_points = p.size();
    int B, C;
    for (int A = 0; A < num_points; A++)
    {
        B = (A + 1) % num_points;
        C = (B + 1) % num_points;

        float cross_product = crossProductLength(	p[A].x(), p[A].y(), p[B].x(), p[B].y(),	p[C].x(), p[C].y());
        if (cross_product < 0)
        {
            got_negative = true;
        }
        else if (cross_product > 0)
        {
            got_positive = true;
        }
        if (got_negative && got_positive)
            return false;
    }

    //If we got this far, the polygon is convex.
    return true;
}

 /*Return the cross product AB x BC.
 The cross product is a vector perpendicular to AB
 and BC having length |AB| * |BC| * Sin(theta) and
 with direction given by the right-hand rule.
 For two vectors in the X-Y plane, the result is a
 vector with X and Y components 0 so the Z component
 gives the vector's length and direction.*/

float Scene::crossProductLength(float Ax, float Ay, float Bx, float By, float Cx, float Cy)
{
    // Get the vectors' coordinates.
    float BAx = Ax - Bx;
    float BAy = Ay - By;
    float BCx = Cx - Bx;
    float BCy = Cy - By;

    // Calculate the Z coordinate of the cross product.
    return (BAx * BCy - BAy * BCx);
}
