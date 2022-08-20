// Copyright (c) 2022 Mika Pi

#include "QofL/log.h"

#include <CoreMinimal.h>
#include <fstream>

namespace Internal
{
  auto dumpLog(const char *value) -> void
  {
    UE_LOG(LogTemp, Warning, TEXT("%s"), ANSI_TO_TCHAR(value));
  }

  auto dumpLogErr(const char *value) -> void
  {
    UE_LOG(LogTemp, Error, TEXT("%s"), ANSI_TO_TCHAR(value));
  }

  auto dumpLogInfo(const char *value) -> void
  {
    UE_LOG(LogTemp, Display, TEXT("%s"), ANSI_TO_TCHAR(value));
  }
} // namespace Internal

auto operator<<(std::ostream &strm, const FString &val) -> std::ostream &
{
  strm << TCHAR_TO_UTF8(*val);
  return strm;
}

auto operator<<(std::ostream &strm, const FVector &val) -> std::ostream &
{
  strm << "(" << val.X << ", " << val.Y << ", " << val.Z << ")";
  return strm;
}

auto operator<<(std::ostream &strm, const FRotator &val) -> std::ostream &
{
  strm << "(" << val.Pitch << ", " << val.Yaw << ", " << val.Roll << ")";
  return strm;
}
