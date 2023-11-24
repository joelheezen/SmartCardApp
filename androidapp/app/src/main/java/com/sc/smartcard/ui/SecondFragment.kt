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
import com.sc.smartcard.MyApp
import com.sc.smartcard.R
import com.sc.smartcard.data.model.BarcodeEntity
import com.sc.smartcard.databinding.FragmentSecondBinding
import kotlinx.coroutines.launch

/**
 * A simple [Fragment] subclass as the second destination in the navigation.
 */
class SecondFragment : Fragment() {

    private var _binding: FragmentSecondBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
            inflater: LayoutInflater, container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {


//        binding = FragmentSecondBinding.inflate(inflater, container, false)
        _binding = FragmentSecondBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?:return
        val rename = sharedPref.getString("tempBarcodeNumber", "empty")
        binding.numberView.text = rename
        binding.saveBtn.setOnClickListener {
            //navigate back to home, and save data to prefs
            Log.d("app", rename.toString())
            Log.d("app", binding.nameEdit.getText().toString())
            val barcodeEntity = BarcodeEntity(name=binding.nameEdit.getText().toString(), barcode=rename.toString() )

            lifecycleScope.launch{
                MyApp.database.barcodeDao().insertBarcode(barcodeEntity)
            }
            findNavController().navigate(R.id.action_SecondFragment_to_FirstFragment)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}