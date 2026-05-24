#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

/* ── TDLib JSON C API stub ──────────────────────────────────────────
   Provides td_json_client_* symbols so progressive_native.so links
   and the Telegram auth/messaging UI works without a real TDLib .so.

   Auth flow simulated:
     1. Client create → returns "authorizationStateWaitPhoneNumber"
     2. Send setAuthenticationPhoneNumber → returns "authorizationStateWaitCode"
     3. Send checkAuthenticationCode → returns "authorizationStateReady"
     4. All other requests → returns "ok" or empty responses

   To replace with real TDLib: delete this file, link against libtdjson.
   ─────────────────────────────────────────────────────────────────── */

typedef struct {
    int step;         /* 0=phone, 1=code, 2=ready, 3=closed */
    int has_updates;  /* number of pending updates */
    int msg_id;       /* fake message ID counter */
    int chat_id;      /* fake chat ID counter */
    pthread_mutex_t lock;
    char *staged_response;
} td_client_t;

static td_client_t *client_new(void) {
    td_client_t *c = calloc(1, sizeof(*c));
    pthread_mutex_init(&c->lock, NULL);
    c->step = 0;
    c->msg_id = 1;
    c->chat_id = 1000;
    return c;
}

static void client_free(td_client_t *c) {
    if (!c) return;
    pthread_mutex_destroy(&c->lock);
    free(c->staged_response);
    free(c);
}

static void set_response(td_client_t *c, const char *r) {
    pthread_mutex_lock(&c->lock);
    free(c->staged_response);
    c->staged_response = r ? strdup(r) : NULL;
    pthread_mutex_unlock(&c->lock);
}

void *td_json_client_create(void) {
    return client_new();
}

void td_json_client_send(void *client, const char *request) {
    if (!client || !request) return;
    td_client_t *c = (td_client_t *)client;

    /* Minimal JSON decision: look for @type in the request */
    if (strstr(request, "setTdlibParameters")) {
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{\"@type\":\"authorizationStateWaitPhoneNumber\"}}");
        c->has_updates = 1;
    } else if (strstr(request, "setAuthenticationPhoneNumber")) {
        c->step = 1;
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{"
            "\"@type\":\"authorizationStateWaitCode\","
            "\"code_info\":{"
            "\"@type\":\"authenticationCodeInfo\","
            "\"phone_number\":\"+1234567890\","
            "\"type\":{\"@type\":\"authenticationCodeTypeSms\",\"length\":5},"
            "\"next_type\":null,"
            "\"timeout\":60}}}");
    } else if (strstr(request, "checkAuthenticationCode")) {
        c->step = 2;
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{\"@type\":\"authorizationStateReady\"}}");
        /* Also stage a ready user */
        c->has_updates = 2;
    } else if (strstr(request, "checkAuthenticationPassword")) {
        c->step = 2;
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{\"@type\":\"authorizationStateReady\"}}");
    } else if (strstr(request, "logOut")) {
        c->step = 3;
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{\"@type\":\"authorizationStateClosed\"}}");
    } else if (strstr(request, "getMe")) {
        /* Return a fake user after auth is ready */
        if (c->step >= 2) {
            set_response(c,
                "{\"@type\":\"user\",\"id\":777000,\"first_name\":\"Telegram\","
                "\"last_name\":\"\",\"username\":\"telegram\",\"phone_number\":\"+1234567890\","
                "\"type\":{\"@type\":\"userTypeRegular\"},"
                "\"profile_photo\":null,\"is_verified\":false,\"is_premium\":false}");
        }
    } else if (strstr(request, "loadChats") || strstr(request, "getChats")) {
        /* Return empty chat list */
        set_response(c,
            "{\"@type\":\"chats\",\"chat_ids\":[],\"total_count\":0}");
    } else if (strstr(request, "getOption")) {
        /* Return fake version and my_id */
        if (strstr(request, "\"version\"")) {
            set_response(c,
                "{\"@type\":\"optionValueString\",\"value\":\"1.8.46\"}");
        } else if (strstr(request, "\"my_id\"")) {
            set_response(c,
                "{\"@type\":\"optionValueInteger\",\"value\":777000}");
        } else {
            set_response(c, "{\"@type\":\"ok\"}");
        }
    } else {
        /* Generic ok response */
        set_response(c, "{\"@type\":\"ok\"}");
    }
}

const char *td_json_client_receive(void *client, double timeout) {
    if (!client) return NULL;
    td_client_t *c = (td_client_t *)client;

    pthread_mutex_lock(&c->lock);
    char *r = c->staged_response;
    c->staged_response = NULL;

    /* Generate updateNewMessage after auth is ready */
    if (!r && c->step >= 2 && c->has_updates > 0) {
        c->has_updates--;
        if (c->has_updates == 1) {
            r = strdup(
                "{\"@type\":\"updateNewMessage\","
                "\"message\":{"
                "\"@type\":\"message\","
                "\"id\":1,"
                "\"chat_id\":777000,"
                "\"sender_id\":{\"@type\":\"messageSenderUser\",\"user_id\":777000},"
                "\"date\":1000000,"
                "\"is_outgoing\":false,"
                "\"content\":{"
                "\"@type\":\"messageText\","
                "\"text\":{\"@type\":\"formattedText\",\"text\":\"Hello from Progressive Chat!\"}},"
                "\"can_be_edited\":false,"
                "\"can_be_deleted_only_for_self\":false,"
                "\"can_be_deleted_for_all_users\":false}}");
        }
    }
    pthread_mutex_unlock(&c->lock);
    return r;
}

const char *td_json_client_execute(void *client, const char *request) {
    if (!client || !request) return NULL;
    /* For ping — return ok */
    return "{\"@type\":\"ok\"}";
}

void td_json_client_destroy(void *client) {
    client_free((td_client_t *)client);
}
