#pragma once

template <typename dataType>
class Node
{
private:
    dataType data;
    Node* next;
protected:
public:
    Node() = delete;
    Node(dataType a_data);
    ~Node() = default;
    void setData(dataType a_data);
    void setNext(Node* a_next);
    dataType getData(void);
    Node* getNext(void);
};

template <typename dataType>
class Linkedlist
{
private:
    Node<dataType>* head;
protected:
public:
    Linkedlist();
    ~Linkedlist();
    void append(dataType a_data);
    void remove(dataType a_data);
};