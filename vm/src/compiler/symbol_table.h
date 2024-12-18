#include <map>

struct SymbolTable {
  enum class SymbolType {
    GLOBAL,
    LOCAL,
    RERERENCE,
    FREE,
    NOT_FOUND
  };
  SymbolTable* parent_;
  std::map<std::string, SymbolType> table_;

  SymbolTable() : parent_(nullptr) {}
  SymbolTable(SymbolTable* parent) : parent_(parent) {}

  void insert(const std::string& name, SymbolType type) {
    table_[name] = type;
  }

  SymbolType lookup_locally(const std::string& name) {
    if (table_.find(name) != table_.end()) {
      return table_[name];
    }
    return SymbolType::NOT_FOUND;
  }

  SymbolType lookup(const std::string& name) {
    if (table_.find(name) != table_.end()) {
      SymbolType type = table_[name];
      return type;
    }
    SymbolType retrieved_type;
    if (parent_ == nullptr) {
      return SymbolType::NOT_FOUND;
    }
    retrieved_type = parent_->lookup(name);
    if (retrieved_type == SymbolType::GLOBAL) {
      return SymbolType::GLOBAL;
    } else {
      return SymbolType::FREE;
    }
  }
};


