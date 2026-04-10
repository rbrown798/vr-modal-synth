using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RotateObject : MonoBehaviour
{
    private bool motorOn;
    private float frequencyHertz;

    private Toggle motorToggle;
    private Slider motorSpeedSlider;
    
    void Start()
    {
        frequencyHertz = 1.0f;

        motorToggle = GameObject.Find("Motor Toggle").GetComponent<Toggle>();
        if (motorToggle != null)
        {
            motorToggle.onValueChanged.AddListener(SetMotorOnOff);
        }

        motorOn = motorToggle.isOn;

        motorSpeedSlider = GameObject.Find("Motor Speed Slider").GetComponent<Slider>();
        if (motorSpeedSlider != null)
        {
            motorSpeedSlider.onValueChanged.AddListener(SetMotorSpeed);
        }

        frequencyHertz = motorSpeedSlider.value;
    }

    void Update()
    {
        if (motorOn)
        {
            // Convert Hertz to degrees per second
            float rotationSpeedDegrees = frequencyHertz * 360;

            // Rotate the object along its local Y axis (upward) by the specified speed
            transform.Rotate(Vector3.right * rotationSpeedDegrees * Time.deltaTime);
        }
    }

    private void SetMotorOnOff(bool value)
    {
        motorOn = value;

        // Create a new Quaternion with a 90-degree rotation around the Z-axis
        //Quaternion rotation = Quaternion.Euler(0, 0, 0);
        Quaternion rotation = Quaternion.Euler(90, 0, 0);

        // Apply the rotation to the object's transform
        transform.rotation = rotation;
    }

    private void SetMotorSpeed(float value)
    {
        frequencyHertz = value;
    }
}