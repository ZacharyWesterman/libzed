#pragma once
#ifndef REGEX_H_INCLUDED
#define REGEX_H_INCLUDED

#include <z/int.h>
#include <z/core/timeout.h>
#include <z/core/stream.h>
#include <z/core/array.h>

#ifndef NULL
    #define NULL 0
#endif // NULL

#include <z/system/console.h>

namespace z
{
    namespace util
    {
        template <typename CHAR>
        class regex
        {
        private:
            enum
            {
                REGEX_NONE = 0,
                REGEX_RANGE_AZ,
                REGEX_RANGE_az,
                REGEX_RANGE_09,

                REGEX_SYMBOL,

                REGEX_START_CASE_I,
                REGEX_STOP_CASE_I,

                REGEX_WHITESPACE,
                REGEX_ANYTHING,
                REGEX_OR,

                REGEX_START_GROUP,
                REGEX_STOP_GROUP,
                REGEX_START_OR,
                REGEX_STOP_OR,

                REGEX_0_OR_MORE,
                REGEX_1_OR_MORE,
                REGEX_0_OR_1,

                REGEX_ENDLINE,
                REGEX_NEWLINE,
            };

            struct regexSymbol
            {
                Int type;
                CHAR value;

                regexSymbol(Int Type, CHAR Value = 0) : type(Type), value(Value) {}

                regexSymbol& operator=(const regexSymbol& other)
                {
                    type = other.type;
                    value = other.value;
                }

                bool operator==(const regexSymbol& other) const
                {
                    return (type == other.type) && (value == other.value);
                }
            };

            struct node
            {
                const regexSymbol symbol;

                const Int amount;

                core::array<node*> children;
                node* parent;
            };



            Int search_pos;
            Int search_len;

            node* root;

            node* newNode(Int, Int);

            void deleteTree(node*);

            void readSymbols(const core::string<CHAR>&, core::array<regexSymbol>*);

            void printSymbols(const core::array<regexSymbol>& symbols)
            {
                system::console console;

                for (Int i=0; i<symbols.size(); i++)
                {
                    core::string<char> String;

                    switch (symbols[i].type)
                    {
                    case REGEX_RANGE_AZ:
                        String = "RANGE : A-Z";
                        break;
                    case REGEX_RANGE_az:
                        String = "RANGE : a-z";
                        break;
                    case REGEX_RANGE_09:
                        String = "RANGE : 0-9";
                        break;
                    case REGEX_SYMBOL:
                        String = cs("SYMBOL : ") + cs(symbols[i].value);
                        break;
                    case REGEX_START_CASE_I:
                        String = "START CASE INSENSITIVE";
                        break;
                    case REGEX_STOP_CASE_I:
                        String = "STOP CASE INSENSITIVE";
                        break;
                    case REGEX_WHITESPACE:
                        String = "WHITE SPACE";
                        break;
                    case REGEX_ANYTHING:
                        String = "ANY CHARACTER";
                        break;
                    case REGEX_OR:
                        String = "OR";
                        break;
                    case REGEX_START_GROUP:
                        String = "START GROUP";
                        break;
                    case REGEX_STOP_GROUP:
                        String = "STOP GROUP";
                        break;
                    case REGEX_START_OR:
                        String = "START OR GROUP";
                        break;
                    case REGEX_STOP_OR:
                        String = "STOP OR GROUP";
                        break;
                    case REGEX_0_OR_MORE:
                        String = "0 OR MORE";
                        break;
                    case REGEX_1_OR_MORE:
                        String = "1 OR MORE";
                        break;
                    case REGEX_0_OR_1:
                        String = "0 OR 1";
                        break;
                    case REGEX_ENDLINE:
                        String = "END LINE";
                        break;
                    case REGEX_NONE:
                        String = "NONE";
                        break;
                    case REGEX_NEWLINE:
                        String = "NEW LINE";
                        break;
                    default:
                        String = cs("UNK : ") + cs(symbols[i].type);
                    }

                    console.write(String);
                    console.write('\n');
                }
            }

            void createTreeToRoot(const core::array<regexSymbol>&);

        public:
            regex(const core::string<CHAR>&);
            ~regex() {deleteTree(root);}

            bool match(const core::string<CHAR>&);

            bool search(const core::string<CHAR>&, const core::timeout& time = -1);

            bool searchFound();
            Int searchPos();
            Int searchLen();
        };

        template <typename CHAR>
        typename regex<CHAR>::node* regex<CHAR>::newNode(Int type, Int count)
        {
            node* thisNode = new node;
            thisNode->type = type;
            thisNode->count = count;

            if (root == NULL)
                root = thisNode;

            return thisNode;
        }

        template <typename CHAR>
        void regex<CHAR>::deleteTree(node* aNode)
        {
            if (aNode)
            {
                //for (Int i=0; i<(aNode->children()); i++)
                  //  deleteTree(aNode->children[i]);

                delete aNode;
            }
        }

        template <typename CHAR>
        void regex<CHAR>::readSymbols(const core::string<CHAR>& expr,
                                      core::array<regexSymbol>* symbols)
        {
            Int i = 0;
            while (i < expr.length())
            {
                if (expr[i] == '[')
                {
                    symbols->add(regexSymbol(REGEX_START_OR));
                }
                else if (expr[i] == ']')
                {
                    symbols->add(regexSymbol(REGEX_STOP_OR));
                }
                else if (expr[i] == ')')
                {
                    symbols->add(regexSymbol(REGEX_STOP_GROUP));
                }
                else if (expr[i] == '.')
                {
                    symbols->add(regexSymbol(REGEX_ANYTHING));
                }
                else if (expr[i] == '*')
                {
                    symbols->add(regexSymbol(REGEX_0_OR_MORE));
                }
                else if (expr[i] == '+')
                {
                    symbols->add(regexSymbol(REGEX_1_OR_MORE));
                }
                else if (expr[i] == '$')
                {
                    symbols->add(regexSymbol(REGEX_ENDLINE));
                }
                else if (expr[i] == '^')
                {
                    symbols->add(regexSymbol(REGEX_NEWLINE));
                }
                else if (expr.foundAt("(?i)", i))
                {
                    symbols->add(regexSymbol(REGEX_START_CASE_I));
                    i+=3;
                }
                else if (expr.foundAt("(?!i)", i))
                {
                    symbols->add(regexSymbol(REGEX_STOP_CASE_I));
                    i+=4;
                }
                else if (expr[i] == '?')
                {
                    symbols->add(regexSymbol(REGEX_0_OR_1));
                }
                else if (expr[i] == '(')
                {
                    symbols->add(regexSymbol(REGEX_START_GROUP));
                }
                else if (expr.foundAt("a-z", i))
                {
                    symbols->add(regexSymbol(REGEX_RANGE_az));
                    i+=2;
                }
                else if (expr.foundAt("A-Z", i))
                {
                    symbols->add(regexSymbol(REGEX_RANGE_AZ));
                    i+=2;
                }
                else if (expr.foundAt("0-9", i))
                {
                    symbols->add(regexSymbol(REGEX_RANGE_09));
                    i+=2;
                }
                else if (expr.foundAt("\\w", i))
                {
                    symbols->add(regexSymbol(REGEX_WHITESPACE));
                    i++;
                }
                else
                {
                    if ((expr[i] == '\\') && (i+1 < expr.length()))
                        i++;

                    symbols->add(regexSymbol(REGEX_SYMBOL, expr[i]));
                }

                i++;
            }
        }

        template <typename CHAR>
        void regex<CHAR>::createTreeToRoot(const core::array<regexSymbol>& symbols)
        {
            if (root)
                deleteTree(root);

            root = new node;
            root->parent = NULL;


            node* currentNode = root;

            for (Int i=0; i<symbols.size(); i++)
            {
                if ((symbols[i].type == REGEX_START_GROUP) ||
                    (symbols[i].type == REGEX_START_OR) ||
                    (symbols[i].type == REGEX_START_CASE_I)
                    )
                {
                    node* tmp = new node;
                    tmp->value = symbols[i];
                    tmp->parent = currentNode;
                    currentNode->children.add(tmp);

                    currentNode->children.add(new regexSymbol(symbols[i]));
                    currentNode = &(currentNode->children[currentNode->children.size()-1]);
                }
            }
        }

        template <typename CHAR>
        regex<CHAR>::regex(const core::string<CHAR>& expr)
        {
            search_pos = -1;
            search_len = 0;
            root = NULL;

            core::array<regexSymbol> symbols;

            readSymbols(expr, &symbols);

            printSymbols(symbols);

            //createTreeToRoot(symbols);
        }
    }
}

#endif // REGEX_H_INCLUDED
