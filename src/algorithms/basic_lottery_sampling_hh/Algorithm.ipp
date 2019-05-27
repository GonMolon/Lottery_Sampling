#include "algorithms/basic_lottery_sampling_hh/Algorithm.h"
#include <math.h>


namespace BasicLotterySamplingHH {


template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    phi = stod(parameters.get_parameter("-phi"));
    delta = stod(parameters.get_parameter("-delta"));
    error = stod(parameters.get_parameter("-error"));
    r = phi/error * log(1/(phi * delta));
    int seed;
    if(parameters.has_parameter("-seed")) {
        seed = stoi(parameters.get_parameter("-seed"));
    } else {
        seed = -1;
    }
    ticket_generator = TicketUtils(seed);
}

template<class T>
FrequencyOrder<Element<T>>& Algorithm<T>::get_frequency_order() {
    return frequency_order;
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {

    element.ticket = ticket_generator.generate_token();

    Ticket threshold = (Ticket) (get_threshold() * TicketUtils::MAX_TICKET);
    if(element.ticket < threshold) {
        return false;
    }

    frequency_order.insert_element(&element);
    ticket_order.push(&element);

    if(this->sample_size() > 0) {
        for(int i = 0; i < 2; ++i) {
            Element<T>* element_min_ticket = ticket_order.top();
            if(element_min_ticket->ticket < threshold) {
                frequency_order.remove_element(element_min_ticket);
                ticket_order.pop();
                this->remove_element(element_min_ticket->id);
            } else {
                break;
            }
        }
    }

    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    // Updating frequency
    frequency_order.increase_key(&element);

    // Updating ticket
    Token token = ticket_generator.generate_token();
    if(token > element.ticket) {
        element.ticket = token;
        ticket_order.key_updated(&element);
    }
}

template<class T>
unordered_map<string, double> Algorithm<T>::get_custom_stats() const {
    unordered_map<string, double> stats;
    stats["threshold"] = get_threshold();
    return stats;
}

template<class T>
double Algorithm<T>::get_threshold() const {
    double threshold = 1 - (1 / ((phi / r) * this->N));
    return max(threshold, 0.0);
}

template<class T>
double Algorithm<T>::get_frequency_threshold(double f) const {
    assert(f == phi);
    return phi - error;
}


}
