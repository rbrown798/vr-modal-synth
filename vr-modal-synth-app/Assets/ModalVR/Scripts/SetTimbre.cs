using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetTimbre : MonoBehaviour
{
    //public GameObject idiophone;
    public AudioMixer mixer;

    public void SetTimbreFromValue(float value)
    {
        //idiophone.BroadcastMessage("SetTimbre", value);
        mixer.SetFloat("BarTimbre", value);
    }
}
