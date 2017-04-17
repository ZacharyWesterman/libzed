/**
 * File:            dynamicStack.h
 * Namespace:       z::core
 * Description:     Template class for dynamic stack.
 *
 * Methods:         push()      : adds item to the top of the stack
 *                  pop()       : gets item from the top of the stack
 *                  peek()      : returns a copy of the top item
 *                  dump()      : empties the stack
 *                  isEmpty()   : returns whether the stack is empty
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef DYNAMICSTACK_H_INCLUDED
#define DYNAMICSTACK_H_INCLUDED


#ifndef null
    #define null 0
#endif // null

namespace z
{
    namespace core
    {
        //template for dynamic stack
        template <typename T>
        class dynamic_stack
        {
        private:
            struct ll_node
            {
                T data;
                ll_node* next;
            };

            ll_node* head;

        public:
            //constructor
            dynamic_stack() {head = null;}

            //destructor
            ~dynamic_stack()
            {
                while (head != null)
                {
                    ll_node* nodePtr = head->next;
                    delete head;
                    head = nodePtr;
                }
            }

            //push an object to the top of the stack.
            void push(T data)
            {
                ll_node* newNode = new ll_node;

                newNode->data = data;
                newNode->next = head;

                head = newNode;
            }

            //pop an object from the top of the stack.
            //returns whether pop was successful.
            bool pop(T& data)
            {
                if (head == null)
                {
                    return false;
                }
                else
                {
                    data = head->data;

                    ll_node* nodePtr = head->next;
                    delete head;
                    head = nodePtr;

                    return true;
                }
            }

            T peek()
            {
                return head->data;
            }

            //return whether the stack is empty.
            bool isEmpty() {return (head == null);}


            void dump()
            {
                while (head != null)
                {
                    ll_node* nodePtr = head->next;
                    delete head;
                    head = nodePtr;
                }
            }
        };
    }
}

#endif // DYNAMICSTACK_H_INCLUDED
