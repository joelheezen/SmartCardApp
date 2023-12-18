package com.sc.smartcard.nfc

import android.content.Context
import android.content.SharedPreferences
import android.nfc.cardemulation.HostApduService
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import com.sc.smartcard.Constants
import com.sc.smartcard.FirstFragment
import com.sc.smartcard.R
import com.sc.smartcard.databinding.FragmentFirstBinding

class MyHostApduService : HostApduService() {


    override fun processCommandApdu(p0: ByteArray?, p1: Bundle?): ByteArray {
        Log.d("HCE", "returning: " + "TEST".toByteArray())

        val context = applicationContext
        val sharedPref = Constants.getBarcodes(context)
        Log.d("HCE", sharedPref.toString())

        Log.d("HCE", sharedPref.toString())
        sharedPref.forEach{
            Log.d("HCE", it)

        }

        return sharedPref.toString().toByteArray()
    }

    override fun onDeactivated(p0: Int) {
        Log.d("HCE", "Deactivated: " + p0)
    }
}