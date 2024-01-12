package com.sc.smartcard.nfc

import android.nfc.cardemulation.HostApduService
import android.os.Bundle
import android.util.Log
import com.sc.smartcard.Constants

class HostApduService : HostApduService() {

    private var currentIndex = 0
    private var dataToSend: ByteArray? = null

    override fun processCommandApdu(commandApdu: ByteArray?, extras: Bundle?): ByteArray {
        val context = applicationContext
        var sharedPref = Constants.getBarcodes(context)
        if (dataToSend == null) {
            dataToSend = (sharedPref.toString()+"DONE").toByteArray()
        }

        val response: ByteArray
        sharedPref.forEach {
            Log.d("HCE", it.toString())
        }
        if (currentIndex < dataToSend!!.size) {
            val endIndex = Math.min(currentIndex + MAX_PAYLOAD_SIZE, dataToSend!!.size)
            val chunk = dataToSend!!.copyOfRange(currentIndex, endIndex)
            currentIndex = endIndex

            response = chunk
        } else {
            response = "".toByteArray()
            resetData()
        }
        Log.d("HCE", "Sending chunk: ${response.toString()}")

        return response
    }

    override fun onDeactivated(reason: Int) {
        Log.d("HCE", "Deactivated: $reason")
        resetData()
    }

    private fun resetData() {
        currentIndex = 0
        dataToSend = null
    }

    companion object {
        private const val MAX_PAYLOAD_SIZE = 64
    }
}
