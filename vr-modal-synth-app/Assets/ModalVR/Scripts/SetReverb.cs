using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetReverb : MonoBehaviour
{
    //public AudioReverbFilter reverbFilter;
    public AudioMixer mixer;

    public void SetReverbLevel(float value)
    {
        //reverbFilter.reverbLevel = value;
        mixer.SetFloat("Room", value);
    }
}
