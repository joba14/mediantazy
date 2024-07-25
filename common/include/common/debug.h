
/**
 * @file debug.h
 * 
 * @copyright This file's a part of the "mediantazy" project and is distributed
 * and licensed under "mediantazy gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-07-25
 */

#ifndef __common__include__common__debug_h__
#define __common__include__common__debug_h__

#ifndef NDEBUG
#	include "common/types.h"

/**
 * @brief Debug assert implementation.
 * 
 * @param expression        expression to evaluate
 * @param expression_string stringified expression
 * @param file              file in which assert exists
 * @param line              line in which assert exists
 */
void _common_debug_assert_impl(const bool_t expression, const char_t* const expression_string, const char_t* const file, const uint64_t line);

/**
 * @brief Debug assert wrapper.
 * 
 * @note It abstracts the @ref _common_debug_assert_impl function and passes line,
 * file, and stringified expression to the implementation function.
 */
#	define common_debug_assert(_expression)                                    \
		_common_debug_assert_impl(                                             \
			_expression,                                                       \
			#_expression,                                                      \
			(const char_t* const)__FILE__,                                     \
			(const uint64_t)__LINE__                                           \
		)
#else
/**
 * @brief Debug assert wrapper.
 * 
 * @note It abstracts the @ref _common_debug_assert_impl function and passes line,
 * file, and stringified expression to the implementation function.
 */
#	define common_debug_assert(_expression) ((void)(_expression))
#endif

#endif
