
#include "ThreadPool.h"

namespace script::utils {

ThreadPool::ThreadPool(size_t workerThreads, std::unique_ptr<MessageQueue>&& queue)
    : queue_(std::move(queue)), workers_(workerThreads), threadMutex_() {
  std::lock_guard<std::mutex> lg(threadMutex_);

  if (!queue_) {
    queue_ = std::make_unique<MessageQueue>();
  }

  for (auto& w : workers_) {
    w = std::make_unique<std::thread>([this]() {
      while (this->queue_->loopQueue() != MessageQueue::LoopReturnType::kShutDown) {
      }
    });
  }
}

ThreadPool::~ThreadPool() { shutdownNow(true); }

size_t ThreadPool::workerCount() { return workers_.size(); }

void ThreadPool::removeMessage(int32_t id) { queue_->removeMessage(id); }

void ThreadPool::shutdown(bool awaitTermination) {
  queue_->shutdown(awaitTermination);
  if (awaitTermination) {
    joinWorkers();
  }
}

void ThreadPool::shutdownNow(bool awaitTermination) {
  queue_->shutdownNow(awaitTermination);
  if (awaitTermination) {
    joinWorkers();
  }
}

void ThreadPool::awaitTermination() {
  queue_->awaitTermination();
  joinWorkers();
}

void ThreadPool::joinWorkers() {
  std::lock_guard<std::mutex> lg(threadMutex_);
  for (auto& w : workers_) {
    if (w->joinable()) {
      w->join();
    }
  }
}

}  // namespace script::utils
