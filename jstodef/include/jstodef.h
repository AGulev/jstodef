#pragma once

#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_HTML5)

typedef void (*ObjectMessage)(const char* message_id, const char* message);
typedef void (*NoMessage)(const char* message_id);
typedef void (*NumberMessage)(const char* message_id, float message);
typedef void (*BooleanMessage)(const char* message_id, int message);

extern "C" {
    void JsToDef_RegisterCallbacks(ObjectMessage cb_obj, ObjectMessage cb_string, NoMessage cb_empty, NumberMessage cb_num, BooleanMessage cb_bool);
    void JsToDef_RemoveCallbacks();
}

struct JsToDefListener {
  JsToDefListener() : m_L(0), m_Callback(LUA_NOREF), m_Self(LUA_NOREF) {}
  lua_State* m_L;
  int m_Callback;
  int m_Self;
};

void UnregisterCallback(lua_State* L, JsToDefListener* cbk);
int GetEqualIndexOfListener(lua_State* L, JsToDefListener* cbk);

#endif