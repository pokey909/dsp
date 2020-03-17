//
// Created by Alexander Lenhardt on 16.03.20.
//

#ifndef DSP_DSPNODE_HPP
#define DSP_DSPNODE_HPP

#include <vector>
#include <memory>
#include <any>
#include <unordered_map>
#include <tuple>
#include <string>
#include <sstream>

class DSPNode {
public:
    struct Result {
        explicit Result(std::string errorMessage = "") : errString(std::move(errorMessage)) {}
        std::string errString;
        operator bool() const {
            return errString.empty();
        }
    };
    using DSPNodePtr = DSPNode*;
    using Pin = std::pair<std::string, DSPNodePtr>;

    explicit DSPNode(const std::string &name, size_t sampleRate = 44100);
    virtual ~DSPNode();

    /**
     * Set the samplerate for the component.
     * Each call to process() will assume time advances by 1/SR seconds
     *
     * @param rate Samplerate in Hz
     * @param propagateDownstream If true, then all downstream connected Nodes will be set to the same samplerate.
     */
    void setSamplerate(size_t rate, bool propagateDownstream = false);
    [[nodiscard]] size_t samplerate() const;

    /**
     * Consumes and processes 1 sample per call from the inputs and returns a list of updated output values.
     * @return a list of output values.
     */
    virtual void process() = 0;

    /**
     * Connect the output of this DSPNode to a downstream DSPNode's input
     *
     * @param srcOutName String ID of the output pin
     * @param destNode Weak pointer to a downstream destination DSPNode.
     * @param destInputName String ID of the destination input pin
     * @return Result which can be cast to bool. Result == true is success while false means error.
     */
    Result connect(const std::string &srcOutName, DSPNodePtr destNode, const std::string &destInputName);

    Pin output(const std::string& outName);
    Pin input(const std::string& inName);

    [[nodiscard]] const std::string& name() const;

    template<class T>
    T readPin(const Pin& pin);

protected:
    void addInput(const std::string &id);
    void addOutput(const std::string &id, const std::any &initialValue = std::any());

    template<class T>
    T readOutput(const std::string &id) {
        return std::any_cast<T>(m_outputValues[id]);
    }

    template<class T>
    void store(const std::string &id, const T& value) {
        m_outputValues[id] = std::any(value);
    }
    virtual void onSamplerateUpdate() = 0;

private:
    friend DSPNode& operator>>(const DSPNode::Pin &upstream, const DSPNode::Pin &downstream);

    size_t m_sr;
    std::unordered_map<std::string, Pin> m_inputs;
    std::unordered_map<std::string, Pin> m_outputs;
    std::unordered_map<std::string, std::any> m_outputValues;
    std::string m_name;
};

template<class T>
T DSPNode::readPin(const DSPNode::Pin &pin) {
    const auto &id = pin.first;
    const auto node = pin.second;
    const auto &valueMap = node->m_outputValues;
    const auto value = valueMap.find(id);
    if (value == valueMap.end()) {
        std::stringstream ss;
        ss << "Could not find an upstream output called `" << id << "` on node `" << node->name() << "`.";
        throw std::runtime_error(ss.str());
    }
    return std::any_cast<T>(value->second);
}


DSPNode& operator>>(const DSPNode::Pin &upstream, const DSPNode::Pin &downstream);

#endif //DSP_DSPNODE_HPP
