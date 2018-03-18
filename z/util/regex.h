#pragma once
#ifndef REGEX_H_INCLUDED
#define REGEX_H_INCLUDED

#include <z/int.h>
#include <z/core/timeout.h>
#include <z/core/stream.h>
#include <z/core/array.h>
#include <z/math/value2d.h>

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

                REGEX_SYMBOL,

                REGEX_RANGE_AZ,
                REGEX_RANGE_az,
                REGEX_RANGE_09,

                REGEX_GROUP_AND,
                REGEX_GROUP_OR,

                REGEX_END_INPUT,
                REGEX_BEGIN_INPUT,

                REGEX_WHITESPACE,
                REGEX_ANYTHING,

                REGEX_OR,

                REGEX_START_AND,
                REGEX_START_OR,
                
                REGEX_STOP_AND,
                REGEX_STOP_OR,

                REGEX_START_CASE_I,
                REGEX_STOP_CASE_I,

                REGEX_AMOUNT,

                REGEX_LOOK_AHEAD,
                REGEX_LOOK_BEHIND,

                REGEX_NOT,

                REGEX_ENUM_TYPE_CT
            };

            struct regexSymbol
            {
                Int type;
                CHAR value;

                Int min;
                Int max;

                regexSymbol(Int Type, CHAR Value = 0, Int _max = 1, Int _min = 1) : 
                    type(Type), value(Value), min(_min), max(_max)
                {}

                regexSymbol(const regexSymbol& other)
                {
                   type = other.type;
                   value = other.value;
                   min = other.min;
                   max = other.max;
                }

                const regexSymbol& operator=(const regexSymbol& other)
                {
                   type = other.type;
                   value = other.value;
                   min = other.min;
                   max = other.max;
                }

                bool operator==(const regexSymbol& other) const
                {
                    return (type == other.type) && (value == other.value);
                }
            };

            struct node
            {
                bool negate;
                regexSymbol symbol;

                core::array<node*> children;
                node* parent;


                node(const regexSymbol& _symbol, bool _negate = false) : 
                    symbol(_symbol), negate(_negate)
                {
                    parent = NULL;
                }

                ~node()
                {
                    for (Int i=0; i<children.size(); i++)
                        delete children[i];
                }

                void addChild(node*& child)
                {
                    if (child)
                    {
                        child->parent = this;
                        children.add(child);
                    }
                }
            };


            Int regex_error;

            math::value2d<Int> searchVal;
            bool lastSearchDone;

            node* root;

            void readSymbols(const core::string<CHAR>&, core::array<regexSymbol>*);

            const core::string<char> symbolString(const regexSymbol& symbol, bool negate)
            {
                core::string<char> String;

                switch (symbol.type)
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
                    String = cs("SYMBOL : ") + cs(symbol.value);
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
                case REGEX_NOT:
                    String = "NOT";
                    break;
                case REGEX_START_AND:
                    String = "START AND GROUP";
                    break;
                case REGEX_STOP_AND:
                    String = "STOP AND GROUP";
                    break;
                case REGEX_START_OR:
                    String = "START OR GROUP";
                    break;
                case REGEX_STOP_OR:
                    String = "STOP OR GROUP";
                    break;
                case REGEX_GROUP_OR:
                    String = "OR GROUP";
                    break;
                case REGEX_GROUP_AND:
                    String = "AND GROUP";
                    break;
                case REGEX_AMOUNT:
                    String = "AMOUNT";
                    break;
                case REGEX_END_INPUT:
                    String = "END INPUT";
                    break;
                case REGEX_NONE:
                    String = "NONE";
                    break;
                case REGEX_BEGIN_INPUT:
                    String = "BEGIN INPUT";
                    break;
                default:
                    String = cs("UNK : ") + cs(symbol.type);
                }

                return (negate ? (cs("NOT ")+String) : String);
            }

            void print(core::outputStream<char>& Console, const node* root, Int indent = 0)
            {
                if (root)
                {
                    auto symbol = root->symbol;

                    for (Int i=0; i<indent; i++)
                        Console.write("    ");

                    Console.write(symbolString(symbol, root->negate));
                    Console.write(" | ");
                    
                    if (symbol.min != symbol.max)
                    {
                        Console.write(symbol.min);

                        if (symbol.max < symbol.min)
                            Console.writeln('+');
                        else
                            Console.writeln(cs('-') + cs(symbol.max));
                    }
                    else
                        Console.writeln(symbol.min);

                    for (Int i=0; i<(root->children.size()); i++)
                        this->print(Console, root->children[i], indent+1);
                }
            }

            void createTreeToRoot(core::array<regexSymbol>*);

            void substituteOrInTree(node*&);

            Int matchNode(const node*, core::inputStream<CHAR>&, bool) const;
            Int matchNodeOnce(const node*, core::inputStream<CHAR>&, bool) const;

            void unConsume(core::inputStream<CHAR>&, Int) const;

        public:
            regex(const core::string<CHAR>&);
            ~regex() {if (root) delete root;}

            bool match(core::inputStream<CHAR>&);

            Int search(core::inputStream<CHAR>&, const core::timeout& time = -1);
            Int foundAt() const {return searchVal.x;};
            Int foundLen() const {return searchVal.y;};

            const bool good() const {return !regex_error;}
            const bool bad() const {return (bool)regex_error;}
        };


        template <typename CHAR>
        void regex<CHAR>::readSymbols(const core::string<CHAR>& expr,
                                      core::array<regexSymbol>* symbols)
        {
            core::array<regexSymbol> openSym;
            core::array<char> inBracketOr; //core::array does not work well with bool

            Int i = 0;
            while (i < expr.length())
            {
                bool inOr;

                if (inBracketOr.size())
                    inOr = inBracketOr[inBracketOr.size()-1];
                else
                    inOr = false;


                if (expr[i] == '{')
                {
                    auto loc = expr.findAfter('}', i);

                    if (loc < 0)
                    {
                        regex_error = 1;
                        return;
                    }
                    else
                    {
                        auto minStr = expr.substr(i+1, loc-1);

                        Int cmaLoc = minStr.find(',');

                        Int min, max;

                        if (cmaLoc < 0)
                        {
                            if (!minStr.isInteger())
                            {
                                regex_error = 1;
                                return;
                            }
                            else
                            {
                                max = min = minStr.integer();

                                if (max < 0)
                                {
                                    regex_error = 1;
                                    return;
                                }
                            }
                        }
                        else
                        {
                            auto maxStr = minStr.substr(cmaLoc+1, minStr.length()-1);
                            minStr.remove(cmaLoc, minStr.length()-1);

                            if (!minStr.isInteger() || !maxStr.isInteger())
                            {
                                regex_error = 1;
                                return;
                            }
                            else
                            {
                                max = maxStr.integer();
                                min = minStr.integer();
                            }
                        }

                        if ((min < 0) || (max < 0))
                        {
                            regex_error = 1;
                            return;
                        }

                        symbols->add(regexSymbol(REGEX_AMOUNT, 0, max, min));
                        i = loc;
                    }
                }
                else if (expr[i] == '|')
                {
                    if (inOr)
                        symbols->add(regexSymbol(REGEX_SYMBOL, expr[i]));
                    else
                        symbols->add(regexSymbol(REGEX_OR));
                }
                else if (expr[i] == '[')
                {
                    openSym.add(regexSymbol(REGEX_START_OR));
                    symbols->add(regexSymbol(REGEX_START_OR));
                    inBracketOr.add(true);
                }
                else if (expr[i] == ']')
                {
                    if ((openSym.size() <= 0) || (openSym.at(openSym.size()-1).type != REGEX_START_OR))
                    {
                        regex_error = 1;
                        return;
                    }

                    openSym.remove(openSym.size()-1);
                    symbols->add(regexSymbol(REGEX_STOP_OR));
                    inBracketOr.remove(inBracketOr.size()-1);
                }
                else if (expr[i] == ')')
                {
                    if ((openSym.size() <= 0) || (openSym.at(openSym.size()-1).type != REGEX_START_OR))
                    {
                        regex_error = 1;
                        return;
                    }

                    openSym.remove(openSym.size()-1);
                    symbols->add(regexSymbol(REGEX_STOP_AND));
                    inBracketOr.remove(inBracketOr.size()-1);
                }
                else if (expr[i] == '.')
                {
                    symbols->add(regexSymbol(REGEX_ANYTHING));
                }
                else if (expr[i] == '*')
                {
                    if (inOr)
                        symbols->add(regexSymbol(REGEX_SYMBOL, expr[i]));
                    else
                        symbols->add(regexSymbol(REGEX_AMOUNT, 0, -1, 0));
                }
                else if (expr[i] == '+')
                {
                    if (inOr)
                        symbols->add(regexSymbol(REGEX_SYMBOL, expr[i]));
                    else
                        symbols->add(regexSymbol(REGEX_AMOUNT, 0, -1, 1));
                }
                else if (expr[i] == '$')
                {
                    symbols->add(regexSymbol(REGEX_END_INPUT));
                }
                else if (expr[i] == '^')
                {
                    symbols->add(regexSymbol(REGEX_BEGIN_INPUT));
                }
                else if (expr[i] == '~')
                {
                    symbols->add(regexSymbol(REGEX_NOT));
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
                    if (inOr)
                        symbols->add(regexSymbol(REGEX_SYMBOL, expr[i]));
                    else
                        symbols->add(regexSymbol(REGEX_AMOUNT, 0, 1, 0));
                }
                else if (expr[i] == '(')
                {
                    openSym.add(regexSymbol(REGEX_START_OR));
                    symbols->add(regexSymbol(REGEX_START_AND));
                    inBracketOr.add(false);
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

            if (openSym.size())
                regex_error = 1;
        }

        template <typename CHAR>
        void regex<CHAR>::createTreeToRoot(core::array<regexSymbol>* symbols)
        {
            if (root) delete root;
            root = new node(REGEX_GROUP_AND); //we want to include everything

            //convert all symbols to nodes
            core::array<node*> nodesList;

            for (Int i=0; i<(symbols->size()); i++)
                nodesList.add(new node(symbols->at(i)));
            symbols->clear();

            core::array<Int> openLoc;
            // core::array<char> isSubOrList;

            // bool isSubOr = false;

            //generate regex tree
            for (Int i=0; i<(nodesList.size()); i++)
            {
                regexSymbol symbol = nodesList[i]->symbol;

                if ((symbol.type == REGEX_START_AND) || (symbol.type == REGEX_START_OR))
                {
                    openLoc.add(i);
                    // isSubOrList.add(isSubOr);
                    // isSubOr = false;
                }
                else if ((symbol.type == REGEX_STOP_AND) || (symbol.type == REGEX_STOP_OR))
                {
                    Int loc = openLoc[openLoc.size()-1];
                    openLoc.remove(openLoc.size()-1);

                    Int aType;

                    if (symbol.type == REGEX_STOP_AND)
                        aType = REGEX_GROUP_AND;
                    else if (symbol.type == REGEX_STOP_OR)
                        aType = REGEX_GROUP_OR;
                    else
                        aType = REGEX_ENUM_TYPE_CT; //Should never happen

                    node* aNode = new node(aType);

                    for (Int n=loc+1; n<i; n++)
                        aNode->addChild(nodesList[n]);
                    
                    delete nodesList[loc];
                    delete nodesList[i];
                        
                    nodesList.replace(loc, i, aNode);
                    i = loc;

                    // isSubOr = isSubOrList[isSubOrList.size()-1];
                    // isSubOrList.remove(isSubOrList.size()-1);
                }
                else if (symbol.type == REGEX_AMOUNT)
                {
                    if (!nodesList.is_valid(i-1))
                    {
                        regex_error = 1;
                        break;
                    }

                    if (nodesList.is_valid(i+1) && (nodesList[i+1]->symbol.type == REGEX_AMOUNT))
                    {
                        regex_error = 1;
                        break;
                    }

                    nodesList[i-1]->symbol.min = symbol.min;
                    nodesList[i-1]->symbol.max = symbol.max;

                    delete nodesList[i];
                    nodesList.remove(i);
                    i--;
                }
                // else if (symbol.type == REGEX_OR)
                // {
                //     isSubOr = true;
                // }


                //see if symbol can be negated
                if ((nodesList[i]->symbol.type >= REGEX_SYMBOL) && 
                    (nodesList[i]->symbol.type <= REGEX_ANYTHING))
                {
                    //see if symbol IS being negated
                    if (i && (nodesList[i-1]->symbol.type == REGEX_NOT))
                    {
                        nodesList[i]->negate = !nodesList[i]->negate;

                        delete nodesList[i-1];
                        nodesList.remove(i-1);
                        i--;
                    }
                }
            }

            for (Int i=0; i<nodesList.size(); i++)
                root->addChild(nodesList[i]);

            nodesList.clear();
        }

        template <typename CHAR>
        void regex<CHAR>::substituteOrInTree(node*& aNode)
        {
            if (aNode && aNode->negate)
            {
                //negation can only happen on symbols that occur exactly once
                if ((aNode->symbol.min != 1) || (aNode->symbol.max != 1))
                {
                    regex_error = 1;
                    return;
                }
            }

            if (aNode && aNode->children.size())
            {
                Int children = aNode->children.size();

                bool isSubOr = false;

                for (Int i=0; i<children; i++)
                {
                    if (aNode->children[i]->symbol.type == REGEX_OR)
                        isSubOr = true;
                    //if we have a negate symbol leftover, bad regex.
                    else if (aNode->children[i]->symbol.type == REGEX_NOT)
                    {
                        regex_error = 1;
                        return;
                    }
                    else
                        substituteOrInTree(aNode->children[i]);
                }

                if (isSubOr)
                {
                    auto aType = aNode->symbol.type;
                    aNode->symbol.type = REGEX_GROUP_OR;

                    node* newANode = new node(aNode->symbol);

                    node* bNode = new node(aType);

                    for (Int i=0; i<children; i++)
                    {
                        node* child = aNode->children[i];

                        if (child->symbol.type == REGEX_OR)
                        {
                            newANode->addChild(bNode);
                            delete child;

                            bNode = new node(aType);
                        }
                        else
                            bNode->addChild(child);
                    }
                    newANode->addChild(bNode);

                    aNode->children.clear();
                    delete aNode;
                    aNode = newANode;
                }
            }
        }

        template <typename CHAR>
        regex<CHAR>::regex(const core::string<CHAR>& expr)
        {
            lastSearchDone = true;
            root = NULL;
            regex_error = 0;

            core::array<regexSymbol> symbols;
            readSymbols(expr, &symbols);

            if (!regex_error)
                createTreeToRoot(&symbols);

            if (!regex_error)
                substituteOrInTree(root);

            system::console con;
            print(con, root);
        }

        template <typename CHAR>
        void regex<CHAR>::unConsume(core::inputStream<CHAR>& input, Int count) const
        {
            for (Int i=0; i<count; i++)
                input.unget();
        }

        //Only consumes input if matched
        //returns # chars consumed if matched, -1 otherwise.
        template <typename CHAR>
        Int regex<CHAR>::matchNodeOnce(const node* aNode, 
                                   core::inputStream<CHAR>& input,
                                   bool caseInsensitive) const
        {
            Int totalConsumed = 0;

            auto symbol = aNode->symbol;

            if (symbol.type == REGEX_END_INPUT)
            {
                if (aNode->negate)
                    return (input.empty() ? -1 : 0);
                else
                    return (input.empty() ? 0 : -1);
            }
            else if (symbol.type == REGEX_BEGIN_INPUT)
            {
                if (aNode->negate)
                    return (input.tell() ? 0 : -1);
                else
                    return (input.tell() ? -1 : 0);
            }
            else if (symbol.type == REGEX_GROUP_AND)
            {
                Int consumed = 0;

                for (Int i=0; i<(aNode->children.size()); i++)
                {
                    auto childType = aNode->children[i]->symbol.type;
                    if (childType == REGEX_START_CASE_I)
                        caseInsensitive = true;
                    else if (childType == REGEX_STOP_CASE_I)
                        caseInsensitive = false;
                    else
                    {
                        Int matched = matchNode(aNode->children[i], input, caseInsensitive);
                        if(matched < 0)
                        {
                            unConsume(input, consumed);
                            return (aNode->negate ? 0 : -1);
                        }
                        else
                        {
                            consumed += matched;
                        }
                    }
                }

                if (aNode->negate) unConsume(input, consumed);
                return (aNode->negate ? -1 : consumed);
            }
            else if (symbol.type == REGEX_GROUP_OR)
            {
                for (Int i=0; i<(aNode->children.size()); i++)
                {
                    auto childType = aNode->children[i]->symbol.type;
                    if (childType == REGEX_START_CASE_I)
                        caseInsensitive = true;
                    else if (childType == REGEX_STOP_CASE_I)
                        caseInsensitive = false;
                    else
                    {
                        Int matched = matchNode(aNode->children[i], input, caseInsensitive);
                        if (matched >= 0)
                        {
                            if (aNode->negate)
                            {
                                unConsume(input, matched);
                                return -1;
                            }
                            else
                                return matched;
                        }
                    }
                }

                if (aNode->negate) input.get();
                return (aNode->negate ? 1 : -1);
            }
            else
            {
                CHAR c = input.get();

                Int matched = -1;

                if (symbol.type == REGEX_SYMBOL)
                {
                    if (caseInsensitive)
                    {
                        if (core::toUpper(symbol.value) == core::toUpper(c)) matched = 1;
                    }
                    else
                    {
                        if (symbol.value == c) matched = 1;
                    }
                }
                else if (symbol.type == REGEX_RANGE_AZ)
                {
                    if (caseInsensitive)
                    {
                        if (core::isAlpha(c)) matched = 1;
                    }
                    else
                    {
                        if (core::isUpperAlpha(c)) matched = 1;
                    }
                }
                else if (symbol.type == REGEX_RANGE_az)
                {
                    if (caseInsensitive)
                    {
                        if (core::isAlpha(c)) matched = 1;
                    }
                    else
                    {
                        if (core::isLowerAlpha(c)) matched = 1;
                    }
                }
                else if (symbol.type == REGEX_RANGE_09)
                {
                    if (core::isNumeric(c)) matched = 1;
                }
                else if (symbol.type == REGEX_WHITESPACE)
                {
                    if (core::isWhiteSpace(c)) matched = 1;
                }
                else if (symbol.type == REGEX_ANYTHING)
                {
                    matched = 1;
                }


                if (aNode->negate)
                {
                    if (matched >= 0)
                    {
                        input.unget();
                        return -1;
                    }
                    else
                        return 1;
                }
                else
                {
                    if (matched >= 0)
                        return matched;
                    else
                    {
                        input.unget();
                        return -1;
                    }
                }
            }
        }

        template <typename CHAR>
        Int regex<CHAR>::matchNode(const node* aNode, 
                                   core::inputStream<CHAR>& input,
                                   bool caseInsensitive) const
        {
            Int max = aNode->symbol.max;
            Int min = aNode->symbol.min;

            Int consumed = 0;

            Int iter = 0;

            // system::console con; con.write('a');
            //at least match the minimum
            for (iter = 0; iter<min; iter++)
            {
                Int matched = matchNodeOnce(aNode, input, caseInsensitive);

                if (matched < 0)
                {
                    unConsume(input, consumed);
                    return -1;
                }
                else
                    consumed += matched;
            }


            if (min == max) //exact number of repetitions
            {
                return consumed;
            }
            else if (max < min) //unbounded max, consume greedily.
            {
                Int matched = -1;

                do
                {
                    matched = matchNodeOnce(aNode, input, caseInsensitive);

                    if (matched > 0) consumed += matched;
                }
                while (matched >= 0);
            }
            else //bounded max
            {
                for (iter = min; iter<max; iter++)
                {
                    Int matched = matchNodeOnce(aNode, input, caseInsensitive);

                    if (matched < 0)
                    {
                        unConsume(input, consumed);
                        return -1;
                    }
                    else
                        consumed += matched;
                }
            }

            return consumed;
        }

        template <typename CHAR>
        bool regex<CHAR>::match(core::inputStream<CHAR>& input)
        {
            lastSearchDone = true;

            searchVal.x = 0; //position
            searchVal.y = 0; //length

            if (!root || regex_error) return false;

            return ((searchVal.y = matchNode(root, input, false)) >= 0);
        }

        //does not consume any input. 1 if found, 0 if timeout, and -1 if not found.
        template <typename CHAR>
        Int regex<CHAR>::search(core::inputStream<CHAR>& input, 
                                const core::timeout& time)
        {
            if (!root || regex_error) return -1;

            if (lastSearchDone)
            {
                searchVal.x = 0; //position
                searchVal.y = 0; //length
            }
            system::console con;
            while (!(input.empty() || time.timedOut()))
            {
                input.seek(searchVal.x);
                
                Int matched = matchNode(root, input, false);

                if (matched > 0)
                {
                    input.seek(0);
                    searchVal.y = matched;

                    lastSearchDone = true;
                    return 1;
                }

                searchVal.x ++;
            }

            input.seek(0);

            lastSearchDone = input.empty();

            return (input.empty() ? -1 : 0);
        }
    }
}

#endif // REGEX_H_INCLUDED