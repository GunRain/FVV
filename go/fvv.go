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

package fvv

import (
	"bytes"
	"strconv"
	"strings"
)

func _eq_or[T comparable](a T, args ...T) bool {
	for _, v := range args {
		if a == v {
			return true
		}
	}
	return false
}

type FVVV struct {
	Value any
	Sub   map[string]*FVVV
	Desc  string
	Link  string
}

func (fvvv *FVVV) IsEmpty() bool {
	return len(fvvv.Sub) == 0 && fvvv.Value == nil
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
	var result string
	var print_func func(path string, node *FVVV, indent_lv int)
	print_func = func(path string, node *FVVV, indent_lv int) {
		indent := strings.Repeat(" ", indent_lv*2)
		if node.SubIsNotEmpty() && path != "" {
			if is_min {
				result += path + "={"
			} else {
				result += indent + path + " = {\n"
			}
		}
		if node.SubIsEmpty() && node.IsNotEmpty() {
			if is_min {
				result += path + "="
			} else {
				result += indent + path + " = "
			}
			if node.Link != "" {
				result += node.Link
			} else {
				vec_indent := strings.Repeat(" ", (indent_lv+1)*2)
				switch v := node.Value.(type) {
				case string:
					result += `"` + strings.ReplaceAll(v, `"`, `\"`) + `"`
				case bool:
					result += strconv.FormatBool(v)
				case int:
					result += strconv.Itoa(v)
				case float64:
					result += strconv.FormatFloat(v, 'f', -1, 64)
				case []string:
					result += "["
					if is_biglist {
						result += "\n"
					}
					for _, value := range v {
						if is_biglist {
							result += vec_indent
						}
						result += `"` + strings.ReplaceAll(value, `"`, `\"`) + `",`
						if is_biglist {
							result = result[:len(result)-1]
							result += "\n"
						} else if !is_min {
							result += " "
						}
					}
					if !is_biglist {
						result = result[:len(result)-1]
						if !is_min {
							result = result[:len(result)-1]
						}
					} else {
						result += indent
					}
					result += "]"
				case []bool:
					result += "["
					if is_biglist {
						result += "\n"
					}
					for _, value := range v {
						if is_biglist {
							result += vec_indent
						}
						result += strconv.FormatBool(value) + ","
						if is_biglist {
							result = result[:len(result)-1]
							result += "\n"
						} else if !is_min {
							result += " "
						}
					}
					if !is_biglist {
						result = result[:len(result)-1]
						if !is_min {
							result = result[:len(result)-1]
						}
					} else {
						result += indent
					}
					result += "]"
				case []int:
					result += "["
					if is_biglist {
						result += "\n"
					}
					for _, value := range v {
						if is_biglist {
							result += vec_indent
						}
						result += strconv.Itoa(value) + ","
						if is_biglist {
							result = result[:len(result)-1]
							result += "\n"
						} else if !is_min {
							result += " "
						}
					}
					if !is_biglist {
						result = result[:len(result)-1]
						if !is_min {
							result = result[:len(result)-1]
						}
					} else {
						result += indent
					}
					result += "]"
				case []float64:
					result += "["
					if is_biglist {
						result += "\n"
					}
					for _, value := range v {
						if is_biglist {
							result += vec_indent
						}
						result += strconv.FormatFloat(value, 'f', -1, 64) + ","
						if is_biglist {
							result = result[:len(result)-1]
							result += "\n"
						} else if !is_min {
							result += " "
						}
					}
					if !is_biglist {
						result = result[:len(result)-1]
						if !is_min {
							result = result[:len(result)-1]
						}
					} else {
						result += indent
					}
					result += "]"
				}
			}
			if node.Desc != "" && !is_min && !is_nodesc {
				result += " <" + strings.ReplaceAll(node.Desc, `>`, `\>`) + ">"
			}
			if is_min {
				result += ";"
			} else {
				result += "\n"
			}
		} else {
			for key, value := range node.Sub {
				print_func(key, value, indent_lv+1)
			}
		}
		if node.SubIsNotEmpty() && path != "" {
			if is_min {
				result += "}"
			} else {
				result += indent + "}"
			}
			if node.Desc != "" && !is_min && !is_nodesc {
				result += " <" + strings.ReplaceAll(node.Desc, `>`, `\>`) + ">"
			}
			if is_min {
				result += ";"
			} else {
				result += "\n"
			}
		}
	}
	for key, value := range fvvv.Sub {
		print_func(key, value, 0)
	}
	return result[:len(result)-1]
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
	var idx_desc, tmp_desc, value, value_name string
	var group_num uint64
	values, value_names, group_names, last_group_names := make([]string, 0), make([]string, 0), make([]string, 0), make([][]string, 0)
	for idx, idx_char := range txt {
		is_real_char = last_char != '\\'
		if func() bool {
			idx_key := fvvv
			if in_desc {
				if idx_char != '>' || !is_real_char {
					if idx_char == '>' && !is_real_char {
						tmp_desc = tmp_desc[:len(tmp_desc)-1]
					}
					if in_value || group_num > 0 {
						tmp_desc += string(idx_char)
					}
					return false
				} else if idx_char == '>' && is_real_char {
					idx_desc = tmp_desc
					tmp_desc = ""
					in_desc = false
					return false
				}
			} else {
				if !in_str && _eq_or(idx_char, ' ', '\t') {
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
							if value == "" {
								is_empty_str = true
							} else {
								is_empty_str = false
							}
							in_str = false
							return false
						} else {
							value = value[:len(value)-1]
							value += string(idx_char)
							return false
						}
					} else {
						value += string(idx_char)
						return false
					}
				} else {
					if idx_char == '"' {
						in_str, is_str = true, true
						return false
					} else if idx_char == '[' {
						in_list, is_list = true, true
						return false
					} else if in_list && _eq_or(idx_char, ',', ']', '\n') {
						if idx_char == ']' {
							in_list = false
							j := 1
							for _eq_or(txt[idx-j], ' ', '\t') {
								j++
							}
							if txt[idx-j] == ',' || txt[idx-j] == '\n' {
								return false
							}
						} else if value == "" && (!is_str || !is_empty_str) {
							return false
						}
						values = append(values, value)
						if is_empty_str {
							is_empty_str = false
						} else {
							value = ""
						}
						return false
					} else if idx_char == '{' {
						group_names = append(group_names, value_names...)
						last_group_names = append(last_group_names, value_names)
						value_names = nil
						group_num++
						in_value = false
						return false
					} else if !in_list && _eq_or(idx_char, ';', '\n') {
						for _, key := range group_names {
							if idx_key.Sub == nil {
								idx_key.Sub = make(map[string]*FVVV)
							}
							if idx_key.Sub[key] == nil {
								idx_key.Sub[key] = &FVVV{
									Value: value,
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
										Value: value,
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
										if _eq_or(tmp_str, "true", "false") {
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
									idx_key.Sub[key].Value = value
								} else if _eq_or(value, "true", "false") {
									idx_key.Sub[key].Value = value == "true"
								} else if tmp, err := strconv.Atoi(value); err == nil {
									idx_key.Sub[key].Value = tmp
								} else if tmp, err := strconv.ParseFloat(value, 64); err == nil {
									idx_key.Sub[key].Value = tmp
								} else {
									tmp_names := strings.Split(strings.TrimSpace(value), ".")
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
									idx_key.Sub[key].Link = value
								}
								idx_key.Sub[key].Desc = idx_desc
								idx_desc, value, values, value_names, in_value, is_str, is_list = "", "", nil, nil, false, false, false
								continue
							} else {
								if idx_key.Sub == nil {
									idx_key.Sub = make(map[string]*FVVV)
								}
								if idx_key.Sub[key] == nil {
									idx_key.Sub[key] = &FVVV{
										Value: value,
										Sub:   make(map[string]*FVVV),
									}
								}
								idx_key = idx_key.Sub[key]
							}
						}
					} else {
						value += string(idx_char)
						return false
					}
				}
			} else {
				if !old_fvv && idx_char == '{' && value_name == "" {
					old_fvv = true
					return false
				} else if idx_char == '=' {
					value_names = strings.Split(strings.TrimSpace(value_name), ".")
					value_name = ""
					in_value = true
					return false
				} else if end_group && _eq_or(idx_char, ';', '\n') && group_num > 0 {
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
									Value: value,
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
					value_name += string(idx_char)
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
