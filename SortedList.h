#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    template <typename T>
    class Node{
    public:
        T value;
        Node* next;
        Node(const T& value): value(value), next(nullptr){}
        ~Node() = default;
    };

    template <typename T>
    class SortedList {
    private:
//        struct Node{
//            T value;
//            Node* next;
//            Node(const T& value): value(value), next(nullptr){}
//        };

        Node<T>* head;
        ////    Added the tail so it makes it easier to iterate and add items to the sorted_list
        Node<T>* tail;
        int size;
    public:
        ////    constructor
        SortedList(): head(nullptr), tail(nullptr), size(0){}

        ////    copy constructor
        SortedList(const SortedList& to_copy): head(nullptr), tail(nullptr), size(to_copy.size){
            ////    check if the size is zero, nothing to copy
            if(to_copy.size == 0){
                return;
            }
            const Node<T>* runner = to_copy.head;
            ////    try to copy the first node so we can continue with the rest
            try {
                this->head = new Node<T>(runner->value);
            }catch (const std::bad_alloc& e){
                ////    if the allocation failed
                throw e;
            }
            ////    move the runner to the second node(if exists) so we can continue the copying
            runner = runner->next;
            ////    place the tail to the current node
            ////    At this stage only one is created, so the tail is the same as the head
            this->tail = this->head;
            while (runner)
            {
                try {
                    ////    create a new node and assign it to the next of the current tail
                    this->tail->next = new Node<T>(runner->value);
                    ////    move the tail to the newly allocated node
                    this->tail = this->tail->next;
                    ////    move the runner to the next node to_copy
                    runner = runner->next;
                }catch (const std::bad_alloc& e)
                {
                    ////    reaches this code if one of the allocations failed
                    ////    so we need to delete everything we already copied so far
                    ////    as we did before delete the current list with this loop
                    ////    assign a new pointer to_delete which points to the current node we are going to delete
                    Node<T>* to_delete = this->head;
                    ////    iterate over the current sorted_list with runner
                    Node<T>* runner = this->head;
                    while(runner)
                    {
                        ////    place the to_delete to point to the current node
                        to_delete = runner;
                        ////    move the runner to the next node
                        runner = runner->next;
                        ////    delete the current node
                        delete to_delete;
                    }
                    size = 0;
                    throw e;
                }
            }
        }

        ////    operator=
        SortedList& operator=(const SortedList& to_copy);

        ////    destructor
        ~SortedList(){
            ////    assign a new pointer to_delete which points to the current node we are going to delete
            Node<T>* to_delete = this->head;
            ////    iterate over the current sorted_list with runner
            Node<T>* runner = this->head;
            while (runner){
                ////    place the to_delete to point to the current node
                to_delete = runner;
                ////    move the runner to the next node
                runner = runner->next;
                ////    delete the current node
                delete to_delete;
            }
        }

        ////    constIterator
        class ConstIterator;

        ////    begin method
        ConstIterator begin(){
            return ConstIterator(1, this);
        }

        ////    end method
        ConstIterator end(){
            return ConstIterator(this->size + 1, this);
        }

        ////////    methods for sorted_list
        void insert(const T& to_add);

        void remove(ConstIterator& to_delete);

        int length(){
            return this->size;
        }

        SortedList filter(bool (*predicate)(const T&))const;

        SortedList apply(T (*operation) (const T&))const;

        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. SortedList() - creates an empty list.
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~SortedList() - destructor
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method
         * 7. end method
         *
         * functions:
         * 8. insert - inserts a new element to the list
         * 9. remove - removes an element from the list
         * 10. length - returns the number of elements in the list
         * 11. filter - returns a new list with elements that satisfy a given condition
         * 12. apply - returns a new list with elements that were modified by an operation
         */

    };

    template <class T>
    class SortedList<T>::ConstIterator {
    private:
        int index;
        const SortedList<T>* list;
        friend class SortedList<T>;
        ConstIterator(int index, const SortedList<T>* list): index(index), list(list){}

    public:
        ~ConstIterator() = default;
        ConstIterator(const ConstIterator& iterator) = default;
        ConstIterator& operator=(const ConstIterator& iterator) = default;

        const T& operator*()const{
            if(this->index > this->list->size){
                throw std::out_of_range("Out of range");
            }
            Node<T>* runner = this->list->head;
            int i = 1;
            while(i < this->index){
                runner = runner->next;
                ++i;
            }
            return runner->value;
        }

        bool operator!=(const SortedList<T>::ConstIterator& iterator)const{
            return (this->index != iterator.index) || (this->list != iterator.list);
        }

        const ConstIterator& operator++(){
            if(this->index > this->list->size){
                throw std::out_of_range("Out of range");
            }
            ++this->index;
            return *this;
        }

        ////    not needed
//        const ConstIterator operator++(int i){
//            if(this->index > this->list->size)
//            {
//                throw std::out_of_range("Out of range");
//            }
//            const ConstIterator result = *this;
//            ++(*this);
//            return result;
//        }

    /**
     * the class should support the following public interface:
     * if needed, use =defualt / =delete
     *
     * constructors and destructor:
     * 1. a ctor(or ctors) your implementation needs
     * 2. copy constructor
     * 3. operator= - assignment operator
     * 4. ~ConstIterator() - destructor
     *
     * operators:
     * 5. operator* - returns the element the iterator points to
     * 6. operator++ - advances the iterator to the next element
     * 7. operator!= - returns true if the iterator points to a different element
     *
     */
    };
}

