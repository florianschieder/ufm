#pragma once

#include "../wfmtkdef.h"

template <typename T> class ListNode
{
public:
    METHOD ListNode(T value);

    T __value;
    ListNode* __nextNode;
};

template <typename T> class List
{
public:
    List();
    ~List();
    void free();

    void addItem(T item);
    T getItemAt(size_t index);
    size_t getSize();

    void* (*OnFreeListItem)(T* item);

    ListNode<T>* __firstNode;
    ListNode<T>* __lastNode;

    T operator[](int index);

private:
    size_t __size;
};

//////////////////////////////////////////////////////////////////////////////
// List
//////////////////////////////////////////////////////////////////////////////

template <typename T> List<T>::List()
{
    this->__firstNode = nullptr;
    this->__lastNode = nullptr;

    this->__size = 0;
    this->OnFreeListItem = nullptr;
}

template <typename T> List<T>::~List()
{
    this->free();
}

template <typename T> void List<T>::free()
{
    ListNode<T>* currentItem = this->__firstNode;

    if (currentItem != nullptr)
    {
        while (currentItem->__nextNode != nullptr)
        {
            ListNode<T>* nextItem = currentItem->__nextNode;

            // before freeing up, give the chance to let handles etc. be freed
            // manually (may happen with some kind of Windows handles)

            if (this->OnFreeListItem != nullptr)
            {
                this->OnFreeListItem(&currentItem->__value);
            }

            delete currentItem;

            currentItem = nextItem;
        }

        delete currentItem;
    }
}

template <typename T> void List<T>::addItem(T item)
{
    if (this->__firstNode == nullptr && this->__size == 0)
    {
        ListNode<T>* newFirstNode = new ListNode<T>(item);
        this->__firstNode = newFirstNode;
        this->__firstNode->__nextNode = this->__lastNode;
        this->__lastNode = this->__firstNode;
    }
    else
    {
        ListNode<T>* newNode = new ListNode<T>(item);
        this->__lastNode->__nextNode = newNode;
        this->__lastNode = this->__lastNode->__nextNode;
    }

    __size++;
}

template <typename T> T List<T>::getItemAt(size_t index)
{
    ListNode<T>* currentNode = this->__firstNode;

    for (size_t i = 0; i < index; i++)
    {
        if (currentNode->__nextNode != nullptr)
        {
            currentNode = currentNode->__nextNode;
        }
        else
        {
            throw std::exception("Index out of range");
        }
    }

    return currentNode->__value;
}

template <typename T> size_t List<T>::getSize()
{
    return this->__size;
}

template<typename T> inline T List<T>::operator[](int index)
{
    return this->getItemAt(index);
}

//////////////////////////////////////////////////////////////////////////////
// ListNode
//////////////////////////////////////////////////////////////////////////////

template <typename T> ListNode<T>::ListNode(T value)
{
    this->__value = value;
    this->__nextNode = nullptr;
}
