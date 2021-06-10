
#include <ScriptCore/ScriptCore.h>

#include <utility>

namespace script::internal {

Local<Value> getNamespaceObject(ScriptEngine* engine, const std::string_view& nameSpace,
                                Local<Value> rootNs) {
  Local<Value> nameSpaceObj = std::move(rootNs);
  if (!nameSpace.empty()) {
    std::size_t begin = 0;
    while (begin < nameSpace.size()) {
      auto index = nameSpace.find('.', begin);
      if (index == std::string::npos) {
        index = nameSpace.size();
      }

      StackFrameScope stack;
      auto hasNs = !nameSpaceObj.isNull();

      auto key = String::newString(nameSpace.substr(begin, index - begin));
      auto obj = hasNs ? nameSpaceObj.asObject().get(key) : engine->get(key);
      if (obj.isNull()) {
        // new plain object
        obj = Object::newObject();
        if (hasNs) {
          nameSpaceObj.asObject().set(key, obj);
        } else {
          engine->set(key, obj);
        }
      } else if (!obj.isObject()) {
        throw Exception("invalid namespace");
      }

      nameSpaceObj = stack.returnValue(obj);
      begin = index + 1;
    }
  }
  return nameSpaceObj;
}

}  // namespace script::internal
