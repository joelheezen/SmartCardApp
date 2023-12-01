package com.sc.smartcard

import android.app.AlertDialog
import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.fragment.findNavController
import com.google.android.material.snackbar.Snackbar
import com.sc.smartcard.databinding.FragmentThirdBinding

/**
 * A simple [Fragment] subclass as the default destination in the navigation.
 */
class ThirdFragment : Fragment() {

    private var _binding: FragmentThirdBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {

        _binding = FragmentThirdBinding.inflate(inflater, container, false)
        return binding.root

    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        val sharedPref = activity?.getPreferences(Context.MODE_PRIVATE) ?:return
        val str = sharedPref.getString("tempData", "empty")
        val delimiter = "/"
        val values = str?.split(delimiter)
        if (values != null){
            binding.nameTv.text = values[0]
            binding.codeTv.text = values[1]
            binding.formatTv.text = values[2]
        }
        binding.delBtn.setOnClickListener{view ->
            //save new data and return back to firstfragment
            val builder = AlertDialog.Builder(this.context)
            builder.setTitle("Are you sure?")
            builder.setMessage("Press OK to delete.")
            builder.setPositiveButton("OK") { dialog, which ->
                val stringSet  = sharedPref.getStringSet("barcodes", HashSet())
                val copy = HashSet<String>()
                stringSet?.forEach{
                    if (it != str){
                        copy.add(it)
                    }
                }
                with (sharedPref.edit()) {
                    putStringSet("barcodes", copy)
                    apply()
                }
                findNavController().navigate(R.id.action_ThirdFragment_to_FirstFragment)
            }
            builder.setNegativeButton("cancel") { dialog, which ->
                Snackbar.make(view, "Cancelled delete request" , Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
            }
            builder.show()
        }
    }
    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}