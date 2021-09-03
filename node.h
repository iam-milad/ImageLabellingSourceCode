#ifndef NODE_H
#define NODE_H


template <typename T>
class LinkedList;

/*!
 * \brief The node class is a blue-print for nodes of linkedlist
 */
template <typename T>
class node{
private:
    friend class LinkedList<T>;
    /*!
     * \brief data hold the image or class object since it's of generic type T
     */
    T data;
    /*!
     * \brief next is pointer to the next node in the linkedlist
     */
    node *next;
};

#endif // NODE_H
