package com.sc.smartcard

import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.nfc.NfcAdapter
import android.nfc.Tag
import android.nfc.tech.MifareClassic
import android.nfc.tech.NfcA
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import java.nio.charset.Charset

class MainActivity2 : AppCompatActivity() {

    private var nfcAdapter: NfcAdapter? = null
    private var pendingIntent: PendingIntent? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        nfcAdapter = NfcAdapter.getDefaultAdapter(this)

        // Create a PendingIntent for the NFC foreground dispatch
        val intent = Intent(this, javaClass).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP)
        pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_MUTABLE)
        Log.d("HCE", "ONCREATE")
    }

    override fun onResume() {
        super.onResume()
        val intent = Intent(this, javaClass).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP)
        pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_MUTABLE)
        nfcAdapter?.enableForegroundDispatch(this, pendingIntent, null, null)
    }

    override fun onPause() {
        super.onPause()
        Log.d("HCE", "ONPAUSE")
        nfcAdapter?.disableForegroundDispatch(this)
    }

    private fun writeMifareClassic(tag: Tag){
        val mifareClassic = MifareClassic.get(tag)
        Log.d("HCE", "WRITING MIFARE")
        mifareClassic.connect()

        val sector = 1
        val keyA = byteArrayOf(0xFF.toByte(), 0xFF.toByte(), 0xFF.toByte(), 0xFF.toByte(), 0xFF.toByte(), 0xFF.toByte())
        val block = 2
        mifareClassic.authenticateSectorWithKeyA(sector, keyA)
        Log.d("HCE", "AUTHENTICATED")
        val data = "HELLO, NFC!".toByteArray(Charsets.UTF_8)
        val dataToWrite = data + ByteArray(16 - data.size)

        Log.d("HCE", dataToWrite.toString())
        try {
            mifareClassic.writeBlock(sector * 4 + block, dataToWrite)
        }catch(e: Exception){
            Log.d("HCE", e.stackTraceToString())
            Log.d("HCE", e.toString())
        }
        mifareClassic.close()
    }
    private fun readMifareClassic(tag: Tag) {
        // Mifare Classic specific code goes here
        // You can use the MifareClassic class to read data from the card

        val mifareClassic = MifareClassic.get(tag)
        Log.d("HCE", "READING MIFARE")

        Log.d("HCE", tag.toString())
        val keyA = byteArrayOf(0xFF.toByte(), 0xFF.toByte(), 0xFF.toByte(), 0xFF.toByte(), 0xFF.toByte(), 0xFF.toByte())

        val sector = 1 // Change this to the sector you want to read
        val block = 2 // Change this to the block you want to read

        try {
            mifareClassic.connect()
        }catch (e: Exception ){
            Log.d("HCE", e.toString())
        }
        mifareClassic.authenticateSectorWithKeyA(sector, keyA)
        var data : ByteArray= ByteArray(0)
        try {
            Log.d("HCE", nfcAdapter?.isEnabled.toString())
            if(nfcAdapter?.isEnabled == true ){
                data = mifareClassic.readBlock(sector * 4 + block)

            }
        }catch (e: Exception){
            Log.d("HCE", e.stackTraceToString())
            Log.d("HCE", e.toString())
        }
        Log.d("HCE", String(data, Charsets.UTF_8))

        // Interpret the data as needed
        val dataAsString = String(data, Charset.forName("UTF-8"))
        println("Read data from Mifare Classic: $dataAsString")
        println(data.joinToString(" ") { byte -> "%02X".format(byte) })

        mifareClassic.close()
        Log.d("HCE", dataAsString)
    }

    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)

        if(nfcAdapter == null || !nfcAdapter!!.isEnabled){
            Log.d("HCE", "NFCADAPTER BROEKN")
        }
        // Extract the data you want to send to Arduino
        Log.d("HCE", "FOUND A CARDD!!!")
        Log.d("HCE", intent.action.toString())
        Log.d("HCE", intent.toString())
        if (NfcAdapter.ACTION_TECH_DISCOVERED == intent.action || NfcAdapter.ACTION_TAG_DISCOVERED == intent.action) {
            val tag: Tag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG) ?: return

            // Extract the UID or other data from the tag
            val id = tag.id
            Log.d("HCE", tag.toString())
            writeMifareClassic(tag)
            readMifareClassic(tag)
            Log.d("NFC", "UID: ${id.toString()}")
        }
    }
}