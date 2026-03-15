using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SetTremoloAmount : MonoBehaviour
{
    public GameObject idiophone;

    public void SetTremoloAmountFromValue(float value)
    {
        idiophone.BroadcastMessage("SetTremoloAmount", value);
    }
}
