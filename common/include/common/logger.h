
/**
 * @file logger.h
 * 
 * @copyright This file's a part of the "mediantazy" project and is distributed
 * and licensed under "mediantazy gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-07-25
 */

#ifndef __common__include__common__logger_h__
#define __common__include__common__logger_h__

#include "common/types.h"

#define common_red     "\033[91m"
#define common_yellow  "\033[93m"
#define common_green   "\033[92m"
#define common_blue    "\033[34m"
#define common_magenta "\033[35m"
#define common_cyan    "\033[36m"
#define common_reset   "\033[0m"

/**
 * @brief Log tagless level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void common_logger_log(const char_t* const format, ...) __attribute__ ((format (printf, 1, 2)));

#ifndef NDEBUG
/**
 * @brief Log debug level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void _common_logger_debug_impl(const char_t* const format, ...) __attribute__ ((format (printf, 1, 2)));

#	define common_logger_debug(_format, ...)                                   \
		_common_logger_debug_impl(_format, ##__VA_ARGS__)
#else
#	define common_logger_debug(_format, ...)                                   \
		do                                                                     \
		{                                                                      \
			(void)(_format);                                                   \
			(void)(__VA_ARGS__);                                               \
		} while (0)
#endif

/**
 * @brief Log info level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void common_logger_info(const char_t* const format, ...) __attribute__ ((format (printf, 1, 2)));

/**
 * @brief Log note level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void common_logger_note(const char_t* const format, ...) __attribute__ ((format (printf, 1, 2)));

/**
 * @brief Log warn level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void common_logger_warn(const char_t* const format, ...) __attribute__ ((format (printf, 1, 2)));

/**
 * @brief Log error level formattable messages.
 * 
 * @param format format of the log
 * @param ...    arguments of the log
 */
void common_logger_error(const char_t* const format, ...) __attribute__ ((format (printf, 1, 2)));

#endif
