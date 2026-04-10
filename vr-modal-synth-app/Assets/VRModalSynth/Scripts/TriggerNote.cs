using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.UIElements;
using UnityEngine.XR.Interaction.Toolkit;


/*
*	NOTE: There will be problems when the bar is rotated. Currently the 
*	position is only determined along the z axis. 
*	We might be able to do Vector3.Distance between the exiter and the 
*	endPoint, although it won't be perfect if there is variation along the width.
*	Weird bug with velocity tracking on mallet
*/

public class TriggerNote : MonoBehaviour
{
	private const bool MALLET_DAMPING_ENABLED = false;
	private const float DAMPING_ONSET_TIME = 0.15f;
    private const float RETRIGGER_WAIT_TIME = 0.2f;
    private const float MIN_VELOCITY = 0.2f;
	private const float MAX_VELOCITY = 4f;

    private const float PAD = 0.01f;
    private float length;
    private float width;
	private int note;

	private float mallet1LastCollisionTime;
    private float mallet2LastCollisionTime;

    private bool isNoteOn;
    private int objectsInCollider;
    private float collisionStartTime;

	[DllImport("ModalVR_AudioPlugin")]
	public static extern void ModalSynth_NoteOn(int note, float velocity, 
        float position);

	[DllImport("ModalVR_AudioPlugin")]
	public static extern void ModalSynth_NoteOff(int note);

	void Start()
    {
        MeshRenderer meshRenderer = GetComponent<MeshRenderer>();
        length = meshRenderer.bounds.size.z;
        width = meshRenderer.bounds.size.x;

		note = int.Parse(gameObject.name.Substring(4));

		mallet1LastCollisionTime = 0f;
        mallet2LastCollisionTime = 0f;
        isNoteOn = false;
        objectsInCollider = 0;
    }
    
    void Update()
    {
		float currentTime = Time.time;

		if (MALLET_DAMPING_ENABLED && isNoteOn && (objectsInCollider > 0) &&
				(currentTime - collisionStartTime > DAMPING_ONSET_TIME))
		{
			ModalSynth_NoteOff(note);
			isNoteOn = false;
		}
	}

    void OnCollisionEnter(Collision collision)
    {        
        // Only respond to interaction with mallet head
        if (collision.collider.name != "Mallet Head")
            return;

		float currentTime = Time.time;

        // Used to determine whether the bar is being dampened
		if (objectsInCollider == 0)
			collisionStartTime = currentTime;

		objectsInCollider++;

		GameObject mallet = collision.gameObject;

		// Prevent doubling of notes getting triggered (only for one bar)
		if (mallet.name == "Mallet 1")
        {
            if ((currentTime - mallet1LastCollisionTime) < RETRIGGER_WAIT_TIME)
                return;

            else
                mallet1LastCollisionTime = currentTime;
        }
        else if (mallet.name == "Mallet 2")
        {
            if ((currentTime - mallet2LastCollisionTime) < RETRIGGER_WAIT_TIME)
                return;

            else
                mallet2LastCollisionTime = currentTime;
        }
      
		GameObject malletHead = mallet.transform.GetChild(0).gameObject;

		// Make sure the mallet is actually above the bar (prevents issues
		// with colliders playing 2 notes at once)
		if (InBounds(malletHead))
		{
			// Limit the velocity so that things don't get too loud.
			// Set a minimum velocity so it's responsive enough.
			float velocity = Mathf.Clamp(Mathf.Abs(collision.relativeVelocity.y),
											MIN_VELOCITY, MAX_VELOCITY);

            //float velocity = Mathf.Clamp(
            //    mallet.GetComponent<Rigidbody>().GetPointVelocity(
            //        malletHead.transform.position).magnitude, 
            //        MIN_VELOCITY, MAX_VELOCITY);

            float barEndpointPosition = transform.position.z + (length / 2f);

			// Position in meters along the length of the bar
			float distanceFromEndpoint = Mathf.Abs(
                malletHead.transform.position.z - barEndpointPosition);

			// Position along the length of the bar (between 0 and 1). 
            // Clamped in case the mallet head center is outside of the bar.
			float position = Mathf.Clamp(distanceFromEndpoint / length, 0f, 1f);

			ModalSynth_NoteOn(note, velocity, position);
			isNoteOn = true;
		}
    }

    void OnCollisionExit(Collision collision)
    {
		if (collision.collider.name != "Mallet Head")
			return;

		objectsInCollider--;
	}

    private bool InBounds(GameObject malletHead)
    {
        float xLowerBound = transform.position.x - (width + PAD) / 2;
        float xUpperBound = transform.position.x + (width + PAD) / 2;
        float zLowerBound = transform.position.z - (length / 2);
        float zUpperBound = transform.position.z + (length / 2);

        Vector3 malletHeadPosition = malletHead.transform.position;

        return ((xLowerBound < malletHeadPosition.x) && 
                (malletHeadPosition.x < xUpperBound) && 
                (zLowerBound < malletHeadPosition.z) && 
                (malletHeadPosition.z < zUpperBound));
    }
}
