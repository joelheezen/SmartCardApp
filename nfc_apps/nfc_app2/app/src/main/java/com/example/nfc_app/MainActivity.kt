package com.example.nfc_app

import android.app.PendingIntent
import android.content.Intent
import android.nfc.NfcAdapter
import android.nfc.Tag
import android.nfc.tech.MifareClassic
import android.os.Bundle
import android.preference.Preference
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.PackageManagerCompat.LOG_TAG
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.navigateUp
import androidx.navigation.ui.setupActionBarWithNavController
import com.example.nfc_app.databinding.ActivityMainBinding
import com.google.android.material.snackbar.Snackbar
import java.io.IOException
import java.nio.charset.Charset


class MainActivity : AppCompatActivity() {

    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: ActivityMainBinding
    private var pendingIntent: PendingIntent? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)


        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        setSupportActionBar(binding.toolbar)

        val navController = findNavController(R.id.nav_host_fragment_content_main)
        appBarConfiguration = AppBarConfiguration(navController.graph)
        setupActionBarWithNavController(navController, appBarConfiguration)

        val intent = Intent(this, javaClass).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP)
        pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_MUTABLE)
        Log.d("INTENTTTT", intent.toString())
        binding.fab.setOnClickListener { view ->
            Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                .setAction("Action", null).show()
        }
    }

    override fun onResume() {
        super.onResume()
        Log.d("APP", "RESUMINGGG.")
        Log.d(" INTENTT" , getIntent().toString())
        val nfcAdapter = NfcAdapter.getDefaultAdapter(this)
        val intent = intent
        val techList = arrayOf(arrayOf<String>(MifareClassic::class.java.name))
        if(NfcAdapter.ACTION_TECH_DISCOVERED == intent.action){
            Log.d("APP", "SUSSCSCLSDJFLJLSKDFJJSDLCES")
        }
        nfcAdapter?.enableForegroundDispatch(this, pendingIntent, null, techList)
    }

    override fun onPause() {
        super.onPause()
        val nfcAdapter = NfcAdapter.getDefaultAdapter(this)
        nfcAdapter?.disableForegroundDispatch(this)
    }

    private fun writeMifareClassic(tag: Tag?, blockIndex: Int, data: ByteArray) {
        val mifareClassic = MifareClassic.get(tag)
        var tmpData = ""
        for(byte: Byte in data){
           tmpData += byte
            tmpData += " "
        }
        Log.d("DATA", tmpData)
        Log.d("datasize", data.size.toString())

        try {
            mifareClassic.connect()

            // Authenticate the sector before writing
            val sectorIndex = mifareClassic.blockToSector(blockIndex)
            val byteKey = MifareClassic.KEY_DEFAULT
            if(mifareClassic.authenticateSectorWithKeyA(sectorIndex, byteKey)){
                Log.d("APP", "LOGIN SUCCESLFULL")
            }
            if (mifareClassic.authenticateSectorWithKeyA(sectorIndex, byteKey)) {
                // Write data to the specified block
//                mifareClassic.writeBlock(blockIndex, data.take(16))
                mifareClassic.writeBlock(blockIndex, data)

                Log.d("app", "Write successful to block $blockIndex")
            } else {
                Log.e("app", "Authentication failed for sector $sectorIndex")
            }
        } catch (e: IOException) {
            Log.e("app", "Error writing to Mifare Classic card: ${e.message}")
        } finally {
            try {
                mifareClassic.close()
            } catch (e: IOException) {
                Log.e("app", "Error closing Mifare Classic connection: ${e.message}")
            }
        }
    }


    private fun readMifareClassic(tag: Tag?){
        val mifareClassic = MifareClassic.get(tag)
        try{
            mifareClassic.connect()
//            val key = byteArrayOf(0xFF.toByte(),0xFF.toByte(),0xFF.toByte(),0xFF.toByte(),0xFF.toByte(),0xFF.toByte(),0xFF.toByte(),0xFF.toByte(),0xFF.toByte(),0xFF.toByte(),0xFF.toByte(),0xFF.toByte())
            val key = MifareClassic.KEY_DEFAULT

            if(mifareClassic.authenticateSectorWithKeyA(0, key)){
                Log.d("SUCCESS", "DJSDLFJLSDKJFSLDFJSDKJFSLJDFklSDJFL")
                var data = mifareClassic.readBlock(1)
                Log.d("DATA", data.toString())
            }else if(mifareClassic.authenticateSectorWithKeyB(0, key)){
                Log.d("ULTIMEATE", "FAILURE")
            }else{
                Log.d("NOPE", "NOOOO")
            }
        }catch(e: IOException){
            Log.e("app", "IOESCEPTION$e")
        } finally{
            try{
                mifareClassic.close()
            }catch(e: IOException){
                Log.e("app", "IOEXCEPTION2: " + e.toString())
            }
        }
    }
    override fun onNewIntent(intent: Intent?) {
        super.onNewIntent(intent)

        Log.d("ONNEWINTENT", "NEWWINTETTN")

        if (intent != null) {

            if (NfcAdapter.ACTION_TECH_DISCOVERED == intent.action) {
                val tag = intent.getParcelableExtra<Tag>(NfcAdapter.EXTRA_TAG)
                Log.d("app", "NEWINTENTTTTTT")
//                readMifareClassic(tag)
                writeMifareClassic(tag, 8, "TESTTESTSETTSTST".toByteArray(Charsets.UTF_8))
            }
        }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_settings -> true
            else -> super.onOptionsItemSelected(item)
        }
    }

    override fun onSupportNavigateUp(): Boolean {
        val navController = findNavController(R.id.nav_host_fragment_content_main)
        return navController.navigateUp(appBarConfiguration)
                || super.onSupportNavigateUp()
    }
}