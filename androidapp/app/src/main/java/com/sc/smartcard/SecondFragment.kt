package com.sc.smartcard

import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import com.google.android.material.snackbar.Snackbar
import com.google.mlkit.vision.codescanner.GmsBarcodeScanning
import com.sc.smartcard.databinding.FragmentSecondBinding

/**
 * A simple [Fragment] subclass as the second destination in the navigation.
 */
class SecondFragment : Fragment() {

    private var _binding: FragmentSecondBinding? = null
    private var format = "empty"

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
            inflater: LayoutInflater, container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {



        _binding = FragmentSecondBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?:return
        val rename = "please scan to get number"
        val oldSet = sharedPref.getStringSet("barcodes", HashSet<String>())
        val newSet = HashSet<String>()
        newSet.addAll(oldSet!!)

        binding.numberView.text = rename

        binding.scanBtn.setOnClickListener { view ->
            startScanner(view)
        }
        binding.saveBtn.setOnClickListener {
            //navigate back to home, and save data to prefs if string is changed
            if (binding.numberView.text == rename || format == "empty"){
                Snackbar.make(view, "please scan code before saving" , Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
            }
            else if(binding.nameEdit.text.toString() == "Name"|| binding.nameEdit.text.toString() == ""){
                Snackbar.make(view, "please change name before saving" , Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
            }
            else{
                val numberText = binding.numberView.text as String
                val nameText = binding.nameEdit.text.toString()
                newSet.add(nameText + "|" + numberText + "|" + format)
                with (sharedPref.edit()) {
                    putStringSet("barcodes", newSet)
                    apply()
                }
                findNavController().navigate(R.id.action_SecondFragment_to_FirstFragment)
            }

        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
    private fun startScanner(view: View) {
        val scanner = GmsBarcodeScanning.getClient(requireActivity())
        scanner.startScan()
            .addOnSuccessListener { barcode ->
                // Task completed successfully
                val barNum = barcode.displayValue.toString()
                format = barcode.format.toString()
                binding.numberView.text = barNum
            }
            .addOnCanceledListener {
                // Task canceled
                Snackbar.make(view, "cancelled by user" , Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
            }
            .addOnFailureListener { e ->
                Log.e("error", e.toString())
                Snackbar.make(view, "Failed reason: "+ e , Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
                // Task failed with an exception
            }
    }
}
