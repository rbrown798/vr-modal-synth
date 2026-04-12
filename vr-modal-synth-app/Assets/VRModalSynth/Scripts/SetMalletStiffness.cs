using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;


public class SetMalletStiffness : MonoBehaviour
{
    public Material malletHeadMaterial;
    public AudioMixer mixer;

    void Start()
    {
        SetMalletStiffnessFromValue(0.5f);
    }

    public void SetMalletStiffnessFromValue(float value)
    {
        malletHeadMaterial.SetFloat("_Smoothness", value * 0.8f);
        mixer.SetFloat("MalletStiffness", value);
    }
}
