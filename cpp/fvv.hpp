//================================================================================================================
// Copyright (c) 2023-present Anne Sakitin (Tianwan Ayana).                                                      =
//                                                                                                               =
// Licensed under the F2DLPR License.                                                                            =
//                                                                                                               =
// YOU MAY NOT USE THIS FILE EXCEPT IN COMPLIANCE WITH THE LICENSE.                                              =
// Provided "AS IS", WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,                                               =
// unless required by applicable law or agreed to in writing.                                                    =
//                                                                                                               =
// For details about the F2DLPR License terms and conditions, visit: http://license.fileto.download.             =
//================================================================================================================

#pragma once

#define FVV_API 1

#ifndef __FVV__
#define __FVV__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#if defined(__GNUC__)
#define FVV_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define FVV_INLINE __forceinline
#else
#define FVV_INLINE inline
#endif

class FVV {
  using str = std::string;
  using strv = std::string_view;
  template <typename T>
  using vec = std::vector<T>;

  template <typename _keyType, typename _valueType>
  class PairList {
public:
    class Pair {
  public:
      FVV_INLINE Pair(void) : _key(), _value() {}
      FVV_INLINE Pair(const _keyType& key, const _valueType& value) : _key(key), _value(value) {}
      FVV_INLINE Pair(const Pair& other) : _key(other.key()), _value(other.value()) {}
      FVV_INLINE Pair(Pair&& other) noexcept : _key(other.key_rv()), _value(other.value_rv()) {}
      FVV_INLINE Pair& operator=(const Pair& other) {
        if (std::addressof(other) != this) {
          _key = other.key();
          _value = other.value();
        }
        return *this;
      }
      FVV_INLINE Pair& operator=(Pair&& other) noexcept {
        if (std::addressof(other) != this) {
          _key = other.key();
          _value = other.value();
        }
        return *this;
      }
      FVV_INLINE bool operator==(const Pair& other) const {
        if (std::addressof(other) == this)
          return true;
        return (_key == other.key() && _value == other.value());
      }
      FVV_INLINE bool operator!=(const Pair& other) const {
        if (std::addressof(other) == this)
          return false;
        return (_key != other.key() || _value != other.value());
      }
      FVV_INLINE _keyType& key(void) { return _key; }
      FVV_INLINE const _keyType& key(void) const { return _key; }
      FVV_INLINE _valueType& value(void) { return _value; }
      FVV_INLINE const _valueType& value(void) const { return _value; }
      FVV_INLINE _keyType&& key_rv(void) { return std::move(_key); }
      FVV_INLINE _valueType&& value_rv(void) { return std::move(_value); }

  private:
      _keyType _key;
      _valueType _value;
    };
    typedef typename std::vector<Pair>::iterator iterator;
    typedef typename std::vector<Pair>::const_iterator const_iterator;
    FVV_INLINE PairList(void) : _data() {}
    FVV_INLINE PairList(const PairList& other) : _data(other.data()) {}
    FVV_INLINE PairList(PairList&& other) noexcept : _data(other.data_rv()) {}
    FVV_INLINE ~PairList(void) {}
    FVV_INLINE PairList& operator=(const PairList& other) {
      if (std::addressof(other) != this)
        _data = other.data();
      return *this;
    }
    FVV_INLINE PairList& operator=(PairList&& other) noexcept {
      if (std::addressof(other) != this)
        _data = other.data_rv();
      return *this;
    }
    FVV_INLINE bool operator==(const PairList& other) const {
      if (std::addressof(other) == this)
        return true;
      return (other.data() == _data);
    }
    FVV_INLINE bool operator!=(const PairList& other) const {
      if (std::addressof(other) == this)
        return false;
      return (other.data() != _data);
    }
    FVV_INLINE _valueType& operator[](const _keyType& key) {
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        if (iter->key() == key)
          return iter->value();
      _data.emplace_back(key, _valueType());
      return _data.back().value();
    }
    FVV_INLINE _keyType& operator()(const _valueType& value) {
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        if (iter->value() == value)
          return iter->key();
      _data.emplace_back(_keyType(), value);
      return _data.back().key();
    }
    FVV_INLINE bool hasKey(const _keyType& key) const {
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        if (iter->key() == key)
          return true;
      return false;
    }
    FVV_INLINE bool hasValue(const _valueType& value) const {
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        if (iter->value() == value)
          return true;
      return false;
    }
    FVV_INLINE iterator begin(void) { return _data.begin(); }
    FVV_INLINE const_iterator begin(void) const { return _data.begin(); }
    FVV_INLINE iterator end(void) { return _data.end(); }
    FVV_INLINE const_iterator end(void) const { return _data.end(); }
    FVV_INLINE Pair& front(void) { return _data.front(); }
    FVV_INLINE const Pair& front(void) const { return _data.front(); }
    FVV_INLINE Pair& back(void) { return _data.back(); }
    FVV_INLINE const Pair& back(void) const { return _data.back(); }
    FVV_INLINE const_iterator findKey(const _keyType& key) const {
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        if (iter->key() == key)
          return iter;
      return _data.end();
    }
    FVV_INLINE const_iterator findValue(const _valueType& value) const {
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        if (iter->value() == value)
          return iter;
      return _data.end();
    }
    FVV_INLINE void add(const _keyType& key, const _valueType& value) {
      for (auto& pair : _data)
        if (pair.key() == key) {
          pair.value() = value;
          return;
        }
      _data.emplace_back(key, value);
    }
    FVV_INLINE void add(const Pair& other) {
      for (auto& pair : _data)
        if (pair.key() == other.key()) {
          pair.value() = other.value();
          return;
        }
      _data.emplace_back(other);
    }
    FVV_INLINE void remove(const_iterator iter) { _data.erase(iter); }
    FVV_INLINE void remove(iterator iter) { _data.erase(iter); }
    FVV_INLINE void removeKey(const _keyType& key) {
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        if (iter->key() == key) {
          _data.erase(iter);
          break;
        }
    }
    FVV_INLINE void removeValue(const _valueType& value) {
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        if (iter->value() == value) {
          _data.erase(iter);
          break;
        }
    }
    FVV_INLINE std::vector<_keyType> keys(void) const {
      std::vector<_keyType> keysList{};
      keysList.reserve(_data.size());
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        keysList.emplace_back(iter->key());
      return keysList;
    }
    FVV_INLINE std::vector<_valueType> values(void) const {
      std::vector<_valueType> valuesList{};
      valuesList.reserve(_data.size());
      for (auto iter = _data.begin(); iter < _data.end(); ++iter)
        valuesList.emplace_back(iter->value());
      return valuesList;
    }
    FVV_INLINE const std::vector<Pair>& data(void) const { return _data; }
    FVV_INLINE std::vector<Pair>&& data_rv(void) { return std::move(_data); }
    FVV_INLINE void sort(std::function<bool(const Pair&, const Pair&)> compare_func = nullptr) {
      static const std::function<bool(const Pair&, const Pair&)> default_comp =
          [](const Pair& a, const Pair& b) -> bool { return (a.key() < b.key()); };
      if (compare_func)
        std::sort(_data.begin(), _data.end(), compare_func);
      else
        std::sort(_data.begin(), _data.end(), default_comp);
    }
    FVV_INLINE void reverse(void) { std::reverse(_data.begin(), _data.end()); }
    FVV_INLINE void clear(void) { _data.clear(); }
    FVV_INLINE size_t size(void) const noexcept { return _data.size(); }
    FVV_INLINE bool empty(void) const noexcept { return (_data.begin() == _data.end()); }

private:
    std::vector<Pair> _data;
  };

  public:
  static constexpr const bool defaultBool = false;
  static constexpr const int defaultInt = 0;
  static constexpr const double defaultDouble = 0.0;
  static const str defaultString;
  static const vec<bool> defaultBools;
  static const vec<int> defaultInts;
  static const vec<double> defaultDoubles;
  static const vec<str> defaultStrings;
  struct FVVV {
    using FVVVT = std::variant<std::monostate, bool, int, double, str, vec<bool>, vec<int>, vec<double>, vec<str>>;
    FVVVT value;
    PairList<str, FVVV> children = {};
    str desc = "";
    FVVV* link = nullptr;
    str linkName = "";
    FVV_INLINE FVVV(void) = default;
    FVV_INLINE FVVV(bool v) : value(v) {}
    FVV_INLINE FVVV(int v) : value(v) {}
    FVV_INLINE FVVV(double v) : value(v) {}
    FVV_INLINE FVVV(const strv& v) : value(v.data()) {}
    FVV_INLINE FVVV(const vec<bool>& v) : value(v) {}
    FVV_INLINE FVVV(const vec<int>& v) : value(v) {}
    FVV_INLINE FVVV(const vec<double>& v) : value(v) {}
    FVV_INLINE FVVV(const vec<str>& v) : value(v) {}
    FVV_INLINE FVVV& operator[](const strv& key) { return children[key.data()]; }
    /// @brief  以bool类型返回值
    /// @return 值
    FVV_INLINE bool asBool(void) const {
      using resultType = bool;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultBool);
    }
    /// @brief  以int类型返回值
    /// @return 值
    FVV_INLINE int asInt(void) const {
      using resultType = int;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultInt);
    }
    /// @brief  以double类型返回值
    /// @return 值
    FVV_INLINE double asDouble(void) const {
      using resultType = double;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultDouble);
    }
    /// @brief  以string类型返回值
    /// @return 值
    FVV_INLINE const str asString(void) const {
      using resultType = str;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultString);
    }
    /// @brief  以vector<bool>类型返回值
    /// @return 值
    FVV_INLINE const vec<bool> asBools(void) const {
      using resultType = vec<bool>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultBools);
    }
    /// @brief  以vector<int>类型返回值
    /// @return 值
    FVV_INLINE const vec<int> asInts(void) const {
      using resultType = vec<int>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultInts);
    }
    /// @brief  以vector<double>类型返回值
    /// @return 值
    FVV_INLINE const vec<double> asDoubles(void) const {
      using resultType = vec<double>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultDoubles);
    }
    /// @brief  以vector<string>类型返回值
    /// @return 值
    FVV_INLINE const vec<str> asStrings(void) const {
      using resultType = vec<str>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultStrings);
    }
    /// @brief  判断值是否为空
    /// @return 值为空时返回true，否则为false
    FVV_INLINE bool isEmpty(void) const {
      return !isLink() && std::holds_alternative<std::monostate>(value) && children.empty();
    }
    /// @brief  判断值是否为非空
    /// @return 值为非空时返回true，否则为false
    FVV_INLINE bool isNotEmpty(void) const {
      return isLink() || !std::holds_alternative<std::monostate>(value) || !children.empty();
    }
    template <typename T>
    /// @brief  判断值是否为指定类型
    /// @param  类型
    /// @return 值为指定类型时返回true，否则为false
    FVV_INLINE bool isType(void) const {
      return isNotEmpty() && isLink() ? std::holds_alternative<T>(link->value) : std::holds_alternative<T>(value);
    }
    template <typename T>
    /// @brief  以指定类型返回值
    /// @param  类型
    /// @return 值为指定类型时返回值，否则为nullopt
    FVV_INLINE std::optional<T> as(void) const {
      if (isLink() ? link->isType<T>() : isType<T>())
        return isLink() ? std::get<T>(link->value) : std::get<T>(value);
      else
        return std::nullopt;
    }
    /// @brief  判断值是否有描述
    /// @return 值有描述时返回true，否则为false
    FVV_INLINE bool hasDesc(void) const { return !desc.empty(); }
    /// @brief  返回值的描述
    /// @return 值有描述时返回描述，否则为空字符串
    FVV_INLINE const str& getDesc(void) const { return desc; }
    /// @brief  设置值的描述
    /// @param  描述
    FVV_INLINE void setDesc(const strv& newDesc) {
      desc = newDesc;
      _shrink(&desc);
    }
    /// @brief  删除值的描述
    FVV_INLINE void delDesc(void) { _clearAndShrink(&desc); }
    /// @brief  判断值是否为链接
    /// @return 值为链接时返回true，否则为false
    FVV_INLINE bool isLink(void) const { return link; }
    /// @brief  返回值的链接
    /// @return 值为链接时返回链接的指针，否则为nullopt
    FVV_INLINE FVVV& getLink(void) const { return *link; }
    /// @brief  返回值的链接的名称
    /// @return 值为链接时返回链接的名称，否则为空字符串
    FVV_INLINE const str& getLinkName(void) const { return linkName; }
    /// @brief  设置值的链接
    /// @param  指针
    FVV_INLINE void setLink(FVVV* newLink) { link = newLink; }
    /// @brief  设置值的链接的名称
    /// @param  名称
    FVV_INLINE void setLinkName(const strv& newlinkName) {
      linkName = newlinkName;
      _shrink(&linkName);
    }
    /// @brief  删除值的链接与其名称
    FVV_INLINE void delLink(void) {
      link = nullptr;
      _clearAndShrink(&linkName);
    }
    /// @brief  将值的链接转为值
    FVV_INLINE void link2Real(void) {
      value = link->value;
      link = nullptr;
      _clearAndShrink(&linkName);
    }
    /// @brief  输出格式化后的值
    /// @param  为“common”或空时正常输出
    /// @param  为“min”时最小化输出
    /// @param  为“biglist”时会把值组内每个值换行输出
    /// @return 格式化后的值
    FVV_INLINE str print(const strv& type = "common") const {
      str result;
      std::function<void(const str&, const FVVV*, size_t)> printFunc;
      printFunc = [&](const str& path, const FVVV* node, size_t indentLevel) {
        str indent(indentLevel * 2, ' ');
        if (!node->children.empty() && !path.empty()) {
          if (type == "min")
            result += path + "={";
          else
            result += indent + path + " = {\n";
        }
        if (node->children.empty() && node->isNotEmpty()) {
          if (type == "min")
            result += path + '=';
          else
            result += indent + path + " = ";
          if (node->isLink()) {
            result += node->getLinkName();
          }
          else if (node->isType<str>()) {
            result += '"';
            str tmpStr = node->as<str>().value();
            _utf8ForEach(tmpStr, tmpStr.size(),
                         [&result]([[maybe_unused]] const size_t& index, const strv& index_char,
                                   [[maybe_unused]] const uint8_t& char_size) -> bool {
                           if (index_char == "\"")
                             result += '\\';
                           result += index_char;
                           return false;
                         });
            result += '"';
          }
          else if (node->isType<bool>())
            result += node->as<bool>().value() ? "true" : "false";
          else if (node->isType<int>())
            result += std::to_string(node->as<int>().value());
          else if (node->isType<double>())
            result += std::to_string(node->as<double>().value());
          else if (node->isType<vec<str>>()) {
            result += '[';
            str vecIndent((indentLevel + 1) * 2, ' ');
            if (type == "biglist")
              result += '\n';
            const vec<str> tmp = node->as<vec<str>>().value();
            for (const str& value : tmp) {
              if (type == "biglist")
                result += vecIndent;
              result += '"';
              _utf8ForEach(value, value.size(),
                           [&result]([[maybe_unused]] const size_t& index, const strv& index_char,
                                     [[maybe_unused]] const uint8_t& char_size) -> bool {
                             if (index_char == "\"")
                               result += '\\';
                             result += index_char;
                             return false;
                           });
              result += "\",";
              if (type == "biglist")
                result += '\n';
              else if (type != "min")
                result += ' ';
            }
            if (type != "biglist") {
              result.pop_back();
              if (type != "min")
                result.pop_back();
            }
            else if (type == "biglist")
              result += indent;
            result += ']';
          }
          else if (node->isType<vec<bool>>()) {
            result += '[';
            str vecIndent((indentLevel + 1) * 2, ' ');
            if (type == "biglist")
              result += '\n';
            const vec<bool> tmp = node->as<vec<bool>>().value();
            for (const int& value : tmp) {
              if (type == "biglist")
                result += vecIndent;
              result += str(value ? "true" : "false") + ",";
              if (type == "biglist")
                result += '\n';
              else if (type != "min")
                result += ' ';
            }
            if (type != "biglist") {
              result.pop_back();
              if (type != "min")
                result.pop_back();
            }
            else if (type == "biglist")
              result += indent;
            result += ']';
          }
          else if (node->isType<vec<int>>()) {
            result += '[';
            str vecIndent((indentLevel + 1) * 2, ' ');
            if (type == "biglist")
              result += '\n';
            const vec<int> tmp = node->as<vec<int>>().value();
            for (const int& value : tmp) {
              if (type == "biglist")
                result += vecIndent;
              result += std::to_string(value) + ',';
              if (type == "biglist")
                result += '\n';
              else if (type != "min")
                result += ' ';
            }
            if (type != "biglist") {
              result.pop_back();
              if (type != "min")
                result.pop_back();
            }
            else if (type == "biglist")
              result += indent;
            result += ']';
          }
          else if (node->isType<vec<double>>()) {
            result += '[';
            str vecIndent((indentLevel + 1) * 2, ' ');
            if (type == "biglist")
              result += '\n';
            const vec<double> tmp = node->as<vec<double>>().value();
            for (const double& value : tmp) {
              if (type == "biglist")
                result += vecIndent;
              result += std::to_string(value) + ',';
              if (type == "biglist")
                result += '\n';
              else if (type != "min")
                result += ' ';
            }
            if (type != "biglist") {
              result.pop_back();
              if (type != "min")
                result.pop_back();
            }
            else if (type == "biglist")
              result += indent;
            result += ']';
          }
          if (!node->getDesc().empty() && type != "min" && type != "nodesc") {
            result += " <";
            str tmpStr = node->getDesc();
            _utf8ForEach(tmpStr, tmpStr.size(),
                         [&result]([[maybe_unused]] const size_t& index, const strv& index_char,
                                   [[maybe_unused]] const uint8_t& char_size) -> bool {
                           if (index_char == ">")
                             result += '\\';
                           result += index_char;
                           return false;
                         });
            result += '>';
          }
          if (type == "min")
            result += ';';
          else
            result += '\n';
        }
        else
          for (const PairList<str, FVVV>::Pair& item : node->children)
            printFunc(item.key(), &item.value(), indentLevel + 1);
        if (!node->children.empty() && !path.empty()) {
          if (type == "min")
            result += '}';
          else
            result += indent + '}';
          if (!node->getDesc().empty() && type != "min" && type != "nodesc") {
            result += " <";
            str tmpStr = node->getDesc();
            _utf8ForEach(tmpStr, tmpStr.size(),
                         [&result]([[maybe_unused]] const size_t& index, const strv& index_char,
                                   [[maybe_unused]] const uint8_t& char_size) -> bool {
                           if (index_char == ">")
                             result += '\\';
                           result += index_char;
                           return false;
                         });
            result += '>';
          }
          if (type == "min")
            result += ';';
          else
            result += '\n';
        }
      };
      for (const PairList<str, FVVV>::Pair& item : children)
        printFunc(item.key(), &item.value(), 0);
      result.pop_back();
      _shrink(&result);
      return result;
    }
  };
  class Parser {
public:
    /// @brief           解析字符串为FVVV
    /// @param txt       FVV文本格式的字符串
    /// @param targetFvv 在外部定义好了的FVVV
    static FVV_INLINE void ReadString(str txt, FVVV& targetFvv) {
      if (txt.size() >= 3 && static_cast<unsigned char>(txt[0]) == _bom[0] &&
          static_cast<unsigned char>(txt[1]) == _bom[1] && static_cast<unsigned char>(txt[2]) == _bom[2])
        txt = txt.substr(3);
      size_t start = txt.find_first_not_of(" \t\r\n");
      txt = start == str::npos ? "" : txt.substr(start);
      if (txt.empty())
        return;
      if (txt.back() != '}' && txt.back() != '\n')
        txt += '\n';
      _replace(txt, "\r\n", "\n");
      _replace(txt, "\r", "\n");
      _shrink(&txt);
      str desc, index_desc, value, valueName;
      vec<str> groupNames, valueNames, values;
      vec<vec<str>> lastGroupNames;
      bool endGroup = false, first = false, inValue = false, inDesc = false, inStr = false, inList = false,
           isRealChar = false, isStr = false, isList = false;
      size_t inGroup = 0;
      uint8_t last_char_size = 0;
      _utf8ForEach(txt, txt.size(), [&](const size_t& index, const strv& index_char, const uint8_t& char_size) -> bool {
        FVVV* index_key = &targetFvv;
        isRealChar = index >= 1 ? (last_char_size == 1 ? (txt[index - 1] != '\\' ? true : false) : true) : true;
        last_char_size = char_size;
        if (inDesc) {
          if (index_char != ">" || !isRealChar) {
            if (index_char == ">" && !isRealChar) {
              index_desc.pop_back();
              _shrink(&index_desc);
            }
            if (inValue || inGroup > 0)
              index_desc += index_char;
            return false;
          }
          else if (index_char == ">" && isRealChar) {
            desc = index_desc;
            _clearAndShrink(&index_desc);
            _shrink(&desc);
            inDesc = false;
            return false;
          }
        }
        else {
          if (!inStr && (index_char == " " || index_char == "\t" || index_char == "\r"))
            return false;
          else if (index_char == "<") {
            inDesc = true;
            return false;
          }
        }
        if (inValue) {
          if (inStr) {
            if (index_char == "\"") {
              if (isRealChar) {
                inStr = false;
                return false;
              }
              else {
                value.pop_back();
                _shrink(&value);
                value += index_char;
                return false;
              }
            }
            else {
              value += index_char;
              return false;
            }
          }
          else {
            if (index_char == "\"") {
              inStr = isStr = true;
              return false;
            }
            else if (index_char == "[") {
              inList = isList = true;
              return false;
            }
            else if (inList && _eq_or(index_char, strv(","), strv("]"), strv("\n"))) {
              switch (index_char[0]) {
              case '\n':
                return false;
              case ']': {
                inList = false;
                size_t j = 1;
                while (_eq_or(txt[index - j], ' ', '\t', '\r', '\n'))
                  ++j;
                if (txt[index - j] == ',')
                  return false;
                else
                  break;
              }
              }
              values.push_back(value);
              _clearAndShrink(&value);
              return false;
            }
            else if (index_char == "{") {
              groupNames.insert(groupNames.end(), valueNames.begin(), valueNames.end());
              lastGroupNames.push_back(valueNames);
              _clearAndShrink(&valueNames);
              ++inGroup;
              inValue = false;
              return false;
            }
            else if (!inList && _eq_or(index_char, strv(";"), strv("\n"))) {
              for (size_t i = 0; i < groupNames.size(); ++i)
                index_key = &(*index_key)[groupNames[i]];
              for (size_t i = 0; i < valueNames.size(); ++i) {
                const str& key = valueNames[i];
                if (i == valueNames.size() - 1) {
                  if (isList) {
                    if (isStr)
                      (*index_key)[key] = FVVV(values);
                    else {
                      if (values.empty()) {
                        (*index_key)[key] = FVVV();
                        return false;
                      }
                      str tmpStr = values.front();
                      if (_eq_or(tmpStr, str("true"), str("false"))) {
                        vec<bool> tmp;
                        tmp.reserve(values.size());
                        std::transform(values.begin(), values.end(), std::back_inserter(tmp),
                                       [](const strv& s) { return s == "true"; });
                        (*index_key)[key] = FVVV(tmp);
                      }
                      else if (_isInt(tmpStr)) {
                        vec<int> tmp;
                        for (const str& str : values)
                          if (_isInt(str))
                            tmp.push_back(std::stoi(str));
                        (*index_key)[key] = FVVV(tmp);
                      }
                      else if (_isDouble(tmpStr)) {
                        vec<double> tmp;
                        for (const str& str : values)
                          if (_isDouble(str))
                            tmp.push_back(std::stod(str));
                        (*index_key)[key] = FVVV(tmp);
                      }
                    }
                  }
                  else {
                    if (isStr)
                      (*index_key)[key] = FVVV(value);
                    else if (_eq_or(value, str("true"), str("false")))
                      (*index_key)[key] = FVVV(value == "true");
                    else if (_isInt(value))
                      (*index_key)[key] = FVVV(std::stoi(value));
                    else if (_isDouble(value))
                      (*index_key)[key] = FVVV(std::stod(value));
                    else {
                      vec<str> tmpName = _split(value, '.');
                      FVVV* tmpValue = index_key;
                      for (size_t i = 0; i < tmpName.size(); ++i)
                        if (!tmpValue->children.hasKey(tmpName[i]))
                          continue;
                        else
                          tmpValue = &(*tmpValue)[tmpName[i]];
                      if ((*tmpValue).isNotEmpty())
                        (*index_key)[key].setLink(tmpValue);
                      else {
                        tmpValue = &targetFvv;
                        for (size_t i = 0; i < tmpName.size(); ++i)
                          if (!tmpValue->children.hasKey(tmpName[i]))
                            continue;
                          else
                            tmpValue = &(*tmpValue)[tmpName[i]];
                        if ((*tmpValue).isNotEmpty())
                          (*index_key)[key].setLink(tmpValue);
                      }
                      (*index_key)[key].setLinkName(value);
                    }
                  }
                  (*index_key)[key].setDesc(desc);
                  _clearAndShrink(&desc, &value, &values, &valueNames);
                  isList = isStr = inValue = false;
                  continue;
                }
                else
                  index_key = &(*index_key)[key];
              }
            }
            else {
              value += index_char;
              return false;
            }
          }
        }
        else {
          if (!first && index_char == "{" && valueName.empty()) {
            first = true;
            return false;
          }
          if (index_char == "=") {
            valueNames = _split(valueName, '.');
            _clearAndShrink(&valueName);
            inValue = true;
            return false;
          }
          else if (endGroup && _eq_or(index_char, strv(";"), strv("\n")) && inGroup > 0) {
            endGroup = false;
            if (!desc.empty())
              for (size_t i = 0; i < groupNames.size(); ++i) {
                if (i == groupNames.size() - 1) {
                  (*index_key)[groupNames[i]].setDesc(desc);
                  _clearAndShrink(&desc);
                  break;
                }
                index_key = &(*index_key)[groupNames[i]];
              }
            for ([[maybe_unused]] const str& _ : lastGroupNames.back())
              groupNames.pop_back();
            lastGroupNames.pop_back();
            _shrink(&groupNames, &lastGroupNames);
            inGroup--;
            return false;
          }
          else if (index_char == "}")
            if (inGroup == 0)
              return true;
            else {
              endGroup = true;
              return false;
            }
          else if (index_char != "}" && index_char != ";") {
            valueName += index_char;
            return false;
          }
        }
        return false;
      });
    }

private:
    static constexpr const unsigned char _bom[] = {0xEF, 0xBB, 0xBF};
    template <typename T>
    static FVV_INLINE bool _eq_or(T a, T b) {
      return (a == b);
    }
    template <typename T, typename... Args>
    static FVV_INLINE bool _eq_or(T a, T b, Args... args) {
      return (a == b) || _eq_or(a, args...);
    }
    static FVV_INLINE vec<str> _split(const str& path, char delimiter) {
      size_t start = path.find_first_not_of("\n");
      size_t end = path.find_last_not_of("\n");
      if (start == str::npos || end == str::npos)
        return {};
      str target = path.substr(start, end - start + 1);
      vec<str> result;
      std::stringstream ss(target);
      str item;
      while (std::getline(ss, item, delimiter))
        result.push_back(item);
      return result;
    }
    static FVV_INLINE void _replace(str& s, const strv& f, const strv& t) {
      size_t p = 0;
      while ((p = s.find(f, p)) != str::npos) {
        s.replace(p, f.length(), t);
        p += t.length();
      }
    }
  };
  static FVV_INLINE bool _isInt(const strv& s) {
    if (s.empty())
      return false;
    size_t start = 0;
    if (s[0] == '-' || s[0] == '+') {
      if (s.size() == 1)
        return false;
      start = 1;
    }
    return std::all_of(s.begin() + start, s.end(), ::isdigit);
  }
  static FVV_INLINE bool _isDouble(const strv& s) {
    if (s.empty())
      return false;
    size_t start = 0;
    bool hasDigit = false, hasDot = false;
    if (s[0] == '-' || s[0] == '+') {
      if (s.size() == 1)
        return false;
      start = 1;
    }
    for (size_t i = start; i < s.size(); ++i) {
      char c = s[i];
      if (std::isdigit(c))
        hasDigit = true;
      else if (c == '.') {
        if (hasDot)
          return false;
        hasDot = true;
      }
      else
        return false;
    }
    return hasDigit;
  }

  private:
  template <typename T>
  static FVV_INLINE void _shrink(T* container) {
    if (container)
      container->shrink_to_fit();
  }
  template <typename T, typename... Args>
  static FVV_INLINE void _shrink(T* container, Args... args) {
    _shrink(container);
    _shrink(args...);
  }
  template <typename T>
  static FVV_INLINE void _clearAndShrink(T* container) {
    if (container) {
      container->clear();
      container->shrink_to_fit();
    }
  }
  template <typename T, typename... Args>
  static FVV_INLINE void _clearAndShrink(T* container, Args... args) {
    _clearAndShrink(container);
    _clearAndShrink(args...);
  }
  static FVV_INLINE void _utf8ForEach(const str& target, size_t size,
                                      std::function<bool(const size_t&, const strv&, const uint8_t&)> handler) {
    size_t i = 0;
    while (i < size) {
      unsigned char c = target[i];
      uint8_t char_size = 0;
      if ((c & 0x80) == 0)
        char_size = 1;
      else if ((c & 0xE0) == 0xC0)
        char_size = 2;
      else if ((c & 0xF0) == 0xE0)
        char_size = 3;
      else if ((c & 0xF8) == 0xF0)
        char_size = 4;
      else
        char_size = 1;
      if (handler(i, target.substr(i, char_size), char_size))
        break;
      i += char_size;
    }
  }
};

const std::string FVV::defaultString = "";
const std::vector<bool> FVV::defaultBools = {};
const std::vector<int> FVV::defaultInts = {};
const std::vector<double> FVV::defaultDoubles = {};
const std::vector<std::string> FVV::defaultStrings = {};

#endif
