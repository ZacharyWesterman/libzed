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
        /**
         * \brief Dynamic stack class template
         *
         * This stack uses a singly-linked list to
         * dynamically lengthen and shorten the stack
         * in a "first in, last out" order.
         * While the stack has at least one item on it,
         * only the top item is accessible.
         */
        template <typename T>
        class dynamicStack
        {
        private:
            struct ll_node
            {
                T data;
                ll_node* next;
            };

            ll_node* head;

        public:
            dynamicStack();
            ~dynamicStack();

            void push(T data);

            bool pop(T& data);

            inline T& peek();

            inline bool isEmpty();

            void dump();
        };

        ///Constructor
        template <typename T>
        dynamicStack<T>::dynamicStack()
        {
            head = null;
        }

        ///Destructor
        template <typename T>
        dynamicStack<T>::~dynamicStack()
        {
            while (head != null)
            {
                ll_node* nodePtr = head->next;
                delete head;
                head = nodePtr;
            }
        }

        /**
         * \brief Push an object onto the top of the stack.
         *
         * \param data the object to push onto the stack.
         */
        template <typename T>
        void dynamicStack<T>::push(T data)
        {
            ll_node* newNode = new ll_node;

            newNode->data = data;
            newNode->next = head;

            head = newNode;
        }

        /**
         * \brief Pop an object from the top of the stack.
         *
         * \param data the object to pop from the stack.
         *
         * \return \b True if the pop was successful.
         * \b False if there are no items on the stack.
         */
        template <typename T>
        bool dynamicStack<T>::pop(T& data)
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

        /**
         * \brief Look at the item on the top of the stack.
         *
         * \return A reference to the top item on the stack.
         */
        template <typename T>
        inline T& dynamicStack<T>::peek()
        {
            return head->data;
        }

        /**
         * \brief Check whether the stack is empty.
         *
         * \return \b True if the stack contains no items.
         * \b False otherwise.
         */
        template <typename T>
        inline bool dynamicStack<T>::isEmpty()
        {
            return (head == null);
        }

        /**
         * \brief Empty the stack.
         *
         * Gets rid of all elements on the stack, resetting it.
         */
        template <typename T>
        void dynamicStack<T>::dump()
        {
            while (head != null)
            {
                ll_node* nodePtr = head->next;
                delete head;
                head = nodePtr;
            }
        }
    }
}

#endif // DYNAMICSTACK_H_INCLUDED
