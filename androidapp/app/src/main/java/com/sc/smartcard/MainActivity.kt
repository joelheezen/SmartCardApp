package com.sc.smartcard

import android.app.PendingIntent
import android.content.Intent
import android.nfc.NdefMessage
import android.nfc.NdefRecord
import android.nfc.NfcAdapter
import android.nfc.Tag
import android.nfc.tech.Ndef
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    private var nfcAdapter: NfcAdapter? = null
    private var pendingIntent: PendingIntent? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        nfcAdapter = NfcAdapter.getDefaultAdapter(this)

        if (!nfcAdapter?.isEnabled!!) {
            // NFC is not enabled, prompt the user to enable it
            // You can customize this part based on your application's requirements
            // For simplicity, a toast message is used here
            Toast.makeText(this, "Please enable NFC", Toast.LENGTH_SHORT).show()
        }

        // Create a PendingIntent for the NFC foreground dispatch
        val intent = Intent(this, javaClass).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP)
        pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT)
    }

    override fun onResume() {
        super.onResume()
        nfcAdapter?.enableForegroundDispatch(this, pendingIntent, null, null)
    }

    override fun onPause() {
        super.onPause()
        nfcAdapter?.disableForegroundDispatch(this)
    }

    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)

        if (NfcAdapter.ACTION_TECH_DISCOVERED == intent.action) {
            val tag = intent.getParcelableExtra<Tag>(NfcAdapter.EXTRA_TAG)

            // Convert the string to bytes and send it to the Arduino
            val dataToSend = "Hello, Arduino!".toByteArray(Charsets.UTF_8)

            // Use NFC tech to communicate with the Arduino
            val ndefMessage = NdefMessage(NdefRecord.createMime("text/plain", dataToSend))
            val ndef = Ndef.get(tag)

            // Write the NdefMessage to the tag
            ndef?.connect()
            ndef?.writeNdefMessage(ndefMessage)
            ndef?.close()

            // Handle any additional actions as needed
        }
    }
}
