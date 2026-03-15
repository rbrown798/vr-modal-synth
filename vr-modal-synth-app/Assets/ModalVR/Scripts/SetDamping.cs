using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;


public class SetDamping : MonoBehaviour
{
	public Material barMaterial;
	public AudioMixer mixer;
    
    public void SetDampingFromValue(float value)
    {
		barMaterial.SetFloat("_Smoothness", 0.5f + (1f - value) * 0.5f);
		mixer.SetFloat("BarDamping", value);
    }
}
