/* Copyright 2014-2016 Samsung Electronics Co., Ltd.
 * Copyright 2016 University of Szeged.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ECMA_GLOBALS_H
#define ECMA_GLOBALS_H

#include "config.h"
#include "jrt.h"
#include "lit-magic-strings.h"
#include "jmem-allocator.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmatypes ECMA types
 * @{
 *
 * \addtogroup compressedpointer Compressed pointer
 * @{
 */

/**
 * Ecma-pointer field is used to calculate ecma value's address.
 *
 * Ecma-pointer contains value's shifted offset from common Ecma-pointers' base.
 * The offset is shifted right by JMEM_ALIGNMENT_LOG.
 * Least significant JMEM_ALIGNMENT_LOG bits of non-shifted offset are zeroes.
 */
#define ECMA_POINTER_FIELD_WIDTH JMEM_CP_WIDTH

/**
 * The NULL value for compressed pointers
 */
#define ECMA_NULL_POINTER JMEM_CP_NULL

/**
 * @}
 */

/**
 * Type of ecma value
 */
typedef enum
{
  ECMA_TYPE_DIRECT, /**< directly encoded value, a 28 bit signed integer or a simple value */
  ECMA_TYPE_FLOAT, /**< pointer to a 64 or 32 bit floating point number */
  ECMA_TYPE_STRING, /**< pointer to description of a string */
  ECMA_TYPE_OBJECT, /**< pointer to description of an object */
  ECMA_TYPE___MAX = ECMA_TYPE_OBJECT /** highest value for ecma types */
} ecma_type_t;

/**
 * Simple ecma values
 */
typedef enum
{
  /**
   * Empty value is implementation defined value, used for representing:
   *   - empty (uninitialized) values
   *   - immutable binding values
   *   - special register or stack values for vm
   */
  ECMA_SIMPLE_VALUE_EMPTY, /**< uninitialized value */
  ECMA_SIMPLE_VALUE_ARRAY_HOLE, /**< array hole, used for initialization of an array literal */
  ECMA_SIMPLE_VALUE_FALSE, /**< boolean false */
  ECMA_SIMPLE_VALUE_TRUE, /**< boolean true */
  ECMA_SIMPLE_VALUE_UNDEFINED, /**< undefined value */
  ECMA_SIMPLE_VALUE_NULL, /**< null value */
  ECMA_SIMPLE_VALUE_REGISTER_REF, /**< register reference, a special "base" value for vm */
  ECMA_SIMPLE_VALUE__COUNT /** count of simple ecma values */
} ecma_simple_value_t;

/**
 * Description of an ecma value
 *
 * Bit-field structure: type (2) | error (1) | value (29)
 */
typedef uint32_t ecma_value_t;

/**
 * Type for directly encoded integer numbers in JerryScript.
 */
typedef int32_t ecma_integer_value_t;

#if UINTPTR_MAX <= UINT32_MAX

/**
 * JMEM_ALIGNMENT_LOG aligned pointers can be stored directly in ecma_value_t
 */
#define ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY

#endif /* UINTPTR_MAX <= UINT32_MAX */

/**
 * Mask for ecma types in ecma_type_t
 */
#define ECMA_VALUE_TYPE_MASK 0x3u

/**
 * Error flag in ecma_type_t
 */
#define ECMA_VALUE_ERROR_FLAG 0x4u

/**
 * Shift for value part in ecma_type_t
 */
#define ECMA_VALUE_SHIFT 3

/**
 * Mask for directly encoded values
 */
#define ECMA_DIRECT_TYPE_MASK ((1u << ECMA_VALUE_SHIFT) | ECMA_VALUE_TYPE_MASK)

/**
 * Ecma integer value type
 */
#define ECMA_DIRECT_TYPE_INTEGER_VALUE ((0u << ECMA_VALUE_SHIFT) | ECMA_TYPE_DIRECT)

/**
 * Ecma simple value type
 */
#define ECMA_DIRECT_TYPE_SIMPLE_VALUE ((1u << ECMA_VALUE_SHIFT) | ECMA_TYPE_DIRECT)

/**
 * Shift for directly encoded values in ecma_value_t
 */
#define ECMA_DIRECT_SHIFT 4

/**
 * Maximum integer number for an ecma value
 */
#if CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32
#define ECMA_INTEGER_NUMBER_MAX 0x7fffff
#else /* CONFIG_ECMA_NUMBER_TYPE != CONFIG_ECMA_NUMBER_FLOAT32 */
#define ECMA_INTEGER_NUMBER_MAX 0x7ffffff
#endif /* CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32 */

/**
 * Minimum integer number for an ecma value
 */
#if CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32
#define ECMA_INTEGER_NUMBER_MIN -0x7fffff
#else /* CONFIG_ECMA_NUMBER_TYPE != CONFIG_ECMA_NUMBER_FLOAT32 */
#define ECMA_INTEGER_NUMBER_MIN -0x8000000
#endif /* CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32 */

/**
 * Checks whether the integer number is in the integer number range.
 */
#define ECMA_IS_INTEGER_NUMBER(num) \
  (ECMA_INTEGER_NUMBER_MIN <= (num) && (num) <= ECMA_INTEGER_NUMBER_MAX)

/**
 * Maximum integer number, which if squared, still fits in ecma_integer_value_t
 */
#if CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32
#define ECMA_INTEGER_MULTIPLY_MAX 0xb50
#else /* CONFIG_ECMA_NUMBER_TYPE != CONFIG_ECMA_NUMBER_FLOAT32 */
#define ECMA_INTEGER_MULTIPLY_MAX 0x2d41
#endif /* CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32 */

/**
 * Checks whether the error flag is set.
 */
#define ECMA_IS_VALUE_ERROR(value) \
  (unlikely ((value & ECMA_VALUE_ERROR_FLAG) != 0))

/**
 * Internal properties' identifiers.
 */
typedef enum
{
  ECMA_INTERNAL_PROPERTY_CLASS, /**< [[Class]] */
  ECMA_INTERNAL_PROPERTY_SCOPE, /**< [[Scope]] */
  ECMA_INTERNAL_PROPERTY_PARAMETERS_MAP, /**< [[ParametersMap]] */
  ECMA_INTERNAL_PROPERTY_CODE_BYTECODE, /**< pointer to compact bytecode array */
  ECMA_INTERNAL_PROPERTY_REGEXP_BYTECODE, /**< pointer to RegExp bytecode array */

  ECMA_INTERNAL_PROPERTY_NATIVE_CODE, /**< native handler location descriptor */
  ECMA_INTERNAL_PROPERTY_NATIVE_HANDLE, /**< native handle associated with an object */
  ECMA_INTERNAL_PROPERTY_FREE_CALLBACK, /**< object's native free callback */
  ECMA_INTERNAL_PROPERTY_ECMA_VALUE, /**< [[Primitive value]] for String, Number, and Boolean */
  ECMA_INTERNAL_PROPERTY_DATE_FLOAT, /**< float number value type for date objects */

  /** Bound function internal properties **/
  ECMA_INTERNAL_PROPERTY_BOUND_FUNCTION_TARGET_FUNCTION,
  ECMA_INTERNAL_PROPERTY_BOUND_FUNCTION_BOUND_THIS,
  ECMA_INTERNAL_PROPERTY_BOUND_FUNCTION_BOUND_ARGS,

  ECMA_INTERNAL_PROPERTY_BUILT_IN_ID, /**< Implementation-defined identifier of built-in object */

  ECMA_INTERNAL_PROPERTY_BUILT_IN_ROUTINE_DESC,   /**< Implementation-defined identifier of built-in routine
                                                   * that corresponds to a built-in function object
                                                   * ([[Built-in routine's description]])
                                                   */

  ECMA_INTERNAL_PROPERTY_NON_INSTANTIATED_BUILT_IN_MASK_0_31, /**< Bit-mask of non-instantiated
                                                               * built-in's properties (bits 0-31)
                                                               */

  ECMA_INTERNAL_PROPERTY_NON_INSTANTIATED_BUILT_IN_MASK_32_63, /**< Bit-mask of non-instantiated
                                                                * built-in's properties (bits 32-63)
                                                                */

  ECMA_INTERNAL_PROPERTY__COUNT /**< Number of internal properties' types */
} ecma_internal_property_id_t;

/**
 * Property's 'Writable' attribute's values description.
 */
typedef enum
{
  ECMA_PROPERTY_NOT_WRITABLE, /**< property's 'Writable' attribute is false */
  ECMA_PROPERTY_WRITABLE /**< property's 'Writable' attribute is true */
} ecma_property_writable_value_t;

/**
 * Property's 'Enumerable' attribute's values description.
 */
typedef enum
{
  ECMA_PROPERTY_NOT_ENUMERABLE, /**< property's 'Enumerable' attribute is false */
  ECMA_PROPERTY_ENUMERABLE /**< property's 'Enumerable' attribute is true */
} ecma_property_enumerable_value_t;

/**
 * Property's 'Configurable' attribute's values description.
 */
typedef enum
{
  ECMA_PROPERTY_NOT_CONFIGURABLE, /**< property's 'Configurable' attribute is false */
  ECMA_PROPERTY_CONFIGURABLE /**< property's 'Configurable' attribute is true */
} ecma_property_configurable_value_t;

/**
 * Property list:
 *   The property list of an object is a chain list of various items.
 *   The type of each item is stored in the first byte of the item.
 *
 *   The most common item is the property pair, which contains two
 *   ecmascript properties. It is also important, that after the
 *   first property pair, only property pair items are allowed.
 *
 *   Example for other items is property name hash map, or array of items.
 */

/**
 * Property type list.
 */
typedef enum
{
  ECMA_PROPERTY_TYPE_DELETED, /**< deleted property */
  ECMA_PROPERTY_TYPE_INTERNAL, /**< internal property */
  ECMA_PROPERTY_TYPE_NAMEDDATA, /**< property is named data */
  ECMA_PROPERTY_TYPE_NAMEDACCESSOR, /**< property is named accessor */

  ECMA_PROPERTY_TYPE_PROPERTY_PAIR__MAX = ECMA_PROPERTY_TYPE_NAMEDACCESSOR, /**< highest value for
                                                                             *   property pair types. */

  ECMA_PROPERTY_TYPE_HASHMAP, /**< hash map for fast property access */

  ECMA_PROPERTY_TYPE__MAX = ECMA_PROPERTY_TYPE_HASHMAP, /**< highest value for property types. */
} ecma_property_types_t;

/**
 * Property type mask.
 */
#define ECMA_PROPERTY_TYPE_MASK 0x7

/**
 * Property flags base shift.
 */
#define ECMA_PROPERTY_FLAG_SHIFT 3

/**
 * Property flag list (for ECMA_PROPERTY_TYPE_NAMEDDATA
 * and ECMA_PROPERTY_TYPE_NAMEDACCESSOR).
 */
typedef enum
{
  ECMA_PROPERTY_FLAG_CONFIGURABLE = 1u << (ECMA_PROPERTY_FLAG_SHIFT + 0), /**< property is configurable */
  ECMA_PROPERTY_FLAG_ENUMERABLE = 1u << (ECMA_PROPERTY_FLAG_SHIFT + 1), /**< property is enumerable */
  ECMA_PROPERTY_FLAG_WRITABLE = 1u << (ECMA_PROPERTY_FLAG_SHIFT + 2), /**< property is writable */
  ECMA_PROPERTY_FLAG_LCACHED = 1u << (ECMA_PROPERTY_FLAG_SHIFT + 3), /**< property is lcached */
} ecma_property_flags_t;

/**
 * Property flags configurable, enumerable, writable.
 */
#define ECMA_PROPERTY_CONFIGURABLE_ENUMERABLE_WRITABLE \
  (ECMA_PROPERTY_FLAG_CONFIGURABLE | ECMA_PROPERTY_FLAG_ENUMERABLE | ECMA_PROPERTY_FLAG_WRITABLE)

/**
 * Property flags configurable, enumerable.
 */
#define ECMA_PROPERTY_CONFIGURABLE_ENUMERABLE \
  (ECMA_PROPERTY_FLAG_CONFIGURABLE | ECMA_PROPERTY_FLAG_ENUMERABLE)

/**
 * Property flags configurable, enumerable.
 */
#define ECMA_PROPERTY_CONFIGURABLE_WRITABLE \
  (ECMA_PROPERTY_FLAG_CONFIGURABLE | ECMA_PROPERTY_FLAG_WRITABLE)

/**
 * No attributes can be changed for this property.
 */
#define ECMA_PROPERTY_FIXED 0

/**
 * Abstract property representation.
 *
 * A property is a type_and_flags byte and an ecma_value_t value pair.
 * This pair is represented by a single pointer in JerryScript. Although
 * a packed struct would only consume sizeof(ecma_value_t)+1 memory
 * bytes, accessing such structure is inefficient from the CPU viewpoint
 * because the value is not naturally aligned. To improve performance,
 * multiple type bytes and values are packed together. The maximum
 * number of packed items is sizeof(ecma_value_t). The memory layout is
 * the following when the maximum number of items is present:
 *
 *  [type 1, type 2, type 3, type 4][value 1][value 2][value 3][value 4]
 *
 * This way no memory is wasted and values are naturally aligned.
 *
 * For property pairs, only two values are used:
 *
 *  [type 1, type 2, unused 1, unused 2][value 1][value 2]
 *
 * The unused two bytes are used to store a compressed pointer for the
 * next property pair.
 *
 * The advantage of this layout is that the value reference can be computed
 * from the property address. However, property pointers cannot be compressed
 * anymore.
 */
typedef struct
{
  uint8_t type_and_flags; /**< ecma_property_types_t (3 bit) and ecma_property_flags_t */
} ecma_property_t;

/**
 * Number of items in a property pair.
 */
#define ECMA_PROPERTY_PAIR_ITEM_COUNT 2

/**
 * Property header for all items in a property list.
 */
typedef struct
{
  ecma_property_t types[ECMA_PROPERTY_PAIR_ITEM_COUNT]; /**< two property type slot. The first represent
                                                         *   the type of this property (e.g. property pair) */
  jmem_cpointer_t next_property_cp; /**< next cpointer */
} ecma_property_header_t;

/**
 * Pair of pointers - to property's getter and setter
 */
typedef struct
{
  jmem_cpointer_t getter_p; /**< pointer to getter object */
  jmem_cpointer_t setter_p; /**< pointer to setter object */
} ecma_getter_setter_pointers_t;

/**
 * Property data.
 */
typedef union
{
  ecma_value_t value; /**< value of a property */
  ecma_getter_setter_pointers_t getter_setter_pair; /**< getter setter pair */
} ecma_property_value_t;

/**
 * Property pair.
 */
typedef struct
{
  ecma_property_header_t header; /**< header of the property */
  ecma_property_value_t values[ECMA_PROPERTY_PAIR_ITEM_COUNT]; /**< property value slots */
  jmem_cpointer_t names_cp[ECMA_PROPERTY_PAIR_ITEM_COUNT]; /**< property name slots */
} ecma_property_pair_t;

/**
 * Get property type.
 */
#define ECMA_PROPERTY_GET_TYPE(property_p) \
  ((ecma_property_types_t) ((property_p)->type_and_flags & ECMA_PROPERTY_TYPE_MASK))

/**
 * Returns true if the property pointer is a property pair.
 */
#define ECMA_PROPERTY_IS_PROPERTY_PAIR(property_header_p) \
  (ECMA_PROPERTY_GET_TYPE ((property_header_p)->types + 0) <= ECMA_PROPERTY_TYPE_PROPERTY_PAIR__MAX)

/**
 * Returns the internal property type
 */
#define ECMA_PROPERTY_GET_INTERNAL_PROPERTY_TYPE(property_p) \
  ((ecma_internal_property_id_t) ((property_p)->type_and_flags >> ECMA_PROPERTY_FLAG_SHIFT))

/**
 * Computing the data offset of a property.
 */
#define ECMA_PROPERTY_VALUE_OFFSET(property_p) \
  ((((uintptr_t) (property_p)) & (sizeof (ecma_property_value_t) - 1)) + 1)

/**
 * Computing the base address of property data list.
 */
#define ECMA_PROPERTY_VALUE_BASE_PTR(property_p) \
  ((ecma_property_value_t *) (((uintptr_t) (property_p)) & ~(sizeof (ecma_property_value_t) - 1)))

/**
 * Pointer to property data.
 */
#define ECMA_PROPERTY_VALUE_PTR(property_p) \
  (ECMA_PROPERTY_VALUE_BASE_PTR (property_p) + ECMA_PROPERTY_VALUE_OFFSET (property_p))

/**
 * Internal object types
 */
typedef enum
{
  ECMA_OBJECT_TYPE_GENERAL = 0, /**< all objects that are not String (15.5), Function (15.3),
                                 Arguments (10.6), Array (15.4) specification-defined objects */
  ECMA_OBJECT_TYPE_FUNCTION = 1, /**< Function objects (15.3), created through 13.2 routine */
  ECMA_OBJECT_TYPE_BUILT_IN_FUNCTION = 2, /** One of built-in functions described in section 15
                                           *  of ECMA-262 v5 specification */
  ECMA_OBJECT_TYPE_ARRAY = 3, /**< Array object (15.4) */
  ECMA_OBJECT_TYPE_STRING = 4, /**< String objects (15.5) */
  ECMA_OBJECT_TYPE_EXTERNAL_FUNCTION = 5, /**< External (host) function object */
  ECMA_OBJECT_TYPE_BOUND_FUNCTION = 6, /**< Function objects (15.3), created through 15.3.4.5 routine */
  ECMA_OBJECT_TYPE_ARGUMENTS = 7, /**< Arguments object (10.6) */

  ECMA_OBJECT_TYPE__MAX = ECMA_OBJECT_TYPE_ARGUMENTS /**< maximum value */
} ecma_object_type_t;

/**
 * Types of lexical environments
 */
typedef enum
{
  /* ECMA_OBJECT_TYPE_GENERAL (0) with built-in flag. */
  /* ECMA_OBJECT_TYPE_FUNCTION (1) with built-in flag. */
  /* ECMA_OBJECT_TYPE_BUILT_IN_FUNCTION (2) with built-in flag. */
  /* ECMA_OBJECT_TYPE_ARRAY (3) with built-in flag. */
  /* ECMA_OBJECT_TYPE_STRING (4) with built-in flag. */
  ECMA_LEXICAL_ENVIRONMENT_DECLARATIVE = 5, /**< declarative lexical environment */
  ECMA_LEXICAL_ENVIRONMENT_OBJECT_BOUND = 6, /**< object-bound lexical environment */
  ECMA_LEXICAL_ENVIRONMENT_THIS_OBJECT_BOUND = 7, /**< object-bound lexical environment
                                                   *   with provideThis flag */

  ECMA_LEXICAL_ENVIRONMENT_TYPE_START = ECMA_LEXICAL_ENVIRONMENT_DECLARATIVE, /**< first lexical
                                                                               *    environment type */
  ECMA_LEXICAL_ENVIRONMENT_TYPE__MAX = ECMA_LEXICAL_ENVIRONMENT_THIS_OBJECT_BOUND /**< maximum value */
} ecma_lexical_environment_type_t;

/**
 * Ecma object type mask for getting the object type.
 */
#define ECMA_OBJECT_TYPE_MASK 0x07u

/**
 * Ecma object is built-in or lexical environment.
 *   - built-in, if object type is less than ECMA_LEXICAL_ENVIRONMENT_TYPES_START
 *   - lexical environment, if object type is greater or equal than ECMA_LEXICAL_ENVIRONMENT_TYPES_START
 */
#define ECMA_OBJECT_FLAG_BUILT_IN_OR_LEXICAL_ENV 0x08

/**
 * This object is visited by the garbage collector.
 */
#define ECMA_OBJECT_FLAG_GC_VISITED 0x10

/**
 * Extensible object.
 */
#define ECMA_OBJECT_FLAG_EXTENSIBLE 0x20

/**
 * Value for increasing or decreasing the object reference counter.
 */
#define ECMA_OBJECT_REF_ONE (1u << 6)

/**
 * Maximum value of the object reference counter (1023).
 */
#define ECMA_OBJECT_MAX_REF (0x3ffu << 6)

/**
 * Description of ECMA-object or lexical environment
 * (depending on is_lexical_environment).
 */
typedef struct ecma_object_t
{
  /** type : 3 bit : ecma_object_type_t or ecma_lexical_environment_type_t
                     depending on ECMA_OBJECT_FLAG_BUILT_IN_OR_LEXICAL_ENV
      flags : 3 bit : ECMA_OBJECT_FLAG_BUILT_IN_OR_LEXICAL_ENV,
                      ECMA_OBJECT_FLAG_GC_VISITED,
                      ECMA_OBJECT_FLAG_EXTENSIBLE
      refs : 10 bit (max 1023) */
  uint16_t type_flags_refs;

  /** next in the object chain maintained by the garbage collector */
  jmem_cpointer_t gc_next_cp;

  /** compressed pointer to property list or bound object */
  jmem_cpointer_t property_list_or_bound_object_cp;

  /** object prototype or outer reference */
  jmem_cpointer_t prototype_or_outer_reference_cp;
} ecma_object_t;

/**
 * Description of ECMA property descriptor
 *
 * See also: ECMA-262 v5, 8.10.
 *
 * Note:
 *      If a component of descriptor is undefined then corresponding
 *      field should contain it's default value.
 */
typedef struct
{
  /** Is [[Value]] defined? */
  unsigned int is_value_defined : 1;

  /** Is [[Get]] defined? */
  unsigned int is_get_defined : 1;

  /** Is [[Set]] defined? */
  unsigned int is_set_defined : 1;

  /** Is [[Writable]] defined? */
  unsigned int is_writable_defined : 1;

  /** [[Writable]] */
  unsigned int is_writable : 1;

  /** Is [[Enumerable]] defined? */
  unsigned int is_enumerable_defined : 1;

  /** [[Enumerable]] */
  unsigned int is_enumerable : 1;

  /** Is [[Configurable]] defined? */
  unsigned int is_configurable_defined : 1;

  /** [[Configurable]] */
  unsigned int is_configurable : 1;

  /** [[Value]] */
  ecma_value_t value;

  /** [[Get]] */
  ecma_object_t *get_p;

  /** [[Set]] */
  ecma_object_t *set_p;
} ecma_property_descriptor_t;

#if CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32
/**
 * Description of an ecma-number
 */
typedef float ecma_number_t;
#define DOUBLE_TO_ECMA_NUMBER_T(value) (ecma_number_t) (value)

/**
 * Maximum number of significant digits that ecma-number can store
 */
#define ECMA_NUMBER_MAX_DIGITS  (9)

/**
 * Width of sign field
 *
 * See also:
 *          IEEE-754 2008, 3.6, Table 3.5
 */
#define ECMA_NUMBER_SIGN_WIDTH       (1)

/**
 * Width of biased exponent field
 *
 * See also:
 *          IEEE-754 2008, 3.6, Table 3.5
 */
#define ECMA_NUMBER_BIASED_EXP_WIDTH (8)

/**
 * Width of fraction field
 *
 * See also:
 *          IEEE-754 2008, 3.6, Table 3.5
 */
#define ECMA_NUMBER_FRACTION_WIDTH   (23)
#elif CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT64
/**
 * Description of an ecma-number
 */
typedef double ecma_number_t;
#define DOUBLE_TO_ECMA_NUMBER_T(value) value

/**
 * Maximum number of significant digits that ecma-number can store
 */
#define ECMA_NUMBER_MAX_DIGITS  (19)

/**
 * Width of sign field
 *
 * See also:
 *          IEEE-754 2008, 3.6, Table 3.5
 */
#define ECMA_NUMBER_SIGN_WIDTH       (1)

/**
 * Width of biased exponent field
 *
 * See also:
 *          IEEE-754 2008, 3.6, Table 3.5
 */
#define ECMA_NUMBER_BIASED_EXP_WIDTH (11)

/**
 * Width of fraction field
 *
 * See also:
 *          IEEE-754 2008, 3.6, Table 3.5
 */
#define ECMA_NUMBER_FRACTION_WIDTH   (52)
#endif /* CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32 */

/**
 * Value '0' of ecma_number_t
 */
#define ECMA_NUMBER_ZERO ((ecma_number_t) 0)

/**
 * Value '1' of ecma_number_t
 */
#define ECMA_NUMBER_ONE  ((ecma_number_t) 1)

/**
 * Value '2' of ecma_number_t
 */
#define ECMA_NUMBER_TWO  ((ecma_number_t) 2)

/**
 * Value '0.5' of ecma_number_t
 */
#define ECMA_NUMBER_HALF ((ecma_number_t) 0.5f)

/**
 * Value '-1' of ecma_number_t
 */
#define ECMA_NUMBER_MINUS_ONE ((ecma_number_t) -1)

/**
 * Minimum positive and maximum value of ecma-number
 */
#if CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32
# define ECMA_NUMBER_MIN_VALUE (FLT_MIN)
# define ECMA_NUMBER_MAX_VALUE (FLT_MAX)
#elif CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT64
/**
 * Number.MAX_VALUE
 *
 * See also: ECMA_262 v5, 15.7.3.2
 */
# define ECMA_NUMBER_MAX_VALUE ((ecma_number_t) 1.7976931348623157e+308)
/**
 * Number.MIN_VALUE
 *
 * See also: ECMA_262 v5, 15.7.3.3
 */
# define ECMA_NUMBER_MIN_VALUE ((ecma_number_t) 5e-324)
#endif /* CONFIG_ECMA_NUMBER_TYPE == CONFIG_ECMA_NUMBER_FLOAT32 */

/**
 * Euler number
 */
#define ECMA_NUMBER_E  ((ecma_number_t) 2.7182818284590452354)

/**
 * Natural logarithm of 10
 */
#define ECMA_NUMBER_LN10 ((ecma_number_t) 2.302585092994046)

/**
 * Natural logarithm of 2
 */
#define ECMA_NUMBER_LN2 ((ecma_number_t) 0.6931471805599453)

/**
 * Logarithm base 2 of the Euler number
 */
#define ECMA_NUMBER_LOG2E ((ecma_number_t) 1.4426950408889634)

/**
 * Logarithm base 10 of the Euler number
 */
#define ECMA_NUMBER_LOG10E ((ecma_number_t) 0.4342944819032518)

/**
 * Pi number
 */
#define ECMA_NUMBER_PI  ((ecma_number_t) 3.1415926535897932)

/**
 * Square root of 0.5
 */
#define ECMA_NUMBER_SQRT_1_2  ((ecma_number_t) 0.7071067811865476)

/**
 * Square root of 2
 */
#define ECMA_NUMBER_SQRT2  ((ecma_number_t) 1.4142135623730951)

/**
 * Maximum number of characters in string representation of ecma-number
 */
#define ECMA_MAX_CHARS_IN_STRINGIFIED_NUMBER 64

/**
 * Maximum number of characters in string representation of ecma-uint32
 */
#define ECMA_MAX_CHARS_IN_STRINGIFIED_UINT32 10

/**
 * Maximum value of valid array index
 *
 * See also:
 *          ECMA-262 v5, 15.4
 */
#define ECMA_MAX_VALUE_OF_VALID_ARRAY_INDEX ((uint32_t) (-1))

/**
 * Description of a collection's header.
 */
typedef struct
{
  /** Number of elements in the collection */
  ecma_length_t unit_number;

  /** Compressed pointer to first chunk with collection's data */
  jmem_cpointer_t first_chunk_cp;

  /** Compressed pointer to last chunk with collection's data */
  jmem_cpointer_t last_chunk_cp;
} ecma_collection_header_t;

/**
 * Description of non-first chunk in a collection's chain of chunks
 */
typedef struct
{
  /** Characters */
  lit_utf8_byte_t data[ sizeof (uint64_t) - sizeof (jmem_cpointer_t) ];

  /** Compressed pointer to next chunk */
  jmem_cpointer_t next_chunk_cp;
} ecma_collection_chunk_t;

/**
 * Identifier for ecma-string's actual data container
 */
typedef enum
{
  ECMA_STRING_CONTAINER_LIT_TABLE, /**< actual data is in literal table */
  ECMA_STRING_CONTAINER_HEAP_ASCII_STRING, /**< actual data is on the heap as an ascii string */
  ECMA_STRING_CONTAINER_HEAP_UTF8_STRING, /**< actual data is on the heap as an utf-8 string */
  ECMA_STRING_CONTAINER_UINT32_IN_DESC, /**< actual data is UInt32-represeneted Number
                                             stored locally in the string's descriptor */
  ECMA_STRING_CONTAINER_MAGIC_STRING, /**< the ecma-string is equal to one of ECMA magic strings */
  ECMA_STRING_CONTAINER_MAGIC_STRING_EX, /**< the ecma-string is equal to one of external magic strings */

  ECMA_STRING_CONTAINER__MAX = ECMA_STRING_CONTAINER_MAGIC_STRING_EX /**< maximum value */
} ecma_string_container_t;

/**
 * Mask for getting the container of a string.
 */
#define ECMA_STRING_CONTAINER_MASK 0x7u

/**
 * Value for increasing or decreasing the reference counter.
 */
#define ECMA_STRING_REF_ONE (1u << 3)

/**
 * Maximum value of the reference counter (8191).
 */
#define ECMA_STRING_MAX_REF (0x1fffu << 3)

/**
 * Set reference counter to zero (for refs_and_container member below).
 */
#define ECMA_STRING_SET_REF_TO_ONE(refs_and_container) \
  ((uint16_t) (((refs_and_container) & ECMA_STRING_CONTAINER_MASK) | ECMA_STRING_REF_ONE))

/**
 * Returns with the container type of a string.
 */
#define ECMA_STRING_GET_CONTAINER(string_desc_p) \
  ((ecma_string_container_t) ((string_desc_p)->refs_and_container & ECMA_STRING_CONTAINER_MASK))

/**
 * ECMA string-value descriptor
 */
typedef struct ecma_string_t
{
  /** Reference counter for the string */
  uint16_t refs_and_container;

  /** Hash of the string (calculated from two last characters of the string) */
  lit_string_hash_t hash;

  /**
   * Actual data or identifier of it's place in container (depending on 'container' field)
   */
  union
  {
    /** Index of string in literal table */
    jmem_cpointer_t lit_cp;

    /** Compressed pointer to an ecma_collection_header_t */
    jmem_cpointer_t utf8_collection_cp;

    /**
    * Actual data of an ascii string type
    */
    struct
    {
      /** Compressed pointer to a raw character array */
      jmem_cpointer_t ascii_collection_cp;
      /** Size of ascii string in bytes */
      uint16_t size;
    } ascii_string;

    /** Compressed pointer to an ecma_number_t */
    jmem_cpointer_t number_cp;

    /** UInt32-represented number placed locally in the descriptor */
    uint32_t uint32_number;

    /** Identifier of magic string */
    lit_magic_string_id_t magic_string_id;

    /** Identifier of external magic string */
    lit_magic_string_ex_id_t magic_string_ex_id;

    /** For zeroing and comparison in some cases */
    uint32_t common_field;
  } u;
} ecma_string_t;

/**
 * Representation for native external pointer
 */
typedef uintptr_t ecma_external_pointer_t;

/**
 * Compiled byte code data.
  */
typedef struct
{
  uint16_t size;                    /**< real size >> JMEM_ALIGNMENT_LOG */
  uint16_t refs;                    /**< reference counter for the byte code */
  uint16_t status_flags;            /**< various status flags:
                                      *    CBC_CODE_FLAGS_FUNCTION flag tells whether
                                      *    the byte code is function or regular expression.
                                      *    If function, the other flags must be CBC_CODE_FLAGS...
                                      *    If regexp, the other flags must be RE_FLAG... */
} ecma_compiled_code_t;

/**
 * @}
 * @}
 */

#endif  /* !ECMA_GLOBALS_H */
