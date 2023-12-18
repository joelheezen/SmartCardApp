package com.example.android_hce

import android.content.Context
import android.content.Context.MODE_PRIVATE
import android.content.SharedPreferences
import android.util.Log
import java.util.*

class DataStoreUtil(context: Context) {
    private val ALLOWED_CHARACTERS = "0123456789ABCDEF"
    private val PREFS_FILENAME = "com.example.android_hce.prefs"
    private var prefs: SharedPreferences? = context.getSharedPreferences(PREFS_FILENAME, MODE_PRIVATE)

    public fun getID(): String {
        var uid = prefs!!.getString("uid", null)
        Log.d("APP", uid.toString())
        Log.d("APP", "TESTTTTTTTTTTTTTTTTTTT")
        if (uid == null) {
            uid = this.generateID();
            this.saveID(uid);
        }

        return uid;
    }

    private fun saveID(uid: String) {
        val editor = prefs!!.edit()
        editor.putString("uid", uid)
        editor.apply()
    }

    private fun generateID(sizeOfRandomHexString: Int = 12): String {
        val random = Random()
        val sb = StringBuilder(sizeOfRandomHexString)
        for (i in 0 until sizeOfRandomHexString)
            sb.append(ALLOWED_CHARACTERS[random.nextInt(ALLOWED_CHARACTERS.length)])
        return sb.toString()
    }

}