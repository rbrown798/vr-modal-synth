#include <cmath>
#include "ImpactForce.h"


namespace ModalSynth 
{
void ImpactForce::initialize(float sampleRate)
{
    m_dt = 1.0f / sampleRate;
    update();
}

void ImpactForce::play(float velocity)
{
    m_malletPosition = 0.0f;
    m_malletVelocity = -velocity;
}

void ImpactForce::setMalletHeadRadius(float malletHeadRadius)
{
    m_malletHeadRadius = malletHeadRadius;
    update();
}

void ImpactForce::setMalletHeadMass(float malletHeadMass)
{
    m_malletHeadMass = malletHeadMass;
    update();
}

void ImpactForce::setContactModulus(float contactModulus)
{
    m_contactModulus = contactModulus;
    update();
}

void ImpactForce::update()
{
    m_hertzSpringConstant = (4.f / 3.f) * m_contactModulus * 
                                            sqrtf(m_malletHeadRadius);
}

void ImpactForce::renderBlock(float* outbuffer, unsigned int length)
{
    for (unsigned int n = 0; n < length; n++)
    {
        float netForce = 0.0f;

        if (m_malletPosition < 0.0f)
        {   
            // netForce += m_hertzSpringConstant *
            //                 powf(fabsf(m_malletPosition), 1.5f);
                            
            float malletPos = fabsf(m_malletPosition);
            netForce += m_hertzSpringConstant * malletPos * sqrtf(malletPos);

        }

        float acceleration = netForce / m_malletHeadMass;
        
        m_malletVelocity += acceleration * m_dt;
        m_malletPosition += m_malletVelocity * m_dt;

        outbuffer[n] = netForce;
    }
}
};
