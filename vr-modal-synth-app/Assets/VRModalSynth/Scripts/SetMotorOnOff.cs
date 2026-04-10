using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetMotorOnOff : MonoBehaviour
{
    public AudioMixer mixer;

    public void SetMotorOnOffFromValue(bool value)
    {
        if (value)
            mixer.SetFloat("LfoOn", 1f);
        else
            mixer.SetFloat("LfoOn", -1f);
    }
}
