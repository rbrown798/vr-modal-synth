using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetResonatorOnOff : MonoBehaviour
{
    //public GameObject idiophone;
    public GameObject resonators;
    public GameObject fans;
    public AudioMixer mixer;

    public void SetResonatorOnOffFromValue(bool value)
    {
        //idiophone.BroadcastMessage("SetResonatorOnOff", value);
        resonators.SetActive(value);
        fans.SetActive(value);

		if (value)
			mixer.SetFloat("TubeOn", 1f);
		else
			mixer.SetFloat("TubeOn", -1f);
	}
}
