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

class FVV
{

#if defined(__GNUC__)
#define INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define INLINE __forceinline
#else
#define INLINE inline
#endif

#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline)) inline
#elif defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE __attribute__((noinline))
#endif

  using str = std::string;
  using strv = std::string_view;
  template <typename T>
  using vec = std::vector<T>;

public:
  static INLINE constexpr const bool defaultBool = false;
  static INLINE constexpr const int defaultInt = 0;
  static INLINE constexpr const double defaultDouble = 0.0;
  static INLINE const str defaultString = "";
  static INLINE const vec<bool> defaultBools = {};
  static INLINE const vec<int> defaultInts = {};
  static INLINE const vec<double> defaultDoubles = {};
  static INLINE const vec<str> defaultStrings = {};
  struct FVVV
  {
    using FVVVT = std::variant<std::monostate, bool, int, double, str, vec<bool>, vec<int>, vec<double>, vec<str>>;
    FVVVT value;
    std::map<str, FVVV> children = {};
    str desc = "";
    FVVV *link = nullptr;
    str linkName = "";
    INLINE FVVV(void) = default;
    INLINE FVVV(bool v) : value(v) {}
    INLINE FVVV(int v) : value(v) {}
    INLINE FVVV(double v) : value(v) {}
    INLINE FVVV(const strv &v) : value(v.data()) {}
    INLINE FVVV(const vec<bool> &v) : value(v) {}
    INLINE FVVV(const vec<int> &v) : value(v) {}
    INLINE FVVV(const vec<double> &v) : value(v) {}
    INLINE FVVV(const vec<str> &v) : value(v) {}
    INLINE FVVV &operator[](const strv &key)
    {
      return children[key.data()];
    }
    INLINE bool asBool(void) const
    {
      using resultType = bool;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultBool);
    }
    INLINE int asInt(void) const
    {
      using resultType = int;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultInt);
    }
    INLINE double asDouble(void) const
    {
      using resultType = double;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultDouble);
    }
    INLINE const str &asString(void) const
    {
      using resultType = str;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultString);
    }
    INLINE const vec<bool> &asBools(void) const
    {
      using resultType = vec<bool>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultBools);
    }
    INLINE const vec<int> &asInts(void) const
    {
      using resultType = vec<int>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultInts);
    }
    INLINE const vec<double> &asDoubles(void) const
    {
      using resultType = vec<double>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultDoubles);
    }
    INLINE const vec<str> &asStrings(void) const
    {
      using resultType = vec<str>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultStrings);
    }
    INLINE bool isEmpty(void) const
    {
      return !isLink() && std::holds_alternative<std::monostate>(value);
    }
    INLINE bool isNotEmpty(void) const
    {
      return isLink() || !std::holds_alternative<std::monostate>(value);
    }
    template <typename T>
    INLINE bool isType(void) const
    {
      return isNotEmpty() && isLink() ? std::holds_alternative<T>(link->value) : std::holds_alternative<T>(value);
    }
    template <typename T>
    INLINE std::optional<T> as(void) const
    {
      if (isLink() ? link->isType<T>() : isType<T>())
        return isLink() ? std::get<T>(link->value) : std::get<T>(value);
      else
        return std::nullopt;
    }
    INLINE bool hasDesc(void) const
    {
      return !desc.empty();
    }
    INLINE const str &getDesc(void) const
    {
      return desc;
    }
    INLINE void setDesc(const strv &newDesc)
    {
      desc = newDesc;
      _shrink(&desc);
    }
    INLINE void delDesc(void)
    {
      _clearAndShrink(&desc);
    }
    INLINE bool isLink(void) const
    {
      return link != nullptr;
    }
    INLINE FVVV &getLink(void) const
    {
      return *link;
    }
    INLINE const str &getLinkName(void) const
    {
      return linkName;
    }
    INLINE void setLink(FVVV *newLink)
    {
      link = newLink;
    }
    INLINE void setLinkName(const strv &newlinkName)
    {
      linkName = newlinkName;
      _shrink(&linkName);
    }
    INLINE void delLink(void)
    {
      link = nullptr;
      _clearAndShrink(&linkName);
    }
    INLINE void link2Real(void)
    {
      value = link->value;
      link = nullptr;
      _clearAndShrink(&linkName);
    }
    INLINE str print(const strv &type = "common") const
    {
      str result;
      result += str("{");
      if (type != strv("min"))
        result += str("\n");
      std::function<void(const str &, const FVVV *, int)> printFunc;
      printFunc = [&](const str &path, const FVVV *node, int indentLevel)
      {
        str indent(indentLevel * 2, ' ');
        if (!node->children.empty() && !path.empty())
        {
          if (type == strv("min"))
            result += path + str("={");
          else
            result += indent + path + str(" = {\n");
        }
        if (node->children.empty() && node->isNotEmpty())
        {
          if (type == strv("min"))
            result += path + str("=");
          else
            result += indent + path + str(" = ");
          if (node->isLink())
          {
            result += node->getLinkName();
          }
          else if (node->isType<str>())
          {
            result += str("\"");
            str index_char, tmpStr = node->as<str>().value();
            for (size_t i = 0; i < tmpStr.size();)
            {
              unsigned char c = tmpStr[i];
              size_t char_size = 0;
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
              index_char = tmpStr.substr(i, char_size);
              if (index_char == str("\""))
                result += str("\\");
              result += index_char;
              i += char_size;
            }
            result += str("\"");
          }
          else if (node->isType<bool>())
            result += node->as<bool>().value() ? str("true") : str("false");
          else if (node->isType<int>())
            result += std::to_string(node->as<int>().value());
          else if (node->isType<double>())
            result += std::to_string(node->as<double>().value());
          else if (node->isType<vec<str>>())
          {
            result += str("[");
            const vec<str> tmp = node->as<vec<str>>().value();
            for (const str &value : tmp)
            {
              result += str("\"");
              str index_char;
              for (size_t i = 0; i < value.size();)
              {
                unsigned char c = value[i];
                size_t char_size = 0;
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
                index_char = value.substr(i, char_size);
                if (index_char == str("\""))
                  result += str("\\");
                result += index_char;
                i += char_size;
              }
              result += str("\",");
              if (type != strv("min"))
                result += str(" ");
            }
            if (type != strv("min"))
              result.pop_back();
            result.pop_back();
            result += str("]");
          }
          else if (node->isType<vec<bool>>())
          {
            result += str("[");
            const vec<bool> tmp = node->as<vec<bool>>().value();
            for (const int &value : tmp)
            {
              result += value ? str("true") : str("false") + str(",");
              if (type != strv("min"))
                result += str(" ");
            }
            if (type != strv("min"))
              result.pop_back();
            result.pop_back();
            result += str("]");
          }
          else if (node->isType<vec<int>>())
          {
            result += str("[");
            const vec<int> tmp = node->as<vec<int>>().value();
            for (const int &value : tmp)
            {
              result += std::to_string(value) + str(",");
              if (type != strv("min"))
                result += str(" ");
            }
            if (type != strv("min"))
              result.pop_back();
            result.pop_back();
            result += str("]");
          }
          else if (node->isType<vec<double>>())
          {
            result += str("[");
            const vec<double> tmp = node->as<vec<double>>().value();
            for (const double &value : tmp)
            {
              result += std::to_string(value) + str(",");
              if (type != strv("min"))
                result += str(" ");
            }
            if (type != strv("min"))
              result.pop_back();
            result.pop_back();
            result += str("]");
          }
          if (!node->getDesc().empty() && type != strv("min") && type != strv("nodesc"))
          {
            result += str(" <");
            str index_char, tmpStr = node->getDesc();
            for (size_t i = 0; i < tmpStr.size();)
            {
              unsigned char c = tmpStr[i];
              size_t char_size = 0;
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
              index_char = tmpStr.substr(i, char_size);
              if (index_char == str(">"))
                result += str("\\");
              result += index_char;
              i += char_size;
            }
            result += str(">");
          }
          result += str(";");
          if (type != strv("min"))
            result += str("\n");
        }
        else
          for (const auto &[key, child] : node->children)
            printFunc(key, &child, indentLevel + 1);
        if (!node->children.empty() && !path.empty())
        {
          if (type == strv("min"))
            result += str("}");
          else
            result += indent + str("}");
          if (!node->getDesc().empty() && type != strv("min") && type != strv("nodesc"))
          {
            result += str(" <");
            str index_char, tmpStr = node->getDesc();
            for (size_t i = 0; i < tmpStr.size();)
            {
              unsigned char c = tmpStr[i];
              size_t char_size = 0;
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
              index_char = tmpStr.substr(i, char_size);
              if (index_char == str(">"))
                result += str("\\");
              result += index_char;
              i += char_size;
            }
            result += str(">");
          }
          result += str(";");
          if (type != strv("min"))
            result += str("\n");
        }
      };
      printFunc(str(""), this, 0);
      result += str("}");
      _shrink(&result);
      return result;
    }
  };
  class Parser
  {
  public:
    static INLINE void ReadString(str txt, FVVV &targetFvv)
    {
      if (txt.size() >= 3 &&
          static_cast<unsigned char>(txt[0]) == _bom[0] &&
          static_cast<unsigned char>(txt[1]) == _bom[1] &&
          static_cast<unsigned char>(txt[2]) == _bom[2])
        txt = txt.substr(3);
      size_t start = str::npos;
      size_t end = str::npos;
      for (size_t i = 0; i < txt.size(); ++i)
        if (txt[i] == '{' && (i == 0 || txt[i - 1] != '\\'))
        {
          start = i;
          break;
        }
      if (start == str::npos)
        return;
      for (size_t i = txt.size(); i-- > 0;)
        if (txt[i] == '}' && (i == 0 || txt[i - 1] != '\\'))
        {
          end = i;
          break;
        }
      if (end == str::npos || end <= start)
        return;
      txt = txt.substr(start + 1, end - start - 1);
      start = txt.find_first_not_of(" \t\r\n");
      txt = start == str::npos ? "" : txt.substr(start);
      if (txt.empty())
        return;
      txt.shrink_to_fit();
      str desc, index_char, index_desc, value, valueName;
      vec<str> groupNames, valueNames, values;
      vec<vec<str>> lastGroupNames;
      bool inDesc = false, inStr = false, inValue = false, isList, isRealChar, isStr;
      uint64_t inGroup = 0;
      for (size_t i = 0; i < txt.size();)
      {
        unsigned char c = txt[i];
        size_t char_size = 0;
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
        index_char = txt.substr(i, char_size);
        i += char_size;
        FVVV *index_key = &targetFvv;
        isRealChar = i >= char_size + 1 ? (txt[i - char_size - 1] != '\\' ? true : false) : true;
        if (inDesc)
        {
          if (index_char != str(">") || !isRealChar)
          {
            if (index_char == str(">") && !isRealChar)
            {
              index_desc.pop_back();
              _shrink(&index_desc);
            }
            if (inValue || inGroup > 0)
              index_desc += index_char;
            continue;
          }
          else if (index_char == str(">") && isRealChar)
          {
            desc = index_desc;
            _clearAndShrink(&index_desc);
            _shrink(&desc);
            inDesc = false;
            continue;
          }
        }
        else
        {
          if (!inStr && (index_char == str(" ") || index_char == str("\t") || index_char == str("\r") || index_char == str("\n")))
            continue;
          else if (index_char == str("<"))
          {
            inDesc = true;
            continue;
          }
        }
        if (inValue)
        {
          if (inStr)
          {
            if (index_char == str("\""))
            {
              if (isRealChar)
              {
                inStr = false;
                continue;
              }
              else
              {
                value.pop_back();
                _shrink(&value);
                value += index_char;
                continue;
              }
            }
            else
            {
              value += index_char;
              continue;
            }
          }
          else
          {
            if (index_char == str("\""))
            {
              inStr = isStr = true;
              continue;
            }
            else if (index_char == str("["))
            {
              isList = true;
              continue;
            }
            else if (index_char == str(",") || index_char == str("]"))
            {
              if (index_char == str("]"))
              {
                size_t j = 1;
                while (txt[i - char_size - j] == ' ' || txt[i - char_size - j] == '\t' || txt[i - char_size - j] == '\r' || txt[i - char_size - j] == '\n')
                  j++;
                if (txt[i - char_size - j] == ',')
                  continue;
              }
              values.push_back(value);
              _clearAndShrink(&value);
              continue;
            }
            else if (index_char == str("{"))
            {
              groupNames.insert(groupNames.end(), valueNames.begin(), valueNames.end());
              lastGroupNames.push_back(valueNames);
              _clearAndShrink(&valueNames);
              inGroup++;
              inValue = false;
              continue;
            }
            else if (index_char == str(";"))
            {
              for (size_t i = 0; i < groupNames.size(); ++i)
                index_key = &(*index_key)[groupNames[i]];
              for (size_t i = 0; i < valueNames.size(); ++i)
              {
                const str &key = valueNames[i];
                if (i == valueNames.size() - 1)
                {
                  if (isList)
                  {
                    if (isStr)
                      (*index_key)[key] = FVVV(values);
                    else
                    {
                      str tmpStr = values.front();
                      if (tmpStr == str("true") || tmpStr == str("false"))
                      {
                        vec<bool> tmp;
                        tmp.reserve(values.size());
                        std::transform(values.begin(), values.end(), std::back_inserter(tmp), [](const strv &s)
                                       { return s == strv("true"); });
                        (*index_key)[key] = FVVV(tmp);
                      }
                      else if (_isInt(tmpStr))
                      {
                        vec<int> tmp;
                        for (const str &str : values)
                          if (_isInt(str))
                            tmp.push_back(std::stoi(str));
                        (*index_key)[key] = FVVV(tmp);
                      }
                      else if (_isDouble(tmpStr))
                      {
                        vec<double> tmp;
                        for (const str &str : values)
                          if (_isDouble(str))
                            tmp.push_back(std::stod(str));
                        (*index_key)[key] = FVVV(tmp);
                      }
                    }
                  }
                  else
                  {
                    if (isStr)
                      (*index_key)[key] = FVVV(value);
                    else if (value == str("true") || value == str("false"))
                      (*index_key)[key] = FVVV(value == str("true"));
                    else if (_isInt(value))
                      (*index_key)[key] = FVVV(std::stoi(value));
                    else if (_isDouble(value))
                      (*index_key)[key] = FVVV(std::stod(value));
                    else
                    {
                      vec<str> tmpName = _split(value, '.');
                      FVVV *tmpValue = index_key;
                      for (size_t i = 0; i < tmpName.size(); ++i)
                      {
                        if (tmpValue->children.find(tmpName[i]) == tmpValue->children.end())
                          continue;
                        tmpValue = &(*tmpValue)[tmpName[i]];
                      }
                      if ((*tmpValue).isNotEmpty())
                        (*index_key)[key].setLink(tmpValue);
                      else
                      {
                        tmpValue = &targetFvv;
                        for (size_t i = 0; i < tmpName.size(); ++i)
                        {
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
            else
            {
              value += index_char;
              continue;
            }
          }
        }
        else
        {
          if (index_char == str("="))
          {
            valueNames = _split(valueName, '.');
            _clearAndShrink(&valueName);
            inValue = true;
            continue;
          }
          else if (index_char == str(";") && inGroup > 0)
          {
            if (!desc.empty())
              for (size_t i = 0; i < groupNames.size(); ++i)
              {
                if (i == groupNames.size() - 1)
                {
                  (*index_key)[groupNames[i]].setDesc(desc);
                  _clearAndShrink(&desc);
                  break;
                }
                index_key = &(*index_key)[groupNames[i]];
              }
            for (const auto &_ : lastGroupNames.back())
              groupNames.pop_back();
            lastGroupNames.pop_back();
            _shrink(&groupNames, &lastGroupNames);
            inGroup--;
            continue;
          }
          else if (index_char == str("}") && inGroup == 0)
            break;
          else if (index_char != str("}") && index_char != str(";"))
          {
            valueName += index_char;
            continue;
          }
        }
      }
    }

  private:
    static INLINE constexpr const unsigned char _bom[] = {0xEF, 0xBB, 0xBF};
    static INLINE vec<str> _split(const str &path, char delimiter)
    {
      vec<str> result;
      std::stringstream ss(path);
      str item;
      while (std::getline(ss, item, delimiter))
        result.push_back(item);
      return result;
    }
  };
  static INLINE bool _isInt(const str &s)
  {
    if (s.empty())
      return false;
    size_t start = 0;
    if (s[0] == '-' || s[0] == '+')
    {
      if (s.size() == 1)
        return false;
      start = 1;
    }
    return std::all_of(s.begin() + start, s.end(), ::isdigit);
  }
  static INLINE bool _isDouble(const str &s)
  {
    if (s.empty())
      return false;
    size_t start = 0;
    bool hasDecimal = false;
    bool hasDigit = false;
    if (s[0] == '-' || s[0] == '+')
    {
      if (s.size() == 1)
        return false;
      start = 1;
    }
    for (size_t i = start; i < s.size(); ++i)
    {
      char c = s[i];
      if (std::isdigit(c))
        hasDigit = true;
      else if (c == '.')
      {
        if (hasDecimal)
          return false;
        hasDecimal = true;
      }
      else
        return false;
    }
    return hasDigit;
  }

private:
  template <typename T>
  static INLINE void _shrink(T *container)
  {
    if (container)
      container->shrink_to_fit();
  }
  template <typename T, typename... Args>
  static INLINE void _shrink(T *container, Args... args)
  {
    _shrink(container);
    _shrink(args...);
  }
  template <typename T>
  static INLINE void _clearAndShrink(T *container)
  {
    if (container)
    {
      container->clear();
      container->shrink_to_fit();
    }
  }
  template <typename T, typename... Args>
  static INLINE void _clearAndShrink(T *container, Args... args)
  {
    _clearAndShrink(container);
    _clearAndShrink(args...);
  }
};

#endif