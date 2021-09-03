#ifndef SCENE_H
#define SCENE_H

#include <QtWidgets/QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QKeyEvent>

/*!
 * \brief The Scene class inherits from QGraphicsScene which is used for displaying the images and shapes
 */
class Scene: public QGraphicsScene{
public:
    /*!
     * \brief The Mode enum stores all the possible options for annotating images
     */
    enum class Mode { NoMode, SelectObject, DrawLine, DrawRectangle, RotateRectangle, DrawTrapezoid, Edit, DrawPoligon };

    /*!
     * \brief Scene constructor takes other objects as it's parent (when parent is delete QGraphicsScene is also delete)
     * \param parent is the parent object pointer
     */
    Scene(QObject *parent = nullptr);
    /*!
      *\brief ~Scene destructor deallocate dynamically allocated memory
      */
    ~Scene();
    /*!
     * \brief setMode method sets the mode as the name suggests
     * \param aMode is is an object of type Mode enum
     */
    void setMode(Mode aMode);
    /*!
     * \brief save methods saves the annotated shapes into json file
     * \param aFileName is the file name where the annotated data will be stored
     */
    void save(const QString &aFileName);
    /*!
     * \brief setClassName method sets the class name when the class item is clicked on the class pane widget
     * \param aClassName holds the className
     */
    void setClassName(const QString &aClassName);
    /*!
     * \brief setupShapes method sets the shapes from the json file and make ready to be displayed on the image
     * \param pList is a list the rectangle, trapezoid or polygon coordinates
     * \param m_Object is the annotated object name on the image
     * \param type is the shape type
     */
    void setupShapes(QList<double> *pList,QString *m_Object ,int type);

protected:
    /*!
     * \brief mousePressEvent method delects the mouse press event on the scene and stores the origin point coordinates into a variable
     * \param aEvent is pointer to the mouse event
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief mouseMoveEvent method detects the mouse move event which then calls the respective add"ShapeName" method to draw the respective shape
     * \param aEvent is pointer to the move mouse event
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief mouseReleaseEvent method determined the end point of the shape when mouse left button is released
     * \param aEvent is pointer to the release mouse event
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief keyPressEvent method gets triggered when a keyboard key is pressed (in our case it's used for control and delete buttons)
     * \param event is pointer to the keyboard key press event
     */
    void keyPressEvent(QKeyEvent *event);
    /*!
     * \brief keyReleaseEvent method gets triggered when the keyboard button is released
     * \param event is a pointer to the keyboard release event
     */
    void keyReleaseEvent(QKeyEvent *event);

private:
    /*!
     * \brief drawSceneLine method is resposible for drawing lines
     * \param aEvent is the mouse event passed from mouse move method, which enables drawing the line
     */
    void drawSceneLine(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief addTrapezoid method is called when drawing trapezoid with mouse
     * \param aEvent is unsed parameter
     */
    void addTrapezoid(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief addRectangle method is triggered when rectanle is is being drawn on the image
     * \param aEvent is unsed parameter
     */
    void addRectangle(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief addPolygonPoint method is triggered when drawing polygon the image
     * \param aEvent parameter is used to get the mouse press event
     */
    void addPolygonPoint(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief setSelectable method is used to set a shape selectable
     * \param aSelectable parameter is passed to this method to determine whether the selected item is selectable
     */
    void setSelectable(bool aSelectable);
    /*!
     * \brief editRectangle method gets triggered when resizing the rectangle shape
     * \param aEvent parameter is passed to this method from mouseMoveEvent method to enable resizing the rectangle shape as the move moves
     */
    void editRectangle(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief editPolygon method is used to resize polygon
     * \param aEvent is passed from mouseMoveEvent method to enable resizing the polygon shape
     * \param aShallConvex boolean variable parameter is false for polygon because polygon can be convex or concave and it's true for trapezoid so it can be triangle
     */
    void editPolygon(QGraphicsSceneMouseEvent *aEvent, bool aShallConvex);
    /*!
     * \brief editTrapezoid method is used to resize trapezoid
     * \param aEvent is the mouse move event that's passed from mouseMoveEvent method, which enables resizing the shape
     */
    void editTrapezoid(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief rotateRectangle method is used to rotate both rectangle and trapezoid
     * \param aEvent is mouse move event which enables rotating the shapes
     */
    void rotateRectangle(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief rotateTrapezoid method calls rotateRectangle method since rotating rectangle and trapezoid is the same
     * \param aEvent paramter is recieved from mouseMoveEvent method and then passed to rotateRectangle method
     */
    void rotateTrapezoid(QGraphicsSceneMouseEvent *aEvent);
    /*!
     * \brief drawRectangle method is used to draw rectangle shape automatically when json file data is loaded that contains one or more rectangles
     * \param rectP pointer parameter points to the rectangle coordinates
     */
    void drawRectangle(QRectF *rectP);
    /*!
     * \brief drawTrapezoid method is used for drawing shape with mouse it's called from addTrapezoid method (it does not draw shape automatically using data from json file)
     * \param trapP pointer parameter points to the trapezoid coordinates
     */
    void drawTrapezoid(QRectF *trapP);
    /*!
     * \brief drawStoredTrapezoid method is used to traw trapezoid based on the data from json file (note: drawTrapezoid method is different from drawStoredTrapezoid method)
     * \param polygonP
     */
    void drawStoredTrapezoid(QPolygonF *polygonP);
    /*!
     * \brief drawPolygon method is used to draw polygon shape automatically when json file data is loaded that contains one or more polygons
     * \param polyP is a pointer to polygon coordinates loaded from json file
     */
    void drawPolygon(QPolygonF *polyP);

    /*!
     * \brief crossProductLength method determines whether a polygon or a trapezoid(should be convex) is convex. It takes a set of three adjacent points A, B, C,
     * \return returns the cross product AB x BC
     */
    float crossProductLength(float Ax, float Ay, float Bx, float By, float Cx, float Cy);
    /*!
     * \brief polygonIsConvex returns true if polygon or trapezoid is convex otherwise returns false. Note, trapezoid is technically polygon
     * \param p parameter takes the polygon points object address
     * \return returns true if a shape is convex else returns false
     */
    bool polygonIsConvex(QPolygonF const &p);

private:
    /*!
     * \brief m_Mode is an object of type Mode enum used for determining what action is being taken e.g. drawing line, editing etc
     */
    Mode m_Mode;
    /*!
     * \brief m_origPoint object is used to get the position where mouse is clicked
     */
    QPointF m_origPoint;
    /*!
     * \brief m_itemToDraw object is used to draw line on the scene
     */
    QGraphicsLineItem *m_itemToDraw;
    /*!
     * \brief m_CurrentPolygon object is used to draw polygon on the scene
     */
    QGraphicsPolygonItem *m_CurrentPolygon;
    /*!
     * \brief className string variable is used to be assigned to the class name
     */
    QString className;
};

#endif // SCENE_H
