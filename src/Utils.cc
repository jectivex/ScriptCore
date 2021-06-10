
#include "Utils.h"
#include <atomic>

namespace script {

Tracer::Delegate* Tracer::delegate_;

void Tracer::setDelegate(Tracer::Delegate* d) {
  delegate_ = d;
  // use atomic to insert a memory barrier here.
  std::atomic_int().store(0, std::memory_order_release);
}

Tracer::Tracer(ScriptEngine* engine, const char* traceName) noexcept : engine_(engine) {
  if (delegate_) {
    delegate_->beginTrace(engine, traceName);
  }
}

Tracer::Tracer(ScriptEngine* engine, const std::string& traceName) noexcept
    : Tracer(engine, traceName.c_str()) {}

Tracer::~Tracer() {
  if (delegate_) {
    delegate_->endTrace(engine_);
  }
}

Logger::Delegate* Logger::delegate_ = nullptr;

void Logger::log(const std::string& msg) {
  if (delegate_) {
    delegate_->log(msg);
  }
}

void Logger::log(const char* msg) {
  if (delegate_) {
    delegate_->log(msg);
  }
}

void Logger::log(std::string_view msg) {
  if (delegate_) {
    delegate_->log(msg);
  }
}

void Logger::setDelegate(Delegate* delegate) {
  delegate_ = delegate;
  // use atomic to insert a memory barrier here.
  std::atomic_int().store(0, std::memory_order_release);
}

Logger::~Logger() {
  if (delegate_) {
    delegate_->log(str());
  }
}

}  // namespace script
