using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SetShapeFromTimbre : MonoBehaviour
{
    SkinnedMeshRenderer skinnedMeshRenderer;
    Slider timbreSlider;

    // Start is called before the first frame update
    void Start()
    {
        skinnedMeshRenderer = GetComponent<SkinnedMeshRenderer>();

        timbreSlider = GameObject.Find("Timbre Slider").GetComponent<Slider>();
        if (timbreSlider != null)
            timbreSlider.onValueChanged.AddListener(SetTimbre);
    }

    private void SetTimbre(float value)
    {
        skinnedMeshRenderer.SetBlendShapeWeight(0, value * 100f);
    }
}
