//
// Created by Alexander Lenhardt on 16.03.20.
//

#ifndef DSP_OSCNOISE_HPP
#define DSP_OSCNOISE_HPP

#include "DSPNode.hpp"
#include "types.hpp"
#include <random>

class OscNoise : public DSPNode {
public:
    OscNoise() : DSPNode("OscNoise") {
        addOutput("audio");
    }

    void process() override {
        auto val = (float)distribution(generator);
        if (val < 0.0 || val > 1.0) {
            val = 1.0;
        } else {
            val *= 0.5;
        }
        SampleF32 smp;
        smp = val;
        store("audio", smp);
    }

protected:
    void onSamplerateUpdate() override {
        // do something... or not
    }

private:
    std::vector<SampleF32> m_output;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution{0.5,0.2};

};


#endif //DSP_OSCNOISE_HPP
