#ifndef _BasicLotterySamplingHH_Types_H_
#define _BasicLotterySamplingHH_Types_H_

#include "data_structures/BinaryHeap.h"
#include "data_structures/SortedTree.h"
#include "utils/TicketUtils.h"

namespace BasicLotterySamplingHH {

    
using namespace std;

template<class Element>
using FrequencyOrder = SortedTree::SortedTree<Element, &Element::compare_freq, &Element::frequency_order_locator>;

template<class Element>
using TicketOrder = BinaryHeap::BinaryHeap<Element, &Element::compare_ticket, &Element::ticket_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedTree::Locator<Element, &Element::compare_freq>;

using TicketOrderLocator = BinaryHeap::Locator;

template<class Element>
using FrequencyOrderIterator = SortedTree::Iterator<Element, &Element::compare_freq>;

template<class T>
struct Element {

    T id;
    Ticket ticket;
    unsigned int count;

    Element(const T& id) {
        this->id = id;
    }

    bool compare_freq(const Element<T>& element) const {
        return this->count < element.count;
    }

    bool compare_ticket(const Element<T>& element) const {
        return this->ticket < element.ticket;
    }

    TicketOrderLocator ticket_order_locator = -1;
    FrequencyOrderLocator<Element<T>> frequency_order_locator;

    unsigned int get_count() const {
        return count;
    }
};


}

#endif //_BasicLotterySamplingHH_Types_H_