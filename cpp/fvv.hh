//====================================================================================================
// Copyright (C) 2016-present Anne Sakitin (Tianwan Ayana).                                          =
//                                                                                                   =
// Licensed under the F2DLPR License.                                                                =
//                                                                                                   =
// YOU MAY NOT USE THIS FILE EXCEPT IN COMPLIANCE WITH THE LICENSE.                                  =
// Provided "AS IS", WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,                                   =
// unless required by applicable law or agreed to in writing.                                        =
//                                                                                                   =
// For details about the F2DLPR License terms and conditions, visit: http://license.fileto.download. =
//====================================================================================================

#pragma once

#define FVV_API 1

#include <algorithm>
#include <cstdint>
#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#ifdef __GNUC__
#define FVV_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define FVV_INLINE __forceinline
#else
#define FVV_INLINE inline
#endif

namespace FVV {
    using namespace std;
    using str  = string;
    using strv = string_view;
    template <typename Tp>
    using vec = vector<Tp>;

    template <typename _keyTp, typename _valTp>
    struct KVPair : public pair<_keyTp, _valTp> {
        using pair<_keyTp, _valTp>::pair;
        FVV_INLINE _keyTp&       key(void) { return this->first; }
        FVV_INLINE const _keyTp& key(void) const { return this->first; }
        FVV_INLINE _keyTp&&      key_rv(void) { return std::move(this->first); }
        FVV_INLINE _valTp&       value(void) { return this->second; }
        FVV_INLINE const _valTp& value(void) const { return this->second; }
        FVV_INLINE _valTp&&      value_rv(void) { return std::move(this->second); }
    };
    template <typename _keyTp, typename _valTp>
    struct PairList : public vec<KVPair<_keyTp, _valTp>> {
        using iterator       = typename vec<KVPair<_keyTp, _valTp>>::iterator;
        using const_iterator = typename vec<KVPair<_keyTp, _valTp>>::const_iterator;
        FVV_INLINE vec<KVPair<_keyTp, _valTp>>&& data_rv(void) { return std::move(*this); }
        FVV_INLINE _valTp&                       operator[](const _keyTp& key) {
            if (auto it = find_if(this->begin(), this->end(),
                                                        [&key](const KVPair<_keyTp, _valTp>& p) { return p.key() == key; });
                it != this->end())
                return it->value();
            this->emplace_back(key, _valTp());
            return this->back().value();
        }
        FVV_INLINE _keyTp& operator()(const _valTp& value) {
            if (auto it = find_if(this->begin(), this->end(),
                                  [&value](const KVPair<_keyTp, _valTp>& p) { return p.value() == value; });
                it != this->end())
                return it->key();
            this->emplace_back(_keyTp(), value);
            return this->back().key();
        }
        FVV_INLINE bool hasKey(const _keyTp& key) const {
            return any_of(this->begin(), this->end(),
                          [&key](const KVPair<_keyTp, _valTp>& p) { return p.key() == key; });
        }
        FVV_INLINE bool hasValue(const _valTp& value) const {
            return any_of(this->begin(), this->end(),
                          [&value](const KVPair<_keyTp, _valTp>& p) { return p.value() == value; });
        }
        FVV_INLINE const_iterator findKey(const _keyTp& key) const {
            return find_if(this->begin(), this->end(),
                           [&key](const KVPair<_keyTp, _valTp>& p) { return p.key() == key; });
        }
        FVV_INLINE const_iterator findValue(const _valTp& value) const {
            return find_if(this->begin(), this->end(),
                           [&value](const KVPair<_keyTp, _valTp>& p) { return p.value() == value; });
        }
        FVV_INLINE void eraseKey(const _keyTp& key) {
            this->erase(remove_if(this->begin(), this->end(),
                                  [&key](const KVPair<_keyTp, _valTp>& p) { return p.key() == key; }),
                        this->end());
        }
        FVV_INLINE void eraseValue(const _valTp& value) {
            this->erase(remove_if(this->begin(), this->end(),
                                  [&value](const KVPair<_keyTp, _valTp>& p) { return p.value() == value; }),
                        this->end());
        }
        FVV_INLINE vec<_keyTp> keys(void) const {
            vec<_keyTp> keys;
            keys.reserve(this->size());
            transform(this->begin(), this->end(), back_inserter(keys),
                      [](const KVPair<_keyTp, _valTp>& p) { return p.key(); });
            return keys;
        }
        FVV_INLINE vec<_valTp> values(void) const {
            vec<_valTp> vals;
            vals.reserve(this->size());
            transform(this->begin(), this->end(), back_inserter(vals),
                      [](const KVPair<_keyTp, _valTp>& p) { return p.second; });
            return vals;
        }
        FVV_INLINE void sort(
            function<bool(const KVPair<_keyTp, _valTp>&, const KVPair<_keyTp, _valTp>&)> func = nullptr) {
            std::sort(
                this->begin(), this->end(),
                func ? func : [](const KVPair<_keyTp, _valTp>& a, const KVPair<_keyTp, _valTp>& b) {
                    return a.key() < b.key();
                });
        }
    };
    class FVVV {
    public:
        using FVVVT = variant<monostate, bool, int, double, str, vec<bool>, vec<int>, vec<double>, vec<str>>;
        /// @brief 值
        FVVVT value;
        /// @brief 子值
        PairList<str, FVVV> sub = {};
        /// @brief 描述
        str desc = "";
        /// @brief 链接名称
        str              link       = "";
        FVV_INLINE       FVVV(void) = default;
        FVV_INLINE       FVVV(const FVVVT& v) : value(v) {}
        FVV_INLINE FVVV& operator[](strv key) { return sub[key.data()]; }
        FVV_INLINE FVVV& operator=(const FVVVT& val) {
            value = val;
            return *this;
        }
        FVV_INLINE FVVV& operator=(FVVVT&& val) {
            value = val;
            return *this;
        }
        FVV_INLINE bool operator==(const FVVV& other) const {
            return addressof(other) == this || value == other.value;
        }
        FVV_INLINE bool operator!=(const FVVV& other) const {
            return addressof(other) != this && value != other.value;
        }
        /// @brief  以bool类型返回值
        /// @param  默认值(可选)
        /// @return 值
        FVV_INLINE bool asBool(bool defaultValue = _getDfltVal<bool>()) const { return as<bool>(defaultValue); }
        /// @brief  以int类型返回值
        /// @param  默认值(可选)
        /// @return 值
        FVV_INLINE int asInt(int defaultValue = _getDfltVal<int>()) const { return as<int>(defaultValue); }
        /// @brief  以double类型返回值
        /// @param  默认值(可选)
        /// @return 值
        FVV_INLINE double asDouble(double defaultValue = _getDfltVal<double>()) const {
            return as<double>(defaultValue);
        }
        /// @brief  以string类型返回值
        /// @param  默认值(可选)
        /// @return 值
        FVV_INLINE const str& asString(const str& defaultValue = _getDfltVal<str>()) const {
            return as<str>(defaultValue);
        }
        /// @brief  以vector<bool>类型返回值
        /// @param  默认值(可选)
        /// @return 值
        FVV_INLINE const vec<bool>& asBools(const vec<bool>& defaultValue = _getDfltVal<vec<bool>>()) const {
            return as<vec<bool>>(defaultValue);
        }
        /// @brief  以vector<int>类型返回值
        /// @param  默认值(可选)
        /// @return 值
        FVV_INLINE const vec<int>& asInts(const vec<int>& defaultValue = _getDfltVal<vec<int>>()) const {
            return as<vec<int>>(defaultValue);
        }
        /// @brief  以vector<double>类型返回值
        /// @param  默认值(可选)
        /// @return 值
        FVV_INLINE const vec<double>& asDoubles(
            const vec<double>& defaultValue = _getDfltVal<vec<double>>()) const {
            return as<vec<double>>(defaultValue);
        }
        /// @brief  以vector<string>类型返回值
        /// @param  默认值(可选)
        /// @return 值
        FVV_INLINE const vec<str>& asStrings(const vec<str>& defaultValue = _getDfltVal<vec<str>>()) const {
            return as<vec<str>>(defaultValue);
        }
        /// @brief              以指定类型返回值
        /// @param Tp           类型
        /// @param defaultValue 默认值(可选)
        /// @return             值为指定类型时返回值，否则为默认值
        template <typename Tp>
        FVV_INLINE const Tp& as(const Tp& defaultValue = _getDfltVal<Tp>()) const {
            if (auto ptr = get_if<Tp>(&value)) return *ptr;
            return defaultValue;
        }
        /// @brief  判断值是否为空
        /// @return 值为空时返回true，否则为false
        FVV_INLINE bool isEmpty(void) const { return holds_alternative<monostate>(value); }
        /// @brief  判断值是否为非空
        /// @return 值为非空时返回true，否则为false
        FVV_INLINE bool isNotEmpty(void) const { return !isEmpty(); }
        template <typename Tp>
        /// @brief  判断值是否为指定类型
        /// @param  类型
        /// @return 值为指定类型时返回true，否则为false
        FVV_INLINE bool isType(void) const {
            return isNotEmpty() && holds_alternative<Tp>(value);
        }
        /// @brief  输出FVV文本格式格式化后的值
        /// @param  为“common”或空时正常输出
        /// @param  为“min”时最小化输出
        /// @param  为“biglist”时会把值组内每个值换行输出
        /// @return FVV文本格式格式化后的值
        FVV_INLINE str print(strv type = "common") const {
            bool         is_min = type == "min", is_biglist = type == "biglist", is_nodesc = type == "nodesc";
            stringstream result;
            function<void(strv, const FVVV*, size_t)> print_func;
            print_func = [&](strv path, const FVVV* node, size_t indent_lv) {
                if (path.empty() || (node->isEmpty() && node->sub.empty())) return;
                str indent(indent_lv * 2, ' ');
                if (!node->sub.empty() && node->link.empty()) {
                    if (is_min)
                        result << path << "={";
                    else
                        result << indent << path << " = {\n";
                }
                if (!node->link.empty() || node->isNotEmpty()) {
                    if (is_min)
                        result << path << '=';
                    else
                        result << indent << path << " = ";
                    if (!node->link.empty())
                        result << node->link;
                    else if (node->isType<str>())
                        result << '"' << _replace(node->as<str>(), "\"", "\\\"") << '"';
                    else if (node->isType<bool>())
                        result << (node->as<bool>() ? "true" : "false");
                    else if (node->isType<int>())
                        result << to_string(node->as<int>());
                    else if (node->isType<double>())
                        result << to_string(node->as<double>());
                    else if (node->isType<vec<str>>() || node->isType<vec<bool>>() || node->isType<vec<int>>() ||
                             node->isType<vec<double>>()) {
                        result << '[';
                        str vec_indent((indent_lv + 1) * 2, ' ');
                        if (is_biglist) result << '\n';
                        bool is_empty_list = true;
                        if (node->isType<vec<str>>()) {
                            const vec<str> tmp = node->as<vec<str>>();
                            is_empty_list      = tmp.empty();
                            for (const str& value : tmp) {
                                if (is_biglist) result << vec_indent;
                                result << '"' << _replace(value, "\"", "\\\"") << '"';
                                if (is_biglist)
                                    result << '\n';
                                else {
                                    result << ',';
                                    if (!is_min) result << ' ';
                                }
                            }
                        } else if (node->isType<vec<bool>>()) {
                            const vec<bool> tmp = node->as<vec<bool>>();
                            is_empty_list       = tmp.empty();
                            for (const int& value : tmp) {
                                if (is_biglist) result << vec_indent;
                                result << str(value ? "true" : "false");
                                if (is_biglist)
                                    result << '\n';
                                else {
                                    result << ',';
                                    if (!is_min) result << ' ';
                                }
                            }
                        } else if (node->isType<vec<int>>()) {
                            const vec<int> tmp = node->as<vec<int>>();
                            is_empty_list      = tmp.empty();
                            for (const int& value : tmp) {
                                if (is_biglist) result << vec_indent;
                                result << to_string(value);
                                if (is_biglist)
                                    result << '\n';
                                else {
                                    result << ',';
                                    if (!is_min) result << ' ';
                                }
                            }
                        } else if (node->isType<vec<double>>()) {
                            const vec<double> tmp = node->as<vec<double>>();
                            is_empty_list         = tmp.empty();
                            for (const double& value : tmp) {
                                if (is_biglist) result << vec_indent;
                                result << to_string(value);
                                if (is_biglist)
                                    result << '\n';
                                else {
                                    result << ',';
                                    if (!is_min) result << ' ';
                                }
                            }
                        }
                        if (is_biglist)
                            result << indent;
                        else if (!is_empty_list) {
                            _removeLastChar(result);
                            if (!is_min) _removeLastChar(result);
                        }
                        result << ']';
                    }
                } else
                    for (const auto& [key, sub] : node->sub) print_func(key, &sub, indent_lv + 1);
                if (!node->sub.empty() && node->link.empty()) {
                    if (!is_min) result << indent;
                    result << '}';
                }
                if (!node->desc.empty() && !is_min && !is_nodesc)
                    result << " <" << _replace(node->desc, ">", "\\>") << '>';
                if (is_min)
                    result << ';';
                else
                    result << '\n';
            };
            for (const auto& [key, value] : sub) print_func(key, &value, 0);
            _removeLastChar(result);
            return result.str();
        }
        /// @brief     解析字符串到此FVVV
        /// @param txt FVV文本格式的字符串
        FVV_INLINE void addFromString(str txt) {
            if (txt.size() >= 3 && static_cast<unsigned char>(txt[0]) == _bom[0] &&
                static_cast<unsigned char>(txt[1]) == _bom[1] && static_cast<unsigned char>(txt[2]) == _bom[2])
                txt = txt.substr(3);
            size_t start = txt.find_first_not_of(" \t\r\n");
            txt          = start == str::npos ? "" : txt.substr(start);
            if (txt.empty()) return;
            if (txt.back() != '}' && txt.back() != '\n') txt += '\n';
            _replaceBase(txt, "\r\n", "\n");
            _replaceBase(txt, "\r", "\n");
            _shrink(&txt);
            stringstream  tmp_desc, value, value_name;
            str           idx_desc;
            vec<str>      group_names, value_names, values;
            vec<vec<str>> last_group_names;
            bool end_group = false, old_fvv = false, is_real_char = false, in_value = false, in_desc = false,
                 in_str = false, is_str = false, is_all_str = false, is_empty_str = false, in_list = false,
                 is_list           = false;
            size_t  group_num      = 0;
            uint8_t last_char_size = 0;
            FVVV*   root_key       = this;
            _utf8ForEach(
                txt, txt.size(), [&](const size_t& idx, strv idx_char, const uint8_t& char_size) -> bool {
                    FVVV* idx_key = root_key;
                    is_real_char =
                        idx >= 1 ? (last_char_size == 1 ? (txt[idx - 1] != '\\' ? true : false) : true) : true;
                    last_char_size = char_size;
                    if (in_desc) {
                        if (idx_char != ">" || !is_real_char) {
                            if (idx_char == ">" && !is_real_char) _removeLastChar(tmp_desc);
                            if (in_value || group_num > 0) tmp_desc << idx_char;
                            return false;
                        } else if (idx_char == ">") {
                            idx_desc = tmp_desc.str();
                            tmp_desc.str("");
                            tmp_desc.clear();
                            _shrink(&idx_desc);
                            in_desc = false;
                            return false;
                        }
                    } else {
                        if (!in_str && (idx_char == " " || idx_char == "\t"))
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
                                    if (value.str().empty())
                                        is_empty_str = true;
                                    else
                                        is_empty_str = false;
                                    in_str = false;
                                    return false;
                                } else {
                                    _removeLastChar(value);
                                    value << idx_char;
                                    return false;
                                }
                            } else {
                                value << idx_char;
                                return false;
                            }
                        } else {
                            if (idx_char == "\"") {
                                in_str = is_str = is_all_str = true;
                                return false;
                            } else if (idx_char == "[") {
                                in_list = is_list = true;
                                return false;
                            } else if (in_list && _eqOr(idx_char, strv(","), strv("]"), strv("\n"))) {
                                const str value_str = value.str();
                                if (idx_char == "]") {
                                    in_list             = false;
                                    size_t pos          = 1;
                                    bool   in_list_desc = false;
                                    for (;;) {
                                        if ([&txt, &idx, &pos, &in_list_desc]() -> bool {
                                                switch (txt[idx - pos]) {
                                                    case '<':
                                                        if (!in_list_desc) return true;
                                                        if (idx - pos < 1 || txt[idx - pos - 1] != '\\')
                                                            in_list_desc = false;
                                                        return false;
                                                    case '>':
                                                        in_list_desc = true;
                                                        return false;
                                                    case ' ':
                                                    case '\t':
                                                        return false;
                                                    case ',':
                                                    case '\n':
                                                    default:
                                                        if (in_list_desc) return false;
                                                        return true;
                                                }
                                            }())
                                            break;
                                        ++pos;
                                    }
                                    if (txt[idx - pos] == ',' || txt[idx - pos] == '\n') return false;
                                } else if (value_str.empty() && (!is_all_str || !is_empty_str))
                                    return false;
                                if ((is_all_str && is_str) || _eqOr(value_str, str("true"), str("false")) ||
                                    _isInt(value_str) || _isDouble(value_str))
                                    values.push_back(value_str);
                                else if (idx_key = _getKey({value_str}, _getKey(group_names, idx_key));
                                         idx_key->isNotEmpty()) {
                                    if (idx_key->isType<str>())
                                        values.push_back(idx_key->as<str>());
                                    else if (idx_key->isType<bool>())
                                        values.push_back(idx_key->as<bool>() ? "true" : "false");
                                    else if (idx_key->isType<int>())
                                        values.push_back(to_string(idx_key->as<int>()));
                                    else if (idx_key->isType<double>())
                                        values.push_back(to_string(idx_key->as<double>()));
                                    else if (idx_key->isType<vec<str>>()) {
                                        const vec<str> tmp = idx_key->as<vec<str>>();
                                        values.insert(values.end(), tmp.begin(), tmp.end());
                                    } else if (idx_key->isType<vec<bool>>()) {
                                        const vec<bool> tmp = idx_key->as<vec<bool>>();
                                        transform(tmp.begin(), tmp.end(), back_inserter(values),
                                                  [](bool v) { return v ? "true" : "false"; });
                                    } else if (idx_key->isType<vec<int>>()) {
                                        const vec<int> tmp = idx_key->as<vec<int>>();
                                        transform(tmp.begin(), tmp.end(), back_inserter(values),
                                                  [](int v) { return to_string(v); });
                                    } else if (idx_key->isType<vec<double>>()) {
                                        const vec<double> tmp = idx_key->as<vec<double>>();
                                        transform(tmp.begin(), tmp.end(), back_inserter(values),
                                                  [](double v) { return to_string(v); });
                                    }
                                }
                                if (is_empty_str)
                                    is_empty_str = false;
                                else {
                                    value.str("");
                                    value.clear();
                                }
                                is_str = false;
                                return false;
                            } else if (idx_char == "{") {
                                group_names.insert(group_names.end(), value_names.begin(), value_names.end());
                                last_group_names.push_back(value_names);
                                _clearAndShrink(&value_names);
                                ++group_num;
                                in_value = false;
                                return false;
                            } else if (!in_list && _eqOr(idx_char, strv(";"), strv("\n"))) {
                                idx_key = _getKey(value_names, _getKey(group_names, idx_key));
                                if (is_list) {
                                    if (values.empty())
                                        *idx_key = FVVV();
                                    else if (is_all_str)
                                        *idx_key = values;
                                    else {
                                        str tmp_str = values.front();
                                        if (_eqOr(tmp_str, str("true"), str("false"))) {
                                            vec<bool> tmp;
                                            tmp.reserve(values.size());
                                            transform(values.begin(), values.end(), back_inserter(tmp),
                                                      [](strv s) { return s == "true"; });
                                            *idx_key = tmp;
                                        } else if (_isInt(tmp_str)) {
                                            vec<int> tmp;
                                            for (const str& str : values)
                                                if (_isInt(str)) tmp.push_back(stoi(str));
                                            *idx_key = tmp;
                                        } else if (_isDouble(tmp_str)) {
                                            vec<double> tmp;
                                            for (const str& str : values)
                                                if (_isDouble(str)) tmp.push_back(stod(str));
                                            *idx_key = tmp;
                                        }
                                    }
                                } else {
                                    const str value_str = value.str();
                                    if (is_all_str)
                                        *idx_key = value_str;
                                    else if (_eqOr(value_str, str("true"), str("false")))
                                        *idx_key = value_str == "true";
                                    else if (_isInt(value_str))
                                        *idx_key = stoi(value_str);
                                    else if (_isDouble(value_str))
                                        *idx_key = stod(value_str);
                                    else if (FVVV* tmp_key = _findKey(value_str, idx_key, root_key); tmp_key) {
                                        if (tmp_key->sub.empty())
                                            *idx_key = tmp_key->value;
                                        else
                                            idx_key->sub = tmp_key->sub;
                                        idx_key->link = value_str;
                                    }
                                }
                                idx_key->desc = idx_desc;
                                value.str("");
                                value.clear();
                                _clearAndShrink(&idx_desc, &values, &value_names);
                                is_list = is_str = is_all_str = in_value = false;
                                return false;
                            } else {
                                value << idx_char;
                                return false;
                            }
                        }
                    } else {
                        if (!old_fvv && idx_char == "{" && value_name.str().empty()) {
                            old_fvv = true;
                            return false;
                        } else if (idx_char == "=") {
                            value_names = _split(value_name.str(), '.');
                            value_name.str("");
                            value_name.clear();
                            in_value = true;
                            return false;
                        } else if (end_group && _eqOr(idx_char, strv(";"), strv("\n")) && group_num > 0) {
                            end_group = false;
                            if (!idx_desc.empty()) {
                                _getKey(group_names, idx_key)->desc = idx_desc;
                                _clearAndShrink(&idx_desc);
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
                            value_name << idx_char;
                            return false;
                        }
                    }
                });
        }

    private:
        static constexpr const unsigned char _bom[] = {0xEF, 0xBB, 0xBF};
        template <typename Tp>
        FVV_INLINE static const Tp& _getDfltVal() {
            if constexpr (is_same_v<Tp, bool>) {
                static constexpr const bool dfltBool = false;
                return dfltBool;
            } else if constexpr (is_same_v<Tp, int>) {
                static constexpr const int dfltInt = 0;
                return dfltInt;
            } else if constexpr (is_same_v<Tp, double>) {
                static constexpr const double dfltDouble = 0.0;
                return dfltDouble;
            } else if constexpr (is_same_v<Tp, str>) {
                static const str dfltStr = "";
                return dfltStr;
            } else if constexpr (is_same_v<Tp, vec<bool>>) {
                static const vec<bool> dfltBools = {};
                return dfltBools;
            } else if constexpr (is_same_v<Tp, vec<int>>) {
                static const vec<int> dfltInts = {};
                return dfltInts;
            } else if constexpr (is_same_v<Tp, vec<double>>) {
                static const vec<double> dfltDoubles = {};
                return dfltDoubles;
            } else if constexpr (is_same_v<Tp, vec<str>>) {
                static const vec<str> dfltStrs = {};
                return dfltStrs;
            }
            static Tp dfltTp{};
            return dfltTp;
        }
        FVV_INLINE static FVVV* _getKey(const vec<str>& paths, FVVV* root_key) {
            FVVV* tmp_key = root_key;
            for (const str& path : paths) tmp_key = &(*tmp_key)[path];
            return tmp_key;
        }
        FVV_INLINE static FVVV* _findKey(strv path, FVVV* idx_key, FVVV* root_key) {
            vec<str> tmp_names = _split(path.data(), '.');
            FVVV*    tmp_key   = idx_key;
            for (const str& tmp_name : tmp_names)
                if (!tmp_key->sub.hasKey(tmp_name)) {
                    tmp_key = nullptr;
                    break;
                } else
                    tmp_key = &(*tmp_key)[tmp_name];
            if (!tmp_key) {
                tmp_key = root_key;
                for (const str& tmp_name : tmp_names)
                    if (!tmp_key->sub.hasKey(tmp_name)) {
                        tmp_key = nullptr;
                        break;
                    } else
                        tmp_key = &(*tmp_key)[tmp_name];
            }
            return tmp_key;
        }
        template <typename Tp>
        FVV_INLINE static bool _eqOr(Tp a, Tp b) {
            return a == b;
        }
        template <typename Tp, typename... Args>
        FVV_INLINE static bool _eqOr(Tp a, Tp b, Args... args) {
            return a == b || _eqOr(a, args...);
        }
        FVV_INLINE static vec<str> _split(const str& path, char delimiter) {
            size_t start = path.find_first_not_of("\n");
            size_t end   = path.find_last_not_of("\n");
            if (start == str::npos || end == str::npos) return {};
            str          target = path.substr(start, end - start + 1);
            vec<str>     result;
            stringstream ss(target);
            str          item;
            while (getline(ss, item, delimiter)) result.push_back(item);
            return result;
        }
        FVV_INLINE static str _replace(str s, strv f, strv t) {
            _replaceBase(s, f, t);
            return s;
        }
        FVV_INLINE static void _replaceBase(str& s, strv f, strv t) {
            size_t p = 0;
            while ((p = s.find(f, p)) != str::npos) {
                s.replace(p, f.length(), t);
                p += t.length();
            }
        }
        FVV_INLINE static bool _isInt(strv s) {
            if (s.empty()) return false;
            size_t start = 0;
            if (s[0] == '-' || s[0] == '+') {
                if (s.size() == 1) return false;
                start = 1;
            }
            return all_of(s.begin() + start, s.end(), ::isdigit);
        }
        FVV_INLINE static bool _isDouble(strv s) {
            if (s.empty()) return false;
            size_t start    = 0;
            bool   hasDigit = false, hasDot = false;
            if (s[0] == '-' || s[0] == '+') {
                if (s.size() == 1) return false;
                start = 1;
            }
            for (size_t i = start; i < s.size(); ++i) {
                char c = s[i];
                if (isdigit(c))
                    hasDigit = true;
                else if (c == '.') {
                    if (hasDot) return false;
                    hasDot = true;
                } else
                    return false;
            }
            return hasDigit;
        }
        template <typename Tp>
        FVV_INLINE static void _shrink(Tp* container) {
            if (container) container->shrink_to_fit();
        }
        template <typename Tp, typename... Args>
        FVV_INLINE static void _shrink(Tp* container, Args... args) {
            _shrink(container);
            _shrink(args...);
        }
        template <typename Tp>
        FVV_INLINE static void _clearAndShrink(Tp* container) {
            if (container) {
                container->clear();
                container->shrink_to_fit();
            }
        }
        template <typename Tp, typename... Args>
        FVV_INLINE static void _clearAndShrink(Tp* container, Args... args) {
            _clearAndShrink(container);
            _clearAndShrink(args...);
        }
        FVV_INLINE static void _removeLastChar(stringstream& ss) {
            str str = ss.str();
            if (!str.empty()) {
                str.pop_back();
                ss.str("");
                ss.clear();
                ss << str;
            }
        }
        FVV_INLINE static void _utf8ForEach(strv target, size_t size,
                                            function<bool(const size_t&, strv, const uint8_t&)> handler) {
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
}  // namespace FVV