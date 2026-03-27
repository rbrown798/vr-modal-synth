using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;


public class SetDamping : MonoBehaviour
{
	public AudioMixer mixer;
    
    public void SetDampingFromValue(float value)
    {
		mixer.SetFloat("BarDamping", value);
    }
}
