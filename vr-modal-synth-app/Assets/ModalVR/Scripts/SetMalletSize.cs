using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class SetMalletSize : MonoBehaviour
{
    public GameObject malletHead1;
    public GameObject malletHead2;
    public AudioMixer mixer;

    void Start()
    {
        SetMalletSizeFromValue(3.5f);
    }

    public void SetMalletSizeFromValue(float value)
    {
        float diameterMeters = value / 100.0f;

        Vector3 scale = new Vector3(
            diameterMeters / malletHead1.transform.parent.localScale.x,
            diameterMeters / malletHead2.transform.parent.localScale.y,
            diameterMeters / malletHead2.transform.parent.localScale.z);

        malletHead1.transform.localScale = scale;
        malletHead2.transform.localScale = scale;

        mixer.SetFloat("MalletDiameter", value);

        //malletHead1.transform.localScale = new Vector3(2.567029f, 0.08556764f, 2.567029f) * (value * 0.25f + 0.1f); //1.5f;//  / 3f;
        //malletHead2.transform.localScale = new Vector3(2.567029f, 0.08556764f, 2.567029f) * (value * 0.25f + 0.1f); //1.5f;//  / 3f;
    }
}
