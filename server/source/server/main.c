
/**
 * @file main.c
 * 
 * @copyright This file's a part of the "mediantazy" project and is distributed
 * and licensed under "mediantazy gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-07-25
 */

#include "common/logger.h"

#include "server/main.h"
#include "server/config.h"

#include <stdio.h>

int32_t main(int32_t argc, const char_t** argv)
{
	server_config_s config = server_config_from_cli(&argc, &argv);
	common_logger_info("config=[address=%s, port=%u, backlog=%u]", config.address, config.port, config.backlog);

	(void)printf("hello, from server!\n");
	return 0;
}
