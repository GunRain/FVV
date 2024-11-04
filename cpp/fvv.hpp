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

public:
  static INLINE constexpr const bool defaultBool = false;
  static INLINE constexpr const int defaultInt = 0;
  static INLINE constexpr const double defaultDouble = 0.0;
  static INLINE const std::string defaultString = "";
  static INLINE const std::vector<bool> defaultBools = {};
  static INLINE const std::vector<int> defaultInts = {};
  static INLINE const std::vector<double> defaultDoubles = {};
  static INLINE const std::vector<std::string> defaultStrings = {};
  struct FVVV
  {
    using FVVVT = std::variant<std::monostate, bool, int, double, std::string, std::vector<bool>, std::vector<int>, std::vector<double>, std::vector<std::string>>;
    FVVVT value;
    std::map<std::string, FVVV> children = {};
    std::string desc = "";
    FVVV *link = nullptr;
    std::string linkName = "";
    INLINE FVVV(void) = default;
    INLINE FVVV(bool v) : value(v) {}
    INLINE FVVV(int v) : value(v) {}
    INLINE FVVV(double v) : value(v) {}
    INLINE FVVV(const std::string_view &v) : value(v.data()) {}
    INLINE FVVV(const std::vector<bool> &v) : value(v) {}
    INLINE FVVV(const std::vector<int> &v) : value(v) {}
    INLINE FVVV(const std::vector<double> &v) : value(v) {}
    INLINE FVVV(const std::vector<std::string> &v) : value(v) {}
    INLINE FVVV &operator[](const std::string_view &key)
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
    INLINE const std::string &asString(void) const
    {
      using resultType = std::string;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultString);
    }
    INLINE const std::vector<bool> &asBools(void) const
    {
      using resultType = std::vector<bool>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultBools);
    }
    INLINE const std::vector<int> &asInts(void) const
    {
      using resultType = std::vector<int>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultInts);
    }
    INLINE const std::vector<double> &asDoubles(void) const
    {
      using resultType = std::vector<double>;
      std::optional<resultType> result = isLink() ? link->as<resultType>() : as<resultType>();
      return result.value_or(defaultDoubles);
    }
    INLINE const std::vector<std::string> &asStrings(void) const
    {
      using resultType = std::vector<std::string>;
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
    INLINE const std::string &getDesc(void) const
    {
      return desc;
    }
    INLINE void setDesc(const std::string_view &newDesc)
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
    INLINE const std::string &getLinkName(void) const
    {
      return linkName;
    }
    INLINE void setLink(FVVV *newLink)
    {
      link = newLink;
    }
    INLINE void setLinkName(const std::string_view &newlinkName)
    {
      linkName = newlinkName;
      _shrink(&linkName);
    }
    INLINE void delLink(void)
    {
      link = nullptr;
      _clearAndShrink(&linkName);
    }
    INLINE std::string print(const std::string_view &type = "common") const
    {
      std::string result;
      result += std::string("{");
      if (type != std::string_view("min"))
        result += std::string("\n");
      std::function<void(const std::string &, const FVVV *, int)> printFunc;
      printFunc = [&](const std::string &path, const FVVV *node, int indentLevel)
      {
        std::string indent(indentLevel * 2, ' ');
        if (!node->children.empty() && !path.empty())
        {
          if (type == std::string_view("min"))
            result += path + std::string("={");
          else
            result += indent + path + std::string(" = {\n");
        }
        if (node->children.empty() && node->isNotEmpty())
        {
          if (type == std::string_view("min"))
            result += path + std::string("=");
          else
            result += indent + path + std::string(" = ");
          if (node->isLink())
          {
            result += node->getLinkName();
          }
          else if (node->isType<std::string>())
          {
            result += std::string("\"");
            std::string index_char, tmpStr = node->as<std::string>().value();
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
              if (index_char == std::string("\""))
                result += std::string("\\");
              result += index_char;
              i += char_size;
            }
            result += std::string("\"");
          }
          else if (node->isType<bool>())
            result += node->as<bool>().value() ? std::string("true") : std::string("false");
          else if (node->isType<int>())
            result += std::to_string(node->as<int>().value());
          else if (node->isType<double>())
            result += std::to_string(node->as<double>().value());
          else if (node->isType<std::vector<std::string>>())
          {
            result += std::string("[");
            const std::vector<std::string> tmp = node->as<std::vector<std::string>>().value();
            for (const std::string &value : tmp)
            {
              result += std::string("\"");
              std::string index_char;
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
                if (index_char == std::string("\""))
                  result += std::string("\\");
                result += index_char;
                i += char_size;
              }
              result += std::string("\",");
              if (type != std::string_view("min"))
                result += std::string(" ");
            }
            if (type != std::string_view("min"))
              result.pop_back();
            result.pop_back();
            result += std::string("]");
          }
          else if (node->isType<std::vector<bool>>())
          {
            result += std::string("[");
            const std::vector<bool> tmp = node->as<std::vector<bool>>().value();
            for (const int &value : tmp)
            {
              result += value ? std::string("true") : std::string("false") + std::string(",");
              if (type != std::string_view("min"))
                result += std::string(" ");
            }
            if (type != std::string_view("min"))
              result.pop_back();
            result.pop_back();
            result += std::string("]");
          }
          else if (node->isType<std::vector<int>>())
          {
            result += std::string("[");
            const std::vector<int> tmp = node->as<std::vector<int>>().value();
            for (const int &value : tmp)
            {
              result += std::to_string(value) + std::string(",");
              if (type != std::string_view("min"))
                result += std::string(" ");
            }
            if (type != std::string_view("min"))
              result.pop_back();
            result.pop_back();
            result += std::string("]");
          }
          else if (node->isType<std::vector<double>>())
          {
            result += std::string("[");
            const std::vector<double> tmp = node->as<std::vector<double>>().value();
            for (const double &value : tmp)
            {
              result += std::to_string(value) + std::string(",");
              if (type != std::string_view("min"))
                result += std::string(" ");
            }
            if (type != std::string_view("min"))
              result.pop_back();
            result.pop_back();
            result += std::string("]");
          }
          if (!node->getDesc().empty() && type != std::string_view("min"))
          {
            result += std::string(" <");
            std::string index_char, tmpStr = node->getDesc();
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
              if (index_char == std::string(">"))
                result += std::string("\\");
              result += index_char;
              i += char_size;
            }
            result += std::string(">");
          }
          result += std::string(";");
          if (type != std::string_view("min"))
            result += std::string("\n");
        }
        else
          for (const auto &[key, child] : node->children)
            printFunc(key, &child, indentLevel + 1);
        if (!node->children.empty() && !path.empty())
        {
          if (type == std::string_view("min"))
            result += std::string("}");
          else
            result += indent + std::string("}");
          if (!node->getDesc().empty() && type != std::string_view("min"))
          {
            result += std::string(" <");
            std::string index_char, tmpStr = node->getDesc();
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
              if (index_char == std::string(">"))
                result += std::string("\\");
              result += index_char;
              i += char_size;
            }
            result += std::string(">");
          }
          result += std::string(";");
          if (type != std::string_view("min"))
            result += std::string("\n");
        }
      };
      printFunc(std::string(""), this, 0);
      result += std::string("}");
      _shrink(&result);
      return result;
    }
  };
  class Parser
  {
  public:
    static INLINE void ReadString(std::string txt, FVVV &targetFvv)
    {
      if (txt.size() >= 3 &&
          static_cast<unsigned char>(txt[0]) == _bom[0] &&
          static_cast<unsigned char>(txt[1]) == _bom[1] &&
          static_cast<unsigned char>(txt[2]) == _bom[2])
        txt = txt.substr(3);
      size_t start = std::string::npos;
      size_t end = std::string::npos;
      for (size_t i = 0; i < txt.size(); ++i)
        if (txt[i] == '{' && (i == 0 || txt[i - 1] != '\\'))
        {
          start = i;
          break;
        }
      if (start == std::string::npos)
        return;
      for (size_t i = txt.size(); i-- > 0;)
        if (txt[i] == '}' && (i == 0 || txt[i - 1] != '\\'))
        {
          end = i;
          break;
        }
      if (end == std::string::npos || end <= start)
        return;
      txt = txt.substr(start + 1, end - start - 1);
      start = txt.find_first_not_of(" \t\r\n");
      txt = start == std::string::npos ? "" : txt.substr(start);
      if (txt.empty())
        return;
      txt.shrink_to_fit();
      std::string desc, index_char, index_desc, value, valueName;
      std::vector<std::string> groupNames, valueNames, values;
      std::vector<std::vector<std::string>> lastGroupNames;
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
          if (index_char != std::string(">") || !isRealChar)
          {
            if (index_char == std::string(">") && !isRealChar)
            {
              index_desc.pop_back();
              _shrink(&index_desc);
            }
            if (inValue || inGroup > 0)
              index_desc += index_char;
            continue;
          }
          else if (index_char == std::string(">") && isRealChar)
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
          if (!inStr && (index_char == std::string(" ") || index_char == std::string("\t") || index_char == std::string("\r") || index_char == std::string("\n")))
            continue;
          else if (index_char == std::string("<"))
          {
            inDesc = true;
            continue;
          }
        }
        if (inValue)
        {
          if (inStr)
          {
            if (index_char == std::string("\""))
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
            if (index_char == std::string("\""))
            {
              inStr = isStr = true;
              continue;
            }
            else if (index_char == std::string("["))
            {
              isList = true;
              continue;
            }
            else if (index_char == std::string(",") || index_char == std::string("]"))
            {
              if (index_char == std::string("]"))
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
            else if (index_char == std::string("{"))
            {
              groupNames.insert(groupNames.end(), valueNames.begin(), valueNames.end());
              lastGroupNames.push_back(valueNames);
              _clearAndShrink(&valueNames);
              inGroup++;
              inValue = false;
              continue;
            }
            else if (index_char == std::string(";"))
            {
              for (size_t i = 0; i < groupNames.size(); ++i)
                index_key = &(*index_key)[groupNames[i]];
              for (size_t i = 0; i < valueNames.size(); ++i)
              {
                const std::string &key = valueNames[i];
                if (i == valueNames.size() - 1)
                {
                  if (isList)
                  {
                    if (isStr)
                      (*index_key)[key] = FVVV(values);
                    else
                    {
                      std::string tmpStr = values.front();
                      if (tmpStr == std::string("true") || tmpStr == std::string("false"))
                      {
                        std::vector<bool> tmp;
                        tmp.reserve(values.size());
                        std::transform(values.begin(), values.end(), std::back_inserter(tmp), [](const std::string_view &str)
                                       { return str == std::string_view("true"); });
                        (*index_key)[key] = FVVV(tmp);
                      }
                      else if (_isInt(tmpStr))
                      {
                        std::vector<int> tmp;
                        for (const std::string &str : values)
                          if (_isInt(str))
                            tmp.push_back(std::stoi(str));
                        (*index_key)[key] = FVVV(tmp);
                      }
                      else if (_isDouble(tmpStr))
                      {
                        std::vector<double> tmp;
                        for (const std::string &str : values)
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
                    else if (value == std::string("true") || value == std::string("false"))
                      (*index_key)[key] = FVVV(value == std::string("true"));
                    else if (_isInt(value))
                      (*index_key)[key] = FVVV(std::stoi(value));
                    else if (_isDouble(value))
                      (*index_key)[key] = FVVV(std::stod(value));
                    else
                    {
                      std::vector<std::string> tmpName = _split(value, '.');
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
          if (index_char == std::string("="))
          {
            valueNames = _split(valueName, '.');
            _clearAndShrink(&valueName);
            inValue = true;
            continue;
          }
          else if (index_char == std::string(";") && inGroup > 0)
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
          else if (index_char == std::string("}") && inGroup == 0)
            break;
          else if (index_char != std::string("}") && index_char != std::string(";"))
          {
            valueName += index_char;
            continue;
          }
        }
      }
    }

  private:
    static INLINE constexpr const unsigned char _bom[] = {0xEF, 0xBB, 0xBF};
    static INLINE std::vector<std::string> _split(const std::string &path, char delimiter)
    {
      std::vector<std::string> result;
      std::stringstream ss(path);
      std::string item;
      while (std::getline(ss, item, delimiter))
        result.push_back(item);
      return result;
    }
  };
  static INLINE bool _isInt(const std::string &str)
  {
    if (str.empty())
      return false;
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+')
    {
      if (str.size() == 1)
        return false;
      start = 1;
    }
    return std::all_of(str.begin() + start, str.end(), ::isdigit);
  }
  static INLINE bool _isDouble(const std::string &str)
  {
    if (str.empty())
      return false;
    size_t start = 0;
    bool hasDecimal = false;
    bool hasDigit = false;
    if (str[0] == '-' || str[0] == '+')
    {
      if (str.size() == 1)
        return false;
      start = 1;
    }
    for (size_t i = start; i < str.size(); ++i)
    {
      char c = str[i];
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