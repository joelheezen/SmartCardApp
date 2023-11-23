package com.sc.smartcard

import android.content.Context
import android.media.session.MediaSession.Token
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.findNavController
import androidx.navigation.fragment.findNavController
import com.google.android.material.snackbar.Snackbar
import com.google.mlkit.vision.barcode.common.Barcode
import com.google.mlkit.vision.codescanner.GmsBarcodeScanning
import com.sc.smartcard.databinding.FragmentFirstBinding
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

/**
 * A simple [Fragment] subclass as the default destination in the navigation.
 */
class FirstFragment : Fragment() {

    private var _binding: FragmentFirstBinding? = null
    private var barNum = "empty"




    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
            inflater: LayoutInflater, container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {

        _binding = FragmentFirstBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?:return




        binding.fab.setOnClickListener { view ->
            startScanner(view)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    private fun startScanner(view: View) {
        val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?:return
        val scanRan = sharedPref.getString("scanDone", "false")
        Log.e("scanran", scanRan.toString())
        if(scanRan == "true"){
            with (sharedPref.edit()) {
                putString("scanDone", "false")
                apply()
            }
            nextFragment()
        }
        else{
            with (sharedPref.edit()) {
                putString("tempBarcodeNumber", "empty")
                putString("scanDone", "true")
                apply()
            }
            val scanner = GmsBarcodeScanning.getClient(requireActivity())
            scanner.startScan()
                .addOnSuccessListener { barcode ->
                    // Task completed successfully
                    barNum = barcode.displayValue.toString()
                    saveNum(barNum)
                }
                .addOnCanceledListener {
                    // Task canceled
                    Snackbar.make(view, "cancelled by user" , Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show()
                }
                .addOnFailureListener { e ->
                    Snackbar.make(view, "Failed reason: "+ e , Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show()
                    // Task failed with an exception
                }
        }
    }
    private fun saveNum(num: String){
        val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?:return
        with (sharedPref.edit()) {
            putString("tempBarcodeNumber", num)
            apply()
        }
        Log.e("barnum= ", barNum)
    }

    private fun nextFragment(){
        Log.e("test", barNum)
        if (barNum != "empty"){
            Log.e("test", "message")
            findNavController().navigate(R.id.action_FirstFragment_to_SecondFragment)
        }
    }
}