#pragma once
#ifndef GENERIC_H_INCLUDED
#define GENERIC_H_INCLUDED

#include <z/float.h>
#include <z/int.h>
#include <z/char.h>

#include <z/core/string.h>
#include <z/core/array.h>

#include <complex>
#include <z/math/complexOps.h>
#include <z/math/remainder.h>

namespace z
{
    namespace util
    {
        enum type
        {
            NONE = 0,

            STRING,
            ARRAY,

            COMPLEX_FLOAT,
            COMPLEX_INT,

            FLOATING,
            INTEGER,

            TYPE_COUNT
        };

        enum opError
        {
            NO_ERROR = 0,
            DIV_BY_ZERO,
            INVALID_STRING_OP,
            INVALID_ARRAY_OP,
            OP_ON_NULL,
            VAL_OVERFLOW,
            VAL_UNDERFLOW,
            MUST_REAL,
            MUST_INTEGER,
            MUST_NONNEG
        };

        class generic
        {
        private:
            opError _error;

            type _type;

            union data_union
            {
                core::string<Char>* String;
                core::array<generic>* Array;

                std::complex<Float>* ComplexFloat;
                std::complex<Int>* ComplexInt;

                Float Floating;
                Int Integer;
            } data;

            void alloc(const generic&);
            void dealloc();

            bool comparableTo(const generic&) const;
            //bool compareToMe(const generic&, math::comparisonOperator*) const;
            //bool compareToMe(const generic&, math::unaryBoolOperator*);
            type operationType(const generic&) const;

        public:
            generic();
            generic(const generic&);
            generic(generic&&);
            generic(const core::string<Char>&);
            generic(const core::array<generic>&);
            generic(const std::complex<Float>&);
            generic(const std::complex<Int>&);
            generic(const Float&);
            generic(const Int&);
            generic(opError);

            ~generic();

            inline const opError error() const {return _error;}

            //Casting methods
            core::string<Char> string() const;
            core::array<generic> array() const;

            std::complex<Float> complexFloat() const;
            std::complex<Int> complexInt() const;

            Float floating() const;
            Int integer() const;

            inline const bool isNull() const {return _type == type::NONE;}

            inline const bool isString() const {return _type == type::STRING;}
            inline const bool isArray() const {return _type == type::ARRAY;}

            inline const bool isComplexFloat() const {return _type == type::COMPLEX_FLOAT;}
            inline const bool isComplexInt() const {return _type == type::COMPLEX_INT;}
            inline const bool isComplex() const {return isComplexFloat() || isComplexInt();}

            inline const bool isFloating() const {return _type == type::FLOATING;}
            inline const bool isInteger() const {return _type == type::INTEGER;}

            inline const bool isNumeric() const {return _type > type::ARRAY;}


            const generic& operator=(const generic&);

            //comparison operators
            const bool operator==(const generic&) const;
            const bool operator>(const generic&) const;
            const bool operator<(const generic&) const;
            const bool operator>=(const generic& other) const;
            const bool operator<=(const generic& other) const;

            inline const bool operator!=(const generic& other) const
            { return !operator==(other); }





            const generic index(const generic&) const;
            const generic index(const generic&,
                               const generic&) const;

            const generic subIndex(const generic&) const;
            const generic subIndex(const generic&,
                                  const generic&) const;

            const generic& merge(const core::array< generic >&);

            //operators
            const generic& operator++();
            const generic& operator--();
            const generic operator++(int);
            const generic operator--(int);

            const generic operator-() const;
            const generic operator+(const generic&) const;
            const generic operator-(const generic&) const;
            const generic& operator+=(const generic&);
            const generic& operator-=(const generic&);

            const generic operator*(const generic&) const;
            const generic operator/(const generic&) const;
            const generic operator%(const generic&) const;
            const generic int_divide(const generic&) const;
            const generic& operator*=(const generic&);
            const generic& operator/=(const generic&);
            const generic& operator%=(const generic&);
            const generic& int_divide_equals(const generic&);

            const generic operator^(const generic&) const;
            const generic factorial() const;

            const generic operator&(const generic&) const;
            const generic operator&&(const generic&) const;
            const generic operator|(const generic&) const;
            const generic operator||(const generic&) const;
            const generic operator!() const;
            const generic operator~() const;
            const generic xor_bitwise(const generic&) const;
            const generic xor_logical(const generic&) const;
            const generic nand_bitwise(const generic&) const;
            const generic nand_logical(const generic&) const;
            const generic nor_bitwise(const generic&) const;
            const generic nor_logical(const generic&) const;
            const generic nxor_bitwise(const generic&) const;
            const generic nxor_logical(const generic&) const;
        };

        generic::generic()
        {
            _type = type::NONE;
            _error = opError::NO_ERROR;
        }

        generic::generic(const generic& other)
        {
            alloc(other);
        }

        generic::generic(generic&& other)
        {
            _type = other._type;
            _error = other._error;
            data = other.data;
        }

        generic::generic(const core::string<Char>& init)
        {
            _type = type::STRING;
            data.String = new core::string<Char>(init);
            _error = opError::NO_ERROR;
        }

        generic::generic(const core::array<generic>& init)
        {
            _type = type::ARRAY;
            data.Array = new core::array<generic>(init);
            _error = opError::NO_ERROR;
        }

        generic::generic(const std::complex<Float>& init)
        {
            if (init.imag())
            {
                if ((init.real() == (Float)(Int)init.real()) &&
                    (init.imag() == (Float)(Int)init.imag())
                    )
                {
                    _type = type::COMPLEX_INT;
                    data.ComplexInt = new std::complex<Int>(init);
                }
                else
                {
                    _type = type::COMPLEX_FLOAT;
                    data.ComplexFloat = new std::complex<Float>(init);
                }
            }
            else
            {
                if (init.real() == (Float)(Int)init.real())
                {
                    _type = type::INTEGER;
                    data.Integer = (Int)init.real();
                }
                else
                {
                    _type = type::FLOATING;
                    data.Floating = (Float)init.real();
                }
            }

            _error = opError::NO_ERROR;
        }

        generic::generic(const std::complex<Int>& init)
        {
            if (init.imag())
            {
                _type = type::COMPLEX_INT;
                data.ComplexInt = new std::complex<Int>(init);
            }
            else
            {
                _type = type::INTEGER;
                data.Integer = init.real();
            }

            _error = opError::NO_ERROR;
        }

        generic::generic(const Float& init)
        {
            if (init == (Float)(Int)init)
            {
                _type = type::INTEGER;
                data.Integer = init;
            }
            else
            {
                _type = type::FLOATING;
                data.Floating = init;
            }

            _error = opError::NO_ERROR;
        }

        generic::generic(const Int& init)
        {
            _type = type::INTEGER;
            data.Integer = init;
            _error = opError::NO_ERROR;
        }

        generic::generic(opError err)
        {
            _type = type::NONE;
            _error = err;
        }

        generic::~generic()
        {
            dealloc();
        }

        core::string<Char> generic::string() const
        {
            if (_type == type::STRING)
                return *data.String;
            else if (_type == type::ARRAY)
            {
                core::string<Char> r_string = "{";

                for (int i=0; i<(data.Array->size()); i++)
                {
                    if (i > 0)
                        r_string += ",";

                    r_string += (data.Array->at(i)).string();
                }

                r_string += "}";

                return r_string;
            }
            else if (_type == type::COMPLEX_FLOAT)
                return core::string<Char>(*data.ComplexFloat);
            else if (_type == type::COMPLEX_INT)
                return core::string<Char>(*data.ComplexInt);
            else if (_type == type::FLOATING)
                return core::string<Char>(data.Floating);
            else if (_type == type::INTEGER)
                return core::string<Char>(data.Integer);
            else
                return core::string<Char>("[NUL]");
        }

        core::array<generic> generic::array() const
        {
            if (_type == type::ARRAY)
            {
                return *data.Array;
            }
            else
                return core::array<generic>();
        }

        std::complex<Float> generic::complexFloat() const
        {
            if (_type == type::STRING)
                return (*data.String).complexValue();
            else if (_type == type::COMPLEX_FLOAT)
                return *data.ComplexFloat;
            else if (_type == type::COMPLEX_INT)
                return std::complex<Float>(data.ComplexInt->real(),
                                           data.ComplexInt->imag());
            else if (_type == type::FLOATING)
                return std::complex<Float>(data.Floating);
            else if (_type == type::INTEGER)
                return std::complex<Float>(data.Integer);
            else
                return std::complex<Float>();
        }

        std::complex<Int> generic::complexInt() const
        {
            if (_type == type::STRING)
            {
                std::complex<Float> value = (*data.String).complexValue();

                return std::complex<Int>(value.real(),
                                         value.imag());
            }
            else if (_type == type::COMPLEX_FLOAT)
                return std::complex<Int>(data.ComplexFloat->real(),
                                         data.ComplexFloat->imag());
            else if (_type == type::COMPLEX_INT)
                return *data.ComplexInt;
            else if (_type == type::FLOATING)
                return std::complex<Int>(data.Floating);
            else if (_type == type::INTEGER)
                return std::complex<Int>(data.Integer);
            else
                return std::complex<Float>();
        }

        Float generic::floating() const
        {
            if (_type == type::STRING)
                return (*data.String).value();
            else if (_type == type::COMPLEX_FLOAT)
                return data.ComplexFloat->real();
            else if (_type == type::COMPLEX_INT)
                return data.ComplexInt->real();
            else if (_type == type::FLOATING)
                return data.Floating;
            else if (_type == type::INTEGER)
                return data.Integer;
            else
                return 0;
        }

        Int generic::integer() const
        {
            if (_type == type::STRING)
                return (*data.String).value();
            else if (_type == type::COMPLEX_FLOAT)
                return data.ComplexFloat->real();
            else if (_type == type::COMPLEX_INT)
                return data.ComplexInt->real();
            else if (_type == type::FLOATING)
                return data.Floating;
            else if (_type == type::INTEGER)
                return data.Integer;
            else
                return 0;
        }

        void generic::alloc(const generic& other)
        {
            _type = other._type;
            _error = other._error;

            if (_type == type::STRING)
            {
                data.String =
                new core::string<Char>(*(other.data.String));
            }
            else if (_type == type::ARRAY)
            {
                data.Array =
                new core::array<generic>(*(other.data.Array));
            }
            else if (_type == type::COMPLEX_FLOAT)
            {
                data.ComplexFloat =
                new std::complex<Float>(*(other.data.ComplexFloat));
            }
            else if (_type == type::COMPLEX_INT)
            {
                data.ComplexInt =
                new std::complex<Int>(*(other.data.ComplexInt));
            }
            else
            {
                data = other.data;
            }
        }

        void generic::dealloc()
        {
            if (_type == type::STRING)
            {
                delete data.String;
            }
            else if (_type == type::ARRAY)
            {
                delete data.Array;
            }
            else if (_type == type::COMPLEX_FLOAT)
            {
                delete data.ComplexFloat;
            }
            else if (_type == type::COMPLEX_INT)
            {
                delete data.ComplexInt;
            }
        }

        bool generic::comparableTo(const generic& other) const
        {
            if (_type == other._type)
                return true;

            if ((_type > type::ARRAY) &&
                (other._type > type::ARRAY))
                return true;

            return false;
        }

        type generic::operationType(const generic& other) const
        {
            if ((_type == type::COMPLEX_FLOAT) ||
                (other._type == type::COMPLEX_FLOAT) ||
                ((_type == type::COMPLEX_INT) &&
                 (other._type == type::FLOATING)
                 ) ||
                ((other._type == type::COMPLEX_INT) &&
                 (_type == type::FLOATING)
                 )
                )
            {
                return type::COMPLEX_FLOAT;
            }
            else if ((_type == type::COMPLEX_INT) ||
                     (other._type == type::COMPLEX_INT)
                     )
            {
                return type::COMPLEX_INT;
            }
            else if ((_type == type::FLOATING) ||
                     (other._type == type::FLOATING)
                     )
            {
                return type::FLOATING;
            }
            else //INTEGER & INTEGER
            {
                return type::INTEGER;
            }
        }


        const generic& generic::operator=(const generic& other)
        {
            dealloc();

            alloc(other);

            return *this;
        }

        const bool generic::operator==(const generic& other) const
        {
            if (comparableTo(other))
            {
                if (_type == type::STRING)
                {
                    return *(data.String) == *(other.data.String);
                }
                else if (_type == type::ARRAY)
                {
                    return *(data.Array) == *(other.data.Array);
                }
                else if (_type == type::NONE)
                {
                    return true;
                }
                else //numeric type
                {
                    type opType = operationType(other);

                    if (opType == type::COMPLEX_FLOAT)
                    {
                        return (complexFloat() == other.complexFloat());
                    }
                    else if (opType == type::COMPLEX_INT)
                    {
                        return (complexInt() == other.complexInt());
                    }
                    else if (opType == type::FLOATING)
                    {
                        return (floating() == other.floating());
                    }
                    else//INTEGER
                    {
                        return (integer() == other.integer());
                    }
                }
            }

            return false;
        }

        const bool generic::operator>(const generic& other) const
        {
            if (comparableTo(other))
            {
                if (_type == type::STRING)
                {
                    return *(data.String) > *(other.data.String);
                }
                else if (_type == type::ARRAY)
                {
                    return *(data.Array) > *(other.data.Array);
                }
                else if (_type == type::NONE)
                {
                    return false;
                }
                else //numeric type
                {
                    type opType = operationType(other);

                    if (opType == type::COMPLEX_FLOAT)
                    {
                        return (complexFloat() > other.complexFloat());
                    }
                    else if (opType == type::COMPLEX_INT)
                    {
                        return (complexInt() > other.complexInt());
                    }
                    else if (opType == type::FLOATING)
                    {
                        return (floating() > other.floating());
                    }
                    else//INTEGER
                    {
                        return (integer() > other.integer());
                    }
                }
            }

            return false;
        }

        const bool generic::operator<(const generic& other) const
        {
            if (comparableTo(other))
            {
                if (_type == type::STRING)
                {
                    return *(data.String) < *(other.data.String);
                }
                else if (_type == type::ARRAY)
                {
                    return *(data.Array) < *(other.data.Array);
                }
                else if (_type == type::NONE)
                {
                    return false;
                }
                else //numeric type
                {
                    type opType = operationType(other);

                    if (opType == type::COMPLEX_FLOAT)
                    {
                        return (complexFloat() < other.complexFloat());
                    }
                    else if (opType == type::COMPLEX_INT)
                    {
                        return (complexInt() < other.complexInt());
                    }
                    else if (opType == type::FLOATING)
                    {
                        return (floating() < other.floating());
                    }
                    else//INTEGER
                    {
                        return (integer() < other.integer());
                    }
                }
            }

            return false;
        }

        const bool generic::operator>=(const generic& other) const
        {
            if (comparableTo(other))
            {
                if (_type == type::STRING)
                {
                    return *(data.String) >= *(other.data.String);
                }
                else if (_type == type::ARRAY)
                {
                    return *(data.Array) >= *(other.data.Array);
                }
                else if (_type == type::NONE)
                {
                    return true;
                }
                else //numeric type
                {
                    type opType = operationType(other);

                    if (opType == type::COMPLEX_FLOAT)
                    {
                        return (complexFloat() >= other.complexFloat());
                    }
                    else if (opType == type::COMPLEX_INT)
                    {
                        return (complexInt() >= other.complexInt());
                    }
                    else if (opType == type::FLOATING)
                    {
                        return (floating() >= other.floating());
                    }
                    else//INTEGER
                    {
                        return (integer() >= other.integer());
                    }
                }
            }

            return false;
        }

        const bool generic::operator<=(const generic& other) const
        {
            if (comparableTo(other))
            {
                if (_type == type::STRING)
                {
                    return *(data.String) <= *(other.data.String);
                }
                else if (_type == type::ARRAY)
                {
                    return *(data.Array) <= *(other.data.Array);
                }
                else if (_type == type::NONE)
                {
                    return true;
                }
                else //numeric type
                {
                    type opType = operationType(other);

                    if (opType == type::COMPLEX_FLOAT)
                    {
                        return (complexFloat() <= other.complexFloat());
                    }
                    else if (opType == type::COMPLEX_INT)
                    {
                        return (complexInt() <= other.complexInt());
                    }
                    else if (opType == type::FLOATING)
                    {
                        return (floating() <= other.floating());
                    }
                    else//INTEGER
                    {
                        return (integer() <= other.integer());
                    }
                }
            }

            return false;
        }


        const generic& generic::operator++()
        {
            if (isNumeric())
            {
                if (isComplexFloat())
                {
                    *data.ComplexFloat += 1;
                }
                else if (isComplexInt())
                {
                    *data.ComplexInt += 1;
                }
                else if (isFloating())
                {
                    data.Floating += 1;
                }
                else//INTEGER
                {
                    data.Integer += 1;
                }
            }
            else
            {
                if (isString())
                    _error = opError::INVALID_STRING_OP;
                else if (isArray())
                    _error = opError::INVALID_ARRAY_OP;
                else //NULL
                    _error = opError::OP_ON_NULL;
            }

            return *this;
        }

        const generic& generic::operator--()
        {
            if (isNumeric())
            {
                if (isComplexFloat())
                {
                    *data.ComplexFloat -= 1;
                }
                else if (isComplexInt())
                {
                    *data.ComplexInt -= 1;
                }
                else if (isFloating())
                {
                    data.Floating -= 1;
                }
                else//INTEGER
                {
                    data.Integer -= 1;
                }
            }
            else
            {
                if (isString())
                    _error = opError::INVALID_STRING_OP;
                else if (isArray())
                    _error = opError::INVALID_ARRAY_OP;
                else //NULL
                    _error = opError::OP_ON_NULL;
            }

            return *this;
        }

        const generic generic::operator++(int)
        {
            const generic oldValue = *this;

            if (isNumeric())
            {
                if (isComplexFloat())
                {
                    *data.ComplexFloat += 1;
                }
                else if (isComplexInt())
                {
                    *data.ComplexInt += 1;
                }
                else if (isFloating())
                {
                    data.Floating += 1;
                }
                else//INTEGER
                {
                    data.Integer += 1;
                }
            }
            else
            {
                if (isString())
                    _error = opError::INVALID_STRING_OP;
                else if (isArray())
                    _error = opError::INVALID_ARRAY_OP;
                else //NULL
                    _error = opError::OP_ON_NULL;
            }

            return oldValue;
        }

        const generic generic::operator--(int)
        {
            const generic oldValue = *this;

            if (isNumeric())
            {
                if (isComplexFloat())
                {
                    *data.ComplexFloat -= 1;
                }
                else if (isComplexInt())
                {
                    *data.ComplexInt -= 1;
                }
                else if (isFloating())
                {
                    data.Floating -= 1;
                }
                else//INTEGER
                {
                    data.Integer -= 1;
                }
            }
            else
            {
                if (isString())
                    _error = opError::INVALID_STRING_OP;
                else if (isArray())
                    _error = opError::INVALID_ARRAY_OP;
                else //NULL
                    _error = opError::OP_ON_NULL;
            }

            return oldValue;
        }


        const generic generic::operator-() const
        {
            if (isNumeric())
            {
                if (isComplexFloat())
                {
                    return -(*data.ComplexFloat);
                }
                else if (isComplexInt())
                {
                    return -(*data.ComplexInt);
                }
                else if (isFloating())
                {
                    return -data.Floating;
                }
                else//INTEGER
                {
                    return -data.Integer;
                }
            }
            else
            {
                if (isString())
                    return opError::INVALID_STRING_OP;
                else if (isArray())
                    return opError::INVALID_ARRAY_OP;
                else //NULL
                    return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator+(const generic& other) const
        {
            if (isArray() || other.isArray())
            {
                if (isArray() && other.isArray())
                {
                    core::array<generic> arr = *data.Array;
                    arr.add(*other.data.Array);
                    return arr;
                }
                else if (isArray())
                {
                    core::array<generic> arr = *data.Array;
                    arr.add(other);
                    return arr;
                }
                else //other is array
                {
                    core::array<generic> arr = *this;
                    arr.add(*other.data.Array);
                    return arr;
                }
            }
            else if (isString() || other.isString())
            {
                return (string() + other.string());
            }
            else if (isNumeric() && other.isNumeric())
            {
                type opType = operationType(other);

                if (opType == type::COMPLEX_FLOAT)
                {
                    return (complexFloat() + other.complexFloat());
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (complexInt() + other.complexInt());
                }
                else if (opType == type::FLOATING)
                {
                    return (floating() + other.floating());
                }
                else//INTEGER
                {
                    return (integer() + other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator-(const generic& other) const
        {
            if (isArray() || other.isArray())
            {
                return opError::INVALID_ARRAY_OP;
            }
            else if (isString() || other.isString())
            {
                return opError::INVALID_STRING_OP;
            }
            else if (isNumeric() && other.isNumeric())
            {
                type opType = operationType(other);

                if (opType == type::COMPLEX_FLOAT)
                {
                    return (complexFloat() - other.complexFloat());
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (complexInt() - other.complexInt());
                }
                else if (opType == type::FLOATING)
                {
                    return (floating() - other.floating());
                }
                else//INTEGER
                {
                    return (integer() - other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator*(const generic& other) const
        {
            if (isArray() || other.isArray())
            {
                return opError::INVALID_ARRAY_OP;
            }
            else if (isString() || other.isString())
            {
                return opError::INVALID_STRING_OP;
            }
            else if (isNumeric() && other.isNumeric())
            {
                type opType = operationType(other);

                if (opType == type::COMPLEX_FLOAT)
                {
                    return (complexFloat() * other.complexFloat());
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (complexInt() * other.complexInt());
                }
                else if (opType == type::FLOATING)
                {
                    return (floating() * other.floating());
                }
                else//INTEGER
                {
                    return (integer() * other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator/(const generic& other) const
        {
            if (isArray() || other.isArray())
            {
                return opError::INVALID_ARRAY_OP;
            }
            else if (isString() || other.isString())
            {
                return opError::INVALID_STRING_OP;
            }
            else if (isNumeric() && other.isNumeric())
            {
                type opType = operationType(other);

                if (opType == type::COMPLEX_FLOAT)
                {
                    return (complexFloat() / other.complexFloat());
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (complexInt() / other.complexInt());

                    std::complex<Int> cintv1 = integer();
                    std::complex<Int> cintv2 = other.integer();

                    std::complex<Int> remd = math::remainder(cintv1,cintv2);

                    if (remd.real() || remd.imag())
                    {
                        return (complexFloat() / other.complexFloat());
                    }
                    else
                        return (cintv1 / cintv2);
                }
                else if (opType == type::FLOATING)
                {
                    return (floating() / other.floating());
                }
                else//INTEGER
                {
                    Int intv1 = integer();
                    Int intv2 = other.integer();

                    if (intv1 % intv2)
                    {
                        return (floating() / other.floating());
                    }
                    else
                        return (intv1 / intv2);
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator%(const generic& other) const
        {
            if (isArray() || other.isArray())
            {
                return opError::INVALID_ARRAY_OP;
            }
            else if (isString() || other.isString())
            {
                return opError::INVALID_STRING_OP;
            }
            else if (isNumeric() && other.isNumeric())
            {
                type opType = operationType(other);

                if ((opType == type::COMPLEX_FLOAT) ||
                    (opType == type::COMPLEX_INT))
                {
                    return math::remainder(complexInt(),other.complexInt());
                }
                else//REAL value only
                {
                    return math::remainder(integer(),other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::int_divide(const generic& other) const
        {
            if (isArray() || other.isArray())
            {
                return opError::INVALID_ARRAY_OP;
            }
            else if (isString() || other.isString())
            {
                return opError::INVALID_STRING_OP;
            }
            else if (isNumeric() && other.isNumeric())
            {
                type opType = operationType(other);

                if ((opType == type::COMPLEX_FLOAT) ||
                    (opType == type::COMPLEX_INT))
                {
                    return (complexInt() / other.complexInt());
                }
                else//REAL value only
                {
                    return (integer() / other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic& generic::operator+=(const generic& other)
        {
            *this = (*this + other);
            return *this;
        }

        const generic& generic::operator-=(const generic& other)
        {
            *this = (*this - other);
            return *this;
        }

        const generic& generic::operator*=(const generic& other)
        {
            *this = (*this * other);
            return *this;
        }

        const generic& generic::operator/=(const generic& other)
        {
            *this = (*this / other);
            return *this;
        }

        const generic& generic::operator%=(const generic& other)
        {
            *this = (*this % other);
            return *this;
        }

        const generic& generic::int_divide_equals(const generic& other)
        {
            *this = this->int_divide(other);
            return *this;
        }


        const generic generic::operator^(const generic& other) const
        {
            if (isArray() || other.isArray())
            {
                return opError::INVALID_ARRAY_OP;
            }
            else if (isString() || other.isString())
            {
                return opError::INVALID_STRING_OP;
            }
            else if (isNumeric() && other.isNumeric())
            {
                type opType = operationType(other);

                if ((opType == type::COMPLEX_FLOAT) ||
                    (opType == type::COMPLEX_INT))
                {
                    return pow(complexFloat(),other.complexFloat());
                }
                else if (opType == type::FLOATING)
                {
                    return (Float)pow(floating(),other.floating());
                }
                else//INTEGER
                {
                    return (Int)pow(integer(),other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::factorial() const
        {
            if (isArray())
            {
                return opError::INVALID_ARRAY_OP;
            }
            else if (isString())
            {
                return opError::INVALID_STRING_OP;
            }
            else if (isComplex())
            {
                return opError::MUST_REAL;
            }
            else if (isFloating())
            {
                return opError::MUST_INTEGER;
            }
            else if (isInteger())
            {
                if (data.Integer < 0)
                    return opError::MUST_NONNEG;

                bool did_overflow;
                Int result = math::factorial(data.Integer, did_overflow);

                if (did_overflow)
                    return opError::VAL_OVERFLOW;

                return result;
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

/*
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::index(const generic<CHAR>& _index) const
        {
            generic<CHAR> result;

            if ((_index.d_type != data::VALUE) ||
                (_index.d_value->imag() != (Float)0)) //bad index
            {
                result = error("Illegal index");
            }
            else if (d_type <= data::VALUE)
            {
                result = error("Cannot index");
            }
            else if (d_type == data::ARRAY)
            {
                int i_index = (int)_index.d_value->real();

                if (d_array->is_valid(i_index))
                    result = d_array->at(i_index);
                else
                {
                    result = error("Index out of bounds");
                }
            }
            else //STRING and (_index.type = VALUE)
            {
                int i_index = (int)_index.d_value->real();

                result = core::string<CHAR>(d_string->at(i_index));
            }

            return result;
        }


        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::index(const generic<CHAR>& start,
                                            const generic<CHAR>& stop) const
        {
            generic<CHAR> result;

            if ((start.d_type != data::VALUE) ||
                (stop.d_type != data::VALUE) ||
                (start.d_value->imag() != (Float)0) ||
                (stop.d_value->imag() != (Float)0)) //bad index
            {
                result = error("Illegal index");
            }
            else if (d_type <= data::VALUE)
            {
                result = error("Cannot index");
            }
            else if (d_type == data::ARRAY)
            {
                int i_start = (int)start.d_value->real();
                int i_stop = (int)stop.d_value->real();

                if (d_array->is_valid(i_start) &&
                    d_array->is_valid(i_stop))
                    result = d_array->subset(i_start, i_stop);
                else
                {
                    result = error("Index out of bounds");
                }
            }
            else //STRING
            {
                int i_start = (int)start.d_value->real();
                int i_stop = (int)stop.d_value->real();

                result = d_string->substr(i_start, i_stop);
            }

            return result;
        }


        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::subIndex(const generic<CHAR>& _index) const
        {
            generic<CHAR> result;

            if ((_index.d_type != data::VALUE) ||
                (_index.d_value->imag() != (Float)0)) //bad index
            {
                result = error("Illegal index");
            }
            else if (d_type != data::ARRAY)
            {
                result = error("Cannot index");
            }
            else
            {
                result.d_type = data::ARRAY;

                int i_index = (int)_index.d_value->real();

                for (int i=0; i<d_array->size(); i++)
                {
                    if (d_array->at(i).d_type <= data::VALUE)
                    {
                        result = error("Cannot index");
                        return result;
                    }
                    else if (d_array->at(i).d_type == data::ARRAY)
                    {
                        if (d_array->at(i).d_array->is_valid(i_index))
                        {
                            result.d_array.add(d_array->at(i).
                                               d_array->at(i_index));
                        }
                        else
                        {
                            result = error("Index out of bounds");
                            return result;
                        }
                    }
                    else //STRING
                    {
                        result.d_array.add(core::string<char>(
                                        d_array->at(i).
                                        d_string->at(i_index)));
                    }
                }
            }

            return result;
        }


        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::subIndex(const generic<CHAR>& start,
                                                  const generic<CHAR>& stop) const
        {
            generic<CHAR> result;

            if ((start.d_type != data::VALUE) ||
                (stop.d_type != data::VALUE) ||
                (start.d_value->imag() != (Float)0) ||
                (stop.d_value->imag() != (Float)0)) //bad index
            {
                result = error("Illegal index");
            }
            else if (d_type != data::ARRAY)
            {
                result = error("Cannot index");
            }
            else
            {
                result = data::ARRAY;

                int i_start = (int)start.d_value->real();
                int i_stop = (int)stop.d_value->real();

                for (int i=0; i<d_array.size(); i++)
                {
                    if (d_array->at(i).d_type <= data::VALUE)
                    {
                        result = error("Cannot index");
                        return result;
                    }
                    else if (d_array->at(i).d_type == data::ARRAY)
                    {
                        if (d_array->at(i).d_array->is_valid(i_start) &&
                            d_array->at(i).d_array->is_valid(i_stop))
                        {

                            result.d_array.add(
                                generic<CHAR>(d_array->at(i).
                                d_array->subset(i_start, i_stop)));
                        }
                        else
                        {
                            result = error("Index out of bounds");
                            return result;
                        }
                    }
                    else //STRING
                    {
                        result.d_array.add(d_array->at(i).
                                           d_string->substr(i_start, i_stop));
                    }
                }
            }

            return result;
        }


        ///Assuming a 2D array list, change array grouping
        ///from horizontal to vertical.


        //{a,b,c}     {a,d,g}
        //{d,e,f} --> {b,e,h}
        //{g,h,i}     {c,f,i}

        template <typename CHAR>
        const generic<CHAR>& generic<CHAR>::merge(const core::array< generic<CHAR> >& arr)
        {
            clear();
            d_type = data::ARRAY;


            int max_width = 1;

            //find max array width
            for (int i=0; i<arr.size(); i++)
            {
                if((arr[i].d_type == data::ARRAY) &&
                   (arr[i].d_array->size() > max_width))
                {
                    max_width = arr[i].d_array->size();
                }
            }


            //fill new arrays in order
            for (int i=0; i<max_width; i++)
            {
                core::array< generic<CHAR> > current;

                for (int j=0; j<arr.size(); j++)
                {
                    int width;
                    if (arr[j].d_type == data::ARRAY)
                        width = arr[j].d_array->size();
                    else
                        width = 1;


                    if (width > i)
                    {
                        if (arr[j].d_type == data::ARRAY)
                            current.add(arr[j].d_array->at(i));
                        else
                            current.add(arr[j]);
                    }
                }

                d_array->add(generic<CHAR>(current));
            }


            return *this;
        }





        ///Operators

        //power
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator^(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }
            else if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = pow(*d_value,*(other.d_value));
            }

            return result;
        }

        //factorial
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::factorial() const
        {
            generic<CHAR> result;

            if (d_type == data::ARRAY)
            {
                result = error("Invalid operation on array");
            }
            else if (d_type == data::STRING)
            {
                result = error("Invalid operation on string");
            }
            else if (d_type == data::VALUE)
            {
                if (d_value->imag() != 0)
                {
                    result = error("Factorial requires a real number");
                }
                else if (d_value->real() < 0)
                {
                    result = error("Factorial must be non-negative");
                }
                else
                {
                    Float _rslt = math::factorial((long)d_value->real());

                    if (_rslt == 0)
                    {
                        result = error("Factorial input overflow");
                    }
                    else
                        result = _rslt;
                }
            }

            return result;
        }

        //logical AND
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator&&(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() && (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise AND
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator&(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() & (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical OR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator||(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() || (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise OR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator|(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() | (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical NOT
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator!() const
        {
            generic<CHAR> result;

            if (d_type == data::VALUE)
                result = !(long)d_value->real();
            else if (d_type == data::STRING)
            {
                result = error("Invalid operation on string");
            }
            else if (d_type == data::ARRAY)
            {
                result = error("Invalid operation on array");
            }

            return result;
        }


        //bitwise NOT
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::operator~() const
        {
            generic<CHAR> result;

            if (d_type == data::VALUE)
                result = ~(long)d_value->real();
            else if (d_type == data::STRING)
            {
                result = error("Invalid operation on string");
            }
            else if (d_type == data::ARRAY)
            {
                result = error("Invalid operation on array");
            }

            return result;
        }

        //logical XOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::xor_logical(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !(long)d_value->real() != !(long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise XOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::xor_bitwise(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = (long)d_value->real() ^ (long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical NAND
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nand_logical(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !((long)d_value->real() && (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise NAND
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nand_bitwise(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = ~((long)d_value->real() & (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical NOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nor_logical(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !((long)d_value->real() || (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise NOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nor_bitwise(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = ~((long)d_value->real() | (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //logical NXOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nxor_logical(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = !(long)d_value->real() == !(long)other.d_value->real();
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }


        //bitwise NXOR
        template <typename CHAR>
        const generic<CHAR> generic<CHAR>::nxor_bitwise(const generic<CHAR>& other) const
        {
            generic<CHAR> result;

            if ((d_type == data::VALUE) && (other.d_type == data::VALUE))
            {
                result = ~((long)d_value->real() ^ (long)other.d_value->real());
            }
            else if ((d_type == data::ARRAY) || (other.d_type == data::ARRAY))
            {
                result = error("Invalid operation on array");
            }
            else if ((d_type == data::STRING) || (other.d_type == data::STRING))
            {
                result = error("Invalid operation on string");
            }

            return result;
        }*/

        /*class generic
        {
        protected:
            bool _error;

        public:
            generic() {_error = false;}
            virtual ~generic() {}

            inline bool error()
            {
                return _error;
            }

            //equality methods
            virtual const bool operator==(const generic&) const;
            virtual const bool operator!=(const generic&) const;
            virtual const bool operator>(const generic&) const;
            virtual const bool operator<(const generic&) const;
            virtual const bool operator>=(const generic&) const;
            virtual const bool operator<=(const generic&) const;

            //casting methods
            virtual const core::string<Char> string() const;
            virtual const core::array<generic>* array() const;
            virtual const std::complex<Float> complexFloat() const;
            virtual const std::complex<Int> complexInt() const;
            virtual const Float floating() const;
            virtual const Int integer() const;

            //type checking
            virtual bool isNull() const;
            virtual bool isString() const;
            virtual bool isArray() const;
            virtual bool isComplexFloat() const;
            virtual bool isComplexInt() const;
            virtual bool isFloating() const;
            virtual bool isInteger() const;

            //operators
            const generic& operator++();
            const generic& operator--();
            const generic operator++(int);
            const generic operator--(int);

            const generic operator-() const;
            const generic operator+(const generic&) const;
            const generic operator+=(const generic&) const;
            const generic operator-(const generic&) const;
            const generic operator-=(const generic&) const;

            const generic operator*(const generic&) const;
            const generic operator/(const generic&) const;
            const generic operator%(const generic&) const;
            const generic int_divide(const generic&) const;
            const generic int_divide_equals(const generic&) const;

            const generic operator^(const generic&) const;
            const generic factorial() const;

            const generic operator&(const generic&) const;
            const generic operator&&(const generic&) const;
            const generic operator|(const generic&) const;
            const generic operator||(const generic&) const;
            const generic operator!() const;
            const generic operator~() const;
            const generic xor_bitwise(const generic&) const;
            const generic xor_logical(const generic&) const;
            const generic nand_bitwise(const generic&) const;
            const generic nand_logical(const generic&) const;
            const generic nor_bitwise(const generic&) const;
            const generic nor_logical(const generic&) const;
            const generic nxor_bitwise(const generic&) const;
            const generic nxor_logical(const generic&) const;
        };

        const core::string<Char> generic::string() const
        {
            return core::string<Char>();
        }

        const core::array<generic>* generic::array() const
        {
            return NULL;
        }

        const std::complex<Float> generic::complexFloat() const
        {
            return std::complex<Float>();
        }

        const std::complex<Int> generic::complexInt() const
        {
            return std::complex<Int>();
        }

        const Float generic::floating() const
        {
            return 0;
        }

        const Int generic::integer() const
        {
            return 0;
        }*/
    }
}

#endif // GENERIC_H_INCLUDED
