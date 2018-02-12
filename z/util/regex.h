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
            };

            struct node
            {
                const Int type;
                const Int count;
                core::array<node*> children;
            };

            struct symbol
            {
                Int type;
                CHAR value;
                symbol(Int Type, CHAR Value = 0) : type(Type), value(Value) {}

                bool operator==(const symbol& other) const
                {
                    return (type == other.type) && (value == other.value);
                }
            };

            Int search_pos;
            Int search_len;

            node* root;

            node* newNode(Int, Int);

            void readSymbols(const core::string<CHAR>&, core::array<symbol>*);

            void printSymbols(const core::array<symbol>& symbols)
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
                        String = "ENDLINE";
                        break;
                    case REGEX_NONE:
                        String = "NONE";
                        break;
                    default:
                        String = cs("UNK : ") + cs(symbols[i].type);
                    }

                    console.write(String);
                    console.write('\n');
                }
            }

        public:
            regex(const core::string<CHAR>&);

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
        void regex<CHAR>::readSymbols(const core::string<CHAR>& expr,
                                      core::array<symbol>* symbols)
        {
            Int i = 0;
            while (i < expr.length())
            {
                if (expr[i] == '[')
                {
                    symbols->add(symbol(REGEX_START_OR));
                }
                else if (expr[i] == ']')
                {
                    symbols->add(symbol(REGEX_STOP_OR));
                }
                else if (expr[i] == ')')
                {
                    symbols->add(symbol(REGEX_STOP_GROUP));
                }
                else if (expr[i] == '.')
                {
                    symbols->add(symbol(REGEX_ANYTHING));
                }
                else if (expr[i] == '*')
                {
                    symbols->add(symbol(REGEX_0_OR_MORE));
                }
                else if (expr[i] == '+')
                {
                    symbols->add(symbol(REGEX_1_OR_MORE));
                }
                else if (expr[i] == '$')
                {
                    symbols->add(symbol(REGEX_ENDLINE));
                }
                else if (expr.foundAt("(?i)", i))
                {
                    symbols->add(symbol(REGEX_START_CASE_I));
                    i+=3;
                }
                else if (expr.foundAt("(?!i)", i))
                {
                    symbols->add(symbol(REGEX_STOP_CASE_I));
                    i+=4;
                }
                else if (expr[i] == '?')
                {
                    symbols->add(symbol(REGEX_0_OR_1));
                }
                else if (expr[i] == '(')
                {
                    symbols->add(symbol(REGEX_START_GROUP));
                }
                else if (expr.foundAt("a-z", i))
                {
                    symbols->add(REGEX_RANGE_az);
                    i+=2;
                }
                else if (expr.foundAt("A-Z", i))
                {
                    symbols->add(REGEX_RANGE_AZ);
                    i+=2;
                }
                else if (expr.foundAt("0-9", i))
                {
                    symbols->add(REGEX_RANGE_09);
                    i+=2;
                }
                else if (expr.foundAt("\\w", i))
                {
                    symbols->add(symbol(REGEX_WHITESPACE));
                }
                else
                {
                    if ((expr[i] == '\\') && (i+1 < expr.length()))
                        i++;

                    symbols->add(symbol(REGEX_SYMBOL, expr[i]));
                }

                i++;
            }
        }

        template <typename CHAR>
        regex<CHAR>::regex(const core::string<CHAR>& expr)
        {
            search_pos = -1;
            search_len = 0;
            root = NULL;

            core::array<symbol> symbols;

            readSymbols(expr, &symbols);

            printSymbols(symbols);
        }
    }
}

#endif // REGEX_H_INCLUDED
