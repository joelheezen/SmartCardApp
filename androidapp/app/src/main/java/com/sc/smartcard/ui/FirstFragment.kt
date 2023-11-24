package com.sc.smartcard.ui

import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.google.android.gms.tasks.Task
import com.google.android.material.snackbar.Snackbar
import com.google.mlkit.vision.barcode.common.Barcode
import com.google.mlkit.vision.codescanner.GmsBarcodeScanning
import com.sc.smartcard.R
import com.sc.smartcard.databinding.FragmentFirstBinding
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlin.coroutines.resume
import kotlin.coroutines.resumeWithException

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
            GlobalScope.launch{
                startScanner(view)
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    private suspend fun startScanner(view: View) {
        val scanner = GmsBarcodeScanning.getClient(requireActivity())
        try{
            val barcode = scanner.startScan().await()
            val barNum = barcode.displayValue.toString()
            saveNum(barNum)
            Log.d("app", "GONIG TO NEXTFRAMGNET")
            nextFragment()
        } catch(e: Exception){
            Snackbar.make(view, "Failed reason: $e", Snackbar.LENGTH_LONG)
                .setAction("Action", null).show()
            Log.e("app", e.toString())
        }
    }

    @OptIn(ExperimentalCoroutinesApi::class)
    private suspend fun Task<Barcode>.await(): Barcode = suspendCancellableCoroutine { cont ->
        addOnSuccessListener { barcode ->
            cont.resume(barcode)
            saveNum(barcode.displayValue.toString())
            barNum = barcode.displayValue.toString()
        }
        addOnCanceledListener {
            cont.cancel()
        }
        addOnFailureListener {
            cont.resumeWithException(it)
        }
    }

    private fun saveNum(num: String){
        val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?:return
        with (sharedPref.edit()) {
            putString("tempBarcodeNumber", num)
            apply()
        }
        Log.e("app ", barNum)
    }

    private fun nextFragment(){
        Log.e("app", barNum)
        if (barNum != "empty"){
            Log.e("app", "message")
            lifecycleScope.launch(Dispatchers.Main){
                findNavController().navigate(R.id.action_FirstFragment_to_SecondFragment)
            }
        }
    }
}