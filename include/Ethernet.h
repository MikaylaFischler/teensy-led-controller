#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <Arduino.h>
#include <fnet.h>

#define FNET_NET_HEAP_SIZE 64 * 1024u

class Ethernet {
public:
    static uint8_t init(uint8_t* mac_addr);
    static uint8_t req_ip();
private:
    static fnet_netif_desc_t     _iface;
    static fnet_link_desc_t      _link;
    static fnet_dhcp_cln_desc_t  _dhclient;
    static fnet_dhcp_cln_options _dhopts;
    static fnet_dns_desc_t       _dns;

    static IntervalTimer _service_poll;

    static uint8_t* _fnet_heap;

    static fnet_mutex_api_t _mutex_api;
    static fnet_timer_api_t _timer_api;

    static void link_state__callback(fnet_netif_desc_t netif, fnet_bool_t connected, void* callback_param);
    static void dhcp__callback(fnet_dhcp_cln_desc_t desc, fnet_netif_desc_t netif, void* callback_param);
};

#endif
