using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;
using UnityEngine.InputSystem;

public class ActivateGrabRay : MonoBehaviour
{
	public GameObject leftGrabRay;
	public GameObject rightGrabRay;

    public XRRayInteractor leftRayInteractor;
    public XRRayInteractor rightRayInteractor;

    public XRInteractorLineVisual leftRayVisual;
    public XRInteractorLineVisual rightRayVisual;

    public XRDirectInteractor leftDirectInteractor;
    public XRDirectInteractor rightDirectInteractor;

    // public InputActionProperty leftGrab;
    // public InputActionProperty rightGrab;


    // Update is called once per frame
    void Update()
    {
        // leftGrabRay.SetActive(leftDirectGrab.interactablesSelected.Count == 0);
        // rightGrabRay.SetActive(rightDirectGrab.interactablesSelected.Count == 0);

        // leftGrabRay.SetActive(leftCancel.action.ReadValue<float>() == 0);
        // rightGrabRay.SetActive(rightCancel.action.ReadValue<float>() == 0);

        // leftGrabRay.SetActive(leftGrab.action.ReadValue<float>() < 0.1f);
        // rightGrabRay.SetActive(rightGrab.action.ReadValue<float>() < 0.1f);

        leftRayVisual.enabled = !leftRayInteractor.hasSelection && !leftDirectInteractor.hasSelection;
        leftRayInteractor.enableUIInteraction = !leftRayInteractor.hasSelection && !leftDirectInteractor.hasSelection;

        rightRayVisual.enabled = !rightRayInteractor.hasSelection && !rightDirectInteractor.hasSelection;
        rightRayInteractor.enableUIInteraction = !rightRayInteractor.hasSelection && !rightDirectInteractor.hasSelection;

    }
}
