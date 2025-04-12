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
  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;
    return other is FVVV && _value == other._value;
  }

  @override
  int get hashCode => _value.hashCode;
  @override
  String toString() => _value?.toString() ?? 'null';

  T? as<T>([T? dfltVal]) {
    if (_value is T) return _value;
    if (_value is FVVV) return _value.as<T>(dfltVal);
    return dfltVal;
  }

  bool asBool([bool dfltVal = false]) => as<bool>() ?? dfltVal;
  int asInt([int dfltVal = 0]) => as<int>() ?? dfltVal;
  double asDouble([double dfltVal = 0]) => as<double>() ?? dfltVal;
  String asString([String dfltVal = '']) => as<String>() ?? dfltVal;
  List<bool> asBools([List<bool>? dfltVal]) => (as<List<bool>>() ?? dfltVal ?? []).toList();
  List<int> asInts([List<int>? dfltVal]) => (as<List<int>>() ?? dfltVal ?? []).toList();
  List<double> asDoubles([List<double>? dfltVal]) => (as<List<double>>() ?? dfltVal ?? []).toList();
  List<String> asStrings([List<String>? dfltVal]) => (as<List<String>>() ?? dfltVal ?? []).toList();
  List<bool> asBoolsRef([List<bool>? dfltVal]) => as<List<bool>>() ?? dfltVal ?? [];
  List<int> asIntsRef([List<int>? dfltVal]) => as<List<int>>() ?? dfltVal ?? [];
  List<double> asDoublesRef([List<double>? dfltVal]) => as<List<double>>() ?? dfltVal ?? [];
  List<String> asStringsRef([List<String>? dfltVal]) => as<List<String>>() ?? dfltVal ?? [];

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
            case List _:
              result += '[';
              if (isBiglist) result += '\n';
              switch (node._value) {
                case List<String> v:
                  for (String value in v) {
                    if (isBiglist) result += vecIndent;
                    result += '"${value.replaceAll('"', '\\"')}",';
                    if (isBiglist) {
                      result = result.removeLast;
                      result += '\n';
                    } else if (!isMin) {
                      result += ' ';
                    }
                  }
                  break;
                case List<bool> v:
                  for (bool value in v) {
                    if (isBiglist) result += vecIndent;
                    result += '$value,';
                    if (isBiglist) {
                      result = result.removeLast;
                      result += '\n';
                    } else if (!isMin) {
                      result += ' ';
                    }
                  }
                  break;
                case List<int> v:
                  for (int value in v) {
                    if (isBiglist) result += vecIndent;
                    result += '$value,';
                    if (isBiglist) {
                      result = result.removeLast;
                      result += '\n';
                    } else if (!isMin) {
                      result += ' ';
                    }
                  }
                  break;
                case List<double> v:
                  for (double value in v) {
                    if (isBiglist) result += vecIndent;
                    result += '$value,';
                    if (isBiglist) {
                      result = result.removeLast;
                      result += '\n';
                    } else if (!isMin) {
                      result += ' ';
                    }
                  }
                  break;
              }
              if (!isBiglist && (node._value as List).isNotEmpty) {
                result = result.removeLast;
                if (!isMin && (node._value as List).isNotEmpty) result = result.removeLast;
              } else {
                result += indent;
              }
              result += ']';
              break;
          }
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
      }
      if (node.desc.isNotEmpty && !isMin && !isNodesc) result += ' <${node.desc.replaceAll('>', '\\>')}>';
      if (isMin) {
        result += ';';
      } else {
        result += '\n';
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
                int pos = idxChar.length;
                bool inListDesc = false;
                for (;;) {
                  if (() {
                    switch (txt[idx - pos]) {
                      case '<':
                        if (!inListDesc) return true;
                        if (idx - pos < 1 || txt[idx - pos - 1] != '\\') inListDesc = false;
                        return false;
                      case '>':
                        inListDesc = true;
                        return false;
                      case ' ':
                      case '\t':
                        return false;
                      case ',':
                      case '\n':
                      default:
                        if (inListDesc) return false;
                        return true;
                    }
                  }()) {
                    break;
                  }
                  pos++;
                }
                if (txt[idx - pos] == ',' || txt[idx - pos] == '\n') return false;
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
