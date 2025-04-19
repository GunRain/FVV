//====================================================================================================
// Copyright (c) 2023-present Anne Sakitin (Tianwan Ayana).                                          =
//                                                                                                   =
// Licensed under the F2DLPR License.                                                                =
//                                                                                                   =
// YOU MAY NOT USE THIS FILE EXCEPT IN COMPLIANCE WITH THE LICENSE.                                  =
// Provided "AS IS", WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,                                   =
// unless required by applicable law or agreed to in writing.                                        =
//                                                                                                   =
// For details about the F2DLPR License terms and conditions, visit: http://license.fileto.download. =
//====================================================================================================

package fvv

import (
	"bytes"
	"strconv"
	"strings"
)

func eqOr[Tp comparable](a Tp, args ...Tp) bool {
	for _, v := range args {
		if a == v {
			return true
		}
	}
	return false
}

func removeLastChar(builder *strings.Builder) {
	runes := []rune(builder.String())
	if len(runes) > 0 {
		runes = runes[:len(runes)-1]
		builder.Reset()
		builder.WriteString(string(runes))
	}
}

type FVVV struct {
	Value any
	Sub   map[string]*FVVV
	Desc  string
	Link  string
}

func (fvvv *FVVV) IsEmpty() bool {
	return fvvv.Value == nil
}

func (fvvv *FVVV) IsNotEmpty() bool {
	return !fvvv.IsEmpty()
}

func (fvvv *FVVV) SubIsEmpty() bool {
	return len(fvvv.Sub) == 0
}

func (fvvv *FVVV) SubIsNotEmpty() bool {
	return !fvvv.SubIsEmpty()
}

func (fvvv *FVVV) Print(tp ...string) string {
	var is_min, is_biglist, is_nodesc bool
	if len(tp) > 0 {
		is_min, is_biglist, is_nodesc = tp[0] == "min", tp[0] == "biglist", tp[0] == "nodesc"
	}
	var result strings.Builder
	var print_func func(path string, node *FVVV, indent_lv int)
	print_func = func(path string, node *FVVV, indent_lv int) {
		if path == "" || (node.IsEmpty() && node.SubIsEmpty()) {
			return
		}
		indent := strings.Repeat(" ", indent_lv*2)
		if node.SubIsNotEmpty() && node.Link == "" {
			if is_min {
				result.WriteString(path + "={")
			} else {
				result.WriteString(indent + path + " = {\n")
			}
		}
		if node.Link != "" || node.IsNotEmpty() {
			if is_min {
				result.WriteString(path + "=")
			} else {
				result.WriteString(indent + path + " = ")
			}
			if node.Link != "" {
				result.WriteString(node.Link)
			} else {
				switch v := node.Value.(type) {
				case string:
					result.WriteString(`"` + strings.ReplaceAll(v, `"`, `\"`) + `"`)
				case bool:
					result.WriteString(strconv.FormatBool(v))
				case int:
					result.WriteString(strconv.Itoa(v))
				case float64:
					result.WriteString(strconv.FormatFloat(v, 'f', -1, 64))
				case []string, []bool, []int, []float64:
					vec_indent := strings.Repeat(" ", (indent_lv+1)*2)
					result.WriteString("[")
					if is_biglist {
						result.WriteString("\n")
					}
					is_empty_list := true
					switch v := node.Value.(type) {
					case []string:
						is_empty_list = len(v) == 0
						for _, value := range v {
							if is_biglist {
								result.WriteString(vec_indent)
							}
							result.WriteString(`"` + strings.ReplaceAll(value, `"`, `\"`) + `"`)
							if is_biglist {
								removeLastChar(&result)
								result.WriteString("\n")
							} else {
								result.WriteString(",")
								if !is_min {
									result.WriteString(" ")
								}
							}
						}
					case []bool:
						is_empty_list = len(v) == 0
						for _, value := range v {
							if is_biglist {
								result.WriteString(vec_indent)
							}
							result.WriteString(strconv.FormatBool(value) + ",")
							if is_biglist {
								removeLastChar(&result)
								result.WriteString("\n")
							} else {
								result.WriteString(",")
								if !is_min {
									result.WriteString(" ")
								}
							}
						}
					case []int:
						is_empty_list = len(v) == 0
						for _, value := range v {
							if is_biglist {
								result.WriteString(vec_indent)
							}
							result.WriteString(strconv.Itoa(value) + ",")
							if is_biglist {
								removeLastChar(&result)
								result.WriteString("\n")
							} else {
								result.WriteString(",")
								if !is_min {
									result.WriteString(" ")
								}
							}
						}
					case []float64:
						is_empty_list = len(v) == 0
						for _, value := range v {
							if is_biglist {
								result.WriteString(vec_indent)
							}
							result.WriteString(strconv.FormatFloat(value, 'f', -1, 64) + ",")
							if is_biglist {
								removeLastChar(&result)
								result.WriteString("\n")
							} else {
								result.WriteString(",")
								if !is_min {
									result.WriteString(" ")
								}
							}
						}
					}
					if is_biglist {
						result.WriteString(indent)
					} else if !is_empty_list {
						removeLastChar(&result)
						if !is_min {
							removeLastChar(&result)
						}
					}
					result.WriteString("]")
				}
			}
		} else {
			for key, value := range node.Sub {
				print_func(key, value, indent_lv+1)
			}
		}
		if node.SubIsNotEmpty() && node.Link == "" {
			if !is_min {
				result.WriteString(indent)
			}
			result.WriteString("}")
		}
		if node.Desc != "" && !is_min && !is_nodesc {
			result.WriteString(" <" + strings.ReplaceAll(node.Desc, `>`, `\>`) + ">")
		}
		if is_min {
			result.WriteString(";")
		} else {
			result.WriteString("\n")
		}
	}
	for key, value := range fvvv.Sub {
		print_func(key, value, 0)
	}
	removeLastChar(&result)
	return result.String()
}

func (fvvv *FVVV) AddFromString(txt string) {
	txtBytes := []byte(txt)
	if len(txtBytes) >= 3 && txtBytes[0] == 0xEF && txtBytes[1] == 0xBB && txtBytes[2] == 0xBF {
		txtBytes = txtBytes[3:]
	}
	txtBytes = bytes.ReplaceAll(txtBytes, []byte("\r\n"), []byte("\n"))
	txtBytes = bytes.ReplaceAll(txtBytes, []byte("\r"), []byte("\n"))
	txt = strings.TrimSpace(string(txtBytes)) + "\n"
	if txt == "" {
		return
	}
	var last_char rune
	var end_group, old_fvv, is_real_char, in_value, in_desc, in_str, is_str, is_empty_str, in_list, is_list bool
	var tmp_desc, value, value_name strings.Builder
	var idx_desc string
	var group_num uint64
	values, value_names, group_names, last_group_names := make([]string, 0), make([]string, 0), make([]string, 0), make([][]string, 0)
	for idx, idx_char := range txt {
		is_real_char = last_char != '\\'
		if func() bool {
			idx_key := fvvv
			if in_desc {
				if idx_char != '>' || !is_real_char {
					if idx_char == '>' && !is_real_char {
						removeLastChar(&tmp_desc)
					}
					if in_value || group_num > 0 {
						tmp_desc.WriteRune(idx_char)
					}
					return false
				} else if idx_char == '>' && is_real_char {
					idx_desc = tmp_desc.String()
					tmp_desc.Reset()
					in_desc = false
					return false
				}
			} else {
				if !in_str && eqOr(idx_char, ' ', '\t') {
					return false
				} else if idx_char == '<' {
					in_desc = true
					return false
				}
			}
			if in_value {
				if in_str {
					if idx_char == '"' {
						if is_real_char {
							if value.String() == "" {
								is_empty_str = true
							} else {
								is_empty_str = false
							}
							in_str = false
							return false
						} else {
							removeLastChar(&value)
							value.WriteRune(idx_char)
							return false
						}
					} else {
						value.WriteRune(idx_char)
						return false
					}
				} else {
					if idx_char == '"' {
						in_str, is_str = true, true
						return false
					} else if idx_char == '[' {
						in_list, is_list = true, true
						return false
					} else if in_list && eqOr(idx_char, ',', ']', '\n') {
						if idx_char == ']' {
							in_list = false
							pos := 1
							in_list_desc := false
							for {
								if func() bool {
									switch txt[idx-pos] {
									case '<':
										if !in_list_desc {
											return true
										}
										if idx-pos < 1 || txt[idx-pos-1] != '\\' {
											in_list_desc = false
										}
										return false
									case '>':
										in_list_desc = true
										return false
									case ' ', '\t':
										return false
									case ',', '\n':
										if in_list_desc {
											return false
										}
										return true
									default:
										if in_list_desc {
											return false
										}
										return true
									}
								}() {
									break
								}
								pos++
							}
							if txt[idx-pos] == ',' || txt[idx-pos] == '\n' {
								return false
							}
						} else if value.String() == "" && (!is_str || !is_empty_str) {
							return false
						}
						values = append(values, value.String())
						if is_empty_str {
							is_empty_str = false
						} else {
							value.Reset()
						}
						return false
					} else if idx_char == '{' {
						group_names = append(group_names, value_names...)
						last_group_names = append(last_group_names, value_names)
						value_names = nil
						group_num++
						in_value = false
						return false
					} else if !in_list && eqOr(idx_char, ';', '\n') {
						for _, key := range group_names {
							if idx_key.Sub == nil {
								idx_key.Sub = make(map[string]*FVVV)
							}
							if idx_key.Sub[key] == nil {
								idx_key.Sub[key] = &FVVV{
									Value: nil,
									Sub:   make(map[string]*FVVV),
								}
							}
							idx_key = idx_key.Sub[key]
						}
						for idx, key := range value_names {
							if idx == len(value_names)-1 {
								if idx_key.Sub == nil {
									idx_key.Sub = make(map[string]*FVVV)
								}
								if idx_key.Sub[key] == nil {
									idx_key.Sub[key] = &FVVV{
										Value: nil,
										Sub:   make(map[string]*FVVV),
									}
								}
								if is_list {
									if is_str {
										idx_key.Sub[key].Value = values
									} else {
										if len(values) == 0 {
											idx_key.Sub[key].Value = nil
											return false
										}
										tmp_str := values[0]
										if eqOr(tmp_str, "true", "false") {
											tmps := make([]bool, len(values))
											for i, s := range values {
												tmps[i] = s == "true"
											}
											idx_key.Sub[key].Value = tmps
										} else if _, err := strconv.Atoi(tmp_str); err == nil {
											tmps := make([]int, len(values))
											for i, s := range values {
												if tmp, err := strconv.Atoi(s); err == nil {
													tmps[i] = tmp
												}
											}
											idx_key.Sub[key].Value = tmps
										} else if _, err := strconv.ParseFloat(tmp_str, 64); err == nil {
											tmps := make([]float64, len(values))
											for i, s := range values {
												if tmp, err := strconv.ParseFloat(s, 64); err == nil {
													tmps[i] = tmp
												}
											}
											idx_key.Sub[key].Value = tmps
										}
									}
								} else if is_str {
									idx_key.Sub[key].Value = value.String()
								} else if eqOr(value.String(), "true", "false") {
									idx_key.Sub[key].Value = value.String() == "true"
								} else if tmp, err := strconv.Atoi(value.String()); err == nil {
									idx_key.Sub[key].Value = tmp
								} else if tmp, err := strconv.ParseFloat(value.String(), 64); err == nil {
									idx_key.Sub[key].Value = tmp
								} else {
									tmp_names := strings.Split(strings.TrimSpace(value.String()), ".")
									tmp_key := idx_key
									for _, key := range tmp_names {
										if tmp_key.Sub[key] == nil {
											tmp_key = nil
											break
										} else {
											tmp_key = tmp_key.Sub[key]
										}
									}
									if tmp_key != nil {
										tmp_key = fvvv
										for _, key := range tmp_names {
											if tmp_key.Sub[key] == nil {
												tmp_key = nil
												break
											} else {
												tmp_key = tmp_key.Sub[key]
											}
										}
									}
									if tmp_key != nil {
										if tmp_key.Sub == nil {
											idx_key.Sub[key].Value = tmp_key.Value
										} else {
											idx_key.Sub[key].Sub = tmp_key.Sub
										}
									}
									idx_key.Sub[key].Link = value.String()
								}
								idx_key.Sub[key].Desc = idx_desc
								idx_desc, values, value_names, in_value, is_str, is_list = "", nil, nil, false, false, false
								value.Reset()
								continue
							} else {
								if idx_key.Sub == nil {
									idx_key.Sub = make(map[string]*FVVV)
								}
								if idx_key.Sub[key] == nil {
									idx_key.Sub[key] = &FVVV{
										Value: nil,
										Sub:   make(map[string]*FVVV),
									}
								}
								idx_key = idx_key.Sub[key]
							}
						}
					} else {
						value.WriteRune(idx_char)
						return false
					}
				}
			} else {
				if !old_fvv && idx_char == '{' && value_name.String() == "" {
					old_fvv = true
					return false
				} else if idx_char == '=' {
					value_names = strings.Split(strings.TrimSpace(value_name.String()), ".")
					value_name.Reset()
					in_value = true
					return false
				} else if end_group && eqOr(idx_char, ';', '\n') && group_num > 0 {
					end_group = false
					if idx_desc != "" {
						for idx, key := range group_names {
							if idx == len(group_names)-1 {
								idx_key.Sub[key].Desc = idx_desc
								idx_desc = ""
								break
							}
							if idx_key.Sub == nil {
								idx_key.Sub = make(map[string]*FVVV)
							}
							if idx_key.Sub[key] == nil {
								idx_key.Sub[key] = &FVVV{
									Value: nil,
									Sub:   make(map[string]*FVVV),
								}
							}
							idx_key = idx_key.Sub[key]
						}
					}
					for range last_group_names[len(last_group_names)-1] {
						group_names = group_names[:len(group_names)-1]
					}
					last_group_names = last_group_names[:len(last_group_names)-1]
					group_num--
					return false
				} else if idx_char == '}' {
					if group_num == 0 {
						return true
					} else {
						end_group = true
						return false
					}
				} else {
					value_name.WriteRune(idx_char)
					return false
				}
			}
			return false
		}() {
			break
		}
		last_char = idx_char
	}
}

func NewFVVV(value any) *FVVV {
	return &FVVV{
		Value: value,
		Sub:   make(map[string]*FVVV),
	}
}
