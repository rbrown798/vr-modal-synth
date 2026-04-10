using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;
using UnityEngine.InputSystem;


public class RotateDamping : MonoBehaviour
{
    public InputActionProperty rightActivate;
    public float dampingRotationAmount;
    public GameObject damperObject; // testing
    private MeshRenderer damperMeshRenderer; // testing

    private MeshRenderer meshRenderer;
    private float length;
    private Vector3 originalPosition;
    private Vector3 damperOriginalPosition; //testing

    private bool inCMajorScale;

	public MidiHandler midiHandler;
	public bool midiEnabled;
	public bool pedalEnabled;


	// Start is called before the first frame update
	void Start()
    {
        meshRenderer = GetComponent<MeshRenderer>();
        originalPosition = transform.position;
        length = meshRenderer.bounds.size.z;

        int note = int.Parse(gameObject.name.Substring(4));
        List<int> cMajorScale = new List<int> { 0, 2, 4, 5, 7, 9, 11 };

        inCMajorScale = cMajorScale.Contains(note % 12);

        damperOriginalPosition = damperObject.transform.position; // testing
        damperMeshRenderer = damperObject.GetComponent<MeshRenderer>(); // testing

		//damperWidth = damperMeshRenderer.bounds.size.z;
		//damperThickness = damperMeshRenderer.bounds.size.y;
		//pivotPoint = GetPivotPoint();
		//pivotDistance = GetPivotDistance();

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

				float atanValue;
				if (inCMajorScale)
				{
					//atanValue = -(originalPosition.y + 0.1f - rightActivate.action.ReadValue<float>() / 10f);
					atanValue = -dampingRotationAmount * (1 - value);
				}
				else
				{
					//atanValue = originalPosition.y + 0.1f - rightActivate.action.ReadValue<float>() / 10f;
					atanValue = dampingRotationAmount * (1 - value);


				}
				// Calculate the angle in radians from the arctangent value
				float radians = Mathf.Atan(atanValue);

				// Convert radians to degrees
				float rotationAngle = radians * Mathf.Rad2Deg;


				//Quaternion rotation = Quaternion.Euler(rotationAngle, 0, 0);
				Quaternion rotation = Quaternion.Euler(-90 + rotationAngle, 0, 0);
				transform.rotation = rotation;

				float zPosition;
				float yPosition;
				if (inCMajorScale)
				{
					zPosition = originalPosition.z - (0.28f * length) * (1f - Mathf.Cos(radians));
					yPosition = originalPosition.y - 0.28f * length * Mathf.Sin(radians);
				}
				else
				{
					zPosition = originalPosition.z + (0.28f * length) * (1f - Mathf.Cos(radians));
					yPosition = originalPosition.y + 0.28f * length * Mathf.Sin(radians);
				}

				transform.position = new Vector3(transform.position.x, yPosition, zPosition);
			}
		}
	}

	//private Vector3 GetPivotPoint

	// Update is called once per frame
	void Update()
    {

        //float damperYPosition = damperOriginalPosition.y + 0.005f * (1 - rightActivate.action.ReadValue<float>()); // testing
        Debug.Log("DIFFERENCE" + (damperObject.transform.position.y - originalPosition.y).ToString());

		if (pedalEnabled && !midiEnabled)
		{

			// NOTE: This isn't really going to work, but the idea is sort of there. Look at paper
			//if (rightActivate.action.ReadValue<float>() < 0.5f)
			//{
			float atanValue;
            if (inCMajorScale)
            {
                //atanValue = -(originalPosition.y + 0.1f - rightActivate.action.ReadValue<float>() / 10f);
                atanValue = -dampingRotationAmount * (1 - rightActivate.action.ReadValue<float>());
            }
            else
            {
                //atanValue = originalPosition.y + 0.1f - rightActivate.action.ReadValue<float>() / 10f;
                atanValue = dampingRotationAmount * (1 - rightActivate.action.ReadValue<float>());


            }
            // Calculate the angle in radians from the arctangent value
            float radians = Mathf.Atan(atanValue);

            // Convert radians to degrees
            float rotationAngle = radians * Mathf.Rad2Deg;


            //Quaternion rotation = Quaternion.Euler(rotationAngle, 0, 0);
            Quaternion rotation = Quaternion.Euler(-90 + rotationAngle, 0, 0);
            transform.rotation = rotation;

            float zPosition;
            float yPosition;
            if (inCMajorScale)
            {
                zPosition = originalPosition.z - (0.28f * length) * (1f - Mathf.Cos(radians));
                yPosition = originalPosition.y - 0.28f * length * Mathf.Sin(radians);
            }
            else
            {
                zPosition = originalPosition.z + (0.28f * length) * (1f - Mathf.Cos(radians));
                yPosition = originalPosition.y + 0.28f * length * Mathf.Sin(radians);
            }

            transform.position = new Vector3(transform.position.x, yPosition, zPosition);
        }
    }
}
