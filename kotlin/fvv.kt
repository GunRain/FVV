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

@file:Suppress("unused")

package in_.sakit.fvv

import kotlin.reflect.KClass

class FVVV(
  var value: Any? = null,
  var sub: MutableMap<String, FVVV> = mutableMapOf(),
  var desc: String = "",
  var link: String = "",
) {
  operator fun get(key: String): FVVV = sub.getOrPut(key) { FVVV() }
  operator fun set(key: String, v: Any?) {
    sub.getOrPut(key) { FVVV() }.value = v
  }

  override fun equals(other: Any?): Boolean = when {
    this === other -> true
    other !is FVVV -> false
    else           -> this.value == other.value
  }

  override fun hashCode(): Int = value?.hashCode() ?: 0
  override fun toString(): String = value?.toString() ?: "null"

  inline fun <reified T> asType(default: T? = null): T? {
    var v: Any? = this.value
    while (v is FVVV) v = v.value
    return v as? T ?: default
  }

  fun asBool(default: Boolean = false): Boolean = asType(default) ?: default
  fun asInt(default: Int = 0): Int = asType(default) ?: default
  fun asDouble(default: Double = 0.0): Double = asType(default) ?: default
  fun asString(default: String = ""): String = asType(default) ?: default
  fun asBools(default: List<Boolean> = emptyList()): List<Boolean> =
    (asType<List<*>>()?.mapNotNull { it as? Boolean } ?: default)

  fun asInts(default: List<Int> = emptyList()): List<Int> =
    (asType<List<*>>()?.mapNotNull { it as? Int } ?: default)

  fun asDoubles(default: List<Double> = emptyList()): List<Double> =
    (asType<List<*>>()?.mapNotNull { it as? Double } ?: default)

  fun asStrings(default: List<String> = emptyList()): List<String> =
    (asType<List<*>>()?.mapNotNull { it as? String } ?: default)

  val bool get(): Boolean = asType() ?: false
  val int get(): Int = asType() ?: 0
  val double get(): Double = asType() ?: 0.0
  val string get(): String = asType() ?: ""
  val bools get(): List<Boolean> = (asType<List<*>>()?.mapNotNull { it as? Boolean } ?: emptyList())
  val ints get(): List<Int> = (asType<List<*>>()?.mapNotNull { it as? Int } ?: emptyList())
  val doubles get(): List<Double> = (asType<List<*>>()?.mapNotNull { it as? Double } ?: emptyList())
  val strings get(): List<String> = (asType<List<*>>()?.mapNotNull { it as? String } ?: emptyList())

  val isEmpty: Boolean
    get() = when (value) {
      null       -> true
      is String  -> (value as String).isEmpty()
      is List<*> -> (value as List<*>).isEmpty()
      is FVVV    -> (value as FVVV).isEmpty
      else       -> false
    }
  val isNotEmpty: Boolean get() = !isEmpty

  inline fun <reified T> isType(): Boolean {
    var v: Any? = this.value
    while (v is FVVV) v = v.value
    return v is T
  }

  fun getType(): KClass<*>? = when (value) {
    is FVVV -> (value as FVVV).getType()
    else    -> value?.let { it::class }
  }

  fun print(type: String = "common"): String {
    var isMin = false
    var isBiglist = false
    var isNodesc = false
    when (type) {
      "min"     -> isMin = true
      "biglist" -> isBiglist = true
      "nodesc"  -> isNodesc = true
    }
    val result = StringBuilder()
    fun printFunc(path: String, node: FVVV, indentLv: Int) {
      if (path.isEmpty() || (node.isEmpty && node.sub.isEmpty())) return
      val indent = " ".repeat(indentLv * 2)
      if (node.sub.isNotEmpty() && node.link.isEmpty()) {
        if (isMin) result.append("$path={")
        else result.append("$indent$path = {\n")
      }
      if (node.link.isNotEmpty() || node.value != null) {
        if (isMin) result.append("$path=")
        else result.append("$indent$path = ")
        if (node.link.isNotEmpty()) result.append(node.link)
        else {
          val vecIndent = " ".repeat((indentLv + 1) * 2)
          when (val v = node.value) {
            is String                     -> result.append('"').append(v.replace("\"", "\\\""))
              .append('"')

            is Boolean, is Int, is Double -> result.append(v.toString())
            is List<*>                    -> {
              result.append("[")
              if (isBiglist) result.append("\n")
              v.forEach { item ->
                if (isBiglist) result.append(vecIndent)
                result.append(when (item) {
                  is String -> "\"${item.replace("\"", "\\\"")}\""
                  else      -> item.toString()
                })
                if (isBiglist) result.append("\n")
                else {
                  result.append(",")
                  if (!isMin) result.append(" ")
                }
              }
              if (isBiglist) result.append(indent)
              else if (v.isNotEmpty()) {
                result.setLength(result.length - 1)
                if (!isMin) result.setLength(result.length - 1)
              }
              result.append("]")
            }
          }
        }
      } else node.sub.forEach { (k, v) -> printFunc(k, v, indentLv + 1) }
      if (node.sub.isNotEmpty() && node.link.isEmpty()) {
        if (!isMin) result.append(indent)
        result.append("}")
      }
      if (node.desc.isNotEmpty() && !isMin && !isNodesc) result.append(" <")
        .append(node.desc.replace(">", "\\>")).append(">")
      if (isMin) result.append(";") else result.append("\n")
    }
    sub.forEach { (k, v) -> printFunc(k, v, 0) }
    if (result.isNotEmpty()) result.setLength(result.length - 1)
    return result.toString()
  }

  fun addFromString(targetTxt: String) {
    var txt = targetTxt.trimStart { it == '﻿' }.trim().replace("\r\n", "\n").replace("\r", "\n")
    if (txt.isEmpty()) return
    if (!txt.endsWith("}")) txt += "\n"

    fun getKey(paths: List<String>, rootKey: FVVV): FVVV {
      var tmpKey = rootKey
      for (path in paths) tmpKey = tmpKey[path]
      return tmpKey
    }

    fun findKey(path: String, idxKey: FVVV, rootKey: FVVV): FVVV {
      val tmpNames = path.trim().split('.')
      var tmpKey = idxKey
      for (key in tmpNames) if (tmpKey.sub.containsKey(key)) tmpKey = tmpKey[key]
      else break
      if (tmpKey.isEmpty || tmpKey.sub.isEmpty()) {
        tmpKey = rootKey
        for (key in tmpNames) if (tmpKey.sub.containsKey(key)) tmpKey = tmpKey[key]
        else break
      }
      return tmpKey
    }

    var endGroup = false
    var oldFVV = false
    var isRealChar: Boolean
    var inValue = false
    var inDesc = false
    var inStr = false
    var isStr = false
    var isAllStr = false
    var isEmptyStr = false
    var inList = false
    var isList = false

    val tmpDesc = StringBuilder()
    val value = StringBuilder()
    val valueName = StringBuilder()
    var idxDesc = ""
    var idxChar: Char
    var lastChar = '\u0000'
    var groupNum = 0
    var idx = 0
    val values = mutableListOf<String>()
    var valueNames = mutableListOf<String>()
    val groupNames = mutableListOf<String>()
    val lastGroupNames = mutableListOf<List<String>>()

    val runes = txt.toCharArray()
    while (idx < runes.size) {
      idxChar = runes[idx]
      isRealChar = lastChar != '\\'
      if (this.let { rootKey ->
          var idxKey: FVVV
          if (inDesc) {
            if (idxChar != '>' || !isRealChar) {
              if (idxChar == '>' && !isRealChar) tmpDesc.setLength(tmpDesc.length - 1)
              if (inValue || groupNum > 0) tmpDesc.append(idxChar)
              return@let false
            } else {
              idxDesc = tmpDesc.toString()
              tmpDesc.clear()
              inDesc = false
              return@let false
            }
          } else {
            if (!inStr && idxChar in listOf(' ', '\t')) return@let false
            if (idxChar == '<') {
              inDesc = true
              return@let false
            }
          }
          if (inValue) {
            if (inStr) {
              if (idxChar == '"') {
                if (isRealChar) {
                  isEmptyStr = value.isEmpty()
                  inStr = false
                  return@let false
                } else {
                  value.apply {
                    setLength(value.length - 1)
                    append(idxChar)
                  }
                  return@let false
                }
              } else {
                value.append(idxChar)
                return@let false
              }
            } else {
              when {
                idxChar == '"'                              -> {
                  inStr = true
                  isStr = true
                  isAllStr = true
                  return@let false
                }

                idxChar == '['                              -> {
                  inList = true
                  isList = true
                  return@let false
                }

                inList && idxChar in listOf(']', ',', '\n') -> {
                  if (idxChar == ']') {
                    inList = false
                    var pos = 1
                    var inListDesc = false
                    while (true) {
                      if (when (txt.getOrNull(idx - pos) ?: break) {
                          '<'       -> if (!inListDesc) true else (txt.getOrNull(
                            idx - pos - 1) != '\\').also {
                            inListDesc = false
                          }

                          '>'       -> false.also { inListDesc = true }
                          ' ', '\t' -> false
                          ',', '\n' -> !inListDesc
                          else      -> !inListDesc
                        }) break
                      ++pos
                    }
                    if (txt.getOrNull(idx - pos) in listOf(',', '\n')) return@let false
                  } else if (value.isEmpty() && (!isAllStr || !isEmptyStr)) return@let false
                  val valueStr = value.toString()
                  if ((isAllStr && isStr) || valueStr in listOf("true",
                      "false") || valueStr.toIntOrNull() != null || valueStr.toDoubleOrNull() != null) {
                    values.add(valueStr)
                  } else {
                    idxKey = getKey(listOf(valueStr), getKey(groupNames, rootKey))
                    when (val v = idxKey.value) {
                      is String                     -> values.add(v)
                      is Boolean, is Int, is Double -> values.add(v.toString())
                      is List<*>                    -> values.addAll(v.map { it.toString() })
                    }
                  }
                  if (isEmptyStr) isEmptyStr = false
                  else value.clear()
                  isStr = false
                  return@let false
                }

                idxChar == '{'                              -> {
                  groupNames.addAll(valueNames)
                  lastGroupNames.add(valueNames.toList())
                  valueNames.clear()
                  ++groupNum
                  inValue = false
                  return@let false
                }

                idxChar in listOf(';', '\n')                -> {
                  idxKey = getKey(valueNames, getKey(groupNames, rootKey))
                  if (isList) {
                    idxKey.value = when {
                      values.isEmpty()                     -> null
                      isAllStr                             -> values.toList()
                      values[0] in listOf("true", "false") -> values.map { it == "true" }
                      values[0].toIntOrNull() != null      -> values.mapNotNull { it.toIntOrNull() }
                      values[0].toDoubleOrNull() != null   -> values.mapNotNull { it.toDoubleOrNull() }
                      else                                 -> null
                    }
                  } else {
                    val valueStr = value.toString()
                    idxKey.value = when {
                      isAllStr                            -> valueStr
                      valueStr in listOf("true", "false") -> valueStr == "true"
                      valueStr.toIntOrNull() != null      -> valueStr.toInt()
                      valueStr.toDoubleOrNull() != null   -> valueStr.toDouble()
                      else                                -> {
                        val tmpKey = findKey(valueStr, idxKey, rootKey)
                        if (tmpKey.value != null || tmpKey.sub.isNotEmpty()) {
                          idxKey.link = valueStr
                          if (tmpKey.sub.isEmpty()) tmpKey.value else {
                            idxKey.sub = tmpKey.sub
                            null
                          }
                        } else null
                      }
                    }
                  }
                  idxKey.desc = idxDesc
                  idxDesc = ""
                  value.clear()
                  values.clear()
                  valueNames.clear()
                  inValue = false
                  isStr = false
                  isAllStr = false
                  isList = false
                  return@let false
                }

                else                                        -> {
                  value.append(idxChar)
                  return@let false
                }
              }
            }
          } else {
            if (!oldFVV && idxChar == '{' && valueName.isEmpty()) {
              oldFVV = true
              return@let false
            } else if (idxChar == '=') {
              valueNames = valueName.toString().trim().split(".").toMutableList()
              valueName.clear()
              inValue = true
              return@let false
            } else if (endGroup && idxChar in listOf(';', '\n') && groupNum > 0) {
              endGroup = false
              if (idxDesc.isNotEmpty()) {
                getKey(groupNames, rootKey).desc = idxDesc
                idxDesc = ""
              }
              repeat(lastGroupNames.last().size) { groupNames.removeAt(groupNames.size - 1) }
              lastGroupNames.removeAt(lastGroupNames.size - 1)
              --groupNum
              return@let false
            } else if (idxChar == '}') if (groupNum == 0) return@let true
            else {
              endGroup = true
              return@let false
            }
            else {
              valueName.append(idxChar)
              return@let false
            }
          }
        }) break
      lastChar = idxChar
      ++idx
    }
  }
}