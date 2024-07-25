
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

#include "client/config.h"
#include "client/main.h"

#include <stdio.h>

int32_t main(int32_t argc, const char_t** argv)
{
	client_config_s config = client_config_from_cli(&argc, &argv);
	common_logger_info("config=[address=%s, port=%u]", config.address, config.port);

	(void)printf("hello, from client!\n");
	return 0;
}
