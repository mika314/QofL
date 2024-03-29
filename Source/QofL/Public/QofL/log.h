// Copyright (c) 2022 Mika Pi

#pragma once
#include <Math/Rotator.h>
#include <Math/Vector.h>
#include <sstream>

auto QOFL_API toStrm(std::ostream &strm, const FString &) -> std::ostream &;
auto QOFL_API toStrm(std::ostream &strm, const FText &) -> std::ostream &;
auto QOFL_API toStrm(std::ostream &strm, const FVector &) -> std::ostream &;
auto QOFL_API toStrm(std::ostream &strm, const FRotator &) -> std::ostream &;
auto QOFL_API toStrm(std::ostream &strm, const TCHAR *) -> std::ostream &;
auto QOFL_API toStrm(std::ostream &strm, const FName &) -> std::ostream &;
auto QOFL_API toStrm(std::ostream &strm, const std::string &) -> std::ostream &;
auto QOFL_API toStrm(std::ostream &strm, const char *) -> std::ostream &;
auto QOFL_API toStrm(std::ostream &strm, const std::string_view &) -> std::ostream &;

auto toStrm(std::ostream &strm, auto v) -> std::ostream &
{
  strm << std::move(v) << " ";
  return strm;
}

namespace Internal
{
  void QOFL_API dumpLog(const char *);
  void QOFL_API dumpLogErr(const char *);
  void QOFL_API dumpLogInfo(const char *);
  template <typename... Args>
  void log(const char *fileName, int line, Args &&...args)
  {
    std::ostringstream strm;
    (toStrm(strm, std::forward<Args>(args)), ...);
    strm << fileName << ":" << line;
    dumpLog(strm.str().c_str());
  }
  template <typename... Args>
  void logErr(const char *fileName, int line, Args &&...args)
  {
    std::ostringstream strm;
    (toStrm(strm, std::forward<Args>(args)), ...);
    strm << fileName << ":" << line;
    dumpLogErr(strm.str().c_str());
  }

  template <typename... Args>
  void logInfo(const char *fileName, int line, Args &&...args)
  {
    std::ostringstream strm;
    (toStrm(strm, std::forward<Args>(args)), ...);
    strm << fileName << ":" << line;
    dumpLogInfo(strm.str().c_str());
  }

  template <typename... Args>
  void shortLog(Args &&...args)
  {
    std::ostringstream strm;
    (toStrm(strm, std::forward<Args>(args)), ...);
    dumpLog(strm.str().c_str());
  }
} // namespace Internal

#define LOG(...) ::Internal::log(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) ::Internal::logInfo(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...) ::Internal::logErr(__FILE__, __LINE__, __VA_ARGS__)
#define SHORT_LOG(...) ::Internal::shortLog(__VA_ARGS__)
