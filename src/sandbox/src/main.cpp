#include <csignal>
#include <cstdlib>

#include "common/dump_helper.h"
#include "app.h"

#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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

static void InitLog(int level = SPDLOG_LEVEL_INFO, bool enable_console = true,
                    bool                         enable_file = true,
                    const std::filesystem::path &file_path   = "logs/default.log") {
    spdlog::level::level_enum     log_level = static_cast<spdlog::level::level_enum>(level);
    constexpr char const         *pattern   = "[%Y-%m-%d %H:%M:%S.%e][%s:%#][tid %t][%l] %v";
    std::vector<spdlog::sink_ptr> sink_list;
    if (enable_console) {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern(pattern);
        console_sink->set_level(log_level);
        sink_list.push_back(std::move(console_sink));
    }
    if (enable_file) {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_path.string());
        file_sink->set_pattern(pattern);
        file_sink->set_level(log_level);
        sink_list.push_back(std::move(file_sink));
    }
    auto default_logger =
        std::make_shared<spdlog::logger>("default", sink_list.begin(), sink_list.end());
    default_logger->set_level(log_level);
    spdlog::set_default_logger(std::move(default_logger));
    spdlog::flush_every(std::chrono::seconds(3));
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
    CPPTRACE_TRY {
        InitLog();
        SPDLOG_INFO("Hello, Opengl App");
        sandbox::App{}.Run();
    }
    CPPTRACE_CATCH(const cpptrace::exception &e) {
        SPDLOG_ERROR("trace exception: {} \n trace: \n {}", e.message(), e.trace().to_string());
    }
    CPPTRACE_CATCH_ALT(const std::exception &e) {
        SPDLOG_ERROR("common exception: {} \n trace: \n {}", e.what(),
                     cpptrace::from_current_exception().to_string());
    }
    catch (...) {
        SPDLOG_ERROR("An unknown error occoured while app is running ...");
    }
    return 0;
}
