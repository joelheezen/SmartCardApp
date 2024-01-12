package com.sc.smartcard

import android.content.Context
import android.util.Log

object Constants {
    const val PREF_FILE_KEY = "your_preference_file_key"
    const val BARCODES_KEY = "barcodes"
    const val KEY_TEMP_DATA = "tempData"

    fun saveBarcodes(context: Context, barcodes: Set<String>){
        val prefs = context.getSharedPreferences(PREF_FILE_KEY, Context.MODE_PRIVATE)
        prefs.edit().putStringSet(BARCODES_KEY, barcodes).apply() ?: ""
    }

    fun getBarcodes(context: Context): Set<String> {
        val prefs = context.getSharedPreferences(PREF_FILE_KEY, Context.MODE_PRIVATE)
        return prefs.getStringSet(BARCODES_KEY, HashSet()) ?: emptySet()
    }

    fun saveTempData(context: Context, data: String) {
        val prefs = context.getSharedPreferences(PREF_FILE_KEY, Context.MODE_PRIVATE)
        prefs.edit().putString(KEY_TEMP_DATA, data).apply()
    }

    fun getTempData(context: Context): String {
        val prefs = context.getSharedPreferences(PREF_FILE_KEY, Context.MODE_PRIVATE)
        return prefs.getString(KEY_TEMP_DATA, "") ?: ""
    }
}