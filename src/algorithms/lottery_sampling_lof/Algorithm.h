#ifndef _LotterySamplingLOF_Algorithm_H_
#define _LotterySamplingLOF_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling_lof/Types.h"


namespace LotterySamplingLOF {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    TicketOrder<Element<T>> ticket_order;

    TicketUtils ticket_generator;
    unsigned int m;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;
};


}

#include "algorithms/lottery_sampling_lof/Algorithm.ipp"

#endif //_LotterySamplingLFO_Algorithm_H_