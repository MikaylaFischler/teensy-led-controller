#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <Arduino.h>
#include <fnet.h>

#define FNET_NET_HEAP_SIZE 64 * 1024u

class Ethernet {
public:
    uint8_t init(uint8_t* mac_addr);
private:
    fnet_netif_desc_t _iface;
    fnet_link_desc_t _link;
    fnet_dhcp_cln_desc_t _dhclient;
    fnet_dns_desc_t _dns;

    IntervalTimer _service_poll;

    uint8_t* _fnet_heap;

    fnet_mutex_api_t _mutex_api;
    fnet_timer_api_t _timer_api;
};

#endif
