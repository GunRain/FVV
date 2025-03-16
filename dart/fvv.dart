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

class FVV {
  static String bool2FVV(String name, bool value) {
    return '$name = $value';
  }

  static String double2FVV(String name, double value) {
    return '$name = $value';
  }

  static String int2FVV(String name, int value) {
    return '$name = $value';
  }

  static String listString2FVV(String name, List<String> value) {
    return '$name = [${value.map((v) => '"$v"').join(',')}]';
  }

  static String mapListString2FVVGroup(String name, Map<String, List<String>> input) {
    String result = '$name = {\n';
    input.forEach((k, l) {
      result += '  $k = [\n';
      for (var v in l) {
        result += '    "$v"\n';
      }
      result += '  ]\n';
    });
    result += '}';
    return result;
  }

  static String string2FVV(String name, String value) {
    return '$name = "$value"';
  }
}
