package com.sc.smartcard

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.Gravity
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
import androidx.cardview.widget.CardView
import androidx.navigation.fragment.findNavController
import com.sc.smartcard.databinding.FragmentFirstBinding

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

        val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?: return
        binding.linLay.removeAllViews()

        sharedPref.getStringSet("barcodes", HashSet())?.forEach {
            Log.e("loop", it)
            val card = CardView(requireActivity())
            val lp = LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,LinearLayout.LayoutParams.WRAP_CONTENT)
            lp.setMargins(0,7,0,0)
            card.layoutParams = lp
            val tvlp = LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,LinearLayout.LayoutParams.WRAP_CONTENT)
            tvlp.setMargins(0,7,0,0)
            val name = TextView(requireActivity())
            name.layoutParams = tvlp
            val number = TextView(requireActivity())
            number.gravity = Gravity.RIGHT
            number.layoutParams = tvlp
            binding.linLay.addView(card)
            card.removeAllViews()
            card.addView(name)
            val delimiter = "/"
            val values = it.split(delimiter)
            name.text = values[0]
            card.addView(number)
            number.text = values[1]
        }

        binding.fab.setOnClickListener { view ->
            nextFragment()
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
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
            findNavController().navigate(R.id.action_FirstFragment_to_SecondFragment)
    }
}