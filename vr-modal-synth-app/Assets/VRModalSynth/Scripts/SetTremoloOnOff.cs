using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetTremoloOnOff : MonoBehaviour
{
    public AudioMixer mixer;
	void Start()
	{
		SetTremoloOnOffFromValue(false);
	}

	public void SetTremoloOnOffFromValue(bool value)
    {
        if (value)
            mixer.SetFloat("TremoloOn", 1f);
        else
            mixer.SetFloat("TremoloOn", -1f);
    }
}
