class FVV {
  static String mapListString2FvvGroup(String name, Map<String, List<String>> input) {
    String result = '{\n  $name = {\n';
    input.forEach((k, l) {
      result += '    $k = [\n';
      for (var v in l) {
        result += '      "$v",\n';
      }
      result += '    ];\n';
    });
    result += '  };\n}';
    return result;
  }

  static Map<String, List<String>> fvvGroup2MapListString(String name, String input) {
    RegExp regExp = RegExp(r'\b' + RegExp.escape(name) + r'\s*=\s*\{(.*?)\};', dotAll: true);
    Match? match = regExp.firstMatch(input);
    Map<String, List<String>> result = {};
    if (match != null) {
      String valueContent = match.group(1) ?? '';
      RegExp keyValueRegExp = RegExp(r'(\w+)\s*=\s*\[\s*(.*?)\s*\];', dotAll: true);
      Iterable<Match> keyMatches = keyValueRegExp.allMatches(valueContent);
      for (var keyMatch in keyMatches) {
        String key = keyMatch.group(1) ?? '';
        String valuesString = keyMatch.group(2) ?? '';
        List<String> values = valuesString.split(',').map((e) => e.trim().replaceAll('"', '')).where((e) => e.isNotEmpty).toList();
        result[key] = values;
      }
    }
    return result;
  }

  static String string2Fvv(String name, String value) {
    return '{ $name = "$value"; }';
  }

  static String double2Fvv(String name, double value) {
    return '{ $name = $value; }';
  }

  static String int2Fvv(String name, int value) {
    return '{ $name = $value; }';
  }

  static String bool2Fvv(String name, bool value) {
    return '{ $name = $value; }';
  }
}
