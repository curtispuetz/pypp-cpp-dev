#pragma once

#include <filesystem>
#include <ios>
#include <new>
#include <stdexcept>
#include <string>
#include <system_error>
#include <typeinfo>

// Base class for all Pypp exceptions
class PyppException : public std::exception {
  public:
    explicit PyppException(const std::string &msg) : msg_(msg) {}
    const char *what() const noexcept override { return msg_.c_str(); }

  protected:
    std::string msg_;
};

// Derived classes
class PyppRuntimeError : public std::runtime_error {
  public:
    explicit PyppRuntimeError(const std::string &msg)
        : std::runtime_error(msg) {}
};

class PyppValueError : public std::invalid_argument {
  public:
    explicit PyppValueError(const std::string &msg)
        : std::invalid_argument(msg) {}
};

class PyppTypeError : public std::invalid_argument {
  public:
    explicit PyppTypeError(const std::string &msg)
        : std::invalid_argument(msg) {}
};

class PyppIndexError : public std::out_of_range {
  public:
    explicit PyppIndexError(const std::string &msg) : std::out_of_range(msg) {}
};

class PyppKeyError : public std::out_of_range {
  public:
    explicit PyppKeyError(const std::string &msg) : std::out_of_range(msg) {}
};

class PyppZeroDivisionError : public std::domain_error {
  public:
    explicit PyppZeroDivisionError(const std::string &msg)
        : std::domain_error(msg) {}
};

class PyppFileNotFoundError : public std::filesystem::filesystem_error {
  public:
    PyppFileNotFoundError(const std::string &msg,
                          const std::filesystem::path &path)
        : std::filesystem::filesystem_error(msg, path, std::error_code()) {}
};

class PyppIOError : public std::ios_base::failure {
  public:
    explicit PyppIOError(const std::string &msg)
        : std::ios_base::failure(msg) {}
};

class PyppOSError : public std::system_error {
  public:
    PyppOSError(const std::string &msg, std::error_code ec)
        : std::system_error(ec, msg) {}
};

class PyppMemoryError : public std::bad_alloc {
  public:
    const char *what() const noexcept override {
        return "PyppMemoryError: failed to allocate memory";
    }
};

class PyppAssertionError : public std::logic_error {
  public:
    explicit PyppAssertionError(const std::string &msg)
        : std::logic_error(msg) {}
};

class PyppNotImplementedError : public std::logic_error {
  public:
    explicit PyppNotImplementedError(const std::string &msg)
        : std::logic_error(msg) {}
};

class PyppAttributeError : public std::logic_error {
  public:
    explicit PyppAttributeError(const std::string &msg)
        : std::logic_error(msg) {}
};

// No direct mapping – included for completeness
class PyppNameError : public PyppException {
  public:
    explicit PyppNameError(const std::string &msg) : PyppException(msg) {}
};

class PyppImportError : public PyppException {
  public:
    explicit PyppImportError(const std::string &msg) : PyppException(msg) {}
};

class PyppStopIteration : public PyppException {
  public:
    explicit PyppStopIteration(const std::string &msg = "Iteration stopped")
        : PyppException(msg) {}
};
