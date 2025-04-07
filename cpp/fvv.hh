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

#ifdef __GNUC__
#define FVV_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define FVV_INLINE __forceinline
#else
#define FVV_INLINE inline
#endif

class FVV {
public:
    using str  = std::string;
    using strv = std::string_view;
    template <typename T>
    using vec = std::vector<T>;

    template <typename _keyType, typename _valueType>
    class PairList {
    public:
        class Pair {
        public:
            FVV_INLINE       Pair(void) : _key(), _value() {}
            FVV_INLINE       Pair(const _keyType& key, const _valueType& value) : _key(key), _value(value) {}
            FVV_INLINE       Pair(const Pair& other) : _key(other.key()), _value(other.value()) {}
            FVV_INLINE       Pair(Pair&& other) noexcept : _key(other.key_rv()), _value(other.value_rv()) {}
            FVV_INLINE Pair& operator=(const Pair& other) {
                if (std::addressof(other) != this) {
                    _key   = other.key();
                    _value = other.value();
                }
                return *this;
            }
            FVV_INLINE Pair& operator=(Pair&& other) noexcept {
                if (std::addressof(other) != this) {
                    _key   = other.key();
                    _value = other.value();
                }
                return *this;
            }
            FVV_INLINE bool operator==(const Pair& other) const {
                if (std::addressof(other) == this) return true;
                return (_key == other.key() && _value == other.value());
            }
            FVV_INLINE bool operator!=(const Pair& other) const {
                if (std::addressof(other) == this) return false;
                return (_key != other.key() || _value != other.value());
            }
            FVV_INLINE _keyType&         key(void) { return _key; }
            FVV_INLINE const _keyType&   key(void) const { return _key; }
            FVV_INLINE _valueType&       value(void) { return _value; }
            FVV_INLINE const _valueType& value(void) const { return _value; }
            FVV_INLINE _keyType&&        key_rv(void) { return std::move(_key); }
            FVV_INLINE _valueType&&      value_rv(void) { return std::move(_value); }

        private:
            _keyType   _key;
            _valueType _value;
        };
        typedef typename vec<Pair>::iterator       iterator;
        typedef typename vec<Pair>::const_iterator const_iterator;
        FVV_INLINE                                 PairList(void) : _data() {}
        FVV_INLINE                                 PairList(const PairList& other) : _data(other.data()) {}
        FVV_INLINE                                 PairList(PairList&& other) noexcept : _data(other.data_rv()) {}
        FVV_INLINE ~PairList(void) {}
        FVV_INLINE PairList& operator=(const PairList& other) {
            if (std::addressof(other) != this) _data = other.data();
            return *this;
        }
        FVV_INLINE PairList& operator=(PairList&& other) noexcept {
            if (std::addressof(other) != this) _data = other.data_rv();
            return *this;
        }
        FVV_INLINE bool operator==(const PairList& other) const {
            if (std::addressof(other) == this) return true;
            return (other.data() == _data);
        }
        FVV_INLINE bool operator!=(const PairList& other) const {
            if (std::addressof(other) == this) return false;
            return (other.data() != _data);
        }
        FVV_INLINE _valueType& operator[](const _keyType& key) {
            for (auto iter = _data.begin(); iter < _data.end(); ++iter)
                if (iter->key() == key) return iter->value();
            _data.emplace_back(key, _valueType());
            return _data.back().value();
        }
        FVV_INLINE _keyType& operator()(const _valueType& value) {
            for (auto iter = _data.begin(); iter < _data.end(); ++iter)
                if (iter->value() == value) return iter->key();
            _data.emplace_back(_keyType(), value);
            return _data.back().key();
        }
        FVV_INLINE bool hasKey(const _keyType& key) const {
            for (auto iter = _data.begin(); iter < _data.end(); ++iter)
                if (iter->key() == key) return true;
            return false;
        }
        FVV_INLINE bool hasValue(const _valueType& value) const {
            for (auto iter = _data.begin(); iter < _data.end(); ++iter)
                if (iter->value() == value) return true;
            return false;
        }
        FVV_INLINE iterator       begin(void) { return _data.begin(); }
        FVV_INLINE const_iterator begin(void) const { return _data.begin(); }
        FVV_INLINE iterator       end(void) { return _data.end(); }
        FVV_INLINE const_iterator end(void) const { return _data.end(); }
        FVV_INLINE Pair&          front(void) { return _data.front(); }
        FVV_INLINE const Pair&    front(void) const { return _data.front(); }
        FVV_INLINE Pair&          back(void) { return _data.back(); }
        FVV_INLINE const Pair&    back(void) const { return _data.back(); }
        FVV_INLINE const_iterator findKey(const _keyType& key) const {
            for (auto iter = _data.begin(); iter < _data.end(); ++iter)
                if (iter->key() == key) return iter;
            return _data.end();
        }
        FVV_INLINE const_iterator findValue(const _valueType& value) const {
            for (auto iter = _data.begin(); iter < _data.end(); ++iter)
                if (iter->value() == value) return iter;
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
        FVV_INLINE vec<_keyType> keys(void) const {
            vec<_keyType> keysList{};
            keysList.reserve(_data.size());
            for (auto iter = _data.begin(); iter < _data.end(); ++iter) keysList.emplace_back(iter->key());
            return keysList;
        }
        FVV_INLINE vec<_valueType> values(void) const {
            vec<_valueType> valuesList{};
            valuesList.reserve(_data.size());
            for (auto iter = _data.begin(); iter < _data.end(); ++iter) valuesList.emplace_back(iter->value());
            return valuesList;
        }
        FVV_INLINE const vec<Pair>& data(void) const { return _data; }
        FVV_INLINE vec<Pair>&& data_rv(void) { return std::move(_data); }
        FVV_INLINE void        sort(std::function<bool(const Pair&, const Pair&)> compare_func = nullptr) {
            static const std::function<bool(const Pair&, const Pair&)> default_comp =
                [](const Pair& a, const Pair& b) -> bool { return (a.key() < b.key()); };
            if (compare_func)
                std::sort(_data.begin(), _data.end(), compare_func);
            else
                std::sort(_data.begin(), _data.end(), default_comp);
        }
        FVV_INLINE void   reverse(void) { std::reverse(_data.begin(), _data.end()); }
        FVV_INLINE void   clear(void) { _data.clear(); }
        FVV_INLINE size_t size(void) const noexcept { return _data.size(); }
        FVV_INLINE bool   empty(void) const noexcept { return (_data.begin() == _data.end()); }

    private:
        vec<Pair> _data;
    };

    static constexpr const bool   defaultBool   = false;
    static constexpr const int    defaultInt    = 0;
    static constexpr const double defaultDouble = 0.0;
    static const str              defaultString;
    static const vec<bool>        defaultBools;
    static const vec<int>         defaultInts;
    static const vec<double>      defaultDoubles;
    static const vec<str>         defaultStrings;
    struct FVVV {
        using FVVVT =
            std::variant<std::monostate, bool, int, double, str, vec<bool>, vec<int>, vec<double>, vec<str>>;
        FVVVT value;
        /// @brief 子值
        PairList<str, FVVV> sub = {};
        /// @brief 描述
        str desc = "";
        /// @brief 链接名称
        str        link       = "";
        FVV_INLINE FVVV(void) = default;
        FVVV(const FVVVT& v) : value(v) {}
        FVV_INLINE FVVV& operator[](const strv& key) { return sub[key.data()]; }
        FVV_INLINE FVVV& operator=(const FVVVT& val) {
            value = val;
            return *this;
        }
        FVV_INLINE FVVV& operator=(FVVVT&& val) {
            value = val;
            return *this;
        }
        /// @brief  以bool类型返回值
        /// @return 值
        FVV_INLINE bool asBool(void) const { return as<bool>().value_or(defaultBool); }
        /// @brief  以int类型返回值
        /// @return 值
        FVV_INLINE int asInt(void) const { return as<int>().value_or(defaultInt); }
        /// @brief  以double类型返回值
        /// @return 值
        FVV_INLINE double asDouble(void) const { return as<double>().value_or(defaultDouble); }
        /// @brief  以string类型返回值
        /// @return 值
        FVV_INLINE const str asString(void) const { return as<str>().value_or(defaultString); }
        /// @brief  以vector<bool>类型返回值
        /// @return 值
        FVV_INLINE const vec<bool> asBools(void) const { return as<vec<bool>>().value_or(defaultBools); }
        /// @brief  以vector<int>类型返回值
        /// @return 值
        FVV_INLINE const vec<int> asInts(void) const { return as<vec<int>>().value_or(defaultInts); }
        /// @brief  以vector<double>类型返回值
        /// @return 值
        FVV_INLINE const vec<double> asDoubles(void) const { return as<vec<double>>().value_or(defaultDoubles); }
        /// @brief  以vector<string>类型返回值
        /// @return 值
        FVV_INLINE const vec<str> asStrings(void) const { return as<vec<str>>().value_or(defaultStrings); }
        /// @brief  判断值是否为空
        /// @return 值为空时返回true，否则为false
        FVV_INLINE bool isEmpty(void) const {
            return std::holds_alternative<std::monostate>(value) && sub.empty();
        }
        /// @brief  判断值是否为非空
        /// @return 值为非空时返回true，否则为false
        FVV_INLINE bool isNotEmpty(void) const { return !isEmpty(); }
        template <typename T>
        /// @brief  判断值是否为指定类型
        /// @param  类型
        /// @return 值为指定类型时返回true，否则为false
        FVV_INLINE bool isType(void) const {
            return isNotEmpty() && std::holds_alternative<T>(value);
        }
        template <typename T>
        /// @brief  以指定类型返回值
        /// @param  类型
        /// @return 值为指定类型时返回值，否则为nullopt
        FVV_INLINE std::optional<T> as(void) const {
            if (isType<T>())
                return std::get<T>(value);
            else
                return std::nullopt;
        }
        /// @brief  输出格式化后的值
        /// @param  为“common”或空时正常输出
        /// @param  为“min”时最小化输出
        /// @param  为“biglist”时会把值组内每个值换行输出
        /// @return 格式化后的值
        FVV_INLINE str print(const strv& type = "common") const {
            bool is_min = type == "min", is_biglist = type == "biglist", is_nodesc = type == "nodesc";
            str  result;
            std::function<void(const str&, const FVVV*, size_t)> print_func;
            print_func = [&](const str& path, const FVVV* node, size_t indent_lv) {
                str indent(indent_lv * 2, ' ');
                if (!node->sub.empty() && !path.empty()) {
                    if (is_min)
                        result += path + "={";
                    else
                        result += indent + path + " = {\n";
                }
                if (node->sub.empty() && node->isNotEmpty()) {
                    if (is_min)
                        result += path + '=';
                    else
                        result += indent + path + " = ";
                    if (!node->link.empty())
                        result += node->link;
                    else if (node->isType<str>()) {
                        result += '"';
                        str tmp_str = node->as<str>().value();
                        _utf8ForEach(tmp_str, tmp_str.size(),
                                     [&result]([[maybe_unused]] const size_t& idx, const strv& idx_char,
                                               [[maybe_unused]] const uint8_t& char_size) -> bool {
                                         if (idx_char == "\"") result += '\\';
                                         result += idx_char;
                                         return false;
                                     });
                        result += '"';
                    } else if (node->isType<bool>())
                        result += node->as<bool>().value() ? "true" : "false";
                    else if (node->isType<int>())
                        result += std::to_string(node->as<int>().value());
                    else if (node->isType<double>())
                        result += std::to_string(node->as<double>().value());
                    else if (node->isType<vec<str>>()) {
                        result += '[';
                        str vec_indent((indent_lv + 1) * 2, ' ');
                        if (is_biglist) result += '\n';
                        const vec<str> tmp = node->as<vec<str>>().value();
                        for (const str& value : tmp) {
                            if (is_biglist) result += vec_indent;
                            result += '"';
                            _utf8ForEach(value, value.size(),
                                         [&result]([[maybe_unused]] const size_t& idx, const strv& idx_char,
                                                   [[maybe_unused]] const uint8_t& char_size) -> bool {
                                             if (idx_char == "\"") result += '\\';
                                             result += idx_char;
                                             return false;
                                         });
                            result += "\",";
                            if (is_biglist) {
                                result.pop_back();
                                result += '\n';
                            } else if (!is_min)
                                result += ' ';
                        }
                        if (!is_biglist) {
                            result.pop_back();
                            if (!is_min) result.pop_back();
                        } else
                            result += indent;
                        result += ']';
                    } else if (node->isType<vec<bool>>()) {
                        result += '[';
                        str vec_indent((indent_lv + 1) * 2, ' ');
                        if (is_biglist) result += '\n';
                        const vec<bool> tmp = node->as<vec<bool>>().value();
                        for (const int& value : tmp) {
                            if (is_biglist) result += vec_indent;
                            result += str(value ? "true" : "false") + ",";
                            if (is_biglist) {
                                result.pop_back();
                                result += '\n';
                            } else if (!is_min)
                                result += ' ';
                        }
                        if (!is_biglist) {
                            result.pop_back();
                            if (!is_min) result.pop_back();
                        } else
                            result += indent;
                        result += ']';
                    } else if (node->isType<vec<int>>()) {
                        result += '[';
                        str vec_indent((indent_lv + 1) * 2, ' ');
                        if (is_biglist) result += '\n';
                        const vec<int> tmp = node->as<vec<int>>().value();
                        for (const int& value : tmp) {
                            if (is_biglist) result += vec_indent;
                            result += std::to_string(value) + ',';
                            if (is_biglist) {
                                result.pop_back();
                                result += '\n';
                            } else if (!is_min)
                                result += ' ';
                        }
                        if (!is_biglist) {
                            result.pop_back();
                            if (!is_min) result.pop_back();
                        } else
                            result += indent;
                        result += ']';
                    } else if (node->isType<vec<double>>()) {
                        result += '[';
                        str vec_indent((indent_lv + 1) * 2, ' ');
                        if (is_biglist) result += '\n';
                        const vec<double> tmp = node->as<vec<double>>().value();
                        for (const double& value : tmp) {
                            if (is_biglist) result += vec_indent;
                            result += std::to_string(value) + ',';
                            if (is_biglist) {
                                result.pop_back();
                                result += '\n';
                            } else if (!is_min)
                                result += ' ';
                        }
                        if (!is_biglist) {
                            result.pop_back();
                            if (!is_min) result.pop_back();
                        } else
                            result += indent;
                        result += ']';
                    }
                    if (!node->desc.empty() && !is_min && !is_nodesc) {
                        result += " <";
                        str tmp_str = node->desc;
                        _utf8ForEach(tmp_str, tmp_str.size(),
                                     [&result]([[maybe_unused]] const size_t& idx, const strv& idx_char,
                                               [[maybe_unused]] const uint8_t& char_size) -> bool {
                                         if (idx_char == ">") result += '\\';
                                         result += idx_char;
                                         return false;
                                     });
                        result += '>';
                    }
                    if (is_min)
                        result += ';';
                    else
                        result += '\n';
                } else
                    for (const PairList<str, FVVV>::Pair& item : node->sub)
                        print_func(item.key(), &item.value(), indent_lv + 1);
                if (!node->sub.empty() && !path.empty()) {
                    if (is_min)
                        result += '}';
                    else
                        result += indent + '}';
                    if (!node->desc.empty() && !is_min && !is_nodesc) {
                        result += " <";
                        str tmp_str = node->desc;
                        _utf8ForEach(tmp_str, tmp_str.size(),
                                     [&result]([[maybe_unused]] const size_t& idx, const strv& idx_char,
                                               [[maybe_unused]] const uint8_t& char_size) -> bool {
                                         if (idx_char == ">") result += '\\';
                                         result += idx_char;
                                         return false;
                                     });
                        result += '>';
                    }
                    if (is_min)
                        result += ';';
                    else
                        result += '\n';
                }
            };
            for (const PairList<str, FVVV>::Pair& item : sub) print_func(item.key(), &item.value(), 0);
            result.pop_back();
            _shrink(&result);
            return result;
        }
    };
    class Parser {
    public:
        /// @brief            解析字符串为FVVV
        /// @param txt        FVV文本格式的字符串
        /// @param target_fvvv 在外部定义好了的FVVV
        static FVV_INLINE void ReadString(str txt, FVVV& target_fvvv) {
            if (txt.size() >= 3 && static_cast<unsigned char>(txt[0]) == _bom[0] &&
                static_cast<unsigned char>(txt[1]) == _bom[1] && static_cast<unsigned char>(txt[2]) == _bom[2])
                txt = txt.substr(3);
            size_t start = txt.find_first_not_of(" \t\r\n");
            txt          = start == str::npos ? "" : txt.substr(start);
            if (txt.empty()) return;
            if (txt.back() != '}' && txt.back() != '\n') txt += '\n';
            _replace(txt, "\r\n", "\n");
            _replace(txt, "\r", "\n");
            _shrink(&txt);
            str           idx_desc, tmp_desc, value, value_name;
            vec<str>      group_names, value_names, values;
            vec<vec<str>> last_group_names;
            bool end_group = false, old_fvv = false, is_real_char = false, in_value = false, in_desc = false,
                 in_str = false, is_str = false, is_empty_str = false, in_list = false, is_list = false;
            size_t  group_num      = 0;
            uint8_t last_char_size = 0;
            _utf8ForEach(
                txt, txt.size(), [&](const size_t& idx, const strv& idx_char, const uint8_t& char_size) -> bool {
                    FVVV* idx_key = &target_fvvv;
                    is_real_char =
                        idx >= 1 ? (last_char_size == 1 ? (txt[idx - 1] != '\\' ? true : false) : true) : true;
                    last_char_size = char_size;
                    if (in_desc) {
                        if (idx_char != ">" || !is_real_char) {
                            if (idx_char == ">" && !is_real_char) {
                                tmp_desc.pop_back();
                            }
                            if (in_value || group_num > 0) tmp_desc += idx_char;
                            return false;
                        } else if (idx_char == ">") {
                            idx_desc = tmp_desc;
                            _clearAndShrink(&tmp_desc);
                            _shrink(&idx_desc);
                            in_desc = false;
                            return false;
                        }
                    } else {
                        if (!in_str && (idx_char == " " || idx_char == "\t" || idx_char == "\r"))
                            return false;
                        else if (idx_char == "<") {
                            in_desc = true;
                            return false;
                        }
                    }
                    if (in_value) {
                        if (in_str) {
                            if (idx_char == "\"") {
                                if (is_real_char) {
                                    if (value.empty())
                                        is_empty_str = true;
                                    else
                                        is_empty_str = false;
                                    in_str = false;
                                    return false;
                                } else {
                                    value.pop_back();
                                    value += idx_char;
                                    return false;
                                }
                            } else {
                                value += idx_char;
                                return false;
                            }
                        } else {
                            if (idx_char == "\"") {
                                in_str = is_str = true;
                                return false;
                            } else if (idx_char == "[") {
                                in_list = is_list = true;
                                return false;
                            } else if (in_list && _eq_or(idx_char, strv(","), strv("]"), strv("\n"))) {
                                if (idx_char == "]") {
                                    in_list  = false;
                                    size_t j = 1;
                                    while (_eq_or(txt[idx - j], ' ', '\t', '\r')) ++j;
                                    if (txt[idx - j] == ',' || txt[idx - j] == '\n') return false;
                                } else if (value.empty() && (!is_str || !is_empty_str))
                                    return false;
                                values.push_back(value);
                                if (is_empty_str)
                                    is_empty_str = false;
                                else
                                    _clearAndShrink(&value);
                                return false;
                            } else if (idx_char == "{") {
                                group_names.insert(group_names.end(), value_names.begin(), value_names.end());
                                last_group_names.push_back(value_names);
                                _clearAndShrink(&value_names);
                                ++group_num;
                                in_value = false;
                                return false;
                            } else if (!in_list && _eq_or(idx_char, strv(";"), strv("\n"))) {
                                for (size_t i = 0; i < group_names.size(); ++i)
                                    idx_key = &(*idx_key)[group_names[i]];
                                for (size_t i = 0; i < value_names.size(); ++i) {
                                    const str& key = value_names[i];
                                    if (i == value_names.size() - 1) {
                                        if (is_list) {
                                            if (is_str)
                                                (*idx_key)[key] = values;
                                            else {
                                                if (values.empty()) {
                                                    (*idx_key)[key] = FVVV();
                                                    return false;
                                                }
                                                str tmp_str = values.front();
                                                if (_eq_or(tmp_str, str("true"), str("false"))) {
                                                    vec<bool> tmp;
                                                    tmp.reserve(values.size());
                                                    std::transform(values.begin(), values.end(),
                                                                   std::back_inserter(tmp),
                                                                   [](const strv& s) { return s == "true"; });
                                                    (*idx_key)[key] = tmp;
                                                } else if (_isInt(tmp_str)) {
                                                    vec<int> tmp;
                                                    for (const str& str : values)
                                                        if (_isInt(str)) tmp.push_back(std::stoi(str));
                                                    (*idx_key)[key] = tmp;
                                                } else if (_isDouble(tmp_str)) {
                                                    vec<double> tmp;
                                                    for (const str& str : values)
                                                        if (_isDouble(str)) tmp.push_back(std::stod(str));
                                                    (*idx_key)[key] = tmp;
                                                }
                                            }
                                        } else if (is_str)
                                            (*idx_key)[key] = value;
                                        else if (_eq_or(value, str("true"), str("false")))
                                            (*idx_key)[key] = value == "true";
                                        else if (_isInt(value))
                                            (*idx_key)[key] = std::stoi(value);
                                        else if (_isDouble(value))
                                            (*idx_key)[key] = std::stod(value);
                                        else {
                                            vec<str> tmp_names = _split(value, '.');
                                            FVVV*    tmp_key   = idx_key;
                                            for (size_t i = 0; i < tmp_names.size(); ++i)
                                                if (!tmp_key->sub.hasKey(tmp_names[i])) {
                                                    tmp_key = nullptr;
                                                    break;
                                                } else
                                                    tmp_key = &(*tmp_key)[tmp_names[i]];
                                            if (!tmp_key) {
                                                tmp_key = &target_fvvv;
                                                for (size_t i = 0; i < tmp_names.size(); ++i)
                                                    if (!tmp_key->sub.hasKey(tmp_names[i])) {
                                                        tmp_key = nullptr;
                                                        break;
                                                    } else
                                                        tmp_key = &(*tmp_key)[tmp_names[i]];
                                            }
                                            if (tmp_key) {
                                                if (tmp_key->sub.empty())
                                                    (*idx_key)[key] = FVVV(*tmp_key);
                                                else
                                                    (*idx_key)[key].sub = tmp_key->sub;
                                            }
                                            (*idx_key)[key].link = value;
                                        }
                                        (*idx_key)[key].desc = idx_desc;
                                        _clearAndShrink(&idx_desc, &value, &values, &value_names);
                                        is_list = is_str = in_value = false;
                                        continue;
                                    } else
                                        idx_key = &(*idx_key)[key];
                                }
                            } else {
                                value += idx_char;
                                return false;
                            }
                        }
                    } else {
                        if (!old_fvv && idx_char == "{" && value_name.empty()) {
                            old_fvv = true;
                            return false;
                        } else if (idx_char == "=") {
                            value_names = _split(value_name, '.');
                            _clearAndShrink(&value_name);
                            in_value = true;
                            return false;
                        } else if (end_group && _eq_or(idx_char, strv(";"), strv("\n")) && group_num > 0) {
                            end_group = false;
                            if (!idx_desc.empty())
                                for (size_t i = 0; i < group_names.size(); ++i) {
                                    if (i == group_names.size() - 1) {
                                        (*idx_key)[group_names[i]].desc = idx_desc;
                                        _clearAndShrink(&idx_desc);
                                        break;
                                    }
                                    idx_key = &(*idx_key)[group_names[i]];
                                }
                            for ([[maybe_unused]] const str& _ : last_group_names.back()) group_names.pop_back();
                            last_group_names.pop_back();
                            _shrink(&group_names, &last_group_names);
                            group_num--;
                            return false;
                        } else if (idx_char == "}")
                            if (group_num == 0)
                                return true;
                            else {
                                end_group = true;
                                return false;
                            }
                        else {
                            value_name += idx_char;
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
            size_t end   = path.find_last_not_of("\n");
            if (start == str::npos || end == str::npos) return {};
            str               target = path.substr(start, end - start + 1);
            vec<str>          result;
            std::stringstream ss(target);
            str               item;
            while (std::getline(ss, item, delimiter)) result.push_back(item);
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
        if (s.empty()) return false;
        size_t start = 0;
        if (s[0] == '-' || s[0] == '+') {
            if (s.size() == 1) return false;
            start = 1;
        }
        return std::all_of(s.begin() + start, s.end(), ::isdigit);
    }
    static FVV_INLINE bool _isDouble(const strv& s) {
        if (s.empty()) return false;
        size_t start    = 0;
        bool   hasDigit = false, hasDot = false;
        if (s[0] == '-' || s[0] == '+') {
            if (s.size() == 1) return false;
            start = 1;
        }
        for (size_t i = start; i < s.size(); ++i) {
            char c = s[i];
            if (std::isdigit(c))
                hasDigit = true;
            else if (c == '.') {
                if (hasDot) return false;
                hasDot = true;
            } else
                return false;
        }
        return hasDigit;
    }

private:
    template <typename T>
    static FVV_INLINE void _shrink(T* container) {
        if (container) container->shrink_to_fit();
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
            unsigned char c         = target[i];
            uint8_t       char_size = 0;
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
            if (handler(i, target.substr(i, char_size), char_size)) break;
            i += char_size;
        }
    }
};

const std::string              FVV::defaultString  = "";
const std::vector<bool>        FVV::defaultBools   = {};
const std::vector<int>         FVV::defaultInts    = {};
const std::vector<double>      FVV::defaultDoubles = {};
const std::vector<std::string> FVV::defaultStrings = {};

#endif
