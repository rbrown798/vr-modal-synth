#include "AudioPluginUtil.h"
#include "Synthesizer.h"


namespace Event 
{
    enum MessageCommand
    {
        NOTE_ON,
        NOTE_OFF,
        SET_L_EAR_POS,
        SET_L_EAR_DIR,
        SET_R_EAR_POS,
        SET_R_EAR_DIR
    };

    struct Message
    {
        MessageCommand commandCode;
        float data1;
        float data2;
        float data3;
    };

    static AudioPluginUtil::RingBuffer<8192, Message> messages;
};

namespace ModalSynth 
{
    enum Param
    { 
        P_MALLET_HEAD_STIFFNESS,
        P_MALLET_HEAD_DIAMETER,
        P_BAR_TIMBRE,
        P_BAR_DAMPING,
        P_BAR_METALLIC,
        P_RESONATOR_TREMOLO_RATE,
        P_RESONATORS_ON,
        P_LFO_ON,
        P_PEDAL_VALUE,
        P_NUM
    };

    struct EffectData
    {
        float p[P_NUM];
        float prevp[P_NUM];
        Synthesizer* synth;
    };

    int InternalRegisterEffectDefinition( 
            UnityAudioEffectDefinition& definition)
    {
        int numparams = P_NUM;
        definition.paramdefs = new UnityAudioParameterDefinition[numparams];

        AudioPluginUtil::RegisterParameter(definition, "MalletStiffness",
            "", 0.0f, 1.0f, 0.5f, 1.0f, 1.0f, P_MALLET_HEAD_STIFFNESS);

        AudioPluginUtil::RegisterParameter(definition, "MalletDiameter", 
            "cm", 0.2f, 4.0f, 2.0f, 1.0f, 1.0f, P_MALLET_HEAD_DIAMETER);

        AudioPluginUtil::RegisterParameter(definition, "BarTimbre", "", 0.0f, 
            1.0f, 1.0f, 1.0f, 1.0f, P_BAR_TIMBRE);

        // changed damping min to 0.02. would be better to have range from 0 to 1
        // and have this dealt with by the synth
        AudioPluginUtil::RegisterParameter(definition, "BarDamping", "", 0.02f,
            1.0f, 0.2f, 1.0f, 1.0f, P_BAR_DAMPING);

        AudioPluginUtil::RegisterParameter(definition, "BarMetallic", "", 0.f,
            1.0f, 0.5f, 1.0f, 1.0f, P_BAR_METALLIC);

        AudioPluginUtil::RegisterParameter(definition, "TremoloRate", "", 
            0.1f, 5.0f, 4.0f, 1.0f, 1.0f, P_RESONATOR_TREMOLO_RATE);

        AudioPluginUtil::RegisterParameter(definition, "TubeOn", "",
            -1.f, 1.f, 1.f, 1.0f, 1.0f, P_RESONATORS_ON);

        AudioPluginUtil::RegisterParameter(definition, "LfoOn", "",
            -1.f, 1.f, 1.f, 1.0f, 1.0f, P_LFO_ON);

        AudioPluginUtil::RegisterParameter(definition, "PedalValue", "",
            0.f, 1.f, 0.f, 1.0f, 1.0f, P_PEDAL_VALUE);

        return numparams;
    }

    UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK CreateCallback(
            UnityAudioEffectState* state)
    {
        EffectData* effectdata = new EffectData;
        memset(effectdata, 0, sizeof(EffectData));

        effectdata->synth = new Synthesizer();
        effectdata->synth->initialize(static_cast<float>(state->samplerate), 
            state->dspbuffersize);
        
        state->effectdata = effectdata;
        AudioPluginUtil::InitParametersFromDefinitions(
            InternalRegisterEffectDefinition, effectdata->p);
        return UNITY_AUDIODSP_OK;
    }

    UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK ReleaseCallback(
            UnityAudioEffectState* state)
    {
        EffectData* effectdata = state->GetEffectData<EffectData>();
        delete effectdata->synth;
        delete effectdata;
        return UNITY_AUDIODSP_OK;
    }

    UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK SetFloatParameterCallback(
            UnityAudioEffectState* state, int index, float value)
    {
        EffectData* effectData = state->GetEffectData<EffectData>();
        if (index >= P_NUM)
            return UNITY_AUDIODSP_ERR_UNSUPPORTED;
        effectData->p[index] = value;

        return UNITY_AUDIODSP_OK;
    }

    UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK GetFloatParameterCallback(
            UnityAudioEffectState* state, int index, float* value, 
            char *valuestr)
    {
        EffectData* data = state->GetEffectData<EffectData>();
        if (index >= P_NUM)
            return UNITY_AUDIODSP_ERR_UNSUPPORTED;
        if (value != NULL)
            *value = data->p[index];
        if (valuestr != NULL)
            valuestr[0] = 0;
        return UNITY_AUDIODSP_OK;
    }

    int UNITY_AUDIODSP_CALLBACK GetFloatBufferCallback(
            UnityAudioEffectState* state, const char* name, float* buffer, 
            int numsamples)
    {
        return UNITY_AUDIODSP_OK;
    }

    static void SetParameters(EffectData* effectData)
    {
        Synthesizer* synth = effectData->synth;
        float* p = effectData->p;

        synth->setMalletHeadStiffness(p[P_MALLET_HEAD_STIFFNESS]);
        synth->setMalletHeadDiameter(p[P_MALLET_HEAD_DIAMETER]);
        synth->setMalletHeadDensity(8000.0f);
        synth->setBarDamping(p[P_BAR_DAMPING]);
        synth->setBarTimbre(p[P_BAR_TIMBRE]);
        synth->setBarMetallic(p[P_BAR_METALLIC]);
        synth->setMotorFrequency(p[P_RESONATOR_TREMOLO_RATE]);
        synth->setTubeOn(p[P_RESONATORS_ON] > 0);
        synth->setMotorOn(p[P_LFO_ON] > 0);
        synth->setPedalValue(p[P_PEDAL_VALUE]);
    }

    static void HandleEvent(EffectData* effectData, Event::Message msg)
    {
        switch (msg.commandCode)
        {
        case Event::NOTE_ON:
            effectData->synth->noteOn(static_cast<int>(msg.data1), msg.data2, msg.data3);
            break;
        case Event::NOTE_OFF:
            effectData->synth->noteOff(static_cast<int>(msg.data1));
            break;
        case Event::SET_L_EAR_POS:
            effectData->synth->setLeftEarPosition(Vector3(msg.data1, msg.data2, msg.data3));
            break;
        case Event::SET_L_EAR_DIR:
            effectData->synth->setLeftEarDirection(Vector3(msg.data1, msg.data2, msg.data3));
            break;
        case Event::SET_R_EAR_POS:
            effectData->synth->setRightEarPosition(Vector3(msg.data1, msg.data2, msg.data3));
            break;
        case Event::SET_R_EAR_DIR:
            effectData->synth->setRightEarDirection(Vector3(msg.data1, msg.data2, msg.data3));
            break;
        default:
            break;
        }
    }

    UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK ProcessCallback(
        UnityAudioEffectState* state, float* inbuffer, float* outbuffer,
        unsigned int length, int inchannels, int outchannels)
    {
        EffectData* effectData = state->GetEffectData<EffectData>();

        // Update parameters if any value changed
        if (memcmp(effectData->prevp, effectData->p, sizeof(effectData->p)) != 0)
        {
            memcpy(effectData->prevp, effectData->p, sizeof(effectData->p));
            SetParameters(effectData);
        }

        Event::Message msg;
        while (Event::messages.Read(msg))
        {
            HandleEvent(effectData, msg);
        }

        effectData->synth->renderBlock(outbuffer, length, outchannels);

        return UNITY_AUDIODSP_OK;
    }

    extern "C" UNITY_AUDIODSP_EXPORT_API void ModalSynth_NoteOn(int note,
            float velocity, float position)
    {
        Event::Message msg;
        msg.commandCode = Event::NOTE_ON;
        msg.data1 = static_cast<float>(note);
        msg.data2 = velocity;
        msg.data3 = position;
        Event::messages.Feed(msg);
    }

    extern "C" UNITY_AUDIODSP_EXPORT_API void ModalSynth_NoteOff(int note)
    {
        Event::Message msg;
        msg.commandCode = Event::NOTE_OFF;
        msg.data1 = static_cast<float>(note);
        msg.data2 = 0.f;
        msg.data3 = 0.f;
        Event::messages.Feed(msg);
    }

    extern "C" UNITY_AUDIODSP_EXPORT_API void ModalSynth_SetLeftEarPosition(
        Vector3 leftEarPosition)
    {
        Event::Message msg;
        msg.commandCode = Event::SET_L_EAR_POS;
        msg.data1 = leftEarPosition.x;
        msg.data2 = leftEarPosition.y;
        msg.data3 = leftEarPosition.z;
        Event::messages.Feed(msg);
    }

    extern "C" UNITY_AUDIODSP_EXPORT_API void ModalSynth_SetLeftEarDirection(
        Vector3 leftEarDirection)
    {
        Event::Message msg;
        msg.commandCode = Event::SET_L_EAR_DIR;
        msg.data1 = leftEarDirection.x;
        msg.data2 = leftEarDirection.y;
        msg.data3 = leftEarDirection.z;
        Event::messages.Feed(msg);
    }

    extern "C" UNITY_AUDIODSP_EXPORT_API void ModalSynth_SetRightEarPosition(
        Vector3 rightEarPosition)
    {
        Event::Message msg;
        msg.commandCode = Event::SET_R_EAR_POS;
        msg.data1 = rightEarPosition.x;
        msg.data2 = rightEarPosition.y;
        msg.data3 = rightEarPosition.z;
        Event::messages.Feed(msg);
    }

    extern "C" UNITY_AUDIODSP_EXPORT_API void ModalSynth_SetRightEarDirection(
        Vector3 rightEarDirection)
    {
        Event::Message msg;
        msg.commandCode = Event::SET_R_EAR_DIR;
        msg.data1 = rightEarDirection.x;
        msg.data2 = rightEarDirection.y;
        msg.data3 = rightEarDirection.z;
        Event::messages.Feed(msg);
    }

    //// SetResonatorsEnabled
    //extern "C" UNITY_AUDIODSP_EXPORT_API void ModalSynth_SetEnableResonators(bool value)
    //{
    //    Event::Message msg;
    //    msg.commandCode = Event::SET_ENABLE_RESONATORS;
    //    msg.data1 = value;

    //}

    //extern "C" UNITY_AUDIODSP_EXPORT_API void ModalSynth_SetEnableLFO(bool value)
    //{

    //}
}
