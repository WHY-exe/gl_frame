#include "common/dump_helper.h"
#include "common/log.h"
#include "sandbox/app.h"
#include <csignal>

#ifdef WIN32
void HandleQuitSignal(int signal) noexcept {
  switch (signal) {
  case SIGSEGV:
    LOG_ERROR_COUT("segment fault, there might be problems in code");
    break;
  case SIGABRT:
    LOG_ERROR_COUT("signal abort, there might be problems in code");
    break;
  case SIGFPE:
    LOG_ERROR_COUT("floating point exception");
    break;
  case SIGILL:
    LOG_ERROR_COUT("illegal instruction, there might be problems in code");
    break;
  case SIGINT:
    LOG_WARN_COUT("signal keyboard interrupt, quiting...");
    break;
  case SIGTERM:
    LOG_WARN_COUT("signal terminate, quiting...");
    break;
  default:
    LOG_ERROR_COUT("recieving signal %d, stopping the instance", signal);
    break;
  }
  util::dmp_helper::SnapshotMem();
  exit(1);
}
#endif

int main() {
#ifdef WIN32
  SetUnhandledExceptionFilter(util::dmp_helper::ExceptionFilter);
#endif
#ifdef linux
  // for linux generating might still need to do
  // bash -c "echo core > /proc/sys/kernel/core_pattern"
  util::dmp_helper::PrepareCore();
#endif
#ifdef unix
#define HandleQuitSignal SIG_DFL
  signal(SIGPIPE, SIG_IGN);
  signal(SIGBUS, HandleQuitSignal);
#endif
  signal(SIGSEGV, HandleQuitSignal);
  signal(SIGABRT, HandleQuitSignal);
  signal(SIGFPE, HandleQuitSignal);
  signal(SIGINT, HandleQuitSignal);
  signal(SIGTERM, HandleQuitSignal);
  signal(SIGILL, HandleQuitSignal);
  sandbox::App app{};
  if (!app.InitGLFW()) {
    LOG_ERROR_COUT("Fail to init glfw component");
    return 1;
  }
  if (!app.InitWindow()) {
    LOG_ERROR_COUT("Fail to init glfw window");
    return 1;
  }
  uint32_t glew_init_stat;
  if ((glew_init_stat = app.InitGLEW()) != GLEW_OK) {
    LOG_ERROR_COUT("fail to init glew: {}",
                   (char *)glewGetErrorString(glew_init_stat));
    return 1;
  }
  return app.Run();
}
