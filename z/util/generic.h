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
        /**
         * \brief Possible \b generic data types.
         */
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

        /**
         * \brief Possible errors that can occur in operations with \b generic objects
         */
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

        /**
         * \brief An all-purpose generic datatype.
         *
         * This class can be initialized as a real or complex number, \b or
         * a string \b or an array of generic objects, and handles all type
         * conversions for the user. Additionally has some error-checking
         * built in, as some operations may be invalid.
         */
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

            /**
             * \brief Integer constructor.
             *
             * Constructs the object as any integer value.
             *
             * \param init the value to initialize this generic object as.
             */
            template<typename T,
            typename = typename std::enable_if<std::is_integral<T>::value>::type>
            generic(const T& init)
            {
                _type = type::INTEGER;
                data.Integer = (Int)init;
                _error = opError::NO_ERROR;
            }

            generic(opError);

            ~generic();

            /**
             * \brief Get the error code of this object.
             *
             * This object will only have a non-zero error code if the previous
             * operation to produce the current state of the object was invalid
             * (e.g. dividing by a string).
             *
             * \return The error code for this object. \b 0 if no error.
             */
            inline const opError error() const {return _error;}

            //Casting methods
            core::string<Char> string() const;
            core::array<generic> array() const;

            std::complex<Float> complexFloat() const;
            std::complex<Int> complexInt() const;

            Float floating() const;
            Int integer() const;

            /**
             * \brief Get whether this object is \b null.
             *
             * \return \b True if the type is \b NONE. \b False otherwise.
             */
            inline const bool isNull() const {return _type == type::NONE;}

            /**
             * \brief Get whether this object is a string.
             *
             * \return \b True if the type is \b STRING. \b False otherwise.
             */
            inline const bool isString() const {return _type == type::STRING;}

            /**
             * \brief Get whether this object is an array.
             *
             * \return \b True if the type is \b ARRAY. \b False otherwise.
             */
            inline const bool isArray() const {return _type == type::ARRAY;}

            /**
             * \brief Get whether this object is complex and floating-point.
             *
             * \return \b True if the type is \b COMPLEX_FLOAT. \b False otherwise.
             */
            inline const bool isComplexFloat() const {return _type == type::COMPLEX_FLOAT;}

            /**
             * \brief Get whether this object is complex and integral.
             *
             * \return \b True if the type is \b COMPLEX_INT. \b False otherwise.
             */
            inline const bool isComplexInt() const {return _type == type::COMPLEX_INT;}

            /**
             * \brief Get whether this object is a complex number.
             *
             * \return \b True if the type is \b COMPLEX_FLOAT or \b COMPLEX_INT. \b False otherwise.
             */
            inline const bool isComplex() const {return isComplexFloat() || isComplexInt();}

            /**
             * \brief Get whether this object is a floating-point value.
             *
             * \return \b True if the type is \b FLOATING. \b False otherwise.
             */
            inline const bool isFloating() const {return _type == type::FLOATING;}

            /**
             * \brief Get whether this object is an integer value.
             *
             * \return \b True if the type is \b INTEGER. \b False otherwise.
             */
            inline const bool isInteger() const {return _type == type::INTEGER;}

            /**
             * \brief Get whether this object is a numeric value.
             *
             * \return \b True if the type is \b FLOATING, \b INTEGER, \b COMPLEX_FLOAT or \b COMPLEX_INT. \b False otherwise.
             */
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

            //const generic subIndex(const generic&) const;
            //const generic subIndex(const generic&,
              //                    const generic&) const;

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
            const generic intDivide(const generic&) const;
            const generic& operator*=(const generic&);
            const generic& operator/=(const generic&);
            const generic& operator%=(const generic&);
            const generic& intDivideEqual(const generic&);

            const generic operator^(const generic&) const;
            const generic factorial() const;

            const generic operator&(const generic&) const;
            const generic operator&&(const generic&) const;
            const generic operator|(const generic&) const;
            const generic operator||(const generic&) const;
            const generic operator!() const;
            const generic operator~() const;
            const generic logicalXor(const generic&) const;
            const generic bitwiseXor(const generic&) const;
            const generic logicalNand(const generic&) const;
            const generic bitwiseNand(const generic&) const;
            const generic logicalNor(const generic&) const;
            const generic bitwiseNor(const generic&) const;
            const generic logicalNxor(const generic&) const;
            const generic bitwiseNxor(const generic&) const;
        };

        /**
         * \brief Default constructor. Constructs as type \b null.
         */
        generic::generic()
        {
            _type = type::NONE;
            _error = opError::NO_ERROR;
        }

        /**
         * \brief Copy constructor.
         *
         * \param other the generic object to duplicate.
         */
        generic::generic(const generic& other)
        {
            alloc(other);
        }

        /**
         * \brief Move constructor.
         *
         * \param other the generic object containing the data to be moved.
         */
        generic::generic(generic&& other)
        {
            _type = other._type;
            _error = other._error;
            data = other.data;
        }

        /**
         * \brief String constructor.
         *
         * Constructs the object as a string type.
         *
         * \param init the core::string to initialize this generic object as.
         */
        generic::generic(const core::string<Char>& init)
        {
            _type = type::STRING;
            data.String = new core::string<Char>(init);
            _error = opError::NO_ERROR;
        }

        /**
         * \brief Array constructor.
         *
         * Constructs the object as an array of generic objects.
         *
         * \param init the core::array to initialize this generic object as.
         */
        generic::generic(const core::array<generic>& init)
        {
            _type = type::ARRAY;
            data.Array = new core::array<generic>(init);
            _error = opError::NO_ERROR;
        }

        /**
         * \brief Complex float constructor.
         *
         * Constructs the object as a floating-point complex number.
         *
         * \param init the complex value to initialize this generic object as.
         */
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

        /**
         * \brief Complex int constructor.
         *
         * Constructs the object as an integer complex number.
         *
         * \param init the complex value to initialize this generic object as.
         */
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

        /**
         * \brief Float constructor.
         *
         * Constructs the object as a floating-point number.
         *
         * \param init the value to initialize this generic object as.
         */
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


        /**
         * \brief Error constructor.
         *
         * Constructs the object with an error code.
         *
         * \param err the error value to initialize this generic object as.
         */
        generic::generic(opError err)
        {
            _type = type::NONE;
            _error = err;
        }

        /**
         * \brief Destructor.
         */
        generic::~generic()
        {
            dealloc();
        }

        /**
         * \brief String data cast.
         *
         * Casts the data contained by this object to a core::string object.
         *
         * \return A string representation of the data.
         */
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

        /**
         * \brief Array data cast.
         *
         * Casts the data contained by this object to a core::array object.
         *
         * \return The original data, if type is \b ARRAY. Otherwise, an empty array.
         */
        core::array<generic> generic::array() const
        {
            if (_type == type::ARRAY)
            {
                return *data.Array;
            }
            else
                return core::array<generic>();
        }

        /**
         * \brief Complex float data cast.
         *
         * Casts the data contained by this object to a std::complex<Float> object.
         *
         * \return A complex floating-point representation of the data.
         */
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

        /**
         * \brief Complex integer data cast.
         *
         * Casts the data contained by this object to a std::complex<Int> object.
         *
         * \return A complex integer representation of the data.
         */
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

        /**
         * \brief Floating-point data cast.
         *
         * Casts the data contained by this object to a z::Float value.
         *
         * \return A floating-point representation of the data.
         */
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

        /**
         * \brief Integer data cast.
         *
         * Casts the data contained by this object to a z::Int value.
         *
         * \return An integer representation of the data.
         */
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

        //Internal function to allocate and copy data from the given generic object
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

        //Internal function to de-allocate data.
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

        const generic generic::intDivide(const generic& other) const
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

        const generic& generic::intDivideEqual(const generic& other)
        {
            *this = this->intDivide(other);
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


        const generic generic::operator&&(const generic& other) const
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
                    return (Int)((complexFloat().real() || complexFloat().imag()) &&
                        (other.complexFloat().real() || other.complexFloat().imag()));
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (Int)((complexInt().real() || complexInt().imag()) &&
                        (other.complexInt().real() || other.complexInt().imag()));
                }
                else if (opType == type::FLOATING)
                {
                    return (Int)(floating() && other.floating());
                }
                else//INTEGER
                {
                    return (Int)(integer() && other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator&(const generic& other) const
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
                return (Int)(integer() & other.integer());
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator||(const generic& other) const
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
                    return (Int)((complexFloat().real() || complexFloat().imag()) ||
                        (other.complexFloat().real() || other.complexFloat().imag()));
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (Int)((complexInt().real() || complexInt().imag()) ||
                        (other.complexInt().real() || other.complexInt().imag()));
                }
                else if (opType == type::FLOATING)
                {
                    return (Int)(floating() || other.floating());
                }
                else//INTEGER
                {
                    return (Int)(integer() || other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator|(const generic& other) const
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
                return (Int)(integer() | other.integer());
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator!() const
        {
            if (isArray())
            {
                return opError::INVALID_ARRAY_OP;
            }
            else if (isString())
            {
                return opError::INVALID_STRING_OP;
            }
            else if (isNumeric())
            {
                if (_type == type::COMPLEX_FLOAT)
                {
                    return (Int)!(data.ComplexFloat->real() ||
                                  data.ComplexFloat->imag());
                }
                else if (_type == type::COMPLEX_INT)
                {
                    return (Int)!(data.ComplexInt->real() ||
                                  data.ComplexInt->imag());
                }
                else if (_type == type::FLOATING)
                {
                    return (Int)!data.Floating;
                }
                else//INTEGER
                {
                    return (Int)!data.Integer;
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::operator~() const
        {
            if (isArray())
            {
                return opError::INVALID_ARRAY_OP;
            }
            else if (isString())
            {
                return opError::INVALID_STRING_OP;
            }
            else if (isNumeric())
            {
                return (Int)~integer();
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::logicalXor(const generic& other) const
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
                    return (Int)((complexFloat().real() || complexFloat().imag()) !=
                        (other.complexFloat().real() || other.complexFloat().imag()));
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (Int)((complexInt().real() || complexInt().imag()) !=
                        (other.complexInt().real() || other.complexInt().imag()));
                }
                else if (opType == type::FLOATING)
                {
                    return (Int)(!floating() != !other.floating());
                }
                else//INTEGER
                {
                    return (Int)(!integer() != !other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::bitwiseXor(const generic& other) const
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
                return (Int)(integer() ^ other.integer());
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::logicalNand(const generic& other) const
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
                    return (Int)!((complexFloat().real() || complexFloat().imag()) &&
                        (other.complexFloat().real() || other.complexFloat().imag()));
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (Int)!((complexInt().real() || complexInt().imag()) &&
                        (other.complexInt().real() || other.complexInt().imag()));
                }
                else if (opType == type::FLOATING)
                {
                    return (Int)!(floating() && other.floating());
                }
                else//INTEGER
                {
                    return (Int)!(integer() && other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::bitwiseNand(const generic& other) const
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
                return (Int)~(integer() & other.integer());
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::logicalNor(const generic& other) const
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
                    return (Int)!((complexFloat().real() || complexFloat().imag()) ||
                        (other.complexFloat().real() || other.complexFloat().imag()));
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (Int)!((complexInt().real() || complexInt().imag()) ||
                        (other.complexInt().real() || other.complexInt().imag()));
                }
                else if (opType == type::FLOATING)
                {
                    return (Int)!(floating() || other.floating());
                }
                else//INTEGER
                {
                    return (Int)!(integer() || other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::bitwiseNor(const generic& other) const
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
                return (Int)~(integer() | other.integer());
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::logicalNxor(const generic& other) const
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
                    return (Int)((complexFloat().real() || complexFloat().imag()) ==
                        (other.complexFloat().real() || other.complexFloat().imag()));
                }
                else if (opType == type::COMPLEX_INT)
                {
                    return (Int)((complexInt().real() || complexInt().imag()) ==
                        (other.complexInt().real() || other.complexInt().imag()));
                }
                else if (opType == type::FLOATING)
                {
                    return (Int)(!floating() == !other.floating());
                }
                else//INTEGER
                {
                    return (Int)(!integer() == !other.integer());
                }
            }
            else //NULL
            {
                return opError::OP_ON_NULL;
            }
        }

        const generic generic::bitwiseNxor(const generic& other) const
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
                return (Int)~(integer() ^ other.integer());
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
        }*/
    }
}

#endif // GENERIC_H_INCLUDED
