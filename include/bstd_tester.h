#pragma once

#ifndef BSTD_MAX_LOG_LENGTH
#define BSTD_MAX_LOG_LENGTH (1024 * 1024)
#endif
#ifndef BSTD_MAX_ERR_LENGTH
#define BSTD_MAX_ERR_LENGTH (1024 * 1024)
#endif

void bstd_log_init(const char *optional_file_path);

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_TESTER_IMPLEMENTATION__
#endif

#ifdef __BSTD_TESTER_IMPLEMENTATION__

#endif
