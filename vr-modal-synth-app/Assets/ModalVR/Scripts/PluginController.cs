using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;
using System.Runtime.InteropServices;

using UnityEngine.UI;


public class PluginController : MonoBehaviour
{
    [DllImport("ModalVR_AudioPlugin")]
    public static extern void ModalSynth_NoteOn(int note, float velocity, float position);

    [DllImport("ModalVR_AudioPlugin")]
    public static extern void ModalSynth_SetLeftEarPosition(Vector3 leftEarPosition);

    [DllImport("ModalVR_AudioPlugin")]
    public static extern void ModalSynth_SetLeftEarDirection(Vector3 leftEarDirection);

    [DllImport("ModalVR_AudioPlugin")]
    public static extern void ModalSynth_SetRightEarPosition(Vector3 rightEarPosition);

    [DllImport("ModalVR_AudioPlugin")]
    public static extern void ModalSynth_SetRightEarDirection(Vector3 rightEarDirection);

    public AudioMixer mixer;
    public GameObject leftEar;
    public GameObject leftEarDirection;
    public GameObject rightEar;
    public GameObject rightEarDirection;

    public MidiHandler midiHandler;
	public bool midiEnabled;
	public bool pedalEnabled;

	// Start is called before the first frame update
	void Start()
    {
		//mixer.SetFloat("BarTimbre", 0.0f);
		midiEnabled = true;
		pedalEnabled = true;

		if (midiHandler != null)
		{
			midiHandler.onMidiInMessage.AddListener(OnMidiMessageReceived);
		}
	}
	private void OnMidiMessageReceived(MidiMessage msg)
	{
		if (pedalEnabled && midiEnabled)
		{
			if ((msg.status == 0xB0) && (msg.data1 == 1))
			{
				float value = (float)msg.data2 / 127f;
                mixer.SetFloat("PedalValue", value);
                Debug.Log("VALUE: " + value);
			}
		}
	}

	// Update is called once per frame
	void Update() 
    {
        ModalSynth_SetLeftEarPosition(leftEar.transform.position);
		//ModalSynth_SetLeftEarDirection(leftEarDirection.transform.localPosition);
		ModalSynth_SetLeftEarDirection(leftEar.transform.forward);
		ModalSynth_SetRightEarPosition(rightEar.transform.position);
        //ModalSynth_SetRightEarDirection(rightEarDirection.transform.localPosition);
        ModalSynth_SetRightEarDirection(rightEar.transform.forward);

		if (Input.GetKeyDown(KeyCode.Space))
        {
            int note = 60;
            float position = Random.Range(0.2f, 0.8f);
            float velocity = Random.Range(0.2f, 5.0f);
            ModalSynth_NoteOn(note, velocity, position);
        }
    }

    public void PlayNote(int note, float velocity, float position)
    {
        ModalSynth_NoteOn(note, velocity, position);
    }

    //public void SetTune(float value)
    //{
    //    //float frequency = 440 * Mathf.Pow(2, ((note + value - 69) / 12.0f));

    //    pdPatch.SendFloat("tune", value);
    //}

    public void SetMalletSize(float value)
    {
        mixer.SetFloat("MalletDiameter", value);
    }

    public void SetMalletHardness(float value)
    {
        mixer.SetFloat("MalletStiffness", value);
    }

    public void SetTimbre(float value)
    {
        mixer.SetFloat("BarTimbre", value);
    }

    public void SetDamping(float value)
    {
        mixer.SetFloat("BarDamping", value);
    }

    public void SetTremoloRate(float value)
    {
        mixer.SetFloat("TremoloRate", value);
    }
}
