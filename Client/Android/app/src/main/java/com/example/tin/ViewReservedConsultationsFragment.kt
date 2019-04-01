package com.example.tin

import android.os.Bundle
import android.support.v4.app.Fragment
import android.support.v7.widget.LinearLayoutManager
import android.support.v7.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.example.tin.data.Consultation
import com.example.tin.data.ConsultationType
import kotlinx.android.synthetic.main.fragment_reserve_consultation.view.*


// TODO: Rename parameter arguments, choose names that match
// the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
private const val ARG_PARAM1 = "param1"
private const val ARG_PARAM2 = "param2"

/**
 * A simple [Fragment] subclass.
 * Activities that contain this fragment must implement the
 * [ViewReservedConsultationsFragment.OnFragmentInteractionListener] interface
 * to handle interaction events.
 * Use the [ViewReservedConsultationsFragment.newInstance] factory method to
 * create an instance of this fragment.
 *
 */
class ViewReservedConsultationsFragment : Fragment() {
    // TODO: Rename and change types of parameters
    private var param1: String? = null
    private var param2: String? = null

    private lateinit var recyclerAdapter: RecyclerView.Adapter<RecyclerView.ViewHolder>

    private val consultations = listOf(
        Consultation("Dr. inż. Kozdrowski", "12.00", "12.15", "21.03.2019", ConsultationType.LECTURER_SUGGESTED),
        Consultation("Dr. inż. Kozdrowski", "12.15", "12.30", "21.03.2019", ConsultationType.LECTURER_SUGGESTED)
    )

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            param1 = it.getString(ARG_PARAM1)
            param2 = it.getString(ARG_PARAM2)
        }
        recyclerAdapter = MyBookedConsultationsRecyclerAdapter(consultations.sortedWith (compareBy ({it.day}, {it.startTime})))
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        val view = inflater.inflate(R.layout.fragment_view_reserved_consultations, container, false)
        view.recyclerView.apply {
            setHasFixedSize(true)
            adapter = recyclerAdapter
            layoutManager = LinearLayoutManager(context)
        }
        return view
    }

    companion object {
        /**
         * Use this factory method to create a new instance of
         * this fragment using the provided parameters.
         *
         * @param param1 Parameter 1.
         * @param param2 Parameter 2.
         * @return A new instance of fragment ViewReservedConsultationsFragment.
         */
        // TODO: Rename and change types and number of parameters
        @JvmStatic
        fun newInstance(param1: String, param2: String) =
            ViewReservedConsultationsFragment().apply {
                arguments = Bundle().apply {
                    putString(ARG_PARAM1, param1)
                    putString(ARG_PARAM2, param2)
                }
            }
    }
}
