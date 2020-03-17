//
// Created by Alexander Lenhardt on 16.03.20.
//

#include "DSPNode.hpp"
#include <sstream>

DSPNode::Pin emptyPin() {
    static auto pin = DSPNode::Pin("", nullptr);
    return pin;
}

DSPNode::Pin DSPNode::output(const std::string &outName) {
    return std::make_pair(outName, this);
}

DSPNode::Pin DSPNode::input(const std::string &inName) {
    return std::make_pair(inName, this);
}

DSPNode::DSPNode(const std::string &name, size_t sampleRate) : m_sr(sampleRate), m_name(name) {
}

DSPNode::~DSPNode() {

}

size_t DSPNode::samplerate() const {
    return m_sr;
}

DSPNode::Result
DSPNode::connect(const std::string &srcOutName, DSPNode::DSPNodePtr destNode, const std::string &destInputName) {
    return DSPNode::Result();
}

void DSPNode::addInput(const std::string &id) {
    m_inputs[id] = emptyPin();
}

void DSPNode::addOutput(const std::string &id, const std::any &initialValue) {
    m_outputs[id] = emptyPin();
    m_outputValues[id] = initialValue;
}

DSPNode& operator>>(const DSPNode::Pin &upstream, const DSPNode::Pin &downstream) {
    DSPNode::DSPNodePtr downstreamNode = downstream.second;
    std::string downstreamPinId = downstream.first;

    if (downstreamNode->m_inputs.find(downstreamPinId) == downstreamNode->m_inputs.end()) {
        auto err = std::string("Input ") + downstreamPinId + " does not exist for node " + downstreamNode->name();
        throw std::runtime_error(err);
    }
    downstreamNode->m_inputs[downstreamPinId] = upstream;

    return *downstreamNode;
}

void DSPNode::setSamplerate(size_t rate, bool propagateDownstream) {
    if (rate != m_sr) {
        m_sr = rate;
        onSamplerateUpdate();
    }
    for([[maybe_unused]]const auto & [id, downstreamPin] : m_outputs) {
        downstreamPin.second->setSamplerate(rate, true);
    }
}

const std::string &DSPNode::name() const {
    return m_name;
}

