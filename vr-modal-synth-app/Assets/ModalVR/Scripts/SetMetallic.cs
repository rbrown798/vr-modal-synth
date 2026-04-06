using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetMetallic : MonoBehaviour
{
	public Material barMaterial;
	public AudioMixer mixer;

	void Start()
	{
		SetMetallicFromValue(0f);
	}

	public void SetMetallicFromValue(float value)
	{
		barMaterial.SetFloat("_Smoothness", 0.5f + value * 0.5f);
		mixer.SetFloat("BarMetallic", value);
	}
}
