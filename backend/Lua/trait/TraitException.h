
class ExceptionFields {
 public:
  mutable Global<Value> exception_{};

  mutable std::string message_{};
  mutable bool hasMessage_ = false;

  mutable std::string stacktrace_{};
  mutable bool hasStacktrace_ = false;

  void fillMessage() const noexcept;
  void fillStacktrace() const noexcept;
};

}  // namespace lua_backend

template <>
struct internal::ImplType<Exception> {
  using type = lua_backend::ExceptionFields;
};

}  // namespace script
