using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetTremoloRate : MonoBehaviour
{
    public AudioMixer mixer;

    void Start()
    {
        SetTremoloRateFromValue(1f);
    }

    public void SetTremoloRateFromValue(float value)
    {
        mixer.SetFloat("TremoloRate", value);
    }
}