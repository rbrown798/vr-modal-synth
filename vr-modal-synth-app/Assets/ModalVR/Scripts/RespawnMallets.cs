using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RespawnMallets : MonoBehaviour
{
    public GameObject mallet1;
    public GameObject mallet2;

	private Rigidbody rigidBody1;
	private Rigidbody rigidBody2;

    private Vector3 mallet1StartPosition;
    private Quaternion mallet1StartRotation;

	private Vector3 mallet2StartPosition;
	private Quaternion mallet2StartRotation;

	void Start()
    {
		mallet1StartPosition = mallet1.transform.position;
		mallet1StartRotation = mallet1.transform.rotation;

		mallet2StartPosition = mallet2.transform.position;
		mallet2StartRotation = mallet2.transform.rotation;

		rigidBody1 = mallet1.GetComponent<Rigidbody>();
		rigidBody2 = mallet2.GetComponent<Rigidbody>();
    }

    public void Respawn()
    {
		mallet1.transform.position = mallet1StartPosition;
		mallet1.transform.rotation = mallet1StartRotation;
		mallet2.transform.position = mallet2StartPosition;
		mallet2.transform.rotation = mallet2StartRotation;

		rigidBody1.velocity = Vector3.zero;
		rigidBody1.angularVelocity = Vector3.zero;

		rigidBody2.velocity = Vector3.zero;
		rigidBody2.angularVelocity = Vector3.zero;
    }
}
