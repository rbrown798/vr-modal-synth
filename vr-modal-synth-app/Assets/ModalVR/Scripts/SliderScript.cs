using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SliderScript : MonoBehaviour
{
    public delegate void DensitySliderEventTrigger(float value);
    public event DensitySliderEventTrigger densitySliderEventTrigger;

    public void SetDensityFromValue(float value)
    {
  
        densitySliderEventTrigger?.Invoke(value);

        //SendMessage("SetDensity", value);
    }

    public void SetStiffnessFromValue(float value)
    {
        //SendMessage("SetStiffness", value);
    }

    public void SetTimbreFromValue(float value)
    {
        //SendMessage("SetTimbre", value);
    }
}
