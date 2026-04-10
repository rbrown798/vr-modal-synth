using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

public class SetLockPosition : MonoBehaviour
{
    public ActionBasedContinuousMoveProvider continuousMove;
    
    public void SetLockPositionOnOffFromValue(bool value)
    {
        continuousMove.enabled = !value;
    }
}
