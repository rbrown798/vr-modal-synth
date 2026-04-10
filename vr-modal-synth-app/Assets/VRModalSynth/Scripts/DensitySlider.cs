using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SetDensity : MonoBehaviour
{
    public void SetDensityFromValue(float value)
    {
        SendMessage("SetDensity", value);
    }
}
