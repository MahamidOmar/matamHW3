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
                    ////    iterate over the current sorted_list with runner2
                    Node<T>* runner2 = this->head;
                    while(runner2)
                    {
                        ////    place the to_delete to point to the current node
                        to_delete = runner2;
                        ////    move the runner to the next node
                        runner2 = runner2->next;
                        ////    delete the current node
                        delete to_delete;
                    }
                    size = 0;
                    throw e;
                }
            }
        }

        ////    operator=
        SortedList& operator=(const SortedList& to_copy){
            ////    check if assigning this to this, nothing to do
            if(this == &to_copy){
                return *this;
            }
            ////    check if assigning to an empty sorted_list
            ////    need to delete all the nodes in the current sorted_list
            if(to_copy.size == 0){
                ////    call the destructor for this so we can delete all the nodes
                this->~SortedList();
                ////    return all the fields to their initial values
                this->size = 0;
                this->head = nullptr;
                this->tail = nullptr;
                return *this;
            }
            ////    copyNodes points to the current node from to_copy sorted_list that we are going to copy
            Node<T>* copyNodes = to_copy.head;
            Node<T>* newHead = nullptr;
            try{
                ////    try to copy the first node so we can continue with the rest
                newHead = new Node<T>(copyNodes->value);
            }catch (const std::bad_alloc& e){
                ////    if the allocation failed
                throw e;
            }
            ////    If reached here the first node is allocated and copied successfully
            ////    assign a new pointer to the new tail
            ////    At this stage only one is created, so the new tail is the same as the new head
            Node<T>* newTail = newHead;
            ////    move the copyNodes to the second node(if exists) so we can continue the copying
            copyNodes = copyNodes->next;
            while (copyNodes)
            {
                try {
                    ////    create a new node and assign it to the next of the new_tail
                    newTail->next = new Node<T>(copyNodes->value);
                    ////    move the new_tail to the newly allocated node
                    newTail = newTail->next;
                    ////    move the copyNodes to the next node to_copy
                    copyNodes = copyNodes->next;
                }
                catch (const std::bad_alloc& e)
                {
                    ////    reaches this code if one of the allocations failed
                    ////    so we need to delete everything we already copied so far
                    ////    as we did before delete the current list with this loop
                    ////    assign a new pointer to_delete which points to the current node we are going to delete

                    while(newHead)
                    {
                        ////    place the to_delete to point to the current node
                        Node<T>* to_delete = newHead;
                        ////    move the newHead to the next node
                        newHead = newHead->next;
                        ////    delete the current node
                        delete to_delete;
                    }
                    this->size = 0;
                    throw e;
                }
            }
            ////    if reached this code this means that all the new allocations finished successfully
            ////    now we need to delete all of the old nodes
            while (this->head)
            {
                ////    place the to_delete to point to the current node
                Node<T>* toDelete = this->head;
                ////    move this->head to the next node
                this->head = this->head->next;
                ////    delete the current node
                delete toDelete;
            }
            ////    update the size field
            this->size = to_copy.size;
            ////    assign the current head to point to the newly created list
            this->head = newHead;
            ////    assign the tail to the newly created tail
            this->tail = newTail;
            return *this;
        }

        ////    destructor
        ~SortedList(){
            ////    iterate over the current sorted_list with runner
            Node<T>* runner = this->head;
            while (runner){
                ////    assign to_delete to point to the current node
                Node<T>* to_delete = runner;
                ////    move the runner to the next node
                runner = runner->next;
                ////    delete the current node
                delete to_delete;
            }
        }

        ////    iterator:
        ////    constIterator
        class ConstIterator;

        ////    begin method
        ConstIterator begin()const{
            ////    create a new constIterator with current_index = 1,
            ////    and assign this to be the list that the constIterator points to
            return ConstIterator(1, this);
        }

        ////    end method
        ConstIterator end()const{
            ////    create a new constIterator with current_index = this(sorted_list)->size + 1,
            ////    and assign this to be the list that the constIterator points to
            return ConstIterator(this->size + 1, this);
        }

        ////////    methods for sorted_list
        void insert(const T& to_add){
            Node<T>* new_node;
            try{
                ////    try to allocate a new node that will include the new value to_add
                new_node = new Node<T>(to_add);
            }catch (const std::bad_alloc& e){
                throw e;
            }
            ////    check if the current sorted_list is empty
            if(this->size == 0){
                ////    assign the head and tail to the new_node
                this->head = new_node;
                this->tail = new_node;
                ////    increase the sorted_list size
                ++this->size;
            }else{
                ////    if the current sorted_list has at least one element
                ////    assign current_node to point to the current node
                Node<T>* current_node = this->head;
                ////    at the end of this loop, current_node will point to the last node that has a value > to_add
                ////    this means that new_node will be current_node->next
                while(current_node->next && current_node->next->value > to_add){
                    current_node = current_node->next;
                }
                ////    now just need to insert new_node right after current_node
                ////    assign the new_node->next to be the next of current node
                new_node->next = current_node->next;
                ////    and finally assign the current_node->next to be new_node
                current_node->next = new_node;
            }
        }

        void remove(const ConstIterator& to_delete){
            ////    check if no elements in the current sorted_list,
            ////    or if the iterator provided points to end(),
            ////    or if to_delete doesn't point to a node in the current list
            ////    nothing to remove in these cases
            if(this->size == 0 || !(to_delete != this->end()) || (to_delete.list != this)){
                return;
            }
            ////    check if to_delete points to the head of the sorted_list
            if(to_delete == this->begin()){
                ////    if only one node, then the tail should point to nullptr
                if(this->size == 1){
                    this->tail = nullptr;
                }
                ////    assign a pointer to the head so we can delete it
                Node<T>* node_to_delete = this->head;
                ////    move head to point to the next (if exists) or to nullptr (otherwise)
                this->head = this->head->next;
                ////    delete the head
                delete node_to_delete;
                ////    decrease the size of the sorted_list
                --this->size;
                return;
            }
            ////    assign a pointer to the current node
            Node<T>* current_node = this->head;
            ////    at the end of the loop current node will be the previous of the node we want to delete
            while(current_node->next && current_node->next != to_delete.current){
                current_node = current_node->next;
            }
            ////    reaches here if the loop didn't find a matching node for to_delete iterator
            if(!(current_node->next)){
                return;
            }
            ////    assign a pointer to the node we want to delete
            Node<T>* node_to_delete = current_node->next;
            ////    move the next of the current_node 2 nodes ahead, or to nullptr if we are deleting the last node
            current_node->next = current_node->next->next;
            ////    deallocate the required node
            delete node_to_delete;
            ////    decrease the size of the sorted_list
            --this->size;
        }

        int length()const{
            ////    return the size of the sorted_list from the size field
            return this->size;
        }

        template <typename Predicate>
        SortedList filter(const Predicate& predicate)const{
            SortedList<T> result;
            for(ConstIterator it = begin() ; it != end() ; ++it){
                if(predicate(*it)){
                    result.insert(*it);
                }
            }
            return result;
        }

        template<typename Operation>
        SortedList apply(const Operation& operation)const{
            SortedList<T> result;
            for(ConstIterator it = begin() ; it != end() ; ++it){
                result.insert(operation(*it));
            }
            return result;
        }

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
        ////    index makes it easier to compare with operator!=
        ////    also helps with iterating in operator* and searching for the correct node
        int index;
        ////    pointer to the current list the iterator is assigned to
        const SortedList<T>* list;
        ////    make the SortedList class a friend, so it can access ConstIterator fields and it's private constructor
        friend class SortedList<T>;
        ////    constructor for building the iterator
        ////    it is private so that no outside users can create iterators with it
        ////    only the SortedList class can use it :)
        ConstIterator(int index, const SortedList<T>* list): index(index), list(list){}

    public:
        ~ConstIterator() = default;
        ConstIterator(const ConstIterator& iterator) = default;
        ConstIterator& operator=(const ConstIterator& iterator) = default;

        const T& operator*()const{
            ////    check if the current index is out of range, can't return any value
            if(this->index > this->list->size){
                throw std::out_of_range("Out of range");
            }
            ////    assign runner to iterate over the list, so we can search for the correct node required for operator*
            Node<T>* runner = this->list->head;
            ////    i increases till we reach the current index
            int i = 1;
            ////    The loop keeps moving till the runner points to the current required node
            while(i < this->index){
                runner = runner->next;
                ++i;
            }
            ////    when we reach this line, it means that i = index, so we can return the node->value
            return runner->value;
        }

        bool operator!=(const SortedList<T>::ConstIterator& iterator)const{
            ////    if the index is not correct, or the iterators point to 2 different sorted_list
            ////    return true
            return (this->index != iterator.index) || (this->list != iterator.list);
        }

        const ConstIterator& operator++(){
            ////    check if the current index is above the list size, then the current iterator points to end()
            ////    so we can't use operator++ in this case and we are out of range
            if(this->index > this->list->size){
                throw std::out_of_range("Out of range");
            }
            ////    only update the index
            ++this->index;
            return *this;
        }

        ////    will use this function only when we know that the iterator points to a legal node
        ////    returns the node that the iterator points to.
        Node<T>* current()const{
            int i = 1;
            Node<T>* runner = this->list->head;
            ////    iterate over te sorted_list till we reach the correct index
            while(runner && i < this->index){
                runner = runner->next;
                ++i;
            }
            ////    here i = index, and runner points to the requested node
            return runner;
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

