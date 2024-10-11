#pragma once

#define FVV_API 1

#ifndef __FVV__
#define __FVV__

#include <algorithm>
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
  static INLINE constexpr const std::string defaultString = "";
  static INLINE constexpr const std::vector<bool> defaultVectorBool = {};
  static INLINE constexpr const std::vector<int> defaultVectorInt = {};
  static INLINE constexpr const std::vector<double> defaultVectorDouble = {};
  static INLINE constexpr const std::vector<std::string> defaultVectorString = {};
  struct FVVV
  {
    using FVVVT = std::variant<std::monostate, bool, int, double, std::string, std::vector<bool>, std::vector<int>, std::vector<double>, std::vector<std::string>>;
    FVVVT value;
    std::map<std::string, FVVV> children;
    std::string desc;
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
      return isType<bool>() ? std::get<bool>(value) : defaultBool;
    }
    INLINE int asInt(void) const
    {
      return isType<int>() ? std::get<int>(value) : defaultInt;
    }
    INLINE double asDouble(void) const
    {
      return isType<double>() ? std::get<double>(value) : defaultDouble;
    }
    INLINE const std::string &asString(void) const
    {
      return isType<std::string>() ? std::get<std::string>(value) : defaultString;
    }
    INLINE const std::vector<bool> &asBools(void) const
    {
      return isType<bool>() ? std::get<std::vector<bool>>(value) : defaultVectorBool;
    }
    INLINE const std::vector<int> &asInts(void) const
    {
      return isType<std::vector<int>>() ? std::get<std::vector<int>>(value) : defaultVectorInt;
    }
    INLINE const std::vector<double> &asDoubles(void) const
    {
      return isType<std::vector<double>>() ? std::get<std::vector<double>>(value) : defaultVectorDouble;
    }
    INLINE const std::vector<std::string> &asStrings(void) const
    {
      return isType<std::vector<std::string>>() ? std::get<std::vector<std::string>>(value) : defaultVectorString;
    }
    INLINE bool isEmpty(void) const
    {
      return std::holds_alternative<std::monostate>(value);
    }
    template <typename T>
    INLINE bool isType(void) const
    {
      return !isEmpty() && std::holds_alternative<T>(value);
    }
    template <typename T>
    INLINE std::optional<T> as(void) const
    {
      if (isType<T>())
        return std::get<T>(value);
      else
        return std::nullopt;
    }
    INLINE const std::string &getDesc(void) const
    {
      return desc;
    }
    INLINE void setDesc(const std::string_view &newDesc)
    {
      desc = newDesc;
    }
    INLINE std::string print(const std::string_view &type = "common") const
    {
      std::string result;
      if (type == std::string_view("min"))
        result += std::string("{");
      else
        result += std::string("{\n");
      std::function<void(const std::string &, const FVVV *, int)> printFunc;
      printFunc = [&](const std::string &path, const FVVV *node, int indentLevel)
      {
        bool hasChildren = !node->children.empty();
        std::string indent(indentLevel * 2, ' ');
        if (hasChildren && !path.empty())
        {
          if (type == std::string_view("min"))
            result += path + std::string("={");
          else
            result += indent + path + std::string(" = {\n");
        }
        if (node->children.empty())
        {
          if (type == std::string_view("min"))
            result += path + std::string("=");
          else
            result += indent + path + std::string(" = ");
          if (node->isType<std::string>())
            result += std::string("\"") + node->as<std::string>().value() + std::string("\"");
          else if (node->isType<int>())
            result += std::to_string(node->as<int>().value());
          else if (node->isType<double>())
            result += std::to_string(node->as<double>().value());
          else if (node->isType<std::vector<std::string>>())
          {
            result += std::string("[");
            const std::vector<std::string> tmp = node->as<std::vector<std::string>>().value();
            for (const std::string &value : tmp)
              if (type == std::string_view("min"))
                result += std::string("\"") + value + std::string("\",");
              else
                result += std::string("\"") + value + std::string("\", ");
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
              if (type == std::string_view("min"))
                result += std::to_string(value) + std::string(",");
              else
                result += std::to_string(value) + std::string(", ");
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
              if (type == std::string_view("min"))
                result += std::to_string(value) + std::string(",");
              else
                result += std::to_string(value) + std::string(", ");
            if (type != std::string_view("min"))
              result.pop_back();
            result.pop_back();
            result += std::string("]");
          }
          if (!node->getDesc().empty() && type != std::string_view("min"))
            result += std::string(" <") + node->getDesc() + std::string(">");
          if (type == std::string_view("min"))
            result += std::string(";");
          else
            result += std::string(";\n");
        }
        else
        {
          for (const auto &[key, child] : node->children)
            printFunc(key, &child, indentLevel + 1);
        }
        if (hasChildren && !path.empty())
        {
          if (type == std::string_view("min"))
            result += std::string("}");
          else
            result += indent + std::string("}");
          if (!node->getDesc().empty() && type != std::string_view("min"))
            result += std::string(" <") + node->getDesc() + std::string(">");
          if (type == std::string_view("min"))
            result += std::string(";");
          else
            result += std::string(";\n");
        }
      };
      printFunc(std::string(""), this, 0);
      result += std::string("}");
      return result;
    }
  };
  class Parser
  {
  public:
    static INLINE void ReadString(std::string txt, FVVV &cfg)
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
        FVVV *index_key = &cfg;
        isRealChar = i >= char_size + 1 ? (txt[i - char_size - 1] != '\\' ? true : false) : true;
        if (!inDesc && !inStr && (index_char == std::string(" ") || index_char == std::string("\t") || index_char == std::string("\r") || index_char == std::string("\n")))
          continue;
        if (!inDesc && index_char == std::string("<"))
        {
          inDesc = true;
          continue;
        }
        if (inDesc && (index_char != std::string(">") || !isRealChar))
        {
          if (index_char == std::string(">") && !isRealChar)
            index_desc.pop_back();
          if (inValue || inGroup > 0)
            index_desc += index_char;
          continue;
        }
        if (inDesc && index_char == std::string(">") && isRealChar)
        {
          desc = index_desc;
          index_desc.clear();
          index_desc.shrink_to_fit();
          inDesc = false;
          continue;
        }
        if (!inValue && index_char == std::string("="))
        {
          valueNames = _split(valueName, '.');
          valueName.clear();
          valueName.shrink_to_fit();
          inValue = true;
          continue;
        }
        if (inValue && !inStr && index_char == std::string("{"))
        {
          groupNames.insert(groupNames.end(), valueNames.begin(), valueNames.end());
          lastGroupNames.push_back(valueNames);
          valueNames.clear();
          valueNames.shrink_to_fit();
          inGroup++;
          inValue = false;
          continue;
        }
        if (inValue && !inStr && index_char == std::string("["))
        {
          isList = true;
          continue;
        }
        if (inValue && !inStr && index_char == std::string("\""))
        {
          inStr = true;
          isStr = true;
          continue;
        }
        if (inValue && inStr && index_char == std::string("\"") && isRealChar)
        {
          inStr = false;
          continue;
        }
        else if (inValue && inStr && index_char == std::string("\"") && !isRealChar)
          value.pop_back();
        if (inValue && !inStr && (index_char == std::string(",") || index_char == std::string("]")))
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
          value.clear();
          value.shrink_to_fit();
          continue;
        }
        if (!inValue && !inStr && index_char == std::string(";") && inGroup > 0)
        {
          if (!desc.empty())
            for (size_t i = 0; i < groupNames.size(); ++i)
            {
              if (i == groupNames.size() - 1)
              {
                (*index_key)[groupNames[i]].setDesc(desc);
                desc.clear();
                desc.shrink_to_fit();
                break;
              }
              index_key = &(*index_key)[groupNames[i]];
            }
          for (const auto &_ : lastGroupNames.back())
            groupNames.pop_back();
          lastGroupNames.pop_back();
          inGroup--;
          continue;
        }
        if (inValue && !inStr && index_char == std::string(";"))
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
                  std::string tmpStr = values.back();
                  if (tmpStr == std::string("true") || tmpStr == std::string("false"))
                  {
                    std::vector<bool> tmp;
                    tmp.reserve(values.size());
                    std::transform(values.begin(), values.end(), std::back_inserter(tmp), [](const std::string_view &str)
                                   { return str == std::string_view("true"); });
                    (*index_key)[key] = FVVV(tmp);
                  }
                  else if (std::all_of(tmpStr.begin(), tmpStr.end(), ::isdigit))
                  {
                    std::vector<int> tmp;
                    tmp.reserve(values.size());
                    std::transform(values.begin(), values.end(), std::back_inserter(tmp), [](const std::string_view &str)
                                   { return std::stoi(str.data()); });
                    (*index_key)[key] = FVVV(tmp);
                  }
                  else
                  {
                    tmpStr.erase(std::remove(tmpStr.begin(), tmpStr.end(), '.'), tmpStr.end());
                    if (std::all_of(tmpStr.begin(), tmpStr.end(), ::isdigit))
                    {
                      std::vector<double> tmp;
                      tmp.reserve(values.size());
                      std::transform(values.begin(), values.end(), std::back_inserter(tmp), [](const std::string_view &str)
                                     { return std::stod(str.data()); });
                      (*index_key)[key] = FVVV(tmp);
                    }
                  }
                }
              }
              else
              {
                if (isStr)
                  (*index_key)[key] = FVVV(value);
                else
                {
                  if (value == std::string("true") || value == std::string("false"))
                    (*index_key)[key] = FVVV(value == std::string("true"));
                  else if (std::all_of(value.begin(), value.end(), ::isdigit))
                    (*index_key)[key] = FVVV(std::stoi(value));
                  else
                  {
                    std::string tmpStr = value;
                    tmpStr.erase(std::remove(tmpStr.begin(), tmpStr.end(), '.'), tmpStr.end());
                    if (std::all_of(tmpStr.begin(), tmpStr.end(), ::isdigit))
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
                      if (!(*tmpValue).isEmpty())
                        (*index_key)[key] = (*tmpValue);
                      else
                      {
                        tmpValue = &cfg;
                        for (size_t i = 0; i < tmpName.size(); ++i)
                        {
                          if (tmpValue->children.find(tmpName[i]) == tmpValue->children.end())
                            continue;
                          tmpValue = &(*tmpValue)[tmpName[i]];
                        }
                        if (!(*tmpValue).isEmpty())
                          (*index_key)[key] = (*tmpValue);
                      }
                    }
                  }
                }
              }
              (*index_key)[key].setDesc(desc);
              desc.clear();
              desc.shrink_to_fit();
              value.clear();
              value.shrink_to_fit();
              values.clear();
              values.shrink_to_fit();
              valueNames.clear();
              valueNames.shrink_to_fit();
              isList = false;
              isStr = false;
              inValue = false;
              continue;
            }
            else
              index_key = &(*index_key)[key];
          }
        }
        if (!inValue && !inStr && index_char == std::string("}") && inGroup == 0)
          break;
        if (inValue)
        {
          value += index_char;
          continue;
        }
        else if (!inValue && index_char != std::string("}") && index_char != std::string(";"))
        {
          valueName += index_char;
          continue;
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
};

#endif