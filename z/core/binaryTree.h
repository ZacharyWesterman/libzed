/**
 * File:            binaryTree.h
 * Namespace:
 *
 * Description:     An old bit of code that uses a binary tree
 *                  to store script variables.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   ?
**/

#pragma once
#ifndef BINARYTREE_H_INCLUDED
#define BINARYTREE_H_INCLUDED


//! THIS CODE IS DEPRECATED
//!
//! Use 'z/script/variableTree.h' instead!
//!

//this class defines a binary sorting tree
//whose data is script variables
class VariableTree
{
private:
    struct node
    {
        Variable* data;
        node* left;
        node* right;
    };

    node* root;

    void empty(node*&);

    void insert(node*&, Variable*);

    const str find(node*, const str) const;

    void strVars(node*, str&);

public:
    VariableTree();
    ~VariableTree();

    void clear();

    void set(const str, const str);
    const str get(const str) const;

    const str getString();
};


VariableTree::VariableTree()
{
    root = NULL;
}

//functions to delete all nodes in the tree
VariableTree::~VariableTree()
{
    empty(root);
}

void VariableTree::clear()
{
    empty(root);
}

void VariableTree::empty(node*& nodePtr)
{
    if (nodePtr)
    {
        empty(nodePtr->left);
        empty(nodePtr->right);

        delete nodePtr->data;
        delete nodePtr;

        nodePtr = NULL;
    }
}

//functions to set the appropriate node variable's value
void VariableTree::set(const str varName, const str varValue)
{
    Variable* var = new Variable (varName);
    *var = varValue;

    insert(root, var);
}

void VariableTree::insert(node*& nodePtr, Variable* var)
{
    if (nodePtr)
    {

        int compare = equality(var->name(), nodePtr->data->name());

        if (compare > 0)//var name > node var name
        {
            insert(nodePtr->right, var);
        }
        else if (compare < 0)//var name < node var name
        {
            insert(nodePtr->left, var);
        }
        else//var name == node var name
        {
            delete nodePtr->data;
            nodePtr->data = var;
        }
    }
    else
    {

        nodePtr = new node;

        nodePtr->data = var;

        nodePtr->left = NULL;
        nodePtr->right = NULL;
    }
}

//functions to get the value of a variable in the tree
const str VariableTree::get(const str varName) const
{
    return find(root, varName);
}

const str VariableTree::find(node* nodePtr, const str varName) const
{
    if (nodePtr)
    {
        int compare = equality(varName, nodePtr->data->name());

        if (compare > 0)//var name > node var name
            return find(nodePtr->right, varName);
        else if (compare < 0)//var name < node var name
            return find(nodePtr->left, varName);
        else//var name == node var name
            return nodePtr->data->value();
    }
    else
    {
        return NULL;
    }
}

//functions to print all nodes in the tree
const str VariableTree::getString()
{
    str varString = null;
    strVars(root, varString);

    return const_cast<const str>(varString);
}

void VariableTree::strVars(node* nodePtr, str& String)
{
    if (nodePtr)
    {
        strVars(nodePtr->left, String);

        str midStr = null;
        append(midStr, nodePtr->data->name());
        append(midStr, equ);
        append(midStr, nodePtr->data->value());
        append(midStr, ret);

        append(String, midStr);

        strVars(nodePtr->right, String);
    }
}

#endif // BINARYTREE_H_INCLUDED
