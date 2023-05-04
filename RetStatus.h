#pragma once

// #include <fmt/format.h>

// #include "gl_types.h"

enum class E_RetStatus { Ok, NotOk, Tab, TooManyParams, Parsing, Range, Empty, Undefined, Execution, Busy };

struct /*[[deprecated("yet another return code")]] */ RetStatus {
    RetStatus( ) = default;

    RetStatus(E_RetStatus status) : status(status) { }

    explicit RetStatus(std::string_view errorString) : status {E_RetStatus::Execution}, reason {errorString} { }

    // template<typename... T>
    // explicit RetStatus(fmt::format_string<T...> fmt, T&&... args) : RetStatus(fmt::format(fmt, std::forward<T>(args)...))
    // {
    // }

    bool ok ( ) const { return status == E_RetStatus::Ok; }

    std::string message ( ) const
    {
        switch ( code( ) ) {
            case E_RetStatus::TooManyParams: return "Too many parameters";
            case E_RetStatus::Parsing: return "Ambiguous command";
            case E_RetStatus::Empty: return "Not enough parameters";
            case E_RetStatus::Range: return "Invalid input";
            case E_RetStatus::Undefined: return "Undefined error";
            case E_RetStatus::Busy: return "Busy";
            case E_RetStatus::Execution: return reason;
            default: return "unexpected error";
        }
    }

    E_RetStatus code ( ) const { return status; }

    operator E_Status ( ) const { return ok( ) ? E_Status::Ok : E_Status::Error; }

private:
    E_RetStatus status {E_RetStatus::Undefined};
    std::string reason;
};
