#pragma once

#define FVV_API 1

#ifndef __FVV__
#define __FVV__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
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
    std::map<str, FVVV> children = {};
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
    FVV_INLINE bool asBool(void) const {
      using resultType = bool;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultBool);
    }
    FVV_INLINE int asInt(void) const {
      using resultType = int;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultInt);
    }
    FVV_INLINE double asDouble(void) const {
      using resultType = double;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultDouble);
    }
    FVV_INLINE const str asString(void) const {
      using resultType = str;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultString);
    }
    FVV_INLINE const vec<bool> asBools(void) const {
      using resultType = vec<bool>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultBools);
    }
    FVV_INLINE const vec<int> asInts(void) const {
      using resultType = vec<int>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultInts);
    }
    FVV_INLINE const vec<double> asDoubles(void) const {
      using resultType = vec<double>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultDoubles);
    }
    FVV_INLINE const vec<str> asStrings(void) const {
      using resultType = vec<str>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultStrings);
    }
    FVV_INLINE bool isEmpty(void) const {
      return !isLink() && std::holds_alternative<std::monostate>(value) && children.empty();
    }
    FVV_INLINE bool isNotEmpty(void) const {
      return isLink() || !std::holds_alternative<std::monostate>(value) || !children.empty();
    }
    template <typename T>
    FVV_INLINE bool isType(void) const {
      return isNotEmpty() && isLink() ? std::holds_alternative<T>(link->value) : std::holds_alternative<T>(value);
    }
    template <typename T>
    FVV_INLINE std::optional<T> as(void) const {
      if (isLink() ? link->isType<T>() : isType<T>())
        return isLink() ? std::get<T>(link->value) : std::get<T>(value);
      else
        return std::nullopt;
    }
    FVV_INLINE bool hasDesc(void) const { return !desc.empty(); }
    FVV_INLINE const str& getDesc(void) const { return desc; }
    FVV_INLINE void setDesc(const strv& newDesc) {
      desc = newDesc;
      _shrink(&desc);
    }
    FVV_INLINE void delDesc(void) { _clearAndShrink(&desc); }
    FVV_INLINE bool isLink(void) const { return link; }
    FVV_INLINE FVVV& getLink(void) const { return *link; }
    FVV_INLINE const str& getLinkName(void) const { return linkName; }
    FVV_INLINE void setLink(FVVV* newLink) { link = newLink; }
    FVV_INLINE void setLinkName(const strv& newlinkName) {
      linkName = newlinkName;
      _shrink(&linkName);
    }
    FVV_INLINE void delLink(void) {
      link = nullptr;
      _clearAndShrink(&linkName);
    }
    FVV_INLINE void link2Real(void) {
      value = link->value;
      link = nullptr;
      _clearAndShrink(&linkName);
    }
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
          for (const auto& [key, child] : node->children)
            printFunc(key, &child, indentLevel + 1);
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
      for (const auto& [key, child] : children)
        printFunc(key, &child, 0);
      result.pop_back();
      _shrink(&result);
      return result;
    }
  };
  class Parser {
public:
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
                      for (size_t i = 0; i < tmpName.size(); ++i) {
                        if (tmpValue->children.find(tmpName[i]) == tmpValue->children.end())
                          continue;
                        tmpValue = &(*tmpValue)[tmpName[i]];
                      }
                      if ((*tmpValue).isNotEmpty())
                        (*index_key)[key].setLink(tmpValue);
                      else {
                        tmpValue = &targetFvv;
                        for (size_t i = 0; i < tmpName.size(); ++i) {
                          if (tmpValue->children.find(tmpName[i]) == tmpValue->children.end())
                            continue;
                          tmpValue = &(*tmpValue)[tmpName[i]];
                        }
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
            for ([[maybe_unused]] const auto& _ : lastGroupNames.back())
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
