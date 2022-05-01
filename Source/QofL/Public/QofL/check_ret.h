// Copyright (c) 2022 Mika Pi

#pragma once
#include <QofL/log.h>

#define CHECK_RET(x, ...)                       \
  if (!(x))                                     \
  {                                             \
    ::Internal::logErr(__FILE__, __LINE__, #x); \
    return __VA_ARGS__;                         \
  }
