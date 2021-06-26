#include "Ethernet.h"

fnet_netif_desc_t     Ethernet::_iface;
fnet_link_desc_t      Ethernet::_link;
fnet_dhcp_cln_desc_t  Ethernet::_dhclient;
fnet_dhcp_cln_options Ethernet::_dhopts;
fnet_dns_desc_t       Ethernet::_dns;

IntervalTimer Ethernet::_service_poll;

uint8_t* Ethernet::_fnet_heap;

fnet_mutex_api_t Ethernet::_mutex_api;
fnet_timer_api_t Ethernet::_timer_api;

// unimplemented mutex functions
fnet_return_t eth_mutex_init(fnet_mutex_t* mutex) { return FNET_OK; }
void eth_mutex_release(fnet_mutex_t* mutex) {}
void eth_mutex_lock(fnet_mutex_t* mutex) {}
void eth_mutex_unlock(fnet_mutex_t* mutex) {}

// callbacks
void eth_link_callback(fnet_netif_desc_t netif, fnet_bool_t connected, void* callback_param) {}

/**
 * @brief Initialize the ethernet interface.
 * @return uint8_t 0 on success, non-zero on failure
 */
uint8_t Ethernet::init(uint8_t* mac_addr) {
    // set the interface
    _iface = FNET_CPU_ETH0_IF;

    // check if initialized, if not, initialize
    if (fnet_netif_is_initialized(_iface)) {
        return 0;
    }

    Serial.print("[ETH] init fnet v");
    Serial.println(FNET_VERSION);

    struct fnet_init_params init_params;

    _mutex_api = {
        .mutex_init    = eth_mutex_init,
        .mutex_release = eth_mutex_release,
        .mutex_lock    = eth_mutex_lock,
        .mutex_unlock  = eth_mutex_unlock
    };

    _timer_api = {
        .timer_get_ms = millis,
        .timer_delay  = 0
    };

    _fnet_heap = (uint8_t*) malloc(FNET_NET_HEAP_SIZE);

    init_params.mutex_api = &_mutex_api;
    init_params.timer_api = &_timer_api;
    init_params.netheap_ptr = _fnet_heap;
    init_params.netheap_size = FNET_NET_HEAP_SIZE;

    // initialize fnet
    if (fnet_init(&init_params) != FNET_OK) {
        return 1;
    }

    // initialize interface
    // hw_addr_size is 6 for ethernet interface
    if (fnet_netif_init(_iface, mac_addr, 6) == FNET_OK) {
        fnet_link_params_t link_params = {
            .netif_desc     = _iface,
            .callback       = eth_link_callback,
            .callback_param = NULL
        };
        
        // .....LINK START!
        _link = fnet_link_init(&link_params);

        // start service poll for 10ms
        _service_poll.begin(fnet_service_poll, 10000);

        Serial.println("[ETH:OK] completed initialization");

        return 0;
    }

    return -1;
}

/**
 * @brief Request an IP using DHCP.
 * @return uint8_t OK on success, FAIL on failure
 */
uint8_t Ethernet::req_ip() {
    fnet_dhcp_cln_params_t dhcp_params;

    fnet_memset_zero(&dhcp_params, sizeof(dhcp_params));
    dhcp_params.netif = _iface;

    if (_dhclient = fnet_dhcp_cln_init(&dhcp_params)) {
        fnet_dhcp_cln_set_callback_updated(_dhclient, &dhcp__callback, NULL);
        return 0;
    } else {
        Serial.println("[ETH:FAIL] DHCP initialization failed");
        return -1;
    }
}

/**
 * @brief Callback for DHCP server assignment response.
 * @param desc Client handle
 * @param netif Interface
 * @param cookie Cookie
 */
void Ethernet::dhcp__callback(fnet_dhcp_cln_desc_t desc, fnet_netif_desc_t netif, void* cookie) {
    Serial.println("[ETH:OK] DHCP answered");
    fnet_dhcp_cln_get_options(_dhclient, &_dhopts);

    char addr[FNET_IP4_ADDR_STR_SIZE];
    fnet_inet_ntop(AF_INET, &_dhopts.ip_address.s_addr, addr, FNET_IP4_ADDR_STR_SIZE);
    char mask[FNET_IP4_ADDR_STR_SIZE];
    fnet_inet_ntop(AF_INET, &_dhopts.netmask.s_addr, mask, FNET_IP4_ADDR_STR_SIZE);

    Serial.print("[ETH] I am ");
    Serial.print(addr);
    Serial.print("/");
    Serial.println(mask);
}
