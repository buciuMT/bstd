#pragma once

#include "bstd_core.h"
#include "bstd_error.h"
#include "bstd_utils.h"

typedef enum {
  BS_S_OK,
  BS_INVALID_STATE,
  BS_FLUSH_ERROR,
  BS_WAITING
} bstd_stream_errs;
bstd_error_union_def(bstd_stream_errs, usize) bstd_stream_opt_length;

typedef struct __bstd_sink__ {
  void *backing_obj;
  bstd_stream_opt_length (*write)(void *backing_obj, const u8 *, usize);
  u8 *buffer;
  usize buffer_size;
  void (*close)(void *backing_obj);
  usize position;
  usize start;
} bstd_sink;

bstd_stream_errs bstd_sink_write(bstd_sink *sink, const void *src,
                                 usize objsize, usize count);
bstd_stream_errs bstd_sink_flush(bstd_sink *sink);

ALWAYS_INLINE void bstd_sink_close(bstd_sink *);
void _bstd_stream_close_null(bstd_sink *);

#ifndef __BSTD_EMBEDED__

typedef enum { BS_F_OK, BS_F_EPERM, BS_F_ENOENT, BS_F_EIO } BSTD_FILE_ERR;
bstd_error_union_def(BSTD_FILE_ERR, bstd_sink) bstd_f_sink_e;

bstd_f_sink_e bstd_sink_file_open(const char *path, bool append, bool create,
                                  u8 *buffer, usize buffer_size);
#endif

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_STREAM__
#endif

#ifdef __BSTD_STREAM__

bstd_stream_errs bstd_sink_write(bstd_sink *sink, const void *src,
                                 usize objsize, usize count) {
  usize total = objsize * count;
  if (!sink->buffer) {
    usize current = 0;

    while (current < total)
      current += bstd_or_else(
          sink->write(sink->backing_obj, src + current, total - current), ({
            return err;
            0;
          }));

    return BS_S_OK;
  }
  for (usize written = 0; total - written;) {
    usize cnt = min(total - written, sink->buffer_size - sink->position);

    for (usize i = 0; i < cnt; i++)
      sink->buffer[sink->position + i] = ((u8 *)src)[written + i];

    sink->position += cnt;
    written += cnt;
    if (sink->buffer_size == sink->position) {
      bstd_stream_errs err = bstd_sink_flush(sink);
      if (err)
        return err;
    }
  }
  return BS_S_OK;
}

bstd_stream_errs bstd_sink_flush(bstd_sink *sink) {
  if (sink->start > sink->position || sink->position > sink->buffer_size)
    return BS_INVALID_STATE;
  while (sink->start < sink->position)
    sink->start +=
        bstd_or_else(sink->write(sink->backing_obj, sink->buffer + sink->start,
                                 sink->position - sink->start),
                     ({
                       return err;
                       0;
                     }));
  if (sink->start == sink->position)
    sink->start = sink->position = 0;
  return BS_S_OK;
}

ALWAYS_INLINE void bstd_sink_close(bstd_sink *sink) {
  bstd_sink_flush(sink);
  if (sink->close)
    sink->close(sink->backing_obj);
}

void _bstd_stream_close_null(bstd_sink *sink) {}

#ifndef __BSTD_EMBEDED__
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

bstd_stream_opt_length _bstd_file_write(void *backing_obj, const u8 *buffer,
                                        usize count) {
  int file = (int)(usize)backing_obj;
  ssize_t ret = write(file, buffer, count);
  if (ret < 0)
    return bstd_err(bstd_stream_opt_length,
                    BS_F_EIO); // TODO CONVERT ALL ERRORS
  return bstd_val(bstd_stream_opt_length, ret);
}

void _bstd_sink_file_close(void *backing_obj) {
  close((int)(usize)backing_obj);
}

bstd_f_sink_e bstd_sink_file_open(const char *path, bool append, bool create,
                                  u8 *buffer, usize buffer_size) {
  int file = open(
      path, O_WRONLY | (append ? O_APPEND : O_TRUNC) | (create ? O_CREAT : 0),
      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if (file < 0) {
    switch (errno) {
    case EPERM:
      return bstd_err(bstd_f_sink_e, BS_F_EPERM);
    case ENOENT:
      return bstd_err(bstd_f_sink_e, BS_F_ENOENT);
    case EIO:
      return bstd_err(bstd_f_sink_e, BS_F_EIO);
    };
  }
  return bstd_val(bstd_f_sink_e,
                  ((bstd_sink){.backing_obj = ((void *)(usize)file),
                               .write = _bstd_file_write,
                               .buffer = buffer,
                               .buffer_size = buffer_size,
                               .close = _bstd_sink_file_close}));
}
#endif
#endif
