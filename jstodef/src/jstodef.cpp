#define EXTENSION_NAME jstodef
#define LIB_NAME "jstodef"
#define MODULE_NAME "jstodef"
#ifndef DLIB_LOG_DOMAIN
#define DLIB_LOG_DOMAIN LIB_NAME
#endif
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_HTML5)

typedef void (*ObjectMessage)(const char* message_id, const char* message, const int length);
typedef void (*NoMessage)(const char* message_id);
typedef void (*NumberMessage)(const char* message_id, float message);
typedef void (*BooleanMessage)(const char* message_id, int message);

extern "C" {
    void JsToDef_RegisterCallbacks(ObjectMessage cb_obj, ObjectMessage cb_string, NoMessage cb_empty, NumberMessage cb_num, BooleanMessage cb_bool);
    void JsToDef_RemoveCallbacks();
}

struct JsToDefListener {
    lua_State* m_L;
    dmScript::LuaCallbackInfo* m_CallbackInfo;
    int m_FunctionRef;
    int m_SelfRef;
};

static int GetEqualIndexOfListener(lua_State* L, JsToDefListener* cbk);
static void DestroyListener(uint32_t index);

static dmArray<JsToDefListener> m_listeners;

static void ReleaseListener(JsToDefListener* cbk)
{
    if(cbk->m_CallbackInfo != 0x0)
    {
        dmScript::DestroyCallback(cbk->m_CallbackInfo);
        cbk->m_CallbackInfo = 0x0;
    }

    if(cbk->m_FunctionRef != LUA_NOREF)
    {
        dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_FunctionRef);
        cbk->m_FunctionRef = LUA_NOREF;
    }

    if(cbk->m_SelfRef != LUA_NOREF)
    {
        dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_SelfRef);
        cbk->m_SelfRef = LUA_NOREF;
    }
}

static void DestroyListener(uint32_t index)
{
    ReleaseListener(&m_listeners[index]);
    m_listeners.EraseSwap(index);

    if(m_listeners.Empty())
    {
        JsToDef_RemoveCallbacks();
    }
}

static dmScript::LuaCallbackInfo* GetValidCallback(uint32_t index)
{
    dmScript::LuaCallbackInfo* callback = m_listeners[index].m_CallbackInfo;
    if(!dmScript::IsCallbackValid(callback))
    {
        dmLogError("Could not run JsToDef callback because the instance has been deleted.");
        DestroyListener(index);
        return 0x0;
    }
    return callback;
}

static void JsToDef_SendObjectMessage(const char* message_id, const char* message, const int length)
{
    for(int i = (int)m_listeners.Size() - 1; i >= 0; --i)
    {
        if((uint32_t)i >= m_listeners.Size())
        {
            continue;
        }

        dmScript::LuaCallbackInfo* callback = GetValidCallback(i);
        if(callback == 0x0)
        {
            continue;
        }

        lua_State* L = dmScript::GetCallbackLuaContext(callback);
        int top = lua_gettop(L);

        if(dmScript::SetupCallback(callback))
        {
            lua_pushstring(L, message_id);
            dmScript::JsonToLua(L, message, length); // throws lua error if it fails

            int ret = dmScript::PCall(L, 3, 0);
            (void)ret;
            dmScript::TeardownCallback(callback);
        }
        assert(top == lua_gettop(L));
    }
}

static void JsToDef_SendStringMessage(const char* message_id, const char* message, const int length)
{
    for(int i = (int)m_listeners.Size() - 1; i >= 0; --i)
    {
        if((uint32_t)i >= m_listeners.Size())
        {
            continue;
        }

        dmScript::LuaCallbackInfo* callback = GetValidCallback(i);
        if(callback == 0x0)
        {
            continue;
        }

        lua_State* L = dmScript::GetCallbackLuaContext(callback);
        int top = lua_gettop(L);

        if(dmScript::SetupCallback(callback))
        {
            lua_pushstring(L, message_id);
            lua_pushlstring(L, message, length);

            int ret = dmScript::PCall(L, 3, 0);
            (void)ret;
            dmScript::TeardownCallback(callback);
        }
        assert(top == lua_gettop(L));
    }
}

static void JsToDef_SendEmptyMessage(const char* message_id)
{
    for(int i = (int)m_listeners.Size() - 1; i >= 0; --i)
    {
        if((uint32_t)i >= m_listeners.Size())
        {
            continue;
        }

        dmScript::LuaCallbackInfo* callback = GetValidCallback(i);
        if(callback == 0x0)
        {
            continue;
        }

        lua_State* L = dmScript::GetCallbackLuaContext(callback);
        int top = lua_gettop(L);

        if(dmScript::SetupCallback(callback))
        {
            lua_pushstring(L, message_id);

            int ret = dmScript::PCall(L, 2, 0);
            (void)ret;
            dmScript::TeardownCallback(callback);
        }
        assert(top == lua_gettop(L));
    }
}

static void JsToDef_SendNumMessage(const char* message_id, float message)
{
    for(int i = (int)m_listeners.Size() - 1; i >= 0; --i)
    {
        if((uint32_t)i >= m_listeners.Size())
        {
            continue;
        }

        dmScript::LuaCallbackInfo* callback = GetValidCallback(i);
        if(callback == 0x0)
        {
            continue;
        }

        lua_State* L = dmScript::GetCallbackLuaContext(callback);
        int top = lua_gettop(L);

        if(dmScript::SetupCallback(callback))
        {
            lua_pushstring(L, message_id);
            lua_pushnumber(L, message);

            int ret = dmScript::PCall(L, 3, 0);
            (void)ret;
            dmScript::TeardownCallback(callback);
        }
        assert(top == lua_gettop(L));
    }
}

static void JsToDef_SendBoolMessage(const char* message_id, int message)
{
    for(int i = (int)m_listeners.Size() - 1; i >= 0; --i)
    {
        if((uint32_t)i >= m_listeners.Size())
        {
            continue;
        }

        dmScript::LuaCallbackInfo* callback = GetValidCallback(i);
        if(callback == 0x0)
        {
            continue;
        }

        lua_State* L = dmScript::GetCallbackLuaContext(callback);
        int top = lua_gettop(L);

        if(dmScript::SetupCallback(callback))
        {
            lua_pushstring(L, message_id);
            lua_pushboolean(L, message);

            int ret = dmScript::PCall(L, 3, 0);
            (void)ret;
            dmScript::TeardownCallback(callback);
        }
        assert(top == lua_gettop(L));
    }
}

static int GetEqualIndexOfListener(lua_State* L, JsToDefListener* cbk)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_FunctionRef);
    int first = lua_gettop(L);
    int second = first + 1;
    for(uint32_t i = 0; i != m_listeners.Size(); ++i)
    {
        JsToDefListener* cb = &m_listeners[i];
        if(cb->m_L != cbk->m_L)
        {
            continue;
        }

        lua_rawgeti(L, LUA_REGISTRYINDEX, cb->m_FunctionRef);
        if(lua_equal(L, first, second))
        {
            lua_pop(L, 1);
            lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_SelfRef);
            lua_rawgeti(L, LUA_REGISTRYINDEX, cb->m_SelfRef);
            if(lua_equal(L, second, second + 1))
            {
                lua_pop(L, 3);
                return i;
            }
            lua_pop(L, 2);
        }
        else
        {
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    return -1;
}

static int AddListener(lua_State* L)
{
    JsToDefListener cbk = {dmScript::GetMainThread(L), 0x0, LUA_NOREF, LUA_NOREF};

    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_pushvalue(L, 1);
    cbk.m_FunctionRef = dmScript::Ref(L, LUA_REGISTRYINDEX);

    dmScript::GetInstance(L);
    cbk.m_SelfRef = dmScript::Ref(L, LUA_REGISTRYINDEX);

    int index = GetEqualIndexOfListener(L, &cbk);
    if(index >= 0)
    {
        ReleaseListener(&cbk);
        dmLogError("Can't register a callback again. Callback has been registered before.");
        return 0;
    }

    cbk.m_CallbackInfo = dmScript::CreateCallback(L, 1);
    if(cbk.m_CallbackInfo == 0x0)
    {
        ReleaseListener(&cbk);
        return luaL_error(L, "Failed to create JsToDef callback.");
    }

    if(m_listeners.Full())
    {
        m_listeners.OffsetCapacity(1);
    }
    m_listeners.Push(cbk);

    if(m_listeners.Size() == 1)
    {
        JsToDef_RegisterCallbacks((ObjectMessage)JsToDef_SendObjectMessage, (ObjectMessage)JsToDef_SendStringMessage,
            (NoMessage)JsToDef_SendEmptyMessage, (NumberMessage)JsToDef_SendNumMessage, (BooleanMessage)JsToDef_SendBoolMessage);
    }
    return 0;
}

static int RemoveListener(lua_State* L)
{
    JsToDefListener cbk = {dmScript::GetMainThread(L), 0x0, LUA_NOREF, LUA_NOREF};

    luaL_checktype(L, 1, LUA_TFUNCTION);
    lua_pushvalue(L, 1);
    cbk.m_FunctionRef = dmScript::Ref(L, LUA_REGISTRYINDEX);

    dmScript::GetInstance(L);
    cbk.m_SelfRef = dmScript::Ref(L, LUA_REGISTRYINDEX);

    int index = GetEqualIndexOfListener(L, &cbk);
    ReleaseListener(&cbk);

    if(index >= 0)
    {
        DestroyListener(index);
    }
    else
    {
        dmLogError("Can't remove a callback that wasn't registered.");
    }
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
    (void)params;
    while(!m_listeners.Empty())
    {
        DestroyListener(m_listeners.Size() - 1);
    }
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
