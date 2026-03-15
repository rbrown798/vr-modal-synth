using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;


public class SetMalletHardness : MonoBehaviour
{
    public Material malletHeadMaterial;
    public AudioMixer mixer;

    public void SetMalletHardnessFromValue(float value)
    {
        malletHeadMaterial.SetFloat("_Smoothness", value * 0.8f);
        mixer.SetFloat("MalletStiffness", value);
    }
}
