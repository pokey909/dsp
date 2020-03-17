//
// Created by Alexander Lenhardt on 16.03.20.
//

#ifndef DSP_LOWPASS_HPP
#define DSP_LOWPASS_HPP

#include "DSPNode.hpp"
#include "types.hpp"

class Lowpass : public DSPNode {
public:
    explicit Lowpass(size_t order = 4) : DSPNode("Lowpass"), m_inputValues(order) {
        addInput("audio");
        addOutput("audio");

        // n-th order moving average
        for(int i = 0; i < order; ++i) {
            m_coef.push_back(1.0 / order);
        }
    }

    void process() override {
        auto out = readOutput<SampleF32>("audio"); // reads from its own output buffer
        m_inputValues[m_idx++] = readPin<SampleF32>(input("audio")); // reads from upstream node output which is connected to our input
        for(int t = 0; t < m_coef.size(); ++t) {
           out += m_inputValues[t] * m_coef[t];
        }
        store("audio", out);
        m_idx %= m_coef.size();
    }

protected:
    void onSamplerateUpdate() override {
        // do something... or not
    }

private:
    std::vector<double> m_coef;
    std::vector<SampleF32> m_inputValues; // should be a ringbuffer.
    size_t m_idx{0};
};

#endif //DSP_LOWPASS_HPP
