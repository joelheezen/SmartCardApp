package com.sc.smartcard

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
import androidx.cardview.widget.CardView
import androidx.fragment.app.Fragment
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

            val inflater = LayoutInflater.from(context)
            val layout = inflater.inflate(R.layout.card, null, false)
            val cv = layout.findViewById<CardView>(R.id.CV)
            val vg = cv.parent as ViewGroup
            vg.removeView(cv)
            binding.linLay.addView(cv)
            val delimiter = "|"
            val values = it.split(delimiter)
            cv.findViewById<TextView>(R.id.textView).text = values[0]
            cv.findViewById<TextView>(R.id.textView2).text = values[1]
            cv.findViewById<TextView>(R.id.textView3).text = values[2]
            cv.setOnClickListener {view ->
                editFragment(it)
                thirdFragment()
            }
        }

        binding.fab.setOnClickListener { view ->
            nextFragment()
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    private fun nextFragment(){
            findNavController().navigate(R.id.action_FirstFragment_to_SecondFragment)
    }

    private fun editFragment(data:String){
        val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?:return
        with (sharedPref.edit()){
            putString("tempData", data)
            apply()
        }
        Log.e("test", data)

    }
    private fun thirdFragment(){
        findNavController().navigate(R.id.action_FirstFragment_to_ThirdFragment)
    }
}