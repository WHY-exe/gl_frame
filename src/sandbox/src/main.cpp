#include <csignal>
#include <cstdlib>

#include "app.h"
#include "common/dump_helper.h"
#include "common/log_formatter.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>

static void HandleQuitSignal(int signal) noexcept {
    ::signal(signal, SIG_DFL);
#ifdef WIN32
    util::dmp_helper::SnapshotMem();
#endif
    ::raise(SIGABRT);
}

static void TerminateHandle() {
    try {
        std::shared_ptr<spdlog::logger> trace_lg =
            spdlog::basic_logger_st("trace_logger", "log/crash_log.log");
        SPDLOG_LOGGER_ERROR(trace_lg, "{}", cpptrace::generate_trace().to_string());
        std::exception_ptr exptr{std::current_exception()};
        if (exptr) {
            std::rethrow_exception(exptr);
        } else {
            SPDLOG_ERROR("Exiting without exception ...");
        }
    } catch (const std::exception &e) {
        SPDLOG_ERROR("Exception: {}", e.what());
    } catch (...) {
        SPDLOG_ERROR("Unkonown exception, exiting ...");
    }
    std::exit(EXIT_FAILURE);
}

int main() {
    std::set_terminate(TerminateHandle);
#ifdef WIN32
    SetUnhandledExceptionFilter(util::dmp_helper::ExceptionFilter);
#endif
#ifdef linux
    // for linux generating might still need to do
    // bash -c "echo core > /proc/sys/kernel/core_pattern"
    util::dmp_helper::PrepareCore();
#endif
#ifdef unix
    signal(SIGPIPE, SIG_IGN);
#endif
    signal(SIGSEGV, HandleQuitSignal);
    signal(SIGABRT, HandleQuitSignal);
    // spdlog formatter setup: modual, fuction, and line
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<util::log::ModuleFlagFormatter>('.').set_pattern(
        "[%Y-%m-%d %H:%M:%S.%e][%.][%!:%#][tid %t][%l] %v");
    spdlog::set_formatter(std::move(formatter));
    CPPTRACE_TRY {
        sandbox::App{}.Run();
    }
    CPPTRACE_CATCH(const cpptrace::exception &e) {
        SPDLOG_ERROR("trace exception: {}", e.message());
        e.trace().print();
    }
    CPPTRACE_CATCH_ALT(const std::exception &e) {
        SPDLOG_ERROR("common exception: {}", e.what());
        SPDLOG_ERROR("trace: \n{}", cpptrace::from_current_exception().to_string());
    }
    catch (...) {
        SPDLOG_ERROR("An unknown error occoured while app is running ...");
    }
    return 0;
}
