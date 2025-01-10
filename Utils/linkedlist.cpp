#include "linkedlist.hpp"

template <typename dataType>
Node<dataType>::Node(dataType a_data)
{
    this->data = a_data;
    this->next = nullptr;
}

template <typename dataType>
void Node<dataType>::setData(dataType a_data)
{
    this->data = a_data;
}

template <typename dataType>
void Node<dataType>::setNext(Node* a_next)
{
    this->next = nullptr;
}

template <typename dataType>
dataType Node<dataType>::getData(void)
{
    return this->data;
}

template <typename dataType>
Node<dataType>* Node<dataType>::getNext(void)
{
    return this->next;
}

template <typename dataType>
Linkedlist<dataType>::Linkedlist()
{
    this->head = nullptr;
}

template <typename dataType>
Linkedlist<dataType>::~Linkedlist()
{
    /* de-allocate all the nodes. */
}

template <typename dataType>
void Linkedlist<dataType>::append(dataType a_data)
{
    Node<dataType>* newNode = new Node<dataType>(a_data);
    newNode->next = this->head;
    this->head = newNode;
}

template <typename dataType>
void Linkedlist<dataType>::remove(dataType a_data)
{
    Node<dataType> currentNode = this->head;

    if(currentNode->getData() == a_data)
    {
        this->head = currentNode->getNext();
        delete currentNode;
        return;
    }

    while(currentNode->getNext() != nullptr)
    {
        if(currentNode->getNext()->getData() == a_data)
        {
            Node<dataType>* deletedNode = currentNode->getNext();
            currentNode->setNext(deletedNode->getNext());
            delete deletedNode;
        }

        currentNode = currentNode->getNext();
    }
}
