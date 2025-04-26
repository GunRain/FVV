//====================================================================================================
// Copyright (C) 2023-present Anne Sakitin (Tianwan Ayana).                                          =
//                                                                                                   =
// Licensed under the F2DLPR License.                                                                =
//                                                                                                   =
// YOU MAY NOT USE THIS FILE EXCEPT IN COMPLIANCE WITH THE LICENSE.                                  =
// Provided "AS IS", WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,                                   =
// unless required by applicable law or agreed to in writing.                                        =
//                                                                                                   =
// For details about the F2DLPR License terms and conditions, visit: http://license.fileto.download. =
//====================================================================================================

import 'dart:core';

extension FVVStrExt on String {
  String get removeLastChar => substring(0, length - 1);
}

extension FVVStrBfExt on StringBuffer {
  void removeLastChar() {
    String result = toString();
    if (result.isEmpty) return;
    result = result.substring(0, result.length - 1);
    clear();
    write(result);
  }
}

class FVVV {
  FVVV(this.value, {final Map<String, FVVV>? sub, this.desc = '', this.link = ''})
      : sub = sub ?? <String, FVVV>{};
  dynamic value;
  Map<String, FVVV> sub;
  String desc, link;

  FVVV operator [](final String key) => sub.putIfAbsent(key, () => FVVV(null));
  void operator []=(final String key, final dynamic val) =>
      sub.containsKey(key) ? sub[key]!.value = val : sub[key] = FVVV(val);
  @override
  bool operator ==(final Object other) => identical(this, other) || (other is FVVV && value == other.value);

  @override
  int get hashCode => value.hashCode;
  @override
  String toString() => value?.toString() ?? 'null';

  T? as<T>([final T? dfltVal]) {
    if (value is T) return value as T;
    if (value is FVVV) return (value as FVVV).as<T>(dfltVal) as T;
    return dfltVal;
  }

  bool asBool([final bool dfltVal = false]) => as<bool>() ?? dfltVal;
  int asInt([final int dfltVal = 0]) => as<int>() ?? dfltVal;
  double asDouble([final double dfltVal = 0]) => as<double>() ?? dfltVal;
  String asString([final String dfltVal = '']) => as<String>() ?? dfltVal;
  List<bool> asBools([final List<bool>? dfltVal]) => (as<List<bool>>() ?? dfltVal ?? <bool>[]).toList();
  List<int> asInts([final List<int>? dfltVal]) => (as<List<int>>() ?? dfltVal ?? <int>[]).toList();
  List<double> asDoubles([final List<double>? dfltVal]) => (as<List<double>>() ?? dfltVal ?? <double>[]).toList();
  List<String> asStrings([final List<String>? dfltVal]) => (as<List<String>>() ?? dfltVal ?? <String>[]).toList();
  List<bool> asBoolsRef([final List<bool>? dfltVal]) => as<List<bool>>() ?? dfltVal ?? <bool>[];
  List<int> asIntsRef([final List<int>? dfltVal]) => as<List<int>>() ?? dfltVal ?? <int>[];
  List<double> asDoublesRef([final List<double>? dfltVal]) => as<List<double>>() ?? dfltVal ?? <double>[];
  List<String> asStringsRef([final List<String>? dfltVal]) => as<List<String>>() ?? dfltVal ?? <String>[];

  bool get isEmpty {
    if (value == null) return true;
    if (value is String) return (value as String).isEmpty;
    // ignore: always_specify_types
    if (value is List) return (value as List).isEmpty;
    if (value is FVVV) return (value as FVVV).isEmpty;
    return false;
  }

  bool get isNotEmpty => !isEmpty;

  bool isType<T>() => getType() is T;
  Type getType() => value is FVVV ? (value as FVVV).getType() : value.runtimeType;

  String print([final String type = 'common']) {
    bool isMin = false, isBiglist = false, isNodesc = false;
    switch (type) {
      case 'min':
        isMin = true;
      case 'biglist':
        isBiglist = true;
      case 'nodesc':
        isNodesc = true;
    }
    final StringBuffer result = StringBuffer();
    Null Function(String path, FVVV node, int indentLv) printFunc =
        (final String path, final FVVV node, final int indentLv) {};
    printFunc = (final String path, final FVVV node, final int indentLv) {
      if (path.isEmpty || (isEmpty && sub.isEmpty)) return;
      final String indent = ' ' * indentLv * 2;
      if (node.sub.isNotEmpty && node.link.isEmpty) if (isMin)
        result.write('$path={');
      else
        result.write('$indent$path = {\n');
      if (node.link.isNotEmpty || node.value != null) {
        if (isMin)
          result.write('$path=');
        else
          result.write('$indent$path = ');
        if (node.link.isNotEmpty)
          result.write(node.link);
        else {
          final String vecIndent = ' ' * (indentLv + 1) * 2;
          switch (node.value) {
            case final String v:
              result.write('"${v.replaceAll('"', r'\"')}"');
            case final bool v:
              result.write(v.toString());
            case final int v:
              result.write(v.toString());
            case final double v:
              result.write(v.toString());
            case List<dynamic> _:
              result.write('[');
              if (isBiglist) result.write('\n');
              switch (node.value) {
                case final List<String> v:
                  for (final String value in v) {
                    if (isBiglist) result.write(vecIndent);
                    result.write('"${value.replaceAll('"', r'\"')}"');
                    if (isBiglist)
                      result.write('\n');
                    else {
                      result.write(',');
                      if (!isMin) result.write(' ');
                    }
                  }
                case final List<bool> v:
                  for (final bool value in v) {
                    if (isBiglist) result.write(vecIndent);
                    result.write(value.toString());
                    if (isBiglist)
                      result.write('\n');
                    else {
                      result.write(',');
                      if (!isMin) result.write(' ');
                    }
                  }
                case final List<int> v:
                  for (final int value in v) {
                    if (isBiglist) result.write(vecIndent);
                    result.write(value.toString());
                    if (isBiglist)
                      result.write('\n');
                    else {
                      result.write(',');
                      if (!isMin) result.write(' ');
                    }
                  }
                case final List<double> v:
                  for (final double value in v) {
                    if (isBiglist) result.write(vecIndent);
                    result.write(value.toString());
                    if (isBiglist)
                      result.write('\n');
                    else {
                      result.write(',');
                      if (!isMin) result.write(' ');
                    }
                  }
              }
              if (isBiglist)
                result.write(indent);
              // ignore: always_specify_types
              else if ((node.value as List).isNotEmpty) {
                result.removeLastChar();
                if (!isMin) result.removeLastChar();
              }
              result.write(']');
          }
        }
      } else
        node.sub.forEach((final String key, final FVVV value) => printFunc(key, value, indentLv + 1));
      if (node.sub.isNotEmpty && node.link.isEmpty) {
        if (!isMin) result.write(indent);
        result.write('}');
      }
      if (node.desc.isNotEmpty && !isMin && !isNodesc) result.write(' <${node.desc.replaceAll('>', r'\>')}>');
      if (isMin)
        result.write(';');
      else
        result.write('\n');
    };
    sub.forEach((final String key, final FVVV value) => printFunc(key, value, 0));
    return (result..removeLastChar()).toString();
  }

  void addFromString(String txt) {
    if (txt.startsWith('\u{FEFF}')) txt = txt.substring(1);
    txt = txt.trim().replaceAll(RegExp(r'\r\n|\r'), '\n');
    if (txt.isEmpty) return;
    if (String.fromCharCode(txt.runes.last) != '}') txt += '\n';

    bool eqOr<T>(final List<T> values) {
      if (values.isEmpty) return false;
      final T first = values[0];
      for (int i = 1; i < values.length; i++) if (first == values[i]) return true;
      return false;
    }

    FVVV getKey(final List<String> paths, final FVVV rootKey) {
      FVVV tmpKey = rootKey;
      for (final String path in paths) tmpKey = tmpKey[path];
      return tmpKey;
    }

    FVVV findKey(final String path, final FVVV idxKey, final FVVV rootKey) {
      final List<String> tmpNames = path.trim().split('.');
      FVVV tmpKey = idxKey;
      for (final String key in tmpNames)
        if (tmpKey.sub.containsKey(key))
          tmpKey = tmpKey[key];
        else
          break;
      if (tmpKey.isEmpty || tmpKey.sub.isEmpty) {
        tmpKey = rootKey;
        for (final String key in tmpNames)
          if (tmpKey.sub.containsKey(key))
            tmpKey = tmpKey[key];
          else
            break;
      }
      return tmpKey;
    }

    int? tryInt(final String s) {
      try {
        return int.parse(s);
      } catch (_) {
        return null;
      }
    }

    double? tryDouble(final String s) {
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
        isAllStr = false,
        isEmptyStr = false,
        inList = false,
        isList = false;
    final StringBuffer tmpDesc = StringBuffer(), value = StringBuffer(), valueName = StringBuffer();
    String idxDesc = '', idxChar = '', lastChar = '';
    int groupNum = 0, idx = 0;
    List<String> values = <String>[], valueNames = <String>[], groupNames = <String>[];
    List<List<String>> lastGroupNames = <List<String>>[];
    for (final int rune in txt.runes) {
      idxChar = String.fromCharCode(rune);
      isRealChar = lastChar != r'\';
      if (((final FVVV rootKey) {
        FVVV idxKey = rootKey;
        if (inDesc) {
          if (idxChar != '>' || !isRealChar) {
            if (idxChar == '>' && !isRealChar) tmpDesc.removeLastChar();
            if (inValue || groupNum > 0) tmpDesc.write(idxChar);
            return false;
          } else if (idxChar == '>' && isRealChar) {
            idxDesc = tmpDesc.toString();
            inDesc = false;
            return false;
          }
        } else {
          if (!inStr && eqOr(<String>[idxChar, ' ', '\t']))
            return false;
          else if (idxChar == '<') {
            inDesc = true;
            return false;
          }
        }
        if (inValue) {
          if (inStr) {
            if (idxChar == '"') {
              if (isRealChar) {
                if (value.isEmpty)
                  isEmptyStr = true;
                else
                  isEmptyStr = false;
                inStr = false;
                return false;
              } else {
                value
                  ..removeLastChar()
                  ..write(idxChar);
                return false;
              }
            } else {
              value.write(idxChar);
              return false;
            }
          } else {
            if (idxChar == '"') {
              inStr = isStr = isAllStr = true;
              return false;
            } else if (idxChar == '[') {
              inList = isList = true;
              return false;
            } else if (inList && eqOr(<String>[idxChar, ',', ']', '\n'])) {
              if (idxChar == ']') {
                inList = false;
                int pos = idxChar.length;
                bool inListDesc = false;
                for (;;) {
                  if (() {
                    switch (txt[idx - pos]) {
                      case '<':
                        if (!inListDesc) return true;
                        if (idx - pos < 1 || txt[idx - pos - 1] != r'\') inListDesc = false;
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
                  }()) break;
                  pos++;
                }
                if (txt[idx - pos] == ',' || txt[idx - pos] == '\n') return false;
              } else if (value.isEmpty && (!isAllStr || !isEmptyStr)) return false;
              final String valueStr = value.toString();
              if ((isAllStr && isStr) ||
                  eqOr(<String>[valueStr, 'true', 'false']) ||
                  tryInt(valueStr) != null ||
                  tryDouble(valueStr) != null) {
                values.add(valueStr);
              } else {
                idxKey = getKey(<String>[valueStr], getKey(groupNames, idxKey));
                if (idxKey.isNotEmpty) {
                  switch (idxKey.value) {
                    case final String v:
                      values.add(v);
                    case final bool v:
                      values.add(v.toString());
                    case final int v:
                      values.add(v.toString());
                    case final double v:
                      values.add(v.toString());
                    case final List<String> v:
                      values.addAll(v);
                    case final List<bool> v:
                      values.addAll(v.map((final bool v) => v.toString()));
                    case final List<int> v:
                      values.addAll(v.map((final int v) => v.toString()));
                    case final List<double> v:
                      values.addAll(v.map((final double v) => v.toString()));
                  }
                }
              }
              if (isEmptyStr)
                isEmptyStr = false;
              else
                value.clear();
              isStr = false;
              return false;
            } else if (idxChar == '{') {
              groupNames.addAll(valueNames);
              lastGroupNames.add(valueNames.toList());
              valueNames.clear();
              groupNum++;
              inValue = false;
              return false;
            } else if (!inList && eqOr(<String>[idxChar, ';', '\n'])) {
              idxKey = getKey(valueNames, getKey(groupNames, rootKey));
              if (isList) {
                if (values.isEmpty)
                  idxKey.value = null;
                else if (isAllStr)
                  idxKey.value = values.toList();
                else {
                  final String tmpStr = values[0];
                  if (eqOr(<String>[tmpStr, 'true', 'false'])) {
                    final List<bool> tmps = <bool>[];
                    for (final String s in values) tmps.add(s == 'true');
                    idxKey.value = tmps;
                  } else if (tryInt(tmpStr) != null) {
                    final List<int> tmps = <int>[];
                    for (final String s in values) tmps.add(tryInt(s)!);
                    idxKey.value = tmps;
                  } else if (tryDouble(tmpStr) != null) {
                    final List<double> tmps = <double>[];
                    for (final String s in values) tmps.add(tryDouble(s)!);
                    idxKey.value = tmps;
                  }
                }
              } else {
                final String valueStr = value.toString();
                if (isAllStr)
                  idxKey.value = valueStr;
                else if (eqOr(<String>[valueStr, 'true', 'false']))
                  idxKey.value = valueStr == 'true';
                else if (tryInt(valueStr) != null)
                  idxKey.value = tryInt(valueStr);
                else if (tryDouble(valueStr) != null)
                  idxKey.value = tryDouble(valueStr);
                else {
                  final FVVV tmpKey = findKey(valueStr, idxKey, rootKey);
                  if (tmpKey.value != null || tmpKey.sub.isNotEmpty) {
                    if (tmpKey.sub.isEmpty)
                      idxKey = tmpKey;
                    else
                      idxKey.sub = tmpKey.sub;
                    idxKey.link = valueStr;
                  }
                }
              }
              idxKey.desc = idxDesc;
              idxDesc = '';
              value.clear();
              values.clear();
              valueNames.clear();
              inValue = isStr = isAllStr = isList = false;
              return false;
            } else {
              value.write(idxChar);
              return false;
            }
          }
        } else {
          if (!oldFVV && idxChar == '{' && valueName.isEmpty) {
            oldFVV = true;
            return false;
          } else if (idxChar == '=') {
            valueNames = valueName.toString().trim().split('.');
            valueName.clear();
            inValue = true;
            return false;
          } else if (endGroup && eqOr(<String>[idxChar, ';', '\n']) && groupNum > 0) {
            endGroup = false;
            if (idxDesc.isNotEmpty) {
              getKey(groupNames, rootKey).desc = idxDesc;
              idxDesc = '';
            }
            for (final String _ in lastGroupNames.last) groupNames = groupNames.sublist(0, groupNames.length - 1);
            lastGroupNames = lastGroupNames.sublist(
              0,
              lastGroupNames.length - 1,
            );
            groupNum--;
            return false;
          } else if (idxChar == '}') if (groupNum == 0)
            return true;
          else {
            endGroup = true;
            return false;
          }
          else {
            valueName.write(idxChar);
            return false;
          }
        }
      })(this)) break;
      idx += idxChar.length;
      lastChar = idxChar;
    }
  }
}
