using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetOutputVolume : MonoBehaviour
{
	// Start is called before the first frame update
	public AudioMixer mixer;

	public void SetOutputVolumeFromValue(float value)
	{
		mixer.SetFloat("OutputVolume", value);
	}
}
