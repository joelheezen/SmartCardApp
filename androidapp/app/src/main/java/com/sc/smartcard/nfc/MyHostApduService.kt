package com.sc.smartcard.nfc

import android.nfc.cardemulation.HostApduService
import android.os.Bundle
import android.util.Log
import com.sc.smartcard.Constants


class MyHostApduService : HostApduService() {

    private val APDU_SELECT = byteArrayOf(
        0x00.toByte(),
        0xA4.toByte(),
        0x04.toByte(),
        0x00.toByte(),
        0x07.toByte(),
        0xF0.toByte(),
        0x39.toByte(),
        0x41.toByte(),
        0x48.toByte(),
        0x14.toByte(),
        0x81.toByte(),
        0x00.toByte(),
        0x00.toByte()
    )



    @OptIn(ExperimentalStdlibApi::class)
    override fun processCommandApdu(p0: ByteArray?, p1: Bundle?): ByteArray {
        if (p0 != null) {
            Log.d("HCE", "prcessCommandApdu() | incoming command: " + p0.toHexString(HexFormat.Default))
        }

        val context = applicationContext
        val sharedPref = Constants.getBarcodes(context)
        Log.d("HCE", sharedPref.toString())

        sharedPref.forEach{
            Log.d("HCE", it)
            sendResponseApdu("TEST".toByteArray())
        }
//        sendResponseApdu("TEST".toByteArray())
        for(i in 0 until 5){
            sendResponseApdu(byteArrayOf(i.toByte()))
            Log.d("HCE", "SENDING: " + i.toString())
        }
        return byteArrayOf()
//        return sharedPref.toString().toByteArray()
    }

    override fun onDeactivated(p0: Int) {
        Log.d("HCE", "Deactivated: " + p0)
    }
}