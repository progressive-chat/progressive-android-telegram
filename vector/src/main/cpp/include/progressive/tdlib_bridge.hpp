#ifndef PROGRESSIVE_TDLIB_BRIDGE_HPP
#define PROGRESSIVE_TDLIB_BRIDGE_HPP

#ifdef PROGRESSIVE_HAS_TDLIB

extern "C" {

void* td_json_client_create();
void td_json_client_send(void* client, const char* request);
const char* td_json_client_receive(void* client, double timeout);
const char* td_json_client_execute(void* client, const char* request);
void td_json_client_destroy(void* client);

}

#endif // PROGRESSIVE_HAS_TDLIB
#endif // PROGRESSIVE_TDLIB_BRIDGE_HPP
