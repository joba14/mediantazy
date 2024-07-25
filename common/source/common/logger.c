
/**
 * @file logger.c
 * 
 * @copyright This file's a part of the "mediantazy" project and is distributed
 * and licensed under "mediantazy gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-07-25
 */

#include "common/debug.h"
#include "common/logger.h"

#include <stdarg.h>
#include <stdio.h>

static void _log_with_tag(FILE* const stream, const char_t* const tag, const char_t* const format, va_list args);

void common_logger_log(const char_t* const format, ...)
{
	common_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	_log_with_tag(stdout, NULL, format, args);
	va_end(args);
}

#ifndef NDEBUG
void _common_logger_debug_impl(const char_t* const format, ...)
{
	common_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	_log_with_tag(stdout, common_blue "debug" common_reset, format, args);
	va_end(args);
}
#endif

void common_logger_info(const char_t* const format, ...)
{
	common_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	_log_with_tag(stdout, common_green "info" common_reset, format, args);
	va_end(args);
}

void common_logger_note(const char_t* const format, ...)
{
	common_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	_log_with_tag(stdout, common_cyan "note" common_reset, format, args);
	va_end(args);
}

void common_logger_warn(const char_t* const format, ...)
{
	common_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	_log_with_tag(stderr, common_yellow "warn" common_reset, format, args);
	va_end(args);
}

void common_logger_error(const char_t* const format, ...)
{
	common_debug_assert(format != NULL);
	va_list args; va_start(args, format);
	_log_with_tag(stderr, common_red "error" common_reset, format, args);
	va_end(args);
}

static void _log_with_tag(FILE* const stream, const char_t* const tag, const char_t* const format, va_list args)
{
	common_debug_assert(stream != NULL);
	common_debug_assert(format != NULL);

	if (tag != NULL)
	{
		(void)fprintf(stream, "%s: ", tag);
	}

	(void)vfprintf(stream, format, args);
	(void)fprintf(stream, "\n");
}
