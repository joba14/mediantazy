
/**
 * @file build.c
 * 
 * @copyright This file is a part of the "lasm" project and is distributed, and
 * licensed under "lasm gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-06-03
 */

#define __build_c__
#include "build.h"

#define version_major 1
#define version_minor 0
#define version_patch 0

#define _stringify(x) #x
#define stringify(x) _stringify(x)
#define static_array_length(_array) ((const uint64_t)(sizeof(_array) / sizeof(*(_array))))

static const char_t* const _g_common_defines[] =
{
	"version_major="stringify(version_major),
	"version_minor="stringify(version_minor),
	"version_patch="stringify(version_patch),
};

static const char_t* const _g_common_sources[] =
{
	"./common/source/common/hi.c",
};

static const char_t* const _g_server_sources[] =
{
	"./server/source/server/main.c",
};

static const char_t* const _g_client_sources[] =
{
	"./client/source/client/main.c",
};

static const char_t* const _g_server_includes[] =
{
	"./common/include",
	"./server/include",
};

static const char_t* const _g_client_includes[] =
{
	"./common/include",
	"./client/include",
};

typedef enum
{
	build_conf_dev_server,
	build_conf_rel_server,
	build_conf_dev_client,
	build_conf_rel_client,
} build_conf_e;

static void make_compiler_command(build_command_s* const command, const build_conf_e conf);
static bool_t build(const build_conf_e conf);
static void make_linter_command(build_command_s* const command, const build_conf_e conf);
static bool_t lint(const build_conf_e conf);
static void make_run_command(build_command_s* const command, const build_conf_e conf);
static bool_t run(const build_conf_e conf);

build_target(clean, "clean the project and remove the build directory with all its artefacts.")
{
	build_command_s command = {0};
	build_command_append(&command, "rm", "-fr", "./build");
	const bool_t status = build_proc_run_sync(&command);
	build_vector_drop(&command);
	return status;
}

build_target(build_dev_server, "build the mediantazy server in the develop configuration.")
{
	return build(build_conf_dev_server);
}

build_target(build_rel_server, "build the mediantazy server in the release configuration.")
{
	return build(build_conf_rel_server);
}

build_target(build_dev_client, "build the mediantazy client in the develop configuration.")
{
	return build(build_conf_dev_client);
}

build_target(build_rel_client, "build the mediantazy client in the release configuration.")
{
	return build(build_conf_rel_client);
}

build_target(build_dev_all, "build the mediantazy server and client in the develop configuration.")
{
	return build_dev_server() && build_dev_client();
}

build_target(build_rel_all, "build the mediantazy server and client in the release configuration.")
{
	return build_rel_server() && build_rel_client();
}

build_target(build_all, "build the mediantazy server and client in the develop and release configurations.")
{
	return build_dev_all() && build_rel_all();
}

build_target(lint_dev_server, "lint the mediantazy server in the develop configuration.")
{
	return lint(build_conf_dev_server);
}

build_target(lint_rel_server, "lint the mediantazy server in the release configuration.")
{
	return lint(build_conf_rel_server);
}

build_target(lint_dev_client, "lint the mediantazy client in the develop configuration.")
{
	return lint(build_conf_dev_client);
}

build_target(lint_rel_client, "lint the mediantazy client in the release configuration.")
{
	return lint(build_conf_rel_client);
}

build_target(lint_dev_all, "lint the mediantazy server and client in the develop configuration.")
{
	return lint_dev_server() && lint_dev_client();
}

build_target(lint_rel_all, "lint the mediantazy server and client in the release configuration.")
{
	return lint_rel_server() && lint_rel_client();
}

build_target(lint_all, "lint the mediantazy server and client in the develop and release configurations.")
{
	return lint_dev_all() && lint_rel_all();
}

build_target(docs, "generate the docs for the project.")
{
	build_command_s command = {0};
	build_command_append(&command, "doxygen", "./.doxyfile");
	const bool_t status = build_proc_run_sync(&command);
	build_vector_drop(&command);
	return status;
}

build_target(run_dev_server, "run the mediantazy server in the develop configuration.")
{
	return run(build_conf_dev_server);
}

build_target(run_rel_server, "run the mediantazy server in the release configuration.")
{
	return run(build_conf_rel_server);
}

build_target(run_dev_client, "run the mediantazy client in the develop configuration.")
{
	return run(build_conf_dev_client);
}

build_target(run_rel_client, "run the mediantazy client in the release configuration.")
{
	return run(build_conf_rel_client);
}

build_target(run_dev_all, "run the mediantazy server and client in the develop configuration.")
{
	return run_dev_server() && run_dev_client();
}

build_target(run_rel_all, "run the mediantazy server and client in the release configuration.")
{
	return run_rel_server() && run_rel_client();
}

build_target(run_all, "run the mediantazy server and client in the develop and release configurations.")
{
	return run_dev_all() && run_rel_all();
}

build_targets(
	bind_target(clean           ),
	bind_target(build_dev_server),
	bind_target(build_rel_server),
	bind_target(build_dev_client),
	bind_target(build_rel_client),
	bind_target(build_dev_all   ),
	bind_target(build_rel_all   ),
	bind_target(build_all       ),
	bind_target(lint_dev_server ),
	bind_target(lint_rel_server ),
	bind_target(lint_dev_client ),
	bind_target(lint_rel_client ),
	bind_target(lint_dev_all    ),
	bind_target(lint_rel_all    ),
	bind_target(lint_all        ),
	bind_target(docs            ),
	bind_target(run_dev_server ),
	bind_target(run_rel_server ),
	bind_target(run_dev_client ),
	bind_target(run_rel_client ),
	bind_target(run_dev_all    ),
	bind_target(run_rel_all    ),
	bind_target(run_all        ),
);

static void make_compiler_command(build_command_s* const command, const build_conf_e conf)
{
	build_command_append(command, "gcc", "-std=gnu11",
		"-Wall", "-Wextra", "-Wpedantic", "-Werror", "-Wshadow", "-Wimplicit", "-Wreturn-type", "-Wunknown-pragmas", "-Wunused-variable",
		"-Wunused-function", "-Wmissing-prototypes", "-Wstrict-prototypes", "-Wconversion", "-Wsign-conversion", "-Wunreachable-code"
	);

	switch (conf)
	{
		case build_conf_dev_server: { build_command_append(command, "-O0", "-g3"     , "-o", "./build/mediantazy_dev_server"); } break;
		case build_conf_rel_server: { build_command_append(command, "-O3", "-DNDEBUG", "-o", "./build/mediantazy_rel_server"); } break;
		case build_conf_dev_client: { build_command_append(command, "-O0", "-g3"     , "-o", "./build/mediantazy_dev_client"); } break;
		case build_conf_rel_client: { build_command_append(command, "-O3", "-DNDEBUG", "-o", "./build/mediantazy_rel_client"); } break;
		default:                    { assert(0);                                                                               } break;
	}

	for (uint64_t index = 0; index < static_array_length(_g_common_defines); ++index)
	{
		build_command_append(command, "-D", _g_common_defines[index]);
	}

	for (uint64_t index = 0; index < static_array_length(_g_common_sources); ++index)
	{
		build_command_append(command, _g_common_sources[index]);
	}

	switch (conf)
	{
		case build_conf_dev_server:
		case build_conf_rel_server:
		{
			for (uint64_t index = 0; index < static_array_length(_g_server_includes); ++index)
			{
				build_command_append(command, "-I", _g_server_includes[index]);
			}

			for (uint64_t index = 0; index < static_array_length(_g_server_sources); ++index)
			{
				build_command_append(command, _g_server_sources[index]);
			}
		} break;

		case build_conf_dev_client:
		case build_conf_rel_client:
		{
			for (uint64_t index = 0; index < static_array_length(_g_client_includes); ++index)
			{
				build_command_append(command, "-I", _g_client_includes[index]);
			}

			for (uint64_t index = 0; index < static_array_length(_g_client_sources); ++index)
			{
				build_command_append(command, _g_client_sources[index]);
			}
		} break;

		default: { assert(0); } break;
	}
}

static bool_t build(const build_conf_e conf)
{
	build_command_s command = {0};
	bool_t status = true;

	command.count = 0;
	build_command_append(&command, "mkdir", "-p", "./build");
	if (!build_proc_run_sync(&command))
	{
		status = false;
		goto build_end;
	}

	command.count = 0;
	make_compiler_command(&command, conf);
	if (!build_proc_run_sync(&command))
	{
		status = false;
		goto build_end;
	}

build_end:
	build_vector_drop(&command);
	return status;
}

static void make_linter_command(build_command_s* const command, const build_conf_e conf)
{
	build_command_append(command,
		"cppcheck",
		"--enable=warning,style,performance",
		"--inconclusive",
		"--std=c11",
		"--quiet",
		"--error-exitcode=1",
		"--language=c",
		"-j", "20",
	);

	switch (conf)
	{
		case build_conf_dev_server: {                                            } break;
		case build_conf_rel_server: { build_command_append(command, "-DNDEBUG"); } break;
		case build_conf_dev_client: {                                            } break;
		case build_conf_rel_client: { build_command_append(command, "-DNDEBUG"); } break;
		default:                    { assert(0);                                 } break;
	}

	for (uint64_t index = 0; index < static_array_length(_g_common_defines); ++index)
	{
		build_command_append(command, "-D", _g_common_defines[index]);
	}

	for (uint64_t index = 0; index < static_array_length(_g_common_sources); ++index)
	{
		build_command_append(command, _g_common_sources[index]);
	}

	switch (conf)
	{
		case build_conf_dev_server:
		case build_conf_rel_server:
		{
			for (uint64_t index = 0; index < static_array_length(_g_server_includes); ++index)
			{
				build_command_append(command, "-I", _g_server_includes[index]);
			}

			for (uint64_t index = 0; index < static_array_length(_g_server_sources); ++index)
			{
				build_command_append(command, _g_server_sources[index]);
			}
		} break;

		case build_conf_dev_client:
		case build_conf_rel_client:
		{
			for (uint64_t index = 0; index < static_array_length(_g_client_includes); ++index)
			{
				build_command_append(command, "-I", _g_client_includes[index]);
			}

			for (uint64_t index = 0; index < static_array_length(_g_client_sources); ++index)
			{
				build_command_append(command, _g_client_sources[index]);
			}
		} break;

		default: { assert(0); } break;
	}
}

static bool_t lint(const build_conf_e conf)
{
	build_command_s command = {0};
	make_linter_command(&command, conf);
	const bool_t status = build_proc_run_sync(&command);
	build_vector_drop(&command);
	return status;
}

static void make_run_command(build_command_s* const command, const build_conf_e conf)
{
	switch (conf)
	{
		case build_conf_dev_server: { build_command_append(command, "./build/mediantazy_dev_server"); } break;
		case build_conf_rel_server: { build_command_append(command, "./build/mediantazy_rel_server"); } break;
		case build_conf_dev_client: { build_command_append(command, "./build/mediantazy_dev_client"); } break;
		case build_conf_rel_client: { build_command_append(command, "./build/mediantazy_rel_client"); } break;
		default:                    { assert(0);                                                      } break;
	}
}

static bool_t run(const build_conf_e conf)
{
	build_command_s command = {0};
	make_run_command(&command, conf);
	const bool_t status = build_proc_run_sync(&command);
	build_vector_drop(&command);
	return status;
}
