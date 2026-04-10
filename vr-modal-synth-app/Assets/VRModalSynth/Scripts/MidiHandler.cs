using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using AOT;
using UnityEngine;
using UnityEngine.Events;

public struct MidiMessage
{
    public byte status;
    public byte data1;
    public byte data2;

    public MidiMessage(byte status, byte data1, byte data2)
    {
        this.status = status;
        this.data1 = data1;
        this.data2 = data2;
    }
}

public class MidiHandler : MonoBehaviour
{
    public UnityEvent<MidiMessage> onMidiInMessage;

    private delegate void MidiInProc(IntPtr hMidiIn, uint wMsg,
        IntPtr dwInstance, IntPtr dwParam1, IntPtr dwParam2);

    [DllImport("winmm.dll")]
    private static extern uint midiInGetNumDevs();

    [DllImport("winmm.dll")]
    private static extern uint midiInOpen(out IntPtr handle, uint deviceID,
        MidiInProc callback, IntPtr instance, uint flags);

    [DllImport("winmm.dll")]
    private static extern uint midiInStart(IntPtr handle);

    [DllImport("winmm.dll")]
    private static extern uint midiInStop(IntPtr handle);

    [DllImport("winmm.dll")]
    private static extern uint midiInClose(IntPtr handle);

    private const uint CALLBACK_FUNCTION = 0x00030000;
    private const uint MIM_DATA = 0x3C3;

    private IntPtr midiHandle = IntPtr.Zero;
    private MidiInProc callback;

    private Queue<MidiMessage> midiQueue = new Queue<MidiMessage>();
    private object queueLock = new object();

	void Start()
    {
        uint numDevices = midiInGetNumDevs();

        if (numDevices == 0)
            return;

		callback = new MidiInProc(MidiInHandler);
        midiInOpen(out midiHandle, 0, callback, IntPtr.Zero, CALLBACK_FUNCTION);
        midiInStart(midiHandle);
	}

	[MonoPInvokeCallback(typeof(MidiInProc))]
	private void MidiInHandler(IntPtr hMidiIn, uint wMsg, 
        IntPtr dwInstance, IntPtr dwParam1, IntPtr dwParam2)
    {
        if (wMsg == MIM_DATA)
        {
            uint data = (uint)dwParam1.ToInt32();
            byte status = (byte)(data & 0xFF);
            byte data1 = (byte)((data >> 8) & 0xFF);
            byte data2 = (byte)((data >> 16) & 0xFF);

            MidiMessage msg = new MidiMessage(status, data1, data2);

            lock (queueLock)
            {
                midiQueue.Enqueue(msg);
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        lock (queueLock)
        {
            while (midiQueue.Count > 0)
            {
                MidiMessage msg = midiQueue.Dequeue();
                Debug.Log($"Status: {msg.status}, Data1: {msg.data1}, Data2: {msg.data2}");

                onMidiInMessage?.Invoke(msg);
            }
        }
    }

    void OnDestroy()
    {
        if (midiHandle != IntPtr.Zero)
        {
            midiInStop(midiHandle);
            midiInClose(midiHandle);
            midiHandle = IntPtr.Zero;
        }

        callback = null;
    }
}
