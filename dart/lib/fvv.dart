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

library;

import 'dart:core';

extension FVVExt on String {
  String get removeLast => substring(0, length - 1);
}

class FVVV {
  dynamic _value;
  Map<String, FVVV> sub;
  String desc, link;

  FVVV(this._value, {Map<String, FVVV>? sub, this.desc = '', this.link = ''}) : sub = sub ?? {};

  FVVV operator [](String key) => sub.putIfAbsent(key, () => FVVV(null));

  void operator []=(String key, dynamic val) =>
      sub.containsKey(key) ? sub[key]!._value = val : sub[key] = FVVV(val);

  T? as<T>() {
    if (_value is T) return _value;
    if (_value is FVVV) return _value.as<T>();
    return null;
  }

  bool asBool([bool defaultVal = false]) => as<bool>() ?? defaultVal;
  int asInt([int defaultVal = 0]) => as<int>() ?? defaultVal;
  double asDouble([double defaultVal = 0]) => as<double>() ?? defaultVal;
  String asString([String defaultVal = '']) => as<String>() ?? defaultVal;
  List<bool> asBools([List<bool>? defaultVal]) => as<List<bool>>() ?? defaultVal ?? [];
  List<int> asInts([List<int>? defaultVal]) => as<List<int>>() ?? defaultVal ?? [];
  List<double> asDoubles([List<double>? defaultVal]) => as<List<double>>() ?? defaultVal ?? [];
  List<String> asStrings([List<String>? defaultVal]) => as<List<String>>() ?? defaultVal ?? [];

  bool get isEmpty {
    if (_value == null) return true;
    if (_value is String) return (_value as String).isEmpty;
    if (_value is List) return (_value as List).isEmpty;
    return false;
  }

  bool get isNotEmpty => !isEmpty;

  bool isType<T>() => _value is T;

  Type getType() {
    if (_value is FVVV) {
      return (_value as FVVV).getType();
    } else {
      return _value.runtimeType;
    }
  }

  @override
  String toString() => _value?.toString() ?? 'null';

  String print([String type = 'common']) {
    bool isMin = false, isBiglist = false, isNodesc = false;
    switch (type) {
      case 'min':
        isMin = true;
        break;
      case 'biglist':
        isBiglist = true;
        break;
      case 'nodesc':
        isNodesc = true;
        break;
    }
    String result = '';
    void Function(String path, FVVV node, int indentLv) printFunc = (String path, FVVV node, int indentLv) {};
    printFunc = (String path, FVVV node, int indentLv) {
      String indent = ' ' * indentLv * 2;
      if (node.sub.isNotEmpty && path.isNotEmpty && node.link.isEmpty) {
        if (isMin) {
          result += '$path={';
        } else {
          result += '$indent$path = {\n';
        }
      }
      if (node.link.isNotEmpty || node._value != null) {
        if (isMin) {
          result += '$path=';
        } else {
          result += '$indent$path = ';
        }
        if (node.link.isNotEmpty) {
          result += node.link;
        } else {
          String vecIndent = ' ' * (indentLv + 1) * 2;
          switch (node._value) {
            case String v:
              result += '"${v.replaceAll('"', '\\"')}"';
              break;
            case bool v:
              result += v.toString();
              break;
            case int v:
              result += v.toString();
              break;
            case double v:
              result += v.toString();
              break;
            case List<String> v:
              result += '[';
              if (isBiglist) {
                result += '\n';
              }
              for (String value in v) {
                if (isBiglist) {
                  result += vecIndent;
                }
                result += '"${value.replaceAll('"', '\\"')}",';
                if (isBiglist) {
                  result = result.removeLast;
                  result += '\n';
                } else if (!isMin) {
                  result += ' ';
                }
              }
              if (!isBiglist) {
                result = result.removeLast;
                if (!isMin) {
                  result = result.removeLast;
                }
              } else {
                result += indent;
              }
              result += ']';
              break;
            case List<bool> v:
              result += '[';
              if (isBiglist) {
                result += '\n';
              }
              for (bool value in v) {
                if (isBiglist) {
                  result += vecIndent;
                }
                result += '$value,';
                if (isBiglist) {
                  result = result.removeLast;
                  result += '\n';
                } else if (!isMin) {
                  result += ' ';
                }
              }
              if (!isBiglist) {
                result = result.removeLast;
                if (!isMin) {
                  result = result.removeLast;
                }
              } else {
                result += indent;
              }
              result += ']';
              break;
            case List<int> v:
              result += '[';
              if (isBiglist) {
                result += '\n';
              }
              for (int value in v) {
                if (isBiglist) {
                  result += vecIndent;
                }
                result += '$value,';
                if (isBiglist) {
                  result = result.removeLast;
                  result += '\n';
                } else if (!isMin) {
                  result += ' ';
                }
              }
              if (!isBiglist) {
                result = result.removeLast;
                if (!isMin) {
                  result = result.removeLast;
                }
              } else {
                result += indent;
              }
              result += ']';
              break;
            case List<double> v:
              result += '[';
              if (isBiglist) {
                result += '\n';
              }
              for (double value in v) {
                if (isBiglist) {
                  result += vecIndent;
                }
                result += '$value,';
                if (isBiglist) {
                  result = result.removeLast;
                  result += '\n';
                } else if (!isMin) {
                  result += ' ';
                }
              }
              if (!isBiglist) {
                result = result.removeLast;
                if (!isMin) {
                  result = result.removeLast;
                }
              } else {
                result += indent;
              }
              result += ']';
              break;
          }
        }
        if (node.desc.isNotEmpty && !isMin && !isNodesc) {
          result += ' <${node.desc.replaceAll('>', '\\>')}>';
        }
        if (isMin) {
          result += ';';
        } else {
          result += '\n';
        }
      } else {
        node.sub.forEach((key, value) => printFunc(key, value, indentLv + 1));
      }
      if (node.sub.isNotEmpty && path.isNotEmpty && node.link.isEmpty) {
        if (isMin) {
          result += '}';
        } else {
          result += '$indent}';
        }
        if (node.desc.isNotEmpty && !isMin && !isNodesc) {
          result += ' <${node.desc.replaceAll('>', '\\>')}>';
        }
        if (isMin) {
          result += ';';
        } else {
          result += '\n';
        }
      }
    };
    sub.forEach((key, value) => printFunc(key, value, 0));
    return result.removeLast;
  }

  void addFromString(String txt) {
    if (txt.startsWith('\u{FEFF}')) txt = txt.substring(1);
    txt = txt.trim();
    txt.replaceAll(RegExp(r'\r\n|\r'), '\n');
    if (txt.isEmpty) return;
    if (String.fromCharCode(txt.runes.last) != '}') txt += '\n';
    bool eqOr<T>(List<T> values) {
      if (values.isEmpty) return false;
      T first = values[0];
      for (int i = 1; i < values.length; i++) {
        if (first == values[i]) return true;
      }
      return false;
    }

    int? tryInt(String s) {
      try {
        return int.parse(s);
      } catch (_) {
        return null;
      }
    }

    double? tryDouble(String s) {
      try {
        return double.parse(s);
      } catch (_) {
        return null;
      }
    }

    bool endGroup = false,
        oldFVV = false,
        isRealChar = false,
        inValue = false,
        inDesc = false,
        inStr = false,
        isStr = false,
        isEmptyStr = false,
        inList = false,
        isList = false;
    String idxDesc = '', tmpDesc = '', value = '', valueName = '', idxChar = '', lastChar = '';
    int groupNum = 0, idx = 0;
    List<String> values = [], valueNames = [], groupNames = [];
    List<List<String>> lastGroupNames = [];
    for (int rune in txt.runes) {
      idxChar = String.fromCharCode(rune);
      isRealChar = lastChar != '\\';
      if (((FVVV rootKey) {
        FVVV idxKey = rootKey;
        if (inDesc) {
          if (idxChar != '>' || !isRealChar) {
            if (idxChar == '>' && !isRealChar) tmpDesc = tmpDesc.removeLast;
            if (inValue || groupNum > 0) tmpDesc += idxChar;
            return false;
          } else if (idxChar == '>' && isRealChar) {
            idxDesc = tmpDesc;
            tmpDesc = '';
            inDesc = false;
            return false;
          }
        } else {
          if (!inStr && eqOr([idxChar, ' ', '\t'])) {
            return false;
          } else if (idxChar == '<') {
            inDesc = true;
            return false;
          }
        }
        if (inValue) {
          if (inStr) {
            if (idxChar == '"') {
              if (isRealChar) {
                if (value.isEmpty) {
                  isEmptyStr = true;
                } else {
                  isEmptyStr = false;
                }
                inStr = false;
                return false;
              } else {
                value = value.removeLast;
                value += idxChar;
                return false;
              }
            } else {
              value += idxChar;
              return false;
            }
          } else {
            if (idxChar == '"') {
              inStr = isStr = true;
              return false;
            } else if (idxChar == '[') {
              inList = isList = true;
              return false;
            } else if (inList && eqOr([idxChar, ',', ']', '\n'])) {
              if (idxChar == ']') {
                inList = false;
                int j = idxChar.length;
                while (eqOr([txt[idx - j], ' ', '\t'])) {
                  j++;
                }
                if (txt[idx - j] == ',' || txt[idx - j] == '\n') return false;
              } else if (value.isEmpty && (!isStr || !isEmptyStr)) {
                return false;
              }
              values.add(value);
              if (isEmptyStr) {
                isEmptyStr = false;
              } else {
                value = '';
              }
              return false;
            } else if (idxChar == '{') {
              groupNames.addAll(valueNames);
              lastGroupNames.add(valueNames.toList());
              valueNames.clear();
              groupNum++;
              inValue = false;
              return false;
            } else if (!inList && eqOr([idxChar, ';', '\n'])) {
              for (String key in groupNames) {
                idxKey = idxKey[key];
              }
              for (int idx = 0; idx < valueNames.length; idx++) {
                String key = valueNames[idx];
                if (idx == valueNames.length - 1) {
                  if (isList) {
                    if (isStr) {
                      idxKey[key] = values.toList();
                    } else {
                      if (values.isEmpty) {
                        idxKey[key] = null;
                        return false;
                      }
                      String tmpStr = values[0];
                      if (eqOr([tmpStr, 'true', 'false'])) {
                        List<bool> tmps = [];
                        for (var s in values) {
                          tmps.add(s == "true");
                        }
                        idxKey[key] = tmps;
                      } else if (tryInt(tmpStr) != null) {
                        List<int> tmps = [];
                        for (var s in values) {
                          tmps.add(tryInt(s)!);
                        }
                        idxKey[key] = tmps;
                      } else if (tryDouble(tmpStr) != null) {
                        List<double> tmps = [];
                        for (var s in values) {
                          tmps.add(tryDouble(s)!);
                        }
                        idxKey[key] = tmps;
                      }
                    }
                  } else if (isStr) {
                    idxKey[key] = value;
                  } else if (eqOr([value, 'true', 'false'])) {
                    idxKey[key] = value == 'true';
                  } else if (tryInt(value) != null) {
                    idxKey[key] = tryInt(value);
                  } else if (tryDouble(value) != null) {
                    idxKey[key] = tryDouble(value);
                  } else {
                    List<String> tmpNames = value.trim().split(".");
                    FVVV tmpKey = idxKey;
                    for (String key in tmpNames) {
                      if (tmpKey.sub.containsKey(key)) {
                        tmpKey = tmpKey[key];
                      } else {
                        break;
                      }
                    }
                    if (tmpKey.isEmpty || tmpKey.sub.isEmpty) {
                      tmpKey = rootKey;
                      for (String key in tmpNames) {
                        if (tmpKey.sub.containsKey(key)) {
                          tmpKey = tmpKey[key];
                        } else {
                          break;
                        }
                      }
                    }
                    if (tmpKey._value != null || tmpKey.sub.isNotEmpty) {
                      if (tmpKey.sub.isEmpty) {
                        idxKey[key] = tmpKey;
                      } else {
                        idxKey[key].sub = tmpKey.sub;
                      }
                    }
                    idxKey[key].link = value;
                  }
                  idxKey[key].desc = idxDesc;
                  idxDesc = value = '';
                  values.clear();
                  valueNames.clear();
                  inValue = isStr = isList = false;
                  continue;
                } else {
                  idxKey = idxKey[key];
                }
              }
            } else {
              value += idxChar;
              return false;
            }
          }
        } else {
          if (!oldFVV && idxChar == '{' && valueName.isEmpty) {
            oldFVV = true;
            return false;
          } else if (idxChar == '=') {
            valueNames = valueName.trim().split(".");
            valueName = "";
            inValue = true;
            return false;
          } else if (endGroup && eqOr([idxChar, ';', '\n']) && groupNum > 0) {
            endGroup = false;
            if (idxDesc.isNotEmpty) {
              for (int idx = 0; idx < groupNames.length; idx++) {
                String key = groupNames[idx];
                if (idx == groupNames.length - 1) {
                  idxKey[key].desc = idxDesc;
                  idxDesc = "";
                  break;
                }
                idxKey = idxKey[key];
              }
            }
            for (String _ in lastGroupNames.last) {
              groupNames = groupNames.sublist(0, groupNames.length - 1);
            }
            lastGroupNames = lastGroupNames.sublist(
              0,
              lastGroupNames.length - 1,
            );
            groupNum--;
            return false;
          } else if (idxChar == '}') {
            if (groupNum == 0) {
              return true;
            } else {
              endGroup = true;
              return false;
            }
          } else {
            valueName += idxChar;
            return false;
          }
        }
        return false;
      })(this)) {
        break;
      }
      idx += idxChar.length;
      lastChar = idxChar;
    }
  }
}
