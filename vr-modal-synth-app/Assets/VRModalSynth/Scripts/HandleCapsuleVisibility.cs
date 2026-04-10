using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

public class HandleCapsuleVisibility : MonoBehaviour
{
    public MeshRenderer grabCapsuleRenderer;
    public XRGrabInteractable interactable;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void OnHoverEntered()
    {
        if (!interactable.isSelected)
            grabCapsuleRenderer.enabled = true;
    }

    public void OnHoverExited()
    {
        grabCapsuleRenderer.enabled = false;
    }
}
