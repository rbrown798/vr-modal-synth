using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetTremoloRate : MonoBehaviour
{
    public AudioMixer mixer;

    public void SetTremoloRateFromValue(float value)
    {
        mixer.SetFloat("TremoloRate", value);
    }
}