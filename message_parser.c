#include "message_parser.h"

char *get_json_string_username_password(const char *username, const char *password) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char *serialized_string = json_serialize_to_string_pretty(root_value);
    return serialized_string;
}

char *get_json_string_book(const char *title, const char *author, const char *genre, int page_count, const char *publisher) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "genre", genre);
    json_object_set_number(root_object, "page_count", page_count);
    json_object_set_string(root_object, "publisher", publisher);

    char *serialized_string = json_serialize_to_string_pretty(root_value);
    return serialized_string;
}

char *get_session_cookie(char *msg) {
    char *posStartCookie = strstr(msg, CONNECT_SID);
    char *posEndCookie = strchr(posStartCookie, ';');

    int cookieLen = posEndCookie - posStartCookie;
    char *cookie = (char *) calloc((cookieLen + 1), sizeof(char));

    memcpy(cookie, posStartCookie, cookieLen);
    return cookie;
}

char *get_token_from_body(const char *body) {
    JSON_Value *json_body = json_parse_string(body);
    char *token = (char *) json_object_get_string(json_object(json_body), TOKEN);
    return token;
}

char *get_message_body(const char *msg) {
    char *posStartLen = strstr(msg, CONTENT_LEN);
    posStartLen += LEN_NUMBER_OFFSET;

    // pentru o functionare corecta a functiei atoi se plaseaza '\0' la finalul numarului
    char *posEndLen = strchr(posStartLen, '\r');
    *posEndLen = '\0';

    int bodyLen = atoi(posStartLen);
    *posEndLen = '\r';

    char *posStartBody = strstr(msg, BODY_START_STRING);
    posStartBody += BODY_OFFSET;

    char *body = calloc(bodyLen, sizeof(char));
    memcpy(body, posStartBody, bodyLen);

    return body;
}

int get_status_code(const char *msg) {
    char *posStatusCodeStart = strchr(msg, ' ');
    ++posStatusCodeStart;

    char *statusCode = calloc(STATUS_CODE_LEN + 1, sizeof(char));
    memcpy(statusCode, posStatusCodeStart, STATUS_CODE_LEN);

    int statusCodeInt = atoi(statusCode);
    return statusCodeInt;
}