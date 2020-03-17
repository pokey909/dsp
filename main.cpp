#include <iostream>
#include "Lowpass.hpp"
#include "OscNoise.hpp"
#include <fstream>

int main() {
    Lowpass lp1;
    Lowpass lp2;
    Lowpass lp3;
    OscNoise noise;

    auto &chain = ((noise.output("audio") >> lp1.input("audio"))
    .output("audio") >> lp2.input("audio"))
    .output("audio") >> lp3.input("audio");


    std::ofstream f("out.raw", std::ios::binary);
    for(int i = 0; i < chain.samplerate(); ++i) {
        chain.process();
        auto out = chain.readPin<SampleF32>(chain.output("audio"));
        f.write((char*)out.stereoData, sizeof(out.stereoData));
    }
    f.close();

    return 0;
}
