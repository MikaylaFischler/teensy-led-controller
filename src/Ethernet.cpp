#include "Ethernet.h"

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
    }
}
