using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;
using UnityEngine.InputSystem;


public class SetDamperPosition : MonoBehaviour
{
    public InputActionProperty rightActivate;
    //public float dampingRotationAmount;

    private Vector3 originalPosition;

    public MidiHandler midiHandler;

    public bool midiEnabled;
    public bool pedalEnabled;

    // Start is called before the first frame update
    void Start()
    {
        midiEnabled = true;
        pedalEnabled = true;

        originalPosition = transform.position;

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
                float yPosition = originalPosition.y + 0.01f * (1 - value);

                transform.position = new Vector3(originalPosition.x, yPosition, originalPosition.z);
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        /*
        float atanValue = dampingRotationAmount * (1 - rightActivate.action.ReadValue<float>() );
        // Calculate the angle in radians from the arctangent value
        float radians = Mathf.Atan(atanValue);
        */

        //float yPosition = originalPosition.y + 0.005f * (1 - rightActivate.action.ReadValue<float>());


        if (pedalEnabled && !midiEnabled)
        {
            float yPosition = originalPosition.y + 0.01f * (1 - rightActivate.action.ReadValue<float>());
            transform.position = new Vector3(originalPosition.x, yPosition, originalPosition.z);
        }
    }
}
