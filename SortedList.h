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
        int size;
    public:
        ////    constructor
        SortedList(): head(nullptr), size(0){}

        ////    copy constructor
        SortedList(const SortedList& to_copy);

        ////    operator=
        SortedList& operator=(const SortedList& to_copy);

        ////    destructor
        ~SortedList();

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

