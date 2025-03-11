class FVV {
  static String bool2Fvv(String name, bool value) {
    return '$name = $value';
  }

  static String double2Fvv(String name, double value) {
    return '$name = $value';
  }

  static String int2Fvv(String name, int value) {
    return '$name = $value';
  }

  static String listString2Fvv(String name, List<String> value) {
    return '$name = [${value.map((v) => '"$v"').join(',')}]';
  }

  static String mapListString2FvvGroup(String name, Map<String, List<String>> input) {
    String result = '$name = {\n';
    input.forEach((k, l) {
      result += '  $k = [\n';
      for (var v in l) {
        result += '    "$v",\n';
      }
      result += '  ]\n';
    });
    result += '}';
    return result;
  }

  static String string2Fvv(String name, String value) {
    return '$name = "$value"';
  }
}
