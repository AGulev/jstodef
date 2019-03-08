#define EXTENSION_NAME jstodef
#define LIB_NAME "jstodef"
#define MODULE_NAME "jstodef"

#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>
#include "jstodef.h"

#if defined(DM_PLATFORM_HTML5)

lua_State* _L;

dmArray<JsToDefListener> m_listeners;

bool check_callback_and_instance(JsToDefListener* cbk)
{
    if(cbk->m_Callback == LUA_NOREF)
    {
        dmLogInfo("JsToDef callback do not exist.");
        return false;
    }
    lua_State* L = cbk->m_L;
    int top = lua_gettop(L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Callback);
    //[-1] - callback
    lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Self);
    //[-1] - self
    //[-2] - callback
    lua_pushvalue(L, -1);
    //[-1] - self
    //[-2] - self
    //[-3] - callback
    dmScript::SetInstance(L);
    //[-1] - self
    //[-2] - callback
    
    if (!dmScript::IsInstanceValid(L)) {
        UnregisterCallback(L, cbk);
        dmLogError("Could not run JsToDef callback because the instance has been deleted.");
        lua_pop(L, 2);
        assert(top == lua_gettop(L));
        return false;
    }
    return true;
}

static void JsToDef_SendObjectMessage(const char* message_id, const char* message)
{
    for(int i = m_listeners.Size() - 1; i >= 0; --i)
    {
        JsToDefListener* cbk = &m_listeners[i];
        lua_State* L = cbk->m_L;
        int top = lua_gettop(L);
        if (check_callback_and_instance(cbk)) {
            //[-1] - self
            //[-2] - callback
            lua_pushstring(L, message_id);
            //[-1] - message_id
            //[-2] - self
            //[-3] - callback

            lua_getglobal(L, "json");
            //[-1] - global json table
            //[-2] - message_id
            //[-3] - self
            //[-4] - callback
            lua_getfield(L, -1, "decode");
            //[-1] - decode function
            //[-2] - global json table
            //[-3] - message_id
            //[-4] - self
            //[-5] - callback
            lua_pushfstring(L, message);
            //[-1] - message string
            //[-2] - decode function
            //[-3] - global json table
            //[-4] - message_id
            //[-5] - self
            //[-6] - callback

            if (lua_pcall(L, 1, 1, 0) != 0){
                //[-1] - error
                //[-2] - global json table
                //[-3] - message_id
                //[-4] - self
                //[-5] - callback
                dmLogError("error running function `json.decode': %s", lua_tostring(L, -1));
                lua_pop(L, 5);
                assert(top == lua_gettop(L));
                return;
            }
            //[-1] - result lua  table
            //[-2] - global json table
            //[-3] - message_id
            //[-4] - self
            //[-5] - callback
            lua_remove(L, -2);
            //[-1] - result lua  table
            //[-2] - message_id
            //[-3] - self
            //[-4] - callback
            int ret = lua_pcall(L, 3, 0, 0);
            if(ret != 0) {
                dmLogError("Error running callback: %s", lua_tostring(L, -1));
                lua_pop(L, 1);
            }
        }
        assert(top == lua_gettop(L));
    }
}

static void JsToDef_SendStringMessage(const char* message_id, const char* message)
{
    for(int i = m_listeners.Size() - 1; i >= 0; --i)
    {
        if(i > m_listeners.Size()){
          return;
        }
        JsToDefListener* cbk = &m_listeners[i];
        lua_State* L = cbk->m_L;
        int top = lua_gettop(L);
        if (check_callback_and_instance(cbk)) {
            lua_pushstring(L, message_id);
            lua_pushstring(L, message);
            
            int ret = lua_pcall(L, 3, 0, 0);
            if(ret != 0) {
                dmLogError("Error running callback: %s", lua_tostring(L, -1));
                lua_pop(L, 1);
            }
        }
        assert(top == lua_gettop(L));
    }
}

static void JsToDef_SendEmptyMessage(const char* message_id)
{
    for(int i = m_listeners.Size() - 1; i >= 0; --i)
    {
        if(i > m_listeners.Size()){
          return;
        }
        JsToDefListener* cbk = &m_listeners[i];
        lua_State* L = cbk->m_L;
        int top = lua_gettop(L);
        if (check_callback_and_instance(cbk)) {
            lua_pushstring(L, message_id);
            
            int ret = lua_pcall(L, 2, 0, 0);
            if(ret != 0) {
                dmLogError("Error running callback: %s", lua_tostring(L, -1));
                lua_pop(L, 1);
            }
        }
        assert(top == lua_gettop(L));
    }
}

static void JsToDef_SendNumMessage(const char* message_id, float message)
{
    for(int i = m_listeners.Size() - 1; i >= 0; --i)
    {
        if(i > m_listeners.Size()){
          return;
        }
        JsToDefListener* cbk = &m_listeners[i];
        lua_State* L = cbk->m_L;
        int top = lua_gettop(L);
        if (check_callback_and_instance(cbk)) {
            lua_pushstring(L, message_id);
            lua_pushnumber(L, message);
            
            int ret = lua_pcall(L, 3, 0, 0);
            if(ret != 0) {
                dmLogError("Error running callback: %s", lua_tostring(L, -1));
                lua_pop(L, 1);
            }
        }
        assert(top == lua_gettop(L));
    }
}

static void JsToDef_SendBoolMessage(const char* message_id, int message)
{
    for(int i = m_listeners.Size() - 1; i >= 0; --i)
    {
        JsToDefListener* cbk = &m_listeners[i];
        lua_State* L = cbk->m_L;
        int top = lua_gettop(L);
        if (check_callback_and_instance(cbk)) {
            lua_pushstring(L, message_id);
            lua_pushboolean(L, message);
            
            int ret = lua_pcall(L, 3, 0, 0);
            if(ret != 0) {
                dmLogError("Error running callback: %s", lua_tostring(L, -1));
                lua_pop(L, 1);
            }
        }
        assert(top == lua_gettop(L));
    }
}

int GetEqualIndexOfListener(lua_State* L, JsToDefListener* cbk)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Callback);
    int first = lua_gettop(L);
    int second = first + 1;
    for(uint32_t i = 0; i != m_listeners.Size(); ++i)
    {
        JsToDefListener* cb = &m_listeners[i];
        lua_rawgeti(L, LUA_REGISTRYINDEX, cb->m_Callback);
        if (lua_equal(L, first, second)){
            lua_pop(L, 1);
            lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Self);
            lua_rawgeti(L, LUA_REGISTRYINDEX, cb->m_Self);
            if (lua_equal(L, second, second + 1)){
              lua_pop(L, 3);
              return i;
            }
            lua_pop(L, 2);
        } else {
            lua_pop(L, 1);
        }
      }
      lua_pop(L, 1);
      return -1;
}

void UnregisterCallback(lua_State* L, JsToDefListener* cbk)
{
    int index = GetEqualIndexOfListener(L, cbk);
    if (index >= 0){
      if(cbk->m_Callback != LUA_NOREF)
      {
          dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Callback);
          dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Self);
          cbk->m_Callback = LUA_NOREF;
      }
      m_listeners.EraseSwap(index);
      if (m_listeners.Size() == 0) {
          JsToDef_RemoveCallbacks();
        }
    } else {
      dmLogError("Can't remove a callback that didn't not register.");
    }
}

static int AddListener(lua_State* L)
{
    JsToDefListener cbk;
    cbk.m_L = dmScript::GetMainThread(L);

    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_pushvalue(L, 1);
    cbk.m_Callback = dmScript::Ref(L, LUA_REGISTRYINDEX);

    dmScript::GetInstance(L);
    cbk.m_Self = dmScript::Ref(L, LUA_REGISTRYINDEX);

    if(cbk.m_Callback != LUA_NOREF)
    {
      int index = GetEqualIndexOfListener(L, &cbk);
        if (index < 0){
            if(m_listeners.Full())
            {
                m_listeners.OffsetCapacity(1);
            }
            m_listeners.Push(cbk);
      } else {
        dmLogError("Can't register a callback again. Callback has been registered before.");
      }
      if (m_listeners.Size() == 1){
          JsToDef_RegisterCallbacks((ObjectMessage)JsToDef_SendObjectMessage, (ObjectMessage)JsToDef_SendStringMessage, 
              (NoMessage)JsToDef_SendEmptyMessage,(NumberMessage)JsToDef_SendNumMessage, (BooleanMessage)JsToDef_SendBoolMessage);
      }
    }
    return 0;
}

static int RemoveListener(lua_State* L)
{
    JsToDefListener cbk;
    cbk.m_L = dmScript::GetMainThread(L);

    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_pushvalue(L, 1);

    cbk.m_Callback = dmScript::Ref(L, LUA_REGISTRYINDEX);

    dmScript::GetInstance(L);
    cbk.m_Self = dmScript::Ref(L, LUA_REGISTRYINDEX);

    UnregisterCallback(L, &cbk);
    return 0;
}

static const luaL_reg Module_methods[] =
{
        {"add_listener", AddListener},
        {"remove_listener", RemoveListener},
        {0, 0}
};

static void LuaInit(lua_State* L)
{
        _L = L;
        int top = lua_gettop(L);
        luaL_register(L, MODULE_NAME, Module_methods);
        lua_pop(L, 1);
        assert(top == lua_gettop(L));
}

dmExtension::Result InitializeJsToDef(dmExtension::Params* params)
{
        LuaInit(params->m_L);
        return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeJsToDef(dmExtension::Params* params)
{
        return dmExtension::RESULT_OK;
}

#else // unsupported platforms


dmExtension::Result InitializeJsToDef(dmExtension::Params* params)
{
        return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeJsToDef(dmExtension::Params* params)
{
        return dmExtension::RESULT_OK;
}

#endif


DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializeJsToDef, 0, 0, FinalizeJsToDef)
