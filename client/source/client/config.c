
/**
 * @file config.c
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

#include "client/config.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define address_default_value "127.0.0.1"
#define port_default_value    "25505"

static const char_t* _g_program = NULL;

const char_t _g_usage_banner[] =
	"usage: %s <command>\n"                                                                                            \
	"\n"                                                                                                               \
	"commands:\n"                                                                                                      \
	"    run <address> <port>                run the server with provided settings and configuration.\n"               \
	"        required:\n"                                                                                              \
	"            <address>                   set the server address to connect to. if not provided, defaults to %s.\n" \
	"            <port>                      set the server port to bind to. if not provided, defaults to %s.\n"       \
	"        optional:\n"                                                                                              \
	"            ---\n"                                                                                                \
	"\n"                                                                                                               \
	"    help                                print this help message banner.\n"                                        \
	"\n"                                                                                                               \
	"    version                             print the version of this executable.\n"                                  \
	"\n"                                                                                                               \
	"notice:\n"                                                                                                        \
	"    this executable is distributed under the \"mediantazy gplv1\" license.\n";

static void _print_usage_banner(void);

static const char_t* _shift_cli_args(int32_t* const argc, const char_t*** const argv);

static bool_t _match_cli_option(const char_t* const option, const char_t* const long_name, const char_t* const short_name);

static const char_t* _get_option_argument(const char_t* const option, int32_t* const argc, const char_t*** const argv);

static client_config_s _parse_run_command(int32_t* const argc, const char_t*** const argv);

client_config_s client_config_from_cli(int32_t* const argc, const char_t*** const argv)
{
	common_debug_assert(argc != NULL);
	common_debug_assert(argv != NULL);

	common_debug_assert(NULL == _g_program);
	_g_program = _shift_cli_args(argc, argv);
	common_debug_assert(_g_program != NULL);

	if (*argc <= 0)
	{
		common_logger_error("no command was provided.");
		_print_usage_banner();
		exit(1);
	}

	const char_t* const command = _shift_cli_args(argc, argv);
	common_debug_assert(command != NULL);

	if (strcmp(command, "run") == 0)
	{
		return _parse_run_command(argc, argv);
	}
	else if (strcmp(command, "help") == 0)
	{
		_print_usage_banner();
		exit(0);
	}
	else if (strcmp(command, "version") == 0)
	{
#if !defined(version_major)
#	error "missing 'version_major' definition!"
#endif

#if !defined(version_minor)
#	error "missing 'version_minor' definition!"
#endif

#if !defined(version_patch)
#	error "missing 'version_patch' definition!"
#endif

		common_logger_log("%s v%u.%u.%u", _g_program, version_major, version_minor, version_patch);
		exit(0);
	}
	else
	{
		common_logger_error("unknown or invalid command was provided: %s.", command);
		_print_usage_banner();
		exit(1);
	}

	common_debug_assert(0);  // note: should not be reached.
	return (const client_config_s) {0};
}

static void _print_usage_banner(void)
{
	common_debug_assert(_g_usage_banner != NULL);
	common_debug_assert(_g_program != NULL);
	common_logger_log(_g_usage_banner, _g_program, address_default_value, port_default_value);
}

static const char_t* _shift_cli_args(int32_t* const argc, const char_t*** const argv)
{
	common_debug_assert(argc != NULL);
	common_debug_assert(argv != NULL);

	const char_t* argument = NULL;

	if (*argc > 0)
	{
		argument = **argv;
		++(*argv);
		--(*argc);
	}

	return argument;
}

static bool_t _match_cli_option(const char_t* const option, const char_t* const long_name, const char_t* const short_name)
{
	common_debug_assert(option != NULL);
	common_debug_assert(long_name != NULL);
	common_debug_assert(short_name != NULL);

	const uint64_t option_length     = strlen(option);
	const uint64_t long_name_length  = strlen(long_name);
	const uint64_t short_name_length = strlen(short_name);

	return (
		((option_length == long_name_length ) && (strncmp(option, long_name, option_length ) == 0)) || \
		((option_length == short_name_length) && (strncmp(option, short_name, option_length) == 0))
	);
}

static const char_t* _get_option_argument(const char_t* const option, int32_t* const argc, const char_t*** const argv)
{
	common_debug_assert(option != NULL);
	common_debug_assert(argc != NULL);
	common_debug_assert(argv != NULL);

	const char_t* const argument = _shift_cli_args(argc, argv);

	if (NULL == argument)
	{
		common_logger_error("option '%s' requires an argument, but none was provided.", option);
		_print_usage_banner();
		exit(1);
	}

	return argument;
}

static client_config_s _parse_run_command(int32_t* const argc, const char_t*** const argv)
{
	common_debug_assert(argc != NULL);
	common_debug_assert(argv != NULL);

	const char_t* address_as_string = NULL;
	const char_t* port_as_string    = NULL;

	for (uint64_t index = 0; true; ++index)
	{
		const char_t* const option = _shift_cli_args(argc, argv);

		if (NULL == option)
		{
			break;
		}

		if (_match_cli_option(option, "--address", "-a"))
		{
			if (address_as_string != NULL)
			{
				common_logger_error("multiple --address, -a arguments found in the command line arguments in 'run' command.");
				_print_usage_banner();
				exit(1);
			}

			address_as_string = _get_option_argument(option, argc, argv);
			common_debug_assert(address_as_string != NULL);
		}
		else if (_match_cli_option(option, "--port", "-p"))
		{
			if (port_as_string != 0)
			{
				common_logger_error("multiple --port, -p arguments found in the command line arguments in 'run' command.");
				_print_usage_banner();
				exit(1);
			}

			port_as_string = _get_option_argument(option, argc, argv);
			common_debug_assert(port_as_string != NULL);
		}
		else
		{
			common_logger_error("invalid/unrecognized command line argument found in 'run' command: %s.", option);
			_print_usage_banner();
			exit(1);
		}
	}

	if (NULL == address_as_string)
	{
		address_as_string = address_default_value;
	}

	if (NULL == port_as_string)
	{
		port_as_string = port_default_value;
	}

	return (const client_config_s)
	{
		.address = address_as_string                   ,
		.port    = (const uint16_t)atoi(port_as_string),
	};
}
