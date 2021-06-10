
#pragma once

#include <memory>
#include <string_view>
#include "foundation.h"
#include SCRIPTCORE_BACKEND(Engine.h)

namespace script {

class InspectorAgent {
 public:
  InspectorAgent() : sendToEngine_() {}

  SCRIPTCORE_DISALLOW_COPY_AND_MOVE(InspectorAgent);

  virtual ~InspectorAgent() = default;

  /**
   * Engine call this method to send message to debugger.
   * override this method.
   * @param utf8
   */
  virtual void sendMessageToDebugger(const std::string_view& utf8) = 0;

  /**
   * Call this method to dispatch debugger message to Engine.
   * This method can only be called after a ScriptInspector is created using this agent.
   * @param utf8
   */
  void sendMessageToEngine(const std::string_view& utf8) { sendToEngine_(utf8); }

 private:
  /**
   * Engine install a listener, so that debugger can dispatch message to Engine.
   * @param listener
   */
  void setOnDebuggerMessageListener(std::function<void(const std::string_view& utf8)>&& listener) {
    sendToEngine_ = std::move(listener);
  }

  std::function<void(const std::string_view& utf8)> sendToEngine_;

  friend class ScriptInspector;
};

class ScriptInspector {
 public:
  ScriptInspector() = default;

  SCRIPTCORE_DISALLOW_COPY_AND_MOVE(ScriptInspector);

  virtual ~ScriptInspector() = default;

  struct Options {
    std::string_view debugName;
    /**
     * pause engine and wait for debugger to connect.
     */
    bool waitForDebugger;
  };

  static std::unique_ptr<ScriptInspector> create(script::ScriptEngine* engine,
                                                 std::unique_ptr<InspectorAgent> agent,
                                                 const Options& options = {});

 protected:
  // helper method
  static void setListenerToInspectorAgent(
      InspectorAgent* agent, std::function<void(const std::string_view& utf8)> listener) {
    agent->setOnDebuggerMessageListener(std::move(listener));
  }
};

}  // namespace script
