#include "Algorithm.h"
#include "utils/InputParser.h"
#include <iostream>

namespace LotterySampling {


using namespace std;

template<class T>
Algorithm<T>::Algorithm(const InputParser& parameters) {
    m = (unsigned int) stoul(parameters.get_parameter("-m"));
    aging = parameters.has_parameter("-aging");
    multilevel = parameters.has_parameter("-multilevel");
    if(!multilevel) {
        this->set_monitored_size(m);
    }
    int seed;
    if(parameters.has_parameter("-seed")) {
        seed = stoi(parameters.get_parameter("-seed"));
    } else {
        seed = random_device()();
    }
    random_state = mt19937_64(seed);
    if(aging) {
        // If aging is used, then the range of the tickets is decreased
        // in half to avoid overflows.
        MAX_TICKET = static_cast<unsigned long long int>(numeric_limits<int64_t>::max() - 1);
        dist = uniform_int_distribution<Ticket>(0, MAX_TICKET);
    } else {
        MAX_TICKET = numeric_limits<uint64_t>::max();
    }
}

template<class T>
void Algorithm<T>::frequent_query(float f, ostream& stream) {
    for(auto it = frequency_order.rbegin(); it != frequency_order.rend() && (*it)->freq >= f * this->N; ++it) {
        stream << (*it)->id << " " << (*it)->freq / (float) this->N << endl;
    }
}

template<class T>
void Algorithm<T>::k_top_query(int k, ostream& stream) {
    for(auto it = frequency_order.rbegin(); it != frequency_order.rend() && k-- > 0; ++it) {
        stream << (*it)->id << " " << (*it)->freq / (float) this->N << endl;
    }
}

template<class T>
void Algorithm<T>::free_up_level_1() {
    Element<T>* replaced_element = *level_1.begin();
    level_1.erase(level_1.begin());
    if(multilevel) {
        // Element is kicked out from level 1 to level 2
        // TODO profile emplace_hint to see if it improves exec. time
        replaced_element->ticket_iterator = level_2.emplace_hint(level_2.end(), replaced_element);
        replaced_element->level = 2;
    } else {
        // Element is just removed since multilevel is not being used
        frequency_order.erase(replaced_element->frequency_iterator);
        this->remove_element(replaced_element->id);
    }
}

template<class T>
void Algorithm<T>::free_up_level_2() {
    // Element is removed
    Element<T>* removed_element = *level_2.begin();
    const T& removed_id = removed_element->id;
    level_2.erase(level_2.begin());
    frequency_order.erase(removed_element->frequency_iterator);
    this->remove_element(removed_id);
}

template<class T>
bool Algorithm<T>::insert_element(Element<T>& element) {

    element.ticket = generate_ticket();

    if(this->sample_size() < m) {
        element.freq = 1;
        element.over_estimation = 0;
        element.level = 1;
    } else {
        if(element.ticket > (*level_1.begin())->ticket) {
            element.freq = estimate_frequency((*level_1.begin())->ticket);
            element.level = 1;
            free_up_level_1();
        } else if(!level_2.empty() && element.ticket > (*level_2.begin())->ticket) {
            element.freq = estimate_frequency((*level_2.begin())->ticket);
            element.level = 2;
            free_up_level_2();
        } else {
            // New element didn't get a good enough ticket to get sampled, so it's discarded
            return false;
        }
        element.over_estimation = element.freq - 1;
    }

    element.ticket_iterator = (element.level == 1 ? level_1 : level_2).insert(&element);
    element.frequency_iterator = frequency_order.insert(&element);
    return true;
}

template<class T>
void Algorithm<T>::update_element(Element<T>& element) {
    // Updating frequency
    typename Element<T>::FrequencyOrder::iterator hint = next(element.frequency_iterator);
    frequency_order.erase(element.frequency_iterator); // It's needed to remove and reinsert an element since there isn't an "update" method in multiset
    element.freq++;
    element.frequency_iterator = frequency_order.emplace_hint(hint, &element);

    // Updating ticket
    Ticket ticket = generate_ticket();
    if(ticket > element.ticket) { // The new ticket is better than the old one
        Ticket level_1_threshold = (*level_1.begin())->ticket;
        if(element.level == 2 && level_1_threshold < ticket) {
            // element is moving from level_2 to level_1, so we kick out the lowest ticket from level_1 to level_2
            free_up_level_1();
        }

        typename Element<T>::TicketOrder::iterator hint = next(element.ticket_iterator);
        (element.level == 1 ? level_1 : level_2).erase(element.ticket_iterator);
        element.ticket = ticket; // Updating (the better) ticket
        element.ticket_iterator = (ticket > level_1_threshold ? level_1 : level_2).emplace_hint(hint, &element);
        element.level = (ticket > level_1_threshold ? 1 : 2);
    }
}

template<class T>
Ticket Algorithm<T>::generate_ticket() {
    Ticket ticket = dist(random_state);
    if(aging) {
        ticket += this->N;
    }
    return ticket;
}

template<class T>
inline unsigned int Algorithm<T>::estimate_frequency(Ticket min_ticket) const {
    // TODO Protect from infinity
    // TODO take into account aging
    return static_cast<unsigned int>(1 / (1 - min_ticket / (double) MAX_TICKET));
}

template<class T>
void print_container(const T& container) {
    for(auto it = container.rbegin(); it != container.rend(); ++it) {
        cout << (*it)->id << ", " << (*it)->ticket << ", " << (*it)->freq << ", " << (*it)->over_estimation << endl;
    }
}

template<class T>
void Algorithm<T>::print_state() {
    cout << "-----------------------" << endl;
    cout << "%%%%%% level_1 %%%%%%" << endl;
    print_container(level_1);
    cout << "-----------------------" << endl;
    cout << "%%%%%% level_2 %%%%%%" << endl;
    print_container(level_2);
    cout << "-----------------------" << endl;
    cout << "%%%%%% frequency_order %%%%%%" << endl;
    print_container(frequency_order);
    assert(level_1.size() + level_2.size() == this->sample_size());
    assert(frequency_order.size() == this->sample_size());
}


}