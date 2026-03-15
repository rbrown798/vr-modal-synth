using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;
using UnityEngine.InputSystem;

public class DampingController : MonoBehaviour
{
    public InputActionProperty rightActivate;
    //public PdPatchController pdPatchController;

    private bool activatePressed;

    void Start()
    {
        activatePressed = false;   
    }

    void Update()
    {
        // If the user presses or releases 
        if (activatePressed != (rightActivate.action.ReadValue<float>() > 0.1f))
        {
            activatePressed = !activatePressed;
            //Debug.Log("PRESSED " + activatePressed);
            //pdPatchController.TriggerDamping(activatePressed);
        }

    }
}
