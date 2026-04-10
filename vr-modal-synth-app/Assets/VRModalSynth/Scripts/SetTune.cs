using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SetTune : MonoBehaviour
{
    public GameObject idiophone;

    public void SetTuneFromValue(float value)
    {
        idiophone.BroadcastMessage("SetTune", value);
    }
}
