using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RotateFan : MonoBehaviour
{
    private bool tremoloOn;
    private float tremoloRate;

    private Toggle tremoloToggle;
    private Slider tremoloRateSlider;
    
    void Start()
    {
		tremoloRate = 1.0f;

		tremoloToggle = GameObject.Find("Tremolo Toggle").GetComponent<Toggle>();
        if (tremoloToggle != null)
        {
			tremoloToggle.onValueChanged.AddListener(SetTremoloOnOff);
        }

		tremoloOn = tremoloToggle.isOn;

        tremoloRateSlider = GameObject.Find("Tremolo Rate Slider").GetComponent<Slider>();
        if (tremoloRateSlider != null)
        {
			tremoloRateSlider.onValueChanged.AddListener(SetTremoloRate);
        }

		tremoloRate = tremoloRateSlider.value;
    }

    void Update()
    {
        if (tremoloOn)
        {
            // Convert Hertz to degrees per second
            float rotationSpeedDegrees = tremoloRate * 360;

            // Rotate the object along its local Y axis (upward) by the specified speed
            transform.Rotate(Vector3.right * rotationSpeedDegrees * Time.deltaTime);
        }
    }

    private void SetTremoloOnOff(bool value)
    {
		tremoloOn = value;

        // Create a new Quaternion with a 90-degree rotation around the Z-axis
        //Quaternion rotation = Quaternion.Euler(0, 0, 0);
        Quaternion rotation = Quaternion.Euler(90, 0, 0);

        // Apply the rotation to the object's transform
        transform.rotation = rotation;
    }

    private void SetTremoloRate(float value)
    {
		tremoloRate = value;
    }
}